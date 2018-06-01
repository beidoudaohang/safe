/*
author:salan
date:20140422
description:
	iap api
ver:0.1
*/
/*include files*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helios.h"
#include "md5.h"
//#include "fw_update.h"
#//include "w25q32.h"
/*para*/
/* Padding */
static unsigned char MD5_PADDING[64] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


/* Basic MD5 step. MD5_Transform buf based on in */
static void MD5_Transform (UINT4 *buf, UINT4 *in)
{
	UINT4 a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
	MD5_FF ( a, b, c, d, in[ 0], MD5_S11, (UINT4) 3614090360u); /* 1 */
	MD5_FF ( d, a, b, c, in[ 1], MD5_S12, (UINT4) 3905402710u); /* 2 */
	MD5_FF ( c, d, a, b, in[ 2], MD5_S13, (UINT4)  606105819u); /* 3 */
	MD5_FF ( b, c, d, a, in[ 3], MD5_S14, (UINT4) 3250441966u); /* 4 */
	MD5_FF ( a, b, c, d, in[ 4], MD5_S11, (UINT4) 4118548399u); /* 5 */
	MD5_FF ( d, a, b, c, in[ 5], MD5_S12, (UINT4) 1200080426u); /* 6 */
	MD5_FF ( c, d, a, b, in[ 6], MD5_S13, (UINT4) 2821735955u); /* 7 */
	MD5_FF ( b, c, d, a, in[ 7], MD5_S14, (UINT4) 4249261313u); /* 8 */
	MD5_FF ( a, b, c, d, in[ 8], MD5_S11, (UINT4) 1770035416u); /* 9 */
	MD5_FF ( d, a, b, c, in[ 9], MD5_S12, (UINT4) 2336552879u); /* 10 */
	MD5_FF ( c, d, a, b, in[10], MD5_S13, (UINT4) 4294925233u); /* 11 */
	MD5_FF ( b, c, d, a, in[11], MD5_S14, (UINT4) 2304563134u); /* 12 */
	MD5_FF ( a, b, c, d, in[12], MD5_S11, (UINT4) 1804603682u); /* 13 */
	MD5_FF ( d, a, b, c, in[13], MD5_S12, (UINT4) 4254626195u); /* 14 */
	MD5_FF ( c, d, a, b, in[14], MD5_S13, (UINT4) 2792965006u); /* 15 */
	MD5_FF ( b, c, d, a, in[15], MD5_S14, (UINT4) 1236535329u); /* 16 */

	/* Round 2 */
	MD5_GG ( a, b, c, d, in[ 1], MD5_S21, (UINT4) 4129170786u); /* 17 */
	MD5_GG ( d, a, b, c, in[ 6], MD5_S22, (UINT4) 3225465664u); /* 18 */
	MD5_GG ( c, d, a, b, in[11], MD5_S23, (UINT4)  643717713u); /* 19 */
	MD5_GG ( b, c, d, a, in[ 0], MD5_S24, (UINT4) 3921069994u); /* 20 */
	MD5_GG ( a, b, c, d, in[ 5], MD5_S21, (UINT4) 3593408605u); /* 21 */
	MD5_GG ( d, a, b, c, in[10], MD5_S22, (UINT4)   38016083u); /* 22 */
	MD5_GG ( c, d, a, b, in[15], MD5_S23, (UINT4) 3634488961u); /* 23 */
	MD5_GG ( b, c, d, a, in[ 4], MD5_S24, (UINT4) 3889429448u); /* 24 */
	MD5_GG ( a, b, c, d, in[ 9], MD5_S21, (UINT4)  568446438u); /* 25 */
	MD5_GG ( d, a, b, c, in[14], MD5_S22, (UINT4) 3275163606u); /* 26 */
	MD5_GG ( c, d, a, b, in[ 3], MD5_S23, (UINT4) 4107603335u); /* 27 */
	MD5_GG ( b, c, d, a, in[ 8], MD5_S24, (UINT4) 1163531501u); /* 28 */
	MD5_GG ( a, b, c, d, in[13], MD5_S21, (UINT4) 2850285829u); /* 29 */
	MD5_GG ( d, a, b, c, in[ 2], MD5_S22, (UINT4) 4243563512u); /* 30 */
	MD5_GG ( c, d, a, b, in[ 7], MD5_S23, (UINT4) 1735328473u); /* 31 */
	MD5_GG ( b, c, d, a, in[12], MD5_S24, (UINT4) 2368359562u); /* 32 */

	/* Round 3 */
	MD5_HH ( a, b, c, d, in[ 5], MD5_S31, (UINT4) 4294588738u); /* 33 */
	MD5_HH ( d, a, b, c, in[ 8], MD5_S32, (UINT4) 2272392833u); /* 34 */
	MD5_HH ( c, d, a, b, in[11], MD5_S33, (UINT4) 1839030562u); /* 35 */
	MD5_HH ( b, c, d, a, in[14], MD5_S34, (UINT4) 4259657740u); /* 36 */
	MD5_HH ( a, b, c, d, in[ 1], MD5_S31, (UINT4) 2763975236u); /* 37 */
	MD5_HH ( d, a, b, c, in[ 4], MD5_S32, (UINT4) 1272893353u); /* 38 */
	MD5_HH ( c, d, a, b, in[ 7], MD5_S33, (UINT4) 4139469664u); /* 39 */
	MD5_HH ( b, c, d, a, in[10], MD5_S34, (UINT4) 3200236656u); /* 40 */
	MD5_HH ( a, b, c, d, in[13], MD5_S31, (UINT4)  681279174u); /* 41 */
	MD5_HH ( d, a, b, c, in[ 0], MD5_S32, (UINT4) 3936430074u); /* 42 */
	MD5_HH ( c, d, a, b, in[ 3], MD5_S33, (UINT4) 3572445317u); /* 43 */
	MD5_HH ( b, c, d, a, in[ 6], MD5_S34, (UINT4)   76029189u); /* 44 */
	MD5_HH ( a, b, c, d, in[ 9], MD5_S31, (UINT4) 3654602809u); /* 45 */
	MD5_HH ( d, a, b, c, in[12], MD5_S32, (UINT4) 3873151461u); /* 46 */
	MD5_HH ( c, d, a, b, in[15], MD5_S33, (UINT4)  530742520u); /* 47 */
	MD5_HH ( b, c, d, a, in[ 2], MD5_S34, (UINT4) 3299628645u); /* 48 */

	/* Round 4 */
	MD5_II ( a, b, c, d, in[ 0], MD5_S41, (UINT4) 4096336452u); /* 49 */
	MD5_II ( d, a, b, c, in[ 7], MD5_S42, (UINT4) 1126891415u); /* 50 */
	MD5_II ( c, d, a, b, in[14], MD5_S43, (UINT4) 2878612391u); /* 51 */
	MD5_II ( b, c, d, a, in[ 5], MD5_S44, (UINT4) 4237533241u); /* 52 */
	MD5_II ( a, b, c, d, in[12], MD5_S41, (UINT4) 1700485571u); /* 53 */
	MD5_II ( d, a, b, c, in[ 3], MD5_S42, (UINT4) 2399980690u); /* 54 */
	MD5_II ( c, d, a, b, in[10], MD5_S43, (UINT4) 4293915773u); /* 55 */
	MD5_II ( b, c, d, a, in[ 1], MD5_S44, (UINT4) 2240044497u); /* 56 */
	MD5_II ( a, b, c, d, in[ 8], MD5_S41, (UINT4) 1873313359u); /* 57 */
	MD5_II ( d, a, b, c, in[15], MD5_S42, (UINT4) 4264355552u); /* 58 */
	MD5_II ( c, d, a, b, in[ 6], MD5_S43, (UINT4) 2734768916u); /* 59 */
	MD5_II ( b, c, d, a, in[13], MD5_S44, (UINT4) 1309151649u); /* 60 */
	MD5_II ( a, b, c, d, in[ 4], MD5_S41, (UINT4) 4149444226u); /* 61 */
	MD5_II ( d, a, b, c, in[11], MD5_S42, (UINT4) 3174756917u); /* 62 */
	MD5_II ( c, d, a, b, in[ 2], MD5_S43, (UINT4)  718787259u); /* 63 */
	MD5_II ( b, c, d, a, in[ 9], MD5_S44, (UINT4) 3951481745u); /* 64 */

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}
/*funs*/
// Set pseudoRandomNumber to zero for RFC MD5 implementation
void MD5Init (MD5_CTX *mdContext, unsigned long pseudoRandomNumber)
{
	mdContext->i[0] = mdContext->i[1] = (UINT4)0;

	/* Load magic initialization constants */
	mdContext->buf[0] = (UINT4)0x67452301 + (pseudoRandomNumber * 11);
	mdContext->buf[1] = (UINT4)0xefcdab89 + (pseudoRandomNumber * 71);
	mdContext->buf[2] = (UINT4)0x98badcfe + (pseudoRandomNumber * 37);
	mdContext->buf[3] = (UINT4)0x10325476 + (pseudoRandomNumber * 97);
}
void MD5Update (MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen)
{
	UINT4 in[16];
	int mdi = 0;
	unsigned int i = 0, ii = 0;

	/* Compute number of bytes mod 64 */
	//mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	/* Update number of bits */
	/*
	if ((mdContext->i[0] + ((UINT4)inLen << 3)) < mdContext->i[0])
	mdContext->i[1]++;
	mdContext->i[0] += ((UINT4)inLen << 3);
	mdContext->i[1] += ((UINT4)inLen >> 29);
	*/
	while (inLen--)
	{
		/* Add new character to buffer, increment mdi */
		mdContext->in[mdi++] = *inBuf++;

		/* Transform if necessary */
		if (mdi == 0x40)
		{
			for (i = 0, ii = 0; i < 16; i++, ii += 4)
				in[i] = (((UINT4)mdContext->in[ii + 3]) << 24) |
				        (((UINT4)mdContext->in[ii + 2]) << 16) |
				        (((UINT4)mdContext->in[ii + 1]) << 8) |
				        ((UINT4)mdContext->in[ii]);

			MD5_Transform (mdContext->buf, in);
			mdi = 0;
		}
	}
}
void MD5Update1 (MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen)
{
	UINT4 in[16];
	int mdi = 0;
	unsigned int i = 0, ii = 0;

	/* Compute number of bytes mod 64 */
	mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	/* Update number of bits */

	if ((mdContext->i[0] + ((UINT4)inLen << 3)) < mdContext->i[0])
		mdContext->i[1]++;
	mdContext->i[0] += ((UINT4)inLen << 3);
	mdContext->i[1] += ((UINT4)inLen >> 29);

	while (inLen--)
	{
		/* Add new character to buffer, increment mdi */
		mdContext->in[mdi++] = *inBuf++;

		/* Transform if necessary */
		if (mdi == 0x40)
		{
			for (i = 0, ii = 0; i < 16; i++, ii += 4)
				in[i] = (((UINT4)mdContext->in[ii + 3]) << 24) |
				        (((UINT4)mdContext->in[ii + 2]) << 16) |
				        (((UINT4)mdContext->in[ii + 1]) << 8) |
				        ((UINT4)mdContext->in[ii]);

			MD5_Transform (mdContext->buf, in);
			mdi = 0;
		}
	}
}

void MD5Final (MD5_CTX *mdContext)
{
	UINT4 in[16];
	int mdi = 0;
	unsigned int i = 0, ii = 0, padLen = 0;

	/* Save number of bits */
	in[14] = mdContext->i[0];
	in[15] = mdContext->i[1];

	/* Compute number of bytes mod 64 */
	mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	/* Pad out to 56 mod 64 */
	padLen = (mdi < 56) ? (56 - mdi) : (120 - mdi);
	MD5Update1(mdContext, MD5_PADDING, padLen);

	/* Append length in bits and transform */
	for (i = 0, ii = 0; i < 14; i++, ii += 4)
		in[i] = (((UINT4)mdContext->in[ii + 3]) << 24) |
		        (((UINT4)mdContext->in[ii + 2]) << 16) |
		        (((UINT4)mdContext->in[ii + 1]) <<  8) |
		        ((UINT4)mdContext->in[ii]);
	MD5_Transform (mdContext->buf, in);

	/* Store buffer in digest */
	for (i = 0, ii = 0; i < 4; i++, ii += 4)
	{
		mdContext->digest[ii]   = (unsigned char)( mdContext->buf[i]        & 0xFF);
		mdContext->digest[ii + 1] = (unsigned char)((mdContext->buf[i] >>  8) & 0xFF);
		mdContext->digest[ii + 2] = (unsigned char)((mdContext->buf[i] >> 16) & 0xFF);
		mdContext->digest[ii + 3] = (unsigned char)((mdContext->buf[i] >> 24) & 0xFF);
	}
}

/*
checking md5
para:
	pfw_info--arm or fpga fw info
return:
	0--sus
	1--false
	2--suspend
*/

s8 md5_check(const s8 *filename, const s8 *md5, s8 *buf, u32 len)
{
	filename = filename;
	md5 = md5;
	buf = buf;
	len = 0;
#if 0
	MD5_CTX md5_ctx_t;
	uint32_t u32temp;
	uint32_t dat_remain;
	uint8_t *pbuf;
	uint32_t fw_adr;
	uint32_t dat_len = 0;

	if (NULL == pfw_info)
		return 1;
	if (NULL == pdat_buf) {
		return 1;
	}

	MD5Init(&md5_ctx_t, 0);

	if ((md5_ctx_t.i[0] + ((UINT4)(pfw_info->len) << 3)) < md5_ctx_t.i[0])
		md5_ctx_t.i[1]++;
	md5_ctx_t.i[0] += ((UINT4)(pfw_info->len) << 3);
	md5_ctx_t.i[1] += ((UINT4)(pfw_info->len) >> 29);

	dat_remain = (pfw_info->len);
	pbuf = pdat_buf;
	fw_adr = pfw_info->adr_offset;
	while (dat_remain) {
		//read dat
		pbuf = pdat_buf;
		dat_len = 0;
		if (UPDATE_T_ARM == pfw_info->type_t) {
			if (dat_remain > buf_len) {
				u32temp = buf_len;
				dat_len = buf_len;
				while (u32temp) {
					*(uint16_t*)pbuf = *(uint16_t*)(fw_adr);
					pbuf += 2;
					fw_adr += 2;
					u32temp -= 2;
				}
				dat_remain -= buf_len;
			} else {
				u32temp = dat_remain;
				dat_len = dat_remain;
				while (u32temp) {
					*(uint16_t*)pbuf = *(uint16_t*)(fw_adr);
					pbuf += 2;
					fw_adr += 2;
					u32temp -= 2;
				}
				dat_remain = 0;
			}
		} else if (UPDATE_T_FPGA == pfw_info->type_t) {
			if (dat_remain > buf_len) {
				dat_len = buf_len;
				wb_flash_read(fw_adr, pbuf, buf_len);
				fw_adr += buf_len;
				dat_remain -= buf_len;
			} else {
				dat_len = dat_remain;
				wb_flash_read(fw_adr, pbuf, dat_len);
				dat_remain = 0;
			}
		} else if (UPDATE_T_PARA == pfw_info->type_t) {
			if (dat_remain <= buf_len) {
				dat_len = dat_remain;
				wb_flash_read(fw_adr, pbuf, MACHINE_PARA_SIZE/*buf_len*/);
				fw_adr = GLOBAL_PARA_ADR;
				wb_flash_read(fw_adr, (pbuf + MACHINE_PARA_SIZE), dat_remain - MACHINE_PARA_SIZE);
				dat_remain = 0;
			}
		}

		//update md5
		MD5Update(&md5_ctx_t, pdat_buf, dat_len);

		//checking update?,
		if ((UPDATE_OP_NORMAL != update_t.update_op_t) && (UPDATE_OP_MD5_CHECK != update_t.update_op_t) && (UPDATE_OP_FINISH != update_t.update_op_t))
			return 2;
	}
	MD5Final(&md5_ctx_t);

	if (memcmp(md5_ctx_t.digest, pfw_info->md5, 16))
		return 1;
#endif
	return 0;
}

/*
para:
	str:the data stream
	strlen:data len of the stream
	md5:the buf of dest md5
return:
	-1:err
	0:sus
attention:
	the size of md5 must bigger than 16bytes

 */

s8 md5_make(const s8 *str, u32 strlen, s8 *md5)
{
	MD5_CTX md5_ctx_t;

	if ((NULL == str) || (NULL == md5))
		return -1;


	MD5Init(&md5_ctx_t, 0);

	if ((md5_ctx_t.i[0] + ((UINT4)strlen << 3)) < md5_ctx_t.i[0])
		md5_ctx_t.i[1]++;
	md5_ctx_t.i[0] += ((UINT4)strlen << 3);
	md5_ctx_t.i[1] += ((UINT4)strlen >> 29);

	MD5Update(&md5_ctx_t, (unsigned char*)str, strlen);
	MD5Final(&md5_ctx_t);

	memcpy(md5, md5_ctx_t.digest, 16);

	return 0;
}


