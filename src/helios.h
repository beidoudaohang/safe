#ifndef __HELIOS_H__
#define __HELIOS_H__

/*type define*/
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short int u16;
typedef signed short int s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long  int u64;
typedef signed long int s64;
typedef float f32;
typedef double f64;

#ifndef NULL
#define NULL ((void*)0)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

#define SETBIT(DAT,BIT) ((DAT)|=(0x00000001<<(BIT)))
#define CLRBIT(DAT,BIT) ((DAT)&=(~(0x00000001<<(BIT))))
#define GETBIT(DAT,BIT) ((DAT)&(0x00000001<<(BIT)))
#define MIN(A,B)		(((A)>=(B))?(B):(A))
#define MAX(A,B)		(((A)>=(B))?(A):(B))

#define PRO_DEBUG_ENABLE        	1
#define MONITOR_MODULE_ENABLE		1	/*标示是否包含监控模块*/
#define OTHER_MODULE_ENABLE			1	/*标示是否有除监控模块以外的模块*/
//#define RL_PERMISSION_CHECK		1		/*adr access permisson check*/
#define MOD_NUM_IN_ONE_PCB 			3	

//#define PROTOCOL_ENCRYPT_LZO        1


#endif //__HELIOS_H__