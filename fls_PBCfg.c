/******************************************************************************
 *
 * Module: flash memory
 *
 * File Name: fls_PBCfg.h
 *
 * Description: Post build configuration Source file for STM32F405RG Microcontroller - Flash memory Driver
 *
 * Author: Karim Mohamed Amin
 ******************************************************************************/
#include "fls.h"

/*
 * Module Version 1.0.0
 */
#define FLS_PBCFG_SW_MAJOR_VERSION              (1U)
#define FLS_PBCFG_SW_MINOR_VERSION              (0U)
#define FLS_PBCFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define FLS_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define FLS_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define FLS_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

/* AUTOSAR Version checking between fls_PBcfg.c and fls.h files */
#if ((FLS_PBCFG_AR_RELEASE_MAJOR_VERSION != FLS_AR_RELEASE_MAJOR_VERSION)\
 ||  (FLS_PBCFG_AR_RELEASE_MINOR_VERSION != FLS_AR_RELEASE_MINOR_VERSION)\
 ||  (FLS_PBCFG_AR_RELEASE_PATCH_VERSION != FLS_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of PBcfg.c does not match the expected AUTOSAR version"
#endif

/* Software Version checking between Dio_PBcfg.c and Dio.h files */
#if ((FLS_PBCFG_SW_MAJOR_VERSION != FLS_SW_MAJOR_VERSION)\
 ||  (FLS_PBCFG_SW_MINOR_VERSION != FLS_SW_MINOR_VERSION)\
 ||  (FLS_PBCFG_SW_PATCH_VERSION != FLS_SW_PATCH_VERSION))
  #error "The SW version of PBcfg.c does not match the expected Software version"
#endif
/* PB structure used with FLS_Init API */
const Fls_configType  fls_configuration = { five_ws , x32_psize , BOR_off , 0.5 , MEMIF_MODE_SLOW , read_level_0 ,FALSE , TRUE , TRUE , TRUE};
