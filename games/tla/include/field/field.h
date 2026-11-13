#ifndef FIELD_H
#define FIELD_H

#include "types.h"
#include "edition.h"
#include "actor.h"

struct Actor;
struct EffectPosition;

extern const s32 gCamera_FixedViewMatrix[];
extern s32 gIwram_Data1;
extern s32 gIwram_Data2;
extern u8 gSpriteDissolve_PixelOrder[];
extern const s32 gActor_DefaultScript[];
extern const u8 gWorldMap_TerrainBehaviorTable[];
extern u8 Render_DecodePictureCodeSize[];
extern u8 gSpriteColorRemapTables[];
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
s32 Event_ValidatePackedId(u32 packed_id);
void SpriteDissolve_Step(u8 *object, u32 position);
void FieldCamera_FollowActor(s32 id, s32 flag);
void Actor_UpdateAllMotion(void);
void Actor_UpdateAllThumb(void);
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);
void ObjectSystem_UpdateCamera(void);
void ObjectSystem_UpdateCameraFixed(void);
s32 Actor_FindLastActiveId(void);
void BattleMotion_ProjectPositionFar(s32, s32 *);
s32 Unit_KnowsPsynergyFar(s32 group, s32 entry);
s32 Party_ShowJoinedMessageFar(s32 event_id, s32 value);
s32 Party_ShowPairJoinedMessageFar(s32 first, s32 second);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);

struct AnimationObject;
struct EffectPosition;
struct EffectStep;
struct FacingObject;
struct Actor;
struct Actor;
struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneRegion;
struct ScriptInterpreter;

extern u32 gDriftScene_SpawnScript[];
extern u32 gIdejimaExits[];
extern u32 gSceneExits[];
extern struct Actor gSnapshotObjects[];
extern u32 gIdejima_ActorDepartureScript[];
extern u8 gLinkLobby_SlotColumns[];
extern s32 gLinkLobby_SlotValues[];
extern const u8 gActor_BlinkChildValues[4];
extern const u8 gActor_StepAngleScript[];
extern u16 gScript_MainScript[];
s32 Animation_InitializeObjects(struct AnimationObject *);
void Actor_SetBlendMode(struct Actor *object, u32 value);
void AnimationObject_SetActiveTimerSteps(struct AnimationObject *, s32);
s32 BattleEffect_ApplyColorToTargetBufferFar(s32, s32);
void FieldPsynergy_FinishMindRead(void);
void FieldPsynergy_Dispatch(void);
void BattleEffect_LoadActionEffectResources(s32 action, s32 flags);
void FieldPsynergy_Run(void);
void BattleEffect_SelectBattleCue(s32 row, s32 column);
void FieldPsynergy_SetupActorPair(s32 actor, s32 target);
s32 BattleEffect_StartBufferInterpolationFar(s32);
s32 BattleMotion_ProjectScaledPositionFar(s32, s32 *);
struct BattleEffectArgument;
void BattleEffect_RunBurstScene(struct BattleEffectArgument *effect,
                                s32 variant);
void BattleEffect_StopMainActor(void);
struct ActorPlacement;
void Event_SpawnObjectTable(struct ActorPlacement *entry, s32 slot);
void Actor_TurnPairToFaceEachOther(struct FacingObject *first,
                                   struct FacingObject *second);
void Field_StartPaletteTransition(s32 mode);
void FieldPsynergy_RunHandler(s32 kind);
void Field_EndActorSpriteEffect(s32 mode);
void FieldPsynergy_RunExtraHandler(void);

s32 Func_080c88c8(struct ScriptInterpreter *interpreter, s32 value);
void Actor_ReleaseAtIndex(u32 actorIndex);
void Actor_SetSquareSprite(u32 objectId);
void BattleEffect_DropObjectAtAnchor(u32 objectId);
void Func_081b83c4(void);
void GameFlag_RefreshLureCapFar(void);
void Map_CenterOnFollowedPointFar(void);
s32 MapGrid_GetTerrainHeightFar(u8, s32, s32);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
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
void Render_PlaceProjectedSprite(void *sprite, s32 *position, s32 *scale,
                                 s32 angle, s32 layer);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void Scene_SetArrivalFlags(s32 unused);
s32 Message_OpenWindowFar(s32, s32, s32, s32);
void AreaNameWindow_HideFar(void);
s32 Window_IsClosedFar(s32);

extern u8 gWorldMapPackedTiles[];
extern u8 gWorldMapPackedPictures[];
extern u8 gWorldMapPalettes[];

#endif
