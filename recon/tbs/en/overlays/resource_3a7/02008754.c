/* Draft of FieldScene_RunSupplementalSequenceOne, resource_3a7 at 0x02008754 (split from
 * FIELD/KUUPUAPPU_DOU/SELECT_BY_RUNTIME_SELECTOR.C).
 * Remaining difference: it reaches the overlay's own soft-float helpers (the compiler
 * library's double members, which the overlay holds after its import table) through
 * address-derived names; linking it needs double arithmetic in the C and those library
 * members linked where the overlay holds them, so the overlay keeps its listing rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 *Data_03001ebc;

#include "TYPES.H"

u8 *Func_02001664(s32);
u8 *Func_0200174c(s32);
u8 *Func_020017f0(s32);

#include "TYPES.H"

void Func_02001bfc(s32, s32, s32 *);
void Func_02001c38(s32 *, s32, s32, s32);

#include "TYPES.H"

void Func_020004e6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
s32 SceneActor_LiftLowActorOnSubjectTile(s32 subject_actor);

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

#include "TYPES.H"

u8 *Func_020016f8(s32);
u8 *Func_020017a0(s32);
void Func_02001cb6();
void Func_02001d08();

#include "TYPES.H"

extern s32 Data_0200a214[];
extern s32 Data_0200a224;
extern s32 Data_0200a228;
extern s32 Data_0200a22c;
extern s32 Data_0200a230;

void Func_02000600();
s32 Func_02001898();
void Func_020018ba();
double Func_02001bd2();
void Func_02001bde();
double Func_02001c1c();
double Func_02001c2a();
s32 Func_02001cd6();
void Func_020015b0();
s32 Func_02001af6();
s32 Func_02001b36();

/* The scene step counter at 0x1d8 of the shared scene work record. */

static __inline__ void Call1_02000754(void (*f)(), s32 a0)
{
    double Func_02001b8a();

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    double Func_02001b8a();

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    double Func_02001b8a();

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    double Func_02001b8a();

    f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    double Func_02001b8a();

    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    double Func_02001b8a();

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call1_02000aa0(void (*f)(), s32 a0)
{
    void Func_02001b8a();

    f(a0);
}

#include "TYPES.H"

extern s16 Data_02000240[];
extern u8 Value_00000060;
extern u8 Value_00000061;
extern u8 Value_00000062;
extern u8 Data_02009d34[];
extern u8 Data_02009d4c[];
extern u8 Data_02009ecc[];
extern u8 Data_02009d1c[];

#include "TYPES.H"

extern s16 Data_02000240[];
extern u8 Value_00000060;
extern u8 Value_00000061;
extern u8 Value_00000062;
extern u8 Data_02009f98[];
extern u8 Data_0200a064[];
extern u8 Data_0200a190[];
extern u8 Data_02009f8c[];

#include "TYPES.H"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

extern void Func_02000dfc(void);

extern s16 Data_02000240[];
extern u8 Value_00000060;
extern u8 Value_00000061;
extern u8 Value_00000062;
extern u8 Data_02009c80[];
extern u8 Data_02009cc0[];
extern u8 Data_02009cfc[];
extern u8 Data_02009c7c[];

extern s16 Data_02000240[];
extern u8 Value_00000060;
extern u8 Value_00000061;
extern u8 Value_00000062;
extern u8 Data_020098cc[];
extern u8 Data_02009a34[];
extern u8 Data_02009b9c[];
extern u8 Data_0200989c[];

#include "TYPES.H"

/* Deliberate no-op callback. */

#include "TYPES.H"

#include "TYPES.H"

enum SelectByRuntimeSelectorMessage {
    MSG_DOOR_TIGHTLY_LOCKED = 0x953,
    MSG_ROBIN_FLIPPED_SWITCH = 0x1528
};

extern u8 *Func_020016ac(s32);

void FieldScene_RunSupplementalSequenceOne(void)
{
    double Func_02001b8a();

    s32 p10;
    s32 count;
    s32 rec7;
    s32 shown;
    double bias;
    double held;
    double scaled;
    s32 c5;
    s32 c6;
    s32 mode;

    p10 = (0x164 + *(volatile s32 *)0x03001e70);
    Event_Begin();
    if ((*(volatile s32 *)0x03001e40 & 1) != 0) {
        *(volatile s32 *)(p10 + 24) = 1;
        *(volatile s32 *)(p10 + 28) = 1;
    } else {
        *(volatile s32 *)(p10 + 24) = -1;
        *(volatile s32 *)(p10 + 28) = -1;
    }
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Audio_PlayCue(163);
    bias = 4294967296.0;
    count = 0x1df;
    do {
        rec7 = Random_Next();
        held = Func_02001c1c(*(volatile s32 *)(p10 + 36));
        shown = (s32)((u32)(rec7 << 11) >> 16);
        scaled = Func_02001c2a(shown);
        if (shown < 0) {
            bias = 4294967296.0;
            scaled = Func_02001b8a(scaled, bias);
        }
        Func_02001bde(held, Func_02001bd2(4718.592, scaled));
        *(volatile s32 *)(p10 + 36) = Func_02001cd6();
        Event_Wait(1);
        count = (count + -1);
    } while (count >= 0);
    c5 = 6;
    c6 = 6;
    count = 0;
    mode = (c5 << 10);
    do {
        Call2(Func_020018ba, 0x4000052, ((mode | (c5 << 5)) | c6));
        Event_Wait(1);
        if (Value2(Func_02001898, count, 20) == 0) {
            c6 = (c6 - 1);
            c5 = (c5 - 1);
        }
        count = (count + 1);
    } while (count <= 69);
    Map_CopyCells(19, 83, 15, 8, 19, 91);
    Audio_PlayCue(0x120);
    Map_Redraw();
    MapRender_WaitForValues();
    Event_End();
}
