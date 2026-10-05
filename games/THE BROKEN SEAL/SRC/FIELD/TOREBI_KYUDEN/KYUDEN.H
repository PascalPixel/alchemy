#ifndef TOREBI_KYUDEN_KYUDEN_H
#define TOREBI_KYUDEN_KYUDEN_H
extern struct EventWork *gEventWork;

/*
 * The imports only the palace scene calls, by their own names; the launch
 * keeps an inline because its direct call changes the palace's bytes.
 */
struct OwnerState;

void Audio_PlayCueFromEventWork(void);
void Battle_ResetEffectCounterFar(void);
void DisplayTransition_InitializeBattleEffectStateFar(s32 first);
void Event_StartScreenTransitionFar(void);
void Event_WaitTransitionFramesFar(void);
void Inn_PlaySleepFar(s32 first);
void Engine_InventorySnapshotRestore(void);
void Map_ClearLayerEntryFlagFar(s32 first);
void Map_SetLayerEntryFlagFar(s32 value);
void Motion_LaunchFromFocusedObjectFar(s32 first, s32 second, s32 third, s32 fourth);
static inline void Motion_LaunchFromFocusedObject(s32 first, s32 second, s32 third, s32 fourth)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Motion_LaunchFromFocusedObjectFar(first, second, third, fourth);
}

void ObjectMotion_ResetAndSetPositionFar(s32 first, s32 second, s32 third);
void Object_AttachWorkTargetToObjectFar(s32 id, s32 flag);
void Object_LinkObjectAndSetCallbackFar(s32 first, s32 second);
void Object_RefreshSelectorByIdFar(s32 first);
struct OwnerState *Engine_OwnerGetState(s32 owner);
void Engine_OwnerRecalculateRatios(s32 owner);
void Engine_PartyAddActiveOwner(s32 owner);
s32 Engine_PartyListActiveOwners(s16 *owners);
void Engine_UiTextDrawQuantity(s32 first, s32 second);
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
