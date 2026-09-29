/* Draft of FieldScene_RunScene3be_02000fd0, resource_3be at 0x02008fd0 (split from FIELD/KARAGORU_DOU/STAGED_PAIR.C).
 * Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines
 * (Engine_EventOpenMessage, Engine_EventChooseYesNo). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

#include "STAGED_ACTOR_PAIR_SCENE.H"
#include "STAGED_ACTOR.H"
extern u8 MsgKaragoruDoYouWishCrossInto[];


struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

/*
 * resource_3be owner at 0x020011d8, 32 bytes.
 *
 * Runs a step at most 40 times, stopping early once the caller's +12 field has
 * come down to the limit. Both the counter and the field test guard the loop.
 */
struct HeightTrackedObject {
    u8 pad00[12];
    s32 height;                 /* +12 */
};

extern u8 Value_00000098;
extern u8 Value_0000009d;
extern u8 Value_0000009e;
extern u8 Data_020097b4[];
extern u8 Data_020097fc[];
extern u8 Data_02009874[];
extern u8 Data_02009784[];
extern u8 Data_00000088[];
extern u8 Data_00000098[];
extern u8 Data_0000009d[];
extern u8 Data_0000009e[];
extern u8 Data_0200995c[];
extern u8 Data_02009974[];
extern u8 Data_020099d4[];
extern u8 Data_02009a4c[];
extern u8 Data_02009aac[];
extern u8 Data_02009b3c[];
extern u8 Data_02009b48[];
extern u8 Data_02009bcc[];
extern u8 Data_02009c80[];
extern u8 Data_02009ce0[];
extern s16 Data_02000240_t[][1];

s32 *Func_02002698();

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

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

void FieldScene_RunScene3be_02000fd0(void)
{
    u32 i;
    s32 record;
    s32 base5_23cc;

    base5_23cc = (s32)MsgKaragoruDoYouWishCrossInto;
    Event_SetMessage(base5_23cc);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        if (GameFlag_IsSet(0x950) != 0) {
            if (GameFlag_IsSet(0x96f) == 0) {
                Event_SetMessage((base5_23cc + 8));
            }
        }
        Event_ShowMessage(8, 0);
    } else {
        bump_step(1);
        Event_ShowMessage(8, 0);
    }
}
