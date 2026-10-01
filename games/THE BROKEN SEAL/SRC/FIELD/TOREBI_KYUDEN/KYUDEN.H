#ifndef TOREBI_KYUDEN_KYUDEN_H
#define TOREBI_KYUDEN_KYUDEN_H
extern struct EventWork *gEventWork;

void Audio_PlayCueFromEventWorkFar(void);
void Battle_ResetEffectCounterFar();
void DisplayTransition_InitializeBattleEffectStateFar();
void Event_SetStatus1c6Far(void);
void Event_WaitValue1c8FramesFar(void);
void Inn_PlaySleepFar();
void Map_ClearLayerEntryFlagFar();
void Map_SetLayerEntryFlagFar();
void Motion_LaunchFromFocusedObjectFar();
void Object_AttachWorkTargetToObjectFar(s32 id, s32 flag);
void Object_LinkObjectAndSetCallbackFar();
void Object_RefreshSelectorByIdFar();
void ObjectMotion_ResetAndSetPositionFar();
void UiText_DrawQuantity();

/* Branching dialogue and actor presentation sequences. */

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

/* Supplemental scene sequence of the resource_3b8 overlay. */

extern u8 TorebiKyuden_EmptyTable[];
extern u8 TorebiKyuden_MessageTable[];

/* Contiguous unnamed leaf-owner run for resource_3b8. */

/* Contiguous unnamed leaf-owner run for resource_3b8. */

extern u8 TorebiKyuden_MiddleActionScript[];

/* Primary effect sequence of the resource_3b8 overlay: two hundred and fifty scene calls with constant arguments. */

enum {
    /* Message 0x182 + 205. */
    ITEM_CLOAK_BALL = 205
};

static __inline__ void SetSceneEffectOffset(s32 actor, s32 axis, s32 offset)
{
    Actor_CenterAndWalk(actor, axis, offset);
}

#endif
