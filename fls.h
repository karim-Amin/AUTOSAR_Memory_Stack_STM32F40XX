/******************************************************************************
 *
 * Module: flash memory
 *
 * File Name: fls.h
 *
 * Description: Header file for STM32F405RG Microcontroller - Flash memory Driver
 *
 * Author: Karim Mohamed Amin
 ******************************************************************************/
#ifndef FLS_H
#define FLS_H
/* my company id */
#define FLS_VENDOR_ID          (2000U)

/* the FLS driver id in software module doc*/
#define FLS_MODULE_ID          (92U)

/* FLS Instance Id */
#define FLS_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define FLS_SW_MAJOR_VERSION           (1U)
#define FLS_SW_MINOR_VERSION           (0U)
#define FLS_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define FLS_AR_RELEASE_MAJOR_VERSION   (4U)
#define FLS_AR_RELEASE_MINOR_VERSION   (0U)
#define FLS_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for FLS Status
 */
#define FLS_INITIALIZED                (1U)
#define FLS_NOT_INITIALIZED            (0U)

/*
 * Include the standard AUTOSAR types std_types.h file
 */
#include "Std_Types.h"

/*
 * check that the two files from the same software version and AUTOSAR verion
 */

#if ((FLS_SW_MAJOR_VERSION != STD_TYPES_SW_MAJOR_VERSION)\
 ||  (FLS_SW_MINOR_VERSION != STD_TYPES_SW_MINOR_VERSION)\
 ||  (FLS_SW_PATCH_VERSION != STD_TYPES_SW_PATCH_VERSION))

  #error "Incompatable Software versions between fls.h and Std_Types.h";
#endif
   
 /*
 * check that the two files from the same AUTOSAR version 
 */

#if ((FLS_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
   ||(FLS_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
   ||(FLS_AR_RELEASE_PATCH_VERSION != STD_TYPES_AR_RELEASE_PATCH_VERSION))

  #error "Incompatable AUTOSAR versions between fls.h and Std_Types.h";
#endif
   
/* Port Pre-Compile Configuration Header file */
#include "fls_Cfg.h"

/* AUTOSAR Version checking between fls_Cfg.h and fls.h files */
#if ((FLS_CFG_AR_RELEASE_MAJOR_VERSION != FLS_AR_RELEASE_MAJOR_VERSION)\
 ||  (FLS_CFG_AR_RELEASE_MINOR_VERSION != FLS_AR_RELEASE_MINOR_VERSION)\
 ||  (FLS_CFG_AR_RELEASE_PATCH_VERSION != FLS_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of fls_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((FLS_CFG_SW_MAJOR_VERSION != FLS_SW_MAJOR_VERSION)\
 ||  (FLS_CFG_SW_MINOR_VERSION != FLS_SW_MINOR_VERSION)\
 ||  (FLS_CFG_SW_PATCH_VERSION != FLS_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif

 /* include non-AUTOSAR file so we do not have to check the version */
#include "Common_Macros.h"

/******************************************************************************
 *                      API Service Id Macros                                 *
******************************************************************************/
#define FLS_INIT_SID                    (0x00)
#define FLS_ERASE_SID                   (0x01)
#define FLS_WRITE_SID                   (0x02)
#define FLS_CANCEL_SID                  (0x03)
#define FLS_GET_STATUS_SID              (0x04)
#define FLS_GET_JOB_RESULT_SID          (0x05)
#define FLS_MAIN_FUNCTION_SID           (0x06)
#define FLS_READ_SID                    (0x07)
#define FLS_COMPARE_SID                 (0x08)
#define FLS_SET_MODE_SID                (0x09)
#define FLS_GET_VERSION_INFO_SID        (0x10)
/*******************************************************************************
 *                      DET Error Codes                                        *
*******************************************************************************/
#define FLS_E_PARAM_CONFIG              (0x01)
#define FLS_E_PARAM_ADDRESS             (0x02)
#define FLS_E_PARAM_LENGTH              (0x03)
#define FLS_E_PARAM_DATA                (0x04)
#define FLS_E_UNINIT                    (0x05)
#define FLS_E_BUSY                      (0x06)
#define FLS_E_VERIFY_ERASE_FAILED       (0x07)
#define FLS_E_VERIFY_WRITE_FAILED       (0x08)
#define FLS_E_TIMEOUT                   (0x09)
#define FLS_E_PARAM_POINTER             (0x0A)


#endif /* FLS_H */