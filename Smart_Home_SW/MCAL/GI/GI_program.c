
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "GI_interface.h"

#define  SREG		*((volatile u8*)0x5F)
void GI_voidEnable(void)
{
	SET_BIT(SREG,7);
}
void GI_voidDisable(void)
{
	CLR_BIT(SREG,7);
}