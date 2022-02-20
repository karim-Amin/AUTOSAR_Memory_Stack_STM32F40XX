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
/* To Be able to track the module status */
STATIC MemIf_StatusType g_Flash_Status = MEMIF_UNINIT;
/* To Be able to track the module Mode */
STATIC MemIf_ModeType g_Flash_Mode;
/* To Be able to track the result of initiated task  */
STATIC MemIf_JobResultType g_Flash_Job_Result ;
/* To Store the configuration set if wanted in run time */
STATIC const Fls_configType* g_Fls_config_ptr = NULL_PTR;
/* To make the main function Know the type of operation */
FLS_Kind_of_operation g_Fls_operation_type = NO_OPERATION;
/* To hold the data of the initiated task */
STATIC Fls_AddressType g_TargetAdderss;
STATIC Fls_AddressType g_SourceAdderss;
STATIC  const uint8* g_SourceAdderss_ptr;
STATIC  const uint8* g_TargetAdderss_ptr;
STATIC Fls_LengthType  g_Length;
/* To Hold the first sector number and number of sectors which included in the operation*/
STATIC uint8  g_First_Sector_number;
STATIC uint8  g_number_of_sectors = 0;
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
  if(g_Flash_Status == MEMIF_BUSY )
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
    g_Fls_config_ptr = config_ptr;
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
  /* lock the option control register for security by setting OPTION LOCK BIT which is bit number zero */
  SET_BIT(FLASH->OPTCR,FLS_BIT_NUMBER_0);
  /* Get the flash memory Mode ( Fast or slow )*/
  g_Flash_Mode = config_ptr->fls_default_mode ;
  /* After finishing initialization set the flash status  to MEMIF_IDLE */
  g_Flash_Status = MEMIF_IDLE;
  /* After finishing initialization set the flash job result to MEMIF_JOB_OK */
  g_Flash_Job_Result = MEMIF_JOB_OK ; 
}
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
                                                      )
{
  /* Check the development errors if enabled */
#if (FLS_DEV_ERROR_DETECT == STD_ON)
  
  /* Check if the flash memory module is initialized or not */
  if(g_Flash_Status == MEMIF_UNINIT)
  {
    /* Report dev error if the module is not initialized */
    Det_ReportError(FLS_MODULE_ID,FLS_INSTANCE_ID,FLS_ERASE_SID,FLS_E_UNINIT);
    return E_NOT_OK;
  }else{
     /* Do nothing */   
  }
  
  /* Check the flash module status */
  if(g_Flash_Status == MEMIF_BUSY )
  {
     /* Report dev error if the flash module status */
    Det_ReportError(FLS_MODULE_ID,FLS_INSTANCE_ID,FLS_ERASE_SID,FLS_E_BUSY);
    return E_NOT_OK;
  }else{
    /* Do nothing */
  }
  
  /* Check that the address lies within the specified lower and upper flash address boundaries */
  if( !( ((FLS_BASE_ADDRESS + TargetAdderss)>= FLS_BASE_ADDRESS) && ((FLS_BASE_ADDRESS + TargetAdderss)<= FLS_LAST_ADDRESS) ) )
  {
    /* Report dev error if the address is out of range */
    Det_ReportError(FLS_MODULE_ID,FLS_INSTANCE_ID,FLS_ERASE_SID,FLS_E_PARAM_ADDRESS);
    return E_NOT_OK;
  }else{
    /* Do nothing */
  }
  
  /* Check that the address plus the length lies within the specified lower and upper flash address boundaries */
  if( !( ((FLS_BASE_ADDRESS + TargetAdderss + Length)>= FLS_BASE_ADDRESS) && ((FLS_BASE_ADDRESS + TargetAdderss)<= FLS_LAST_ADDRESS) && Length > FLS_OPERATION_ZERO_LENGTH) )
  {
      /* Report dev error if the address is out of range */
    Det_ReportError(FLS_MODULE_ID,FLS_INSTANCE_ID,FLS_ERASE_SID,FLS_E_PARAM_LENGTH);
    return E_NOT_OK;
  }else{
    /* Do nothing */
  }
  
#endif
  /* Copy the method parameters to initiate erase job */
  g_TargetAdderss = TargetAdderss;
  g_Length = Length;
  /* iterator for the for loop which is looping untill the number of sectors */
  uint8 iterator;
  /* to hold the next start address of the sector */
  Fls_AddressType next_start_add = FLS_BASE_ADDRESS;
  /* declare the start address of each Sector */
  Fls_AddressType sectors_start_adresses[FLS_NUM_OF_SECTORS] ;
  
  for(iterator = 0; iterator < FLS_NUM_OF_SECTORS ; iterator++)
  {
    sectors_start_adresses[iterator] = next_start_add;

    switch(iterator){
      /* add the size of the sector to the base address (sector0 -> sector3 = 16 Kbytes)*/
    case FLS_SECTOR0:
    case FLS_SECTOR1:
    case FLS_SECTOR2:
    case FLS_SECTOR3: next_start_add += FLS_SECTOR0_SIZE;
      break;
      /* add the size of the sector to the base address ( sector4 = 16 Kbytes)*/
    case FLS_SECTOR4: next_start_add += FLS_SECTOR4_SIZE;
      break;
      /* add the size of the sector to the base address (sector5 -> sector11 = 128 Kbytes)*/
    case FLS_SECTOR5:
    case FLS_SECTOR6:
    case FLS_SECTOR7:
    case FLS_SECTOR8:
    case FLS_SECTOR9:
    case FLS_SECTOR10:
    case FLS_SECTOR11: next_start_add += FLS_SECTOR5_SIZE;
      break;
    }
  }
  /* make a loop to know the frist sector to erase and the number of sectors */
  for( iterator = 0;iterator < FLS_NUM_OF_SECTORS;iterator++ ){
    /* Check if it is not the last sector */
    if(iterator != FLS_SECTOR11){
      /* Check if the Target address within the sector range */
      if( ((TargetAdderss + FLS_BASE_ADDRESS) >= sectors_start_adresses[iterator])\
         &&((TargetAdderss + FLS_BASE_ADDRESS)< sectors_start_adresses[iterator + 1]))
      {
        /* Store the sector number */
        g_First_Sector_number = iterator;
        /* justficatoin: to end the loop once it finds the sector number */
        break;
      }
    }
    else{
      /* Store the sector number */
        g_First_Sector_number = FLS_SECTOR11;
    }
  }
  /* To Know how many sectors will be erased */
  for( iterator = g_First_Sector_number;iterator < FLS_NUM_OF_SECTORS;iterator++ ){
    /*  Length will be rounded up to the next full sector boundary */
    if( ((TargetAdderss + FLS_BASE_ADDRESS + Length) > sectors_start_adresses[iterator]) )
    {
      g_number_of_sectors++;
    }
  }
  /* Set the status of the module to Busy until it finishes */
  g_Flash_Status = MEMIF_BUSY;
  /* Set the job of the module to job pending */
  g_Flash_Job_Result = MEMIF_JOB_PENDING;
  /* Set the job Type to erase task */
  g_Fls_operation_type = ERASE_OPERATION;
  return E_OK;
}

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
                                                      )
{
  /* Check the development errors if enabled */
#if (FLS_DEV_ERROR_DETECT == STD_ON)
  
  /* Check if the flash memory module is initialized or not */
  if(g_Flash_Status == MEMIF_UNINIT)
  {
    /* Report dev error if the module is not initialized */
    Det_ReportError(FLS_MODULE_ID , FLS_INSTANCE_ID , FLS_WRITE_SID , FLS_E_UNINIT);
    return E_NOT_OK;
  }else{
     /* Do nothing */   
  }
  
  /* Check the flash module status */
  if(g_Flash_Status == MEMIF_BUSY )
  {
     /* Report dev error if the flash module status */
    Det_ReportError(FLS_MODULE_ID , FLS_INSTANCE_ID , FLS_WRITE_SID , FLS_E_BUSY);
    return E_NOT_OK;
  }else{
    /* Do nothing */
  }
  
  /* Check that the address lies within the specified lower and upper flash address boundaries */
  if( !( ((FLS_BASE_ADDRESS + TargetAdderss)>= FLS_BASE_ADDRESS) && ((FLS_BASE_ADDRESS + TargetAdderss)<= FLS_LAST_ADDRESS) ) )
  {
    /* Report dev error if the address is out of range */
    
    Det_ReportError(FLS_MODULE_ID , FLS_INSTANCE_ID , FLS_WRITE_SID , FLS_E_PARAM_ADDRESS);
    return E_NOT_OK;
  }else{
    /* Do nothing */
  }
  
  /* Check that the address plus the length lies within the specified lower and upper flash address boundaries */
  if( !( ((FLS_BASE_ADDRESS + TargetAdderss + Length)>= FLS_BASE_ADDRESS) && ((FLS_BASE_ADDRESS + TargetAdderss)<= FLS_LAST_ADDRESS) && Length > FLS_OPERATION_ZERO_LENGTH) )
  {
    /* Report dev error if the address is out of range */
    Det_ReportError(FLS_MODULE_ID,FLS_INSTANCE_ID,FLS_WRITE_SID,FLS_E_PARAM_LENGTH);
    return E_NOT_OK;
  }else{
  /* Do nothing */
  }
  
  /* Check if the data buffer pointer is being null pointer */
  if( SourceAddressPtr == NULL_PTR )
  {
  /* Report dev error if the data buffer pointer is null */
    
    Det_ReportError(FLS_MODULE_ID,FLS_INSTANCE_ID,FLS_WRITE_SID,FLS_E_PARAM_DATA);
  }else{
  /* Do nothing */
  }
#endif
  /* copy the given parameters to Fls module internal variables */
  g_TargetAdderss = TargetAdderss;
  g_SourceAdderss_ptr = SourceAddressPtr;
  g_Length = Length;
  /* Set the status of the module to Busy until it finishes */
  g_Flash_Status = MEMIF_BUSY;
  /* Set the job of the module to job pending */
  g_Flash_Job_Result = MEMIF_JOB_PENDING;
  /* Set the job Type to write task */
  g_Fls_operation_type = WRITE_OPERATION;
  return E_OK;
}

/*******************************************************************************
* Service Name: Fls_Read
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
                                                      )
{
    /* Check the development errors if enabled */
#if (FLS_DEV_ERROR_DETECT == STD_ON)
  
  /* Check if the flash memory module is initialized or not */
  if(g_Flash_Status == MEMIF_UNINIT)
  {
    /* Report dev error if the module is not initialized */
    Det_ReportError(FLS_MODULE_ID , FLS_INSTANCE_ID , FLS_READ_SID , FLS_E_UNINIT);
    return E_NOT_OK;
  }else{
     /* Do nothing */   
  }
  
  /* Check the flash module status */
  if(g_Flash_Status == MEMIF_BUSY )
  {
     /* Report dev error if the flash module status */
    Det_ReportError(FLS_MODULE_ID , FLS_INSTANCE_ID , FLS_READ_SID , FLS_E_BUSY);
    return E_NOT_OK;
  }else{
    /* Do nothing */
  }
  
  /* Check that the address lies within the specified lower and upper flash address boundaries */
  if( !( ((FLS_BASE_ADDRESS + SourceAdderss)>= FLS_BASE_ADDRESS) && ((FLS_BASE_ADDRESS + SourceAdderss)<= FLS_LAST_ADDRESS) ) )
  {
    /* Report dev error if the address is out of range */
    
    Det_ReportError(FLS_MODULE_ID , FLS_INSTANCE_ID , FLS_READ_SID , FLS_E_PARAM_ADDRESS);
    return E_NOT_OK;
  }else{
    /* Do nothing */
  }
  
  /* Check that the address plus the length lies within the specified lower and upper flash address boundaries */
  if( !( ((FLS_BASE_ADDRESS + SourceAdderss + Length)>= FLS_BASE_ADDRESS) && ((FLS_BASE_ADDRESS + SourceAdderss)<= FLS_LAST_ADDRESS) && Length > FLS_OPERATION_ZERO_LENGTH) )
  {
    /* Report dev error if the address is out of range */
    Det_ReportError( FLS_MODULE_ID , FLS_INSTANCE_ID , FLS_READ_SID , FLS_E_PARAM_LENGTH );
    return E_NOT_OK;
  }else{
  /* Do nothing */
  }
  
  /* Check if the data buffer pointer is being null pointer */
  if( TargetAddressPtr == NULL_PTR )
  {
  /* Report dev error if the data buffer pointer is null */
    
    Det_ReportError(FLS_MODULE_ID,FLS_INSTANCE_ID,FLS_READ_SID,FLS_E_PARAM_DATA);
  }else{
  /* Do nothing */
  }
#endif
  /* copy the given parameters to Fls module internal variables */
  g_SourceAdderss = SourceAdderss;
  g_TargetAdderss_ptr = TargetAddressPtr;
  g_Length = Length;
  /* Set the status of the module to Busy until it finishes */
  g_Flash_Status = MEMIF_BUSY;
  /* Set the job of the module to job pending */
  g_Flash_Job_Result = MEMIF_JOB_PENDING;
  /* Set the job Type to read task */
  g_Fls_operation_type = READ_OPERATION;
  return E_OK;
}
/*******************************************************************************
* Service Name: Fls_MainFunction
* Timing : Fixed_cyclic
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Performs the proccessing of jobs
********************************************************************************/
void Fls_MainFunction( void )
{
#if (FLS_DEV_ERROR_DETECT == STD_ON)
  
  /* Check if the flash memory module is initialized or not */
  if(g_Flash_Status == MEMIF_UNINIT)
  {
    
    /* Report dev error if the module is not initialized */
    Det_ReportError(FLS_MODULE_ID , FLS_INSTANCE_ID , FLS_MAIN_FUNCTION_SID , FLS_E_UNINIT);
    
  }else{
     /* Do nothing */   
  }
#endif
/* Check if the module proccessing is configured to Work with interrups or polling */
#if(FLS_USE_INTERRUPTS == STD_OFF)
  /* Check The job result before execute the function if the job is pending -> execute */
  if( g_Flash_Job_Result == MEMIF_JOB_PENDING )
  {
    /* this variable will be initialized once by to be add to the first sector number */
     STATIC uint8 sector_offest = FLS_ZERO_VALUE;
    /* Check To Know the Type of the Job */
    switch(g_Fls_operation_type)
    {
      case NO_OPERATION:
        
        break;
      case READ_OPERATION:
        
        break;
      case WRITE_OPERATION:
        
        break;
      case ERASE_OPERATION:
          
        /* Check that there are sectors to erase */
        if(g_number_of_sectors != FLS_ZERO_VALUE)
        {
          /* check if the flash module not busy By Check the bit number 16 (BSY BIT) in the status register */
          while(BIT_IS_SET(FLASH->SR , FLS_BIT_NUMBER_16));
          /* UNLOCK the flash control register by put in the flash key register two magic numbers
           * 1=> KEY1 = 0x45670123
           * 2=> KEY2 = 0xCDEF89AB
           */
          FLASH->KEYR = FLS_UNLOCK_CR_KEY1;
          FLASH->KEYR = FLS_UNLOCK_CR_KEY2;
          /* Select the sector number to erase */
          /* 
           * 1) clear the bits [6:3] which is SNB (sector number).
           * 2) add first sector number to sector offest to get the sector number 
           * 3) shift left by 3 as it is the start bit number of SNB bits
           */
          FLASH->CR = (FLASH->CR & FLS_CR_SNB_MASK) | ((g_First_Sector_number + sector_offest) << FLS_BIT_NUMBER_3);
          
          /* set the bit SER (sector erase ) which is bit number 1*/
          SET_BIT(FLASH->CR,FLS_BIT_NUMBER_1);
          
          /* Set the start bit to start the operation which is bit number 16 */
          SET_BIT(FLASH->CR,FLS_BIT_NUMBER_16);
          /* Lock again the control register for security (LOCK bit in 31 position) */
          SET_BIT(FLASH->CR , FLS_BIT_NUMBER_31);
          /* incerment the offest var. */
          sector_offest++;
          /* decrement the number of sectors by one */
          g_number_of_sectors--;
        }
          /* Check that if the erase task is finished and the flash job result is pending  */
        if( (g_number_of_sectors == FLS_ZERO_VALUE) && (g_Flash_Job_Result == MEMIF_JOB_PENDING) )
        {
          /* reset the value of the variable for future use */
          sector_offest = FLS_ZERO_VALUE;
          /* IF the Dev error enable Check by reading the target address and it must equal to flash erase cell (0xFFFFFFFF)*/
          g_Flash_Job_Result = MEMIF_JOB_OK;
          g_Flash_Status = MEMIF_IDLE;
          /* Call Job End notification Function in the configuration structure */
          /*************************************/
        }
        break;
      case COMPARE_OPERATION:
        
        break;
      }
  }
#endif




}