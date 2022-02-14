/*****************************************************
 * file name : fls_reg.h
 * module : flash memory 
 * Date : 14 / 02 /2022
 * Auth : Karim Mohamed Amin
******************************************************/
#ifndef FLS_REG_H
#define FLS_REG_H
#include "Std_Types.h"

/*****************************************************
 *              FLASH MEMORY REGISTERS               *
******************************************************/

#define FLS_AC_REG             (*((volatile uint32 *) 0x00000000))
#define FLS_KEY_REG            (*((volatile uint32 *) 0x00000004))
#define FLS_OPTKEY_REG         (*((volatile uint32 *) 0x00000008))
#define FLS_STATUS_REG         (*((volatile uint32 *) 0x0000000C))

#endif /* FLS_REG_H */