#include "alhafra_interiors.h"

u8 *Func_0200004c(void)
{
    if (GameFlag_Test(0x8a7))
        return Data_0200116c;
    if (GameFlag_Test(0x8ab))
        return Data_02000f14;
    return Data_02000cbc;
}
