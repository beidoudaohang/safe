/*
name:porting.c
author:salan
date:20170411
ver:1.0
description:
	linux porting api
*/

/*****************************include files***********************/
#include "helios.h"
#include <stdio.h>
#include <stdlib.h>
//file control
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
//time delay
#include <time.h>
#include <sys/time.h>
//uart
#include <sys/types.h>
#include <termios.h>
#include <string.h>
#include <stdarg.h>

#include "para_table.h"
#include "porting.h"
#include "md5.h"
#include "pwd.h"
#include "log.h"
/*****************************para define*************************/

static const s8 para_file_path[] = {"datafile"};

static const s8 unit_file_name_a[] = {"datafile/unita"};
static const s8 unit_file_name_b[] = {"datafile/unitb"};

static const s8 band_file_name_a[MOD_NUM_IN_ONE_PCB][20] = {"datafile/band0a","datafile/band1a","datafile/band2a"};
static const s8 band_file_name_b[MOD_NUM_IN_ONE_PCB][20] = {"datafile/band0b","datafile/band1b","datafile/band2b"};

static const s8 pcb_file_name_a[] = {"datafile/pcba"};
static const s8 pcb_file_name_b[] = {"datafile/pcbb"};

static s8 file_buf[MAX(sizeof(unit_para), (MD5_CODE_SIZE + 32 +  sizeof(band_para)))] = {0};

typedef enum {
	PARA_FILE_NORMAL = 0,
	PARA_FILE_READ
} PARA_FILE_FLAG;
static volatile PARA_FILE_FLAG unit_file_read_flag;
static volatile PARA_FILE_FLAG band_file_read_flag;

/*****************************data struct define******************/

/*****************************functions***************************/
/*
relay参数管理办法
1、relay参数分3部分存储，分别问“整机参数”、“模块参数”、“和电路板相关的参数”
每部分的参数又分两个文件来存储，分别为a、b。
2、两份参数文件的管理上，采用文件编号的形式来标识那份文件是最新的。每次读取都取文件编号为大的文件，每次参数写入都取文件编号为小的文件。
3、每份参数文件的组成分为三部分：
	a、文件编号，32bit无符号整数
	b、文件内容
	c、文件内容的md5校验值
4、读取文件流程
	a、打开文件，读取文件编号，选择文件编号大的文件
	b、读取文件内容，对内容进行解密并计算md5值
	c、读取md5值，与所计算的md5值进行比较
	d、若md5值一直，认为文件可用，若不一致，则认为文件不可用，需选取文件编号小的文件老重复a-d流程。
	e、若两份文件皆不可用，则做出提示。
 */
/*
fun：文件编码
para：
	fd：文件标识
	buf：缓存
	len：缓存大小
	size：需要处理的原始数据长度
return：
	0---sus
	-1---err
 */
s32 file_encode(s8 *buf, u32 len, u32 size)
{
	s32 err;
	s8 md5[MD5_CODE_SIZE];

	if (NULL == buf) {
		return -1;
	}
	if (len < (size + MD5_CODE_SIZE)) {
		return -1;
	}

	memset(md5, 0, sizeof(md5));
	err = md5_make(buf, size, md5);
	if (err < 0) {
		return -1;
	}
	EncryptCrackExt(buf, size);

	memcpy((buf + size), md5, sizeof(md5));

	printf("file_encode: buf_len=%d,data_len=%d,md5_buf_size=%ld \r\n", len, size, sizeof(md5));
	for (err = 0; err < sizeof(md5); err++) {
		printf("%02x ", (u8)(md5[err]));
	}
	printf("\r\n");

	return 0;
}

s32 file_decode(s8*buf, u32 len, u32 size)
{
	s32 err;
	s8 md5[MD5_CODE_SIZE];

	if (NULL == buf) {
		return -1;
	}
	if (len < (size + MD5_CODE_SIZE)) {
		return -1;
	}

	EncryptCrackExt(buf, size);
	err = md5_make(buf, size, md5);
	if (err < 0) {
		RLDEBUG("file_decode:md5_make() err\r\n");
		return -1;
	}



	printf("cal md5:");
	for (err = 0; err < (s32)(sizeof(md5)); err++) {
		printf("%02x ", (u8)(md5[err]));
	}
	printf("\r\n");
	printf("src md5:");
	for (err = 0; err < (s32)(sizeof(md5)); err++) {
		printf("%02x ", (u8)((buf + size)[err]));
	}
	printf("\r\n");


	err = memcmp((buf + size), md5, sizeof(md5));
	if (err) {
		RLDEBUG("file_decode:memcmp() md5 err\r\n");
		return -1;
	}

	return 0;
}
/*********************file operation***************/
s32 unit_file_init(void)
{
	s32 err;
	s32 len;
	u32 filecode = 0;
	s32 filenode;

	//test path
	err = access((const char*)para_file_path, F_OK);
	if (err < 0) {
		RLDEBUG("para path is inavilable\r\n");
		err = mkdir((const char*)para_file_path, S_IRWXU | S_IRGRP | S_IWGRP);
		if (err < 0)
		{
			RLDEBUG("mkdir para path err\r\n");
			return -1;
		}
	}

	//unit file a
	err = access((const char*)unit_file_name_a, F_OK);
	if (err < 0) {
		filenode = open((const char*)unit_file_name_a, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		if (filenode < 0) {
			return -1;
		}

		err = write(filenode, &filecode, sizeof(filecode));
		if (err < 0) {
			close(filenode);
			return -1;
		}
		memset(file_buf, 0, sizeof(file_buf));
		err = file_encode(file_buf, sizeof(file_buf), sizeof(unit_para));
		if (err < 0) {
			close(filenode);
			return -1;
		}
		err = write(filenode, file_buf, (sizeof(unit_para) + MD5_CODE_SIZE));
		if ((err < 0) || (err < ((s32)sizeof(unit_para) + MD5_CODE_SIZE))) {
			close(filenode);
			return -1;
		}
		close(filenode);
	}

	//unit file b
	err = access((const char*)unit_file_name_b, F_OK);
	if (err < 0) {
		filenode = open((const char*)unit_file_name_b, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		if (filenode < 0) {
			return -1;
		}

		err = write(filenode, &filecode, sizeof(filecode));
		if (err < 0) {
			close(filenode);
			return -1;
		}
		memset(file_buf, 0, sizeof(file_buf));
		err = file_encode(file_buf, sizeof(file_buf), sizeof(unit_para));
		if (err < 0) {
			close(filenode);
			return -1;
		}
		err = write(filenode, file_buf, (sizeof(unit_para) + MD5_CODE_SIZE));
		if ((err < 0) || (err < ((s32)sizeof(unit_para) + MD5_CODE_SIZE))) {
			close(filenode);
			return -1;
		}
		close(filenode);
	}

	return 0;
}
s32 unit_file_read(unit_para *up)
{
	s32 err;
	s32 file_a, file_b;
	u32 file_code_a, file_code_b;
	s32 read_len;
	s32 file_cur, file_code_cur;

	if (NULL == up)
		return -1;
	//open file
	file_a = open((const char*)unit_file_name_a, O_RDONLY);
	if (file_a < 0) {
		RLDEBUG("open unit file a false\r\n");
		return -1;
	}
	file_b = open((const char*)unit_file_name_b, O_RDONLY);
	if (file_b < 0) {
		RLDEBUG("open unit file b false\r\n");
		close(file_a);
		return -1;
	}

	//chose file
	read_len = read(file_a, &file_code_a, sizeof(file_code_a));
	if ((read_len != sizeof(file_code_a)) || (read_len < 0)) {
		RLDEBUG("unit file a is empty\r\n");
		close(file_a);
		return -1;
	}
	read_len = read(file_b, &file_code_b, sizeof(file_code_b));
	if ((read_len != sizeof(file_code_b)) || (read_len < 0)) {
		RLDEBUG("unit file b is empty\r\n");
		close(file_a);
		close(file_b);
		return -1;
	}

	if (file_code_a > file_code_b) {
		file_code_cur = file_code_a;
		file_cur = file_a;
		//close(file_b);
		//RLDEBUG("unit para chose file a\r\n");
	} else {
		file_code_cur = file_code_b;
		file_cur = file_b;
		//close(file_a);
		//RLDEBUG("unit para chose file b\r\n");
	}
	//read
	file_code_cur = 0;
UNIT_FILE_READ_TRY:
	file_code_cur++;
	memset(file_buf, 0, sizeof(file_buf));
	read_len = read(file_cur, file_buf, (sizeof(unit_para) + MD5_CODE_SIZE));
	if ((read_len < 0)/* || (read_len != (sizeof(unit_para) + MD5_CODE_SIZE))*/) {
		//close(file_cur);
		RLDEBUG("unit_file_read:read unit file false\r\n");
	}
	err = file_decode(file_buf, sizeof(file_buf), read_len - MD5_CODE_SIZE);
	if (err < 0) {
		RLDEBUG("unit_file_read:newer unit file err\r\n");
		if (file_cur == file_a) {
			file_cur = file_b;
		} else if (file_cur == file_b) {
			file_cur = file_a;
		} else {
			goto UNIT_FILE_READ_ERR;
		}
		if (file_code_cur < 2) {
			goto UNIT_FILE_READ_TRY;
		} else {
			goto UNIT_FILE_READ_ERR;
		}
	}

	memcpy(up, file_buf, read_len - MD5_CODE_SIZE);

	//close(file_cur);
	close(file_a);
	close(file_b);
	file_cur = 0;

	return 0;

UNIT_FILE_READ_ERR:
	RLDEBUG("unit_file_write:UNIT_FILE_READ_ERR RUN\r\n");
	close(file_a);
	close(file_b);
	return -1;
}
s32 unit_file_write(unit_para *up)
{
	s32 err;
	s32 file_a, file_b;
	u32 file_code_a, file_code_b;
	s32 read_len;
	s32 file_cur, file_code_cur;
	off_t file_offset = 0;

	if (NULL == up)
		return -1;
	//open file
	file_a = open((const char*)unit_file_name_a, O_RDWR);
	if (file_a < 0) {
		RLDEBUG("unit_file_write:open unit file a false\r\n");
		return -1;
	}
	file_b = open((const char*)unit_file_name_b, O_RDWR);
	if (file_b < 0) {
		close(file_a);
		RLDEBUG("unit_file_write:open unit file b false\r\n");
		return -1;
	}

	//check file point adr
	file_offset = lseek(file_a, 0, SEEK_SET);
	file_offset = lseek(file_b, 0, SEEK_SET);

	//chose file
	read_len = read(file_a, &file_code_a, sizeof(file_code_a));
	if ((read_len != sizeof(file_code_a)) || (read_len < 0)) {
		close(file_a);
		return -1;
	}
	read_len = read(file_b, &file_code_b, sizeof(file_code_b));
	if ((read_len != sizeof(file_code_b)) || (read_len < 0)) {
		close(file_a);
		close(file_b);
		return -1;
	}

	if (file_code_a < file_code_b) {
		file_code_cur = file_code_b;
		file_cur = file_a;
		//close(file_b);
		//RLDEBUG("unit_file_write:choose unit file a\r\n");
	} else {
		file_code_cur = file_code_a;
		file_cur = file_b;
		//close(file_a);
		//RLDEBUG("unit_file_write:choose unit file b\r\n");
	}
	//write
	read_len = lseek(file_cur, 0, SEEK_SET);
	if (0 > read_len) {
		RLDEBUG("unit_file_write:lseek unit file false\r\n");
		goto UNIT_FILE_WRITE_ERR;
	}

	file_code_cur += 1;
	read_len = write(file_cur, &file_code_cur, sizeof(file_code_cur));

	//RLDEBUG("unit_file_write:unit file code is:%d\r\n", file_code_cur);
	//RLDEBUG("unit_file_write:unit para size is:%d\r\n", sizeof(unit_para));

	if (read_len < 0) {
		RLDEBUG("unit_file_write:write unit file code false\r\n");
		goto UNIT_FILE_WRITE_ERR;
	}

	//RLDEBUG("unit_file_write:unit file code len is:%d\r\n", read_len);
	memset(file_buf, 0, sizeof(file_buf));
	memcpy(file_buf, up, sizeof(unit_para));
	err = file_encode(file_buf, sizeof(file_buf), sizeof(unit_para));
	if (err < 0) {
		goto UNIT_FILE_WRITE_ERR;
	}
	read_len = write(file_cur, (const void*)file_buf, (sizeof(unit_para) + MD5_CODE_SIZE));
	if (read_len < 0) {
		RLDEBUG("unit_file_write:write unit file false\r\n");
		goto UNIT_FILE_WRITE_ERR;
	}
	//RLDEBUG("unit_file_write:unit file data len is:%d\r\n", read_len);

	read_len = fsync(file_cur);
	if (read_len < 0)
		RLDEBUG("unit_file_write:fsync unit file false\r\n");
	close(file_cur);
	return 0;

UNIT_FILE_WRITE_ERR:
	RLDEBUG("unit_file_write:UNIT_FILE_WRITE_ERR RUN\r\n");
	close(file_a);
	close(file_b);
	return -1;
}
s32 band_file_init(void)
{
	s32 err;
	s32 len;
	u32 filecode = 0;
	s32 filenode;
	u8 mod_index;
	//test path
	err = access((const char*)para_file_path, F_OK);
	if (err < 0) {
		RLDEBUG("para path is inavilable\r\n");
		err = mkdir((const char*)para_file_path, S_IRWXU | S_IRGRP | S_IWGRP);
		if (err < 0)
		{
			RLDEBUG("mkdir para path err\r\n");
			return -1;
		}
	}

	for(mod_index=0; mod_index< MOD_NUM_IN_ONE_PCB; ++mod_index){
		//band file a
		err = access((const char*)band_file_name_a[mod_index], F_OK);
		if (err < 0) {
			filenode = open((const char*)band_file_name_a[mod_index], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			if (filenode < 0) {
				return -1;
			}

			err = write(filenode, &filecode, sizeof(filecode));
			if (err < 0) {
				close(filenode);
				return -1;
			}
			memset(file_buf, 0, sizeof(file_buf));
			err = file_encode(file_buf, sizeof(file_buf), sizeof(band_para));
			if (err < 0) {
				close(filenode);
				return -1;
			}
			err = write(filenode, file_buf, (sizeof(band_para) + MD5_CODE_SIZE));
			if ((err < 0) || (err < (s32)sizeof(unit_para))) {
				close(filenode);
				return -1;
			}
			close(filenode);
		}

		//unit file b
		err = access((const char*)band_file_name_b[mod_index], F_OK);
		if (err < 0) {
			filenode = open((const char*)band_file_name_b[mod_index], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			if (filenode < 0) {
				return -1;
			}

			err = write(filenode, &filecode, sizeof(filecode));
			if (err < 0) {
				close(filenode);
				return -1;
			}
			memset(file_buf, 0, sizeof(file_buf));
			err = file_encode(file_buf, sizeof(file_buf), sizeof(band_para));
			if (err < 0) {
				close(filenode);
				return -1;
			}
			err = write(filenode, file_buf, (sizeof(band_para) + MD5_CODE_SIZE));
			if ((err < 0) || (err < (s32)sizeof(unit_para))) {
				close(filenode);
				return -1;
			}
			close(filenode);
		}
	}

	return 0;
}
s32 band_file_read(band_para *bp, u8 mod_index)
{
	s32 err;
	s32 file_a, file_b;
	u32 file_code_a = 0;
	u32  file_code_b = 0;
	s32 read_len;
	s32 file_cur, file_code_cur;
	u8 d_buf[4] = {0};

	if (NULL == bp)
		return -1;
	//open file
	file_a = open((const char*)band_file_name_a[mod_index], O_RDONLY);
	if (file_a < 0)
		return -1;
	file_b = open((const char*)band_file_name_b[mod_index], O_RDONLY);
	if (file_b < 0) {
		close(file_a);
		return -1;
	}

	//chose file
	read_len = read(file_a, &file_code_a, sizeof(file_code_b));
	if ((read_len != sizeof(file_code_a)) || (read_len < 0)) {
		close(file_a);
		return -1;
	}

	read_len = read(file_b, &file_code_b, sizeof(file_code_b));
	if ((read_len != sizeof(file_code_b)) || (read_len < 0)) {
		close(file_a);
		close(file_b);
		return -1;
	}

	if (file_code_a > file_code_b) {
		file_code_cur = file_code_a;
		file_cur = file_a;
		//close(file_b);
		//RLDEBUG("band_file_read:choose band file a\r\n");
	} else {
		file_code_cur = file_code_b;
		file_cur = file_b;
		//close(file_a);
		//RLDEBUG("band_file_read:choose band file b\r\n");
	}
	//read
	file_code_cur = 0;
BAND_FILE_READ_TRY:
	file_code_cur++;
	memset(file_buf, 0, sizeof(file_buf));
	read_len = read(file_cur, file_buf, (sizeof(band_para) + MD5_CODE_SIZE));
	if ((read_len < 0) /*|| (read_len != (sizeof(band_para) + MD5_CODE_SIZE))*/) {
		//close(file_cur);
		RLDEBUG("band_file_read:read band file false\r\n");
		RLDEBUG("band_file_read: source len is:%d \r\n", (sizeof(band_para)));
		RLDEBUG("band_file_read: read len is:%d \r\n", (read_len));
	}
	err = file_decode(file_buf, sizeof(file_buf), read_len - MD5_CODE_SIZE);
	if (err < 0) {
		RLDEBUG("band_file_read:newer band file err: band_para size=%d,read_len=%d,read_len-md5=%d \r\n", sizeof(band_para) , read_len, read_len - MD5_CODE_SIZE);
		if (file_cur == file_a) {
			file_cur = file_b;
		} else if (file_cur == file_b) {
			file_cur = file_a;
		} else {
			goto BAND_FILE_READ_ERR;
		}
		if (file_code_cur < 2) {
			goto BAND_FILE_READ_TRY;
		} else {
			goto BAND_FILE_READ_ERR;
		}
	}

	//RLDEBUG("band_file_read:newer band file done: band_para size=%d,read_len=%d,read_len-md5=%d \r\n", sizeof(band_para) , read_len, read_len - MD5_CODE_SIZE);

	memcpy(bp, file_buf, read_len - MD5_CODE_SIZE);

	close(file_a);
	close(file_b);
	file_cur = 0;

	return 0;

BAND_FILE_READ_ERR:
	RLDEBUG("band_file_read:BAND_FILE_READ_ERR RUN\r\n");
	close(file_a);
	close(file_b);
	return -1;
}
s32 band_file_write(band_para *bp, u8 mod_index)
{
	s32 err;
	s32 file_a, file_b;
	u32 file_code_a, file_code_b;
	s32 read_len;
	s32 file_cur, file_code_cur;
	off_t file_offset = 0;

	if (NULL == bp)
		return -1;
	//open file
	file_a = open((const char*)band_file_name_a[mod_index], O_RDWR);
	if (file_a < 0) {
		RLDEBUG("band_file_write:open band file a false\r\n");
		return -1;
	}
	file_b = open((const char*)band_file_name_b[mod_index], O_RDWR);
	if (file_b < 0) {
		close(file_a);
		RLDEBUG("band_file_write:open band file b false\r\n");
		return -1;
	}

	//check file point adr
	file_offset = lseek(file_a, 0, SEEK_SET);
	file_offset = lseek(file_b, 0, SEEK_SET);

	//chose file
	read_len = read(file_a, &file_code_a, sizeof(file_code_a));
	if ((read_len != sizeof(file_code_a)) || (read_len < 0)) {
		close(file_a);
		return -1;
	}
	read_len = read(file_b, &file_code_b, sizeof(file_code_b));
	if ((read_len != sizeof(file_code_b)) || (read_len < 0)) {
		close(file_a);
		close(file_b);
		return -1;
	}

	if (file_code_a < file_code_b) {
		file_code_cur = file_code_b;
		file_cur = file_a;
		//close(file_b);
		//RLDEBUG("band_file_write:choose band file a\r\n");
	} else {
		file_code_cur = file_code_a;
		file_cur = file_b;
		//close(file_a);
		//RLDEBUG("band_file_write:choose band file b\r\n");
	}
	//write
	read_len = lseek(file_cur, 0, SEEK_SET);
	if (0 > read_len) {
		RLDEBUG("band_file_write:lseek band file false\r\n");
		goto BAND_FILE_WRITE_ERR;
	}

	file_code_cur += 1;
	read_len = write(file_cur, (const void*)&file_code_cur, sizeof(file_code_cur));

	//RLDEBUG("band_file_write:band file code is:%d\r\n", file_code_cur);
	//RLDEBUG("band_file_write:band para size is:%d\r\n", (sizeof(band_para) * MOD_NUM_IN_ONE_PCB));

	if (read_len < 0) {
		RLDEBUG("band_file_write:write band file code false\r\n");
		goto BAND_FILE_WRITE_ERR;
	}

	//RLDEBUG("band_file_write:band file code len is:%d\r\n", read_len);
	memset(file_buf, 0, sizeof(file_buf));
	memcpy(file_buf, (void*)bp, sizeof(band_para));
	err = file_encode(file_buf, sizeof(file_buf), sizeof(band_para));
	if (err < 0) {
		RLDEBUG("band_file_write:write band file code false\r\n");
		goto BAND_FILE_WRITE_ERR;
	}
	read_len = write(file_cur, (const void*)file_buf, ((sizeof(band_para)) + MD5_CODE_SIZE));
	if (read_len < 0) {
		RLDEBUG("band_file_write:write band file false\r\n");
		goto BAND_FILE_WRITE_ERR;
	}

	RLDEBUG("band_file_write:write_len=%d ,file_len=%d\r\n", read_len, ((sizeof(band_para) ) + MD5_CODE_SIZE));

	read_len = fsync(file_cur);
	if (read_len < 0)
		RLDEBUG("band_file_write:fsync band file false\r\n");

	close(file_a);
	close(file_b);
	//close(file_cur);
	return 0;

BAND_FILE_WRITE_ERR:
	RLDEBUG("band_file_write:BAND_FILE_WRITE_ERR\r\n");
	close(file_a);
	close(file_b);
	return -1;
}


/********************pcb***********************/
s32 pcb_file_init(void)
{
	s32 err;
	s32 len;
	u32 filecode = 0;
	s32 filenode;

	//test path
	err = access((const char*)para_file_path, F_OK);
	if (err < 0) {
		RLDEBUG("para path is inavilable\r\n");
		err = mkdir((const char*)para_file_path, S_IRWXU | S_IRGRP | S_IWGRP);
		if (err < 0)
		{
			RLDEBUG("mkdir para path err\r\n");
			return -1;
		}
	}

	//file a
	err = access((const char*)pcb_file_name_a, F_OK);
	if (err < 0) {
		filenode = open((const char*)pcb_file_name_a, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		if (filenode < 0) {
			return -1;
		}

		err = write(filenode, &filecode, sizeof(filecode));
		if (err < 0) {
			close(filenode);
			return -1;
		}
		memset(file_buf, 0, sizeof(file_buf));
		//init usr name
		memset((pcb_share.usr.usr[0].name), 0, sizeof(pcb_share.usr.usr[0].name));
		memset((pcb_share.usr.usr[0].pass), 0, sizeof(pcb_share.usr.usr[0].pass));
		memcpy(pcb_share.usr.usr[0].name, "User", sizeof("User"));
		memcpy(pcb_share.usr.usr[0].pass, "User", sizeof("User"));

		memset((pcb_share.usr.usr[1].name), 0, sizeof(pcb_share.usr.usr[1].name));
		memset((pcb_share.usr.usr[1].pass), 0, sizeof(pcb_share.usr.usr[1].pass));
		memcpy(pcb_share.usr.usr[1].name, "Operator", sizeof("Operator"));
		memcpy(pcb_share.usr.usr[1].pass, "Operator", sizeof("Operator"));

		memset((pcb_share.usr.usr[2].name), 0, sizeof(pcb_share.usr.usr[2].name));
		memset((pcb_share.usr.usr[2].pass), 0, sizeof(pcb_share.usr.usr[2].pass));
		memcpy(pcb_share.usr.usr[2].name, "Agent", sizeof("Agent"));
		memcpy(pcb_share.usr.usr[2].pass, "Agent", sizeof("Agent"));

		memset((pcb_share.usr.usr[3].name), 0, sizeof(pcb_share.usr.usr[3].name));
		memset((pcb_share.usr.usr[3].pass), 0, sizeof(pcb_share.usr.usr[3].pass));
		memcpy(pcb_share.usr.usr[3].name, "Factory", sizeof("Factory"));
		memcpy(pcb_share.usr.usr[3].pass, "3A7Q9644", sizeof("3A7Q9644"));

		//ip
		memset(pcb_share.net.ip, 0, sizeof(pcb_share.net.ip));
		memset(pcb_share.net.mask, 0, sizeof(pcb_share.net.mask));
		memset(pcb_share.net.gateway, 0, sizeof(pcb_share.net.gateway));
		memcpy(pcb_share.net.ip, "192.168.0.10", sizeof("192.168.0.10"));
		memcpy(pcb_share.net.mask, "255.255.255.0", sizeof("255.255.255.0"));
		memcpy(pcb_share.net.gateway, "192.168.0.1", sizeof("192.168.0.1"));

		memcpy(file_buf, &(pcb_share), sizeof(pcb_share));

		err = file_encode(file_buf, sizeof(file_buf), sizeof(pcb_share_para));
		if (err < 0) {
			close(filenode);
			return -1;
		}

		err = write(filenode, file_buf, (sizeof(pcb_share_para) + MD5_CODE_SIZE));
		if ((err < 0) || (err < ((s32)sizeof(pcb_share_para) + MD5_CODE_SIZE))) {
			close(filenode);
			return -1;
		}
		close(filenode);
	}

	//unit file b
	err = access((const char*)pcb_file_name_b, F_OK);
	if (err < 0) {
		filenode = open((const char*)pcb_file_name_b, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		if (filenode < 0) {
			return -1;
		}

		err = write(filenode, &filecode, sizeof(filecode));
		if (err < 0) {
			close(filenode);
			return -1;
		}
		memset(file_buf, 0, sizeof(file_buf));
		//init usr name
		memset((pcb_share.usr.usr[0].name), 0, sizeof(pcb_share.usr.usr[0].name));
		memset((pcb_share.usr.usr[0].pass), 0, sizeof(pcb_share.usr.usr[0].pass));
		memcpy(pcb_share.usr.usr[0].name, "User", sizeof("User"));
		memcpy(pcb_share.usr.usr[0].pass, "User", sizeof("User"));

		memset((pcb_share.usr.usr[1].name), 0, sizeof(pcb_share.usr.usr[1].name));
		memset((pcb_share.usr.usr[1].pass), 0, sizeof(pcb_share.usr.usr[1].pass));
		memcpy(pcb_share.usr.usr[1].name, "Operator", sizeof("Operator"));
		memcpy(pcb_share.usr.usr[1].pass, "Operator", sizeof("Operator"));

		memset((pcb_share.usr.usr[2].name), 0, sizeof(pcb_share.usr.usr[2].name));
		memset((pcb_share.usr.usr[2].pass), 0, sizeof(pcb_share.usr.usr[2].pass));
		memcpy(pcb_share.usr.usr[2].name, "Agent", sizeof("Agent"));
		memcpy(pcb_share.usr.usr[2].pass, "Agent", sizeof("Agent"));

		memset((pcb_share.usr.usr[3].name), 0, sizeof(pcb_share.usr.usr[3].name));
		memset((pcb_share.usr.usr[3].pass), 0, sizeof(pcb_share.usr.usr[3].pass));
		memcpy(pcb_share.usr.usr[3].name, "Factory", sizeof("Factory"));
		memcpy(pcb_share.usr.usr[3].pass, "3A7Q9644", sizeof("3A7Q9644"));

		//ip
		memset(pcb_share.net.ip, 0, sizeof(pcb_share.net.ip));
		memset(pcb_share.net.mask, 0, sizeof(pcb_share.net.mask));
		memset(pcb_share.net.gateway, 0, sizeof(pcb_share.net.gateway));
		memcpy(pcb_share.net.ip, "192.168.0.10", sizeof("192.168.0.10"));
		memcpy(pcb_share.net.mask, "255.255.255.0", sizeof("255.255.255.0"));
		memcpy(pcb_share.net.gateway, "192.168.0.1", sizeof("192.168.0.1"));

		memcpy(file_buf, &(pcb_share), sizeof(pcb_share));

		err = file_encode(file_buf, sizeof(file_buf), sizeof(pcb_share_para));
		if (err < 0) {
			close(filenode);
			return -1;
		}
		err = write(filenode, file_buf, (sizeof(pcb_share_para) + MD5_CODE_SIZE));
		if ((err < 0) || (err < ((s32)sizeof(pcb_share_para) + MD5_CODE_SIZE))) {
			close(filenode);
			return -1;
		}
		close(filenode);
	}

	return 0;
}
s32 pcb_file_read(pcb_share_para *pp)
{
	s32 err;
	s32 file_a, file_b;
	u32 file_code_a, file_code_b;
	s32 read_len;
	s32 file_cur, file_code_cur;

	if (NULL == pp)
		return -1;
	//open file
	file_a = open((const char*)pcb_file_name_a, O_RDONLY);
	if (file_a < 0) {
		RLDEBUG("open pcb file a false\r\n");
		return -1;
	}
	file_b = open((const char*)pcb_file_name_b, O_RDONLY);
	if (file_b < 0) {
		RLDEBUG("open pcb file b false\r\n");
		close(file_a);
		return -1;
	}

	//chose file
	read_len = read(file_a, &file_code_a, sizeof(file_code_a));
	if ((read_len != sizeof(file_code_a)) || (read_len < 0)) {
		RLDEBUG("pcb file a is empty\r\n");
		close(file_a);
		return -1;
	}
	read_len = read(file_b, &file_code_b, sizeof(file_code_b));
	if ((read_len != sizeof(file_code_b)) || (read_len < 0)) {
		RLDEBUG("pcb file b is empty\r\n");
		close(file_a);
		close(file_b);
		return -1;
	}

	if (file_code_a > file_code_b) {
		file_code_cur = file_code_a;
		file_cur = file_a;
		//close(file_b);
		//RLDEBUG("unit para chose file a\r\n");
	} else {
		file_code_cur = file_code_b;
		file_cur = file_b;
		//close(file_a);
		//RLDEBUG("unit para chose file b\r\n");
	}
	//read
	file_code_cur = 0;
PCB_FILE_READ_TRY:
	file_code_cur++;
	memset(file_buf, 0, sizeof(file_buf));
	read_len = read(file_cur, file_buf, (sizeof(pcb_share_para) + MD5_CODE_SIZE));
	if ((read_len < 0) || (read_len != (sizeof(pcb_share_para) + MD5_CODE_SIZE))) {
		//close(file_cur);
		RLDEBUG("pcb_file_read:read pcb file false\r\n");
	}
	err = file_decode(file_buf, sizeof(file_buf), sizeof(pcb_share_para));
	if (err < 0) {
		RLDEBUG("pcb_file_read:newer pcb file err\r\n");
		if (file_cur == file_a) {
			file_cur = file_b;
		} else if (file_cur == file_b) {
			file_cur = file_a;
		} else {
			goto PCB_FILE_READ_ERR;
		}
		if (file_code_cur < 2) {
			goto PCB_FILE_READ_TRY;
		}
	}

	memcpy(pp, file_buf, sizeof(pcb_share_para));

	//close(file_cur);
	close(file_a);
	close(file_b);
	file_cur = 0;

	return 0;

PCB_FILE_READ_ERR:
	close(file_a);
	close(file_b);
	return -1;
}
s32 pcb_file_write(pcb_share_para *pp)
{
	s32 err;
	s32 file_a, file_b;
	u32 file_code_a, file_code_b;
	s32 read_len;
	s32 file_cur, file_code_cur;
	off_t file_offset = 0;

	if (NULL == pp)
		return -1;
	//open file
	file_a = open((const char*)pcb_file_name_a, O_RDWR);
	if (file_a < 0) {
		RLDEBUG("pcb_file_write:open pcb file a false\r\n");
		return -1;
	}
	file_b = open((const char*)pcb_file_name_b, O_RDWR);
	if (file_b < 0) {
		close(file_a);
		RLDEBUG("pcb_file_write:open pcb file b false\r\n");
		return -1;
	}

	//check file point adr
	file_offset = lseek(file_a, 0, SEEK_SET);
	file_offset = lseek(file_b, 0, SEEK_SET);

	//chose file
	read_len = read(file_a, &file_code_a, sizeof(file_code_a));
	if ((read_len != sizeof(file_code_a)) || (read_len < 0)) {
		close(file_a);
		return -1;
	}
	read_len = read(file_b, &file_code_b, sizeof(file_code_b));
	if ((read_len != sizeof(file_code_b)) || (read_len < 0)) {
		close(file_a);
		close(file_b);
		return -1;
	}

	if (file_code_a < file_code_b) {
		file_code_cur = file_code_b;
		file_cur = file_a;
		//close(file_b);
		//RLDEBUG("unit_file_write:choose unit file a\r\n");
	} else {
		file_code_cur = file_code_a;
		file_cur = file_b;
		//close(file_a);
		//RLDEBUG("unit_file_write:choose unit file b\r\n");
	}
	//write
	read_len = lseek(file_cur, 0, SEEK_SET);
	if (0 > read_len) {
		RLDEBUG("pcb_file_write:lseek pcb file false\r\n");
		goto PCB_FILE_WRITE_ERR;
	}

	file_code_cur += 1;
	read_len = write(file_cur, &file_code_cur, sizeof(file_code_cur));

	//RLDEBUG("unit_file_write:unit file code is:%d\r\n", file_code_cur);
	//RLDEBUG("unit_file_write:unit para size is:%d\r\n", sizeof(unit_para));

	if (read_len < 0) {
		RLDEBUG("pcb_file_write:write pcb file code false\r\n");
		goto PCB_FILE_WRITE_ERR;
	}

	//RLDEBUG("unit_file_write:unit file code len is:%d\r\n", read_len);
	memset(file_buf, 0, sizeof(file_buf));
	memcpy(file_buf, pp, sizeof(pcb_share_para));
	err = file_encode(file_buf, sizeof(file_buf), sizeof(pcb_share_para));
	if (err < 0) {
		goto PCB_FILE_WRITE_ERR;
	}
	read_len = write(file_cur, (const void*)file_buf, (sizeof(pcb_share_para) + MD5_CODE_SIZE));
	if (read_len < 0) {
		RLDEBUG("pcb_file_write:write pcb file false\r\n");
		goto PCB_FILE_WRITE_ERR;
	}
	//RLDEBUG("unit_file_write:unit file data len is:%d\r\n", read_len);

	read_len = fsync(file_cur);
	if (read_len < 0)
		RLDEBUG("pcb_file_write:fsync pcb file false\r\n");
	close(file_cur);
	return 0;

PCB_FILE_WRITE_ERR:
	//close(file_cur);
	close(file_a);
	close(file_b);
	return -1;
}

/*****************time delay*******************/
/*
fun: time delay
para:
	m--minute
	s--second
	ms--msec
	us--microsecond
return:
 */
void timedelay(u8 m, u8 s, u16 ms, u16 us)
{
	struct timeval tv;

	tv.tv_sec = m * 60 + s + ms / 1000 + us / 1000000;
	tv.tv_usec = (ms % 1000) * 1000 + us;

	//RLDEBUG("timedelay:tv.m=%d,s=%d,ms=%d,us=%d \r\n", m, s, ms, us);
	//RLDEBUG("timedelay:tv.tv_sec=%d,tv.tv_usec=%d \r\n", tv.tv_sec, tv.tv_usec);

	select(0, NULL, NULL, NULL, &tv);
}

/*********************uart porting**************/
static s8 tty_cfg(s32 fd)
{
	struct termios tc;

	if (fd < 0)
		return -1;
	memset((void*)&tc, 0, sizeof(tc));
	//set tty default
	cfmakeraw(&tc);

	//set speed
	cfsetispeed(&tc, TTY_BAUDRATE);
	cfsetospeed(&tc, TTY_BAUDRATE);

	//
	tc.c_cflag |= CLOCAL | CREAD;
	tc.c_cflag &= ~CSIZE;
	tc.c_cflag |= CS8;
	tc.c_cflag &= ~PARENB;
	tc.c_cflag &= ~CSTOPB;

	//set waiting time & min char numbers
	//tc.c_cc[VTIME]=0;
	//tc.c_cc[VMIN] = 24;
	//clean input&output buffer
	tcflush(fd, TCIFLUSH);

	if (tcsetattr(fd, TCSANOW, &tc)) {
		RLDEBUG("tty_cfg:tcsetattr() false\r\n");
		return -1;
	}

	return 0;

}
static int monitor_tty_file;
fd_set monitor_recv_fds;
static volatile int frame_tty_flag = 0;	 //标识端口是否已经打开:0---close,1---open
s8 frame_tty_open(void)
{
	s32 err;

	frame_tty_flag = 0;
	FD_ZERO(&monitor_recv_fds);

	err = open(MONITOR_TTY_NAME, O_RDWR | O_NOCTTY);
	if (err < 0)
		return -1;
	else
		monitor_tty_file = err;

	err = tty_cfg(monitor_tty_file);
	if (err < 0)
		return -1;
	frame_tty_flag = 1;
	return 0;
}
s8 frame_tty_close(void)
{
	if (monitor_tty_file < 0)
		return -1;
	close(monitor_tty_file);

	frame_tty_flag = 0;

	return 0;
}

s8 frame_tty_send(s8 *src, u16 len)
{
	s32 err;

	if (NULL == src)
		return -1;

	if (monitor_tty_file <= 0)
		return -1;

	if (1 != frame_tty_flag)
		return -1;

	err = write(monitor_tty_file, src, len);
	if (err < 0)
		return -1;
	if (err != len)
		return -1;

	return 0;
}
s16 frame_tty_recv(s8 *src, u16 len)
{
	s32 err;
	s16 cnt = 0;
	struct timespec readwaittime;

	if (NULL == src)
		return -1;

	if (1 != frame_tty_flag)
		return -1;

	FD_ZERO(&monitor_recv_fds);
	FD_SET(monitor_tty_file, &monitor_recv_fds);

	memset((void*)&readwaittime, 0, sizeof(readwaittime));
	readwaittime.tv_sec = 0;
	readwaittime.tv_nsec = 20000;

	err = select(monitor_tty_file + 1, &monitor_recv_fds, NULL, NULL, NULL);
	if (err < 0)
		return -1;
	timedelay(0, 0, 20, 0);
	while (0 < (err = read(monitor_tty_file, (src + cnt), (len - cnt)))) {
		cnt += err;
		nanosleep(&readwaittime, NULL);
		if (cnt >= len) {
			break;
		}
	}
	if (err < 0)
		return -1;

	return cnt;
}

#include "dig_comm_def.h"
static int dig_band0_tty_file;
fd_set dig_band0_recv_fds;
struct timeval dig_band0_recv_tv;
extern dig_comm_frame dig_comm_recv_buf;
static volatile int dig_band0_tty_flag = 0;
s8 dig_band0_tty_open(void)
{
	s32 err;

	dig_band0_tty_flag = 0;
	err = open(DIG_TTY_NAME, O_RDWR | O_NOCTTY);
	if (err < 0)
		return -1;
	else
		dig_band0_tty_file = err;

	err = tty_cfg(dig_band0_tty_file);
	if (err < 0)
		return -1;

	memset((void*)&dig_band0_recv_tv, 0, sizeof(dig_band0_recv_tv));
	dig_band0_recv_tv.tv_sec = 0;
	dig_band0_recv_tv.tv_usec = 300 * 1000;

	dig_band0_tty_flag = 1;
	return 0;
}
s8 dig_band0_tty_close(void)
{
	if (dig_band0_tty_file < 0)
		return -1;
	close(dig_band0_tty_file);

	dig_band0_tty_flag = 0;

	return 0;
}

s8 dig_band0_tty_send(s8 *src, u16 len)
{
	s32 err;

	if (NULL == src)
		return -1;

	if (dig_band0_tty_file <= 0) {
		return -1;
	}
	/************************************************/
#if 0
	for (err = 0; err < len; err++)
		RLDEBUG("dig_band0_tty_send:data is:%x  \r\n", (u8)(src[err]));
#endif
	/***************************************************/
	if (1 != dig_band0_tty_flag) {
		RLDEBUG("dig_band0_tty_send:1 != dig_band0_tty_flag \r\n");
		return -1;
	}

	err = write(dig_band0_tty_file, src, len);
	if (err < 0) {
		RLDEBUG("dig_band0_tty_send:write() false \r\n");
		return -1;
	}
	if (err != len) {
		RLDEBUG("dig_band0_tty_send:err != len \r\n");
		return -1;
	}

	return 0;
}
s8 dig_band0_tty_recv(void)
{
	s32 err;
	s16 cnt = 0;
	struct timespec readwaittime;

	if (1 != dig_band0_tty_flag) {
		RLDEBUG("dig_band0_tty_recv:dig_band0_tty_flag != 0 \r\n");
		return -1;
	}

	FD_ZERO(&dig_band0_recv_fds);
	FD_SET(dig_band0_tty_file, &dig_band0_recv_fds);

	memset((void*)&dig_comm_recv_buf, 0, sizeof(dig_comm_recv_buf));
	memset((void*)&readwaittime, 0, sizeof(readwaittime));
	readwaittime.tv_sec = 0;
	readwaittime.tv_nsec = 20000;
	err = select(dig_band0_tty_file + 1, &dig_band0_recv_fds, NULL, NULL, &dig_band0_recv_tv);

	//err = select(dig_band0_tty_file + 1, &dig_band0_recv_fds, NULL, NULL, NULL);
	if (err < 0) {
		band_dynamic_para_a[0].alarm.fpga_uart = 1;
		RLDEBUG("dig_band0_tty_recv:select() time out \r\n");
		return -1;
	} else {
		band_dynamic_para_a[0].alarm.fpga_uart = 0;
	}

	//while (!FD_ISSET(dig_band0_tty_file, &dig_band0_recv_fds));

	timedelay(0, 0, 20, 0);

	while (0 < (err = read(dig_band0_tty_file, (dig_comm_recv_buf.arr + cnt), (sizeof(dig_comm_recv_buf.dig_comm_w) - cnt)))) {
		cnt += err;
		//RLDEBUG("dig_band0_tty_recv:one read len  is:%d \r\n", err);
		nanosleep(&readwaittime, NULL);
		if (cnt > (s16)(sizeof(dig_comm_recv_buf.dig_comm_w))) {
			RLDEBUG("dig_band0_tty_recv:cnt >= (s16)(sizeof(dig_comm_frame)) ---->cnt is:%d \r\n", cnt);
			break;
		}
	}
	if ((err < 0) || (!cnt)) {
		band_dynamic_para_a[0].alarm.fpga_uart = 1;
		RLDEBUG("dig_band0_tty_recv:read() false: err=%d,cnt=%d \r\n", err, cnt);
		return -1;
	} else {
		band_dynamic_para_a[0].alarm.fpga_uart = 0;
	}

	return cnt;
}

static int rs485_tty_file;
fd_set rs485_recv_fds;
struct timeval rs485_recv_tv;
u8 rs485_recv_buf[100];
static volatile int rs485_tty_flag = 0;
s8 rs485_tty_open(void)
{
	s32 err;

	rs485_tty_flag = 0;
	err = open(TTY_485NAME, O_RDWR | O_NOCTTY);
	if (err < 0)
		return -1;
	else
		rs485_tty_file = err;

	err = tty_cfg(rs485_tty_file);
	if (err < 0)
		return -1;

	memset((void*)&rs485_recv_tv, 0, sizeof(rs485_recv_tv));
	rs485_recv_tv.tv_sec = 0;
	rs485_recv_tv.tv_usec = 300 * 1000;

	rs485_tty_flag = 1;
	return 0;
}

s8 rs485_tty_close(void)
{
	if (rs485_tty_file < 0)
		return -1;
	close(rs485_tty_file);

	rs485_tty_flag = 0;

	return 0;
}
/*
*********************************************************************************************************
*	函 数 名: RS485_SEND
*	功能说明: RS485发送字符串
*	形    参：str:字符串地址   len:字符串长度
*	返 回 值: 无
*********************************************************************************************************
*/
u8 RS485_SEND(u8 * src ,u32 len)
{
	s32 err;

	if (NULL == src)
		return -1;

	if (rs485_tty_file <= 0) {
		return -1;
	}

	if (1 != rs485_tty_flag) {
		RLDEBUG("rs485_tty_send:1 != rs485_tty_flag \r\n");
		return -1;
	}

	err = write(rs485_tty_file, src, len);
	if (err < 0) {
		RLDEBUG("rs485_tty_send:write() false \r\n");
		return -1;
	}
	if (err != len) {
		RLDEBUG("rs485_tty_send:err != len \r\n");
		return -1;
	}

	return 0;
}
/*
*********************************************************************************************************
*	函 数 名: RS485_RECV
*	功能说明: RS485接收字符串
*	形    参：RBuf:字符串地址   len:字符串长度 timeout:超时时间
*	返 回 值: 无
*********************************************************************************************************
*/
#if 0
u16 RS485_RECV(u8 *RBuf,u16 len,u16 timeout)
{
	s32 err;
	s16 cnt = 0;
	struct timespec readwaittime;

	if (1 != rs485_tty_flag) {
		RLDEBUG("rs485_tty_recv:rs485_tty_flag != 0 \r\n");
		return -1;
	}

	FD_ZERO(&rs485_recv_fds);
	FD_SET(rs485_tty_file, &rs485_recv_fds);

	memset((void*)&rs485_recv_buf, 0, sizeof(rs485_recv_buf));
	memset((void*)&readwaittime, 0, sizeof(readwaittime));
	readwaittime.tv_sec = timeout;
	readwaittime.tv_nsec = 0;
	err = select(rs485_tty_file + 1, &rs485_recv_fds, NULL, NULL, &rs485_recv_tv);

	// if (err < 0) {
	// 	band_dynamic_para_a[0].alarm.fpga_uart = 1;
	// 	RLDEBUG("rs485_tty_recv:select() time out \r\n");
	// 	return -1;
	// } else {
	// 	band_dynamic_para_a[0].alarm.fpga_uart = 0;
	// }

	timedelay(0, 0, 20, 0);

	while (0 < (err = read(rs485_tty_file, (rs485_recv_buf + cnt), (sizeof(rs485_recv_buf) - cnt)))) {
		cnt += err;
		//RLDEBUG("rs485_tty_recv:one read len  is:%d \r\n", err);
		nanosleep(&readwaittime, NULL);
		if (cnt > (s16)(sizeof(rs485_recv_buf))) {
			RLDEBUG("rs485_tty_recv:cnt >= (s16)(sizeof(dig_comm_frame)) ---->cnt is:%d \r\n", cnt);
			break;
		}
	}
	// if ((err < 0) || (!cnt)) {
	// 	band_dynamic_para_a[0].alarm.fpga_uart = 1;
	// 	RLDEBUG("rs485_tty_recv:read() false: err=%d,cnt=%d \r\n", err, cnt);
	// 	return -1;
	// } else {
	// 	band_dynamic_para_a[0].alarm.fpga_uart = 0;
	// }

	return cnt;
}
#else
u16 RS485_RECV_TEST(u8 *RBuf,u16 len)
{
	s32 err;

	err = read(rs485_tty_file, RBuf, len);

	if(err != -1){
		//memcpy(RBuf, rs485_recv_buf, err);
		return err;
	}
	
	return 0;

}
#endif


/************************monitor uart task**********************/

/************************dig uart task**************************/

