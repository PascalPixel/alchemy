#include "types.h"

#define SceneActor_SetSlotEightMode0WhenPresent Func_0200007c
#define SceneState_MarkActor9AndConfigureRegion7_32 Func_02000094
#define SceneDialogue_IsPromptResultZero Func_020012ac
#define SceneDialogue_AskMessage2549 Func_020012c4
#define SceneDialogue_AskMessage254d Func_020012f0
#define SceneDialogue_AskMessage2557 Func_02001320
#define SceneDialogue_AskMessage254a Func_02001368
#define SceneDialogue_AskMessage255c Func_0200141c
/*
 * resource_3ad owner at 0x02000094, 64 bytes.
 *
 * Marks record 9 as active, then runs the six-argument entry and one more call.
 *
 * The two stack arguments are locals ASSIGNED AFTER the guard, not initialised
 * at their declaration. Initialising them up front makes the compiler
 * materialise and store them one at a time (`movs r3,#8 / str / movs r3,#32 /
 * str`); assigning them after the record writes keeps both live at once, which
 * is the `movs r3,#8 / movs r2,#32 / str / str` the reference has.
 */
struct Rec_3ad {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[49];
    u8 f85;                     /* +85 */
};

s32 Func_02001bbe(s32);
void Func_02001b80(s32, s32);
struct Rec_3ad *Func_02001bd8();
void Func_02001bd8_a(s32);
void Func_02001bb0();
s32 Func_02002de8(s32, s32);
void Func_02002e6e(s32);
s32 Func_02002e7e(s32, s32);
s32 Func_02002e0e(s32, s32);
void Func_02002e9a(s32);
s32 Func_02002eaa(s32, s32);
s32 Func_02002e3a(s32, s32);
void Func_02002eca(s32);
s32 Func_02002eda(s32, s32);
s32 Func_02002e6a(s32, s32);
void Func_02002f12(s32);
s32 Func_02002f22(s32, s32);
s32 Func_02002eb2(s32, s32);
void Func_02002fc6(s32);
s32 Func_02002fd6(s32, s32);
s32 Func_02002f66(s32, s32);

void SceneActor_SetSlotEightMode0WhenPresent(void)
{
    s32 handle = Func_02001bbe(8);

    if (handle != 0) {
        Func_02001b80(handle, 0);
    }
}

void SceneState_MarkActor9AndConfigureRegion7_32(void)
{
    struct Rec_3ad *rec = Func_02001bd8(9);
    s32 p5, p6;

    if (rec != 0) {
        rec->f35 = 1;
        rec->f85 = 0;
    }

    p5 = 8;
    p6 = 32;
    Func_02001bb0(7, 32, 1, 1, p5, p6);
    Func_02001bd8_a(516);
}

s32 SceneDialogue_IsPromptResultZero(void)
{
    return Func_02002de8(0, 0) == 0;
}

s32 SceneDialogue_AskMessage2549(void)
{
    Func_02002e6e(0x2549);
    Func_02002e7e(1, 0);
    return Func_02002e0e(0, 0) == 0;
}

s32 SceneDialogue_AskMessage254d(void)
{
    Func_02002e9a(0x254D);
    Func_02002eaa(1, 0);
    return Func_02002e3a(0, 0) == 0;
}

s32 SceneDialogue_AskMessage2557(void)
{
    Func_02002eca(0x2557);
    Func_02002eda(12, 0);
    return Func_02002e6a(0, 0) == 0;
}

s32 SceneDialogue_AskMessage254a(void)
{
    Func_02002f12(0x254A);
    Func_02002f22(3, 0);
    return Func_02002eb2(0, 0) == 0;
}

s32 SceneDialogue_AskMessage255c(void)
{
    Func_02002fc6(0x255C);
    Func_02002fd6(1, 0);
    return Func_02002f66(0, 0) == 0;
}
