#ifndef FIELD_H
#define FIELD_H

/// @file
/// The field's routines and tables other banks call, and the field Psynergy's
/// reach.

#include "types.h"
#include "edition.h"
#include "actor.h"

struct Actor;
struct ScreenPoint;

extern const s32 gCamera_FixedViewMatrix[];
extern s32 gIwram_Data1;
extern s32 gIwram_Data2;
extern u8 gSpriteDissolve_PixelOrder[];
extern const s32 gActor_DefaultScript[];
extern const u8 gWorldMap_TerrainBehaviorTable[];
extern u8 Render_DecodePictureCodeSize[];
extern u8 gSpriteColorRemapTables[];
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
s32 Party_CheckShortcut(u32 packed_id);
void SpriteDissolve_Step(u8 *object, u32 position);
void FieldCamera_FollowActor(s32 id, s32 flag);
void Actor_UpdateAllMotion(void);
void Actor_UpdateAllThumb(void);
void Actor_SetSpriteFlags(struct Actor *object, u32 flags);
void ObjectSystem_UpdateCamera(void);
void ActorSystem_ProjectSprites(void);
s32 Actor_FindLastActiveId(void);
void BattleMotion_ProjectPositionFar(s32, s32 *);
s32 Unit_KnowsPsynergyFar(s32 group, s32 entry);
s32 Party_ShowJoinedMessageFar(s32 event_id, s32 value);
s32 Party_ShowPairJoinedMessageFar(s32 first, s32 second);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);

struct Sprite;
struct ScreenPoint;
struct Particle;
struct Actor;
struct Actor;
struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneRegion;
struct Actor;

extern u32 gDriftScene_SpawnScript[];
extern u32 gSceneExits[];
extern struct Actor gSnapshotObjects[];
extern u8 gLinkLobby_SlotColumns[];
extern s32 gLinkLobby_SlotValues[];
extern const u8 gActor_BlinkSpriteColors[4];
extern const u8 gActor_StepAngleScript[];
extern u16 gScript_MainScript[];
s32 Animation_InitializeObjects(struct Sprite *);
void Actor_SetBlendMode(struct Actor *object, u32 value);
void AnimationObject_SetActiveTimerSteps(struct Sprite *, s32);
s32 PaletteFade_SetTargetFar(s32, s32);
void FieldPsynergy_FinishMindRead(void);
void FieldPsynergy_Dispatch(void);
void FieldPsynergy_LoadResources(s32 action, s32 flags);
void FieldPsynergy_Run(void);
void BattleEffect_SelectBattleCue(s32 row, s32 column);
void FieldPsynergy_SetupActorPair(s32 actor, s32 target);
s32 PaletteFade_StartInterpolationFar(s32);
s32 BattleMotion_ProjectScaledPositionFar(s32, s32 *);
struct BattleEffectParams;
void BattleEffect_RunFlurry(struct BattleEffectParams *effect, s32 variant);
void BattleEffect_StopMainActor(void);
struct ActorPlacement;
void Event_SpawnObjectTable(struct ActorPlacement *entry, s32 slot);
void Actor_TurnPairToFaceEachOther(struct Actor *first, struct Actor *second);
void Field_StartPaletteTransition(s32 mode);
void FieldPsynergy_RunHandler(s32 kind);
void FieldPsynergy_RunExtraHandler(void);

s32 Actor_PlaySoundInViewFar(struct Actor *interpreter, s32 value);
void Actor_ReleaseAtIndex(u32 actorIndex);
void Actor_SetSquareSprite(u32 objectId);
void BattleEffect_DropObjectAtAnchor(s32 actorId);
void SuperLuckyDice_Run(void);
void GameFlag_RefreshLureCapFar(void);
void MapGrid_CenterOnFollowedPointFar(void);
s32 MapGrid_GetTerrainHeightFar(u8, s32, s32);
void Actor_RunRepeatedMotion(u32 objectId, s32 variant);
s32 Actor_IsTargetUnset(struct Actor *object);
#ifndef OWN_Actor_SetPartAttribute
void Actor_SetSpriteColor(struct Actor *object, s32 attribute);
#endif
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_StartCameraFar(void *object, void *effect);
void Entrance_EndArrivalAnimation(s32 effect);
void Exit_PrepareAnimation(s32 effect);
void Actor_CenterOnColumn(u32 objectId, s32 action, s32 z_offset);
void Actor_WalkByAndWaitById(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_WalkByById(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_SetDestinationById(u32 objectId, s32 x, s32 z);
void Actor_MoveToAndWaitById(u32 objectId, s32 x, s32 z);
void Actor_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z);
void Actor_StartRepeatedMotion(u32 objectId, s32 variant);
void ActorSystem_Configure(s32 mode);
s32 Party_AddMemberFar(s32 event_id);
void Party_RemoveMemberFar(s32 owner);
void Render_ApplyProjectedPlacement(void *sprite, s32 *position, s32 *scale,
                                    u16 angle);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void Scene_SetArrivalFlags(s32 unused);
s32 Message_OpenWindowFar(s32, s32, s32, s32);
void AreaNameWindow_HideFar(void);
s32 Window_IsClosedFar(s32);

/// How far a field Psynergy reaches from its caster, one row a kind in
/// gFieldPsynergy_Reach, ended by -1: Field_MeasurePsynergyHeight and
/// Field_MeasurePsynergyReach read it.
struct FieldPsynergyReach {
  s16 fieldPsynergy; ///< enum FieldPsynergy; -1 ends the table
  s16 range;         ///< pixels over the ground
  s16 height;        ///< pixels above or below the caster
  s16 width;         ///< either side of the caster's facing, 1/65536 turns
};

extern const struct FieldPsynergyReach gFieldPsynergy_Reach[];

extern u8 gWorldMapPackedTiles[];
extern u8 gWorldMapPackedPictures[];
extern u8 gWorldMapPalettes[];

#endif
