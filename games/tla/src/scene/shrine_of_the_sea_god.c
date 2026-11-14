/// @file
/// The Shrine of the Sea God east of Daila, in eleven rooms that share this
/// overlay (SceneId_ShrineOfTheSeaGod1 to SceneId_ShrineOfTheSeaGod11).
///
/// On the first visit Tavi is stuck on a ledge in the entrance hall (room 1)
/// and Riki cannot throw him the rope; Riki goes off to the side room (room 2)
/// and Lash ties the rope so Tavi climbs down. Past the great hall (room 3),
/// where the Djinni Breath is first seen, its footprints cross the room below
/// (room 4) and it is chased around a ledge (room 5) until the torch pushed
/// against the north wall traps it. On the second visit, with Frost on the
/// puddles, Lash and the fallen pillars (rooms 6 and 7), the party reaches the
/// statue on the island (room 8), whose Sea God's Tear raises the water up to
/// the Right Prong (rooms 9 to 11).
#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect/spawn.c"

#include "types.h"
#include "item_ids.h"
#include "field/emote.h"
#include "field/speaker.h"
#include "flags.h"
#include "djinn_ids.h"
#include "sounds.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"

#define FLAG_ROOM3_PILLAR_MOVED 0x84c ///< Move pushed the great hall's pillar
#define FLAG_RIKI_GAVE_UP 0x872       ///< Riki missed with the rope, went off
#define FLAG_DJINNI_FLED_EAST 0x874   ///< the Djinni ran from the great hall
#define FLAG_DJINNI_FLED_SOUTH 0x875  ///< and ran on, out of it
#define FLAG_DJINNI_RAN_PAST 0x876    ///< its footprints are gone
#define FLAG_DJINNI_BY_LEDGE 0x877    ///< it waits by the ledge
#define FLAG_DJINNI_ON_LEDGE 0x87c    ///< it hopped up onto the ledge
#define FLAG_FIRST_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
#define FLAG_SECOND_PUDDLE_FROZEN FLAG_SCENE_TEMP_01
#define FLAG_THIRD_PUDDLE_FROZEN FLAG_SCENE_TEMP_02
#define FLAG_ROPE_TIED FLAG_SCENE_TEMP_03
#define FLAG_SECOND_ROPE_TIED FLAG_SCENE_TEMP_04
#define FLAG_DJINNI_CHASE_OVER FLAG_SCENE_TEMP_05
#define FLAG_DJINNI_TRAPPED FLAG_SCENE_TEMP_06
#define FLAG_TORCH_AT_NORTH_WALL FLAG_SCENE_TEMP_07
#define FLAG_SLIDES_SHOWN FLAG_SCENE_TEMP_08
#define FLAG_LEADER_IN_FRONT FLAG_SCENE_TEMP_0A
#define FLAG_ROOM3_PILLAR_TOPPLED FLAG_SCENE_TEMP_30
#define FLAG_FIRST_PILLAR_TOPPLED FLAG_SCENE_TEMP_31
#define FLAG_SECOND_PILLAR_TOPPLED FLAG_SCENE_TEMP_32
#define FLAG_PILLAR_IN_HOLE FLAG_AREA_TEMP_01
/// Set as the battle with Breath starts, with where it was trapped, so the
/// scene puts it back there if the party runs.
#define FLAG_FIGHTING_BREATH FLAG_AREA_TEMP_40
#define FLAG_BREATH_TRAPPED_EAST FLAG_AREA_TEMP_41
#define FLAG_BREATH_TRAPPED_WEST FLAG_AREA_TEMP_42

struct ShrineOfTheSeaGodObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 terrainHeight;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[2];
  u8 terrainId;
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  u8 unk2c[0x24];
  void *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 unk5a[8];
  u8 state;
  u8 unk63[9];
  void (*update)(struct ShrineOfTheSeaGodObject *object);
};

struct ShrineOfTheSeaGodSprite {
  u8 unk00[0x12];
  u16 rotation;
};

struct ShrineOfTheSeaGodMapLayer {
  u8 unk00[8];
  s32 x;
  s32 y;
  u8 unk10[0x28];
};

struct ShrineOfTheSeaGodMapWork {
  u8 unk000[0x17];
  u8 low17 : 2;
  u8 mode17 : 2;
  u8 high17 : 4;
  u8 unk018[0xf0];
  struct ShrineOfTheSeaGodMapLayer layers[3];
};

struct ShrineOfTheSeaGodObject *Actor_GetById(s32 id);
void Event_Wait(s32 frames);
s32 GameFlag_Get(s32 flag);
void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
void FieldMessage_Show(s32 message, s32 position);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Actor_SetPositionAndResetMotion(struct Actor *obj, s32 x, s32 y, s32 z);
void Actor_SetModeById(u32 actorId, s32 animation);
void Event_Start(void);
void Event_End(void);
void Event_ResolvePendingActions(s32 skip);
void Actor_DisableById(s32 actor);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 width,
                                        s32 height, s32 destX, s32 destY);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Field_RequestDjinnBattle(s32 actor, s32 value);
void Actor_GetCellIndex(void);
void Actor_LiftTargetBlock(const void *script);
void Actor_PlaceMovedBlock(void);
void Sound_Play(s32 cue);
void ShrineOfTheSeaGod_SetSlideActorsShown(s32 show);
void ShrineOfTheSeaGod_KeepWithLeader(struct ShrineOfTheSeaGodObject *object);

static inline void ShrineOfTheSeaGod_CopyRect(s32 sx, s32 sy, s32 w, s32 h,
                                              s32 dx, s32 dy) {
  /* FAKEMATCH(wrapper): the rectangle copy reaches the main image through one
   * inline layer. */
  MapGrid_CopyCellPictureIndicesRect(sx, sy, w, h, dx, dy);
}

static inline void ShrineOfTheSeaGod_BlendStart(s32 a, s32 b, s32 c, s32 d,
                                                s32 e, s32 f) {
  /* FAKEMATCH(wrapper): the blend start reaches the main image through one
   * inline layer. */
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actor, s32 x, s32 z,
                                                    s32 angle);
void Event_SetMessageId(s32 message);
void Event_ShowMessageThenWait(s32 actor, s32 a, s32 b);
void Event_ShowMessageAndWait(s32 speaker, s32 flags);
s32 Actor_ShowEmoteById(s32 actorId, s32 emote, s32 frames);
void FieldEffect_SetAttached(s32 actor, s32 emote);
void Camera_SetSpeedParameters(s32 speed, s32 acceleration);
void FieldCamera_WaitForArrival(void);
struct ShrineOfTheSeaGodObject *FieldCamera_GetActor(void);
void Actor_SetDestinationById(u32 objectId, s32 x, s32 z);
void Actor_WaitForMoveById(s32 actor);
void Actor_Stop(s32 actor);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Actor_FaceActorById(s32 actor, s32 target, s32 frames);
void Actor_WatchAndSetScript(s32 actor, s32 target);
void FieldCamera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void Actor_MoveToAndWaitById(u32 objectId, s32 x, s32 z);
void Actor_SetSpriteFlags(struct Actor *object, u32 flags);

static inline void ShrineOfTheSeaGod_Jump(s32 actor, s32 height, s32 frames) {
  Actor_ShowEmoteById(actor, height, frames);
}

static inline void ShrineOfTheSeaGod_ObjSpeed(s32 actor, s32 speed,
                                              s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static inline void ShrineOfTheSeaGod_ObjWalk(s32 actor, s32 x, s32 z) {
  Actor_WalkToAndWaitById(actor, x, z);
}

static inline void ShrineOfTheSeaGod_ObjCommit(s32 actor, s32 x, s32 z) {
  Actor_MoveToAndWaitById(actor, x, z);
}

static inline void ShrineOfTheSeaGod_ObjTurn(s32 actor, s32 angle, s32 frames) {
  Actor_ArmCallback(actor, angle, frames);
}

static inline void ShrineOfTheSeaGod_ObjPlace(s32 actor, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(actor, x, z);
}

static inline void
ShrineOfTheSeaGod_ObjSetPos(struct ShrineOfTheSeaGodObject *object, s32 x,
                            s32 y, s32 z) {
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

static inline void ShrineOfTheSeaGod_CamSpeed(s32 speed, s32 acceleration) {
  Camera_SetSpeedParameters(speed, acceleration);
}

static inline void ShrineOfTheSeaGod_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static inline void ShrineOfTheSeaGod_MessagePut(s32 speaker, s32 flags) {
  Event_ShowMessageAndWait(speaker, flags);
}

static inline void ShrineOfTheSeaGod_ObjEmote(s32 actor, s32 emote) {
  FieldEffect_SetAttached(actor, emote);
}

void Task_Add(void (*func)(void), s32 priority);

static inline void ShrineOfTheSeaGod_TaskAdd(void (*func)(void), s32 priority) {
  Task_Add(func, priority);
}

#define SHRINE_OF_THE_SEA_GOD_TASK_ADD_LATE(func, priority)                    \
  do {                                                                         \
    s32 priority_;                                                             \
    priority_ = (priority);                                                    \
    Task_Add((func), priority_);                                               \
  } while (0)

static inline s32 ShrineOfTheSeaGod_FlagCheck(s32 flag) {
  return GameFlag_Get(flag);
}

static inline void ShrineOfTheSeaGod_ObjPlaceAt(s32 actor, s32 x, s32 z,
                                                s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}

static inline void ShrineOfTheSeaGod_ObjPlaceTurn(s32 actor, s32 x, s32 z,
                                                  u16 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}
#include "field/field_sequence_event.h"
#include "module/scene/scene.h"
#include "module/scene/shrine_of_the_sea_god.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "call.h"
#include "macros.h"
#include "system/frame.h"
#include "module/field/field_effect.h"
#include "shared/tables.h"

/// Talking to Breath once the torch has it trapped starts its battle.
void ShrineOfTheSeaGod_FightBreath(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *state = &Actor_GetById(14)->state;
  s32 y;

  if ((u8)(*state - 4) > 1)
    return;
  y = leader->y;
  if (y > 0xc0000) {
    GameFlag_Set(FLAG_FIGHTING_BREATH);
    if (*state == 4)
      GameFlag_Set(FLAG_BREATH_TRAPPED_EAST);
    else if (*state == 5)
      GameFlag_Set(FLAG_BREATH_TRAPPED_WEST);
    Field_RequestDjinnBattle(14, 67);
  }
}

struct Actor;
#include "field/field.h"

extern u32 gShrineOfTheSeaGod_Room9Regions[];
extern s16 gShrineOfTheSeaGod_Room10Regions[];
extern u32 gShrineOfTheSeaGod_Room11Regions[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShrineOfTheSeaGod9)
    return (const struct SceneRegion *)gShrineOfTheSeaGod_Room9Regions;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod10)
    return (const struct SceneRegion *)gShrineOfTheSeaGod_Room10Regions;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod11)
    return (const struct SceneRegion *)gShrineOfTheSeaGod_Room11Regions;
  return 0;
}

extern struct ActorPlacement gShrineOfTheSeaGod_Placements[];
extern struct ActorPlacement gShrineOfTheSeaGod_Room1Placements[];
extern struct ActorPlacement gShrineOfTheSeaGod_Room2Placements[];
extern struct ActorPlacement gShrineOfTheSeaGod_Room3Placements[];
extern struct ActorPlacement gShrineOfTheSeaGod_Room4Placements[];
extern struct ActorPlacement gShrineOfTheSeaGod_Room5Placements[];
extern struct ActorPlacement gShrineOfTheSeaGod_Room6Placements[];
extern struct ActorPlacement gShrineOfTheSeaGod_Room7Placements[];
extern struct ActorPlacement gShrineOfTheSeaGod_Room8Placements[];
extern struct Trigger gShrineOfTheSeaGod_Triggers[];
extern struct Trigger gShrineOfTheSeaGod_Room1Triggers[];
extern struct Trigger gShrineOfTheSeaGod_Room2Triggers[];
extern struct Trigger gShrineOfTheSeaGod_Room3Triggers[];
extern struct Trigger gShrineOfTheSeaGod_Room4Triggers[];
extern struct Trigger gShrineOfTheSeaGod_Room5Triggers[];
extern struct Trigger gShrineOfTheSeaGod_Room6Triggers[];
extern struct Trigger gShrineOfTheSeaGod_Room7Triggers[];
extern struct Trigger gShrineOfTheSeaGod_Room8Triggers[];
extern struct Trigger gShrineOfTheSeaGod_Room9Triggers[];

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShrineOfTheSeaGod1)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room1Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod2)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room2Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod3)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room3Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod4)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room4Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod5)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room5Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod6)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room6Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod7)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room7Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod8)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room8Placements;
  return (const struct ActorPlacement *)gShrineOfTheSeaGod_Placements;
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShrineOfTheSeaGod1)
    return gShrineOfTheSeaGod_Room1Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod2)
    return gShrineOfTheSeaGod_Room2Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod3)
    return gShrineOfTheSeaGod_Room3Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod4)
    return gShrineOfTheSeaGod_Room4Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod5)
    return gShrineOfTheSeaGod_Room5Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod6)
    return gShrineOfTheSeaGod_Room6Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod7)
    return gShrineOfTheSeaGod_Room7Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod8)
    return gShrineOfTheSeaGod_Room8Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod9)
    return gShrineOfTheSeaGod_Room9Triggers;
  return gShrineOfTheSeaGod_Triggers;
}

#include "text/msg_ids.h"

extern u32 gShrineOfTheSeaGod_RikiLooksAround[];

void ShrineOfTheSeaGod_ExamineStatue(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgFieldStatueLikeDailaShrine, 1);
  work->fieldMenu = 1;
  Event_End();
}

void ShrineOfTheSeaGod_StartleCompanion(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(11);
  ShrineOfTheSeaGod_Jump(11, 256, 20);
  Actor_FaceActorById(11, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgFieldStartledCompanion);
  Event_ShowMessageAndWait(11, 0);
  Event_ShowMessageAndWait(11, 0);
  Actor_SetModeById(11, 4);
  Event_ShowMessageThenWait(11, 0, 20);
  Actor_SetMovementTypeById(11, gShrineOfTheSeaGod_RikiLooksAround);
  Event_End();
}

/// Talking to actors 25 to 27 opens the field menu, as talking to nothing does.
void ShrineOfTheSeaGod_OpenFieldMenu(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->fieldMenu = 1;
}

void ShrineOfTheSeaGod_LiftWithWater(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  struct ShrineOfTheSeaGodMapLayer *layer2 = &map->layers[2];
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *camera = FieldCamera_GetActor();
  struct ShrineOfTheSeaGodObject *object8 = Actor_GetById(8);
  struct ShrineOfTheSeaGodObject *object9 = Actor_GetById(9);
  struct ShrineOfTheSeaGodObject *object10 = Actor_GetById(10);

  layer0->y += -0x8000;
  layer1->y += 0x8000;
  layer2->y += 0x8000;
  leader->z += -0x8000;
  camera->y += -0x8000;
  object8->z += -0x8000;
  object9->z += -0x8000;
  object10->z += -0x8000;
  wait(4);
}

void ShrineOfTheSeaGod_ScrollWaterUp(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer = &map->layers[0];

  layer->y += -px(1);
  wait(4);
}

/// Copies a 6 by 56 rectangle of cells from (x, y) to (18, 6), then
/// moves the first two layers' y by -0x2000 a frame for 16 frames.
void ShrineOfTheSeaGod_RaiseWaterBand(s32 x, s32 y) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  u8 i;

  ShrineOfTheSeaGod_CopyRect(x, y, 6, 56, 18, 6);
  for (i = 0; i < 16; i++) {
    layer0->y += -0x2000;
    layer1->y += -0x2000;
    wait(1);
  }
}

void BlendScript_Suspend(void);
void MapGrid_SetScreenShake(s32 a, s32 b, s32 c);

/// The Sea God's Tear (item 458) used on the statue on the island, actor 10,
/// makes the water rise.
void ShrineOfTheSeaGod_UseSeaGodsTear(void) {
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  BlendScript_Suspend();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  FieldCamera_MoveWithinMapBounds(248 << 16, -1, 168 << 16, 1);
  FieldCamera_WaitForArrival();
  Sound_Play(SE_SANDSTORM);
  Io_Write16(BLEND_ALPHA(1, 16), &REG_BLDALPHA);
  wait(10);
  Io_Write16(BLEND_ALPHA(2, 16), &REG_BLDALPHA);
  wait(10);
  Io_Write16(BLEND_ALPHA(3, 16), &REG_BLDALPHA);
  wait(10);
  Io_Write16(BLEND_ALPHA(4, 16), &REG_BLDALPHA);
  wait(10);
  Call3((void (*)())MapGrid_SetScreenShake, 0, 0x10000, 0x10000);
  ShrineOfTheSeaGod_CopyRect(59, 124, 9, 12, 12, 3);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpritePriority(8, 1);
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpritePriority(10, 1);
  ShrineOfTheSeaGod_CopyRect(46, 51, 6, 56, 18, 25);
  wait(10);
  Call3((void (*)())MapGrid_SetScreenShake, 0x10000, 0x10000, 0x10000);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 51);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 58);
  Io_Write16(BLEND_ALPHA(5, 15), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 65);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 72);
  Io_Write16(BLEND_ALPHA(6, 14), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 79);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 86);
  Io_Write16(BLEND_ALPHA(7, 13), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 93);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 100);
  Io_Write16(BLEND_ALPHA(8, 12), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 51);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 58);
  Io_Write16(BLEND_ALPHA(9, 11), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 65);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 72);
  Io_Write16(BLEND_ALPHA(10, 10), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 79);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 86);
  Io_Write16(BLEND_ALPHA(11, 9), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 93);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 100);
  Io_Write16(BLEND_ALPHA(12, 8), &REG_BLDALPHA);
  for (i = 0; i < 24; i++)
    ShrineOfTheSeaGod_ScrollWaterUp();
  for (i = 0; i < 24; i++)
    ShrineOfTheSeaGod_LiftWithWater();
  gFieldWork->transition = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetExitRequest(3);
}

void ShrineOfTheSeaGod_LowerWithWater(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  struct ShrineOfTheSeaGodMapLayer *layer2 = &map->layers[2];
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *camera = FieldCamera_GetActor();
  struct ShrineOfTheSeaGodObject *object8 = Actor_GetById(8);
  struct ShrineOfTheSeaGodObject *object9 = Actor_GetById(9);
  struct ShrineOfTheSeaGodObject *object10 = Actor_GetById(10);

  layer0->y += 0x8000;
  layer1->y += -0x8000;
  layer2->y += -0x8000;
  leader->z += 0x8000;
  camera->y += 0x8000;
  object8->z += 0x8000;
  object9->z += 0x8000;
  object10->z += 0x8000;
  wait(4);
}

void ShrineOfTheSeaGod_ScrollWaterDown(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer = &map->layers[0];

  layer->y += px(1);
  wait(4);
}

/// Copies a rectangle of cells from (x, y), then raises map layers 0 and 1 by
/// 0x2000 a frame for 16 frames.
void ShrineOfTheSeaGod_LowerWaterBand(s32 x, s32 y) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  u8 i;

  ShrineOfTheSeaGod_CopyRect(x, y, 6, 56, 18, 6);
  i = 0;
loop:
  layer0->y += 0x2000;
  layer1->y += 0x2000;
  wait(1);
  i++;
  if (i <= 15)
    goto loop;
}

void MapGrid_WaitForShakeToSettle(void);

/// Back on the island after the second Tear, the water sinks to where it was.
void ShrineOfTheSeaGod_LowerWater(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  struct ShrineOfTheSeaGodMapLayer *layer2 = &map->layers[2];
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *ride = FieldCamera_GetActor();
  struct ShrineOfTheSeaGodObject *a = Actor_GetById(8);
  struct ShrineOfTheSeaGodObject *b = Actor_GetById(9);
  struct ShrineOfTheSeaGodObject *c = Actor_GetById(10);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpritePriority(8, 1);
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpritePriority(10, 1);
  BlendScript_Suspend();
  wait(1);
  ShrineOfTheSeaGod_CopyRect(46, 51, 6, 56, 18, 25);
  ShrineOfTheSeaGod_CopyRect(84, 100, 6, 56, 18, 6);
  ShrineOfTheSeaGod_CopyRect(59, 124, 9, 12, 12, 3);
  Io_Write16(0x80c, &REG_BLDALPHA);
  wait(1);
  layer0->y += -px(68);
  layer1->y += -px(20);
  layer2->y += px(12);
  leader->z += -px(12);
  ride->y += -px(12);
  a->z += -px(12);
  b->z += -px(12);
  c->z += -px(12);
  Map_CenterOnFollowedPoint();
  wait(4);
  gFieldWork->transition = 0x209;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call3((void (*)())MapGrid_SetScreenShake, 0x10000, 0x10000, 0x10000);
  Sound_Play(SE_245);
  i = 0;
  do {
    i++;
    ShrineOfTheSeaGod_LowerWithWater();
  } while (i <= 24);
  i = 0;
  do {
    i++;
    ShrineOfTheSeaGod_ScrollWaterDown();
  } while (i <= 24);
  ShrineOfTheSeaGod_LowerWaterBand(84, 93);
  ShrineOfTheSeaGod_LowerWaterBand(84, 86);
  Io_Write16(0x90b, &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(84, 79);
  ShrineOfTheSeaGod_LowerWaterBand(84, 72);
  Io_Write16(0xa0a, &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(84, 65);
  ShrineOfTheSeaGod_LowerWaterBand(84, 58);
  Io_Write16(0xb09, &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(84, 51);
  ShrineOfTheSeaGod_LowerWaterBand(65, 100);
  Io_Write16(0xc08, &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(65, 93);
  ShrineOfTheSeaGod_LowerWaterBand(65, 86);
  Io_Write16(0xd07, &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(65, 79);
  ShrineOfTheSeaGod_LowerWaterBand(65, 72);
  Io_Write16(0xe06, &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(65, 65);
  ShrineOfTheSeaGod_LowerWaterBand(65, 58);
  Io_Write16(0xf05, &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(65, 51);
  Call3((void (*)())MapGrid_SetScreenShake, 0, 0x10000, 0x10000);
  Io_Write16(0x1004, &REG_BLDALPHA);
  wait(10);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Call3((void (*)())MapGrid_SetScreenShake, -1, -1, 0xe666);
  Io_Write16(0x1003, &REG_BLDALPHA);
  wait(10);
  Io_Write16(0x1002, &REG_BLDALPHA);
  wait(10);
  Io_Write16(0x1001, &REG_BLDALPHA);
  wait(10);
  Io_Write16(0x1000, &REG_BLDALPHA);
  ShrineOfTheSeaGod_CopyRect(103, 51, 6, 56, 18, 25);
  ShrineOfTheSeaGod_CopyRect(46, 108, 9, 12, 12, 3);
  wait(10);
  MapGrid_WaitForShakeToSettle();
  gFieldWork->transition = 0x204;
  ShrineOfTheSeaGod_SetUpFlatProp(8);
  ShrineOfTheSeaGod_SetUpFlatProp(9);
  ShrineOfTheSeaGod_SetUpFlatProp(10);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  leader->motionFlags = 3;
  leader->y = 0;
  leader->terrainHeight = 0;
  Event_End();
}

extern s16 gShrineOfTheSeaGod_MovableActors[];

/// The leader is placed at y -50 and map layer 1 moved by -50; then both move
/// by 0x8000 in y a frame for 100 frames as a screen transition starts.
void ShrineOfTheSeaGod_RiseOnWater(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer;
  struct ShrineOfTheSeaGodObject *leader;
  u32 i;
  s32 frames;

  layer = &map->layers[1];
  leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_SANDSTORM);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  FieldCamera_MoveWithinMapBounds(172 << 17, -1, 248 << 16, 0);
  ShrineOfTheSeaGod_CopyRect(62, 77, 62, 18, 19, 6);
  wait(1);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  leader->motionFlags = 0;
  leader->y = -50 << 16;
  leader->terrainHeight = -50 << 16;
  layer->y += -50 << 16;
  Map_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition = 515;
  Event_StartScreenTransition();
  Call3((void (*)())MapGrid_SetScreenShake, 0x10000, 0x10000, 0x10000);
  i = 0;
loop:
  leader->y += 0x8000;
  frames = 2;
  leader->terrainHeight += 0x8000;
  /* FAKEMATCH(barrier): wait()'s argument is set before the count. */
  asm volatile("" : "+l"(frames));
  i++;
  /* FAKEMATCH(barrier): the layer is read after the count. */
  asm("" : "+l"(layer) : "l"(i));
  layer->y += 0x8000;
  wait(frames);
  if (i <= 99)
    goto loop;
  Sound_Play(SE_288);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetScreenShake(0, 0, 0);
  gFieldWork->transition = 521;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  ShrineOfTheSeaGod_CopyRect(62, 69, 62, 18, 19, 6);
  wait(1);
  Event_End();
}

/// The leader turns, then moves with map layer 1 by -0x8000 in y a frame for
/// 100 frames; at frame 90 the screen transition finishes.
void ShrineOfTheSeaGod_SinkWithWater(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer;
  struct ShrineOfTheSeaGodObject *object;
  u32 i;

  layer = &map->layers[1];
  object = Actor_GetById(gMainRoutineWork.currentOwner);

  Event_Start();
  Event_ResolvePendingActions(0);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  FieldCamera_MoveWithinMapBounds(344 << 16, -1, 248 << 16, 1);
  ShrineOfTheSeaGod_CopyRect(62, 77, 62, 18, 19, 6);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 20);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  object->motionFlags = 0;
  Sound_Play(SE_245);
  Call3((void (*)())MapGrid_SetScreenShake, 0x10000, 0x10000, 0x10000);
  i = 0;
loop:
  object->y -= 0x8000;
  object->terrainHeight -= 0x8000;
  layer->y -= 0x8000;
  wait(2);
  if (i == 90) {
    gFieldWork->transition = 0x203;
    Event_FinishScreenTransition();
  }
  i++;
  if (i <= 99)
    goto loop;
  Event_SetExitRequest(1);
}

void ShrineOfTheSeaGod_SetUpPuddle(s32 actor) {
  Actor_DisableById(actor);
  Actor_GetById(actor)->priorityFlags |= 0x20;
}

/// Frost on the first puddle turns it to an ice pillar.
void ShrineOfTheSeaGod_FreezeFirstPuddle(void) {
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 8, 18);
  GameFlag_Set(FLAG_FIRST_PUDDLE_FROZEN);
}

/// Frost on the second puddle turns it to an ice pillar.
void ShrineOfTheSeaGod_FreezeSecondPuddle(void) {
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 18);
  GameFlag_Set(FLAG_SECOND_PUDDLE_FROZEN);
}

/// Frost on the third puddle turns it to an ice pillar.
void ShrineOfTheSeaGod_FreezeThirdPuddle(void) {
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 17);
  GameFlag_Set(FLAG_THIRD_PUDDLE_FROZEN);
}

void ShrineOfTheSeaGod_LiftBlock(void) {
  Actor_LiftTargetBlock(gShrineOfTheSeaGod_MovableActors);
}

void ShrineOfTheSeaGod_SetDownTorch(void) {
  struct ShrineOfTheSeaGodObject *object;

  Actor_PlaceMovedBlock();
  object = Actor_GetById(8);
  if (object->x >> 20 == 21 && object->z >> 20 == 13)
    GameFlag_Set(FLAG_TORCH_AT_NORTH_WALL);
  else
    GameFlag_Clear(FLAG_TORCH_AT_NORTH_WALL);
}

void ShrineOfTheSeaGod_MovePillar(void) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(8);

  Actor_PlaceMovedBlock();
  if (object->x >> 20 == 12)
    GameFlag_Set(FLAG_ROOM3_PILLAR_MOVED);
}

void ShrineOfTheSeaGod_DropPillar(struct ShrineOfTheSeaGodObject *object) {
  object->motionFlags = 3;
  do {
    wait(1);
  } while (object->velocityY != 0);
  Sound_Play(SE_THUMP);
  wait(10);
  object->motionFlags = 0;
}

void ShrineOfTheSeaGod_PushPillarIntoHole(void) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(8);
  s32 z = object->z >> 20;

  if (z == 16) {
    Event_Start();
    Event_ResolvePendingActions(0);
    ShrineOfTheSeaGod_DropPillar(object);
    Map_CopyCellAttributeRect(0, 0, 1, 1, 15, z);
    Actor_SetSpritePriority(8, 3);
    Event_End();
    GameFlag_Set(FLAG_PILLAR_IN_HOLE);
  }
}

void ShrineOfTheSeaGod_LayerRoom3Props(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  u32 id;

  for (id = 9; id <= 13; id++) {
    struct ShrineOfTheSeaGodObject *object = Actor_GetById(id);

    if (leader->y > px(16)) {
      Actor_SetSpritePriority(id, 2);
      object->priorityFlags |= 2;
    } else {
      Actor_SetSpritePriority(id, 1);
      object->priorityFlags &= ~2;
    }
  }
}

void ShrineOfTheSeaGod_TrackUpperLevel(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);

  Actor_GetCellIndex();
  if (leader->y < px(16))
    gMainRoutineWork.unk2c3 = 0;
  else
    gMainRoutineWork.unk2c3 = 1;
}

void ShrineOfTheSeaGod_LayerPuddles(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  s32 y;
  s32 z;

  y = leader->y;
  if (y < 0x100000) {
    z = leader->z;
    if (z < 0x1280000) {
      Actor_SetSpritePriority(10, 1);
      Actor_SetSpritePriority(11, 1);
      Actor_SetSpritePriority(12, 2);
    } else {
      Actor_SetSpritePriority(10, 2);
      Actor_SetSpritePriority(11, 2);
      Actor_SetSpritePriority(12, 3);
    }
  } else {
    Actor_SetSpritePriority(10, 2);
    Actor_SetSpritePriority(11, 2);
    Actor_SetSpritePriority(12, 3);
  }
}

void ShrineOfTheSeaGod_KeepFlameOnTorch(void) {
  struct ShrineOfTheSeaGodObject *first = Actor_GetById(8);
  struct ShrineOfTheSeaGodObject *second = Actor_GetById(9);

  SET_POSITION(second, first->x, 0x140000, first->z);
}

// XXX: what priority flag 0x20 does to the leader, which the slide room sets on
// arrival; a render of the slide room with and without it would settle it.
UNUSED void ShrineOfTheSeaGod_ClearLeaderFlag20(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags &= ~0x20;
}

UNUSED void ShrineOfTheSeaGod_SetLeaderFlag20(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
}

void ShrineOfTheSeaGod_SetActorsHidden(s32 show, s32 first, u32 count);

void ShrineOfTheSeaGod_SetSlideActorsShown(s32 show) {
  if (gMainRoutineWork.scene == (s32)&SceneId_ShrineOfTheSeaGod6)
    ShrineOfTheSeaGod_SetActorsHidden(show, 9, 3);
}

UNUSED void ShrineOfTheSeaGod_ShowSlideActors(void) {
  ShrineOfTheSeaGod_SetSlideActorsShown(1);
  GameFlag_Set(FLAG_SLIDES_SHOWN);
}

void ShrineOfTheSeaGod_HideSlideActors(void) {
  ShrineOfTheSeaGod_SetSlideActorsShown(0);
  GameFlag_Clear(FLAG_SLIDES_SHOWN);
}

void ShrineOfTheSeaGod_SetActorsHidden(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct ShrineOfTheSeaGodObject *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~0x10;
    else
      actor->priorityFlags |= 0x10;
  }
}

void ShrineOfTheSeaGod_DrawLeaderInFront(void) {
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  GameFlag_Set(FLAG_LEADER_IN_FRONT);
}

void ShrineOfTheSeaGod_DrawLeaderBehind(void) {
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  GameFlag_Clear(FLAG_LEADER_IN_FRONT);
}

#include "../field/field_effect/advance_motion.c"

#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#include "text/msg_ids.h"

extern u32 gShrineOfTheSeaGod_PillarOffset[];

/// The leader rocks a pillar back and forth, then it is carried along with him.
void ShrineOfTheSeaGod_RockPillar(struct ShrineOfTheSeaGodObject *object,
                                  struct ShrineOfTheSeaGodObject *other) {
  struct ShrineOfTheSeaGodSprite *sprite = object->sprite;
  u32 n;
  u32 i;

  FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Sound_Play(SE_HIDDEN_PASSAGE_OPEN);
  object->motionFlags = 0;
  for (n = 0; n <= 1; n++) {
    other->y -= px(1);
    for (i = 0; i <= 3; i++) {
      sprite->rotation += 128;
      wait(1);
    }
    other->y += px(1);
    for (i = 0; i <= 7; i++) {
      sprite->rotation -= 128;
      wait(1);
    }
    other->y -= px(1);
    for (i = 0; i <= 3; i++) {
      sprite->rotation += 128;
      wait(1);
    }
  }
  other->y += px(1);
  wait(20);
  gShrineOfTheSeaGod_PillarOffset[0] = other->x - object->x;
  gShrineOfTheSeaGod_PillarOffset[1] = other->y - object->y;
  gShrineOfTheSeaGod_PillarOffset[2] = other->z - object->z;
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
}

void ShrineOfTheSeaGod_KeepWithLeader(struct ShrineOfTheSeaGodObject *object) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  s32 *offset = gShrineOfTheSeaGod_PillarOffset;

  SET_POSITION(object, leader->x + offset[0], leader->y + offset[1],
               leader->z + offset[2]);
}

void ShrineOfTheSeaGod_KeepRoom3PillarWithLeader(void) {
  ShrineOfTheSeaGod_KeepWithLeader(Actor_GetById(14));
}

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void MapGrid_SetScreenShake(s32 a, s32 b, s32 c);
void MapGrid_WaitForShakeToSettle(void);
s32 Trig_Sin(s32 angle);

/// The leader rocks the great hall's pillar until it topples into the water,
/// riding it down.
void ShrineOfTheSeaGod_TopplePillarIntoWater(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *stone = Actor_GetById(14);
  struct ShrineOfTheSeaGodSprite *sprite = stone->sprite;
  struct EffectOptions options;
  u32 i;
  s32 spin;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 408, 328);
  ShrineOfTheSeaGod_RockPillar(leader, stone);
  ShrineOfTheSeaGod_BlendStart(0, 50, 3, 1, 24, 70);
  GameFlag_Set(FLAG_ROOM3_PILLAR_TOPPLED);
  wait(1);
  leader->motionFlags = 3;
  Actor_SetSpritePriority(14, 3);
  stone->priorityFlags |= 2;
  Task_Add(ShrineOfTheSeaGod_KeepRoom3PillarWithLeader, 0x480);
  do
    wait(1);
  while (leader->velocityY != 0);
  Sound_Play(SE_TOPPLE);
  Call3((void (*)())MapGrid_SetScreenShake, 0x10000, 0x20000, 0x10000);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  for (i = 0; i <= 16;) {
    s32 velocity[3];

    velocity[0] = Trig_Cos(i << 12);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(i << 12);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14) >> 16;
    i++;
    velocity[0] += -0x6000;
    velocity[2] += (((Random16() * 3) << 13) >> 16) + -0x3000;
    FieldEffect_Spawn(leader->x, leader->y + -0x40000, leader->z, velocity[0],
                      velocity[1], velocity[2], 0x01090001, &options);
  }
  leader->velocityY = 0x40000;
  wait(2);
  Call3((void (*)())MapGrid_SetScreenShake, -1, -1, 0xe666);
  Task_Remove(ShrineOfTheSeaGod_KeepRoom3PillarWithLeader);
  spin = 0x200;
  for (i = 0; i <= 7;) {
    sprite->rotation += spin;
    /* FAKEMATCH(dead): an empty once-only block after the rotation's store. */
    do {
    } while (0);
    stone->z += px(1);
    i++;
    wait(1);
  }
  MapGrid_WaitForShakeToSettle();
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  leader->terrainId = 0;
  wait(10);
  Event_End();
}

void ShrineOfTheSeaGod_LandPillarAsLogs(struct ShrineOfTheSeaGodObject *object,
                                        s32 base) {
  do {
    wait(1);
  } while (object->velocityY != 0);
  Sound_Play(SE_LOG_FALL);
  Actor_SetSpritePriority(base + 21, 3);
  Actor_SetSpritePriority(base + 22, 3);
  Actor_SetPositionAndResetMotion(Actor_GetById(base + 21), object->x, 0,
                                  object->z);
  Actor_SetPositionAndResetMotion(Actor_GetById(base + 22), object->x, 0,
                                  object->z + -0x100000);
  Actor_SetModeById(base + 21, 1);
  Actor_SetModeById(base + 22, 1);
  wait(20);
}

void ShrineOfTheSeaGod_KeepFirstPillarWithLeader(void) {
  ShrineOfTheSeaGod_KeepWithLeader(Actor_GetById(15));
}

void ShrineOfTheSeaGod_ToppleFirstPillar(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *pillar = Actor_GetById(15);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 328, 216);
  ShrineOfTheSeaGod_RockPillar(leader, pillar);
  Map_CopyCellAttributeRect(0, 0, 3, 1, 19, 13);
  Map_CopyCellAttributeRect(40, 0, 2, 1, 19, 33);
  wait(1);
  leader->motionFlags = 3;
  Actor_SetSpritePriority(15, 3);
  pillar->priorityFlags |= 2;
  Task_Add(ShrineOfTheSeaGod_KeepFirstPillarWithLeader, 0x480);
  ShrineOfTheSeaGod_LandPillarAsLogs(leader, 0);
  Actor_SetSpritePriority(9, 1);
  Actor_GetById(9)->priorityFlags &= ~2;
  Actor_SetSpritePriority(11, 1);
  Actor_GetById(11)->priorityFlags &= ~2;
  leader->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x9999, 0x1999);
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, 328, 224);
  ShrineOfTheSeaGod_ObjCommit(gMainRoutineWork.currentOwner, 321, 256);
  Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  ShrineOfTheSeaGod_ObjCommit(gMainRoutineWork.currentOwner, 321, 320);
  ShrineOfTheSeaGod_ObjCommit(gMainRoutineWork.currentOwner, 321, 400);
  ShrineOfTheSeaGod_ObjCommit(gMainRoutineWork.currentOwner, 321, 480);
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, 321, 536);
  Sound_Play(SE_RATTLE);
  Actor_SetSpritePriority(9, 2);
  Actor_GetById(9)->priorityFlags |= 2;
  Actor_SetSpritePriority(11, 2);
  Actor_GetById(11)->priorityFlags |= 2;
  leader->motionFlags = 3;
  Task_Remove(ShrineOfTheSeaGod_KeepFirstPillarWithLeader);
  wait(1);
  Event_End();
  GameFlag_Set(FLAG_FIRST_PILLAR_TOPPLED);
}

void ShrineOfTheSeaGod_KeepSecondPillarWithLeader(void) {
  ShrineOfTheSeaGod_KeepWithLeader(Actor_GetById(16));
}

void ShrineOfTheSeaGod_ToppleSecondPillar(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *pillar = Actor_GetById(16);

  Event_Start();
  Event_ResolvePendingActions(0);
  ShrineOfTheSeaGod_ObjWalk(gMainRoutineWork.currentOwner, 408, 360);
  ShrineOfTheSeaGod_RockPillar(leader, pillar);
  Map_CopyCellAttributeRect(0, 0, 3, 1, 24, 22);
  Map_CopyCellAttributeRect(40, 0, 2, 1, 24, 24);
  wait(1);
  leader->motionFlags = 3;
  Actor_SetSpritePriority(16, 3);
  pillar->priorityFlags |= 2;
  Task_Add(ShrineOfTheSeaGod_KeepSecondPillarWithLeader, 0x480);
  ShrineOfTheSeaGod_LandPillarAsLogs(leader, 2);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x9999, 0x1999);
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, 406, 360);
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, 400, 392);
  Sound_Play(SE_RATTLE);
  Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Task_Remove(ShrineOfTheSeaGod_KeepSecondPillarWithLeader);
  wait(1);
  Event_End();
  GameFlag_Set(FLAG_SECOND_PILLAR_TOPPLED);
}

/// Reveal shows the seventh stepping stone in the hexagon of rocks.
void ShrineOfTheSeaGod_RevealStone(void) {
  ShrineOfTheSeaGod_CopyRect(42, 1, 15, 14, 1, 2);
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 15, 14);
}

void ShrineOfTheSeaGod_HideStone(void) {
  ShrineOfTheSeaGod_CopyRect(15, 9, 15, 14, 1, 2);
  ShrineOfTheSeaGod_BlendStart(14, 14, 1, 1, 15, 14);
}

void ShrineOfTheSeaGod_ThrowRopeAgain(void) {
  struct ShrineOfTheSeaGodObject *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgFieldRopeThrownAgain);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 12, 0);
  ShrineOfTheSeaGod_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjSpeed(14, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjWalk(gMainRoutineWork.currentOwner, 272, 336);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_SetHorizontalPositionWithTerrain(14, 272 << 16, 336 << 16);
  wait(1);
  ShrineOfTheSeaGod_ObjWalk(14, 256, 336);
  Actor_ArmCallback(14, 0, 0);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, FACING_WEST, 40);
  ShrineOfTheSeaGod_ObjTurn(14, 0xd000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  ShrineOfTheSeaGod_CamSpeed(0x4cccc, 0x9999);
  ShrineOfTheSeaGod_CamMove(288 << 16, -1, 126 << 16, 1);
  FieldCamera_WaitForArrival();
  Actor_Jump(13, 4, 20);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  Motion_SetModeAndWaitAnimation(12, 3);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(12, 2);
  Actor_Jump(12, 6, 80);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  Actor_SetModeById(12, 4);
  ShrineOfTheSeaGod_MessagePut(0x400c, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x3000, 0);
  ShrineOfTheSeaGod_Jump(13, 265, 20);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  ShrineOfTheSeaGod_MessagePut(0x400c, 0);
  Actor_ArmCallback(13, 0, 0);
  FieldEffect_SetAttached(13, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | 13, 0);
  Motion_SetModeAndWaitAnimation(12, 3);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 12, 0);
  Camera_SetSpeedParameters(0x33333, 0x6666);
  ShrineOfTheSeaGod_CamMove(420 << 16, -1, 200 << 16, 1);
  ShrineOfTheSeaGod_ObjSpeed(12, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(12, 346, 186);
  ShrineOfTheSeaGod_ObjWalk(12, 374, 192);
  ShrineOfTheSeaGod_ObjWalk(12, 392, 224);
  ShrineOfTheSeaGod_ObjWalk(12, 394, 246);
  ShrineOfTheSeaGod_ObjWalk(12, 420, 268);
  ShrineOfTheSeaGod_ObjWalk(12, 456, 268);
  ShrineOfTheSeaGod_ObjWalk(12, 488, 240);
  ShrineOfTheSeaGod_ObjWalk(12, 488, 226);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  FieldCamera_MoveWithinMapBounds(264 << 16, -1, 320 << 16, 1);
  FieldCamera_WaitForArrival();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_ArmCallback(14, 0, 20);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(14, 3);
  Actor_SetModeById(14, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != NULL)
    Actor_SetDestinationById(14, ((s16 *)&leader->x)[1],
                             ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(14);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  GameFlag_Set(FLAG_RIKI_GAVE_UP);
  Event_End();
}

extern u32 gShrineOfTheSeaGod_BoysGoHome[];

void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
void Actor_FaceDirectionById(s32 actor, s32 angle);

static inline void ShrineOfTheSeaGod_ObjAngle(s32 actor, s32 angle) {
  Actor_FaceDirectionById(actor, angle);
}
void Actor_SetMovementTypeAndWaitById(s32 actor, const void *callback);

// Lash ties the boys' rope, Tavi climbs down and both meet the party.
void ShrineOfTheSeaGod_LashRope(void) {
  struct ShrineOfTheSeaGodObject *object;
  struct ShrineOfTheSeaGodObject *leader;
  struct ShrineOfTheSeaGodSprite *sprite;
  u8 *motionFlags;
  const void *callback;

  if (ShrineOfTheSeaGod_FlagCheck(FLAG_TAVI_RESCUED))
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  ShrineOfTheSeaGod_CamMove(170 << 17, -1, 176 << 16, 1);
  ShrineOfTheSeaGod_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjSpeed(14, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjWalk(gMainRoutineWork.currentOwner, 344, 184);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, 0x2000, 0);
  ShrineOfTheSeaGod_ObjPlace(14, 344 << 16, 184 << 16);
  wait(1);
  ShrineOfTheSeaGod_ObjWalk(14, 356, 196);
  /* FAKEMATCH(wrapper): Actor_FaceDirectionById called through the inline
   * ShrineOfTheSeaGod_ObjAngle, here and below. */
  ShrineOfTheSeaGod_ObjAngle(14, 0xb000);
  Event_SetMessageId((s32)MsgFieldPraiseRopeWork);
  ShrineOfTheSeaGod_MessagePut(14, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  ShrineOfTheSeaGod_MessagePut(13, 0);
  ShrineOfTheSeaGod_ObjPlace(13, 184 << 16, 168 << 16);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, 0x8000, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0x8000, 0);
  ShrineOfTheSeaGod_CamSpeed(0x4cccc, 0x9999);
  ShrineOfTheSeaGod_CamMove(232 << 16, 64 << 16, 176 << 16, 1);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(13, 232, 168);
  Event_Wait(20);
  ShrineOfTheSeaGod_MessagePut(13, 0);
  ShrineOfTheSeaGod_CamSpeed(0x13333, 0x2666);
  ShrineOfTheSeaGod_CamMove(174 << 17, -1, 216 << 16, 1);
  object = Actor_GetById(13);
  sprite = object->sprite;
  Actor_SetSpritePriority(13, 1);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x19999, 0xcccc);
  Actor_Jump(13, 4, 0);
  ShrineOfTheSeaGod_ObjCommit(13, 248, 168);
  Actor_SetModeById(13, 6);
  /* FAKEMATCH(temporary): the first motion flags store written through a byte
   * pointer. */
  motionFlags = &object->motionFlags;
  /* FAKEMATCH(signature): scaleX written through a one-member union. */
  ((union ObjectWord *)&object->scaleX)->value = -0x10000;
  sprite->rotation = 0xd000;
  *motionFlags = 0;
  object->y += px(6);
  Actor_SetSpriteFlags(Actor_GetById(13), 0);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x6666, 0x3333);
  /* FAKEMATCH(wrapper): Actor_SetMoveTarget called through Call4. */
  Call4((void (*)())Actor_SetMoveTarget, (s32)object, 164 << 17, 152 << 14,
        168 << 16);
  do {
    Actor_WaitForMoveById(13);
  } while (0);
  object->motionFlags = 3;
  object->terrainHeight = 0x200000;
  ((union ObjectWord *)&object->scaleX)->value = 0x10000;
  sprite->rotation = 0;
  Actor_SetModeById(13, 1);
  Actor_SetSpriteFlags(Actor_GetById(13), 1);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x19999, 0xcccc);
  Actor_Jump(13, 4, 0);
  ShrineOfTheSeaGod_ObjCommit(13, 328, 184);
  ShrineOfTheSeaGod_ObjTurn(13, 0, 20);
  ShrineOfTheSeaGod_Jump(13, 256, 40);
  Actor_SetSpritePriority(13, 2);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_MessagePut(0x200e, 0);
  ShrineOfTheSeaGod_Jump(13, 257, 20);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0xb000, 40);
  ShrineOfTheSeaGod_ObjTurn(14, 0x8000, 0);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 14, 0, 60);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  Actor_SetModeById(13, 3);
  ShrineOfTheSeaGod_ObjPlace(12, 394 << 16, 286 << 16);
  wait(1);
  ShrineOfTheSeaGod_ObjSpeed(12, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(12, 394, 248);
  ShrineOfTheSeaGod_ObjTurn(12, 0xb000, 0);
  Actor_Jump(12, 4, 20);
  ShrineOfTheSeaGod_MessagePut(0x400c, 0);
  ShrineOfTheSeaGod_Jump(13, 256, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x3000, 0);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, 0x2000, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0x3000, 0);
  ShrineOfTheSeaGod_CamSpeed(0x4cccc, 0x9999);
  ShrineOfTheSeaGod_CamMove(188 << 17, -1, 264 << 16, 1);
  FieldCamera_WaitForArrival();
  ShrineOfTheSeaGod_MessagePut(12, 0);
  ShrineOfTheSeaGod_CamSpeed(0x26666, 0x4ccc);
  ShrineOfTheSeaGod_CamMove(174 << 17, -1, 216 << 16, 1);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 12);
  Actor_WatchAndSetScript(14, 12);
  Actor_WatchAndSetScript(13, 12);
  ShrineOfTheSeaGod_ObjWalk(12, 394, 220);
  ShrineOfTheSeaGod_ObjWalk(12, 370, 198);
  ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjCommit(14, 368, 180);
  Actor_WatchAndSetScript(14, 12);
  ShrineOfTheSeaGod_ObjWalk(12, 336, 198);
  ShrineOfTheSeaGod_ObjTurn(12, 0xb000, 0);
  Actor_Stop(13);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  Actor_ArmCallback(12, 0xd000, 0);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  ShrineOfTheSeaGod_ObjAngle(12, 0xb000);
  Actor_Jump(12, 4, 20);
  Event_ShowMessageThenWait(12, 0, 20);
  ShrineOfTheSeaGod_Jump(13, 261, 40);
  ShrineOfTheSeaGod_Jump(12, 257, 60);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  Actor_SetModeById(13, 4);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_Jump(12, 256, 20);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x8000, 20);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_ObjTurn(12, 0x8000, 0);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  Motion_SetModeAndWaitAnimation(13, 4);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_Jump(12, 265, 40);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  ShrineOfTheSeaGod_Jump(12, 256, 0);
  ShrineOfTheSeaGod_Jump(13, 256, 0);
  ShrineOfTheSeaGod_ObjTurn(12, 0xb000, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x3000, 40);
  Actor_Jump(13, 4, 40);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  Actor_SetModeById(12, 3);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  Motion_SetModeAndWaitAnimation(13, 3);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  Actor_SetModeById(12, 3);
  Motion_SetModeAndWaitAnimation(13, 3);
  ShrineOfTheSeaGod_ObjSpeed(12, 0x13333, 0x9999);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x13333, 0x9999);
  callback = gShrineOfTheSeaGod_BoysGoHome;
  Actor_SetMovementTypeById(12, callback);
  Event_Wait(10);
  Actor_SetMovementTypeAndWaitById(13, callback);
  Event_Wait(40);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(14);
  wait(1);
  Event_ShowMessageThenWait(14, 0, 40);
  ShrineOfTheSeaGod_ObjPlace(13, 0, 0);
  ShrineOfTheSeaGod_ObjPlace(12, 0, 0);
  ShrineOfTheSeaGod_MessagePut(14, 0);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, 0, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0x8000, 20);
  ShrineOfTheSeaGod_MessagePut(14, 0);
  Actor_SetModeById(14, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != NULL)
    Actor_SetDestinationById(14, leader->x >> 16, leader->z >> 16);
  Actor_WaitForMoveById(14);
  ShrineOfTheSeaGod_ObjPlace(14, 0, 0);
  GameFlag_Set(FLAG_TAVI_RESCUED);
  Event_End();
}

/// Breath, first seen in the great hall, startles and runs off.
void ShrineOfTheSeaGod_DjinniFleesEast(void) {
  u8 off = 0;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = off;
  FieldCamera_MoveWithinMapBounds(480 << 16, 32 << 16, 340 << 16, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(15, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpritePriority(15, 1);
  Actor_GetById(15)->motionFlags = off;
  ShrineOfTheSeaGod_ObjSpeed(15, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(15, 500, 344);
  ShrineOfTheSeaGod_ObjWalk(15, 580, 344);
  Actor_WatchAndSetScript(15, gMainRoutineWork.currentOwner);
  Event_Wait(20);
  GameFlag_Set(FLAG_DJINNI_FLED_EAST);
  Event_End();
}

void ShrineOfTheSeaGod_DjinniFleesSouth(void) {
  u8 off = 0;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(15);
  FieldCamera_GetActor()->motionFlags = off;
  FieldCamera_MoveWithinMapBounds(564 << 16, 32 << 16, 344 << 16, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(15, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpritePriority(15, 1);
  Actor_GetById(15)->motionFlags = off;
  ShrineOfTheSeaGod_ObjSpeed(15, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(15, 616, 376);
  ShrineOfTheSeaGod_ObjWalk(15, 616, 460);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Event_Wait(20);
  GameFlag_Set(FLAG_DJINNI_FLED_SOUTH);
  Event_End();
}

/// The Djinni runs up from the south stairs and waits by the ledge.
void ShrineOfTheSeaGod_DjinniRunsNorth(void) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(13);
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_ShowEmoteById(gMainRoutineWork.currentOwner, 0x100, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  ShrineOfTheSeaGod_CamMove(328 << 16, 32 << 16, 552 << 16, 1);
  FieldCamera_WaitForArrival();
  ShrineOfTheSeaGod_ObjPlaceTurn(14, 328 << 16, 624 << 16, 0xc000);
  wait(1);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 14);
  object->priorityFlags &= ~2;
  Actor_SetSpritePriority(14, 3);
  ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(14, 328, 568);
  ShrineOfTheSeaGod_ObjWalk(14, 328, 496);
  Actor_WalkToAndWaitById(14, 280, 432);
  Actor_Stop(gMainRoutineWork.currentOwner);
  wait(1);
  angle = 0x4000;
  ShrineOfTheSeaGod_ObjPlaceTurn(14, 280 << 16, 328 << 16, angle);
  Actor_SetSpritePriority(14, 2);
  Actor_GetById(14)->priorityFlags |= 1;
  object->priorityFlags |= 2;
  wait(1);
  GameFlag_Set(FLAG_DJINNI_RAN_PAST);
  GameFlag_Set(FLAG_DJINNI_BY_LEDGE);
  Event_End();
}

void ShrineOfTheSeaGod_DjinniRunsWest(void) {
  s32 done = ShrineOfTheSeaGod_FlagCheck(FLAG_DJINNI_RAN_PAST);

  if (done == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_ShowEmoteById(gMainRoutineWork.currentOwner,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldCamera_GetActor()->motionFlags = done;
    FieldCamera_MoveWithinMapBounds(328 << 16, 48 << 16, 552 << 16, 1);
    FieldCamera_WaitForArrival();
    done = 0x4000;
    ShrineOfTheSeaGod_ObjPlaceTurn(14, 424 << 16, 520 << 16, done);
    wait(1);
    Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 14);
    Actor_SetSpritePriority(14, 1);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
    ShrineOfTheSeaGod_ObjWalk(14, 380, 552);
    ShrineOfTheSeaGod_ObjWalk(14, 264, 552);
    Actor_WalkToAndWaitById(14, 264, 540);
    Actor_Stop(gMainRoutineWork.currentOwner);
    wait(1);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
    Actor_SetSpritePriority(14, 2);
    Actor_GetById(14)->priorityFlags |= 1;
    wait(1);
    GameFlag_Set(FLAG_DJINNI_RAN_PAST);
    Event_End();
  }
}

/// Breath hops up onto the ledge, where the chase round it begins.
void ShrineOfTheSeaGod_DjinniHopsOntoLedge(void) {
  u8 off;

  if (GameFlag_Get(FLAG_DJINNI_BY_LEDGE)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    off = 0;
    FieldCamera_GetActor()->motionFlags = off;
    FieldCamera_MoveWithinMapBounds(320 << 16, -1, 300 << 16, 1);
    ShrineOfTheSeaGod_Jump(14, 256, 40);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x13333, 0x9999);
    ShrineOfTheSeaGod_ObjWalk(14, 345, 296);
    ShrineOfTheSeaGod_ObjWalk(14, 392, 296);
    Actor_WatchAndSetScript(14, gMainRoutineWork.currentOwner);
    Actor_GetById(14)->state = off;
    wait(10);
    GameFlag_Set(FLAG_DJINNI_ON_LEDGE);
    Event_End();
  }
}

void ShrineOfTheSeaGod_StartDjinniDash(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  Actor_Stop(14);
  wait(1);
  ShrineOfTheSeaGod_ObjSpeed(14, 0x26666, 0x13333);
}

void ShrineOfTheSeaGod_EndDjinniDash(void) {
  Actor_WatchAndSetScript(14, gMainRoutineWork.currentOwner);
  Event_Wait(5);
  Event_End();
}

#define DEFINE_DJINNI_DASH(name, n1, n2, n3)                                   \
  void name(void) {                                                            \
    ShrineOfTheSeaGod_StartDjinniDash();                                       \
    FieldCamera_MoveWithinMapBounds(n1 << 16, 16 << 16, n2 << 16, 1);          \
    Actor_WalkToAndWaitById(14, n1, n2);                                       \
    Actor_GetById(14)->state = n3;                                             \
    ShrineOfTheSeaGod_EndDjinniDash();                                         \
  }

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_DjinniSouthEastToNorthEast, 392, 216, 1)

#define DEFINE_DJINNI_WALK_AND_JUMP(name, n1, n2, n3, n4, n5, n6)              \
  void name(void) {                                                            \
    struct ShrineOfTheSeaGodObject *block = Actor_GetById(14);                 \
    u8 *state;                                                                 \
    s32 flag;                                                                  \
                                                                               \
    ShrineOfTheSeaGod_StartDjinniDash();                                       \
    ShrineOfTheSeaGod_CamMove(n1 << 16, 16 << 16, 216 << 16, 1);               \
    ShrineOfTheSeaGod_ObjWalk(14, n1, 216);                                    \
    flag = GameFlag_Get(FLAG_TORCH_AT_NORTH_WALL);                             \
    state = &block->state;                                                     \
    if (flag) {                                                                \
      block->facing = n2;                                                      \
      ShrineOfTheSeaGod_ObjEmote(14, 258);                                     \
      Event_Wait(20);                                                          \
      *state = n3;                                                             \
      GameFlag_Set(FLAG_DJINNI_TRAPPED);                                       \
    } else {                                                                   \
      Event_Wait(5);                                                           \
      FieldCamera_MoveWithinMapBounds(n4 << 16, 16 << 16, 216 << 16, 1);       \
      Sound_Play(SE_HIGH_JUMP);                                                \
      Actor_SetSpriteFlags(Actor_GetById(14), 0);                              \
      ShrineOfTheSeaGod_ObjSpeed(14, 0x4cccc, 0x26666);                        \
      block->velocityY = 0x80000;                                              \
      Actor_MoveToAndWaitById(14, n5, 216);                                    \
      Actor_SetSpriteFlags(Actor_GetById(14), 1);                              \
      Event_Wait(10);                                                          \
      ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);                         \
      ShrineOfTheSeaGod_ObjWalk(14, n4, 216);                                  \
      *state = n6;                                                             \
    }                                                                          \
    ShrineOfTheSeaGod_EndDjinniDash();                                         \
  }

DEFINE_DJINNI_WALK_AND_JUMP(ShrineOfTheSeaGod_DjinniNorthEastJumpsWest, 360,
                            0x8000, 4, 296, 328, 2)

#define DEFINE_DJINNI_DASH_AROUND(name, n1, n2, n3, n4)                        \
  void name(void) {                                                            \
    ShrineOfTheSeaGod_StartDjinniDash();                                       \
    FieldCamera_MoveWithinMapBounds(n1 << 16, 16 << 16, n2 << 16, 1);          \
    ShrineOfTheSeaGod_ObjWalk(14, n3, 248);                                    \
    ShrineOfTheSeaGod_ObjWalk(14, n1, 248);                                    \
    Actor_WalkToAndWaitById(14, n1, n2);                                       \
    Actor_GetById(14)->state = n4;                                             \
    ShrineOfTheSeaGod_EndDjinniDash();                                         \
  }

DEFINE_DJINNI_DASH_AROUND(ShrineOfTheSeaGod_DjinniNorthWestToSouthWest, 280,
                          296, 296, 3)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_DjinniSouthWestToSouthEast, 392, 296, 0)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_DjinniSouthEastToSouthWest, 280, 296, 3)

DEFINE_DJINNI_DASH_AROUND(ShrineOfTheSeaGod_DjinniSouthWestToNorthWest, 296,
                          216, 280, 2)

DEFINE_DJINNI_WALK_AND_JUMP(ShrineOfTheSeaGod_DjinniNorthWestJumpsEast, 328, 0,
                            5, 392, 360, 1)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_DjinniNorthEastToSouthEast, 392, 296, 0)

#define DEFINE_DJINNI_JUMP(name, n1, n2, n3, n4, n5)                           \
  void name(void) {                                                            \
    struct ShrineOfTheSeaGodObject *block = Actor_GetById(14);                 \
    u8 *state;                                                                 \
    s32 flag;                                                                  \
                                                                               \
    ShrineOfTheSeaGod_StartDjinniDash();                                       \
    flag = GameFlag_Get(FLAG_TORCH_AT_NORTH_WALL);                             \
    state = &block->state;                                                     \
    if (flag) {                                                                \
      block->facing = n1;                                                      \
      ShrineOfTheSeaGod_ObjEmote(14, 258);                                     \
      Event_Wait(20);                                                          \
      *state = n2;                                                             \
      GameFlag_Set(FLAG_DJINNI_TRAPPED);                                       \
    } else {                                                                   \
      FieldCamera_MoveWithinMapBounds(n3 << 16, 16 << 16, 216 << 16, 1);       \
      Sound_Play(SE_HIGH_JUMP);                                                \
      Actor_SetSpriteFlags(Actor_GetById(14), 0);                              \
      ShrineOfTheSeaGod_ObjSpeed(14, 0x4cccc, 0x26666);                        \
      block->velocityY = 0x80000;                                              \
      Actor_MoveToAndWaitById(14, n4, 216);                                    \
      Actor_SetSpriteFlags(Actor_GetById(14), 1);                              \
      Event_Wait(10);                                                          \
      ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);                         \
      ShrineOfTheSeaGod_ObjWalk(14, n3, 216);                                  \
      *state = n5;                                                             \
    }                                                                          \
    ShrineOfTheSeaGod_EndDjinniDash();                                         \
  }

DEFINE_DJINNI_JUMP(ShrineOfTheSeaGod_TrappedDjinniJumpsWest, 0x8000, 4, 296,
                   328, 2)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_TrappedDjinniToNorthEast, 392, 216, 1)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_TrappedDjinniToNorthWest, 296, 216, 2)

DEFINE_DJINNI_JUMP(ShrineOfTheSeaGod_TrappedDjinniJumpsEast, 0, 5, 392, 360, 1)

/// Each step onto the ledge sends Breath on round it, a jump at a time, unless
/// the torch at the north wall stops the jump and traps it.
void ShrineOfTheSeaGod_ChaseDjinni(void) {
  struct FieldWork *work;
  struct ShrineOfTheSeaGodObject *object;

  if (!GameFlag_Get(FLAG_DJINNI_ON_LEDGE))
    return;
  if (GameFlag_Get(FLAG_DJINNI(DJINNI_BREATH)))
    return;
  work = Ram_HeapSlots->fieldWork;
  object = Actor_GetById(14);
  switch (work->stepTrigger) {
  case 13:
    if (object->state == 0)
      ShrineOfTheSeaGod_DjinniSouthEastToNorthEast();
    GameFlag_Clear(FLAG_DJINNI_TRAPPED);
    break;
  case 14:
    if (object->state == 0)
      ShrineOfTheSeaGod_DjinniSouthEastToSouthWest();
    break;
  case 15: {
    u8 current = object->state;

    if (current == 1)
      ShrineOfTheSeaGod_DjinniNorthEastJumpsWest();
    else if (current == 4 && !GameFlag_Get(FLAG_DJINNI_TRAPPED))
      ShrineOfTheSeaGod_TrappedDjinniJumpsWest();
  } break;
  case 16: {
    u8 current = object->state;

    if (current == 1)
      ShrineOfTheSeaGod_DjinniNorthEastToSouthEast();
    else if (current == 5)
      ShrineOfTheSeaGod_TrappedDjinniToNorthWest();
  } break;
  case 17: {
    u8 current = object->state;

    if (current == 2)
      ShrineOfTheSeaGod_DjinniNorthWestToSouthWest();
    else if (current == 4)
      ShrineOfTheSeaGod_TrappedDjinniToNorthEast();
  } break;
  case 18: {
    u8 current = object->state;

    if (current == 2)
      ShrineOfTheSeaGod_DjinniNorthWestJumpsEast();
    else if (current == 5 && !GameFlag_Get(FLAG_DJINNI_TRAPPED))
      ShrineOfTheSeaGod_TrappedDjinniJumpsEast();
  } break;
  case 19:
    if (object->state == 3)
      ShrineOfTheSeaGod_DjinniSouthWestToSouthEast();
    break;
  case 20:
    if (object->state == 3)
      ShrineOfTheSeaGod_DjinniSouthWestToNorthWest();
    GameFlag_Clear(FLAG_DJINNI_TRAPPED);
    break;
  }
}

extern s16 gShrineOfTheSeaGod_Footprints[];
extern s16 gShrineOfTheSeaGod_MovableActors[];
extern u32 gShrineOfTheSeaGod_RikiLooksAround[];

void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void Actor_PlaceBlocks(const void *script);
void ShrineOfTheSeaGod_LayerPuddles(void);

void ShrineOfTheSeaGod_LightTorch(void) {
  Actor_SetModeById(8, 2);
  Actor_SetModeById(9, 0);
  Actor_DisableById(9);
}

void ShrineOfTheSeaGod_CastSand(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 2;
}

void ShrineOfTheSeaGod_EndSand(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 0;
}

s32 Scene_Initialize(void) {
  s16 scene;

  gFieldWork->transition = 0x204;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod1)
    ShrineOfTheSeaGod_InitializeRoom1();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod2)
    ShrineOfTheSeaGod_InitializeRoom2();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod3)
    ShrineOfTheSeaGod_InitializeRoom3();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod4)
    ShrineOfTheSeaGod_InitializeRoom4();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod5)
    ShrineOfTheSeaGod_InitializeRoom5();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod6)
    ShrineOfTheSeaGod_InitializeRoom6();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod7)
    ShrineOfTheSeaGod_InitializeRoom7();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod8)
    ShrineOfTheSeaGod_InitializeRoom8();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod9)
    ShrineOfTheSeaGod_InitializeRoom9();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod11)
    ShrineOfTheSeaGod_InitializeRoom11();
  return 0;
}

struct ShrineOfTheSeaGodObject *ShrineOfTheSeaGod_SetUpUnderfoot(s32 id) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(id);

  object->priorityFlags |= 2;
  object->y = px(12);
  Actor_DisableById(id);
  return object;
}

#include "shared/actor_hide.h"

void Scene_RunUnlessFlag(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (GameFlag_Get(FLAG_SLIDES_SHOWN) == 0)
      ShrineOfTheSeaGod_HideSlideActors();
  } else {
    GameFlag_Set(FLAG_SLIDES_SHOWN);
  }
}

void Actors_ForRange(void) {
  u32 i;

  for (i = 0; i <= 2; i++)
    Actor_Hide(i + 9);
  Scene_RunUnlessFlag();
}

/// Lays the Djinni's footprints across the room below the great hall.
void ShrineOfTheSeaGod_PlaceFootprints(void) {
  const struct ShrineOfTheSeaGodSpot *spot = gShrineOfTheSeaGod_Footprints;
  u32 i;

  for (i = 0; i <= 15; i++) {
    s32 id = i + 8;
    struct ShrineOfTheSeaGodObject *object = Actor_GetById(id);
    struct ShrineOfTheSeaGodSprite *sprite;

    Actor_DisableById(id);
    Actor_SetModeById(id, 0);
    Actor_SetSpritePriority(id, 3);
    object->priorityFlags |= 2;
    Actor_SetSpriteColor(Actor_GetById(id), 12);
    sprite = object->sprite;
    object->x = spot->x;
    object->z = spot->z;
    object->scaleX = spot->scaleX;
    sprite->rotation = spot->rotation;
    spot++;
  }
}

void ShrineOfTheSeaGod_InitializeRoom1(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;

  ShrineOfTheSeaGod_SetUpFlatProp(8);
  ShrineOfTheSeaGod_SetUpFlatProp(9);
  {
    s32 priority;

    map->mode17 = 1;
    priority = 0x480;
    Task_Add(ShrineOfTheSeaGod_TrackUpperLevel, priority);
  }
  Rope_Initialize();
  Rope_Place(0, FLAG_ROPE_TIED, 10, 11);
  if (GameFlag_Get(FLAG_RIKI_GAVE_UP)) {
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  }
}

void ShrineOfTheSeaGod_TrackRikiSide(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(11);

  if (object->x > leader->x)
    object->collisionFlags |= 8;
  else
    object->collisionFlags &= ~8;
}

void ShrineOfTheSeaGod_InitializeRoom2(void) {

  ShrineOfTheSeaGod_SetUpUnderfoot(10)->y = -px(16);
  Actor_SetSpritePriority(10, 3);
  Actor_PlaceBlocks(gShrineOfTheSeaGod_MovableActors);
  ShrineOfTheSeaGod_LightTorch();
  ShrineOfTheSeaGod_TaskAdd(ShrineOfTheSeaGod_KeepFlameOnTorch, 0x480);
  Actor_DisableById(11);
  Actor_DisableById(12);
  Actor_GetById(12)->motionFlags = 0;
  ShrineOfTheSeaGod_BlendStart(3, 15, 1, 1, 6, 15);
  if (GameFlag_Get(FLAG_TAVI_RESCUED)) {
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  } else if (GameFlag_Get(FLAG_RIKI_GAVE_UP)) {
    ShrineOfTheSeaGod_ObjPlaceTurn(11, 328 << 16, 200 << 16, 0x8000);
    wait(1);
    Actor_SetMovementTypeById(11, gShrineOfTheSeaGod_RikiLooksAround);
    Actor_SetSpriteFlags(Actor_GetById(11), 1);
    Actor_GetById(11)->collisionFlags = 1;
  }
  /* FAKEMATCH(temporary): SHRINE_OF_THE_SEA_GOD_TASK_ADD_LATE's local holding
   * the priority builds it before the task function loads. */
  SHRINE_OF_THE_SEA_GOD_TASK_ADD_LATE(ShrineOfTheSeaGod_TrackRikiSide, 0x480);
}

void ShrineOfTheSeaGod_InitializeRoom3(void) {
  struct ShrineOfTheSeaGodObject *block;

  ShrineOfTheSeaGod_SetUpUnderfoot(9);
  ShrineOfTheSeaGod_SetUpUnderfoot(10);
  ShrineOfTheSeaGod_SetUpUnderfoot(11);
  ShrineOfTheSeaGod_SetUpUnderfoot(12);
  ShrineOfTheSeaGod_SetUpUnderfoot(13);
  block = ShrineOfTheSeaGod_SetUpUnderfoot(14);
  Actor_SetModeById(9, 0);
  Actor_SetModeById(13, 0);
  Actor_SetModeById(14, 2);
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpritePriority(10, 1);
  Actor_SetSpritePriority(11, 1);
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpritePriority(13, 1);
  Actor_SetSpritePriority(14, 1);
  ShrineOfTheSeaGod_TaskAdd(ShrineOfTheSeaGod_LayerRoom3Props, 0x480);
  if (GameFlag_Get(FLAG_ROOM3_PILLAR_TOPPLED)) {
    ShrineOfTheSeaGod_ObjSetPos(block, 408 << 16, -(13 << 16), 336 << 16);
    ((struct ShrineOfTheSeaGodSprite *)block->sprite)->rotation = 0x1000;
    Actor_SetSpritePriority(14, 3);
    block->priorityFlags |= 2;
    ShrineOfTheSeaGod_BlendStart(0, 50, 3, 1, 24, 70);
  }
  if (GameFlag_Get(FLAG_ROOM3_PILLAR_MOVED))
    ShrineOfTheSeaGod_ObjSetPos(Actor_GetById(8), 200 << 16, 0, 296 << 16);
  Actor_PlaceBlocks(gShrineOfTheSeaGod_MovableActors);
  if (GameFlag_Get(FLAG_DJINNI_FLED_SOUTH))
    Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  else if (GameFlag_Get(FLAG_DJINNI_FLED_EAST)) {
    ShrineOfTheSeaGod_ObjPlaceTurn(15, 580 << 16, 344 << 16, 0x4000);
  }
  FieldEffect_StartBlankOverlay(0);
  wait(1);
}

void ShrineOfTheSeaGod_InitializeRoom4(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;

  if (GameFlag_Get(FLAG_DJINNI_RAN_PAST) == 0)
    ShrineOfTheSeaGod_PlaceFootprints();
  {
    s32 priority;

    map->mode17 = 1;
    priority = 0x480;
    Task_Add(ShrineOfTheSeaGod_TrackUpperLevel, priority);
  }
}

void ShrineOfTheSeaGod_InitializeRoom5(void) {
  struct ShrineOfTheSeaGodObject *object = ShrineOfTheSeaGod_SetUpUnderfoot(13);
  u8 *flags = &object->priorityFlags;

  if ((u16)(gMainRoutineWork.entrance - 2) <= 1) {
    Actor_SetSpritePriority(13, 3);
    /* FAKEMATCH(dead): the cancelling update ties the flag or to the constant's
     * register. */
    *flags = *flags + 2 - (*flags & 2);
  } else {
    Actor_SetSpritePriority(13, 1);
    *flags &= ~2;
  }
  ShrineOfTheSeaGod_SetUpPuddle(10);
  ShrineOfTheSeaGod_SetUpPuddle(11);
  ShrineOfTheSeaGod_SetUpPuddle(12);
  if (GameFlag_Get(FLAG_FIRST_PUDDLE_FROZEN))
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 8, 18);
  if (GameFlag_Get(FLAG_SECOND_PUDDLE_FROZEN))
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 18);
  if (GameFlag_Get(FLAG_THIRD_PUDDLE_FROZEN))
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 17);
  ShrineOfTheSeaGod_TaskAdd(ShrineOfTheSeaGod_LayerPuddles, 0x480);
  Actor_PlaceBlocks(gShrineOfTheSeaGod_MovableActors);
  ShrineOfTheSeaGod_LightTorch();
  ShrineOfTheSeaGod_TaskAdd(ShrineOfTheSeaGod_KeepFlameOnTorch, 0x480);
  if (GameFlag_Get(FLAG_DJINNI(DJINNI_BREATH)) ||
      GameFlag_Get(FLAG_FIGHTING_BREATH)) {
    GameFlag_Set(FLAG_DJINNI_CHASE_OVER);
  } else if (GameFlag_Get(FLAG_DJINNI_ON_LEDGE)) {
    struct ShrineOfTheSeaGodObject *block = Actor_GetById(14);
    s32 moved = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);

    if (moved == 0) {
      block->state = moved;
      ShrineOfTheSeaGod_ObjPlace(14, 392 << 16, 296 << 16);
    }
    Actor_WatchAndSetScript(14, gMainRoutineWork.currentOwner);
  } else if (GameFlag_Get(FLAG_DJINNI_BY_LEDGE)) {
    ShrineOfTheSeaGod_ObjPlaceTurn(14, 280 << 16, 328 << 16, 0x4000);
  }
}

void ShrineOfTheSeaGod_InitializeRoom6(void) {
  s32 open;

  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  Actor_SetSpriteFlags(Actor_GetById(8), 0);
  open = GameFlag_Get(FLAG_PILLAR_IN_HOLE);
  if (open) {
    ShrineOfTheSeaGod_ObjPlace(8, 248 << 16, 264 << 16);
    Actor_SetSpritePriority(8, 3);
    Actor_GetById(8)->motionFlags = 0;
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 15, 16);
    Actor_DisableById(8);
  } else {
    Actor_SetSpritePriority(8, 1);
    Actor_GetById(8)->motionFlags = open;
  }
  Actors_ForRange();
}

/// Lays out the two pillars, fallen or standing, and the room's two ropes.
void ShrineOfTheSeaGod_InitializeRoom7(void) {
  struct ShrineOfTheSeaGodObject *object;
  struct ShrineOfTheSeaGodObject *lastObject;

  ((struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork)->mode17 = 1;
  ShrineOfTheSeaGod_SetUpUnderfoot(8);
  ShrineOfTheSeaGod_SetUpUnderfoot(9);
  ShrineOfTheSeaGod_SetUpUnderfoot(10);
  ShrineOfTheSeaGod_SetUpUnderfoot(11);
  ShrineOfTheSeaGod_SetUpUnderfoot(12);
  ShrineOfTheSeaGod_SetUpUnderfoot(13);
  ShrineOfTheSeaGod_SetUpUnderfoot(14);
  ShrineOfTheSeaGod_SetUpUnderfoot(15);
  ShrineOfTheSeaGod_SetUpUnderfoot(16);
  Actor_SetModeById(13, 0);
  Actor_SetModeById(14, 0);
  Actor_SetModeById(15, 2);
  Actor_SetModeById(16, 2);
  if (GameFlag_Get(FLAG_FIRST_PILLAR_TOPPLED)) {
    Map_CopyCellAttributeRect(0, 0, 3, 1, 19, 13);
    Map_CopyCellAttributeRect(40, 0, 2, 1, 19, 33);
    Actor_SetPositionAndResetMotion(Actor_GetById(15), px(320), -px(21),
                                    px(534));
    Actor_SetSpritePriority(15, 3);
  }
  if (GameFlag_Get(FLAG_SECOND_PILLAR_TOPPLED)) {
    Map_CopyCellAttributeRect(0, 0, 3, 1, 24, 22);
    Map_CopyCellAttributeRect(40, 0, 2, 1, 24, 24);
    Actor_SetPositionAndResetMotion(Actor_GetById(16), px(400), -px(21),
                                    px(390));
    Actor_SetSpritePriority(16, 3);
  }
  Rope_Initialize();
  Call4((void (*)())Rope_Place, 0, FLAG_ROPE_TIED, 17, 18);
  Rope_Place(1, FLAG_SECOND_ROPE_TIED, 19, 20);
  Actor_DisableById(21);
  Actor_GetById(21)->motionFlags = 0;
  object = Actor_GetById(22);
  Actor_DisableById(22);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  Actor_DisableById(23);
  Actor_GetById(23)->motionFlags = 0;
  /* FAKEMATCH(temporary): actor 24 fetched into its own variable. */
  lastObject = Actor_GetById(24);
  object = lastObject;
  Actor_DisableById(24);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
}

void ShrineOfTheSeaGod_InitializeRoom8(void) {
  ShrineOfTheSeaGod_SetUpFlatProp(8);
  ShrineOfTheSeaGod_SetUpFlatProp(9);
  ShrineOfTheSeaGod_SetUpFlatProp(10);
  if (gMainRoutineWork.entrance == 3 &&
      GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)
    ShrineOfTheSeaGod_LowerWater();
}

void ShrineOfTheSeaGod_InitializeRoom9(void) {
  if (gMainRoutineWork.entrance == 1 &&
      GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)
    ShrineOfTheSeaGod_RiseOnWater();
}

/// The Right Prong floats above the last room, shedding sparkles.
void ShrineOfTheSeaGod_SparkleRightProng(
    struct ShrineOfTheSeaGodObject *object) {
  struct ShrineOfTheSeaGodEffectParams params;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    {
      s32 mask = 15;

      dx = mask & Random16();
    }
    dy = Random16() & 7;
    params.kind = 673;
    FieldEffect_Spawn(object->x + ((dx - 8) << 16),
                      object->y + ((dy - 8) << 16), object->z, 0, 0x8000, phase,
                      0x100000, &params);
  }
}

void ShrineOfTheSeaGod_InitializeRoom11(void) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(64);
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer = &map->layers[2];

  layer->x += px(128);
  Map_CenterOnFollowedPoint();
  wait(1);
  if (object != NULL) {
    object->motionFlags = 0;
    object->y = px(66);
    object->update = ShrineOfTheSeaGod_SparkleRightProng;
  }
}

void ShrineOfTheSeaGod_SetUpFlatProp(s32 id) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(id);

  Actor_SetSpritePriority(id, 3);
  Actor_DisableById(id);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  object->scaleX = 0xcccc;
  object->scaleY = 0xe666;
}

s32 Scene_PrepareMap(void) {

  if (GameFlag_Get(FLAG_FIGHTING_BREATH)) {
    struct ShrineOfTheSeaGodPartyExtra *party =
        (struct ShrineOfTheSeaGodPartyExtra *)&gMainRoutineWork;

    if (party->value == 1) {
      if (GameFlag_Get(FLAG_BREATH_TRAPPED_EAST))
        ShrineOfTheSeaGod_ObjPlaceAt(14, 360 << 16, 216 << 16, 0);
      else if (GameFlag_Get(FLAG_BREATH_TRAPPED_WEST)) {
        ShrineOfTheSeaGod_ObjPlaceTurn(14, 328 << 16, 216 << 16, 0x8000);
      }
    }
    GameFlag_Clear(FLAG_FIGHTING_BREATH);
    GameFlag_Clear(FLAG_BREATH_TRAPPED_EAST);
    GameFlag_Clear(FLAG_BREATH_TRAPPED_WEST);
  }
  return 0;
}

#undef DEFINE_DJINNI_DASH
#undef DEFINE_DJINNI_WALK_AND_JUMP
#undef DEFINE_DJINNI_DASH_AROUND
#undef DEFINE_DJINNI_JUMP
#include "text/msg_ids.h"

void ShrineOfTheSeaGod_FightBreath(void);
void ShrineOfTheSeaGod_ExamineStatue(void);
void ShrineOfTheSeaGod_PushPillarIntoHole(void);
void ShrineOfTheSeaGod_ToppleSecondPillar(void);
void ShrineOfTheSeaGod_RevealStone(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_MediumScript[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_LongScript[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

u32 gShrineOfTheSeaGod_RikiLooksAround[] = {
    22, 3, 0xb000, 0, 80, 22,
    3, 0x5000, 0, 80, 22, 3,
    0x8000, 0, 120, 13, 0xffff, 0,
};

u32 gShrineOfTheSeaGod_BoysGoHome[] = {
    39, 2, 4, 0x1580000, 0, 0xc60000,
    1, 4, 0x17a0000, 0, 0xc60000, 1,
    4, 0x1880000, 0, 0xdc0000, 1, 4,
    0x1880000, 0, 0x11c0000, 1, 39, 1,
    17,
};

s16 gShrineOfTheSeaGod_MovableActors[] = {
    8, -1,
};

s16 gShrineOfTheSeaGod_Footprints[] = {
    0, 316, 0, 136, 0, -1, 0x8000, 0,
    0, 318, 0, 142, 0, 1, 0x7000, 0,
    0, 324, 0, 154, 0, -1, 0x6000, 0,
    0, 330, 0, 158, 0, 1, 0x5000, 0,
    0, 338, 0, 166, 0, -1, 0x4800, 0,
    0, 344, 0, 166, 0, 1, 0x4800, 0,
    0, 288, 0, 240, 0, 1, 0xd800, 0,
    0, 284, 0, 236, 0, -1, 0xd800, 0,
    0, 272, 0, 232, 0, 1, 0xc800, 0,
    0, 266, 0, 230, 0, -1, 0xc800, 0,
    0, 256, 0, 238, 0, 1, 0xb000, 0,
    0, 248, 0, 238, 0, -1, 0xb800, 0,
    0, 240, 0, 250, 0, 1, 0xa800, 0,
    0, 234, 0, 252, 0, -1, 0xb000, 0,
    0, 224, 0, 262, 0, 1, 0xa800, 0,
    0, 220, 0, 262, 0, -1, 0xa800, 0,
    -1, 0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gShrineOfTheSeaGod_Room9Regions[] = {
    0x200100, 0x11000c0, 0xd00030, 0x2ffff, 0xffff, 0,
    0, 0,
};

s16 gShrineOfTheSeaGod_Room10Regions[] = {
    720, -38, 238, 736, -22, 254, -1, 1,
    864, 32, 208, 880, 48, 224, -1, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gShrineOfTheSeaGod_Room11Regions[] = {
    0xffda0310, 0x32000a0, 0xb0ffea, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(ShrineOfTheSeaGod1),
    SCENE_EXIT(1, ShrineOfTheSeaGod3, 5),
    SCENE_EXIT(2, ShrineOfTheSeaGod2, 1),
    SCENE_EXIT(3, WorldMap, 5),
    SCENE_EXITS(ShrineOfTheSeaGod2),
    SCENE_EXIT(1, ShrineOfTheSeaGod1, 2),
    SCENE_EXITS(ShrineOfTheSeaGod3),
    SCENE_EXIT(1, ShrineOfTheSeaGod7, 2),
    SCENE_EXIT(2, ShrineOfTheSeaGod8, 1),
    SCENE_EXIT(3, ShrineOfTheSeaGod8, 2),
    SCENE_EXIT(4, ShrineOfTheSeaGod4, 1),
    SCENE_EXIT(5, ShrineOfTheSeaGod1, 1),
    SCENE_EXITS(ShrineOfTheSeaGod4),
    SCENE_EXIT(1, ShrineOfTheSeaGod3, 4),
    SCENE_EXIT(2, ShrineOfTheSeaGod5, 2),
    SCENE_EXIT(3, ShrineOfTheSeaGod5, 4),
    SCENE_EXIT(4, ShrineOfTheSeaGod5, 3),
    SCENE_EXITS(ShrineOfTheSeaGod5),
    SCENE_EXIT(1, ShrineOfTheSeaGod6, 1),
    SCENE_EXIT(2, ShrineOfTheSeaGod4, 2),
    SCENE_EXIT(3, ShrineOfTheSeaGod4, 4),
    SCENE_EXIT(4, ShrineOfTheSeaGod4, 3),
    SCENE_EXITS(ShrineOfTheSeaGod6),
    SCENE_EXIT(1, ShrineOfTheSeaGod5, 1),
    SCENE_EXIT(2, ShrineOfTheSeaGod7, 1),
    SCENE_EXITS(ShrineOfTheSeaGod7),
    SCENE_EXIT(1, ShrineOfTheSeaGod6, 2),
    SCENE_EXIT(2, ShrineOfTheSeaGod3, 1),
    SCENE_EXITS(ShrineOfTheSeaGod8),
    SCENE_EXIT(1, ShrineOfTheSeaGod3, 2),
    SCENE_EXIT(2, ShrineOfTheSeaGod3, 3),
    SCENE_EXIT(3, ShrineOfTheSeaGod9, 1),
    SCENE_EXITS(ShrineOfTheSeaGod9),
    SCENE_EXIT(1, ShrineOfTheSeaGod8, 3),
    SCENE_EXIT(2, ShrineOfTheSeaGod10, 40),
    SCENE_EXITS(ShrineOfTheSeaGod10),
    SCENE_EXIT(1, ShrineOfTheSeaGod9, 2),
    SCENE_EXIT(2, ShrineOfTheSeaGod11, 40),
    SCENE_EXITS(ShrineOfTheSeaGod11),
    SCENE_EXIT(1, ShrineOfTheSeaGod10, 41),
    SCENE_EXITS_END,
};

struct ActorPlacement gShrineOfTheSeaGod_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGod_Room1Placements[] = {
    ACTOR_PLACEMENT(297, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 224, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(297, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(18, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 340, 0, 168, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(17, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 168, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGod_Room2Placements[] = {
    ACTOR_PLACEMENT(335, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(18, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGod_Room3Placements[] = {
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(370, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(246, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGod_Room4Placements[] = {
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGod_Room5Placements[] = {
    ACTOR_PLACEMENT(335, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(246, FLAG_DJINNI(DJINNI_BREATH), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGod_Room6Placements[] = {
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGod_Room7Placements[] = {
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(370, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(276, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(276, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(276, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(276, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 448, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 464, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 456, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGod_Room8Placements[] = {                                             
      {297, CONDITION_ALWAYS, 1, px(160), 0, px(202), FACING_SOUTH, 2,    
       0},                                                                     
      {297, CONDITION_ALWAYS, 1, px(176), 0, px(298), FACING_SOUTH, 2,    
       1},                                                                    
      {297, CONDITION_ALWAYS, 1, px(320), 0, px(218), FACING_SOUTH, 2,    
       1},                                                                    
      {SCENE_TABLE_END},                                                       
  };

struct Trigger gShrineOfTheSeaGod_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGod_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 10, FLAG_ROPE_TIED, EVENT(ShrineOfTheSeaGod_LashRope)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_EndSand)},
    {TRIGGER_STEP, 10, FLAG_RIKI_GAVE_UP, EVENT(ShrineOfTheSeaGod_ThrowRopeAgain)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGod_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TALK_TRIGGER_FACING(FACING_WEST), 11, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_StartleCompanion)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgFieldICantFindAnythingAroundHere)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGod_Room3Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, FLAG_ROOM3_PILLAR_MOVED, EVENT(ShrineOfTheSeaGod_LiftBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_ROOM3_PILLAR_MOVED, EVENT(ShrineOfTheSeaGod_MovePillar)},
    {TRIGGER_STEP, 10, FLAG_LEADER_IN_FRONT, EVENT(ShrineOfTheSeaGod_DrawLeaderInFront)},
    {TRIGGER_STEP, 11, IF_FLAG_SET(FLAG_LEADER_IN_FRONT), EVENT(ShrineOfTheSeaGod_DrawLeaderBehind)},
    {TRIGGER_STEP, 15, FLAG_ROOM3_PILLAR_TOPPLED, EVENT(ShrineOfTheSeaGod_TopplePillarIntoWater)},
    {TRIGGER_STEP, 11, FLAG_DJINNI_FLED_EAST, EVENT(ShrineOfTheSeaGod_DjinniFleesEast)},
    {TRIGGER_STEP, 12, FLAG_DJINNI_FLED_SOUTH, EVENT(ShrineOfTheSeaGod_DjinniFleesSouth)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGod_Room4Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gShrineOfTheSeaGod_Room5Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, FLAG_FIRST_PUDDLE_FROZEN,
     EVENT(ShrineOfTheSeaGod_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, FLAG_SECOND_PUDDLE_FROZEN,
     EVENT(ShrineOfTheSeaGod_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, FLAG_THIRD_PUDDLE_FROZEN,
     EVENT(ShrineOfTheSeaGod_FreezeThirdPuddle)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_LiftBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_SetDownTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_LiftBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_SetDownTorch)},
    {TRIGGER_STEP, 10, FLAG_DJINNI_BY_LEDGE,
     EVENT(ShrineOfTheSeaGod_DjinniRunsNorth)},
    {TRIGGER_STEP, 11, FLAG_DJINNI_BY_LEDGE,
     EVENT(ShrineOfTheSeaGod_DjinniRunsWest)},
    {TRIGGER_STEP, 12, FLAG_DJINNI_ON_LEDGE,
     EVENT(ShrineOfTheSeaGod_DjinniHopsOntoLedge)},
    {TRIGGER_STEP, 13, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 14, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 15, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 16, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 17, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 18, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 19, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 20, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_FightBreath)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGod_Room6Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_PUSHED, 0, FLAG_PILLAR_IN_HOLE,
     EVENT(ShrineOfTheSeaGod_PushPillarIntoHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_PILLAR_IN_HOLE,
     EVENT(ShrineOfTheSeaGod_PushPillarIntoHole)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGod_Room7Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 17, FLAG_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 19, FLAG_SECOND_ROPE_TIED, 0},
    {TRIGGER_STEP, 5, FLAG_FIRST_PILLAR_TOPPLED,
     EVENT(ShrineOfTheSeaGod_ToppleFirstPillar)},
    {TRIGGER_STEP, 6, FLAG_SECOND_PILLAR_TOPPLED,
     EVENT(ShrineOfTheSeaGod_ToppleSecondPillar)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_OpenFieldMenu)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_OpenFieldMenu)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_OpenFieldMenu)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGod_Room8Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXAMINE, 10, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_ExamineStatue)},
    {USE_ITEM(ITEM_SEA_GODS_TEAR), 10, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_UseSeaGodsTear)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_RevealStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_HideStone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGod_Room9Triggers[] = {
    {TRIGGER_EXAMINE, 1, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_ExamineStatue)},
    {USE_ITEM(ITEM_SEA_GODS_TEAR), 1, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_SinkWithWater)},
    {TRIGGER_TABLE_END},
};
