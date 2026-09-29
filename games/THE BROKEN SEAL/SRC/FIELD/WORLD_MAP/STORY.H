#ifndef STORY_H
#define STORY_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "MAP_RENDER_WORK.H"


#define SCENE_PHASE (*(s32 *)(*(u8 **)&gEventWork + 0x1c0))
#define ACTOR 8

typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} StorySpawnRecord;

struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
};

struct StoryDialogueWork {
    u8 reserved000[370];
    u16 story_result;
};

/*
 * Complete selected-actor presentation/progress synchronizer.  It derives the
 * actor presentation from the shared phase word, applies the state-one scene
 * marker, conditionally backs higher progress down, then publishes the result.
 */
struct StorySelection {
    u8 reserved000[500];
    s32 actor_id;
};

struct StorySelectionActor {
    u8 reserved00[6];
    u16 presentation;
};

struct StoryProgressWork {
    u8 reserved000[386];
    u16 state_one_marker;
};

/* Complete prologue-less shared-state activation leaf through its pool. */
struct StorySharedState {
    u8 reserved00[52];
    u8 active;
};

struct StoryCompletionWork {
    u8 reserved000[386];
    s16 scene_value;
};

/*
 * Per-frame step of a thirty-two frame rise -- resource_371.  The halfword
 * counter at +0x64 is incremented in place and compared as a signed sixteen
 * bit value; past 31 the effect is handed off and nothing else happens.  The
 * anchor pointer at +0x68 is loaded before the frame test and must stay
 * there.  The angle is the frame shifted left by ten, and the callee that
 * turns it into a 16.16 amplitude is identified only by that use.
 */
struct StoryVerticalEffectActor {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct StoryVerticalEffectActor *anchor;  /* 0x68 */
};

/*
 * The mirror of the rise: the same counter at +0x64 and anchor at +0x68,
 * with the second amplitude store negated, the depth offset subtracted
 * rather than added, and a fixed depth bias of 0x100000.  Its magnitude
 * callee, like the one above, is identified only by use -- one angle in, one
 * 16.16 magnitude out.
 */
struct StoryVerticalEffectActor_02004004 {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct StoryVerticalEffectActor_02004004 *anchor;  /* 0x68 */
};

/* The world map's scene tables, labelled where the overlay's data lies. */
extern u8 gWorldMapEntrances[];
extern u8 gWorldMapExits[];
extern u8 gWorldMapEvents[];
extern u8 gWorldMapPlacements[];
extern u8 gWorldMapPlacements49[];
extern u8 gWorldMapPlacements64[];
extern u8 gWorldMapPlacements65[];
extern u8 gWorldMapPlacements66[];
extern u8 gWorldMapPlacements71[];
extern u8 gWorldMapPlacements72[];
extern u8 gWorldMapPlacements73[];
extern u8 gWorldMapPlacements80[];
extern s32 gWorldMapRewards[];

/* Action scripts the transfer scene hands its actors. */
extern u8 gTransferArrive10[];
extern u8 gTransferDepart10[];
extern u8 gTransferReturn10[];
extern u8 gTransferGather[];
extern u8 gTransferArrive11[];
extern u8 gTransferDepart11[];
extern u8 gTransferReturn11[];
extern u8 gTransferArrive12[];
extern u8 gTransferDepart12[];
extern u8 gTransferReturn12[];
extern u8 gTransferArrive13[];
extern u8 gTransferDepart13[];
extern u8 gTransferReturn13[];
extern u8 gTransferGuide14[];
extern u8 gTransferLeaderIdle[];
extern u8 gTransferLeaderTurn[];

/* The actor that speaks for the world map's triggers, past the image. */
extern s32 gWorldMapTriggerActor;
extern struct StorySharedState *gEffectWork;

/* Main-image services reached through the overlay's import veneers. The
 * declarations stay old-style where call sites pass differing arguments. */
void BattleFx_SetPhaseRequest();
void Battle_SetObjectFlag5bWhenMode3();
void Battle_ClearObjectFlag5bWhenMode3();
void BattleFx_ScheduleRatioTransition();
void BattleFx_SetWeightedResult();
void Event_WaitForDisplayField358Clear();
void Event_SetPairWork1c0();
void ItemMenu_Open();
void Menu_AnimateSelectionToEntry();
void PaletteGlow_Update();
void Map_LoadDefaultCellsAndUpdateBlock();
void Object_SetActionCallbackAndRefreshById();
void Object_RefreshSelectorById();
s32 Engine_MathModulo();
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void QueueIoWriteDelay2(u32 address, u32 value);

/* The world map's own callbacks and scenes. */
void WorldMap_CreateLinkedEffects();
void StoryActor_AdvanceTimer(u8 *actor);
void WorldMap_UpdateBobbingMarker();
void WorldMap_SpawnActorEightPuff(void);
void StoryScene_UpdateSelectedActorProgress(void);
void StoryScene_UpdateTransitionEffect(void);
void FieldScene_RunScene371_020017a4(void);
extern u8 gOpeningLeaderRise[];


/*
 * Story-progress triggers -- resource_371.  Each fires one event once the
 * progress word has reached nine tenths of the level word; each seventy-two
 * byte owner includes its three pool words.
 */

/* Actor callbacks for the story transfer scene. */

/* FAKEMATCH: call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
s32 StoryScene_ComputeOpposingSlotDelta(void);

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    f(a0, a1, a2, a3);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

static __inline__ s32 Value0(s32 (*f)())
{

    return f();
}

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The actor index every call in this scene step operates on. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{

    return f(a0, a1);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{

    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

static __inline__ u8 *Pointer1(u8 *(*f)(), s32 a0)
{
    return f(a0);
}

#endif
