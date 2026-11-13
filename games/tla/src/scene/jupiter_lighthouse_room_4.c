/// @file
/// The scene code of overlay 0x69d, in ROM order.

#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
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
#include "ram_buffer.h"
#include "field_event.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_sprite.h"
#include "macros.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_room_4.h"
#include "module/scene/scene.h"
#include "module/tla.h"

void Func_02001f00(s32 id);
void Func_02001ef8();

extern u16 gData_0200271c[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

static inline void SetCollisionCode(s32 x, s32 z, s32 layer, s32 value) {
  /* FAKEMATCH(wrapper): the call forwarded through this inline, as in ☀️'s
   * scene layer, so each call keeps its own constants; as a direct call, 12
   * of 47 instructions of Func_02000054 differ in tla-en. */
  ((u8)MapGrid_SetCellBehavior(x, z, layer, value));
}

void Func_02000054(void) {
  MapGrid_CopyCellsLayered(17, 10, 1, 4, 19, 10);
  MapGrid_CopyCellAttributes(17, 10, 1, 1, 19, 10);
  SetCollisionCode(0x1300000, 0xa00000, 0, 254);
  Actor_SetPosition(14, PIXELS(504), PIXELS(168));
  if (Event_GetFlag(0xa00)) {
    Actor_RaiseFromCell(14);
    ((u8)FieldMapGrid_SetCellBehavior(0x1f00000, 0xa00000, 0, 254));
  }
}

void Func_020000c0(void) {
  MapGrid_CopyCellsLayered(16, 10, 1, 4, 19, 10);
  MapGrid_CopyCellAttributes(16, 10, 1, 1, 19, 10);
  Actor_SetPosition(14, 0, 0);
  MapGrid_CopyCellAttributes(30, 10, 1, 1, 31, 10);
}

void Func_02000108(s32 unused, s32 id) { Actor_SetSpritePriority(id, 2); }

void Func_02000114(s32 unused, s32 id) {
  struct Actor *object;
  s32 i;

  object = Actor_Get(id);
  if (object->x >> 20 == 10 && object->z >> 20 == 32) {
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += 0x20000;
      System_Wait(1);
    }
    Actor_RaiseFromCell(id);
    Event_SetFlag(0xa06);
  }
}

void Func_02000164(void) { Import_EventSetExitRequest(12); }

void Func_02000170(void) { Import_EventSetExitRequest(13); }

void Func_0200017c(void) {
  MapGrid_CopyCellsLayered(73, 18, 1, 4, 74, 18);
  MapGrid_CopyCellAttributes(10, 9, 1, 1, 10, 20);
}

void Func_020001ac(void) {
  MapGrid_CopyCellsLayered(103, 18, 1, 4, 104, 18);
  MapGrid_CopyCellAttributes(40, 9, 1, 1, 40, 20);
}

/// 2 copies: jupiter_lighthouse_room_4, jupiter_lighthouse_room_4.
#define DEFINE_JUPITER_LIGHTHOUSE_ROOM_4_USE_KEY(name, n1, call1)              \
  void name(s32 item) {                                                        \
    Event_SetFlag(n1);                                                         \
    Event_PlaySound(134);                                                      \
    call1();                                                                   \
    PartyInventory_Remove(item);                                               \
  }

DEFINE_JUPITER_LIGHTHOUSE_ROOM_4_USE_KEY(Func_020001dc, 0xa07, Func_0200017c)

DEFINE_JUPITER_LIGHTHOUSE_ROOM_4_USE_KEY(Func_020001fc, 0xa08, Func_020001ac)

void Func_0200021c(void) {
  ((struct EventWorkLock *)Ram_HeapSlots->eventWork)->locked = 1;
  Message_ShowCentered(MsgFieldDoorLocked, 1);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// A particle's step, Jupiter Lighthouse room 2's Func_02000720 with other
/// numbers: it circles at its angle (action) around x 232 and rises with its
/// phase (unk66) from 120 pixels down, taking mode 7 at phase 1 and mode 2 at
/// phase 38 and starting over once above the ground; drawn in front while the
/// sine is negative and behind otherwise.
void Func_02000244(struct Actor *object) {
  s16 angle = object->timer;
  s16 height = object->unk66;

  SET_POSITION(object,
               Iwram_MulQ16(Math_Cosine(angle), object->speedLimit) +
                   (232 << 16),
               (height * 3 << 16) - (120 << 16), 130 << 18);
  if ((s16)object->unk66 == 1)
    Actor_SelectAnimation(object, 7);
  if ((s16)object->unk66 == 38)
    Actor_SelectAnimation(object, 2);
  if (object->y > 0)
    object->unk66 = 0xffff;
  if (Math_Sin(angle) < 0)
    ((struct FieldSprite *)object->sprite)->priority = 1;
  else
    ((struct FieldSprite *)object->sprite)->priority = 3;
  object->timer += object->acceleration;
  object->unk66++;
}

void Func_02000300(void) {
  struct Actor *object;
  s32 i;
  s32 phase;

  i = 0;
  for (phase = 0; i != 7; i++) {
    object = (FieldActor_Create(672, 0, 0, 0));
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (Random16Far() << 2) + 0x60000;
    object->acceleration = ((Random16Far() << 11) >> 16) + 0x800;
    object->update = (void (*)(void))Func_02000244;
    object->timer = Random16Far();
    object->unk66 = phase;
    Actor_SetSingleChildFlags(object, 0);
    FieldActor_SetSpriteColor(object, 2);
    ((struct FieldSprite *)object->sprite)->priority = 1;
    phase += 5;
  }
}

s32 Scene_Initialize(void) {
  struct Actor *leader;
  s32 flag;
  s32 i;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 516;
  Func_020000c0();
  if (Event_GetFlag(2567))
    Func_0200017c();
  if (Event_GetFlag(2568))
    Func_020001ac();
  Func_02000300();
  flag = Event_GetFlag(265);
  if (flag == 0 && gMainRoutineWork.entrance == 11) {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    leader->priorityFlags &= ~64;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
    Actor_SetAnimationSpeed(leader, 64);
    leader->motionFlags = flag;
    leader->y = leader->terrainHeight = -0x780000;
    Event_StartScreenTransition();
    FieldEvent_Wait(10);
    for (i = 0; i <= 89 && leader->y < 0; i++) {
      leader->terrainHeight = leader->y = leader->y + 0x30000;
      leader->facing += 0x1800;
      System_Wait(2);
    }
    leader->terrainHeight = leader->y = 0;
    leader->facing = 0x4000;
    leader->motionFlags = 3;
    leader->velocityY = 0x30000;
    leader->gravity = 0x8000;
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    FieldEvent_Wait(40);
    leader->gravity = 0x10000;
    leader->priorityFlags |= 64;
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
    Event_End();
  }
  if (!Event_GetFlag(265) && gMainRoutineWork.entrance == 12)
    Func_02001f00(gMainRoutineWork.currentOwner);
  return 0;
}

void JupiterLighthouse_LoadPalettes(s32 value);

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
  if (Event_GetFlag(0xa00)) {
    Actor_RaiseFromCell(8);
    Actor_RaiseFromCell(9);
    Actor_RaiseFromCell(10);
    Actor_RaiseFromCell(11);
    Actor_RaiseFromCell(12);
    Actor_RaiseFromCell(13);
    Actor_RaiseFromCell(16);
    Actor_RaiseFromCell(17);
    Actor_RaiseFromCell(18);
    Actor_RaiseFromCell(19);
    Actor_Get(19)->gravity = 0x30000;
  }
  Actor_SetSingleChildFlags(Actor_Get(19), 0);
  if (Event_GetFlag(0xa06)) {
    Actor_SetPosition(15, PIXELS(168), PIXELS(520));
    Actor_RaiseFromCell(15);
  }
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "shared/jupiter_lighthouse_aerie.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_020026f4;

void Func_020005d8(s32 value) { gData_020026f4 = value; }

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

DEFINE_PALETTE_CYCLE_STEP(PaletteCycle_Step, gPaletteCycleFrame, gData_020026f4,
                          gData_0200271c)

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gData_020026f4, gData_0200271c,
                                        gData_0200273c, PaletteCycle_Step,
                                        gData_02002740)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, Func_02001e28, Func_02001ef8)

#include "frame.h"
#include "event_work.h"

DEFINE_WHIRLWIND_CARRY_LEADER(Whirlwind_CarryLeader)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern const s32 gWhirlwindEndScript[];

s32 Func_02001ee8(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE(Whirlwind_Update, Func_02001ee8, Position_FlatDistance,
                        gWhirlwindEndScript, gData_02002718)
