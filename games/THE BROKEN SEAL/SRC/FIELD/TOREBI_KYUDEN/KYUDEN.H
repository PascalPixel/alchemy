#ifndef TOREBI_KYUDEN_KYUDEN_H
#define TOREBI_KYUDEN_KYUDEN_H
extern struct EventWork *gEventWork;

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

extern u8 TorebiKyuden_EmptyTable[];
extern u8 TorebiKyuden_MessageTable[];

extern u8 TorebiKyuden_MiddleActionScript[];

enum {
    ITEM_CLOAK_BALL = 205
};

#endif
