#include "fls.h"
int main(void)
{ 
  Fls_Init(&fls_configuration);
  Fls_Erase((Fls_AddressType)0x8000,(Fls_LengthType)32767);
  return 0;
}