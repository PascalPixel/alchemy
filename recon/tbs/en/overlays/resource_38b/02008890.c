/* Draft of resource_38b 0x02008890 (Scene_DispatchPuzzleEvent), from
 * games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA. Remaining difference: the
 * ROM loads the scene numbers it compares from the literal pool, as
 * link-time values would; C constants compare against immediates. It also
 * passes the corner spawner's address as a number. The listing keeps these
 * rows. */
#include "TYPES.H"

#define NULL ((void *)0)

#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

enum {
    /* Message 0x182 + 194. */
    ITEM_HARD_NUT = 194
};

enum PushPuzzleMessage {
    MSG_ROBIN_PEERED_INTO = 0x947,
    MSG_ITS_TREE_BUT_ALMOST_LOOKS = 0x13ae,
    MSG_DID_SEE_TREE_AT_ENTRANCE = 0x13b3,
    MSG_HAVE_TRIED_HEADING_SOUTHEAST_FROM = 0x13b7,
    MSG_MCCOYS_HIDDEN_WAREHOUSE_DO_NOT = 0x146e,
    MSG_THERE_TREE_LOOKS_LIKE_PERSON = 0x1470,
    MSG_AREA_OFF_LIMITS_THOSE_WITHOUT = 0x1472,
    MSG_WAS_TURNED_INTO_TREE_FOR = 0x16bf,
    MSG_CURSE_WAS_BROKEN_THANKS_EFFORTS = 0x16c8,
    MSG_HAVE_EVER_BEEN_VILLAGE_IMIL = 0x16cc,
    MSG_THANK_SAVED_ME_FROM_BEING = 0x1774,
    MSG_YOURE_GUY = 0x1775,
    MSG_JILL_GAVE_ROBIN_SPECIAL_GIFT = 0x177a,
    MSG_BOTTOM_NOT_VISIBLE_LOOKS_VERY = 0x29dd
};

extern s16 Data_02000240[];
extern u8 Value_00000020;
extern u8 Data_020093fc[];
extern u8 Data_020091c0[];
extern u8 Value_0000001e;
extern u8 Value_00000023;
extern u8 Data_02009d28[];
extern u8 Data_02009d7c[];

s32 CalculateFacingAngle(s32, s32);
void Func_020012c6(s32, s32, s32);
s32 Func_02001460();
void Func_020016ca();
s32 Func_02001804();
s32 Func_02001828();
s32 Func_020018a2();
void Func_02001960();
void Func_0200196c();
void Func_02001978();
s32 Func_02001cb2();
s32 Func_02001cc2();
s32 Func_02001cd0();
void Func_020018e2();
u8 *Func_02001f90();
void Func_02001fa8();

/* Per-site raw names preserve the stock relocated branch operands; they
 * resolve to Func_080090c8, Object_SetCallback and Object_InitializeMode respectively. */

/* Per-site raw names: the first resolves to Scene_GetRecord and the four
 * renderer sites resolve to Func_080091c0. */

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ u16 ReadU16Elem(u16 *base, s32 idx)
{

    return *(u16 *)(base + idx);
}

static __inline__ void bump_step_020001ec(s32 amount)
{
    gEventWork->message += amount;
}


s32 Scene_DispatchPuzzleEvent(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (gGameState.scene == (s32)&Value_0000001e) {
        FieldScene_RunScene38b_020008f0();
    } else {
        if (gGameState.scene == (s32)&Value_00000023) {
            FieldScene_RunScene38bSequenceA();
            Call2(Func_020018e2, 0x2008ed9, 0xc80);
        } else {
            if (gGameState.scene == (s32)&Value_00000020) {
                FieldScene_RunScene38b_02000d10();
            }
        }
    }
    return 0;
}
