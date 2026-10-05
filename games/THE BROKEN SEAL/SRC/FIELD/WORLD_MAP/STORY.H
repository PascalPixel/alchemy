#ifndef STORY_H
#define STORY_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "MAP_RENDER_WORK.H"

#define SCENE_PHASE (gEventWork->start_transition)
#define ACTOR 8

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

struct StorySharedState {
    u8 reserved00[52];
    u8 active;
};

/* Raise the effect for thirty-two frames, then hand it off; each frame advances its angle by 0x400. */
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

/* The world map's scene tables, labelled where the overlay's data lies. */
extern u8 gWorldMapEntrances[];
extern u8 gWorldMapExits[];
extern struct SceneEvent gWorldMapEvents[];
extern struct ScenePlacement gWorldMapPlacements[];
extern struct ScenePlacement gWorldMapPlacements49[];
extern struct ScenePlacement gWorldMapPlacements64[];
extern struct ScenePlacement gWorldMapPlacements65[];
extern struct ScenePlacement gWorldMapPlacements66[];
extern struct ScenePlacement gWorldMapPlacements71[];
extern struct ScenePlacement gWorldMapPlacements72[];
extern struct ScenePlacement gWorldMapPlacements73[];
extern struct ScenePlacement gWorldMapPlacements80[];
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

void BattleFx_SetPhaseRequest();
void Battle_SetObjectFlag5bWhenMode3();
void Battle_ClearObjectFlag5bWhenMode3();
void BattleFx_ScheduleRatioTransition();
void BattleFx_SetWeightedResult();
void Event_WaitForZoomTransition();
void Event_SetDestination();
void ItemMenu_Open();
void Menu_AnimateSelectionToEntry();
void PaletteGlow_Update();
void Map_LoadDefaultCellsAndUpdateBlock();
void Object_SetActionCallbackAndRefreshById();
void Object_RefreshSelectorById();
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void QueueIoWriteDelay2(u32 address, u32 value);

/* The world map's own callbacks and scenes. */
void WorldMap_CreateLinkedEffects();
void StoryActor_AdvanceTimer(struct FieldActor *actor);
void WorldMap_UpdateBobbingMarker();
void WorldMap_SpawnActorEightPuff(void);
void StoryScene_UpdateSelectedActorProgress(void);
void StoryScene_UpdateTransitionEffect(void);
void WorldMap_UpdateTransferredActorLanding(void);
extern u8 gOpeningLeaderRise[];

/* Each story trigger fires once progress reaches nine tenths of its level. */

/* Actor callbacks for the story transfer scene. */

s32 StoryScene_ComputeOpposingSlotDelta(void);

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The actor index every call in this scene step operates on. */

#endif
