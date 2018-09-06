/*
name:alarm.c
author:salan
date:20170512
ver:1.0
description:
	告警处理
*/
/***************************include files****************************/
#include "helios.h"
#include "alarm_def.h"
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include <semaphore.h>
#include <math.h>
#include "porting.h"
#include "para_table.h"
#include "digital.h"
#include "log.h"
#include "module_adr_def.h"
#include "snmp_send.h"
#include "mib.h"
/****************************data def********************************/
pthread_t alarm_ts_id;
pthread_attr_t alarm_ts_attr;

u8 alarm_send_flag = 0;

/****************************funs************************************/
//#define ALARM_SW(A)	((0x01&(A))&&(0x02&(A)))
#define ALARM_SW(A)	((0x01&(A)))

/*告警搜集*/
void alarm_collect(void)
{
	//整机告警
	//

}

/*告警统计*/
void unit_alarm_census(void)
{
	//整机告警
	//门开告警
	if (unit_dynamic_para_t.alarm.door_open) {
		if ((u_alarm_t.u_alarm_cnt.door_open > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.door_open)) {
			u_alarm_t.u_alarm.door_open = 1;
		} else {
			u_alarm_t.u_alarm_cnt.door_open++;
		}
	} else {
		if (u_alarm_t.u_alarm.door_open) {
			u_alarm_t.u_alarm_cnt.door_open = 0;
			u_alarm_t.u_alarm.door_open = 0;
		}
	}
	//dc告警
	if (unit_dynamic_para_t.alarm.power_dc) {
		if ((u_alarm_t.u_alarm_cnt.power_dc > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.power_dc)) {
			u_alarm_t.u_alarm.power_dc = 1;
		} else {
			u_alarm_t.u_alarm_cnt.power_dc++;
		}
	} else {
		if (u_alarm_t.u_alarm.power_dc) {
			u_alarm_t.u_alarm_cnt.power_dc = 0;
			u_alarm_t.u_alarm.power_dc = 0;
		}
	}
	//ac告警
	if (unit_dynamic_para_t.alarm.power_ac) {
		if ((u_alarm_t.u_alarm_cnt.power_ac > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.power_ac)) {
			u_alarm_t.u_alarm.power_ac = 1;
		} else {
			u_alarm_t.u_alarm_cnt.power_ac++;
		}
	} else {
		if (u_alarm_t.u_alarm.power_ac) {
			u_alarm_t.u_alarm_cnt.power_ac = 0;
			u_alarm_t.u_alarm.power_ac = 0;
		}
	}
	//移动告警
	if (unit_dynamic_para_t.alarm.movement) {
		if ((u_alarm_t.u_alarm_cnt.movement > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.movement)) {
			u_alarm_t.u_alarm.movement = 1;
		} else {
			u_alarm_t.u_alarm_cnt.movement++;
		}
	} else {
		if (u_alarm_t.u_alarm.movement) {
			u_alarm_t.u_alarm_cnt.movement = 0;
			u_alarm_t.u_alarm.movement = 0;
		}
	}
	//gps离线告警
	if (unit_dynamic_para_t.alarm.gps_offline) {
		if ((u_alarm_t.u_alarm_cnt.gps_offline > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.gps_offline)) {
			u_alarm_t.u_alarm.gps_offline = 1;
		} else {
			u_alarm_t.u_alarm_cnt.gps_offline++;
		}
	} else {
		if (u_alarm_t.u_alarm.gps_offline) {
			u_alarm_t.u_alarm_cnt.gps_offline = 0;
			u_alarm_t.u_alarm.gps_offline = 0;
		}
	}
	//内置电池低电压告警
	if (unit_dynamic_para_t.alarm.bat_low_volt) {
		if ((u_alarm_t.u_alarm_cnt.bat_low_volt > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.bat_low_volt)) {
			u_alarm_t.u_alarm.bat_low_volt = 1;
		} else {
			u_alarm_t.u_alarm_cnt.bat_low_volt++;
		}
	} else {
		if (u_alarm_t.u_alarm.bat_low_volt) {
			u_alarm_t.u_alarm_cnt.bat_low_volt = 0;
			u_alarm_t.u_alarm.bat_low_volt = 0;
		}
	}
	//风扇告警
	if (unit_dynamic_para_t.alarm.fan_alarm_1) {
		if ((u_alarm_t.u_alarm_cnt.fan_alarm_1 > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.fan_alarm_1)) {
			u_alarm_t.u_alarm.fan_alarm_1 = 1;
		} else {
			u_alarm_t.u_alarm_cnt.fan_alarm_1++;
		}
	} else {
		if (u_alarm_t.u_alarm.fan_alarm_1) {
			u_alarm_t.u_alarm_cnt.fan_alarm_1 = 0;
			u_alarm_t.u_alarm.fan_alarm_1 = 0;
		}
	}

	if (unit_dynamic_para_t.alarm.fan_alarm_2) {
		if ((u_alarm_t.u_alarm_cnt.fan_alarm_2 > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.fan_alarm_2)) {
			u_alarm_t.u_alarm.fan_alarm_2 = 1;
		} else {
			u_alarm_t.u_alarm_cnt.fan_alarm_2++;
		}
	} else {
		if (u_alarm_t.u_alarm.fan_alarm_2) {
			u_alarm_t.u_alarm_cnt.fan_alarm_2 = 0;
			u_alarm_t.u_alarm.fan_alarm_2 = 0;
		}
	}

	if (unit_dynamic_para_t.alarm.fan_alarm_3) {
		if ((u_alarm_t.u_alarm_cnt.fan_alarm_3 > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.fan_alarm_3)) {
			u_alarm_t.u_alarm.fan_alarm_3 = 1;
		} else {
			u_alarm_t.u_alarm_cnt.fan_alarm_3++;
		}
	} else {
		if (u_alarm_t.u_alarm.fan_alarm_3) {
			u_alarm_t.u_alarm_cnt.fan_alarm_3 = 0;
			u_alarm_t.u_alarm.fan_alarm_3 = 0;
		}
	}

	if (unit_dynamic_para_t.alarm.fan_alarm_4) {
		if ((u_alarm_t.u_alarm_cnt.fan_alarm_4 > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.fan_alarm_4)) {
			u_alarm_t.u_alarm.fan_alarm_4 = 1;
		} else {
			u_alarm_t.u_alarm_cnt.fan_alarm_4++;
		}
	} else {
		if (u_alarm_t.u_alarm.fan_alarm_4) {
			u_alarm_t.u_alarm_cnt.fan_alarm_4 = 0;
			u_alarm_t.u_alarm.fan_alarm_4 = 0;
		}
	}
	//外部告警
	if (unit_dynamic_para_t.alarm.ext_alarm_1) {
		if ((u_alarm_t.u_alarm_cnt.ext_alarm_1 > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.ext_alarm_1)) {
			u_alarm_t.u_alarm.ext_alarm_1 = 1;
		} else {
			u_alarm_t.u_alarm_cnt.ext_alarm_1++;
		}
	} else {
		if (u_alarm_t.u_alarm.ext_alarm_1) {
			u_alarm_t.u_alarm_cnt.ext_alarm_1 = 0;
			u_alarm_t.u_alarm.ext_alarm_1 = 0;
		}
	}
	if (unit_dynamic_para_t.alarm.ext_alarm_2) {
		if ((u_alarm_t.u_alarm_cnt.ext_alarm_2 > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.ext_alarm_2)) {
			u_alarm_t.u_alarm.ext_alarm_2 = 1;
		} else {
			u_alarm_t.u_alarm_cnt.ext_alarm_2++;
		}
	} else {
		if (u_alarm_t.u_alarm.ext_alarm_2) {
			u_alarm_t.u_alarm_cnt.ext_alarm_2 = 0;
			u_alarm_t.u_alarm.ext_alarm_2 = 0;
		}
	}
	if (unit_dynamic_para_t.alarm.ext_alarm_3) {
		if ((u_alarm_t.u_alarm_cnt.ext_alarm_3 > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.ext_alarm_3)) {
			u_alarm_t.u_alarm.ext_alarm_3 = 1;
		} else {
			u_alarm_t.u_alarm_cnt.ext_alarm_3++;
		}
	} else {
		if (u_alarm_t.u_alarm.ext_alarm_3) {
			u_alarm_t.u_alarm_cnt.ext_alarm_3 = 0;
			u_alarm_t.u_alarm.ext_alarm_3 = 0;
		}
	}
	if (unit_dynamic_para_t.alarm.ext_alarm_4) {
		if ((u_alarm_t.u_alarm_cnt.ext_alarm_4 > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.ext_alarm_4)) {
			u_alarm_t.u_alarm.ext_alarm_4 = 1;
		} else {
			u_alarm_t.u_alarm_cnt.ext_alarm_4++;
		}
	} else {
		if (u_alarm_t.u_alarm.ext_alarm_4) {
			u_alarm_t.u_alarm_cnt.ext_alarm_4 = 0;
			u_alarm_t.u_alarm.ext_alarm_4 = 0;
		}
	}
	if (unit_dynamic_para_t.alarm.ext_alarm_5) {
		if ((u_alarm_t.u_alarm_cnt.ext_alarm_5 > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.ext_alarm_5)) {
			u_alarm_t.u_alarm.ext_alarm_5 = 1;
		} else {
			u_alarm_t.u_alarm_cnt.ext_alarm_5++;
		}
	} else {
		if (u_alarm_t.u_alarm.ext_alarm_5) {
			u_alarm_t.u_alarm_cnt.ext_alarm_5 = 0;
			u_alarm_t.u_alarm.ext_alarm_5 = 0;
		}
	}
	if (unit_dynamic_para_t.alarm.ext_alarm_6) {
		if ((u_alarm_t.u_alarm_cnt.ext_alarm_6 > ALARM_CENSUS_TIME) && ALARM_SW(unit_para_t.alarm_sw.ext_alarm_6)) {
			u_alarm_t.u_alarm.ext_alarm_6 = 1;
		} else {
			u_alarm_t.u_alarm_cnt.ext_alarm_6++;
		}
	} else {
		if (u_alarm_t.u_alarm.ext_alarm_6) {
			u_alarm_t.u_alarm_cnt.ext_alarm_6 = 0;
			u_alarm_t.u_alarm.ext_alarm_6 = 0;
		}
	}
}

void mod_alarm_census(void)
{
	u8 md_index = 0;
	s32 cnt;
	s32 alarm_census_flag = 0;

	//模块告警统计
	for (md_index = 0; md_index < MOD_NUM_IN_ONE_PCB; md_index++) {
		//cp init
		if (band_dynamic_para_a[md_index].alarm.cp_init) {
			if ((m_alarm_a[md_index].m_alarm_cnt.cp_init > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.cp_init)) {
				m_alarm_a[md_index].m_alarm.cp_init = 1;
			} else if (ALARM_SW(band_para_a[md_index].alarm_sw.cp_init)) {
				m_alarm_a[md_index].m_alarm_cnt.cp_init++;
			} else {
				m_alarm_a[md_index].m_alarm_cnt.cp_init = 0;
				m_alarm_a[md_index].m_alarm.cp_init = 0;
			}
		} else {
			if (m_alarm_a[md_index].m_alarm.cp_init) {
				m_alarm_a[md_index].m_alarm_cnt.cp_init = 0;
				m_alarm_a[md_index].m_alarm.cp_init = 0;
			}
		}
		//ap init
		if (band_dynamic_para_a[md_index].alarm.ap_init) {
			if ((m_alarm_a[md_index].m_alarm_cnt.ap_init > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.ap_init)) {
				m_alarm_a[md_index].m_alarm.ap_init = 1;
			} else if (ALARM_SW(band_para_a[md_index].alarm_sw.ap_init)) {
				m_alarm_a[md_index].m_alarm_cnt.ap_init++;
			} else {
				m_alarm_a[md_index].m_alarm_cnt.ap_init = 0;
				m_alarm_a[md_index].m_alarm.ap_init = 0;
			}
		} else {
			if (m_alarm_a[md_index].m_alarm.ap_init) {
				m_alarm_a[md_index].m_alarm_cnt.ap_init = 0;
				m_alarm_a[md_index].m_alarm.ap_init = 0;
			}
		}
		//fpga load
		if (band_dynamic_para_a[md_index].alarm.fpga_loaded) {
			if ((m_alarm_a[md_index].m_alarm_cnt.fpga_loaded > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.fpga_loaded)) {
				m_alarm_a[md_index].m_alarm.fpga_loaded = 1;
			} else if (ALARM_SW(band_para_a[md_index].alarm_sw.fpga_loaded)) {
				m_alarm_a[md_index].m_alarm_cnt.fpga_loaded++;
			} else {
				m_alarm_a[md_index].m_alarm_cnt.fpga_loaded = 0;
				m_alarm_a[md_index].m_alarm.fpga_loaded = 0;
			}
		} else {
			if (m_alarm_a[md_index].m_alarm.fpga_loaded) {
				m_alarm_a[md_index].m_alarm_cnt.fpga_loaded = 0;
				m_alarm_a[md_index].m_alarm.fpga_loaded = 0;
			}
		}
		//fpga uart
		if (band_dynamic_para_a[md_index].alarm.fpga_uart) {
			if ((m_alarm_a[md_index].m_alarm_cnt.fpga_uart > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.fpga_uart)) {
				m_alarm_a[md_index].m_alarm.fpga_uart = 1;
			} else if (ALARM_SW(band_para_a[md_index].alarm_sw.fpga_uart)) {
				m_alarm_a[md_index].m_alarm_cnt.fpga_uart++;
			} else {
				m_alarm_a[md_index].m_alarm_cnt.fpga_uart = 0;
				m_alarm_a[md_index].m_alarm.fpga_uart = 0;
			}
		} else {
			if (m_alarm_a[md_index].m_alarm.fpga_uart) {
				m_alarm_a[md_index].m_alarm_cnt.fpga_uart = 0;
				m_alarm_a[md_index].m_alarm.fpga_uart = 0;
			}
		}
		//fpga spi
		if (band_dynamic_para_a[md_index].alarm.fpga_spi) {
			if ((m_alarm_a[md_index].m_alarm_cnt.fpga_spi > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.fpga_spi)) {
				m_alarm_a[md_index].m_alarm.fpga_spi = 1;
			} else if (ALARM_SW(band_para_a[md_index].alarm_sw.fpga_spi)) {
				m_alarm_a[md_index].m_alarm_cnt.fpga_spi++;
			} else {
				m_alarm_a[md_index].m_alarm_cnt.fpga_spi = 0;
				m_alarm_a[md_index].m_alarm.fpga_spi = 0;
			}
		} else {
			if (m_alarm_a[md_index].m_alarm.fpga_spi) {
				m_alarm_a[md_index].m_alarm_cnt.fpga_spi = 0;
				m_alarm_a[md_index].m_alarm.fpga_spi = 0;
			}
		}

		//ad80305-1
		if (band_dynamic_para_a[md_index].alarm.ad80305_1) {
			if ((m_alarm_a[md_index].m_alarm_cnt.ad80305_1 > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.ad80305_1)) {
				m_alarm_a[md_index].m_alarm.ad80305_1 = 1;
			} else if (ALARM_SW(band_para_a[md_index].alarm_sw.ad80305_1)) {
				m_alarm_a[md_index].m_alarm_cnt.ad80305_1++;
			} else {
				m_alarm_a[md_index].m_alarm_cnt.ad80305_1 = 0;
				m_alarm_a[md_index].m_alarm.ad80305_1 = 0;
			}
		} else {
			if (m_alarm_a[md_index].m_alarm.ad80305_1) {
				m_alarm_a[md_index].m_alarm_cnt.ad80305_1 = 0;
				m_alarm_a[md_index].m_alarm.ad80305_1 = 0;
			}
		}
		//ad80305-2
		if (band_dynamic_para_a[md_index].alarm.ad80305_2) {
			if ((m_alarm_a[md_index].m_alarm_cnt.ad80305_2 > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.ad80305_2)) {
				m_alarm_a[md_index].m_alarm.ad80305_2 = 1;
			} else if (ALARM_SW(band_para_a[md_index].alarm_sw.ad80305_2)) {
				m_alarm_a[md_index].m_alarm_cnt.ad80305_2++;
			} else {
				m_alarm_a[md_index].m_alarm_cnt.ad80305_2 = 0;
				m_alarm_a[md_index].m_alarm.ad80305_2 = 0;
			}
		} else {
			if (m_alarm_a[md_index].m_alarm.ad80305_2) {
				m_alarm_a[md_index].m_alarm_cnt.ad80305_2 = 0;
				m_alarm_a[md_index].m_alarm.ad80305_2 = 0;
			}
		}

		alarm_census_flag = 0;
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			//dl pin lower power
			if (band_dynamic_para_a[md_index].ch_rf_t.dl[cnt].pin_lp) {
				alarm_census_flag = 1;
				if ((m_alarm_a[md_index].m_alarm_cnt.ch_pin_dl_lp[cnt] > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.ch_pin_dl_lp[0])) {
					m_alarm_a[md_index].m_alarm.ch_pin_dl_lp[cnt] = 1;
				} else if (ALARM_SW(band_para_a[md_index].alarm_sw.ch_pin_dl_lp[cnt])) {
					m_alarm_a[md_index].m_alarm_cnt.ch_pin_dl_lp[cnt] += 1;
				} else if (!alarm_census_flag) {
					m_alarm_a[md_index].m_alarm_cnt.ch_pin_dl_lp[cnt] = 0;
					m_alarm_a[md_index].m_alarm.ch_pin_dl_lp[cnt] = 0;
				}
			} else if (!alarm_census_flag) {
				if (m_alarm_a[md_index].m_alarm.ch_pin_dl_lp[cnt]) {
					m_alarm_a[md_index].m_alarm_cnt.ch_pin_dl_lp[cnt] = 0;
					m_alarm_a[md_index].m_alarm.ch_pin_dl_lp[cnt] = 0;
				}
			}

			//dl pin over power
			if (band_dynamic_para_a[md_index].ch_rf_t.dl[cnt].pin_op) {
				//RLDEBUG("mod_alarm_census: dl_pin_op[cnt=%d]=%d \r\n", cnt, band_dynamic_para_a[md_index].ch_rf_t.dl[cnt].pin_op);

				alarm_census_flag = 1;
				if ((m_alarm_a[md_index].m_alarm_cnt.ch_pin_dl_op[cnt] > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.ch_pin_dl_op[0])) {
					m_alarm_a[md_index].m_alarm.ch_pin_dl_op[cnt] = 1;
					//RLDEBUG("mod_alarm_census: dl_pin_op=2 \r\n");

				} else if (ALARM_SW(band_para_a[md_index].alarm_sw.ch_pin_dl_op[0])) {
					m_alarm_a[md_index].m_alarm_cnt.ch_pin_dl_op[cnt] += 1;
					//RLDEBUG("mod_alarm_census: dl_pin_op=1 \r\n");

				} else if (!alarm_census_flag) {
					m_alarm_a[md_index].m_alarm_cnt.ch_pin_dl_op[cnt] = 0;
					m_alarm_a[md_index].m_alarm.ch_pin_dl_op[cnt] = 0;

					//RLDEBUG("mod_alarm_census: dl_pin_op=3 \r\n");
				}
			} else if (!alarm_census_flag) {
				if (m_alarm_a[md_index].m_alarm.ch_pin_dl_op[cnt]) {
					m_alarm_a[md_index].m_alarm_cnt.ch_pin_dl_op[cnt] = 0;
					m_alarm_a[md_index].m_alarm.ch_pin_dl_op[cnt] = 0;

					//RLDEBUG("mod_alarm_census: dl_pin_op=4 \r\n");
				}
			}
		}

		alarm_census_flag = 0;
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			//ul pin lower power
			if (band_dynamic_para_a[md_index].ch_rf_t.ul[cnt].pin_lp) {
				alarm_census_flag = 0;
				if ((m_alarm_a[md_index].m_alarm_cnt.ch_pin_ul_lp[cnt] > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.ch_pin_ul_lp[0])) {
					m_alarm_a[md_index].m_alarm.ch_pin_ul_lp[cnt] = 1;
				} else if (ALARM_SW(band_para_a[md_index].alarm_sw.ch_pin_ul_lp[cnt])) {
					m_alarm_a[md_index].m_alarm_cnt.ch_pin_ul_lp[cnt] += 1;
				} else if (!alarm_census_flag) {
					m_alarm_a[md_index].m_alarm_cnt.ch_pin_ul_lp[cnt] = 0;
					m_alarm_a[md_index].m_alarm.ch_pin_ul_lp[cnt] = 0;
				}
			} else if (!alarm_census_flag) {
				if (m_alarm_a[md_index].m_alarm.ch_pin_ul_lp[cnt]) {
					m_alarm_a[md_index].m_alarm_cnt.ch_pin_ul_lp[cnt] = 0;
					m_alarm_a[md_index].m_alarm.ch_pin_ul_lp[cnt] = 0;
				}
			}

			//ul pin over power
			if (band_dynamic_para_a[md_index].ch_rf_t.ul[cnt].pin_op) {
				//RLDEBUG("mod_alarm_census: ul_pin_op[cnt=%d]=%d \r\n", cnt, band_dynamic_para_a[md_index].ch_rf_t.ul[cnt].pin_op);
				if ((m_alarm_a[md_index].m_alarm_cnt.ch_pin_ul_op[cnt] > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.ch_pin_ul_op[0])) {
					m_alarm_a[md_index].m_alarm.ch_pin_ul_op[cnt] = 1;
					//RLDEBUG("mod_alarm_census: ul_pin_op=2 \r\n");

				} else if (ALARM_SW(band_para_a[md_index].alarm_sw.ch_pin_ul_op[0])) {
					m_alarm_a[md_index].m_alarm_cnt.ch_pin_ul_op[cnt] += 1;
					//RLDEBUG("mod_alarm_census: ul_pin_op=1 \r\n");

				} else if (!alarm_census_flag) {
					m_alarm_a[md_index].m_alarm_cnt.ch_pin_ul_op[cnt] = 0;
					m_alarm_a[md_index].m_alarm.ch_pin_ul_op[cnt] = 0;
					//RLDEBUG("mod_alarm_census: ul_pin_op=3 \r\n");

				}
			} else if (!alarm_census_flag) {
				if (m_alarm_a[md_index].m_alarm.ch_pin_ul_op[cnt]) {
					m_alarm_a[md_index].m_alarm_cnt.ch_pin_ul_op[cnt] = 0;
					m_alarm_a[md_index].m_alarm.ch_pin_ul_op[cnt] = 0;
					//RLDEBUG("mod_alarm_census: ul_pin_op=4 \r\n");

				}
			}
		}

		//over temp
		if (band_dynamic_para_a[md_index].md_dynamic_sundry.temperature > band_para_a[md_index].md_basic.temp_ctrl.temp_over_th) {
			if ((m_alarm_a[md_index].m_alarm_cnt.temp_h > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.temp_h)) {
				m_alarm_a[md_index].m_alarm.temp_h = 1;
			} else if (ALARM_SW(band_para_a[md_index].alarm_sw.temp_h)) {
				m_alarm_a[md_index].m_alarm_cnt.temp_h++;
			} else {
				m_alarm_a[md_index].m_alarm_cnt.temp_h = 0;
				m_alarm_a[md_index].m_alarm.temp_h = 0;
			}
		} else if (band_dynamic_para_a[md_index].md_dynamic_sundry.temperature < band_para_a[md_index].md_basic.temp_ctrl.temp_over_clean) {
			if (m_alarm_a[md_index].m_alarm.temp_h) {
				m_alarm_a[md_index].m_alarm_cnt.temp_h = 0;
				m_alarm_a[md_index].m_alarm.temp_h = 0;
			}
		}

		//lower temp
		if (band_dynamic_para_a[md_index].md_dynamic_sundry.temperature < band_para_a[md_index].md_basic.temp_ctrl.temp_lower_th) {
			if ((m_alarm_a[md_index].m_alarm_cnt.temp_l > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.temp_l)) {
				m_alarm_a[md_index].m_alarm.temp_l = 1;
			} else if (ALARM_SW(band_para_a[md_index].alarm_sw.temp_l)) {
				m_alarm_a[md_index].m_alarm_cnt.temp_l++;
			} else {
				m_alarm_a[md_index].m_alarm_cnt.temp_l = 0;
				m_alarm_a[md_index].m_alarm.temp_l = 0;
			}
		} else if (band_dynamic_para_a[md_index].md_dynamic_sundry.temperature > band_para_a[md_index].md_basic.temp_ctrl.temp_lower_clean) {
			if (m_alarm_a[md_index].m_alarm.temp_l) {
				m_alarm_a[md_index].m_alarm_cnt.temp_l = 0;
				m_alarm_a[md_index].m_alarm.temp_l = 0;
			}
		}

		//iso alarm
		if (band_dynamic_para_a[md_index].md_dynamic_sundry.dig_dynamic_sundry.ics_iso_dl < band_para_a[md_index].md_sundry.dig_sundry.iso_alarm_th) {
			if ((m_alarm_a[md_index].m_alarm_cnt.iso_alarm > ALARM_CENSUS_TIME) && ALARM_SW(band_para_a[md_index].alarm_sw.iso_alarm)) {
				m_alarm_a[md_index].m_alarm.iso_alarm = 1;
			} else if (ALARM_SW(band_para_a[md_index].alarm_sw.iso_alarm)) {
				m_alarm_a[md_index].m_alarm_cnt.iso_alarm++;
			} else {
				m_alarm_a[md_index].m_alarm_cnt.iso_alarm = 0;
				m_alarm_a[md_index].m_alarm.iso_alarm = 0;
			}
		} else if (band_dynamic_para_a[md_index].md_dynamic_sundry.dig_dynamic_sundry.ics_iso_dl > (band_para_a[md_index].md_sundry.dig_sundry.iso_alarm_th + 1)) {
			if (m_alarm_a[md_index].m_alarm.iso_alarm) {
				m_alarm_a[md_index].m_alarm_cnt.iso_alarm = 0;
				m_alarm_a[md_index].m_alarm.iso_alarm = 0;
			}
		}

	}
}

void exmod_alarm_census(void)
{
	u8 md_index = 0;
	s16 i;
	s32 alarm_census_flag = 0;

	for (md_index = 0; md_index < MONITOR_MOD_NUM; md_index++) {
		if(exmod_para_a[md_index].md_adr_t.mod_band == 0) continue;

		//ul over power
		if (exmod_dynamic_para_a[md_index].alarm.ch_pin_ul_op[0]) {
			if (!exmod_alarm_a[md_index].m_alarm.ch_pin_ul_op[0] && (exmod_alarm_a[md_index].m_alarm_cnt.ch_pin_ul_op[0] > ALARM_CENSUS_TIME) && ALARM_SW(exmod_para_a[md_index].alarm_sw.ch_pin_ul_op[0])) {
				exmod_alarm_a[md_index].m_alarm.ch_pin_ul_op[0] = 1;
				alarm_send_flag = TRUE;
				if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_UL ){
					snmp_msg_send(MIB_ALARM_UL_PA1_OVER_POWER, 1);
				}
			} else if (ALARM_SW(exmod_para_a[md_index].alarm_sw.ch_pin_ul_op[0])) {
				exmod_alarm_a[md_index].m_alarm_cnt.ch_pin_ul_op[0]++;
			} else {
				exmod_alarm_a[md_index].m_alarm_cnt.ch_pin_ul_op[0] = 0;
				exmod_alarm_a[md_index].m_alarm.ch_pin_ul_op[0] = 0;
			}
		} else if (exmod_dynamic_para_a[md_index].alarm.ch_pin_ul_op[0] == 0) {
			if (exmod_alarm_a[md_index].m_alarm.ch_pin_ul_op[0]) {
				exmod_alarm_a[md_index].m_alarm_cnt.ch_pin_ul_op[0] = 0;
				exmod_alarm_a[md_index].m_alarm.ch_pin_ul_op[0] = 0;
				alarm_send_flag = TRUE;
				if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_UL ){
					snmp_msg_send(MIB_ALARM_UL_PA1_OVER_POWER, 0);
				}
				
			}
		}
		//dl over power
		if (exmod_dynamic_para_a[md_index].alarm.ch_pin_dl_op[0]) {
			if (!exmod_alarm_a[md_index].m_alarm.ch_pin_dl_op[0] && (exmod_alarm_a[md_index].m_alarm_cnt.ch_pin_dl_op[0] > ALARM_CENSUS_TIME) && ALARM_SW(exmod_para_a[md_index].alarm_sw.ch_pin_dl_op[0])) {
				exmod_alarm_a[md_index].m_alarm.ch_pin_dl_op[0] = 1;
				alarm_send_flag = TRUE;
				if(exmod_para_a[md_index].md_adr_t.mod_type == MOD_TYPE_DIG){
					snmp_msg_send(MIB_ALARM_INPUT_OVER_POWER, 1);
				}else if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_DL ){
					snmp_msg_send(MIB_ALARM_DL_PA1_OVER_POWER, 1);
				}

			} else if (ALARM_SW(exmod_para_a[md_index].alarm_sw.ch_pin_dl_op[0])) {
				exmod_alarm_a[md_index].m_alarm_cnt.ch_pin_dl_op[0]++;
			} else {
				exmod_alarm_a[md_index].m_alarm_cnt.ch_pin_dl_op[0] = 0;
				exmod_alarm_a[md_index].m_alarm.ch_pin_dl_op[0] = 0;
			}
		} else if (exmod_dynamic_para_a[md_index].alarm.ch_pin_dl_op[0] == 0) {
			if (exmod_alarm_a[md_index].m_alarm.ch_pin_dl_op[0]) {
				exmod_alarm_a[md_index].m_alarm_cnt.ch_pin_dl_op[0] = 0;
				exmod_alarm_a[md_index].m_alarm.ch_pin_dl_op[0] = 0;
				alarm_send_flag = TRUE;
				if(exmod_para_a[md_index].md_adr_t.mod_type == MOD_TYPE_DIG){
					snmp_msg_send(MIB_ALARM_INPUT_OVER_POWER, 0);
				}else if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_DL ){
					snmp_msg_send(MIB_ALARM_DL_PA1_OVER_POWER, 0);
				}

			}
		}
		//temp_h
		if (exmod_dynamic_para_a[md_index].alarm.temp_h) {
			if (!exmod_alarm_a[md_index].m_alarm.temp_h && (exmod_alarm_a[md_index].m_alarm_cnt.temp_h > ALARM_CENSUS_TIME) && ALARM_SW(exmod_para_a[md_index].alarm_sw.temp_h)) {
				exmod_alarm_a[md_index].m_alarm.temp_h = 1;
				alarm_send_flag = TRUE;
				if(exmod_para_a[md_index].md_adr_t.mod_type == MOD_TYPE_DIG){
					snmp_msg_send(MIB_ALARM_OVER_TEMP, 1);
				}else if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_UL )
					snmp_msg_send(MIB_ALARM_UL_PA1_OVER_TEMP, 1);
				else if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_DL ){
					snmp_msg_send(MIB_ALARM_DL_PA1_OVER_TEMP, 1);
				}
			} else if (ALARM_SW(exmod_para_a[md_index].alarm_sw.temp_h)) {
				exmod_alarm_a[md_index].m_alarm_cnt.temp_h++;
			} else {
				exmod_alarm_a[md_index].m_alarm_cnt.temp_h = 0;
				exmod_alarm_a[md_index].m_alarm.temp_h = 0;
			}
		} else if (exmod_dynamic_para_a[md_index].alarm.temp_h == 0) {
			if (exmod_alarm_a[md_index].m_alarm.temp_h) {
				exmod_alarm_a[md_index].m_alarm_cnt.temp_h = 0;
				exmod_alarm_a[md_index].m_alarm.temp_h = 0;
				alarm_send_flag = TRUE;
				if(exmod_para_a[md_index].md_adr_t.mod_type == MOD_TYPE_DIG){
					snmp_msg_send(MIB_ALARM_OVER_TEMP, 0);
				}else if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_UL )
					snmp_msg_send(MIB_ALARM_UL_PA1_OVER_TEMP, 0);
				else if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_DL ){
					snmp_msg_send(MIB_ALARM_DL_PA1_OVER_TEMP, 0);
				}
			}
		}
/* 		//temp_l
		if (exmod_dynamic_para_a[md_index].alarm.temp_l) {
			if (!exmod_alarm_a[md_index].m_alarm.temp_l && (exmod_alarm_a[md_index].m_alarm_cnt.temp_l > ALARM_CENSUS_TIME) && ALARM_SW(exmod_para_a[md_index].alarm_sw.temp_l)) {
				exmod_alarm_a[md_index].m_alarm.temp_l = 1;
				alarm_send_flag = TRUE;
			} else if (ALARM_SW(exmod_para_a[md_index].alarm_sw.temp_l)) {
				exmod_alarm_a[md_index].m_alarm_cnt.temp_l++;
			} else {
				exmod_alarm_a[md_index].m_alarm_cnt.temp_l = 0;
				exmod_alarm_a[md_index].m_alarm.temp_l = 0;
			}
		} else if (exmod_dynamic_para_a[md_index].alarm.temp_l == 0) {
			if (exmod_alarm_a[md_index].m_alarm.temp_l) {
				exmod_alarm_a[md_index].m_alarm_cnt.temp_l = 0;
				exmod_alarm_a[md_index].m_alarm.temp_l = 0;
				alarm_send_flag = TRUE;
			}
		} */
		//pa1
		if (exmod_dynamic_para_a[md_index].alarm.pa1) {
			if (!exmod_alarm_a[md_index].m_alarm.pa1 && (exmod_alarm_a[md_index].m_alarm_cnt.pa1 > ALARM_CENSUS_TIME) && ALARM_SW(exmod_para_a[md_index].alarm_sw.pa1)) {
				exmod_alarm_a[md_index].m_alarm.pa1 = 1;
				alarm_send_flag = TRUE;
				if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_UL )
					snmp_msg_send(MIB_ALARM_UL_PA1_FAULT, 1);
				else if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_DL ){
					snmp_msg_send(MIB_ALARM_DL_PA1_FAULT, 1);
				}
			} else if (ALARM_SW(exmod_para_a[md_index].alarm_sw.pa1)) {
				exmod_alarm_a[md_index].m_alarm_cnt.pa1++;
			} else {
				exmod_alarm_a[md_index].m_alarm_cnt.pa1 = 0;
				exmod_alarm_a[md_index].m_alarm.pa1 = 0;
			}
		} else if (exmod_dynamic_para_a[md_index].alarm.pa1 == 0) {
			if (exmod_alarm_a[md_index].m_alarm.pa1) {
				exmod_alarm_a[md_index].m_alarm_cnt.pa1 = 0;
				exmod_alarm_a[md_index].m_alarm.pa1 = 0;
				alarm_send_flag = TRUE;
				if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_UL )
					snmp_msg_send(MIB_ALARM_UL_PA1_FAULT, 0);
				else if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_DL ){
					snmp_msg_send(MIB_ALARM_DL_PA1_FAULT, 0);
				}
			}
		}
/* 		//pout_pre
		if (exmod_dynamic_para_a[md_index].alarm.pout_pre) {
			if (!exmod_alarm_a[md_index].m_alarm.pout_pre && (exmod_alarm_a[md_index].m_alarm_cnt.pout_pre > ALARM_CENSUS_TIME) && ALARM_SW(exmod_para_a[md_index].alarm_sw.pout_pre)) {
				exmod_alarm_a[md_index].m_alarm.pout_pre = 1;
				alarm_send_flag = TRUE;
			} else if (ALARM_SW(exmod_para_a[md_index].alarm_sw.pout_pre)) {
				exmod_alarm_a[md_index].m_alarm_cnt.pout_pre++;
			} else {
				exmod_alarm_a[md_index].m_alarm_cnt.pout_pre = 0;
				exmod_alarm_a[md_index].m_alarm.pout_pre = 0;
			}
		} else if (exmod_dynamic_para_a[md_index].alarm.pout_pre == 0) {
			if (exmod_alarm_a[md_index].m_alarm.pout_pre) {
				exmod_alarm_a[md_index].m_alarm_cnt.pout_pre = 0;
				exmod_alarm_a[md_index].m_alarm.pout_pre = 0;
				alarm_send_flag = TRUE;
			}
		} */
		//驻波比告警(回波损耗告警)
		if (exmod_dynamic_para_a[md_index].alarm.rl) {
			if (!exmod_alarm_a[md_index].m_alarm.rl && (exmod_alarm_a[md_index].m_alarm_cnt.rl > ALARM_CENSUS_TIME) && ALARM_SW(exmod_para_a[md_index].alarm_sw.rl)) {
				exmod_alarm_a[md_index].m_alarm.rl = 1;
				alarm_send_flag = TRUE;
				if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_UL )
					snmp_msg_send(MIB_ALARM_UL_PA1_VSWR, 1);
				else if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_DL ){
					snmp_msg_send(MIB_ALARM_DL_PA1_VSWR, 1);
				}
			} else if (ALARM_SW(exmod_para_a[md_index].alarm_sw.rl)) {
				exmod_alarm_a[md_index].m_alarm_cnt.rl++;
			} else {
				exmod_alarm_a[md_index].m_alarm_cnt.rl = 0;
				exmod_alarm_a[md_index].m_alarm.rl = 0;
			}
		} else if (exmod_dynamic_para_a[md_index].alarm.rl == 0) {
			if (exmod_alarm_a[md_index].m_alarm.rl) {
				exmod_alarm_a[md_index].m_alarm_cnt.rl = 0;
				exmod_alarm_a[md_index].m_alarm.rl = 0;
				alarm_send_flag = TRUE;
				if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_UL )
					snmp_msg_send(MIB_ALARM_UL_PA1_VSWR, 0);
				else if(exmod_para_a[md_index].md_adr_t.mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_DL ){
					snmp_msg_send(MIB_ALARM_DL_PA1_VSWR, 0);
				}
			}
		}
		//隔离度告警
		if (exmod_dynamic_para_a[md_index].alarm.iso_alarm) {
			if (!exmod_alarm_a[md_index].m_alarm.iso_alarm && (exmod_alarm_a[md_index].m_alarm_cnt.iso_alarm > ALARM_CENSUS_TIME) && ALARM_SW(exmod_para_a[md_index].alarm_sw.iso_alarm)) {
				exmod_alarm_a[md_index].m_alarm.iso_alarm = 1;
				alarm_send_flag = TRUE;
				snmp_msg_send(MIB_ALARM_ISOLATION, 1);
			} else if (ALARM_SW(exmod_para_a[md_index].alarm_sw.iso_alarm)) {
				exmod_alarm_a[md_index].m_alarm_cnt.iso_alarm++;
			} else {
				exmod_alarm_a[md_index].m_alarm_cnt.iso_alarm = 0;
				exmod_alarm_a[md_index].m_alarm.iso_alarm = 0;
			}
		} else if (exmod_dynamic_para_a[md_index].alarm.iso_alarm == 0) {
			if (exmod_alarm_a[md_index].m_alarm.iso_alarm) {
				exmod_alarm_a[md_index].m_alarm_cnt.iso_alarm = 0;
				exmod_alarm_a[md_index].m_alarm.iso_alarm = 0;
				alarm_send_flag = TRUE;
				snmp_msg_send(MIB_ALARM_ISOLATION, 0);
			}
		}
		//vco1
		if (exmod_dynamic_para_a[md_index].alarm.ad80305_1) {
			if (!exmod_alarm_a[md_index].m_alarm.ad80305_1 && (exmod_alarm_a[md_index].m_alarm_cnt.ad80305_1 > ALARM_CENSUS_TIME) && ALARM_SW(exmod_para_a[md_index].alarm_sw.ad80305_1)) {
				exmod_alarm_a[md_index].m_alarm.ad80305_1 = 1;
				alarm_send_flag = TRUE;
				snmp_msg_send(MIB_ALARM_VCO1_UNLOCK, 1);
			} else if (ALARM_SW(exmod_para_a[md_index].alarm_sw.ad80305_1)) {
				exmod_alarm_a[md_index].m_alarm_cnt.ad80305_1++;
			} else {
				exmod_alarm_a[md_index].m_alarm_cnt.ad80305_1 = 0;
				exmod_alarm_a[md_index].m_alarm.ad80305_1 = 0;
			}
		} else if (exmod_dynamic_para_a[md_index].alarm.ad80305_1 == 0) {
			if (exmod_alarm_a[md_index].m_alarm.ad80305_1) {
				exmod_alarm_a[md_index].m_alarm_cnt.ad80305_1 = 0;
				exmod_alarm_a[md_index].m_alarm.ad80305_1 = 0;
				alarm_send_flag = TRUE;
				snmp_msg_send(MIB_ALARM_VCO1_UNLOCK, 0);
			}
		}
		//vco2
		if (exmod_dynamic_para_a[md_index].alarm.ad80305_2) {
			if (!exmod_alarm_a[md_index].m_alarm.ad80305_2 && (exmod_alarm_a[md_index].m_alarm_cnt.ad80305_2 > ALARM_CENSUS_TIME) && ALARM_SW(exmod_para_a[md_index].alarm_sw.ad80305_2)) {
				exmod_alarm_a[md_index].m_alarm.ad80305_2 = 1;
				alarm_send_flag = TRUE;
				snmp_msg_send(MIB_ALARM_VCO2_UNLOCK, 1);
			} else if (ALARM_SW(exmod_para_a[md_index].alarm_sw.ad80305_2)) {
				exmod_alarm_a[md_index].m_alarm_cnt.ad80305_2++;
			} else {
				exmod_alarm_a[md_index].m_alarm_cnt.ad80305_2 = 0;
				exmod_alarm_a[md_index].m_alarm.ad80305_2 = 0;
			}
		} else if (exmod_dynamic_para_a[md_index].alarm.ad80305_2 == 0) {
			if (exmod_alarm_a[md_index].m_alarm.ad80305_2) {
				exmod_alarm_a[md_index].m_alarm_cnt.ad80305_2 = 0;
				exmod_alarm_a[md_index].m_alarm.ad80305_2 = 0;
				alarm_send_flag = TRUE;
				snmp_msg_send(MIB_ALARM_VCO2_UNLOCK, 0);
			}
		}
		//自激告警
		if (exmod_dynamic_para_a[md_index].alarm.self_excited) {
			if (!exmod_alarm_a[md_index].m_alarm.self_excited && (exmod_alarm_a[md_index].m_alarm_cnt.self_excited > ALARM_CENSUS_TIME) && ALARM_SW(exmod_para_a[md_index].alarm_sw.self_excited)) {
				exmod_alarm_a[md_index].m_alarm.self_excited = 1;
				alarm_send_flag = TRUE;
				snmp_msg_send(MIB_ALARM_SELF_OSCILLATION, 1);
			} else if (ALARM_SW(exmod_para_a[md_index].alarm_sw.self_excited)) {
				exmod_alarm_a[md_index].m_alarm_cnt.self_excited++;
			} else {
				exmod_alarm_a[md_index].m_alarm_cnt.self_excited = 0;
				exmod_alarm_a[md_index].m_alarm.self_excited = 0;
			}
		} else if (exmod_dynamic_para_a[md_index].alarm.self_excited == 0) {
			if (exmod_alarm_a[md_index].m_alarm.self_excited) {
				exmod_alarm_a[md_index].m_alarm_cnt.self_excited = 0;
				exmod_alarm_a[md_index].m_alarm.self_excited = 0;
				alarm_send_flag = TRUE;
				snmp_msg_send(MIB_ALARM_SELF_OSCILLATION, 0);
			}
		}
	}
}

s32 alarm_send(void)
{
	return 0;
}

void alarm_deal_rep(void)
{
	s32 err;
	s16 i;
	u8 md_index = 0;
	u8 is_alarm = 0;

	if(alarm_send_flag){
		for (md_index = 0; md_index < MONITOR_MOD_NUM; md_index++) {
			if(exmod_para_a[md_index].md_adr_t.mod_band == 0) continue;

			switch(exmod_para_a[md_index].md_adr_t.mod_type){
				case MOD_TYPE_DIG:
					if (exmod_alarm_a[md_index].m_alarm.temp_h){
						//
						is_alarm = 1;
					}else if (exmod_alarm_a[md_index].m_alarm.ad80305_1){
						//VCO1
						is_alarm = 1;
					}else if (exmod_alarm_a[md_index].m_alarm.ad80305_2){
						//VCO2 
						is_alarm = 1;
					}else if (exmod_alarm_a[md_index].m_alarm.self_excited){
						//self_excited 
						is_alarm = 1;
					}else if (exmod_alarm_a[md_index].m_alarm.ch_pin_dl_op[0]){
						//DL  Over power
						is_alarm = 1;
						// for(i=0; i<FREQ_CHANNEL_NUMS_MAX; ++i){
						// 	if(exmod_para_a[md_index].ch_info_t.dl[i].sw){
						// 		exmod_alarm_a[md_index].m_alarm.ch_pin_dl_op[i] = 1;
						// 	}
						// }
					}

					// for(i=0; i<FREQ_CHANNEL_NUMS_MAX; ++i){
						
					// 	exmod_alarm_a[md_index].m_alarm.ch_pin_dl_op[i] = 0;
						
					// }
				break;
				case MOD_TYPE_PA:
					if (exmod_alarm_a[md_index].m_alarm.ch_pin_ul_op[0]){
						//UL PA Over power
						is_alarm = 1;
					}else if (exmod_alarm_a[md_index].m_alarm.ch_pin_dl_op[0]){
						//DL PA Over power
						is_alarm = 1;
					}else if (exmod_alarm_a[md_index].m_alarm.temp_h){
						//PA Over Temp
						is_alarm = 1;
					}else if (exmod_alarm_a[md_index].m_alarm.pa1){
						//PA Fault
						
						is_alarm = 1;
					}else if (exmod_alarm_a[md_index].m_alarm.rl){
						//
						is_alarm = 1;
					}else if (exmod_alarm_a[md_index].m_alarm.iso_alarm){
						//Isolation 
						is_alarm = 1;
					}

				break;
			}

		}

		if(is_alarm){
			RLDEBUG("===========================system alarm========================\n");
			led_alarm_control(1);

		}else{
			RLDEBUG("===========================system normal========================\n");
			led_alarm_control(0);
		}
		
		alarm_send_flag = FALSE;
	}
}

void alarm_enable_init()
{
	s16 cnt;
	for(cnt=0; cnt<MONITOR_MOD_NUM; cnt++){
		exmod_para_a[cnt].alarm_sw.ch_pin_ul_op[0] = 1;
		exmod_para_a[cnt].alarm_sw.ch_pin_dl_op[0] = 1;
		exmod_para_a[cnt].alarm_sw.temp_h = 1;
		// exmod_para_a[cnt].alarm_sw.temp_l = 1;
		exmod_para_a[cnt].alarm_sw.pa1 = 1;
		exmod_para_a[cnt].alarm_sw.pout_pre = 1;
		exmod_para_a[cnt].alarm_sw.rl = 1;
		exmod_para_a[cnt].alarm_sw.iso_alarm = 1;
		exmod_para_a[cnt].alarm_sw.ad80305_1 = 1;
		exmod_para_a[cnt].alarm_sw.ad80305_2 = 1;
		exmod_para_a[cnt].alarm_sw.self_excited = 1;
	}
	
}

void *alarm_task(void* arg)
{
	s32 cnt;

	RLDEBUG("alarm task start!\r\n");
	arg = arg;
	snmp_msg_init();
	
	memset((void*)&u_alarm_t, 0, sizeof(u_alarm_t));
	memset((void*)&m_alarm_a, 0, (MOD_NUM_IN_ONE_PCB * sizeof(md_alarm_c)));
	memset((void*)&exmod_alarm_a, 0, (MONITOR_MOD_NUM * sizeof(md_alarm_c)));

	alarm_enable_init();

	while (1) {
		timedelay(0, 1, 0, 0);
		unit_alarm_census();
		mod_alarm_census();
		exmod_alarm_census();

		alarm_deal_rep();
	}
}


#if 0
/************************************led control*****************************/
/*
fun: update led
para:
return:
	0--sus
	-1--err
 */
s32 alarm_led_update(void)
{
	s32 num;
	DIGITRON_OUTER_COLOUR_TYPE colour;
	DIGITRON_DISP_TYPE disp_type;
	f64 dl_pout;
	f32 ch_pout;
	s32 cnt;

	num = -1;

	if (m_alarm_a[0].m_alarm.cp_init) {	//cp
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_OUTER_FLASH;
		num = 254;
	} else if (m_alarm_a[0].m_alarm.ap_init) {	//ap
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NORMAL;
		num = 254;
	} else if (m_alarm_a[0].m_alarm.fpga_loaded) {	//fpga load
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NORMAL;
		num = 0;
	} else if (m_alarm_a[0].m_alarm.fpga_uart) {	//fpga uart
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NUM_FLASH;
		num = 0;
	} else if (RELAY_NET_OFFLINE == band_dynamic_para_a[0].md_dynamic_sundry.net_status) {	//fpga uart
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NORMAL;
		num = 1;
	} else if (m_alarm_a[0].m_alarm.fpga_spi) {	//fpga uart
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NUM_FLASH;
		num = 1;
	} else if ((m_alarm_a[0].m_alarm.ch_pin_ul_op[0]) || (m_alarm_a[0].m_alarm.ch_pin_ul_op[1])\
	           || (m_alarm_a[0].m_alarm.ch_pin_ul_op[2]) || (m_alarm_a[0].m_alarm.ch_pin_ul_op[3])\
	           || (m_alarm_a[0].m_alarm.ch_pin_ul_op[4]) || (m_alarm_a[0].m_alarm.ch_pin_ul_op[5])\
	           || (m_alarm_a[0].m_alarm.ch_pin_ul_op[6]) || (m_alarm_a[0].m_alarm.ch_pin_ul_op[7])\
	           || (m_alarm_a[0].m_alarm.ch_pin_ul_op[8]) || (m_alarm_a[0].m_alarm.ch_pin_ul_op[9])\
	           || (m_alarm_a[0].m_alarm.ch_pin_ul_op[10]) || (m_alarm_a[0].m_alarm.ch_pin_ul_op[11])) {	//fpga uart
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NORMAL;
		num = 2;
	} else if ((m_alarm_a[0].m_alarm.ch_pin_ul_lp[0]) || (m_alarm_a[0].m_alarm.ch_pin_ul_lp[1])\
	           || (m_alarm_a[0].m_alarm.ch_pin_ul_lp[2]) || (m_alarm_a[0].m_alarm.ch_pin_ul_lp[3])\
	           || (m_alarm_a[0].m_alarm.ch_pin_ul_lp[4]) || (m_alarm_a[0].m_alarm.ch_pin_ul_lp[5])\
	           || (m_alarm_a[0].m_alarm.ch_pin_ul_lp[6]) || (m_alarm_a[0].m_alarm.ch_pin_ul_lp[7])\
	           || (m_alarm_a[0].m_alarm.ch_pin_ul_lp[8]) || (m_alarm_a[0].m_alarm.ch_pin_ul_lp[9])\
	           || (m_alarm_a[0].m_alarm.ch_pin_ul_lp[10]) || (m_alarm_a[0].m_alarm.ch_pin_ul_lp[11])) {	//fpga uart
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NUM_FLASH;
		num = 2;
	} else if ((m_alarm_a[0].m_alarm.ch_pin_dl_op[0]) || (m_alarm_a[0].m_alarm.ch_pin_dl_op[1])\
	           || (m_alarm_a[0].m_alarm.ch_pin_dl_op[2]) || (m_alarm_a[0].m_alarm.ch_pin_dl_op[3])\
	           || (m_alarm_a[0].m_alarm.ch_pin_dl_op[4]) || (m_alarm_a[0].m_alarm.ch_pin_dl_op[5])\
	           || (m_alarm_a[0].m_alarm.ch_pin_dl_op[6]) || (m_alarm_a[0].m_alarm.ch_pin_dl_op[7])\
	           || (m_alarm_a[0].m_alarm.ch_pin_dl_op[8]) || (m_alarm_a[0].m_alarm.ch_pin_dl_op[9])\
	           || (m_alarm_a[0].m_alarm.ch_pin_dl_op[10]) || (m_alarm_a[0].m_alarm.ch_pin_dl_op[11])) {	//fpga uart
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NORMAL;
		num = 3;
	} else if ((m_alarm_a[0].m_alarm.ch_pin_dl_lp[0]) || (m_alarm_a[0].m_alarm.ch_pin_dl_lp[1])\
	           || (m_alarm_a[0].m_alarm.ch_pin_dl_lp[2]) || (m_alarm_a[0].m_alarm.ch_pin_dl_lp[3])\
	           || (m_alarm_a[0].m_alarm.ch_pin_dl_lp[4]) || (m_alarm_a[0].m_alarm.ch_pin_dl_lp[5])\
	           || (m_alarm_a[0].m_alarm.ch_pin_dl_lp[6]) || (m_alarm_a[0].m_alarm.ch_pin_dl_lp[7])\
	           || (m_alarm_a[0].m_alarm.ch_pin_dl_lp[8]) || (m_alarm_a[0].m_alarm.ch_pin_dl_lp[9])\
	           || (m_alarm_a[0].m_alarm.ch_pin_dl_lp[10]) || (m_alarm_a[0].m_alarm.ch_pin_dl_lp[11])) {	//fpga uart
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NUM_FLASH;
		num = 3;
	} else if (m_alarm_a[0].m_alarm.ad80305_1) {	//fpga uart
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NORMAL;
		num = 4;
	} else if (m_alarm_a[0].m_alarm.ad80305_2) {	//fpga uart
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NUM_FLASH;
		num = 4;
	} else if (m_alarm_a[0].m_alarm.temp_h) {	//fpga uart
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NORMAL;
		num = 5;
	} else if (m_alarm_a[0].m_alarm.temp_l) {	//fpga uart
		colour = DIGITRON_OUTER_COLOUR_RED;
		disp_type = DIGITRON_DISP_NUM_FLASH;
		num = 5;
	} else {
		/*
		信道1功率：P1
		信道2功率：P2
		信道3功率：P3
		信道n功率：Pn
		求总功率P
		首先，把P1~Pn做指数变换得到：
		X1 = 10^(0.1*P1);
		X2 = 10^(0.1*P2);
		X3 = 10^(0.1*P3);
		Xn = 10^(0.1*Pn);
		...
		总功率P=10*log10(X1+X2+X3+...Xn)

		 */
		dl_pout = 0;
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++)
		{
			f12tof32(band_dynamic_para_a[0].ch_rf_t.dl[cnt].pout, &ch_pout);
			dl_pout += exp(0.1 * ch_pout);
		}
		dl_pout = 10 * log10(dl_pout);
		colour = DIGITRON_OUTER_COLOUR_NULL;
		disp_type = DIGITRON_DISP_NORMAL;

		if (dl_pout < -35.0) {
			num = 0;
		} else if ((dl_pout >= (-35.0)) && (dl_pout < (-30.0))) {
			num = 1;
		} else if ((dl_pout >= (-30.0)) && (dl_pout < (-25.0))) {
			num = 2;
		} else if ((dl_pout >= (-25.0)) && (dl_pout < (-20.0))) {
			num = 3;
		} else if ((dl_pout >= (-20.0)) && (dl_pout < (-15.0))) {
			num = 4;
		} else if ((dl_pout >= (-15.0)) && (dl_pout < (-10.0))) {
			num = 5;
		} else if ((dl_pout >= (-10.0)) && (dl_pout < (-5.0))) {
			num = 6;
		} else if ((dl_pout >= (-5.0)) && (dl_pout < (0.0))) {
			num = 7;
		} else if ((dl_pout >= (0.0)) && (dl_pout < (5.0))) {
			num = 8;
		} else if ((dl_pout >= (5.0))) {
			num = 9;
		}
	}

LED_UPDATE:
	cnt = digitron_display(num, colour, disp_type);
	return cnt;
}

#endif