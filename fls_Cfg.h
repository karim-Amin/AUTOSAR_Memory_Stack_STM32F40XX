/******************************************************************************
 *
 * Module: flash memory
 *
 * File Name: fls_Cfg.h
 *
 * Description: precompile configuration Header file for STM32F405RG Microcontroller - Flash memory Driver
 *
 * Author: Karim Mohamed Amin
 ******************************************************************************/
#ifndef FLS_CFG_H
#define FLS_CFG_H
#include "MemIf_Types.h"
/*
 * Module Version 1.0.0
 */
#define FLS_CFG_SW_MAJOR_VERSION              (1U)
#define FLS_CFG_SW_MINOR_VERSION              (0U)
#define FLS_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define FLS_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define FLS_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define FLS_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/* Pre-compile option for Development Error Detect */
#define FLS_DEV_ERROR_DETECT                 (STD_ON)

/* Pre-compile option for Version Info API */
#define FLS_VERSION_INFO_API                 (STD_ON)
   
/* Pre-compile option for FLS_Cancel API */
#define FLS_CANCEL_API                       (STD_ON)
   
/* Pre-compile option for FLS_Cmpare API */
#define FLS_COMPARE_API                      (STD_ON)

/* Pre-compile option for FLS_GetJobResult API */
#define FLS_GET_JOB_RESULT_API               (STD_ON)
   
/* Pre-compile option for FLS_GetStatus API */
#define FLS_GET_STATUS_API                   (STD_ON)
   
/* Pre-compile option for FLS_SetMode API */
#define FLS_SET_MODE_API                     (STD_ON)
  
/* Job processing triggered by hardware interrupt */
#define FLS_USE_INTERRUPTS                   (STD_OFF)
   
/* Pre-compile option for loading the flash access code to the RAM */
#define FLS_AC_LOAD_ON_JOB_START             (STD_OFF)
   
/* BASE ADDRESS definition for STM32F405RG Microcontroller */
#define FLS_BASE_ADDRESS                     (Fls_AddressType)(0x08000000)

/* Total size of flash memory 1MB (1024 KB => ( (1024 * 1024) -1 )Bytes  ) */
#define FLS_TOTAL_SIZE                       (Fls_AddressType)(1048575U) 
   
/* Flash Memory Driver Index */
#define FLS_DRIVER_INDEX                     (0U)
   

   
#endif FLS_CFG_H        