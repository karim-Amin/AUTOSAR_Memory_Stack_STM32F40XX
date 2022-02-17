/******************************************************************************
 *
 * Module: flash memory
 *
 * File Name: fls.c
 *
 * Description: Source file for STM32F405RG Microcontroller - Flash memory Driver
 *
 * Author: Karim Mohamed Amin
 ******************************************************************************/
#include "fls.h"
#include "fls_reg.h"

/* check as it is optional feature */
#if (FLS_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and flash Modules */
#if ((DET_AR_MAJOR_VERSION != FLS_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != FLS_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != FLS_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of fls.h does not match the expected version"
#endif

#endif
/******************************************************************************
 *                      Private Global variable                               *
******************************************************************************/
   
STATIC MemIf_StatusType Flash_Status = MEMIF_UNINIT;
STATIC MemIf_ModeType Flash_Mode;
STATIC const Fls_configType* Fls_config_ptr = NULL_PTR;

/******************************************************************************
 *                      API Service Definitions                               *
******************************************************************************/

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
void Fls_Init( const Fls_configType  * config_ptr){
/* Check the development errors if enabled */
#if (FLS_DEV_ERROR_DETECT == STD_ON)
  /* Check the content of the given configuration set to be valid  */
  if( (config_ptr->fls_call_cycle > FLS_MAX_CALL_CYCLE)\
     || (config_ptr->fls_p_size > x64_psize)\
     || (config_ptr->fls_BOR_level > BOR_off)\
     || (config_ptr->fls_latancy > seven_ws))
  {
    /* Report dev error if the contents are invalid */
    Det_ReportError(FLS_MODULE_ID,FLS_INSTANCE_ID,FLS_INIT_SID,FLS_E_PARAM_CONFIG);
  }else{
    /* Do nothing */
  }
  /* Check the flash module status */
  if(Flash_Status == MEMIF_BUSY )
  {
     /* Report dev error if the flash module status */
    Det_ReportError(FLS_MODULE_ID,FLS_INSTANCE_ID,FLS_INIT_SID,FLS_E_BUSY);
  }else{
    /* Do nothing */
  }
  
  /* Check if the pointer is null */
  if(config_ptr == NULL_PTR)
  {
    /* Report dev error if the config ptr is null */
    Det_ReportError(FLS_MODULE_ID,FLS_INSTANCE_ID,FLS_INIT_SID,FLS_E_PARAM_POINTER);
  }else
#endif  
  {
    Fls_config_ptr = config_ptr;
    /* After finishing initialization */
    Flash_Status = MEMIF_IDLE;
  }
  /* Insert in the access control register how many wait states */
  FLASH->ACR = (FLASH->ACR & FLS_ACR_MASK)|(config_ptr->fls_latancy);
  
  /* Configure the data and instruction caches and Prefetch */
  if( config_ptr->fls_data_cache_enable == TRUE){

  /* SET DCEN bit => BIT NUMBER 10 in ACR register */
  SET_BIT(FLASH->ACR,FLS_BIT_NUMBER_10);
  }
  else if( config_ptr->fls_data_cache_enable == FALSE)
  {
    
  /* CLEAR DCEN bit in ACR register */
  CLEAR_BIT(FLASH->ACR,FLS_BIT_NUMBER_10);
  }else{
    
    /*do nothing */ 
  }
  
   /* Configure the data and instruction caches and Prefetch */
  if( config_ptr->fls_instruction_cache_enable == TRUE){

  /* SET ICEN bit number 9 in ACR register */
  SET_BIT(FLASH->ACR,FLS_BIT_NUMBER_9);
  }
  else if( config_ptr->fls_data_cache_enable == FALSE)
  {
    
  /* CLEAR ICEN bit number 9 in ACR register */
  CLEAR_BIT(FLASH->ACR,FLS_BIT_NUMBER_9);
  }else{
    
    /*do nothing */ 
  }
  
   /* Configure the data and instruction caches and Prefetch */
  if( config_ptr->fls_prefetch_enable == TRUE){

  /* SET PRFTEN bit number 8 in ACR register */
  SET_BIT(FLASH->ACR,FLS_BIT_NUMBER_8);
  }
  else if( config_ptr->fls_data_cache_enable == FALSE)
  {
    
  /* CLEAR PRFTEN bit number 8 in ACR register */
  CLEAR_BIT(FLASH->ACR,FLS_BIT_NUMBER_8);
  }else{
    
    /*do nothing */ 
  }
  /* UNLOCK the flash control register by put in the flash key register two magic numbers
   * 1=> KEY1 = 0x45670123
   * 2=> KEY2 = 0xCDEF89AB
   */
  FLASH->KEYR = FLS_UNLOCK_CR_KEY1;
  FLASH->KEYR = FLS_UNLOCK_CR_KEY2;
  
  /* check the usage of the interrupts */
#if (FLS_USE_INTERRUPTS == TRUE)
  FLASH->CR  |= FLS_INTERRUPT_ENABLE_MASK;
#endif
  /* Set the parallelism size which start from bit number 8 */
  FLASH->CR |= (config_ptr->fls_p_size) << FLS_BIT_NUMBER_8;
  /* Lock again the control register for security (LOCK bit in 31 position) */
  SET_BIT(FLASH->CR , FLS_BIT_NUMBER_31);
  /* unlock the option control register 
   * 1=> KEY1 = 0x45670123
   * 2=> KEY2 = 0xCDEF89AB
   */
  FLASH->OPTKEYR = FLS_UNLOCK_OPTION_CR_KEY1;
  FLASH->OPTKEYR = FLS_UNLOCK_OPTION_CR_KEY2;
  /* Check the Write protection option */
  if( config_ptr->fls_write_protection_enable == TRUE )
  {
    /* put zeros in the bits number [27 : 16] */
    FLASH->OPTCR &= FLS_WRITE_PROTECTION_MASK; 
  }else{
    /* put ones in the bits number [27 : 16] */
    FLASH->OPTCR |= ~(FLS_WRITE_PROTECTION_MASK);
  }
  /* set the Read protection option ( The Start position of read protection is bit number 8 )*/
  FLASH->OPTCR =  (FLASH->OPTCR & FLS_READ_PROTECTION_MASK) | ((config_ptr->fls_read_protection) << FLS_BIT_NUMBER_8);
  /* Set the brownout level ( The Start position of BOR level is bit number 2 ) */
  FLASH->OPTCR = (FLASH->OPTCR & FLS_BOR_LEVEL_MASK) | ((config_ptr->fls_BOR_level) << FLS_BIT_NUMBER_2);
  /* trigger user option operation The START BIT is the bit number 1 in OPTCR register */
  SET_BIT(FLASH->OPTCR,FLS_BIT_NUMBER_1);
  /* wait untill that operation compelete as it is Synchronous function */
  /* check the BSY bit that is bit number 16 in STATUS register */
  while(BIT_IS_SET(FLASH->SR,FLS_BIT_NUMBER_16));
  /* Unlock the option control register for security by setting OPTION LOCK BIT which is bit number zero */
  SET_BIT(FLASH->OPTCR,FLS_BIT_NUMBER_1);
}