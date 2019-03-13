/* =================================================================================
File Name:      TYPE.H
Description:    Type definition
Developer:      Sikalov Yuiry 
Ts Idea:        20-06-2012 
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 20-06-2012 Version 1.00
===================================================================================== */

#ifndef __type_H__
#define __type_H__

#ifndef NULL
#define NULL    ((void *)0)
#endif

typedef signed int               INT;
typedef unsigned int             UINT;
typedef signed char              CHAR;
typedef unsigned char            UCHAR;
typedef unsigned char            BYTE;
typedef signed short             SHORT;
typedef unsigned short           USHORT;
typedef unsigned short           WORD;
typedef signed long              LONG;
typedef unsigned long            ULONG;
typedef unsigned long            DWORD;
  
typedef long double              f64;
typedef float                    f32;

typedef unsigned long long       u64;
typedef unsigned long            u32;
typedef unsigned short           u16;
typedef unsigned char            u8;

typedef signed long long         s64;
typedef signed long              s32;
typedef signed short             s16;
typedef signed char              s8;

typedef volatile unsigned long   vu32;
typedef volatile unsigned short  vu16;
typedef volatile unsigned char   vu8;

typedef volatile signed long     vs32;
typedef volatile signed short    vs16;
typedef volatile signed char     vs8;

#endif
