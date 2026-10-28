#ifndef FIELD_H
#define FIELD_H

#include "types.h"
#include "edition.h"
#include "actor.h"

struct DispatchObject;
struct EffectPosition;

extern const s32 gCamera_FixedViewMatrix[];
extern s32 gIwram_Data1;
extern s32 gIwram_Data2;
extern u8 gMap_TileDissolveOrder[];
extern const s32 gObjectDispatch_DefaultScript[];
extern const u8 gWorldMap_TerrainBehaviorTable[];
extern u8 Render_DecodePictureCodeSize[];
extern u8 gRuntime_ByteRemapTable[];
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
s32 Event_ValidatePackedId(u32 packed_id);
void Map_RenderAnimatedTilePicture(u8 *object, u32 position);
void Camera_FollowActor(s32 id, s32 flag);
void Actor_UpdateAllMotion(void);
void Actor_UpdateAllThumb(void);
#ifndef OWN_ObjectDispatch_SetSingleChildFlags
void Actor_SetSingleChildFlags(struct DispatchObject *object, u32 value);
#endif
void ObjectSystem_UpdateCamera(void);
void ObjectSystem_UpdateCameraFixed(void);
s32 ObjectTable_FindLastActiveId(void);
void BattleMotion_ProjectPositionFar(s32, s32 *);
s32 Unit_KnowsActionFar(s32 group, s32 entry);
s32 Party_ShowJoinedMessageFar(s32 event_id, s32 value);
s32 Party_ShowPairJoinedMessageFar(s32 first, s32 second);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);

struct AnimationObject;
struct EffectPosition;
struct EffectStep;
struct FacingObject;
struct FieldActor;
struct Actor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;
struct ScriptInterpreter;

extern const u8 gDriftScene_SpawnScript[];
extern const struct SceneEntrance gDailaEntrances[];
extern const struct SceneEntrance gIdejimaEntrances[];
extern const struct SceneEvent gIdejimaEvents[];
extern const struct SceneEvent gIdejimaEventsEntrance1[];
extern const struct SceneEvent gIdejimaEventsWake[];
extern const u32 gIdejimaExits[];
extern const struct ScenePlacement gIdejimaPlacements[];
extern const struct SceneEntrance gSceneEntrances[];
extern const struct SceneEvent gSceneEvents[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct ScenePlacement gScenePlacementsEntrance99[];
extern const struct SceneRegion gSceneRegions[];
extern struct Actor gSnapshotObjects[];
extern const u8 gIdejima_ActorDepartureScript[];
extern u8 gLinkLobby_SlotColumns[];
extern const s32 gLinkLobby_SlotValues[];
extern const u8 gObjectGroup_BlinkChildValues[4];
extern const u8 gObjectMotion_StepAngleScript[];
extern u16 gScript_MainScript[];
s32 Animation_InitializeObjects(struct AnimationObject *);
void Animation_SetStateFlags(struct FieldActor *object, s32 flags);
void AnimationObject_SetActiveTimerSteps(struct AnimationObject *, s32);
s32 BattleEffect_ApplyColorToTargetBufferFar(s32, s32);
void BattleEffect_ClearChildValueOnMismatch(void);
s32 BattleEffect_DispatchRequestKind(void);
void BattleEffect_LoadActionEffectResources(s32 action, s32 flags);
s32 BattleEffect_Run(void);
s32 BattleEffect_SelectBattleCue(s32 row, s32 column);
void BattleEffect_SetupObjectPair(s32 actor, s32 target);
s32 BattleEffect_StartBufferInterpolationFar(s32);
void BattleMotion_ProjectScaledPositionFar(s32, s32 *);
void BattleEffect_RunBurstScene(struct EffectStep *, s32);
void EffectRuntime_StopCurrentObject(void);
s32 Event_SpawnObjectTable(s32 event_id, s32 state);
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first,
                                          struct FacingObject *second);
void Field_StartPaletteTransition(s32 mode);
void Field_DispatchTypeHandler(s32 kind);
void Field_EndActorSpriteEffect(s32 mode);
void FieldEvent_RunTypeHandler(void);
void Func_02000b2c(void);
void Func_02000d30(void);
void Func_02001c7c(s32, s32);
void Func_02001c84(s32, s32);
void Func_02001c8c(void);
void Func_02001c94(void);
void Func_02001c9c(void);
void Func_020023a4(void *object);
void Func_0200258c(void);
void Func_0200260c(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002a34(s32 a, s32 b);
void Func_02002a44(void);
void Func_02002a4c(void);
void Func_02002a54(void);
void Func_02002a5c(struct Actor *object, s32 mode);
s32 Func_080c88c8(struct ScriptInterpreter *interpreter, s32 value);
void Func_080caa2c(u32 objectId);
void Actor_SetSquareSprite(u32 objectId);
void BattleEffect_DropObjectAtAnchor(u32 objectId);
void Func_081b83c4(void);
void GameFlag_RefreshLureCapFar(void);
void Map_CenterOnFollowedPointFar(void);
s32 Map_GetTerrainHeightFar(u8, s32, s32);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
s32 Actor_IsTargetUnset(void *arg0);
#ifndef OWN_Object_SetPartAttribute
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
#endif
void Scene_SetActorSpritePriority(struct FieldActor *object, s32 priority);
void Actor_StartCameraFar(void *object, void *effect);
void ObjectEffect_EndContextEffect(s32 effect);
void ObjectEffect_PrepareContextEffect(s32 effect);
void Actor_CenterOnColumn(u32 objectId, s32 action, s32 z_offset);
void Actor_CommitPositionAndActivate(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_OffsetPositionAndReset(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_ResetAndSetPosition(u32 objectId, s32 x, s32 z);
void Actor_SetPositionAndCommit(u32 objectId, s32 x, s32 z);
void Actor_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z);
void Actor_StartRepeatedMotion(u32 objectId, s32 variant);
void ObjectSystem_Configure(s32 mode);
s32 Party_AddActiveOwnerFar(s32 event_id);
void Party_RemoveActiveOwnerFar(s32 owner);
void Render_ApplyProjectedPlacement(void *sprite, s32 *position, s32 *scale,
                                    u16 angle);
void Render_PlaceProjectedSprite(void *sprite, s32 *position, s32 *scale,
                                 s32 angle, s32 layer);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void Scene_SetArrivalFlags(s32 unused);
s32 Message_OpenWindowFar(s32, s32, s32, s32);
void UiTimedNotice_CloseIfActiveFar(void);
s32 Window_IsClosedFar(s32);

#endif
