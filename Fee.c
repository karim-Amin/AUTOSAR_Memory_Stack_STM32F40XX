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

	FEE_WRITE,
	
	FEE_ERASE_BANK, // request bank erase
	

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
		uint8* DataPtr; // write ram buffer ptr
		uint16			BlockIdx; // write block index
		uint8			Invalidate; // invalidate flag
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
 BlockStartAddress[0] = FLS_BASE_ADDRESS + BankProp[0].Start;
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