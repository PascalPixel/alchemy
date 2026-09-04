#include "types.h"

extern u8 * Func_02001a82(int);
extern void Func_02001a9a(int,int,int);
extern void Func_02001a48(u8 *,int);
extern void Func_02001aca(int,int,int);
extern void Func_02001af4(int,int,int);
extern void Func_02001a34(int);
extern void Func_02001a84(u8 *,int);
void Func_02000970(int actor, int x, int z, int field40)
{
    u8 *record = Func_02001a82(actor); int frames;
    Func_02001a9a(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Func_02001a48(record, 0);
    Func_02001aca(actor, x, z); Func_02001af4(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Func_02001a34(1); if (*(s16 *)(record + 42) == 0) break; }
    Func_02001a84(record, 1); *(s32 *)(record + 72) = 0x10000;
}
