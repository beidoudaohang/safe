/*
name:dig_comm.c
author:salan
date:20161208
ver:1.0
description:

*/
/*include files*/
#include "string.h"
#include "helios.h"
#include "dig_comm_def.h"
#include "dig_band0.h"
#include "dig_band0_para_table.h"
#include "porting.h"
#include "log.h"
#include "para_table.h"

/*para*/
static dig_comm_frame dig_comm_send_buf;
dig_comm_frame dig_comm_recv_buf;
static u8 dig_comm_err_adr[DIG_COMM_KEY_NUM_MAX] = {0};

static dig_transmit dig_transmit_t;
//static dig_set_adr dig_set_adr_t;
dig_set_adr dig_set_adr_t;
/*funs*/
/*static funs*/


/*global funs*/
/*
para:pkey 指令缓存
num:指令缓存内的指令个数

return:
	0:sus
	-1:false
*/
s8 dig_comm_send(dig_adrs *adrs, DIG_COMM_CMD cmd)
{
	u8 cnt;
	s32 err;

	if (NULL == adrs) {
		return -1;
	}

	if ((DIG_CMD_WRITE != cmd) && (DIG_CMD_READ != cmd))
		return -1;

	memset((void*)&dig_comm_send_buf, 0, sizeof(dig_comm_send_buf));

	dig_comm_send_buf.dig_comm_r.comm_start = DIG_COMM_START;
	dig_comm_send_buf.dig_comm_r.comm_cmd = cmd;

	if (DIG_CMD_WRITE == cmd) {
		for (cnt = 0; cnt < DIG_COMM_KEY_NUM_MAX; cnt++) {
			if (adrs->adr[cnt]) {
				dig_comm_send_buf.dig_comm_w.comm_adr[cnt] = adrs->adr[cnt];
				err = fill_one_adr(adrs->adr[cnt], (void*) & (dig_comm_send_buf.dig_comm_w.comm_dat[cnt]));
				if (err < 0) {
					RLDEBUG("dig_comm_send:err=fill_one_adr(adrs->adr[cnt], (void*) & (dig_comm_send_buf.dig_comm_w.comm_dat[cnt]));\r\n");
					return -1;
				}
			}
		}
		dig_comm_crc_make((s8*)(dig_comm_send_buf.arr + 1), (sizeof(dig_comm_send_buf.dig_comm_w) - 3), &(dig_comm_send_buf.dig_comm_w.comm_crc));
	} else if (DIG_CMD_READ == cmd) {
		for (cnt = 0; cnt < DIG_COMM_KEY_NUM_MAX; cnt++) {
			dig_comm_send_buf.dig_comm_r.comm_adr[cnt] = adrs->adr[cnt];
		}
		dig_comm_crc_make((s8*)(dig_comm_send_buf.arr + 1), (sizeof(dig_comm_send_buf.dig_comm_r) - 3), &(dig_comm_send_buf.dig_comm_r.comm_crc));
	}

	//发送
	if (DIG_CMD_WRITE == cmd) {
		if (dig_band0_tty_send((s8*) & (dig_comm_send_buf.arr), sizeof(dig_comm_send_buf.dig_comm_w)))
			return -1;
	} else {
		if (dig_band0_tty_send((s8*) & (dig_comm_send_buf.arr), sizeof(dig_comm_send_buf.dig_comm_r)))
			return -1;
	}
	return 0;
}

/*
para:
src指令数据
len:指令数据长度

return:
	0:sus
	-1:一般错误
	-2:收到帧的crc错误
	-3:发送帧在fpga端校验错误
*/
s8 dig_comm_recv_deal(u8 *src, u8 len)
{
	u8 cnt;
	u16 crc;
	dig_comm_frame *pcomm;

	if ((NULL == src)) {
		return -1;
	}
	if (len < DIG_COMM_DAT_LEN) {
		return -1;
	}

	/***********************************************/
#if 0
	for (cnt = 0; cnt < len; cnt++) {
		RLDEBUG("dig_comm_recv_deal:recv dat is:%x \r\n", (u8)(src[cnt]));
	}
#endif
	/***********************************************/
	for (cnt = 0; cnt < len; cnt++) {
		if (DIG_COMM_START == src[cnt])
			break;
	}
	if ((len - cnt) < DIG_COMM_DAT_LEN) {
		RLDEBUG("dig_comm_recv_deal:(len - cnt) < DIG_COMM_DAT_LEN err \r\n");
		return -1;
	}

	memset((void*)&dig_comm_err_adr, 0, sizeof(dig_comm_err_adr));

	pcomm = (dig_comm_frame *)(src + cnt);
	dig_comm_crc_make((s8*)((pcomm->arr) + 1), (sizeof(pcomm->dig_comm_w) - 3), &crc);
	if (crc != (pcomm->dig_comm_w.comm_crc)) {
		RLDEBUG("dig_comm_recv_deal:crc != (pcomm->dig_comm_w.comm_crc) err \r\n");
		RLDEBUG("dig_comm_recv_deal:new crc:%x \r\n", crc);
		RLDEBUG("dig_comm_recv_deal:old crc:%x \r\n", pcomm->dig_comm_w.comm_crc);
		return -2;
	}

	if ((pcomm->dig_comm_w.comm_cmd)&DIG_COMM_CRC_ERR) {
		RLDEBUG("dig_comm_recv_deal:(pcomm->dig_comm_w.comm_cmd)&DIG_COMM_CRC_ERR err \r\n");
		return -3;
	}

	if (0x0f & (pcomm->dig_comm_w.comm_cmd)) {
		RLDEBUG("dig_comm_recv_deal:0x0f & (pcomm->dig_comm_w.comm_cmd) err \r\n");
		for (cnt = 0; cnt < DIG_COMM_KEY_NUM_MAX; cnt++) {
			if (GETBIT((pcomm->dig_comm_w.comm_cmd), cnt)) {
				dig_comm_err_adr[cnt] = (pcomm->dig_comm_w.comm_adr[cnt]);
			} else if (!((pcomm->dig_comm_w.comm_cmd)&DIG_CMD_WRITE)) {
				analyze_one_adr(pcomm->dig_comm_w.comm_adr[cnt], (s8*)(pcomm->dig_comm_w.comm_dat[cnt]));
			}
		}
	} else {
		//RLDEBUG("dig_comm_recv_deal:analyze dat \r\n");
		for (cnt = 0; cnt < DIG_COMM_KEY_NUM_MAX; cnt++) {
			if (!((pcomm->dig_comm_w.comm_cmd)&DIG_CMD_WRITE)) {
				analyze_one_adr(pcomm->dig_comm_w.comm_adr[cnt], (s8*) & (pcomm->dig_comm_w.comm_dat[cnt]));
			}
		}
	}

	return 0;
}

/*
fun：获取失败的地址码信息
para:
	adrs：地址码存放地址
return:
	0---sus
	-1---err
 */
s8 dig_comm_feedback(dig_adrs *adrs)
{
	u8 cnt;
	u8 index = 0;

	if (NULL == adrs) {
		return -1;
	}
	/*
	for (cnt = 0; cnt < DIG_COMM_KEY_NUM_MAX; cnt++) {
		if (dig_comm_err_adr[cnt]) {
			adrs->adr[index] = dig_comm_err_adr[cnt];
			index++;
		}
	}*/
	memcpy((void*)(adrs->adr), (void*)dig_comm_err_adr, DIG_COMM_KEY_NUM_MAX);

	return 0;
}

/*
fun：添加设置指令到设置队列中
para:
	adr：地址码
return:
	0---sus
	-1---err
 */
s8 band0_set_adr_add(u8 adr)
{
	u16 cnt;

	//RLDEBUG("band0_set_adr_add start!\r\n");

	if (dig_set_adr_t.total >= DIG_SET_ADR_BUF_SIZE) {
		RLDEBUG("band0_set_adr_add:dig_set_adr_t.total >= DIG_SET_ADR_BUF_SIZE -----> dig_set_adr_t.total=%d\r\n", dig_set_adr_t.total);
		return -1;
	}

	//filter
	for (cnt = 0; cnt < DIG_SET_ADR_BUF_SIZE; cnt++) {
		if (adr == dig_set_adr_t.adr[cnt])
			return 0;
	}
	//set
	for (cnt = 0; cnt < DIG_SET_ADR_BUF_SIZE; cnt++) {
		if (!(dig_set_adr_t.adr[cnt])) {
			dig_set_adr_t.adr[cnt] = adr;
			dig_set_adr_t.total++;
			break;
		}
	}

	if (cnt >= DIG_SET_ADR_BUF_SIZE) {
		RLDEBUG("band0_set_adr_add:cnt >= DIG_SET_ADR_BUF_SIZE \r\n");
		return -1;
	}

	return 0;
}
/*
fun：发送临时指令
para:
	trans：临时指令
	cmd：对应的操作
return:
	0---sus
	-1---err
 */
s8 dig_comm_transmit_send(transmit_arry *trans, DIG_COMM_CMD cmd)
{
	u8 cnt;

	if (NULL == trans) {
		return -1;
	}

	if ((DIG_CMD_WRITE != cmd) && (DIG_CMD_READ != cmd))
		return -1;

	memset((void*)&dig_comm_send_buf, 0, sizeof(dig_comm_send_buf));
	dig_comm_send_buf.dig_comm_r.comm_start = DIG_COMM_START;
	dig_comm_send_buf.dig_comm_r.comm_cmd = cmd;

	if (DIG_CMD_WRITE == cmd) {
		dig_comm_send_buf.dig_comm_w.comm_adr[0] = trans->adr;
		dig_comm_send_buf.dig_comm_w.comm_dat[0] = trans->DAT.dat32;
		dig_comm_crc_make((s8*)(dig_comm_send_buf.arr + 1), (sizeof(dig_comm_send_buf.dig_comm_w) - 3), &(dig_comm_send_buf.dig_comm_w.comm_crc));
		cnt = sizeof(dig_comm_write);
	} else if (DIG_CMD_READ == cmd) {
		dig_comm_send_buf.dig_comm_r.comm_adr[0] = trans->adr;
		dig_comm_crc_make((s8*)(dig_comm_send_buf.arr + 1), (sizeof(dig_comm_send_buf.dig_comm_r) - 3), &(dig_comm_send_buf.dig_comm_r.comm_crc));
		cnt = sizeof(dig_comm_read);
	} else {
		return -1;
	}


	//发送
	dig_band0_tty_send((s8*) & (dig_comm_send_buf.arr), cnt);

	return 0;
}
/*
fun：添加转发指令到队列中
para:
	trans：临时指令
return:
	0---sus
	-1---err
 */
s8 band0_transmit_add(transmit_arry *trans)
{
	u8 cnt;

	if (NULL == trans)
		return -1;

	for (cnt = 0; cnt < DIG_TRANSMIT_BUF_SIZE; cnt++) {
		if (!(dig_transmit_t.t_arry[cnt].adr))
			break;
	}

	if (cnt >= DIG_TRANSMIT_BUF_SIZE)
		return -1;

	dig_transmit_t.t_arry[cnt].adr = trans->adr;
	dig_transmit_t.t_arry[cnt].DAT.dat32 = trans->DAT.dat32;
	dig_transmit_t.total++; //@ccTag20170807

	return 0;
}


/*
fun：设置指令处理
return:
	-1-----错误
	0------所有指令已经处理完毕
	>0-----未处理的指令个数

 */
//static void dig_set_deal(void)
s32 dig_set_deal(void)
{
	u8 cnt = 0;
	u16 adr_numbers = 0;
	u16 i, j;
	dig_adrs adrs, check;
	s32 err;

	if (dig_set_adr_t.total) {
		adr_numbers = (((dig_set_adr_t.total) >> 2) + 1);

		for (cnt = 0; cnt < adr_numbers; cnt++) {
			/*one loop*/
			memset(&adrs, 0, sizeof(adrs));
			memset(&check, 0, sizeof(check));

			j = 0;
			for (i = 0; i < DIG_SET_ADR_BUF_SIZE; i++) {
				if (dig_set_adr_t.adr[i]) {
					adrs.adr[j] = dig_set_adr_t.adr[i];
					adrs.index[j] = i;
					j++;
				}
				if (j > 3) {
					break;
				}
			}

			if (j == 0) {
				//RLDEBUG("dig_set_deal:j == 0\r\n");
				continue;
			}

			err = dig_comm_send(&adrs, DIG_CMD_WRITE);
			if (err < 0) {
				RLDEBUG("dig_set_deal:dig_comm_send(&adrs, DIG_CMD_WRITE) false\r\n");
				continue;
			}
			/*等待数据返回*/
			err = dig_band0_tty_recv();
			if (err < 0) {
				RLDEBUG("dig_set_deal:err=dig_band0_tty_recv() false\r\n");
				continue;
			}

			/*处理返回的数据*/
			err = dig_comm_recv_deal((dig_comm_recv_buf.arr), sizeof(dig_comm_recv_buf));
			if (err < 0) {
				RLDEBUG("dig_set_deal:dig_comm_recv_deal((dig_comm_recv_buf.arr), sizeof(dig_comm_recv_buf)) false\r\n");
				continue;
			}
			dig_comm_feedback(&check);
			for (i = 0; i < DIG_COMM_KEY_NUM_MAX; i++) {
				if (!(check.adr[i])) {
					dig_set_adr_t.adr[adrs.index[i]] = 0;
					if (dig_set_adr_t.total) {
						dig_set_adr_t.total--;
					}
				}
			}
		}
	}

	return (dig_set_adr_t.total);
}
/*
fun：转发指令处理

 */
static void dig_transmit_deal(void)
{
	u8 cnt;
	dig_adrs check;
	u8 i;

	if (dig_transmit_t.total) {
		for (cnt = 0; cnt < DIG_TRANSMIT_BUF_SIZE; cnt++) {
			if (!(dig_transmit_t.t_arry[cnt].adr))
				continue;

			memset((void*)&check, 0, sizeof(check));
			dig_comm_transmit_send(&(dig_transmit_t.t_arry[cnt]), DIG_CMD_WRITE);
			/*等待数据返回*/
			dig_band0_tty_recv();
			/*处理返回的数据*/
			dig_comm_recv_deal((dig_comm_recv_buf.arr), sizeof(dig_comm_recv_buf));
			dig_comm_feedback(&check);

			if (!(check.adr[0])) {
				memset((void*)(&dig_transmit_t.t_arry[cnt]), 0, sizeof(transmit_arry));
				dig_transmit_t.total--;
			}
		}
	}
}
/*
fun：读取指令处理

 */
void dig_read_deal(void)
{
	u16 cnt, index, size;
	dig_adrs adrs, check;
	static u32 total = 0, fail_cnt = 0;

	size = get_dig_read_table_size();
	index = 0;
	for (cnt = 0; cnt < (size / 4 + 1); cnt++) {
		memset((void*)&check, 0, sizeof(check));
		memset((void*)&adrs, 0, sizeof(adrs));
		if (get_dig_read_table_next_adrs(index, &index, (s8*)(adrs.adr)) >= 0) {
			// RLDEBUG("dig_read_deal:read adr is %x,%x,%x,%x \r\n", adrs.adr[0], adrs.adr[1], adrs.adr[2], adrs.adr[3]);
			dig_comm_send(&adrs, DIG_CMD_READ);
			
			total++;

			/*等待数据返回*/
			dig_band0_tty_recv();
			/*处理返回的数据*/
			if(dig_comm_recv_deal((dig_comm_recv_buf.arr), sizeof(dig_comm_recv_buf))){
				fail_cnt++;
			}else{
				
				if(dig_comm_err_adr[0]+dig_comm_err_adr[1]+dig_comm_err_adr[2]+dig_comm_err_adr[3] > 0){
					fail_cnt++;
				}
			}
			if(total%1000 == 0)
				printf("total: %d, error rate: %0.2f\n", total, fail_cnt/total);
			
			dig_comm_feedback(&check);
		}
	}
}

void dig_cmd_set_deal(void)
{
	s32 err;
	err = dig_set_deal();
	if (!err) {
		dig_transmit_deal();
	}
}

/*总接口*/
void dig_band0_handle(void)
{
	s32 err;
	static u8 read_cycle;

	err = dig_set_deal();
	if (!err) {
		dig_transmit_deal();
	}

	if (read_cycle > 10) {
		read_cycle = 0;
		dig_read_deal();
	} else {
		read_cycle++;
	}
	

}

/*
fun：读取fpga版本号
para:

return:
	0---sus
	-1---err
*/
s32 dig_band0_read_sv(void)
{
	s32 err;
	dig_adrs adrs;

	memset(&adrs, 0, sizeof(dig_adrs));
	adrs.adr[0] = 0xfd;

	dig_comm_send(&adrs, DIG_CMD_READ);

	/*等待数据返回*/
	err = dig_band0_tty_recv();
	if (err < 0) {
		RLDEBUG("dig_band0_read_sv:dig_band0_tty_recv() false\r\n");
		return -1;
	}

	RLDEBUG("dig_band0_read_sv:dig_band0_tty_recv() len is:%d \r\n", err);
	/*处理返回的数据*/
	if (dig_comm_recv_deal((dig_comm_recv_buf.arr), sizeof(dig_comm_recv_buf))) {
		RLDEBUG("dig_band0_read_sv:dig_comm_recv_deal() false\r\n");
		return -1;
	}

	return 0;
}

/*
fun：fpga参数配置完成
para:

return:
	0---sus
	-1---err
*/
s32 dig_band0_set_finish(void)
{
	s32 err;
	transmit_arry adrs;

	memset(&adrs, 0, sizeof(adrs));
	adrs.adr = 0xff;
	adrs.DAT.dat32 = 0xaaaaaaaaul;

	err = band0_transmit_add(&adrs);

	if (err < 0) {
		RLDEBUG("dig_band0_set_finish:band0_transmit_add() false\r\n");
		return -1;
	}
	return 0;
}
/*
fun：fpga软复位
para:

return:
	0---sus
	-1---err
*/
s32 dig_band0_reset(void)
{
	s32 err;
	transmit_arry adrs;

	memset(&adrs, 0, sizeof(adrs));
	adrs.adr = 0xfe;
	adrs.DAT.dat32 = 0xaaaaaaaaul;

	err = band0_transmit_add(&adrs);

	if (err < 0) {
		RLDEBUG("dig_band0_set_finish:band0_transmit_add() false\r\n");
		return -1;
	}
	return 0;
}


/*
fun:
	init band0 set array
para:
return:

 */
s32 dig_band0_init(void)
{
	s32 err;

	memset((void*)&dig_set_adr_t, 0, sizeof(dig_set_adr_t));

	return 0;
}
