/*
name:communicate.c
author:salan
date:20170322
ver:1.0
description:
    communicate funs
*/

/*****************************include files***********************/
#include <string.h>
#include <stdio.h>
#include "helios.h"
#include "protocol_def.h"
#include "crc.h"
#include "para_table.h"
#include "porting.h"
#include "data_task.h"
#include "minilzo.h"
#include "pwd.h"
#include "log.h"

/*****************************para define*************************/

/*****************************data struct define******************/
static s8 lzo_in_buf[FRAME_BUF_SIZE];
static s8 lzo_out_buf[FRAME_BUF_SIZE];
#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);
/*****************************functions***************************/
/*****************************send deal***************************/
static s8 header_pack (const para_stream *ps, frame *pf)
{
    if (NULL == ps)
        return -1;
    if (NULL == pf)
        return -1;

    memcpy((pf->header.start), FRAME_START, sizeof(FRAME_START));
    if (FRAME_SOURCE_SELF == (ps->source))
        pf->header.remote_code = 0;
    else
        pf->header.remote_code = ps->remote_code;
    pf->header.crc = crc((const s8*)(pf->header.data), ((ps->paralen) + FRAME_CMD_LEN));
    pf->header.device_id = unit_para_t.unit_sundry.device_id;

    pf->header.len = 5 + (ps->paralen) + FRAME_CMD_LEN;

    return 0;
}
static s8 header_repack (const para_stream *ps, frame *pf)
{
    if (NULL == pf)
        return -1;
    if (NULL == ps)
        return -1;

    pf->header.device_id = unit_para_t.unit_sundry.device_id;

    return 0;
}

static s8 cmd_pack(const para_stream *ps, frame *pf)
{
    frame_cmd *p;

    if (NULL == ps)
        return -1;
    if (NULL == pf)
        return -1;

    p = &(pf->cmd);
    p->total = 1;
    p->index = 1;
    p->ver = PROTOCOL_VER;
    p->cmd = ps->cmd;
    memcpy((void*) & (p->md_adr), (void*) & (ps->md_adr), sizeof(md_adr_info));
    p->authority = ps->authority;
    p->ack = ps->ack;

    if (FRAME_SOURCE_SELF == (ps->source)) {
        p->authority = 0;
        p->ack = 0;
    }

    return 0;
}

static s8 para_pack(const para_stream *ps, frame *pf)
{
    if (NULL == ps)
        return -1;

    if ((ps->paralen) > PARA_BUF_SIZE) {
        return -1;
        /*
        memcpy((void*)(frame_send.cmd.data), (void*)(para->next), PARA_BUF_SIZE);
        para->len -= PARA_BUF_SIZE;
        para->index++;
        para->next += PARA_BUF_SIZE;
        */
    } else {
        memcpy((void*)(pf->cmd.data), (void*)(ps->data), (ps->paralen));
        //memcpy((void*)((pf->cmd.data) + (ps->paralen)), FRAME_END, sizeof(FRAME_END));
    }

    return 0;
}
static s16 one_frame_pack(const para_stream *ps, frame *pf)
{
    s32 err;
    s32 len;

    if (NULL == pf)
        return -1;
    if (NULL == ps)
        return -1;

    len = 0;

    para_pack(ps, pf);
    cmd_pack(ps, pf);
    header_pack(ps, pf);

    #ifdef PROTOCOL_ENCRYPT_LZO
    //compress
    memset(lzo_in_buf, 0, sizeof(lzo_in_buf));
    memset(lzo_out_buf, 0, sizeof(lzo_out_buf));

    memcpy(lzo_in_buf, (pf->arr + FRAME_HEADER_LEN), ((pf->header.len) - 5));
    if (LZO_E_OK != lzo1x_1_compress((lzo_bytep)lzo_in_buf, (lzo_uint)((pf->header.len) - 5), (lzo_bytep)(lzo_out_buf), (lzo_uintp)&len, wrkmem)) {
        RLDEBUG("one_frame_pack:lzo1x_1_compress() false\r\n");
        return -1;
    }

    //pwd
    EncryptCrackExt(lzo_out_buf, len);

    //pack start ,end
    memset((pf->arr + FRAME_HEADER_LEN), 0, (len + FRAME_END_LEN));
    pf->header.len = len + 5;
    memcpy((pf->arr + FRAME_HEADER_LEN), lzo_out_buf, len);
    memcpy((pf->arr + FRAME_HEADER_LEN + len), FRAME_END, sizeof(FRAME_END));
    #endif
    
    return ((pf->header.len) + FRAME_START_LEN + 2 + FRAME_END_LEN );

    //return (ps->paralen + FRAME_HEADER_LEN + FRAME_CMD_LEN + FRAME_END_LEN);
}
/*
para:
    src:para data
    srclen: para data len
    srcframe:recv's frame,maybe null
    dest:send buf
    destlen;the data shoud be send
return:
    0:sus
    <0:err
    >0:frame len
 */
s16 frame_pack(const para_stream *para, frame *pf)
{
    s32 cnt;

    if (NULL == para)
        return -1;
    if (NULL == pf) {
        return -1;
    }

    /*
        if ((para->len) > 1024) {
            if ((para->len) % PARA_BUF_SIZE) {
                para->total = ((para->len) / PARA_BUF_SIZE) + 1;
            } else {
                para->total = ((para->len) / PARA_BUF_SIZE);
            }
            para->index = 0;
        } else {
            para->total = 1;
            para->index = 0;
        }
    */
    if ((para->paralen) > 1024)
        return -1;

    //para->next = (s8*)(para->data);
    cnt = one_frame_pack(para, pf);
    if (cnt < 0)
        return -1;

    /**/
    return cnt;
}
/*****************************recv deal***************************/
/*
para:
    pstr:data recv
    len: data len
return:
    0:sus
    <0:err
 */
static s8 frame_shell_check(const s8 *str, u16 len, s8 **begin)
{
    s8 *start, *end;
    frame *pframe;

    if (NULL == str) {
        return -1;
    }

    len = len;

    start = (s8*)strstr((const char*)str, (const char*)FRAME_START);
    if (NULL != start) {
        pframe = (frame*)start;

        /* frame shell end check */
        end = start + (pframe->header.len + FRAME_START_LEN + 2);
        if (strncmp((const char*)end, FRAME_END, (sizeof(FRAME_END) - 1))) {
            RLDEBUG("frame_shell_check:cant find he1.0\r\n");
            RLDEBUG("frame_shell_check:str len:%d\r\n", (len));
            RLDEBUG("frame_shell_check:total len:%d\r\n", (pframe->header.len));
            RLDEBUG("frame_shell_check:head len:%d\r\n", (FRAME_START_LEN));
            return -1;
        }
    } else {
        RLDEBUG("frame_shell_check:cant find hs1.0\r\n");
        return -1;
    }
    *begin = start;

    return 0;
}

/*
para:
    pstr:data recv
    len: data len
return:
    0:sus
    <0:err
 */
static s8 frame_protocol_check(const s8 *str, u16 len, s8 **begin)
{
    s8 *start, *end;
    frame *pframe;
    u16 newcrc;

    if (NULL == str) {
        return -1;
    }

    len = len;

    start = (s8*)strstr((const char*)str, (const char*)FRAME_START);
    if (NULL != start) {
        pframe = (frame*)start;

        /* frame end check */
        end = start + (pframe->header.len + FRAME_START_LEN + 2);
        if (strncmp((const char*)end, FRAME_END, sizeof(FRAME_END))) {
            RLDEBUG("cant find he1.0\r\n");
            RLDEBUG("total len:%d\r\n", (pframe->header.len));
            RLDEBUG("head len:%d\r\n", (FRAME_START_LEN));
            return -1;
        }

        /* crc check */
        newcrc = crc((const s8*)(pframe->header.data), (pframe->header.len - 1 - 2 - 2));
        if (newcrc != (pframe->header.crc)) {
            RLDEBUG("crc err\r\n");
            RLDEBUG("crc recv:%d\r\n", (pframe->header.crc));
            RLDEBUG("crc local:%d\r\n", newcrc);
            return -1;
        }
    } else {
        RLDEBUG("cant find hs1.0\r\n");
        return -1;
    }
    *begin = start;

    return 0;
}

static s8 frame_md_adr_check(frame *pf)
{
    u8 cnt;
    s8 flag = -1;

    if (NULL == pf)
        return -1;
#if MONITOR_MODULE_ENABLE
    if (((pf->cmd.md_adr.mod_type) == (unit_para_t.md_adr_t.mod_type)) && ((pf->cmd.md_adr.mod_band) == (unit_para_t.md_adr_t.mod_band)) && ((pf->cmd.md_adr.mod_adr_t.dat) == (unit_para_t.md_adr_t.mod_adr_t.dat)))
        flag = 0;
#endif
#if OTHER_MODULE_ENABLE
    for (cnt = 0; cnt < MOD_NUM_IN_ONE_PCB; cnt++) {
        if (((pf->cmd.md_adr.mod_type) == (band_para_a[cnt].md_adr_t.mod_type)) && ((pf->cmd.md_adr.mod_band) == (band_para_a[cnt].md_adr_t.mod_band)) && ((pf->cmd.md_adr.mod_adr_t.dat) == (band_para_a[cnt].md_adr_t.mod_adr_t.dat)))
            break;
    }

    if (cnt < MOD_NUM_IN_ONE_PCB) {
        flag = 0;
    }
#endif

    return flag;
}
static s16 frame_get_para_len(const frame *pf)
{
    if (NULL == pf)
        return -1;

    return ((pf->header.len) - ((FRAME_HEADER_LEN - FRAME_START_LEN - 2) + FRAME_CMD_LEN));
}
static s16 frame_get_para_num(const frame *pf)
{
    s16 len, cnt, nums;
    para *_para;

    if (NULL == pf)
        return -1;

    len = ((pf->header.len) - (FRAME_HEADER_LEN - FRAME_START_LEN - 2) - FRAME_CMD_LEN);
    cnt = 0;
    nums = 0;
    while (cnt < len) {
        _para = (para*)((pf->cmd.data) + cnt);
        nums++;
        cnt += ((_para->para_len) + sizeof(para));
    }

    return nums;
}

static s8 read_para_deal(const frame *pf, para_stream *ps)
{
    u16 cnt;
    u16 para_nums;
    u16 adr;

    if (NULL == pf)
        return -1;
    if (NULL == ps)
        return -1;

    //RLDEBUG("read_para_deal\r\n");

    para_nums = frame_get_para_len(pf);
    para_nums >>= 1;

    //RLDEBUG("read_para_deal para numbers:%d\r\n", para_nums);

    for (cnt = 0; cnt < para_nums; cnt++) {
        memcpy((void*)&adr, (pf->cmd.data + cnt * 2), sizeof(u16));
        //RLDEBUG("read_para_deal para adr:%d\r\n", adr);
        one_para_adr_read_processing(adr, ps);
    }
    return 0;
}
#pragma pack(1)
struct read_md_info_s {
    u16 total_len;
    u8 one_adr_len;
//	md_adr_info *adr;
};
#pragma pack()
static s8 read_md_info_deal(para_stream *ps)
{
    u16 cnt;
    struct read_md_info_s *read_md;

    if (NULL == ps)
        return -1;

    read_md = (struct read_md_info_s*)(ps->data);
    read_md->one_adr_len = sizeof(md_adr_info);

#if MONITOR_MODULE_ENABLE
    read_md->total_len = (MOD_NUM_IN_ONE_PCB + 1) * (read_md->one_adr_len);
#else
    read_md->total_len = (MOD_NUM_IN_ONE_PCB) * (read_md->one_adr_len);
#endif

    ps->next += sizeof(struct read_md_info_s);

#if MONITOR_MODULE_ENABLE
    memcpy((void*)(ps->next), (void*) & (unit_para_t.md_adr_t), sizeof(md_adr_info));
    ps->next += sizeof(md_adr_info);
#endif

#if OTHER_MODULE_ENABLE
    for (cnt = 0; cnt < MOD_NUM_IN_ONE_PCB; cnt++) {
        memcpy((void*)(ps->next), (void*) & (band_para_a[cnt].md_adr_t), sizeof(md_adr_info));
        ps->next += sizeof(md_adr_info);
    }
#endif
    ps->paralen = read_md->total_len + sizeof(struct read_md_info_s);
    //RLDEBUG("read_md_info_deal:exit!,mod adr len is:%d\r\n", ps->paralen);
    return 0;
}


static s16 set_para_deal(const frame *pf, para_stream *ps)
{
    u16 cnt;
    u16 para_lens, para_nums;
    para *_para;
    s32 err;

    if (NULL == pf)
        return -1;
    if (NULL == ps)
        return -1;

    //RLDEBUG("set_para_deal processing\r\n");

    para_lens = frame_get_para_len(pf);
    RLDEBUG("set_para_deal para len is:%d\r\n", para_lens);
    para_nums = frame_get_para_num(pf);
    RLDEBUG("set_para_deal para numbers is:%d\r\n", para_nums);

    _para = (para*)(pf->cmd.data);
    para_lens = 0;
    for (cnt = 0; cnt < para_nums; cnt++) {
        RLDEBUG("set_para_deal para adr is:%x\r\n", (_para->para_adr));
        err = one_para_adr_set_processing((s8*)(_para), ps);
        if (err < 0) {
            RLDEBUG("set_para_deal:one_para_adr_set_processing() false \r\n");
        }
        para_lens += sizeof(para) + (_para->para_len);
        _para = (para*)((pf->cmd.data) + para_lens);
    }

    return 0;
}

#if MONITOR_MODULE_ENABLE
s32 heartbeat(para_stream *ps, frame *pf)
{
    if ((NULL == pf) || (NULL == ps))
        return -1;

    ps->ack = 0;
    ps->authority = 1;
    ps->cmd = 0x11;
    ps->device_id = unit_para_t.unit_sundry.device_id;
    ps->index = 0;
    ps->md_adr.mod_type = unit_para_t.md_adr_t.mod_type;
    ps->md_adr.mod_band = unit_para_t.md_adr_t.mod_band;
    ps->md_adr.mod_adr_t.dat = unit_para_t.md_adr_t.mod_adr_t.dat;
    ps->next = (s8*)(ps->data);
    ps->remote_code = 0x00;
    ps->source = FRAME_SOURCE_SELF;
    ps->total = 0;
    ps->paralen = 0;
    //copy heartbeat dat
    //sn
    memcpy((ps->data), (unit_para_t.md_mfrs.sn), sizeof((unit_para_t.md_mfrs.sn)));
    ps->next += sizeof(unit_para_t.md_mfrs.sn);
    ps->paralen += sizeof(unit_para_t.md_mfrs.sn);
    //gps info
    memcpy((ps->next), &(unit_dynamic_para_t.gps_t.gps_status), sizeof(unit_dynamic_para_t.gps_t.gps_status));
    ps->next += sizeof(unit_dynamic_para_t.gps_t.gps_status);
    ps->paralen += sizeof(unit_dynamic_para_t.gps_t.gps_status);
    memcpy((ps->next), &(unit_dynamic_para_t.gps_t.gps_cur.Longitude), sizeof(unit_dynamic_para_t.gps_t.gps_cur.Longitude));
    ps->next += sizeof(unit_dynamic_para_t.gps_t.gps_cur.Longitude);
    ps->paralen += sizeof(unit_dynamic_para_t.gps_t.gps_cur.Longitude);
    memcpy((ps->next), &(unit_dynamic_para_t.gps_t.gps_cur.Latitude), sizeof(unit_dynamic_para_t.gps_t.gps_cur.Latitude));
    ps->next += sizeof(unit_dynamic_para_t.gps_t.gps_cur.Latitude);
    ps->paralen += sizeof(unit_dynamic_para_t.gps_t.gps_cur.Latitude);
    memcpy((ps->next), &(unit_dynamic_para_t.gps_t.gps_cur.altitude), sizeof(unit_dynamic_para_t.gps_t.gps_cur.altitude));
    ps->next += sizeof(unit_dynamic_para_t.gps_t.gps_cur.altitude);
    ps->paralen += sizeof(unit_dynamic_para_t.gps_t.gps_cur.altitude);

    return frame_pack(ps, pf);
}
#endif

s8 frame_recv_deal(s8 *str, u16 len, FRAME_SOURCE source, para_stream *ps)
{
    u16 cnt;
    s8 *begin = NULL;
    frame *pf = NULL;
    u32 frame_len;

    if ((NULL == str) || (NULL == ps))
        return -1;

    if (len < (FRAME_HEADER_LEN + FRAME_CMD_LEN))
    {
        RLDEBUG("frame len err\r\n");
        goto FRAME_ERR;
    }

    frame_len = 0;

    #ifdef PROTOCOL_ENCRYPT_LZO
    /*uncompress and pwd*/
    if (frame_shell_check(str,  len, &begin)) {
        RLDEBUG("frame_recv_deal:frame_shell_check() false \r\n");
        for (cnt = 0; cnt < len; cnt++) {	//@luke20170821
            printf("%02x", (u8)(*(str + cnt)));
        }
        printf("\r\n");
        timedelay(0, 1, 0, 0);
        return -1;
    }
    pf = (frame*)(begin);
    EncryptCrackExt((begin + FRAME_HEADER_LEN), (pf->header.len - 5));
    /*
        RLDEBUG("frame_recv_deal:after deencrypt: recv dat len=%d\r\n", (pf->header.len + 2 + 5 + 5));
        for (cnt = 0; cnt < (pf->header.len + 2 + 5 + 5); cnt++) {
            printf(" %02x", (u8)begin[cnt]);
        }
        RLDEBUG("\r\n");
    */

    memset(lzo_out_buf, 0, sizeof(lzo_out_buf));
    if (LZO_E_OK != lzo1x_decompress((lzo_bytep)(begin + FRAME_HEADER_LEN), (lzo_uint)(pf->header.len - 5), (lzo_bytep)lzo_out_buf, (lzo_uintp)&frame_len, wrkmem)) {
        RLDEBUG("frame_recv_deal:lzo1x_decompress_safe() false \r\n");
    }
    /*
        RLDEBUG("frame_recv_deal:after lzo decompress: recv dat len=%d\r\n", frame_len);
        for (cnt = 0; cnt < (frame_len); cnt++) {
            printf(" %02x", (u8)lzo_buf[cnt]);
        }
        RLDEBUG("\r\n");
    */
    memset((begin + FRAME_HEADER_LEN), 0, len - (begin - str));
    pf = (frame*)begin;
    pf->header.len = frame_len + 5;
    memcpy((begin + FRAME_HEADER_LEN), lzo_out_buf, frame_len);
    memcpy((begin + FRAME_START_LEN + 2 + (pf->header.len)), FRAME_END, sizeof(FRAME_END));

    #endif
    /*
    RLDEBUG("frame_recv_deal:after lzo decompress: recv dat len=%d\r\n", (pf->header.len + 2 + 5 + 5));
    for (cnt = 0; cnt < (pf->header.len + 2 + 5 + 5); cnt++) {
        printf(" %02x", (u8)begin[cnt]);
    }
    RLDEBUG("\r\n");
    */

    //frame decode begin
    if (frame_protocol_check(str,  len, &begin)) {
        RLDEBUG("frame protocol err\r\n");
        goto FRAME_ERR;
    }
    pf = (frame*)(begin);

    /*远控读取模块地址信息的指令，不做地址判断*/
    if (FRAME_CMD_READ_MOD_INFO != (pf->cmd.cmd))
        if (frame_md_adr_check(pf)) {
            RLDEBUG("frame module adr err\r\n");
            goto FRAME_ERR;
        }
    /*
        cnt = 0;
        while (PARA_STREAM_BUSY == (para_stream_t.flag)) {
            timedelay(0, 0, 50, 0);
            cnt++;
            if (cnt > 20) {
                break;
            }
        }
        memset((void*)&para_stream_t, 0, sizeof(para_stream));
    */
    ps->cmd = pf->cmd.cmd;
    ps->device_id = unit_para_t.unit_sundry.device_id;
    ps->flag = PARA_STREAM_BUSY;
    memcpy((void*) & (ps->md_adr), (void*) & (pf->cmd.md_adr), sizeof(md_adr_info));
    ps->remote_code = pf->header.remote_code;
    ps->source = source;
    ps->authority = pf->cmd.authority;
    ps->next = (s8*)(ps->data);

    //RLDEBUG("frame cmd processing\r\n");

    switch (pf->cmd.cmd) {
    case FRAME_CMD_READ_MOD_INFO: {
        read_md_info_deal(ps);
        goto FRAME_DONE;
        break;
    }
    case FRAME_CMD_LOGIN: {
        if (FRAME_SOURCE_LOCAL == source) {
            if (0x00 == pf->cmd.data[0]) {	//login
                if (usr_login(pf)) {
                    ps->ack = FRAME_ACK_USR_PASS_ERR;
                }
            } else if (0x01 == (pf->cmd.data[0])) {	//pwd changed

            } else if (0x02 == (pf->cmd.data[0])) {	//pwd reset

            }
        } else if (FRAME_SOURCE_REMOTE == source) {

        } else {
            ps->ack = FRAME_ACK_OTHER_ERR;
        }

        goto FRAME_DONE;
        break;
    }
    };

    //permission check
#ifdef RL_PERMISSION_CHECK
    if (FRAME_SOURCE_LOCAL == ps->source) {
        if (USR_UNLOGIN == usr_auth.authorize) {
            ps->ack = FRAME_ACK_USR_UNLOGIN;
            RLDEBUG("user unlogin\r\n");
            goto FRAME_DONE;
        }
    }
#endif

    switch (pf->cmd.cmd) {
    case FRAME_CMD_ALARM: {

        break;
    }
    case FRAME_CMD_HEARTBEAT: {
        //para_stream_t.data[0]=unit_dynamic_para_t.gps_t.gps_status;
        //para_stream_t.data[]
        break;
    }
    case FRAME_CMD_READ_PARA: {
        read_para_deal(pf, ps);
        break;
    }
    case FRAME_CMD_READ_MOD_INFO: {
        read_md_info_deal(ps);
        break;
    }
    case FRAME_CMD_SET_PARA: {
        set_para_deal(pf, ps);
#ifdef MONITOR_MODULE_ENABLE
        if (MOD_TYPE_MONITOR == (pf->cmd.md_adr.mod_type)) {
            //data_update(DATA_TYPE_UNIT);
        }
#endif
#ifdef OTHER_MODULE_ENABLE
        if (MOD_TYPE_MONITOR != (pf->cmd.md_adr.mod_type)) {
            //data_update(DATA_TYPE_MOD);
        }

#endif
        break;
    }
    case FRAME_CMD_LOGIN: {
        if (usr_login(pf)) {
            ps->ack = FRAME_ACK_USR_PASS_ERR;
        }
        break;
    }
    case FRAME_CMD_UPDATE: {

        break;
    }

    default: {
        RLDEBUG("can't find cmd\r\n");
        ps->ack = FRAME_ACK_CMD;
    }
    };
FRAME_DONE:
    return 0;
FRAME_ERR:
    return -1;
}
#if 0
s8 frame_send(para_stream *ps)
{
    s16 cnt;

    if (NULL == ps)
        return -1;

    memset((void*)&frame_send, 0, sizeof(frame_send));
    cnt = one_frame_pack(&frame_send, ps);
    if (cnt < 0)
        return -1;

    /*数据发送*/
    if (frame_tty_send((s8*)&frame_send, cnt))
        return -1;

    return 0;
}
#endif
