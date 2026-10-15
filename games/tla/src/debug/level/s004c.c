#include "types.h"
#include "party_state.h"

extern const u8 Data_0200082c[];
extern const u8 Data_020008a4[];
extern const u8 Data_02000a0c[];
extern const u8 Data_02000b74[];
extern const u8 Data_02000cf4[];

const u8 *Func_0200004c(void)
{
    if (gPartyState.entrance == 99)
        return Data_020008a4;
    if (gPartyState.entrance == 98)
        return Data_02000a0c;
    if (gPartyState.entrance == 97)
        return Data_02000b74;
    if (gPartyState.entrance == 96)
        return Data_02000cf4;
    return Data_0200082c;
}
