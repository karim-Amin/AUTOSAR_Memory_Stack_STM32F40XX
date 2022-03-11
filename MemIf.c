/******************************************************************************
 *
 * Module: Memory Abstraction Interface
 *
 * File Name: MemIf.c
 *
 * Description: Source file for MEMIF_MODULE
 *
 * Author: Karim Mohamed Amin
 ******************************************************************************/
#include "MemIf.h"

/******************************************************************************
 *                      API Service Definitions                               *
******************************************************************************/
INLINE void MemIf_SetMode(MemIf_ModeType  Mode)
{
  Fee_SetMode(Mode);
}
INLINE Std_ReturnType MemIf_Read(
                                 uint8 DeviceIndex,
                                 uint16 BlockNumber,
                                 uint16 BlockOffset,
                                 uint8* DataBufferPtr,
                                 uint16 Length
                                   )
{
  
  return Fee_Read(BlockNumber,BlockOffset,DataBufferPtr,Length);
}
INLINE Std_ReturnType MemIf_Write(
                                 uint8 DeviceIndex,
                                 uint16 BlockNumber,
                                 const uint8* DataBufferPtr
                                   )
{
  return Fee_Write(BlockNumber,(uint8*)DataBufferPtr);
}
INLINE void MemIf_Cancel(uint8 DeviceIndex)
{
  Fee_Cancel();
}
INLINE MemIf_StatusType MemIf_GetStatus(uint8 DeviceIndex)
{
  return Fee_GetStatus();
}
INLINE MemIf_JobResultType MemIf_GetJobResult(uint8 DeviceIndex)
{
  return Fee_GetJobResult();
}
INLINE Std_ReturnType MemIf_InvalidateBlock(
                                uint8 DeviceIndex,
                                uint16 BlockNumber
                                    )
{
  return Fee_InvalidateBlock(BlockNumber);
}