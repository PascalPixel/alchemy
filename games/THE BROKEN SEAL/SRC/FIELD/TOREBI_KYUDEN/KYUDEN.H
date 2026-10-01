#ifndef TOREBI_KYUDEN_KYUDEN_H
#define TOREBI_KYUDEN_KYUDEN_H
extern struct EventWork *gEventWork;

/*
 * The imports only the palace scene calls, each behind its inline service as
 * FIELD_SERVICE.H gives the shared ones.
 */
struct OwnerState;

void Audio_PlayCueFromEventWorkFar(void);
static inline void Audio_PlayCueFromEventWork(void)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Audio_PlayCueFromEventWorkFar();
}

void Battle_ResetEffectCounterFar(void);
static inline void Battle_ResetEffectCounter(void)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Battle_ResetEffectCounterFar();
}

void DisplayTransition_InitializeBattleEffectStateFar(s32 first);
static inline void DisplayTransition_InitializeBattleEffectState(s32 first)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    DisplayTransition_InitializeBattleEffectStateFar(first);
}

void Event_SetStatus1c6Far(void);
static inline void Event_SetStatus1c6(void)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Event_SetStatus1c6Far();
}

void Event_WaitValue1c8FramesFar(void);
static inline void Event_WaitValue1c8Frames(void)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Event_WaitValue1c8FramesFar();
}

void Inn_PlaySleepFar(s32 first);
static inline void Inn_PlaySleep(s32 first)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Inn_PlaySleepFar(first);
}

void Engine_InventorySnapshotRestore(void);
static inline void InventorySnapshot_Restore(void)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_InventorySnapshotRestore();
}

void Map_ClearLayerEntryFlagFar(s32 first);
static inline void Map_ClearLayerEntryFlag(s32 first)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Map_ClearLayerEntryFlagFar(first);
}

void Map_SetLayerEntryFlagFar(s32 value);
static inline void Map_SetLayerEntryFlag(s32 value)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Map_SetLayerEntryFlagFar(value);
}

void Motion_LaunchFromFocusedObjectFar(s32 first, s32 second, s32 third, s32 fourth);
static inline void Motion_LaunchFromFocusedObject(s32 first, s32 second, s32 third, s32 fourth)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Motion_LaunchFromFocusedObjectFar(first, second, third, fourth);
}

void ObjectMotion_ResetAndSetPositionFar(s32 first, s32 second, s32 third);
static inline void ObjectMotion_ResetAndSetPosition(s32 first, s32 second, s32 third)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    ObjectMotion_ResetAndSetPositionFar(first, second, third);
}

void Object_AttachWorkTargetToObjectFar(s32 id, s32 flag);
static inline void Object_AttachWorkTargetToObject(s32 id, s32 flag)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Object_AttachWorkTargetToObjectFar(id, flag);
}

void Object_LinkObjectAndSetCallbackFar(s32 first, s32 second);
static inline void Object_LinkObjectAndSetCallback(s32 first, s32 second)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Object_LinkObjectAndSetCallbackFar(first, second);
}

void Object_RefreshSelectorByIdFar(s32 first);
static inline void Object_RefreshSelectorById(s32 first)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Object_RefreshSelectorByIdFar(first);
}

struct OwnerState *Engine_OwnerGetState(s32 owner);
static inline struct OwnerState *Owner_GetState(s32 owner)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    return Engine_OwnerGetState(owner);
}

void Engine_OwnerRecalculateRatios(s32 owner);
static inline void Owner_RecalculateRatios(s32 owner)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_OwnerRecalculateRatios(owner);
}

void Engine_PartyAddActiveOwner(s32 owner);
static inline void Party_AddActiveOwner(s32 owner)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_PartyAddActiveOwner(owner);
}

s32 Engine_PartyListActiveOwners(s16 *owners);
static inline s32 Party_ListActiveOwners(s16 *owners)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    return Engine_PartyListActiveOwners(owners);
}

void Engine_UiTextDrawQuantity(s32 first, s32 second);
static inline void UiText_DrawQuantity(s32 first, s32 second)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_UiTextDrawQuantity(first, second);
}

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

#endif
