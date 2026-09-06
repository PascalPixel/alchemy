#include "types.h"

/* Retained candidate for the complete owner ending at 0x020005dc. */
s32 Func_02000fc6(s32 flag);
void Func_02000fe2(s32 flag);
s32 Func_02000fd8(s32 flag);
void Func_02000ea2(s32 actor);
s32 Func_02000fe8(s32 flag);
void Func_02001068(s32 actor, s32 x, s32 y);
void Func_02001074(s32 actor, s32 x, s32 y);
s32 Func_0200100a(s32 flag);
s32 Func_02001014(s32 flag);
void Func_02001092(s32 actor, s32 x, s32 y);
void Func_020010c6(s32 actor, s32 value, u32 data);
s32 Func_02001034(s32 flag);
void Func_020010b6(s32 actor, s32 x, s32 y);
void Func_0200111a(s32 actor, s32 value, s32 time);
void Func_02001124(s32 actor, s32 value, s32 time);
s32 Func_02001062(s32 flag);
void Func_02000b92(void);

void Func_0200050c(void)
{
    if (Func_02000fc6(265) != 0)
        Func_02000fe2(512);
    if (Func_02000fd8(4050) == 0)
        Func_02000ea2(13);

    if (Func_02000fe8(2122) != 0) {
        Func_02001068(11, 0x1340000, 0x1070000);
        Func_02001074(12, 0x15a0000, 0x1070000);
        if (Func_0200100a(2127) == 0 && Func_02001014(2117) == 0) {
            Func_02001092(11, 0, 0);
            Func_020010c6(12, 0x10000, 0x02008bf4);
        }
    }
    if (Func_02001034(2117) != 0) {
        Func_020010b6(10, 0xe00000, 0x1240000);
        Func_0200111a(10, 0x4000, 0);
        Func_02001124(8, 0, 0);
        if (Func_02001062(2142) == 0)
            Func_02000b92();
    }
}
