#include "TYPES.H"

struct Work {
    u8 unknown_000[448];
    s16 index;
};

extern struct Work gGameState;
#define PARTY_STATE gGameState
extern u8 Data_0809f1a8[];
extern u8 Data_02008000[];

void Resource_LoadCode(s32 first, s32 second);

void BattleFx_LoadResourceGroup(void)
{
    u8 *table = Data_0809f1a8;
    s16 index = PARTY_STATE.index;
    s16 value = *(s16 *)(table + index * 8);
    Resource_LoadCode(value, Data_02008000);
}
