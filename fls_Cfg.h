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
#define FLS_DEV_ERROR_DETECT                (STD_ON)

/* Pre-compile option for Version Info API */
#define FLS_VERSION_INFO_API                (STD_ON)

#endif FLS_CFG_H