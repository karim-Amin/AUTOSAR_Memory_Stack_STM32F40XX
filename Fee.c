/******************************************************************************
 *
 * Module: flash memory
 *
 * File Name: Fee.c
 *
 * Description: Source file for STM32F405RG Microcontroller - Flash memory Driver
 *
 * Author: Karim Mohamed Amin
 ******************************************************************************/
#include "Fee.h"
#include "Fee_Cbk.h"
#include "Det.h"

/* Private Data Types */
/*
 * Variables for the current job
 */
typedef enum {
	FEE_UNINITIALIZED = 0,
        
	FEE_IDLE,
        
	FEE_READ, 
        FEE_READ_WAIT, // wait data read done

	FEE_WRITE,
	FEE_WRITE_WAIT, // wait data write done
        
        
        
	FEE_ERASE, // request bank erase
	FEE_ERASE_WAIT, // wait bank erase done

} CurrentJobStateType;

/*
 * Variables for flash administration
 */
typedef struct {
	Fls_AddressType		Start;
	Fls_LengthType		End;
} BankPropType;

enum {
	NO_JOB = 0,
	PENDING_READ_JOB = 1 << 0,
	PENDING_WRITE_JOB = 1 << 1,
	PENDING_ERASE_JOB = 1 << 2,
};
typedef uint8 PendingJobType;
typedef union {
	struct {
		uint8*                  DataPtr; // read ram buffer ptr
		uint16			BlockIdx; // read block index
		uint16			Length; // read block length
		uint16      	        DataOffset; // read data offset
	} Read;
	struct {
		uint8*                  DataPtr; // write ram buffer ptr
		uint16			BlockIdx; // write block index
	}Write;
        
        struct{
          uint16                        BlockIdx;
        }Erase;
} CurrentJobType;

/* this list holds the start and the end addresses of each bank */
STATIC const BankPropType BankProp[NUM_OF_BANKS] = {
		{
				.Start = FEE_BANK1_OFFSET,
				.End = FEE_BANK1_OFFSET + FEE_BANK1_LENGTH
		},
		{
				.Start = FEE_BANK2_OFFSET,
				.End = FEE_BANK2_OFFSET + FEE_BANK2_LENGTH
		},
};
/* Find the start address for each Block based on banks */
STATIC  Fls_AddressType BlockStartAddress[FEE_NUM_OF_BLOCKS];
/*
 * Variables for quick reporting of status and job result
 */
STATIC MemIf_StatusType 	ModuleStatus = MEMIF_UNINIT;
STATIC MemIf_JobResultType 	JobResult = MEMIF_JOB_OK;
STATIC CurrentJobStateType      CurrentJobStatus = FEE_UNINITIALIZED;
STATIC PendingJobType           PendingJob = NO_JOB;
STATIC CurrentJobType 		CurrentJob;
STATIC uint8                    FailCounter = 0; 

/******************************************************************************
 *                      Helper Functions  Definitions                         *
*******************************************************************************/
STATIC sint16 Get_Block_Idx_From_Block_Number(uint16 block_num)
{

  for(uint8 count = 0;count<FEE_NUM_OF_BLOCKS;count++)
  {
    if(Fee_Config.BlockConfig[count].BlockNumber == block_num)
    {
     return count;
    }
  }
  return -1;
}
  
STATIC boolean CheckFlsJobFinished(void)
{
	MemIf_JobResultType flsJobResult;

	flsJobResult = Fls_GetJobResult();
	return (flsJobResult != MEMIF_JOB_PENDING);
}

STATIC void FinishJob(MemIf_JobResultType jobResult)
{
	/* The Module will be idle */
        CurrentJobStatus = FEE_IDLE;
	
        ModuleStatus = MEMIF_IDLE;
	
        JobResult = jobResult;
	if (jobResult == MEMIF_JOB_OK) {
          /* Call the nvm notifications if configured */
          if (Fee_Config.General.NvmJobEndCallbackNotificationCallback != NULL_PTR) {
		Fee_Config.General.NvmJobEndCallbackNotificationCallback();
          }
	}
	else {
          if (Fee_Config.General.NvmJobErrorCallbackNotificationCallback != NULL_PTR) {
		Fee_Config.General.NvmJobErrorCallbackNotificationCallback();
          }
	}
}
STATIC void AbortWriteJob(void) {
	
	// increase error counter
	FailCounter++;
	if (FailCounter > MAX_NOF_FAILED_WRITE_ATTEMPTS) {
		FinishJob(MEMIF_JOB_FAILED);
	}
	else {
		CurrentJobStatus = FEE_IDLE; // set idle state to restart write job
	}
}

STATIC void Idle(void)
{
  
}
STATIC void Reading(void)
{
  Fls_AddressType addr = BlockStartAddress[CurrentJob.Read.BlockIdx];
  if( Fls_GetStatus() == MEMIF_IDLE)
  {
    /* Initiate the read tassk in underluing flash memory */
    Std_ReturnType ret = Fls_Read(addr + CurrentJob.Read.DataOffset,CurrentJob.Read.DataPtr,CurrentJob.Read.Length);
    if (ret == E_OK) {
      
	CurrentJobStatus = FEE_READ_WAIT;
    }
    else {
	FinishJob(MEMIF_BLOCK_INCONSISTENT);
    }
  }
}
STATIC void ReadWait(void)
{
  if (CheckFlsJobFinished()) {
	MemIf_JobResultType jobResult = Fls_GetJobResult();
	PendingJob &= ~PENDING_READ_JOB;
	if (jobResult == MEMIF_JOB_OK) {
		FinishJob(MEMIF_JOB_OK);
	}
	else {
		FinishJob(MEMIF_BLOCK_INCONSISTENT);
	}
    }
}
STATIC void WriteData(void)
{
  if (Fls_GetStatus() == MEMIF_IDLE) {
    
        /* Get the address of the wanted block*/
        Fls_AddressType address = BlockStartAddress[CurrentJob.Write.BlockIdx];
	Std_ReturnType ret = Fls_Write(address ,CurrentJob.Write.DataPtr,Fee_Config.BlockConfig[CurrentJob.Write.BlockIdx].BlockSize);
	if (ret == E_OK) {
		CurrentJobStatus = FEE_WRITE_WAIT ;
	}
	else {
		AbortWriteJob();
	}
    } 
}
STATIC void WriteDataWait(void)
{
  MemIf_JobResultType readResult;
  /* Check if the job Finished in flash memory */
  if (CheckFlsJobFinished()) {
	readResult = Fls_GetJobResult();
	if (MEMIF_JOB_OK == readResult) {
          
            PendingJob &= ~PENDING_WRITE_JOB;
            FinishJob(MEMIF_JOB_OK);
	}
	else {
            AbortWriteJob();
	}
  }
}
STATIC void Erase(void)
{
  uint8 block_index = CurrentJob.Erase.BlockIdx;
  Fls_AddressType block_address = BlockStartAddress[block_index];
  if (Fls_Erase(block_address,Fee_Config.BlockConfig[block_index].BlockSize) == E_OK) {
	CurrentJobStatus = FEE_ERASE_WAIT;
 }
}
STATIC void EraseWait(void)
{
  MemIf_JobResultType readResult;
  if (CheckFlsJobFinished()) {
      readResult = Fls_GetJobResult();
      if (MEMIF_JOB_OK == readResult) {
        PendingJob &= ~PENDING_ERASE_JOB;
        FinishJob(MEMIF_JOB_OK);
  }
  else {
    // failed to erase, set mode to idle to restart
    CurrentJobStatus = FEE_IDLE;
    // increase error counter
    FailCounter++;
		}
	}
}
/******************************************************************************
 *                               APIS   Definitions                           *
*******************************************************************************/
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
void Fee_Init(void)
{
  /* Initialize the internal variables */
  ModuleStatus = MEMIF_IDLE;
  /* Set the current job to FEE IDLE */
 CurrentJobStatus = FEE_IDLE;
 /* Find the Start Address for ecah block */
 uint8 count  ;
 /* To Hold the prevous value of the counter */
 uint8 prev ;
 /* Set the first start address to the address of the first bank */
 BlockStartAddress[0] =  BankProp[0].Start;
 for(count = 1 ; count<FEE_NUM_OF_BLOCKS ;count++)
 {
   prev = count -1 ;
   BlockStartAddress[count] = BlockStartAddress[prev]  + Fee_Config.BlockConfig[prev].BlockSize;
 }
}
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
void Fee_SetMode(MemIf_ModeType mode)
{
  /* Check the development error */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
  if(ModuleStatus == MEMIF_UNINIT)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_SET_MODE_ID,FEE_E_UNINIT);
    return ;
  }else{
    /* Do Nothing */
  }
   if(ModuleStatus == MEMIF_BUSY)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_SET_MODE_ID,FEE_E_BUSY);
    return ;
  }else{
    /* Do Nothing */
  }
 
#endif
  
  if(ModuleStatus == MEMIF_IDLE)
  {
    Fls_SetMode(mode);
  }else{
    /* Do Nothing */
  }
  
}


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
                        uint16 length)
{
    /* Check the development error */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
  if(ModuleStatus == MEMIF_UNINIT)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_READ_ID,FEE_E_UNINIT);
    return E_NOT_OK;
  }else{
    /* Do Nothing */
  }
   if(ModuleStatus == MEMIF_BUSY)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_READ_ID,FEE_E_BUSY);
    return E_NOT_OK;
  }else{
    /* Do Nothing */
  }
  boolean block_number_valid_flag = FALSE;
   boolean bolck_offest_valid_flag = FALSE;
   uint8 Index;
   uint16 size;
  /* check that the block nuber is valid */
  for(uint8 i=0 ;i<FEE_NUM_OF_BLOCKS;i++)
  {
    if(blockNumber == Fee_Config.BlockConfig[i].BlockNumber)
    {
      block_number_valid_flag = TRUE;
      Index = i;
      size = Fee_Config.BlockConfig[i].BlockSize;
      if(blockOffset < Fee_Config.BlockConfig[i].BlockSize){
        bolck_offest_valid_flag = TRUE;
      }
    }
  }
  if( block_number_valid_flag == FALSE)
  {
     Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_READ_ID,FEE_E_INVALID_BLOCK_NO);
    return E_NOT_OK;
  }else {
    /* Do Nothing */
  }
 if( bolck_offest_valid_flag == FALSE)
  {
     Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_READ_ID,FEE_E_INVALID_BLOCK_OFS);
    return E_NOT_OK;
  }else {
    /* Do Nothing */
  }
  if( dataBufferPtr == NULL_PTR)
  {
     Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_READ_ID,FEE_E_INVALID_DATA_PTR);
    return E_NOT_OK;
  }else {
    /* Do Nothing */
  }
  /* Check that the length is valid */
  if( (length + blockOffset) > (size + BlockStartAddress[Index]) )
  {
     Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_READ_ID,FEE_E_INVALID_BLOCK_LEN);
    return E_NOT_OK;
  }else {
    /* Do Nothing */
  }
#endif
  /* Accept the read operation if the staus is idle*/
  if(ModuleStatus == MEMIF_IDLE)
  {
    uint16 block_idx = Get_Block_Idx_From_Block_Number(blockNumber);
    ModuleStatus = MEMIF_BUSY;
    JobResult = MEMIF_JOB_PENDING;
    CurrentJobStatus = FEE_READ;
    PendingJob |= PENDING_READ_JOB;
    CurrentJob.Read.BlockIdx = block_idx;
    CurrentJob.Read.DataOffset = blockOffset;
    CurrentJob.Read.DataPtr = dataBufferPtr;
    CurrentJob.Read.Length = length;
  }
  return E_OK;
}

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
Std_ReturnType Fee_Write(
                         uint16 blockNumber,
                         uint8* dataBufferPtr)
{
    /* Check the development error */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
  if(ModuleStatus == MEMIF_UNINIT)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_WRITE_ID,FEE_E_UNINIT);
    return E_NOT_OK;
  }else{
    /* Do Nothing */
  }
   if(ModuleStatus == MEMIF_BUSY)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_WRITE_ID,FEE_E_BUSY);
    return E_NOT_OK;
  }else{
    /* Do Nothing */
  }
  
  if( Get_Block_Idx_From_Block_Number(blockNumber) == -1 )
  {
     Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_WRITE_ID,FEE_E_INVALID_BLOCK_NO);
    return E_NOT_OK;
  }else {
    /* Do Nothing */
  }
  if( dataBufferPtr == NULL_PTR)
  {
     Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_WRITE_ID,FEE_E_INVALID_DATA_PTR);
    return E_NOT_OK;
  }else {
    /* Do Nothing */
  }
 
#endif
  
   /* Accept the Write operation if the staus is idle*/
  if(ModuleStatus == MEMIF_IDLE)
  {
    uint16 block_idx = Get_Block_Idx_From_Block_Number(blockNumber);
    ModuleStatus = MEMIF_BUSY;
    JobResult = MEMIF_JOB_PENDING;
    CurrentJobStatus = FEE_WRITE;
    PendingJob |= PENDING_WRITE_JOB;
    CurrentJob.Write.BlockIdx= block_idx;
    CurrentJob.Write.DataPtr = dataBufferPtr;
  }
  return E_OK;
}

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
void Fee_Cancel(void)
{
    /* Check the development error */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
  if(ModuleStatus == MEMIF_UNINIT)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_CANCEL_ID,FEE_E_UNINIT);
    return ;
  }else{
    /* Do Nothing */
  }
#endif
  
  if(ModuleStatus == MEMIF_BUSY)
  {
     /* Initialize the internal variables */
    ModuleStatus = MEMIF_IDLE;
    /* Set the current job to FEE IDLE */
   CurrentJobStatus = FEE_IDLE;
   Fls_Cancel();
  }
}

/*******************************************************************************
* Service Name: Fee_MainFunction
* Timing : ON_PRE_CONDITION 
* Description: Service to handle the requested read / write / erase jobs and the internal management operations.
********************************************************************************/
void Fee_MainFunction(void)
{
  switch (CurrentJobStatus)
  {
  case FEE_IDLE:
    Idle();
    break;
  case FEE_READ:
    Reading();
    break;
  case FEE_READ_WAIT:
    ReadWait();
    break;
  case FEE_WRITE:
    WriteData();
    break;
  case FEE_WRITE_WAIT:
    WriteDataWait();
    break;
  case FEE_ERASE:
    Erase();
    break;
  case FEE_ERASE_WAIT:
    EraseWait();
    break;
  default:
    break;
  }
}
/*******************************************************************************
* Service Name: Fee_InvalidateBlock
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
Std_ReturnType Fee_InvalidateBlock(uint16 blockNumber)
{
   /* Check the development error */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
  if(ModuleStatus == MEMIF_UNINIT)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_INVALIDATE_BLOCK_ID,FEE_E_UNINIT);
    return E_NOT_OK;
  }else{
    /* Do Nothing */
  }
   if(ModuleStatus == MEMIF_BUSY)
  {
    Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_INVALIDATE_BLOCK_ID,FEE_E_BUSY);
    return E_NOT_OK;
  }else{
    /* Do Nothing */
  }
   if( Get_Block_Idx_From_Block_Number(blockNumber) == -1 )
  {
     Det_ReportError(FEE_MODULE_ID,FEE_INSTANCE_ID,FEE_INVALIDATE_BLOCK_ID,FEE_E_INVALID_BLOCK_NO);
    return E_NOT_OK;
  }else {
    /* Do Nothing */
  }
#endif 
  if(ModuleStatus == MEMIF_IDLE)
  {
    /* To Invalidate the block initiate erase operation */
    uint16 block_idx = Get_Block_Idx_From_Block_Number(blockNumber);
    ModuleStatus = MEMIF_BUSY;
    JobResult = MEMIF_JOB_PENDING;
    CurrentJobStatus = FEE_ERASE;
    PendingJob |= PENDING_ERASE_JOB;
    CurrentJob.Erase.BlockIdx=block_idx;
    return E_OK;
  }else{
    return E_NOT_OK;
  }
}
