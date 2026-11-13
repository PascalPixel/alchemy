/// @file
/// The Alhafran Cave under the mayor's manor, in two scenes that share this
/// overlay: the cave behind the rear entrance (SceneId_AlhafranCave), which the
/// hungry boy's father lets the party into, and the town's jail
/// (SceneId_AlhafraJail). Once Briggs and his crew are jailed, his wife Chaucha
/// and their son Eoleo visit him, and the boy shows Psynergy of his own
/// (AlhafranCave_ChauchaAndEoleoVisit). Pound sinks a pillar
/// (AlhafranCave_PoundPillar), Frost freezes a puddle
/// (AlhafranCave_FreezePuddle), and the way down to the jail opens as the
/// leader stands on the ledge above it (AlhafranCave_OpenWayToJail).

#include "flags.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/scene/scene.h"
#include "text/msg_ids.h"

void Pillars_Place(const u16 *table);
void AlhafranCave_TakeLeadersLayer(void);
void Field_StartColorTransition(s32 frames, s32 color);
extern s32 gAlhafranCave_GuardStepsAsideScript[];
extern u32 gAlhafranCave_VisitorsLeaveScript[];
void AlhafranCave_OpenWayToJail(void);
void AlhafranCave_CloseWayToJail(void);
void AlhafranCave_CastSand(void);
void AlhafranCave_StartMovingCrate(void);
void AlhafranCave_FinishMovingCrate(void);
void FlagBlock_PlaceAllMarkingEdges(const void *data);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
extern s32 gAlhafranCave_ClearedWord;

// Set while the way down to the jail is open, as the leader stands on the
// ledge above it.
#define FLAG_WAY_TO_JAIL_OPEN 0x204
// Set as Chaucha and Eoleo leave the jail after visiting Briggs.
#define FLAG_FAMILY_VISITED_BRIGGS 0x8bd

/// The jail's people, numbering the rows of gAlhafranCave_JailActorPlacements.
enum AlhafraJailActorId {
  /// ★ Named from the game: "Chaucha... It's good to see you." (MsgYouHereTo)
  ACTOR_BRIGGS = ACTOR_FIRST_PLACED + 10,
  /// ★ Named from the game: "And you, too, Eoleo." (MsgYouHereTo)
  ACTOR_EOLEO = ACTOR_FIRST_PLACED + 13,
  /// ★ Named from the game: "I'm fine, Chaucha." (MsgYouHereTo)
  ACTOR_CHAUCHA,
  /// ★ Named from the game: "Did the mean old guard frighten you?"
  /// (MsgYouHereTo)
  ACTOR_JAILER = ACTOR_FIRST_PLACED + 16
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "field/field.h"

extern char SceneId_AlhafranCave;
extern char SceneId_AlhafraJail;

struct AlhafranCaveMapCell {
  u8 unk0[2];
  u8 state;
  u8 unk3;
};

struct AlhafranCaveMapLayer {
  struct AlhafranCaveMapCell *cells;
  u8 unk04[0x34];
};

struct AlhafranCaveMapWork {
  u8 unk000[0x17];
  u8 low17 : 2;
  u8 mode17 : 2;
  u8 high17 : 4;
  u8 unk018[0x120];
  struct AlhafranCaveMapLayer layers[1];
};

struct AlhafranCaveEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
  u8 unk1b0[0xb9a];
  u16 valueD4a;
  u16 valueD4c;
};

struct AlhafranCaveEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

struct AlhafranCaveSwitch {
  u16 id;
  s16 kind;
  s16 flag;
};

void Actor_ApplyRandomChildValues(void);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);
void FlagBlock_ApplyFlags(const void *data);
s32 Func_02001588(void);
void Actor_TakeLeaderLayer(void);
void BattleEffect_DrawSpriteAtWorldPosition(const void *data, s32 x, s32 y,
                                            s32 z);

void AlhafranCave_SetUpCave(void);
void AlhafranCave_SetUpJail(void);
void Pillars_Pound(const u16 *entry, s32 mode, s32 frame);
void Pillar_ShowState(s32 id, u32 kind, s32 flag, s32 layer);

extern s16 gAlhafranCave_Puddles[];
extern s16 gAlhafranCave_Pillars[];
extern s16 gAlhafranCave_CaveActorPlacements[];
extern struct ActorPlacement gAlhafranCave_JailActorPlacements[];
extern struct Trigger gAlhafranCave_CaveTriggers[];
extern struct Trigger gAlhafranCave_JailTriggers[];
extern u8 gAlhafranCave_CrateSprite[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void MapGrid_SetCellHeight_(s32 x, s32 z, s32 a, s32 b) {
  MapGrid_SetCellHeight(x, z, a, b);
}

static __inline__ void MapGrid_SetCellBehavior_(s32 x, s32 z, s32 a, s32 b) {
  MapGrid_SetCellBehavior(x, z, a, b);
}

static __inline__ void Map_CopyCellAttributeRect_(s32 a, s32 b, s32 c, s32 d,
                                                  s32 e, s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static __inline__ void MapGrid_CopyCellPictureCellsRect_(s32 a, s32 b, s32 c,
                                                         s32 d, s32 e, s32 f) {
  MapGrid_CopyCellPictureCellsRect(a, b, c, d, e, f);
}
#include "version.h"

const struct ActorPlacement *Scene_GetPlacements(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_AlhafranCave)
    return gAlhafranCave_CaveActorPlacements;
  if (scene == (s32)&SceneId_AlhafraJail)
    return gAlhafranCave_JailActorPlacements;
  return gAlhafranCave_CaveActorPlacements;
}

void AlhafranCave_PoundPillar(s32 mode, s32 frame) {
  Pillars_Pound(gAlhafranCave_Pillars, mode, frame);
}

void AlhafranCave_StartMovingCrate(void) {}

void AlhafranCave_FinishMovingCrate(void) {}

void AlhafranCave_TalkToBriggs(s32 id) {
  FieldEvent_SetMessageId((s32)MsgOhStoppedBy);
  Event_OpenMessage(id, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId((s32)MsgOhStoppedBy + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId((s32)MsgOhStoppedBy + 2);
  }
  Event_ShowMessage(id, 0);
}

void AlhafranCave_FreezePuddle(void) { FlagBlock_ApplyFlags(gAlhafranCave_Puddles); }

void AlhafranCave_OpenWayToJail(void) {
  Event_SetFlag(FLAG_WAY_TO_JAIL_OPEN);
  MapGrid_SetCellHeight_(0x1900000, 0x1300000, 0, 4);
  MapGrid_SetCellHeight_(0x1a00000, 0x1300000, 0, 4);
  MapGrid_SetCellHeight(0x1b00000, 0x1300000, 0, 4);
  MapGrid_SetCellBehavior_(0x1800000, 0x1200000, 0, 255);
  MapGrid_SetCellHeight_(0x1200000, 0x1800000, 0, 4);
  MapGrid_SetCellHeight(0x1500000, 0x1800000, 0, 4);
}

void AlhafranCave_CloseWayToJail(void) {
  Event_ClearFlag(FLAG_WAY_TO_JAIL_OPEN);
  MapGrid_SetCellHeight_(0x1900000, 0x1300000, 0, 0);
  MapGrid_SetCellHeight_(0x1a00000, 0x1300000, 0, 0);
  MapGrid_SetCellHeight(0x1b00000, 0x1300000, 0, 0);
  MapGrid_SetCellBehavior_(0x1800000, 0x1200000, 0, 0);
  MapGrid_SetCellHeight_(0x1200000, 0x1800000, 0, 0);
  MapGrid_SetCellHeight(0x1500000, 0x1800000, 0, 0);
}

void AlhafranCave_CastSand(void) {
  struct AlhafranCaveEventWork *work = Ram_HeapSlots->eventWork;

  work->valueD4a = 2;
}

const struct Trigger *Scene_GetEvents(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_AlhafranCave)
    return gAlhafranCave_CaveTriggers;
  if (scene == (s32)&SceneId_AlhafraJail)
    return gAlhafranCave_JailTriggers;
  return gAlhafranCave_CaveTriggers;
}

void AlhafranCave_TrackLeadersLevel(void) {
  s32 y = Actor_Get(gMainRoutineWork.currentOwner)->y;

  if (y < 0x80000)
    gMainRoutineWork.unk2c3 = 0;
  else
    gMainRoutineWork.unk2c3 = 1;
}

s32 AlhafranCave_DrawCrateSprite(void) {
  struct Actor *object = Actor_Get(8);

  BattleEffect_DrawSpriteAtWorldPosition(gAlhafranCave_CrateSprite, object->x,
                                         object->y + 0x1c0000, object->z);
  return 0;
}

s32 Scene_Initialize(void) {
#if !TLA_PATCH_1 /* The Japanese edition does not check the entrance. */
  ((struct AlhafranCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x201;
  if (gMainRoutineWork.scene == (s32)&SceneId_AlhafranCave)
    AlhafranCave_SetUpCave();
  else if (gMainRoutineWork.scene == (s32)&SceneId_AlhafraJail)
    AlhafranCave_SetUpJail();
  return 0;
#else
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_AlhafranCave) {
    ((struct AlhafranCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac =
        0x201;
    AlhafranCave_SetUpCave();
  } else if (scene == (s32)&SceneId_AlhafraJail) {
    if (gMainRoutineWork.entrance == 8 || gMainRoutineWork.entrance == 9)
      ((struct AlhafranCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac =
          0x209;
    else
      ((struct AlhafranCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac =
          0x201;
    AlhafranCave_SetUpJail();
  }
  return 0;
#endif
}

#include "task.h"

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

void AlhafranCave_SetUpCave(void) {
  struct AlhafranCaveMapWork *map =
      (struct AlhafranCaveMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object;

  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  map->mode17 = 1;
  Task_AddFrame(AlhafranCave_TrackLeadersLevel, TASK_PASS_VBLANK);
  if (Event_GetFlag(FLAG_WAY_TO_JAIL_OPEN))
    AlhafranCave_OpenWayToJail();
  object = Actor_Get(8);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object = Actor_Get(9);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object = Actor_Get(10);
  object->motionFlags = 0;
  object->y = 0xc0000;
  Actor_SetSpriteColorMode(Actor_Get(8), 6);
  Actor_SetSpriteColorMode(Actor_Get(9), 6);
  Actor_SetSpriteColorMode(Actor_Get(10), 6);
  object = Actor_Get(12);
  object->motionFlags = 0;
  object->y = 0x100000;
  object = Actor_Get(11);
  object->motionFlags = 0;
  object->y = 0x100000;
  FieldActor_SetSpriteRotationById(13, 0x1000);
  Actor_SetAnimation(11, 2);
  Actor_SetAnimation(12, 3);
  Actor_SetAnimation(13, 2);
  FieldActor_SetSpritePriority(11, 2);
  FieldActor_SetSpritePriority(12, 2);
  FieldActor_SetSpritePriority(13, 3);
  Actor_Get(11)->priorityFlags |= 2;
  Actor_Get(12)->priorityFlags |= 2;
  Actor_Get(13)->priorityFlags |= 2;
  Actor_SetSpriteColorMode(Actor_Get(11), 6);
  Actor_SetSpriteColorMode(Actor_Get(12), 6);
  Actor_SetSpriteColorMode(Actor_Get(13), 6);
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    Actor_SetPosition(14, 0xc80000, 0x880000);
    Actor_SetPosition(15, 0x1880000, 0x1680000);
  }
  object = Actor_Get(14);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  MapGrid_SetCellHeight(object->x, object->z, 0, 4);
  object = Actor_Get(15);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  MapGrid_SetCellHeight(object->x, object->z, 0, 4);
  FlagBlock_PlaceAllMarkingEdges(gAlhafranCave_Puddles);
  Actor_SetActionCallback(Actor_Get(16), 9);
  if (!Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    /* FAKEMATCH(wrapper): the three calls forwarded through
     * MapGrid_CopyCellPictureCellsRect_; as any one a direct call, 4 of 223
     * instructions differ in tla-en. */
    MapGrid_CopyCellPictureCellsRect_(0, 58, 12, 7, 13, 58);
    MapGrid_CopyCellPictureCellsRect_(0, 58, 2, 9, 25, 66);
    MapGrid_CopyCellPictureCellsRect_(0, 58, 10, 5, 15, 74);
  }
}

#include "call.h"

void FieldEffect_StartBlankOverlay(s32 value);
void Rope_Initialize(void);
void EffectSprite_InitFromSlot94(const void *data, s32 a, s32 b, s32 c);

/// The rooms' setup: sprite priorities, the scene callback, actors 12 to 14,
/// actors 15 to 24 by flags FLAG_ALHAFRA_MAST_CLEARED, FLAG_BRIGGS_CAUGHT and
/// FLAG_FAMILY_VISITED_BRIGGS, and actor 8 stepping in at entrance 9 until flag
/// 0x109 is set.
void AlhafranCave_SetUpJail(void) {
  const void *data;
  struct AlhafranCaveEventWork *work;

  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Actor_Get(8)->priorityFlags |= 32;
  FieldEffect_StartBlankOverlay(0);
  data = gAlhafranCave_CrateSprite;
  BattleEffect_FillOverlayTiles(0);
  Call4((void (*)())EffectSprite_InitFromSlot94, (s32)data, 8, 15, 0x40000000);
  FieldTask_AddFrame(AlhafranCave_DrawCrateSprite, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 8) <= 1) {
    work = Ram_HeapSlots->eventWork;

    work->valueD4c = 3;
  }
  Rope_Initialize();
  Rope_Place(0, 0x200, 10, 11);
  Actor_Get(10)->priorityFlags |= 32;
  Actor_Get(11)->priorityFlags |= 32;
  Pillars_Place(gAlhafranCave_Pillars);
  Actor_SetSpriteFlags(Actor_Get(9), 0);
  FieldActor_SetSpriteRotationById(14, 0x1000);
  Actor_SetAnimation(12, 0);
  Actor_SetAnimation(13, 1);
  Actor_SetAnimation(14, 2);
  FieldActor_SetSpritePriority(12, 1);
  FieldActor_SetSpritePriority(13, 1);
  Actor_Get(12)->priorityFlags |= 2;
  Actor_Get(13)->priorityFlags |= 2;
  Actor_Get(14)->priorityFlags |= 2;
  Actor_SetSpriteColorMode(Actor_Get(12), 6);
  Actor_SetSpriteColorMode(Actor_Get(13), 6);
  Actor_SetSpriteColorMode(Actor_Get(14), 6);
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    Actor_SetPosition(15, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(19, 0, 0);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_Get(23)->update = AlhafranCave_TakeLeadersLayer;
  } else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    Actor_SetPosition(23, 0, 0);
    FieldActor_SetSpritePriority(18, 2);
    FieldActor_SetSpritePriority(16, 2);
    FieldActor_SetSpritePriority(17, 2);
    FieldActor_SetSpritePriority(19, 2);
    FieldActor_SetSpritePriority(20, 2);
    if (!Event_GetFlag(FLAG_FAMILY_VISITED_BRIGGS)) {
      Actor_SetPosition(17, PIXELS(712), PIXELS(216));
      Actor_SetPosition(19, PIXELS(744), PIXELS(216));
    } else {
      Actor_SetPosition(22, 0, 0);
      Actor_SetPosition(21, 0, 0);
    }
  } else {
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(19, 0, 0);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_Get(23)->update = AlhafranCave_TakeLeadersLayer;
  }
  if (gMainRoutineWork.entrance == 9 &&
      !Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Actor_SetSpeed(8, 0x4ccc, 0x2666);
    Actor_SetDestinationOffset(8, 0, 16);
    FieldActor_WaitForMoveById(8);
  }
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_AlhafraJail &&
      Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    MapGrid_CopyCellPictureCellsRect_(23, 46, 4, 5, 21, 14);
    MapGrid_CopyCellPictureCellsRect_(86, 44, 5, 6, 84, 12);
    Map_CopyCellAttributeRect_(22, 45, 5, 6, 20, 13);
  }
  return 0;
}

void AlhafranCave_IdleTask(void) {}

extern u8 MsgYouHereTo[];

void AlhafranCave_ShakeUnderEoleosPower(void);

void AlhafranCave_ChauchaAndEoleoVisit(void) {
  struct Actor *actor21;
  struct Actor *actor15;
  struct Actor *effect;
  s32 position[3];

  /* FAKEMATCH(wrapper): 79 calls forwarded through call.h's Call2, Call3,
   * Call4 and Value3; as direct calls, 221 of 993 instructions differ in
   * tla-en (any one alone, 2 to 71). */
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT) == 0)
    return;
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED) == 1)
    return;
  Event_SetFlag(FLAG_FAMILY_VISITED_BRIGGS);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(ACTOR_CHAUCHA, 0x10000, 0x8000);
  Actor_SetSpeed(ACTOR_EOLEO, 0x10000, 0x8000);
  Actor_SetSpeed(17, 0x10000, 0x8000);
  Actor_SetSpeed(19, 0x10000, 0x8000);
  Actor_SetSpeed(20, 0x10000, 0x8000);
  FieldEvent_SetMessageId((s32)MsgYouHereTo);
  FieldEvent_ShowMessageThenWait(ACTOR_JAILER, 0, 10);
  Actor_WalkToAndWait(4, 0x320, 0x120);
  Actor_FaceDirection(4, 0x8000, 0);
  FieldCamera_MoveTo(0x2e80000, -1, 0x1080000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_BRIGGS, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_Jump(ACTOR_EOLEO, 6, 15);
  FieldActor_Jump(ACTOR_EOLEO, 6, 23);
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_ShowEmote(ACTOR_CHAUCHA, 0x102, 30);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_RunRepeatedMotion(16, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(16, 0, 10);
  Actor_ShowEmote(ACTOR_CHAUCHA, 0x100, 30);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0x3000, 0);
  FieldEvent_Wait(30);
  (void *)Actor_ShowEmote(ACTOR_CHAUCHA, 0x107, 50);
  FieldCamera_MoveTo(0x2e80000, -1, 0x1380000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_EOLEO, 3);
  FieldActor_StartRepeatedMotion(ACTOR_JAILER, 3);
  Actor_SetAttachedEffect(ACTOR_JAILER, 0x102);
  FieldEvent_Wait(40);
  EventCamera_FollowActor(ACTOR_JAILER, 1);
  FieldCamera_WaitForArrival();
  Actor_SetSpeed(ACTOR_JAILER, 0x18000, 0xc000);
  Actor_SetScript(ACTOR_JAILER, gAlhafranCave_GuardStepsAsideScript);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_EOLEO, 0x3000, 0);
  FieldActor_RefreshSelectorById(ACTOR_JAILER);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0, 0);
  FieldCamera_MoveTo(0x2e80000, -1, 0x1100000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_JAILER, 0x102, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_JAILER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JAILER, 4);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JAILER, 0, 10);
  Actor_FaceDirection(ACTOR_JAILER, 0xb000, 0);
  FieldEvent_Wait(20);
  (void *)Actor_ShowEmote(ACTOR_JAILER, 0x103, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_JAILER, 0, 10);
  actor21 = Actor_Get(ACTOR_EOLEO);
  actor15 = Actor_Get(15);
  Call2((void (*)())Field_StartColorTransition, 8, 0x1210);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_EOLEO), 2);
  Event_PlaySound(201);
  FieldEvent_Wait(40);
  effect = FieldEffect_SpawnMoveHand(actor21);
  position[0] = actor15->x;
  effect->watchedObject = actor15;
  position[1] = actor15->y + 0x100000;
  position[2] = actor15->z;
  FieldVector_AddPolarOffset(0x100000, 0xc000, position);
  FieldActor_SetMoveTarget(effect, position[0], position[1], position[2]);
  FieldEffect_SnapScaleToFull(effect);
  Actor_SelectAnimation(effect, 4);
  actor15->update = AlhafranCave_ShakeUnderEoleosPower;
  Actor_FaceDirection(4, 0x6000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(17, 0, 0);
  Actor_FaceDirection(19, 0x8000, 0);
  Actor_ShowEmote(4, 0x100, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(17, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(19, 3);
  Actor_FaceDirection(20, 0x3000, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(0x3280000, -1, 0x1200000, 1);
  FieldCamera_WaitForArrival();
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldEvent_Wait(50);
  Actor_ShowEmote(20, 0x102, 60);
  FieldCamera_MoveTo(0x2e80000, -1, 0x1100000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(20, -16, 0);
  FieldEvent_Wait(20);
  (void *)Actor_ShowEmote(20, 0x105, 60);
  Actor_FaceDirection(ACTOR_BRIGGS, 0x3000, 0);
  FieldEvent_Wait(40);
  FieldActor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS, 0x102);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_BRIGGS, 0x5000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 20);
  Actor_ShowEmote(ACTOR_EOLEO, 0x100, 30);
  actor15->update = 0;
  Actor_SetSpriteColorMode(actor15, 0);
  UpdateRisingParticleBurst(effect);
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_EOLEO), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(8);
  FieldEvent_Wait(30);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(16, 0, 0);
  Actor_FaceDirection(17, 0x3000, 0);
  Actor_FaceDirection(19, 0x5000, 0);
  Actor_FaceDirection(20, 0x8000, 0);
  Actor_FaceDirection(ACTOR_JAILER, 0xb000, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0xb000, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_EOLEO, 3);
  Actor_SetAttachedEffect(ACTOR_EOLEO, 0x102);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_EOLEO, 0xc000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 4);
  FieldEvent_Wait(20);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_FaceDirection(17, 0x3000, 0);
  Actor_FaceDirection(19, 0x5000, 0);
  Actor_FaceDirection(20, 0x5000, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, 0x5000, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0x5000, 0);
  Actor_FaceDirection(ACTOR_JAILER, 0x8000, 0);
  FieldEvent_Wait(40);
  Actor_ShowEmote(ACTOR_EOLEO, 0x100, 40);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, 4);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_EOLEO, 0x3000, 0);
  FieldEvent_Wait(20);
  Field_StartColorTransition(8, 0x1210);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_EOLEO), 2);
  Event_PlaySound(201);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_EOLEO), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(8);
  Actor_FaceDirection(16, 0, 0);
  Actor_FaceDirection(17, 0x3000, 0);
  Actor_FaceDirection(19, 0x5000, 0);
  Actor_FaceDirection(20, 0x8000, 0);
  Actor_FaceDirection(ACTOR_JAILER, 0xb000, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0xb000, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_EOLEO, 0xd000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  (void *)Actor_ShowEmote(ACTOR_EOLEO, 0x109, 50);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0x5000, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_CHAUCHA, 0x102, 50);
  Actor_RunRepeatedMotion(ACTOR_EOLEO, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_FaceDirection(17, 0x3000, 0);
  Actor_FaceDirection(19, 0x5000, 0);
  Actor_FaceDirection(20, 0x5000, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, 0x5000, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0x5000, 0);
  Actor_FaceDirection(ACTOR_JAILER, 0x8000, 0);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(ACTOR_EOLEO, 0x101);
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 20);
  gAlhafranCave_ClearedWord = 0;
  Task_AddFrame(AlhafranCave_IdleTask, 0x480);
  Actor_ShowEmote(ACTOR_JAILER, 0x102, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_JAILER, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0xb000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0x5000, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, 0, 8);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0x8000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_EOLEO, 2);
  FieldEvent_Wait(10);
  Actor_SetAttachedEffect(ACTOR_EOLEO, 0x100);
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_FaceDirection(ACTOR_EOLEO, 0, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0xf000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JAILER, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JAILER, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0xb000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_CHAUCHA, 0x8000, 0);
  Actor_FaceDirection(ACTOR_EOLEO, 0, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_CHAUCHA, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, 3);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(ACTOR_JAILER, 21);
  Actor_WalkBy(ACTOR_CHAUCHA, 0, -16);
  Actor_WalkByAndWait(ACTOR_EOLEO, 0, -16);
  Actor_WalkBy(ACTOR_CHAUCHA, 52, 0);
  Actor_WalkByAndWait(ACTOR_EOLEO, 52, 0);
  FieldEvent_Wait(15);
  FieldActor_StartRepeatedMotion(ACTOR_CHAUCHA, 3);
  Actor_SetAttachedEffect(ACTOR_CHAUCHA, 0x102);
  FieldEvent_Wait(40);
  FieldActor_WatchAndSetScript(4, 21);
  FieldActor_SetSpritePriority(ACTOR_CHAUCHA, 2);
  FieldActor_SetSpritePriority(ACTOR_EOLEO, 2);
  Actor_SetScript(ACTOR_CHAUCHA, gAlhafranCave_VisitorsLeaveScript);
  FieldEvent_Wait(10);
  Actor_SetScript(ACTOR_EOLEO, gAlhafranCave_VisitorsLeaveScript);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(20, 16, 0);
  Actor_FaceDirection(20, 0x3000, 0);
  Actor_WalkBy(17, 0, 16);
  Actor_WalkByAndWait(19, 0, 16);
  FieldEvent_Wait(5);
  Actor_FaceDirection(17, 0x3000, 0);
  Actor_FaceDirection(19, 0x3000, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, 0x3000, 0);
  FieldActor_RefreshSelectorById(ACTOR_EOLEO);
  FieldActor_Stop(ACTOR_JAILER);
  FieldActor_Stop(4);
  Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
  Actor_SetPosition(ACTOR_EOLEO, 0, 0);
  Actor_SetActionCallback(Actor_Get(ACTOR_JAILER), 9);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Event_End();
}

void AlhafranCave_TakeLeadersLayer(void) { Actor_TakeLeaderLayer(); }

void AlhafranCave_ShakeUnderEoleosPower(void) {
  Actor_ApplyRandomChildValues();
}

/// Walks a list of three-halfword entries (object id, kind, flag) ended by
/// 0xffff, clearing each object's collision bit and child flags before passing
/// the entry to Pillar_ShowState.
void Pillars_Place(const u16 *list) {
  const u16 *entry;

  if (list[0] == 0xffff)
    return;
  entry = list;
loop:
  {
    u32 id = entry[0];
    struct Actor *object = Actor_Get(id);
    s16 kind = entry[1];
    s16 flag = entry[2];

    object->collisionFlags &= ~1;
    Actor_SetSpriteFlags(object, 0);
    Pillar_ShowState(id, (u16)kind, (u16)flag, object->terrainId);
    entry += 3;
  }
  if (entry[0] != 0xffff)
    goto loop;
}

void Pillars_Pound(const u16 *entry, s32 mode, s32 frame) {
  struct AlhafranCaveEffectWork *work = Ram_HeapSlots->effectWork;
  while (entry[0] != 0xffff) {
    u32 value = *entry;
    s32 id = work->objectId;

    if (id == value) {
      struct Actor *object = Actor_Get(id);
      s32 kind;
      s32 flag;

      entry++;
      kind = entry[0];
      flag = entry[1];

      if (frame <= 7)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        Event_SetFlag(flag);
        Pillar_ShowState(id, kind, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
    entry += 3;
  }
}

void Pillar_ShowState(s32 id, u32 kind, s32 flag, s32 layer) {
  struct Actor *object = Actor_Get(id);
  struct AlhafranCaveMapCell *cell =
      ((struct AlhafranCaveMapWork *)Ram_HeapSlots->mapWork)
          ->layers[layer]
          .cells;

  cell += Func_02001588();
  if (Event_GetFlag(flag)) {
    cell->state = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Actor_SelectAnimation(object, 2);
      break;
    case 1:
      Actor_SelectAnimation(object, 4);
      break;
    case 2:
      Actor_SelectAnimation(object, 6);
      break;
    }
  } else {
    cell->state = 255;
    switch (kind) {
    case 0:
      Actor_SelectAnimation(object, 1);
      break;
    case 1:
      Actor_SelectAnimation(object, 3);
      break;
    case 2:
      Actor_SelectAnimation(object, 5);
      break;
    }
  }
}

// clang-format off
s16 gAlhafranCave_Puddles[] = {
    16, 515, -1, 0,
};

s32 gAlhafranCave_GuardStepsAsideScript[] = {
    39, 2, 5, 0x100000, 0, 0,
    1, 5, 0, 0, -0x3c0000, 1,
    5, -0x40000, 0, 0, 1, 39,
    1, 22, 3, 0x8000, 0, 1,
    17,
};

u32 gAlhafranCave_VisitorsLeaveScript[] = {
    39, 2, 4, 0x30c0000, 0, 0x1200000,
    1, 5, 0, 0, 0xc0000, 1,
    5, 0x400000, 0, 0, 1, 5,
    0x200000, 0, 0x200000, 1, 5, 0,
    0, 0x360000, 1, 17,
};

s16 gAlhafranCave_Pillars[] = {
    9, 1, 513, -1,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(100),
    SCENE_EXIT_IF(1, 95, 12, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 100, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 100, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 101, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 101, 7, CONDITION_ALWAYS),
    SCENE_EXITS(101),
    SCENE_EXIT_IF(6, 100, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 100, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 95, 8, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 101, 10, CONDITION_ALWAYS),
    SCENE_EXIT_IF(10, 101, 9, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

s16 gAlhafranCave_CaveActorPlacements[] = {
    369, -1, 7, 0, 0, 120, 0, 0,
    0, 296, 0, 2, 369, -1, 7, 0,
    0, 472, 0, 0, 0, 200, 0, 2,
    369, -1, 7, 0, 0, 424, 0, 0,
    0, 312, 0, 2, 371, -1, 7, 0,
    0, 296, 0, 0, 0, 392, 0, 2,
    371, -1, 7, 0, 0, 344, 0, 0,
    0, 392, FACING_WEST, 2, 369, -1, 7, 0,
    0, 324, 0, 0, 0, 416, FACING_SOUTH, 2,
    272, -1, 1, 0, 0, 216, 0, 0,
    0, 136, FACING_SOUTH, 2, 272, -1, 1, 0,
    0, 408, 0, 0, 0, 360, FACING_SOUTH, 2,
    290, -1, 1, 0, 0, 312, 0, 0,
    0, 392, FACING_SOUTH, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

struct ActorPlacement gAlhafranCave_JailActorPlacements[] = {
    {287, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(328), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(256), 0, PIXELS(312), FACING_SOUTH, 2, 1},
    {371, CONDITION_ALWAYS, 7, PIXELS(184), 0, PIXELS(284), FACING_EAST, 2, 0},
    {371, CONDITION_ALWAYS, 7, PIXELS(232), 0, PIXELS(284), FACING_WEST, 2, 0},
    {369, CONDITION_ALWAYS, 7, PIXELS(196), 0, PIXELS(304), FACING_SOUTH, 2, 0},
    {683, CONDITION_ALWAYS, 7, PIXELS(720), 0, PIXELS(336), FACING_SOUTH, 2, 0},
    {39, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {39, CONDITION_ALWAYS, 1, PIXELS(712), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {16, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {39, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, 0, 0},
    {39, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, 0, 0},
    {38, CONDITION_ALWAYS, 1, PIXELS(712), 0, PIXELS(304), FACING_NORTH, 2, 0},
    {37, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(296), FACING_NORTHWEST, 2, 0},
    {231, CONDITION_ALWAYS, 2, PIXELS(744), 0, PIXELS(320), FACING_SOUTH, 0, 0},
    {231, CONDITION_ALWAYS, 1, PIXELS(736), 0, PIXELS(360), FACING_NORTHWEST + FACING_STEP, 0, 0},
    {SCENE_TABLE_END},
};

struct Trigger gAlhafranCave_CaveTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x21, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(AlhafranCave_CloseWayToJail)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(AlhafranCave_OpenWayToJail)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 16, 0x203, EVENT(AlhafranCave_FreezePuddle)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AlhafranCave_CastSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafranCave_JailTriggers[] = {
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 20, FLAG_FAMILY_VISITED_BRIGGS, EVENT(AlhafranCave_ChauchaAndEoleoVisit)},
    {TRIGGER_TALK, 23, FLAG_ALHAFRA_MAST_CLEARED, MSGNO(MsgEverythingSSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, FLAG_ALHAFRA_MAST_CLEARED, MSGNO(MsgIDonTKnow)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgBeforeIKnew)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgNoMatterHow)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgIVeBeen)},
    {TRIGGER_TALK, ACTOR_BRIGGS, CONDITION_ALWAYS, EVENT(AlhafranCave_TalkToBriggs)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgIHatePrison)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgItSoundsLike)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgEverythingWouldHave)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgISawYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgItSSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgHasStrangePowers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgIMNotStaying)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgIfTheyDon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgOhWhatS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgIHearSome)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 10, 0x200, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, 5), 9, 0x201, EVENT(AlhafranCave_PoundPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 16, 0x203, EVENT(AlhafranCave_FreezePuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 8, CONDITION_ALWAYS, EVENT(AlhafranCave_StartMovingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(AlhafranCave_FinishMovingCrate)},
    {TRIGGER_TABLE_END},
};
