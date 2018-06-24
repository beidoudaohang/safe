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

typedef unsigned char  			bool_t;
typedef unsigned char  			uint8_t;                    /* Unsigned  8 bit quantity       */
typedef signed   char  			int8_t;                    /* Signed    8 bit quantity       */
typedef unsigned short 			uint16_t;                   /* Unsigned 16 bit quantity       */
typedef signed   short 			int16_t;                   /* Signed   16 bit quantity       */
typedef unsigned int  			uint32_t;                   /* Unsigned 32 bit quantity       */
typedef signed   int   			int32_t;                   /* Signed   32 bit quantity       */

// #ifndef uint64_t
// #define uint64_t unsigned long 
// #endif
// #ifndef int64_t
// #define int64_t signed long 
// #endif
// typedef unsigned long 		uint64_t; 
// typedef signed   long 		int64_t; 
// typedef float 						fp32_t;
// typedef double 					fp64_t;


#ifndef NULL
#define NULL ((void*)0)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif
#ifndef ERROR
#define ERROR -1
#endif
#ifndef RET_SUCC
#define RET_SUCC 0
#endif
#ifndef RET_FAIL
#define RET_FAIL 1
#endif
#define SETBIT(DAT,BIT) ((DAT)|=(0x00000001<<(BIT)))
#define CLRBIT(DAT,BIT) ((DAT)&=(~(0x00000001<<(BIT))))
#define GETBIT(DAT,BIT) ((DAT)&(0x00000001<<(BIT)))
#define MIN(A,B)		(((A)>=(B))?(B):(A))
#define MAX(A,B)		(((A)>=(B))?(A):(B))

#define PRO_DEBUG_ENABLE        	1
#define MONITOR_MODULE_ENABLE		1	/*标示是否包含监控模块*/
#define PA_MODULE_ENABLE			0	/*标示是否包含PA模块*/
#define OTHER_MODULE_ENABLE			1	/*标示是否有除监控模块以外的模块*/

//#define RL_PERMISSION_CHECK		1		/*adr access permisson check*/
#if MONITOR_MODULE_ENABLE
#define MONITOR_MOD_NUM 			160 //8*10*2	
#define SYS_NUM 8
#endif

#if PA_MODULE_ENABLE
#define MOD_NUM_IN_ONE_PCB 			3	
#else
#define MOD_NUM_IN_ONE_PCB 			1	
#endif

//#define PROTOCOL_ENCRYPT_LZO        1

//#define TEST_DATA
#define TEST_WEB_DATA
#endif //__HELIOS_H__