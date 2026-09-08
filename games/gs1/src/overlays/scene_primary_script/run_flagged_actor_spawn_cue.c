#include "types.h"

#define FieldScene_RunFlaggedActorSpawnCue Func_02001d78

struct SpawnArgs {
    u8 kind;
    u8 pad01[3];
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    s32 f14;
};

extern void Func_02001c18(s32, s32, s32, s32, s32, s32, s32, struct SpawnArgs *);
s32 Func_02004026(void);
s32 Func_02004034(void);
void Func_020041ba(s32);

void FieldScene_RunFlaggedActorSpawnCue(s32 a0, s32 a1)
{
    s32 gate;
    s32 v0;
    s32 v1;
    s32 term1;
    s32 term2;
    struct SpawnArgs record;

    gate = *(volatile s32 *)0x03001e40;
    if ((gate & 3) == 0) {
        record.f04 = 10;
        record.f08 = 0x8000;
        record.f0c = 0x8000;
        record.f10 = 0x1cccc;
        record.f14 = 0x1cccc;
        if ((*(volatile s32 *)0x03001e40 & 7) == 0) {
            Func_020041ba(136);
        }
        v0 = Func_02004026();
        term1 = -0x10000 - (((u32)(v0 << 1) >> 16) << 16);
        v1 = Func_02004034();
        term2 = -(((((u32)((v1 << 1) + v1) >> 16) * 3) * 17) * 257);
        Func_02001c18(0x1340000, 0x400000, 0xde0000, term1, term2, (gate & 3), 0xd0001, &record);
    }
}
