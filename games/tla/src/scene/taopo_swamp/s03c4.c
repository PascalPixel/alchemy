#include "local.h"

struct PartyState254 {
    u8 unk000[0x254];
    u16 value;
};

void Func_020003c4(void)
{
    Func_02002990_(76, 70, 14, 8, 14, 37, 0);
    if (!GameFlag_Test(0x9b6)) {
        Func_020029a8_(249, 344, 648);
        ((struct PartyState254 *)&gPartyState)->value = 0x19b6;
        gPartyState.battleRule24b = 2;
        Func_02002938(106, 1);
    }
}
