#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

//fpga加载模块设备
#define FPGA_DEVICE_NAME "/dev/fpga_altera"
//fpga升级文件1
#define FPGA_IMAGE_FILE_DIR_AND_FILE_NAME "/system/bin/fpgafireware_new.rbf" //"/system/bin/fpgafireware.bin"
//fpga升级文件2
#define FPGA_IMAGE_FILE_DIR_AND_FILE_NAME_PRE "/system/bin/fpgafireware.bin"
//fpga升级文件路径和文件名的最大长度
#define FPGA_IMAGE_FILEPAHT_LENTH 96

char fpga_bin_dir[FPGA_IMAGE_FILEPAHT_LENTH]="";

int helios_dev_fpga_fd = -1;
char fpga_load_finish =0;

int fpgaloader_init(void){
	
	int fd = -1;
	fd = open(FPGA_DEVICE_NAME, O_RDWR);
	if(fd == -1) {
		printf("Failed to open device %s./n", FPGA_DEVICE_NAME);
		return -1;
	}
	helios_dev_fpga_fd = fd;
	
	return 0;
	
}


int fpgaloader_start_fpga_load(char * fpga_bin_file_dir_name){
	int val;
	
	if(helios_dev_fpga_fd ==-1){
        return -1;
    }
	read(helios_dev_fpga_fd,&val,sizeof(val));
	if(val ==0x01){
		return -1;
	}
	
	if(strlen(fpga_bin_file_dir_name ) > FPGA_IMAGE_FILEPAHT_LENTH){
		return -1;
	}
	
	sscanf(fpga_bin_file_dir_name, "%s",fpga_bin_dir );
	
	write(helios_dev_fpga_fd,fpga_bin_dir,strlen(fpga_bin_dir));
	
	
	return 0;
}

int fpgaloader_get_fpga_load_finish(void){
	int ret =0;
	int val;
	if(helios_dev_fpga_fd ==-1){
        return -1;
    }
	read(helios_dev_fpga_fd,&val,sizeof(val));
	//printf("read val=%d.\n",val); 
	 return val;
	 
	
}

int helios_fpga_dev_work(void){
    int val;
    if(helios_dev_fpga_fd ==-1){
        return -1;
    }
    if(fpga_load_finish ==0){
        val =fpgaloader_get_fpga_load_finish();
        if(val ==0x03){
            fpga_load_finish =1;
            //close(helios_dev_fpga_fd);
        }else{
            if(val != 0x01){
                fpgaloader_start_fpga_load(FPGA_IMAGE_FILE_DIR_AND_FILE_NAME_PRE);
                usleep(300000);//3000
            }
        }
    }
    return 0;

}

#if 0
int main(int argc, char** argv)
{
	fpgaloader_init();
	//printf("write file.\n");
	
	fpgaloader_start_fpga_load(FPGA_IMAGE_FILE_DIR_AND_FILE_NAME);
	while(1)
	{
		usleep(3000000);
		helios_fpga_dev_work();
	}
	
	return 0;
}
#endif
