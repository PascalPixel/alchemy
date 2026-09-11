#include "types.h"

#define SceneDialogue_RunActor12Event Func_02000328
#define SceneDialogue_ShowLine1CF8 Func_02000240
#define SceneDialogue_RunActor8Message1f09 Func_02000688
#define SceneDialogue_RunActor10Message1f15 Func_020006a8

extern u8 Data_00001d20[];
extern u8 Data_03001ebc[];

void Func_020017c4(void);
void Func_02001862(s32);
s32 Func_0200188a(s32, s32);
void Func_020017de(void);
s32 Func_02001898();
s32 Func_020018a2();
void Func_020018c2();
u8 *Func_020018ce();
void Func_02001910();
void Func_02001922();
s32 Func_02001926();
void Func_0200193c();
s32 Func_0200194a();
s32 Func_0200194c();
s32 Func_0200194c_next();
void Func_02001974();
void Func_0200197a();
void Func_02001984();
void Func_02001996();
s32 Func_020019a6();
void Func_020019ac();
void Func_020019b6();
void Func_020019be();
void Func_020019ce();
void Func_020019ea();
void Func_020019ea_position();
u8 *Func_020019fa();
void Func_020019fe();
void Func_020019fe_mode();
void Func_02001a02();
void Func_02001a04();
void Func_02001a1c();
void Func_02001a20();
void Func_02001a30();
void Func_02001a3c();
void Func_02001a48();
void Func_02001a6e();
void Func_02001a6e_close();
void Func_02001abc();
void Func_02001aca();
void Func_02001c0c(void);
void Func_02001caa(s32);
s32 Func_02001cd2(s32, s32);
void Func_02001c26(void);
void Func_02001c2c(void);
void Func_02001cca(s32);
s32 Func_02001cf2(s32, s32);
void Func_02001c46(void);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Contiguous unnamed leaf-owner run for resource_3ae. */

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void SceneDialogue_ShowLine1CF8(void)
{
    Func_020017c4();
    Func_02001862(0x1CF8);
    Func_0200188a(8, 0);
    Func_020017de();
}

void SceneDialogue_RunActor12Event(void)
{
    s32 rec7;
    s32 rec8;
    u8 *record;
    s16 angle;

    record = Func_020018ce(0);
    angle = (u16)((*(u16 *)(record + 6) + 0x2000) & ~0x3fff);
    Func_020018c2();
    if (Value1(Func_02001898, 0x8a7) != 0) {
        if (Value1(Func_020018a2, 0x8a9) != 0) {
            Call1(Func_02001974, 0x1d23);
            Func_02001984(12, 0);
            goto L_02000496;
        }
        Func_02001996((s32)Data_00001d20);
        Value2(Func_020019a6, 12, 0);
        if (Value2(Func_02001926, 0, 0) == 0) {
            Func_02001910(10);
            Func_020019b6(((s32)Data_00001d20 + 1));
            Func_020019ce(12, 0);
            Call3(Func_0200197a, 12, 88, 0x508);
            Call3(Func_020019fe, 12, 0x4000, 0);
            Func_0200193c(20);
            Call1(Func_02001922, 0x8a9);
            goto L_02000496;
        }
        Func_020019ea(((s32)Data_00001d20 + 2));
        Func_02001a02(12, 0);
    } else {
        if ((u16)angle != 0x8000) {
            goto L_0200049a;
        }
        Call1(Func_02001a04, 0x1d16);
        Func_02001a1c(12, 0);
        if (Value1(Func_0200194a, 0x8a5) != 0) {
            rec7 = Func_0200194c(235);
            rec8 = Func_0200194c_next(rec7, 235);
            Func_020019fe_mode(12, 3);
            Call3(Func_020019ea_position, 12, 88, 0x508);
            Call3(Func_02001a6e, 12, 0x4000, 0);
            bump_step(1);
            Func_02001a6e_close(12, 0);
            Func_020019be(rec7, rec8);
            Call1(Func_020019ac, 0x8a7);
            record = Func_020019fa(0);
            Call3(Func_02001a30, 0, *(s16 *)(record + 10), 0x518);
            Call3(Func_02001a3c, 0, 72, 0x518);
            Call3(Func_02001a48, 12, 88, 0x518);
            Func_02001aca(12, 0, 0);
        } else {
            Func_02001abc(12, 0);
        }
    }
    L_02000496:;
    Func_02001a20();
    L_0200049a:;
}

void SceneDialogue_RunActor8Message1f09(void)
{
    Func_02001c0c();
    Func_02001caa(0x1F09);
    Func_02001cd2(8, 0);
    Func_02001c26();
}

void SceneDialogue_RunActor10Message1f15(void)
{
    Func_02001c2c();
    Func_02001cca(0x1F15);
    Func_02001cf2(10, 0);
    Func_02001c46();
}
