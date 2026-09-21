#include "TYPES.H"

struct Work {
    u8 unknown_000[448];
    s16 index;
};

extern struct Work Data_02000240;
#define PARTY_STATE Data_02000240
extern u8 Data_0809f1a8[];
extern u8 Data_02008000[];

void Func_08002fb0(s32 first, s32 second);

void BattleFx_LoadResourceGroup(void)
{
    u8 *table = Data_0809f1a8;
    s16 index = PARTY_STATE.index;
    s16 value = *(s16 *)(table + index * 8);
    Func_08002fb0(value, Data_02008000);
}
