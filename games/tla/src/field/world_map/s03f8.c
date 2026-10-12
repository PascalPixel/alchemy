#include "types.h"
#include "party_state.h"

extern u8 Data_02006338[];
extern u8 Data_02006500[];
extern u8 Data_02006598[];
extern u8 Data_02006700[];
extern u8 Data_02006790[];

s32 GameFlag_Test(s32 flag);

u8 *Func_020003f8(void)
{
    switch (gPartyState.entrance) {
    case 64:
        return Data_02006500;
    case 65:
        return Data_02006598;
    case 68:
        if (!GameFlag_Test(0x8de))
            return Data_02006700;
    case 77:
        return Data_02006790;
    default:
        return Data_02006338;
    }
}
