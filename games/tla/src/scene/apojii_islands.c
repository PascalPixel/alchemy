/// @file
#include "shared/actors.h"
#include "flags.h"
#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "system/iwram_call.h"
#include "system/io_reg.h"
#include "field_sprite.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "text/msg_ids.h"

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Animation_ApplyChildArgument(void *animation, s32 value);
u8 *ResourceMetadata_Register(void *animation, s32 id);
s32 VramCache_FindFreeEntry(void);
void Irq_SetHandler(s32 a, s32 b, void *callback);
void Actor_StartScript(struct Actor *object, const void *data);
s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);
void Field_TryJumpForward(void);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);
void Shop_Run(s32 a, s32 b);
void Inn_CheckIn(s32 a, s32 b);

void Func_020003e8(void);
void Func_02000540(void);
void Func_020006bc(void);
void Func_02000770(struct Actor *object);
void Actor_FlickerChild(struct Actor *object);
void Func_02000974(void);
void Scroll_HBlank(void);
void Layer_TrackScroll(void);
void Scroll_StartHBlank(void);
void ApojiiIslands_InitializeRoom2(void);
void Func_020010b4(void);
void Func_020010d8(struct Actor *object);
s32 Actor_EmitMist(struct Actor *object);
void Leader_RiseFromWater(void);
void Func_020015f8(void);
void Func_020017bc(void);
void Func_020017f8(u32 base, s32 a, s32 b);
void GroundParticles_Draw(void);
void Func_02001ad8(void);
void Func_02001ba0(s32 id);

extern s32 gData_02002a98;
extern u16 gData_02002a9c;
extern u16 gData_02002a9e;
struct CellMarker {
  s32 x;
  s32 y;
  s32 z;
  s32 rest;
  u16 kind;
  u16 state;
  u32 oam[3];
};

struct CellMarkers {
  u16 block;
  u16 count;
  struct CellMarker markers[128];
};

extern struct CellMarkers gData_02002aa0;
extern u32 gData_02001ee8[];
extern u32 gWaterDropScript[];

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column, s32 offset) {
  Actor_CenterOnColumn(id, column, offset);
}

static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 a, s32 z, s32 b) {
  FieldCamera_MoveWithinMapBounds(x, a, z, b);
}

static __inline__ void Func_02001d04_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ void Func_02001e0c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}
#include "shared/effects.h"
#include "shared/tables.h"
#include "types.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "call.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "system/io_write_queue.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "field/event.h"
#include "event_work.h"
#include "field/field.h"
#include "field_event.h"
#include "flag_call.h"
#include "field/ground_mask.h"
#include "module/field/field.h"
#include "scene_tables.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "system/system.h"
#include "system/task.h"
#include "actor.h"
#include "map.h"
#include "field_sprite.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"
#include "system/heap_release.h"
#include "graphics/resource.h"
#include "version.h"
#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

DEFINE_SCENE_CREATE_ACTOR(Scene_CreateActor)

#include "../field/field_effect/move.c"

// @file
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

extern struct ActorPlacement gApojiiIslands_Placements[];
extern struct ActorPlacement gApojiiIslands_Room1Placements[];
extern struct ActorPlacement gApojiiIslands_Room3Placements[];
extern struct ActorPlacement gApojiiIslands_Room2Placements[];
extern s32 gData_020024cc[];

extern struct Trigger gApojiiIslands_Triggers[];
extern struct Trigger gApojiiIslands_Room1Triggers[];
extern struct Trigger gApojiiIslands_Room3Triggers[];
extern struct Trigger gApojiiIslands_Room2Triggers[];
void Func_020003b0(void);
void Func_020003cc(void);
void Func_020006a8(void);
void Func_02000b88(void);
void Func_02000ba8(void);
void ApojiiIslands_EnterDoor(void);
void Func_02000d74(void);
void Func_020010d0(void);
extern u32 gData_02001e34[];
extern u32 gData_02001e70[];
extern u32 gData_02001eac[];
extern s32 gData_02002044[];

extern u16 gData_02002a58[16];

extern u8 Data_0202c001[];

void Func_0200033c(void) { Djinn_JoinParty(10, 3, 11); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ApojiiIslands1)
    return gApojiiIslands_Room1Placements;
  if (scene == (s32)&SceneId_ApojiiIslands3)
    return gApojiiIslands_Room3Placements;
  if (scene == (s32)&SceneId_ApojiiIslands2)
    return gApojiiIslands_Room2Placements;
  return gApojiiIslands_Placements;
}

void Func_020003b0(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->terrainId = 2;
}

void Func_020003cc(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->terrainId = 0;
}

extern s32 gData_020024c4;
extern u16 Data_02002958[];
extern u16 Data_02002978[];
extern u16 gData_02002998[];
extern u16 gData_020029b8[];

void Func_020003e8(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 phase, k, offset;
  u16 *firstCopy, *secondCopy;

  if (*(s16 *)(work + 0x166) != 0)
    return;
  if (*(s8 *)(work + 0xca4) != 0)
    return;
  if (gData_020024c4 == -1) {
    Call3((void (*)())Iwram_CopyWordsEntry, Data_02002958, (void *)0x05000060,
          32);
    Iwram_CopyWordsEntry(Data_02002978, Data_02002958, 32);
    Iwram_CopyWordsEntry(gData_02002998, (void *)0x05000080, 32);
    Iwram_CopyWordsEntry(gData_020029b8, gData_02002998, 32);
    gData_020024c4 = 0;
  }
  phase = gData_020024c4 / 8 % 4;
  k = 0;
  firstCopy = Data_02002978;
  secondCopy = gData_020029b8;
  for (; k <= 3; k++) {
    offset = (phase + k) % 4 * 2 + 24;
    *(u16 *)((u8 *)firstCopy + offset) = Data_02002958[12 + k];
    *(u16 *)((u8 *)secondCopy + offset) = gData_02002998[12 + k];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)firstCopy, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x05000080, (u32)secondCopy, 0x84000008);
  gData_020024c4++;
}

extern s32 gData_020024c8;
extern u16 Data_020029d8[];
extern u16 Data_020029f8[];
extern u16 Data_02002a18[];
extern u16 Data_02002a38[];

void Func_02000540(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 busy, phase, k, j;

  if (*(s16 *)(work + 0x166) != 0)
    return;
  busy = *(s8 *)(work + 0xca4);
  if (busy != 0)
    return;
  if (gData_020024c8 == -1) {
    Call3((void (*)())Iwram_CopyWordsEntry, (s32)Data_020029d8, 0x05000060, 32);
    Iwram_CopyWordsEntry(Data_020029f8, Data_020029d8, 32);
    Iwram_CopyWordsEntry(Data_02002a18, (void *)0x050001a0, 32);
    Iwram_CopyWordsEntry(Data_02002a38, Data_02002a18, 32);
    gData_020024c8 = busy;
  }
  phase = gData_020024c8 / 8 % 4;
  for (k = 0; k <= 3; k++) {
    /* FAKEMATCH(dead): the phase and the first colour in a once-only block. */
    do {
      j = (phase + k) % 4;
      Data_020029f8[12 + j] = Data_020029d8[12 + k];
    } while (0);
    Data_02002a38[12 + j] = Data_02002a18[12 + k];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)Data_020029f8, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x050001a0, (u32)Data_02002a38, 0x84000008);
  gData_020024c8++;
}

void Func_02000694(void) {
  Task_SuspendAll(Func_02000540);
  System_Wait(3);
}

void Func_020006a8(void) {
  Task_ResumeAll(Func_02000540);
  System_Wait(3);
}

extern u16 gData_02002a78[16];

s32 __modsi3(s32 a, s32 b);

void Func_020006bc(void) {
  s32 base;
  s32 i;

  if (gData_020024cc[0] == -1) {
    Iwram_CopyWordsEntry(gData_02002a58, (void *)0x05000080, 32);
    gData_020024cc[0] = 0;
  }
  base = (gData_020024cc[0] / 2) % 15;
  for (i = 0; i < 15; i++)
    gData_02002a78[(base + i) % 15 + 1] = gData_02002a58[i + 1];
  IO_WRITE_QUEUE_PUSH(0x050003c0, (u32)gData_02002a78, 0x84000008);
  gData_020024cc[0]++;
}

void Func_02000770(struct Actor *object) {
  Actor_SetSpriteColorMode(object, 0);
  object->update = 0;
}

DEFINE_ACTOR_FLICKER_CHILD(Actor_FlickerChild, 7)

/* VIEW(s07a4): routine BattleEffect_ApplyColorToTargetBuffer as s32
 * BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b); event.h has void
 * BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode); tried as the
 * definition's void BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode),
 * which the object already declares, Actor_FlickerChild (gone) moves: 13 of 13
 * instructions in tla-ja. */
extern s32 BattleEffect_ApplyColorToTargetBuffer__s07a4(s32 a, s32 b) __asm__(
    "BattleEffect_ApplyColorToTargetBuffer");

static inline void ApojiiIslands_Flash(s32 strength, s32 mode) {
  BattleEffect_ApplyColorToTargetBuffer__s07a4(strength, mode);
}

static inline void ApojiiIslands_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

// An Apojii Islands scene inside an event: the actor takes Actor_FlickerChild
// as its update, cues 78 and 21 play, a sparkle setup (Func_02001ad8) runs and
// four flashes of random strength follow; then the screen blends in over six
// steps while the palette cycle gives way to Func_020006bc and the cell
// markers are rebuilt (Func_020017f8). Flags 0x95f and 0x95e are set and the
// camera pans through three points before cue 77.
void Func_020007a4(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->update = Actor_FlickerChild;
  Sound_Play(SOUND_FADE_OUT_BGM);
  wait(30);
  Sound_Play(MUS_021);
  Func_02001ad8();
  Func_02001ba0(17);
  Scene_QueueMusic(SE_170);
  i = 3;
  /* FAKEMATCH(wrapper): both flashes forwarded through ApojiiIslands_Flash. */
  /* FAKEMATCH(signature): BattleEffect_ApplyColorToTargetBuffer declared
   * returning s32, though its target returns nothing. */
flash:
  ApojiiIslands_Flash(Random16() & 0x7fff, 0);
  PaletteFade_StartInterpolation(15);
  wait(10);
  ApojiiIslands_Flash(0x10000, 0);
  PaletteFade_StartInterpolation(10);
  i--;
  wait(10);
  if (i >= 0)
    goto flash;
  ObjectTable_Snapshot();
  IO_WRITE_QUEUE_PUSH(0x04000050, 0xf00, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000052, 0x1000, 0x20000);
  wait(1);
  Task_Remove(Func_02000540);
  Task_Add(Func_020006bc, TASK_PASS_VBLANK);
  Func_020017bc();
  Func_020017f8(33, 0, 0);
  i = 1;
blend:
  IO_WRITE_QUEUE_PUSH(0x04000052, 0x1000 | i, 0x20000);
  wait(4);
  i++;
  if (i <= 6)
    goto blend;
  GameFlag_Set(0x95f);
  GameFlag_Set(0x95e);
  Camera_SetSpeedParameters(0x10000, 0x2000);
  /* FAKEMATCH(wrapper): the camera moves forwarded through
   * ApojiiIslands_CamMove. */
  ApojiiIslands_CamMove(PIXELS(360), -1, PIXELS(328), 1);
  Actor_ShowEmoteById(16, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  FieldCamera_WaitForArrival();
  ApojiiIslands_CamMove(PIXELS(440), -1, PIXELS(376), 1);
  FieldCamera_WaitForArrival();
  ApojiiIslands_CamMove(PIXELS(472), -1, PIXELS(408), 1);
  Import_EventSetExitRequest(77);
}

void Func_02000974(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_Get(17)->update = (void (*)(void))Func_02000770;
  FieldEvent_SetMessageId((s32)&MsgWhatWhatJust);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 16, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_FaceDirection(16, FACING_EAST, 0);
  Actor_SetAnimationAndWait(16, 3);
  Actor_ArmCallback_(16, FACING_WEST, 0);
  Actor_SetAnimationAndWait(16, 3);
  Actor_ArmCallback_(16, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(16, 3);
  FieldActor_ShowEmote(16, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Event_ShowMessage(16, 0);
  FieldActor_Jump(16, 2, 25);
  Event_ShowMessage(16, 0);
  Actor_RunRepeatedMotion(16, 1);
  Event_ShowMessage(16, 0);
  Actor_SetAnimationAndWait(16, 3);
  Event_ShowMessage(16, 0);
  Event_End();
  Event_ClearFlag(0x95e);
}

void Func_02000a48(void) { Func_02001e0c_(68, 38, 39, 24, 68, 4, 1); }

#define DEFINE_APOJII_ISLANDS_SHIFT_CELLS_AND_HOP(name, n1, n2, n3, n4, n5,    \
                                                  n6, n7, n8, n9, n10)         \
  void name(void) {                                                            \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
                                                                               \
    Func_02001e0c_(n1, 38, n2, n3, n1, n4, 1);                                 \
    if (Event_GetFlag(n5) == 0) {                                              \
      Func_02001d04_(n6, PIXELS(n7), PIXELS(n8));                              \
      if (leader->x >> 20 == n9 && leader->z >> 20 == n10) {                   \
        Event_Start();                                                         \
        FieldEvent_ResolvePendingActions(0);                                   \
        Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,                 \
                                ATTACHED_EFFECT_IN_FRONT |                     \
                                    ATTACHED_EFFECT_ONCE);                     \
        Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);      \
        leader->velocityY = 0x60000;                                           \
        FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);             \
        Event_End();                                                           \
      }                                                                        \
    }                                                                          \
  }

DEFINE_APOJII_ISLANDS_SHIFT_CELLS_AND_HOP(Func_02000a68, 68, 38, 21, 4, 0xf9b,
                                          65, 376, 376, 23, 23)

DEFINE_APOJII_ISLANDS_SHIFT_CELLS_AND_HOP(Func_02000af8, 68, 38, 21, 4, 0xf9a,
                                          64, 600, 408, 37, 25)

void Func_02000b88(void) { Func_02001e0c_(67, 38, 11, 6, 67, 8, 1); }

DEFINE_APOJII_ISLANDS_SHIFT_CELLS_AND_HOP(Func_02000ba8, 67, 11, 6, 8, 0xf9e,
                                          64, 200, 152, 12, 9)

// Stepping onto a door: the leader walks through it and the scene leaves by
// the exit of its number.
void ApojiiIslands_EnterDoor(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Event_PlaySound(SE_MAP_EXIT);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterOnColumn_(gMainRoutineWork.currentOwner, 2, -6);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

s32 Func_02000cb4(void) {
  u16 facing;

  facing = (s16)((Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) &
                 0xc000);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (facing) {
    Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)&MsgWhyDoesMy);
    Event_ShowMessage(9, 0);
    Actor_ArmCallback_(9, FACING_SOUTHEAST, 0);
  }
  Event_End();
  return 0;
}

s32 Func_02000d20(void) {
  u16 facing;

  facing = (s16)((Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) &
                 0xc000);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (facing) {
    FieldEvent_SetMessageId((s32)&MsgICanTEver);
    Event_ShowMessage(9, 0);
  }
  Event_End();
  return 0;
}

void Func_02000d74(void) {
  Actor_JumpDownLedge(gMainRoutineWork.currentOwner, 1, 5);
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ApojiiIslands1)
    return gApojiiIslands_Room1Triggers;
  if (scene == (s32)&SceneId_ApojiiIslands3)
    return gApojiiIslands_Room3Triggers;
  if (scene == (s32)&SceneId_ApojiiIslands2)
    return gApojiiIslands_Room2Triggers;
  return gApojiiIslands_Triggers;
}

void ApojiiIslands_TalkToInnkeeper(s32 actor) {
  if ((u16)((Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Inn_CheckIn(8, actor);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgWowYouLook);
  Event_ShowMessage(actor, 0);
  Event_End();
}

void Func_02000e40(s32 actor) {
  s32 msg;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  msg = (s32)&MsgYouGuysAren;
  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
  Event_End();
}

void ApojiiIslands_TalkToMerchant(s32 actor) {
  if ((u16)((Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Shop_Run(23, actor);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgItDBe);
  Event_ShowMessage(actor, 0);
  Event_End();
}

void Scroll_HBlank(void) {
  u32 value;

  if (REG_VCOUNT < gData_02002a98) {
    value = gData_02002a9c;
    REG_BG1HOFS = value;
    REG_BLDCNT = 0;
  } else {
    value = gData_02002a9e;
    /* FAKEMATCH(dead): the once-only block orders this statement. */
    do {
      REG_BG1HOFS = value;
    } while (0);
    REG_BLDALPHA = 0x100c;
    REG_BLDCNT = 0x3f42;
  }
}

DEFINE_LAYER_TRACK_SCROLL(Layer_TrackScroll, gData_02002a98, gData_02002a9c,
                          gData_02002a9e)

DEFINE_SCROLL_START_HBLANK(Scroll_StartHBlank, Scroll_HBlank, Layer_TrackScroll)

// The overlay's entry: sets the map mode and screen effect 0x202, then starts
// each room's own setup.
s32 Scene_Initialize(void) {
  s32 *effect;
  s16 scene;
  struct MainRoutineWork *party;

  ((struct MapBits *)Ram_HeapSlots->mapWork)->mode = 1;
  effect = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
  *effect = 0x202;
  party = &gMainRoutineWork;
  scene = party->scene;
  if (scene == (s32)&SceneId_ApojiiIslands2) {
    ApojiiIslands_InitializeRoom2();
  } else if (scene == (s32)&SceneId_ApojiiIslands1) {
    FieldTask_AddFrame(Func_02000540, 0x480);
    if (Event_GetFlag(0x95e))
      Func_02000974();
  } else if (scene == (s32)&SceneId_ApojiiIslands3) {
    *effect = 0x100;
    if (party->entrance == 6)
      Func_020010b4();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void ApojiiIslands_InitializeRoom2(void) {
  Task_AddFrame(Func_020003e8, TASK_PASS_VBLANK);
  Rope_Initialize();
  Rope_Place(0, 0x200, 8, 9);
  Scene_QueueMusic(SE_170);
  Scroll_StartHBlank();
  if (gMainRoutineWork.entrance == 3)
    Leader_RiseFromWater();
  if (gMainRoutineWork.entrance == 4)
    Leader_RiseFromWater();
  if (gMainRoutineWork.entrance == 5)
    Leader_RiseFromWater();
}

void Func_020010b4(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
}

void Func_020010d0(void) { Field_TryJumpForward(); }

void Func_020010d8(struct Actor *object) {
  struct Actor *child;
  struct FieldSprite *parent;
  struct FieldSprite *sprite;

  child = (FieldActor_Create(14, object->x, object->y + 0x40000,
                             object->z + 0x40000));
  parent = object->sprite;
  if (child != NULL) {
    sprite = child->sprite;
    child->terrainHeight = object->terrainHeight;
    Actor_StartScript(child, gData_02001ee8);
    child->motionFlags = 0;
    if (sprite != NULL) {
      Animation_ApplyChildArgument(sprite, 1);
      sprite->nextFrame = 0;
      sprite->priority = parent->priority;
    }
  }
}

DEFINE_ACTOR_EMIT_WATER_DROP(Actor_EmitWaterDrop, gWaterDropScript)

void Func_020011a8(void) {
  struct EventWork1cc *work = Ram_HeapSlots->eventWork;
  s32 owner = gMainRoutineWork.currentOwner;
  struct Actor *anchor = work->object;
  struct Actor *leader = Actor_Get(owner);
  s32 position[3];
  s32 result;

  if (gMainRoutineWork.scene != (s32)&SceneId_AquaRock1 ||
      leader->terrainHeight != 0) {
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z + 0x20000;
    result = MapGrid_CheckCollision(leader, position);
    if ((gFrameCount & 4) == 0)
      Func_020010d8(leader);
    if (result < 0) {
      Actor_SetAttachedEffect(owner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      FieldActor_SetMoveTargetFixed(leader, leader->x, leader->y,
                                    leader->z + 0x80000);
      Actor_SelectAnimation(leader, 49);
      FieldActor_WaitForMove(leader);
      do {
        System_Wait(1);
      } while (leader->y != leader->terrainHeight);
      Func_020010d8(leader);
      Actor_SelectAnimation(leader, 49);
      System_Wait(3);
    } else {
      position[0] = leader->x;
      position[1] = leader->y;
      position[2] = leader->z + 0x80000;
      result = MapGrid_CheckCollision(leader, position);
      if (result <= 0) {
        position[0] = leader->x + 0x5b333;
        position[1] = leader->y;
        position[2] = leader->z + 0x5b333;
        result = MapGrid_CheckCollision(leader, position);
        if (result <= 0) {
          position[0] = leader->x - 0x5b333;
          position[1] = leader->y;
          position[2] = leader->z + 0x5b333;
          result = MapGrid_CheckCollision(leader, position);
          if (result <= 0) {
            anchor->z += 0x20000;
            leader->z += 0x20000;
            leader->facing = 0x4000;
          }
        }
      }
    }
  }
}

DEFINE_ACTOR_SHOW_PARTS(Actor_ShowParts, Actor_SelectAnimation)

DEFINE_LEADER_SPIN_AWAY(Leader_SpinAway, Actor_SelectAnimation, Func_020010d8)

// Every other frame, emits particle 7 (5 on every other such frame) from the
// object, 26 pixels up, swaying with the frame count and drifting by a random
// amount.
DEFINE_ACTOR_EMIT_MIST(Actor_EmitMist)

DEFINE_LEADER_SINK_AWAY(Leader_SinkAway, Actor_EmitMist)

DEFINE_LEADER_RISE_FROM_WATER(Leader_RiseFromWater, Actor_EmitMist)

#define COPY_PICTURE(from, to)                                                 \
  Dma_Set((void *)(from), (void *)(offset + (to)), DMA_COPY_WORDS(16), REG_DMA3)

void Func_020015f8(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  // DEAD: viewY is subtracted from both height and z and cancels in dy; the
  // source keeps its load and a store to the stack that nothing reads.
  s32 viewY = map->origin[1];
  struct CellMarkers *table = &gData_02002aa0;
  struct VramBlockCacheEntry *entry = &gResourceTableEntries[table->block];
  u32 tile = entry->offset >> 5;
  u32 offset = entry->offset;
  struct CellMarker *record;
  s32 i;

  COPY_PICTURE(0x0600f000, 0x06010000);
  COPY_PICTURE(0x0600f400, 0x06010040);
  COPY_PICTURE(0x0600f040, 0x06010080);
  COPY_PICTURE(0x0600f440, 0x060100c0);
  COPY_PICTURE(0x0600f080, 0x06010100);
  COPY_PICTURE(0x0600f480, 0x06010140);
  COPY_PICTURE(0x0600f080, 0x06010180);
  COPY_PICTURE(0x0600f480, 0x060101c0);
  record = table->markers;
  for (i = 127; i >= 0; i--, record++) {
    s32 dx, dy, height, z, palette, flip, kind;
    s32 x, y, left, top, picture;
    u32 clear;
    u32 *attr;

    if (record->state == 0)
      continue;
    dx = record->x - viewX;
    height = record->y - viewY;
    z = record->z - viewZ - viewY;
    dy = z - height;
    if (record->y == record->rest && record->state != 2)
      palette = 0xec00;
    else
      palette = 0xe800;
    y = dy >> 16;
    clear = 0;
    x = dx >> 16;
    left = x - 8;
    top = y - 8;
    if ((u32)(x + 7) > 334)
      continue;
    if ((u32)(y + 39) > 286)
      continue;
    left &= 0x1ff;
    /* FAKEMATCH(dead): a self-update of Ram_HeapSlots->mapWork after left's
     * mask. */
    Ram_HeapSlots->mapWork += 0;
    top &= 255;
    attr = record->oam;
    kind = record->kind;
    picture = kind * 4;
    flip = 0;
    if (kind == 2)
      flip = 0x10000000;
    *attr++ = clear;
    *attr++ = top | (left << 16) | flip | 0x40000400;
    *attr = palette | (tile + picture);
    Oam_AddSpriteAtDepth(record->oam, 0);
  }
}

// Clears the effect table, takes a free VRAM entry for it with a 0x200-byte
// block and no source, then schedules Func_020015f8.
void Func_020017bc(void) {
  Iwram_ClearWordsEntry(&gData_02002aa0, 0x1004);
  gData_02002aa0.block = VramCache_FindFreeEntry();
  Vram_Load(gData_02002aa0.block, 0x200, 0);
  FieldTask_AddFrame(Func_020015f8, 0x480);
}

void Func_020017f8(u32 base, s32 a, s32 b) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *second = &map->layers[1];
  struct MapLayer *layer = &map->layers[0];
  struct MapCell *cells = second->cells;
  s32 left = *(s32 *)&layer->unk08[0] + map->minX;
  s32 top = *(s32 *)&layer->unk08[4] + map->minZ;
  struct CellMarkers *markers = &gData_02002aa0;
  s32 width = (map->maxX - map->minX) >> 20;
  s32 height = (map->maxZ - map->minZ) >> 20;
  struct CellMarker *entry = &markers->markers[markers->count];
  s32 *slot = (s32 *)(Data_0202c001 + 0x1df);
  s32 origin = ((map->minZ >> 20) << 7) + (map->minX >> 20);
  struct MapCell *cell;
  s16 x, z;
  u32 value;

  *slot = a << 8;
  cells += origin;
  z = 0;
  if (z < height) {
  outer:
    cell = cells + ((u16)z << 7);
    x = 0;
    if (x < width) {
    inner:
      value = cell->collisionCode;
      if (value != 0 && value >= base && value < base + 4) {
        entry->x = ((u16)x << 20) + left + 0x80000;
        entry->z = ((u16)z << 20) + top + 0x80000;
        entry->rest = b << 19;
        entry->y = a << 19;
        entry->kind = value - base;
        entry->state = 1;
        entry++;
        markers->count++;
        // Layer 0's row base in its own local (written inline, 10 of 159
        // differ).
        {
          u8 *terrain = map->layers[0].terrainIds + origin;

          terrain[(u16)x + ((u16)z << 7)] = 120;
        }
      }
      x = ((x << 16) + 0x10000) >> 16;
      cell++;
      if ((u16)x < width)
        goto inner;
    }
    z = ((z << 16) + 0x10000) >> 16;
    if ((u16)z < height)
      goto outer;
  }
}

extern s16 gGroundParticleFrames[];

DEFINE_GROUND_PARTICLES_DRAW(GroundParticles_Draw, gGroundParticleFrames)

// Starts the sparkles: 64 sprites on the ground at the origin, their tiles
// decoded into VRAM, and GroundParticles_Draw scheduled at priority 0x480 to
// move them; the overlay's copy of FieldEffect_InitializeSparkles.
void Func_02001ad8(void) {
  struct DustWork *work = HeapSlot_AllocEwram(116, sizeof(struct DustWork));
  struct DustParticle *p = work->particles;
  u8 *buf;
  u32 i;
  u32 size;
  volatile u32 zero;

  size = 0;
  zero = size;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 517, REG_DMA3);
  buf = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gField_Tiles679, buf);
  work->vramEntry = VramCache_FindFreeEntry();
  work->tileBase = Vram_Load(work->vramEntry, 0x300, buf);
  HeapSlot_Release(56);
  work->field808 = size;
  i = 0;
loop:
  size = (u32)p;
  *(u32 *)size = 0;
  size += 4;
  *(u32 *)size = 0x40000400;
  size += 4;
  *(u32 *)size = 0xb400;
  p->posX = 0;
  p->posZ = 0;
  p->posY = Map_GetTerrainHeight(0, 0, 0) << 16;
  p->timer = (i & 15) + 1;
  i++;
  p++;
  if (i < 64)
    goto loop;
  Task_AddFrame(GroundParticles_Draw, TASK_PASS_VBLANK);
}

void Func_02001ba0(s32 id) {
  u8 *work = Ram_HeapSlots->unk70[1];
  struct Actor *object = Actor_Get(id);
  s32 *slot = (s32 *)(work + 0x808);

  *slot = 0;
  if (id != 0 && object != 0)
    *slot = (s32)&object->x;
}

#include "text/msg_ids.h"

#if !TLA_PATCH_1
// The Japanese table holds 131 where later editions hold MsgFieldOff's number,
// which the Japanese text numbers alike: a changed value, from patch 1.
// XXX: is it a message at all; what reads gData_02002044 would settle it.
#define UNK_02002044_VALUE 131
#else
#define UNK_02002044_VALUE MSGNO(MsgFieldOff)
#endif

// clang-format off
u32 gData_02001e34[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001e70[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001eac[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001ee8[] = {
    0, 20, 38,
};

u32 gWaterDropScript[] = {
    23, 6, 0x4000, 0, 1, 13,
    30, 0, 38,
};

DEFINE_G_DATA_02001F18(gGroundParticleFrames)

const s32 *gFieldEffect_DurationScripts[] = {
    gData_02001e34,
    gData_02001e70,
    gData_02001eac,
};

s32 gData_02002044[] = {
    22,
    30,
    UNK_02002044_VALUE,
    22,
    16,
    0x3333,
    22,
    15,
    0x9999,
    (s32)0x80010000,
    39,
    2,
    4,
    0x1280000,
    0,
    0x880000,
    1,
    4,
    0x1280000,
    0,
    0x680000,
    1,
    39,
    1,
    0,
    60,
    0,
    10,
    0,
    60,
    39,
    2,
    4,
    0x1280000,
    0,
    0x880000,
    1,
    4,
    0x1180000,
    0,
    0x880000,
    1,
    39,
    1,
    0,
    60,
    0,
    10,
    0,
    60,
    13,
    0xffff,
    (s32)0xc0010000,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(ApojiiIslands1),
    SCENE_EXIT_UNLESS(1, WorldMap, 36, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, ApojiiIslands2, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, ApojiiIslands2, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, ApojiiIslands3, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, ApojiiIslands3, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, ApojiiIslands3, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, ApojiiIslands3, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, ApojiiIslands3, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, MikasallaInteriors2, 5, EXIT_FLAG_NONE),
    SCENE_EXIT(77, WorldMap, 78),
    SCENE_EXITS(ApojiiIslands3),
    SCENE_EXIT_UNLESS(1, ApojiiIslands1, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, ApojiiIslands1, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, ApojiiIslands1, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, ApojiiIslands1, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, ApojiiIslands1, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Islets5, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(ApojiiIslands2),
    SCENE_EXIT_UNLESS(1, ApojiiIslands1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, ApojiiIslands1, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, ApojiiIslandsCave, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, ApojiiIslandsCave, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, ApojiiIslandsCave, 5, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gApojiiIslands_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gApojiiIslands_Room1Placements[] = {
    {115, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(168), FACING_NORTHWEST, 1, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(88), FACING_NORTHEAST, 3, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(312), FACING_NORTHEAST, 1, 0},
    {128, CONDITION_ALWAYS, 2, PIXELS(504), 0, PIXELS(200), FACING_NORTH, 0, 0},
    {112, CONDITION_ALWAYS, 1, PIXELS(616), 0, PIXELS(264), FACING_SOUTHEAST, 1, 0},
    {113, CONDITION_ALWAYS, 2, PIXELS(184), 0, PIXELS(152), FACING_NORTHEAST, 1, 0},
    {136, CONDITION_ALWAYS, 3, PIXELS(216), 0, PIXELS(376), FACING_SOUTH, 0, 0},
    {138, CONDITION_ALWAYS, 3, PIXELS(104), 0, PIXELS(344), FACING_SOUTHEAST, 0, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(280), FACING_NORTH, 1, 0},
    {383, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gApojiiIslands_Room3Placements[] = {
    {116, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(152), FACING_EAST, 1, 0},
    {136, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(104), FACING_SOUTHEAST, 2, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(104), FACING_SOUTHEAST, 1, 0},
    {128, CONDITION_ALWAYS, (s32)gData_02002044, PIXELS(280), 0, PIXELS(136), FACING_EAST, 1, 0},
    {136, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(104), FACING_SOUTH, 1, 0},
    {138, CONDITION_ALWAYS, 2, PIXELS(344), 0, PIXELS(120), FACING_NORTHEAST, 1, 0},
    {112, CONDITION_ALWAYS, 3, PIXELS(592), 0, PIXELS(120), FACING_SOUTHEAST, 1, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(136), FACING_SOUTH, 1, 0},
    {110, CONDITION_ALWAYS, 1, PIXELS(416), 0, PIXELS(360), FACING_SOUTH, 1, 0},
    {134, CONDITION_ALWAYS, 1, PIXELS(144), 0, PIXELS(360), FACING_SOUTH, 1, 0},
    {135, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(376), FACING_SOUTHWEST, 1, 0},
    {112, CONDITION_ALWAYS, 3, PIXELS(128), 0, PIXELS(620), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gApojiiIslands_Room2Placements[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(280), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(280), FACING_NORTH, 2, 0},
    {246, 0x77, 1, PIXELS(376), 0, PIXELS(200), FACING_WEST, 0, 0},
    {SCENE_TABLE_END},
};

struct Trigger gApojiiIslands_Triggers[] = {
    {TRIGGER_TABLE_END},
};

s32 gData_020024c4 = -1;

s32 gData_020024c8 = -1;

s32 gData_020024cc[] = {
    -1,
};

struct Trigger gApojiiIslands_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_STEP, 7, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_STEP, 8, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0xc602, 33, CONDITION_ALWAYS, EVENT(Func_020010d0)},
    {0xc602, 21, CONDITION_ALWAYS, EVENT(Func_020010d0)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgThisIsApojii)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgWhateverYouDo)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTheHugeWaterfall)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgOnAquaIsland)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgHaveYouAny)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgIDonTKnowWhat)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgWhenTheyCome)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgDoYouThink)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgIfThereS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLookingOverGaia)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTheCurrentsNear)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgAllOfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgWhyWouldAnyone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgWellAtLeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgApojiiHasSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgAquaIslandIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgIWonderWhere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgAquaRockMust)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 17, 0x95f, EVENT(Func_020007a4)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_020003b0)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020003cc)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_02000694)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006a8)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 21, CONDITION_ALWAYS, EVENT(Func_02000a48)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 22, CONDITION_ALWAYS, EVENT(Func_02000a68)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 23, CONDITION_ALWAYS, EVENT(Func_02000af8)},
    {TRIGGER_TABLE_END},
};

struct Trigger gApojiiIslands_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgThereSThis)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Func_02000cb4)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgIAbsolutelyRefuse)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTheCurrentsAround)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgIMNotAllowed)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgHeyMisterDid)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgThatOldAqua)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgHeSToo)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(ApojiiIslands_TalkToMerchant)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(ApojiiIslands_TalkToInnkeeper)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Func_02000e40)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgIJustWish)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT(Func_02000d20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgISinkLike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgPoorBoyHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgItSStupid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgWowHeSwam)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgYouUnlockThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgHeAlwaysWanted)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgNoOneShips)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgIMSure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgThoseGuysOn)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTheTemperatureOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgLongAgoPoseidon)},
    {TRIGGER_TABLE_END},
};

struct Trigger gApojiiIslands_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Leader_SinkAway)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Leader_SinkAway)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Leader_SinkAway)},
    {0x4602, 40, CONDITION_ALWAYS, EVENT(Func_02000d74)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Func_0200033c)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x200, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 21, CONDITION_ALWAYS, EVENT(Func_02000b88)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 22, CONDITION_ALWAYS, EVENT(Func_02000ba8)},
    {TRIGGER_TABLE_END},
};
#undef COPY_PICTURE
#undef UNK_02002044_VALUE
#undef DEFINE_APOJII_ISLANDS_SHIFT_CELLS_AND_HOP
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
