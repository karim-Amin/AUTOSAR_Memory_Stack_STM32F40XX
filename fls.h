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
 *                         Constants Definitions                              *
*******************************************************************************/
#define FLS_MAX_CALL_CYCLE              (1U)
#define FLS_DCEN_BIT_NUMBER             (10U)
#define FLS_ICEN_BIT_NUMBER             (9U)
#define FLS_PRFTEN_BIT_NUMBER           (8U)
#define FLS_KEY2_NUM_OF_SHIFTS          (16U)
#define FLS_ACR_MASK                    (0xFFFFFFF8)
#define FLS_UNLOCK_CR_KEY1              (0x45670123)
#define FLS_UNLOCK_CR_KEY2              (0xCDEF89AB)
/******************************************************************************
 *                      API Service Id Macros                                 *
*******************************************************************************/
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
********************************************************************************/
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
/*******************************************************************************
 *                        Types Definitions                                    *
********************************************************************************/

/* Used as address offset from the configured flash base address to access a certain flash memory area. */
typedef uint32          Fls_AddressType;
/* Specifies the number of bytes to read/write/erase/compare.  */
typedef uint32          Fls_LengthType;

/* Configuration definitions */

/* this is relation between the voltage range and cpu frequency (Data sheet page 80)*/
typedef enum{
  zero_ws,
  one_ws,
  two_ws,
  three_ws,
  four_ws,
  five_ws,
  six_ws,
  seven_ws
}Latancy_Type;

/* select this parallelism based on voltage range you use  (Data sheet page 85)*/
typedef enum{
  x8_psize,
  
  x16_psize,
  
  x32_psize,
  
  x64_psize
}parallelism_Type;
/*
 * Level 0 => read protection is not active 
 * Level 1 => read protection of memories active
 * Level 2 => chip read protection active
 */
typedef enum{
  read_level_0 = 0xAA,
  read_level_1 = 0x00,
  read_level_2 = 0xCC
}Read_protection_Type;
/*
 * BOR ( Brownout threshold level )=> These bits contain the supply level threshold that activates/releases the reset
 * 00: BOR Level 3 (VBOR3), brownout threshold level 3
 * 01: BOR Level 2 (VBOR2), brownout threshold level 2
 * 10: BOR Level 1 (VBOR1), brownout threshold level 1
 * 11: BOR off, POR/PDR reset threshold level is applied
 */
typedef enum{
  BOR_level_3,
  BOR_level_2,
  BOR_level_1,
  BOR_off
}BOR_Level_Type;
/* this struct for initializing the flash memory */
typedef struct{
  /* flash speed of operations */
  Latancy_Type          fls_latancy;
  parallelism_Type      fls_p_size;
  /* user option bytes */
  BOR_Level_Type        fls_BOR_level;
  /* Cycle time of calls of the flash driver's main function (in seconds ) range : 0 -> 1 */
  float32               fls_call_cycle;
  /* This parameter is the default FLS device mode after initialization */
  MemIf_ModeType        fls_default_mode;
  /* flash memory protection */
  Read_protection_Type  fls_read_protection;    
  boolean               fls_write_protection_enable;
  /* caching and prefetch feature */
  boolean               fls_data_cache_enable;
  boolean               fls_instruction_cache_enable;
  boolean               fls_prefetch_enable;
}Fls_configType;

/*******************************************************************************
 *                        Function Prototype                                   *
********************************************************************************/
/*******************************************************************************
* Service Name: Fls_Init
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration struct
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: initilaze the flash memory driver module
********************************************************************************/
void Fls_Init( const Fls_configType  * config_ptr);

/*******************************************************************************
* Service Name: Fls_Erase
* Sync/Async: ASynchronous
* Reentrancy: Non-reentrant
* Parameters (in): TargetAdderss - Target address in flash memory. This address offset will be added to the flash memory base address.
*                  Length - Number of bytes to erase
* Parameters (inout): None
* Parameters (out): None
* Return value:  E_OK: erase command has been accepted 
*                E_NOT_OK: erase command has not been accepted
* Description: Erases flash sector(s).
********************************************************************************/
Std_ReturnType Fls_Erase(
                         Fls_AddressType TargetAdderss,
                         Fls_LengthType  Length
                                                      );

/*******************************************************************************
* Service Name: Fls_Write
* Sync/Async: ASynchronous
* Reentrancy: Non-reentrant
* Parameters (in): TargetAdderss => Target address in flash memory. This address offset will be added to the flash memory base address.
*                  SourceAddressPtr => Pointer to source data buffer.
*                  Length => Number of bytes to erase.
* Parameters (inout): None
* Parameters (out): None
* Return value:  E_OK: write command has been accepted 
*                E_NOT_OK: write command has not been accepted
* Description: Writes one or more complete flash pages.
********************************************************************************/
Std_ReturnType Fls_Write(
                         Fls_AddressType TargetAdderss,
                         const uint8* SourceAddressPtr,
                         Fls_LengthType  Length
                                                      );

/* Check if the user configured this api on or off*/
#if ( FLS_CANCEL_API == STD_ON)
/*******************************************************************************
* Service Name: Fls_Cancel
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: cancels an ongoing job 
********************************************************************************/
void Fls_Cancel( void );

#endif

/* Check if the user configured this api on or off*/
#if ( FLS_GET_STATUS_API == STD_ON)
/*******************************************************************************
* Service Name: Fls_GetStatus
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: MemIf_StatusType
* Description: returns the driver state
********************************************************************************/
MemIf_StatusType Fls_GetStatus( void );

#endif

/* Check if the user configured this api on or off*/
#if ( FLS_GET_JOB_RESULT_API == STD_ON)
/*******************************************************************************
* Service Name: Fls_GetJobResult
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: MemIf_JobResultType
* Description: returns the Result of the last job 
********************************************************************************/
MemIf_JobResultType Fls_GetJobResult( void );

#endif

/*******************************************************************************
* Service Name: Fls_Write
* Sync/Async: ASynchronous
* Reentrancy: Non-reentrant
* Parameters (in): SourceAdderss => Source address in flash memory. This address offset will be added to the flash memory base address
*                  TargetAddressPtr => Pointer to Target data buffer.
*                  Length => Number of bytes to Read.
*
* Parameters (inout): None
* Parameters (out): TargetAddressPtr => Pointer to Target data buffer.
*
* Return value:  E_OK: read command has been accepted 
*                E_NOT_OK: read command has not been accepted
* Description: Reads from the flash memory.
********************************************************************************/
Std_ReturnType Fls_Read(
                         Fls_AddressType SourceAdderss,
                         const uint8* TargetAddressPtr,
                         Fls_LengthType  Length
                                                      );
/* Check if the user configured this api on or off*/
#if ( FLS_COMPARE_API == STD_ON)
/*******************************************************************************
* Service Name: Fls_Compare
* Sync/Async: ASynchronous
* Reentrancy: Non-reentrant
* Parameters (in): SourceAdderss => Source address in flash memory. This address offset will be added to the flash memory base address
*                  TargetAddressPtr => Pointer to Target data buffer.
*                  Length => Number of bytes to Read.
*
* Parameters (inout): None
* Parameters (out): None
*
* Return value:  E_OK: read command has been accepted 
*                E_NOT_OK: read command has not been accepted
*
* Description: Compares the contents of an area of flash memory with that of an application data buffer.
********************************************************************************/
Std_ReturnType Fls_Compare(
                         Fls_AddressType SourceAdderss,
                         const uint8* TargetAddressPtr,
                         Fls_LengthType  Length
                                                      );
#endif

/* Check if the user configured this api on or off*/
#if ( FLS_SET_MODE_API == STD_ON)
/*******************************************************************************
* Service Name: Fls_SetMode
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): Mode => MEMIF_MODE_SLOW: Slow read access / normal SPI access. 
*                          MEMIF_MODE_FAST: Fast read access / SPI burst access. 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: returns the driver state
********************************************************************************/
void Fls_SetMode( MemIf_ModeType Mode);

#endif  

/* Check if the user configured this api on or off*/
#if ( FLS_VERSION_INFO_API == STD_ON)
/*******************************************************************************
* Service Name: Fls_GetVersionInfo
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): versioninfoPtr => Pointer to where to store the version information of this module.
* Return value: None
* Description: Returns the version information of this module. 
********************************************************************************/
void Fls_GetVersionInfo( Std_VersionInfoType* versioninfoPtr );

#endif 

/*******************************************************************************
* Service Name: Fls_MainFunction
* Timing : Fixed_cyclic
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Performs the proccessing of jobs
********************************************************************************/
void Fls_MainFunction( void );

/* Extern PB structures to be used by fls and other modules */
extern const Fls_configType  fls_configuration;
#endif /* FLS_H */