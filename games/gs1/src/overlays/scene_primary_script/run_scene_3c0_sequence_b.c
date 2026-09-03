#include "types.h"

#define FieldScene_RunActorThirteenRestoration Func_02000838

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_02001a8c();
s32 Func_02001a98();
s32 Func_02001aa6();
void Func_02001aba();
void Func_02001ae6();
u8 *Func_02001b02();
void Func_02001b50();
void Func_02001b5a();
void Func_02001b6c();
void Func_02001b74();
void Func_02001b76();
void Func_02001b7c();
void Func_02001ba0();
void Func_02001bb4();
void Func_02001bbe();
void Func_02001be0();
void Func_02001be4();
void Func_02001bee();
void Func_02001c00();
void Func_02001c06();
void Func_02001c10();
u8 *Func_02001c12();
void Func_02001c16();
void Func_02001c24();
void Func_02001c30();
void Func_02001c32();
void Func_02001c44();
void Func_02001c54();
void Func_02001c62();
void Func_02001c74();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunActorThirteenRestoration(void)
{
    u32 i;
    u8 *record;

    if (Value1(Func_02001a8c, 0x9a0) == 0) {
    } else {
        if (Value1(Func_02001a98, 0x1b7) != 0) {
        } else {
            if (Value1(Func_02001aa6, 0x9b0) == 0) {
            } else {
                Call1(Func_02001aba, 0x9b5);
                Func_02001ae6();
                Call1(Func_02001b6c, 0x2633);
                record = Func_02001b02(0);
                if (record != 0) {
                    Func_02001b50(13, *(s32 *)(record + 8), *(s32 *)(record + 16));
                }
                Call3(Func_02001b7c, 13, 0xc000, 0);
                Call3(Func_02001b5a, 0, 0x1b8, 0x4e8);
                Call3(Func_02001bbe, 13, 0x4000, 0);
                Call3(Func_02001b74, 0, 0x1bc, 0x4d8);
                Call3(Func_02001be0, 0, 0x100, 40);
                Call3(Func_02001be4, 0, 0x4000, 30);
                Func_02001bb4(13, 4);
                Func_02001be4(13, 0);
                Call3(Func_02001c06, 0, 0x105, 60);
                Call3(Func_02001c10, 13, 0x105, 60);
                Func_02001c00(13, 0);
                Func_02001b76(30);
                Func_02001bee(13, 2);
                Func_02001c16(13, 0);
                Call3(Func_02001c32, 13, 0xc000, 30);
                Func_02001c32(13, 0);
                Func_02001ba0(30);
                Call3(Func_02001c54, 13, 0x106, 60);
                Func_02001c44(13, 0);
                Func_02001c24(13, 3);
                Func_02001c54(13, 0);
                Call3(Func_02001bee, 13, 0xb333, 0x5999);
                Call3(Func_02001c24, 13, 0x1b8, 0x4e8);
                Func_02001c74(13, 0);
                Func_02001c54(0, 3);
                Func_02001c54(13, 2);
                record = Func_02001c12(0);
                if (record != 0) {
                    Func_02001c44(13, *(s16 *)(record + 10), *(s16 *)(record + 18));
                }
                Func_02001c62(13);
                Func_02001c74(13, 0, 0);
                Func_02001c30();
            }
        }
    }
}
