/* Near misses of Func_0200051c and Func_020005b8 of overlay 658 (60 and 51 instructions, 4
 * differ each): in the two four-argument calls the native loads the last argument's byte
 * first, then the two position bytes, then shifts in the same order; with the byte behind a
 * "+r" barrier, as below, its shift comes after the positions'. Measured: direct calls and
 * shared constants 30 differ; flags and the other calls through the inline layer 8; the whole
 * last argument behind the barrier shifts it too early (4). */
#include "types.h"
#include "object_runtime.h"
#include "flag_call.h"
#include "call.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void Func_02000404(void);
void Func_02000490(void);
void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002b90(s32 id, s32 x, s32 z, s32 a);
void Func_02002bf8(s32 id, s32 a);
void Func_02002c30(s32 a);
void Func_02002c58(s32 a);
void Func_02002c90(s32 a);

void Func_0200051c(void)
{
    s32 a;

    if (Flag_IsSet(0x910)) {
        Func_02002c90(123);
        Func_02002c30(16);
    } else if (!Flag_IsSet(0x280)) {
        Func_02002b28();
        Func_02002c58(0);
        a = 128;
        /* FAKEMATCH: the native loads the last argument's byte before the two positions and shifts it after theirs; a direct call starts it after them, a forwarding inline after the first. */
        __asm__ volatile("" : "+r"(a));
        Func_02002b90(26, 744 << 16, 0x01010000, a << 7);
        Object_GetById(4)->velocity_y = 0x50000;
        Call3((void (*)())ObjectMotion_SetSpeedParameters, 4, 0x20000, 0x10000);
        Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, 720, 244);
        Call2((void (*)())Func_02002bf8, 4, 0x2000);
        a = 128;
        /* FAKEMATCH: the native loads the last argument's byte before the two positions and shifts it after theirs; a direct call starts it after them, a forwarding inline after the first. */
        __asm__ volatile("" : "+r"(a));
        Func_02002b90(4, 720 << 16, 244 << 16, a << 6);
        Flag_Set(0x280);
        Func_02000404();
        Func_02002b30();
    }
}

void Func_020005b8(void)
{
    s32 a;

    if (!Flag_IsSet(0x280)) {
        Func_02002b28();
        Func_02002c58(0);
        a = 128;
        /* FAKEMATCH: the native loads the last argument's byte before the two positions and shifts it after theirs; a direct call starts it after them, a forwarding inline after the first. */
        __asm__ volatile("" : "+r"(a));
        Func_02002b90(26, 744 << 16, 256 << 16, a << 7);
        Object_GetById(4)->velocity_y = 0x50000;
        Call3((void (*)())ObjectMotion_SetSpeedParameters, 4, 0x20000, 0x10000);
        Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, 720, 244);
        Call2((void (*)())Func_02002bf8, 4, 0x2000);
        a = 128;
        /* FAKEMATCH: the native loads the last argument's byte before the two positions and shifts it after theirs; a direct call starts it after them, a forwarding inline after the first. */
        __asm__ volatile("" : "+r"(a));
        Func_02002b90(4, 720 << 16, 244 << 16, a << 6);
        Flag_Set(0x280);
        Func_02000490();
        Func_02002b30();
    }
}
