/*
name:
	ad_dev_api.c
author:
	salan
date:
	20180816
description:
	ad dev common api

*/
/****************************include files*******************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include "helios.h"
#include "log.h"
#include "ad_dev_api.h"
/****************************para define*********************************/

/********************************funs****************************/
static char spi_transfer(int fd, char *dat, struct ad_spi_cfg *cfg)
{
	int ret;

	char rx[4] = {0};

	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)dat,
		.rx_buf = (unsigned long)rx,
		.len = 4,
		.delay_usecs = cfg->delay,
		.speed_hz = cfg->speed,
		.bits_per_word = cfg->bits
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (1 == ret) {
		RLDEBUG("spidev send message false\n");
	}
#if 0
	for(ret=0;ret<4;ret++){
		if(rx[ret]){
			RLDEBUG("spi_transfer rx[%d] = %x \n",ret,rx[ret]);	
		}
	}
#endif	
	return rx[0];
}

int ad_dev_write(int fd, int reg, char dat, struct ad_spi_cfg *cfg)
{
	int cnt;
	char tx[4] = {0};

	memset(tx, 0, sizeof(tx));
	tx[0] = dat;
	tx[1] = reg & 0x00ff;
	tx[2] = ((reg >> 8) & 0x007f);
#if 0
/*
	if(fd == test_fd){
		RLDEBUG("spi dat[0]=%2x \n",tx[0]);
		RLDEBUG("spi dat[1]=%2x \n",tx[1]);
		RLDEBUG("spi dat[2]=%2x \n",tx[2]);
	}
*/	
	RLDEBUG("spi dat=%6x \n",*((int*)tx));
#endif	
	spi_transfer(fd, tx, cfg);

	return 0;
}

char ad_dev_read(int fd, int reg, struct ad_spi_cfg *cfg)
{
	int cnt;
	char tx[4] = {0};

	memset(tx, 0, sizeof(tx));
	tx[0] = 0;
	tx[1] = reg & 0x00ff;
	tx[2] = (0x80 | ((reg >> 8) & 0x007f));
#if 0
	//if(fd == test_fd)
		RLDEBUG("spi dat=%6x \n",*((int*)tx));
#endif
	return spi_transfer(fd, tx, cfg);
}


