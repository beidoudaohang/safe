//FPGA加载模块对外接口
#ifndef _FPGA_LOADER_H_
#define _FPGA_LOADER_H_

/*
*********************************************************************************************************
*	函 数 名: fpgaloader_init
*	功能说明: FPGA加载模块初始化
*	形    参：void
*	返 回 值: 0：初始化成功；-1：初始化失败
*	备注：
*********************************************************************************************************
*/
int fpgaloader_init(void);

/*
*********************************************************************************************************
*	函 数 名: fpgaloader_start_fpga_load
*	功能说明: 启动加载指定的FPGA升级文件到FPGA
*	形    参：fpga_bin_file_dir_name:fpga 升级文件的系统路径(包括升级文件名)，system/bin/relay_lte_fpgafile.bin
*	返 回 值: 0:FPGA加载启动成功；-1：FPGA加载启动失败；
*	备注：该函数只是启动加载流程，查询加载情况使用fpgaloader_get_fpga_load_finish()；如果一次加载不成功，
* 			建议多尝试几次，直至成功，建议尝试10次；正常加载一个FPGA升级文件需要5-10s；
*********************************************************************************************************
*/
int fpgaloader_start_fpga_load(char * fpga_bin_file_dir_name);

/*
*********************************************************************************************************
*	函 数 名: fpgaloader_get_fpga_load_finish
*	功能说明: 获取FPGA加载完成情况
*	形    参：void
*	返 回 值: 0：FPGA未开始加载；1：FPGA正在加载；2：FPGA加载失败；3：FPGA加载成功；
*	备注：
*********************************************************************************************************
*/
int fpgaloader_get_fpga_load_finish(void);



#endif
