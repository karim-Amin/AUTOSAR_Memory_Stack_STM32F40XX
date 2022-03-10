/******************************************************************************
 *
 * Module: flash memory
 *
 * File Name: Fee.h
 *
 * Description: Header file for Fee module
 *
 * Author: Karim Mohamed Amin
 ******************************************************************************/
#ifndef FEE_H_
#define FEE_H_

#define FEE_MODULE_ID			21
#define FEE_INSTANCE_ID                 0
#define FEE_VENDOR_ID			1000

#define FEE_SW_MAJOR_VERSION	1
#define FEE_SW_MINOR_VERSION	0
#define FEE_SW_PATCH_VERSION	0
#define FEE_AR_MAJOR_VERSION	4
#define FEE_AR_MINOR_VERSION	0
#define FEE_AR_PATCH_VERSION	3

#include "Std_Types.h"
#include "Fee_Cfg.h"

#if (FEE_DEV_ERROR_DETECT == STD_ON)
 // Error codes reported by this module defined by AUTOSAR
#define FEE_E_UNINIT					0x01
#define FEE_E_INVALID_BLOCK_NO				0x02
#define FEE_E_INVALID_BLOCK_OFS				0x03
#define FEE_E_INVALID_DATA_PTR				0x04
#define FEE_E_INVALID_BLOCK_LEN				0x05
#define FEE_E_BUSY					0x06



// Other error codes reported by this module
#define FEE_PARAM_OUT_OF_RANGE				0x40
#define FEE_UNEXPECTED_STATE				0x41
#define FEE_FLASH_CORRUPT				0xfa
#define FEE_UNEXPECTED_STATUS				0xfb
#define FEE_E_WRONG_CONFIG				0xfc
#define FEE_E_UNEXPECTED_EXECUTION			0xfd
#define FEE_E_NOT_SUPPORTED				0xfe
#define FEE_E_NOT_IMPLEMENTED_YET			0xff

#endif
// Service ID in this module
#define FEE_INIT					0x00
#define FEE_SET_MODE_ID				        0x01
#define FEE_READ_ID					0x02
#define FEE_WRITE_ID					0x03
#define FEE_CANCEL_ID					0x04
#define FEE_GET_STATUS_ID				0x05
#define FEE_GET_JOB_RESULT_ID				0x06
#define FEE_INVALIDATE_BLOCK_ID				0x07
#define FEE_GET_VERSION_INFO_ID				0x08
#define FEE_ERASE_IMMEDIATE_ID				0x09
#define FEE_JOB_END_NOTIFICTION_ID			0x10
#define FEE_JOB_ERROR_NOTIFICTION_ID		        0x11
#define FEE_MAIN_FUNCTION_ID				0x12

#define FEE_STARTUP_ID					0x40
#define FEE_GARBAGE_WRITE_HEADER_ID		        0x41
#define FEE_GLOBAL_ID					0xff
/*******************************************************************************
 *                        Types Definitions                                    *
********************************************************************************/

/*
 * Containers and configuration parameters
 */
typedef void (*Fee_JobEndCallbackFunctionType)(void);	/** @req FEE098 */
typedef void (*Fee_JobErrorCallbackFunctionType)(void);

typedef struct {
	Fee_JobEndCallbackFunctionType		NvmJobEndCallbackNotificationCallback;
	Fee_JobErrorCallbackFunctionType	NvmJobErrorCallbackNotificationCallback;
} Fee_GeneralType;

typedef struct {
	uint8	DeviceIndex;			
	uint16	BlockNumber;			
	uint16	BlockSize;			
	boolean	ImmediateData;			
	uint32	NumberOfWriteCycles;	       
} Fee_BlockConfigType;	

typedef struct {
	/* Hold the call back notifiactions */
	Fee_GeneralType					General;
        /* Hold list of Configured blocks */
	const Fee_BlockConfigType*                      BlockConfig;		
} Fee_ConfigType;

/*******************************************************************************
 *                        Functions ProtoType                                  *
********************************************************************************/
/*******************************************************************************
* Service Name: Fee_MainFunction
* Timing : ON_PRE_CONDITION 
* Description: Service to handle the requested read / write / erase jobs and the internal management operations.
********************************************************************************/
void Fee_MainFunction(void);	
/*******************************************************************************
* Service Name: Fee_Init
* Sync/Async: ASynchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: None
********************************************************************************/
void Fee_Init(void);	
/*******************************************************************************
* Service Name: Fee_SetMode
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): mode -> desired mode for underlying flash driver 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Service to call the Fls_SetMode function of the underlying flash driver. 
********************************************************************************/
void Fee_SetMode(MemIf_ModeType mode);	
/*******************************************************************************
* Service Name: Fee_Read
* Sync/Async: ASynchronous
* Reentrancy: Non-reentrant
* Parameters (in):  blockNumber  -> Number of logical block, also denoting start address of that block in flash memory.
                    blockOffset -> Read address offset inside the block                   
                    Length -> Number of bytes to read 
* Parameters (inout): None
* Parameters (out): dataBufferPtr -> pointer to data buffer to store the read data
* Return value:  E_OK: The requested job has been accepted by the module.  
*                E_NOT_OK:  The requested job has not been accepted by the module.
* Description: Service to initiate the read job
********************************************************************************/
Std_ReturnType Fee_Read(
                        uint16 blockNumber,
                        uint16 blockOffset,
                        uint8* dataBufferPtr,
                        uint16 length);
/*******************************************************************************
* Service Name: Fee_Write
* Sync/Async: ASynchronous
* Reentrancy: Non-reentrant
* Parameters (in):  blockNumber  -> Number of logical block, also denoting start address of that block in flash memory.
                    dataBufferPtr -> pointer to data buffer to store the read data
* Parameters (inout): None
* Parameters (out): None
* Return value:  E_OK: The requested job has been accepted by the module.  
*                E_NOT_OK:  The requested job has not been accepted by the module.
* Description: Service to initiate the Write job
********************************************************************************/
Std_ReturnType Fee_Write(uint16 blockNumber, uint8* dataBufferPtr); 
/*******************************************************************************
* Service Name: Fee_Cancel
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Service to call the cancel function of the underlying flash driver.
********************************************************************************/
void Fee_Cancel(void);	
/*******************************************************************************
* Service Name: Fee_GetStatus
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None 
* Parameters (inout): None
* Parameters (out): None
* Return value: MemIfStatusType :       MEMIF_UNINIT -> The FEE module has not been initialized.
                                        MEMIF_BUSY -> The FEE module is currently busy.
                                        MEMIF_BUSY_INTERNAL -> The FEE module is busy with internal management operations. 
                                        MEMIF_IDLE -> The FEE module is currently idle.
* Description: Service to return the status.  
********************************************************************************/
MemIf_StatusType Fee_GetStatus(void);	
/*******************************************************************************
* Service Name: Fee_GetJobResult
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None 
* Parameters (inout): None
* Parameters (out): None
* Return value: MemIf_JobResultType :           MEMIF_JOB_OK ->  The last job has been finished successfully
                                                MEMIF_JOB_PENDING ->The last job is waiting for execution or currently being executed. 
                                                MEMIF_JOB_CANCELED ->The last job has been canceled (which means it failed).
                                                MEMIF_JOB_FAILED ->The last job has not been finished successfully (it failed). 
                                                MEMIF_BLOCK_INCONSISTENT ->  The requested block is inconsistent, it may contain corrupted data. 
                                                MEMIF_BLOCK_INVALID ->  The requested block has been invalidated, the requested read operation can not be performed. 
* Description: Service to query the result of the last accepted job issued by the upper layer software.  
********************************************************************************/
MemIf_JobResultType Fee_GetJobResult(void);	
/*******************************************************************************
* Service Name: Fee_GetJobResult
* Sync/Async: ASynchronous
* Reentrancy: Non-reentrant
* Parameters (in): blockNumber -> Number of logical block, also denoting start address of that block in flash memory 
* Parameters (inout): None
* Parameters (out): None
* Return value: Std_ReturnType :   E_OK: The requested job has been accepted by the module.  
*                                  E_NOT_OK:  The requested job has not been accepted by the module.
*
* Description: Service to invalidate a logical block.  
********************************************************************************/
Std_ReturnType Fee_InvalidateBlock(uint16 blockNumber);	
/* Not Supportted */
Std_ReturnType Fee_EraseImmediateBlock(uint16 blockNumber);

/*
 * Make the Fee_Config visible for others.
 */
extern const Fee_ConfigType Fee_Config;
#endif /*FEE_H_*/