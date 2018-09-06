/*
 * File      : digmod_task.c
 * 
 *dig module
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-30      sujj      first version
*/
#include <pthread.h>
#include <unistd.h>

#include "helios.h"
#include "log.h"
#include "ad9528.h"
#include "ad9370.h"
#include "para_table.h"

pthread_t digmod_tid;
pthread_attr_t digmod_attr;


s32 fpga_load()
{
    //TODO: FPGA Load
    return 0;
}

s32 fpga_config()
{
    //TODO: FPGA Config
    return 0;
}

s32 fpga_monitor()
{
    //TODO: FPGA Monitor
    return 0;
}

s32 dig_temp_monitor()
{
    //TODO: temp monitor

    //get temp val

    //cheak temp 


    return 0;
}

s32 dig_alarm_deal()
{
    //TODO: alarm deal
    return 0;
}



void *digmod_thread(void *arg)
{
    s32 err;
    
    // RLDEBUG("start digmod thread...\r\n"); 

/*     while(fpga_load()){
        RLDEBUG("fpga load fail!");
    }

    while(fpga_config()){
        RLDEBUG("fpga config fail!");
    }

    while(fpga_monitor()){
        RLDEBUG("fpga monitor fail!");
    } */

    if(ad9528_config()){
        RLDEBUG("ad9528 config fail!");
    }
    timedelay(0, 10, 0, 0);


	//RLDEBUG("dig band0 task start!\r\n");
	dig_band0_init();

	err = dig_band0_tty_open();
	if (err < 0) {
		RLDEBUG("band0_task:dig_band0_tty_open false\r\n");
	}

BAND0_READ_FPGA_SV:
	err = dig_band0_read_sv();
	if (err < 0) {
		RLDEBUG("band0_task:dig_band0_read_sv() false\r\n");
		goto BAND0_READ_FPGA_SV;
	} else {
		RLDEBUG("band0_task:dig_band0_read_sv() sus\r\n");
		RLDEBUG("band0_task:sv is:%x \r\n", band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.sv);
	}

	//dig_read_deal();
	// dig_para_set_again(); //ad80305相关

    if(ad9370_config()){
        RLDEBUG("ad9370 config fail!");
    }

	err = 0;

    while(1){
        timedelay(0, 0, 100, 0);

        // dig_temp_monitor();

        // dig_alarm_deal();

		// dig_gain_processing(&(band_para_a[0].md_adr_t));
		dig_band0_handle();

    }

}