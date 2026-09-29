#ifndef TOREBI_KYUDEN_KYUDEN_H
#define TOREBI_KYUDEN_KYUDEN_H
extern struct EventWork *gEventWork;

s32 Audio_PlayCueFromEventWorkFar();
void Battle_ResetEffectCounterFar();
void DisplayTransition_InitializeBattleEffectStateFar();
s32 Event_SetStatus1c6Far();
void Event_WaitValue1c8FramesFar();
void Inn_PlaySleepFar();
void Map_ClearLayerEntryFlagFar();
void Map_SetLayerEntryFlagFar();
void Motion_LaunchFromFocusedObjectFar();
s32 Object_AttachWorkTargetToObjectFar();
void Object_LinkObjectAndSetCallbackFar();
void Object_RefreshSelectorByIdFar();
void ObjectMotion_ResetAndSetPositionFar();
void UiText_DrawQuantity();

/* Branching dialogue and actor presentation sequences. */

/* FAKEMATCH: Inline calls retain the original argument evaluation order. */

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The work-record cells are written as a union, not as bare halfwords: the
 * halfword store and the scene-pointer read that follows are only ordered
 * against each other when they can alias. */
union WorkCell {
    s32 w;
    u16 h;
};

static __inline__ void SetHalf(u16 *at, s32 value)
{
    ((union WorkCell *)at)->h = value;
}

extern u8 *gWork[];
extern s32 gKeysHeld;

/* FAKEMATCH: Call scopes retain the argument lifetimes of the exact build. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void AdvanceMessage(s32 amount)
{
    u8 *scene = *(u8 **)&gEventWork;

    *(u16 *)(scene + 0x1d8) = (u16)(*(u16 *)(scene + 0x1d8) + amount);
}
static __inline__ void AdvanceMessageAt(
    u8 **scene_address, s32 amount)
{
    u8 *scene = *scene_address;

    *(u16 *)(scene + 0x1d8) = (u16)(*(u16 *)(scene + 0x1d8) + amount);
}

static __inline__ void AdvanceMessageCursor(s32 amount)
{
    AdvanceMessageAt((u8 **)gWork, amount);
}

/* Three identical tails: an optional record whose signed halfwords at +10 and
 * +18 are forwarded to a per-channel import. */
static __inline__ void follow_record(
    s16 *record, s32 channel, void (*apply)())
{
    if (record != 0)
        apply(channel, record[5], record[9]);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* Supplemental scene sequence of the resource_3b8 overlay. */

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

extern u8 TorebiKyuden_EmptyTable[];
extern u8 TorebiKyuden_MessageTable[];

/* Contiguous unnamed leaf-owner run for resource_3b8. */

/* Contiguous unnamed leaf-owner run for resource_3b8. */

extern u8 TorebiKyuden_MiddleActionScript[];

/* Primary effect sequence of the resource_3b8 overlay: two hundred and fifty scene calls with constant arguments. */

static __inline__ void Call1_scene_effect_sequence_head(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ s32 Value1_scene_effect_sequence_head(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ void Call3_scene_effect_sequence_head(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

enum {
    /* Message 0x182 + 205. */
    ITEM_CLOAK_BALL = 205
};


static __inline__ void SetSceneEffectOffset(s32 actor, s32 axis, s32 offset)
{
    Actor_CenterAndWalk(actor, axis, offset);
}

#endif
