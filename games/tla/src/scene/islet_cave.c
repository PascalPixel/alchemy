/// @file
/// The scene code of overlay 0x675, in ROM order.

#include "shared/actors.h"
#include "shared/effects.h"
#include "field/field.h"
#include "system/main_routine_work.h"

#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"
#include "event_work.h"
#include "module/scene/garoh.h"
#include "module/scene/islet_cave.h"
#include "module/scene/scene.h"

extern u16 *gPaletteColors;
extern u16 *gPaletteCycleSlots;

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
DEFINE_PALETTE_SET_COLORS(Palette_SetColors, gData_02001a24, gPaletteColors,
    Data_02001a2c, gPaletteCycleSlots, Func_02000328)

#include "task.h"

extern u16 gPaletteCycleTimer;

DEFINE_PALETTE_CYCLE_START_VBLANK(IsletCave_StartSlowPaletteCycle, gData_02001a34, gData_02001a30,
    List_CountEntries, gData_02001a24, gData_02001a20, 15, gPaletteCycleTimer, 16,
    gData_02001a14, PaletteCycle_Fade)

void IsletCave_StopPaletteCycle(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Task_RemoveFar(PaletteCycle_Fade);
}

DEFINE_PALETTE_CYCLE_START_VBLANK(IsletCave_StartFastPaletteCycle, gData_02001a34, gData_02001a30,
    List_CountEntries, gData_02001a24, gData_02001a20, 2, gPaletteCycleTimer, 2,
    gData_02001a14, PaletteCycle_Fade)

DEFINE_LIST_COUNT_ENTRIES(List_CountEntries)

DEFINE_PALETTE_CYCLE_FADE(PaletteCycle_Fade, gPaletteCycleTimer, gData_02001a20,
    Data_02001a2c, gData_02001a30, gPaletteCycleSlots, gData_02001a34,
    gData_02001a24, gData_02001a14, gPaletteColors)

#include "actor.h"

void Func_02000328(void) {
  struct IsletCaveEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  BattleEffect_ApplyColorToTargetBuffer(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  ((struct IsletCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  IsletCave_StartFastPaletteCycle();
  Func_0200132c();
  wait(40);
  IsletCave_StopPaletteCycle();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  BattleEffect_StartBufferInterpolation(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

#include "event_work.h"
#include "text/msg_ids.h"

void Func_020003b0(void) {
  Func_080d2b4c(15, 12);
  return;
}

void Func_020003bc(void) {
  Func_080d2b4c(9, 37);
  return;
}

s32 Func_020003c8(s32 arg0) {
  Actor_SetSingleChildFlags(arg0, 0);
  return 0;
}

#include "scene_tables.h"

s32 Func_020003d4(struct Actor *object) {
  u8 *phase = (u8 *)object + 0x62;
  s32 step = *phase & 63;
  s16 *animation = object->sprite;

  object->motionFlags = 3;
  if (step < 32) {
    if (step == 0)
      object->velocityY = PIXELS(1);
    animation[9] = Math_Cosine(step << 12) / 128;
  }
  (*phase)++;
  return -1;
}

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "scene_ids.h"

extern const u8 gData_02001548[];
extern const u8 gData_02001668[];
extern const u8 gData_02001740[];
extern const u8 gData_02001788[];
extern const u8 gData_02001914[];
extern const u8 gData_020019b0[];

const u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov675Room1)
    return gData_02001668;
  if (scene == (s32)&SceneId_Ov675Room2)
    return gData_02001740;
  return gData_02001548;
}

const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov675Room2)
    return gData_02001914;
  if (scene == (s32)&SceneId_Ov675Room3)
    return gData_020019b0;
  return gData_02001788;
}

void Func_020004ac(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s32 effect = work->screenEffect;

  IsletCave_StartFastPaletteCycle();
  Func_0200132c();
  wait(40);
  IsletCave_StopPaletteCycle();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  BattleEffect_StartBufferInterpolation(16);
  wait(16);

  gMainRoutineWork.unk2c2 = 0;
  Actor_ConfigureSpriteColor(gMainRoutineWork.currentOwner, 0);
  work->screenEffect = effect;
}

/// 2 copies: islet_cave, islet_cave.
#define DEFINE_FUNC_02000508(name, n1, n2)                                     \
  void name(void) {                                                            \
    IsletCave_StartSlowPaletteCycle();                                                           \
    Func_02001334();                                                           \
                                                                               \
    gMainRoutineWork.unk2c2 = 1;                                               \
    Event_Wait(40);                                                            \
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,      \
                                           PIXELS(n1), PIXELS(n2));            \
    Func_020004ac();                                                           \
  }

DEFINE_FUNC_02000508(Func_02000508, 232, 136)

DEFINE_FUNC_02000508(Func_02000544, 240, 480)

void Func_02000580(s32 scale) {
  u16 *source = Ram_HeapSlots->block32;
  u16 *channel = (u16 *)((u8 *)source + 0xe00);
  u32 count = 224;
  u32 i = 0;
  u32 color, value, mask;

  do {
    color = *source++;
    mask = ~0x3ff;
    value = (color >> 10) & 31;
    value <<= 10;
    /* FAKEMATCH(dead): blue masked like the other two channels, clearing
       nothing; as no mask on blue, 38 of 59 instructions differ in tla-en. */
    value &= mask;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel++ = value;
    value = (color >> 5) & 31;
    value *= scale;
    value &= mask;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel++ = value;
    value = color & 31;
    value *= scale;
    value &= mask;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel++ = value;
    i++;
  } while (i < count);
}

#include "stddef.h"

extern const struct IsletCaveEntry gData_020019ec[];

void Func_020005fc(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u32 i;
  u32 index;

  Event_Start();
  Event_ResolvePendingActions(0);
  // A goto loop carries no loop notes: the bytes rebuild the zero on every
  // pass; as a for loop, loop.c hoists it and 20 of 63 instructions differ.
  i = 8;
loop:
  {
    struct Actor *object = Actor_GetById(i);

    if (object != 0)
      object->motionFlags = 0;
  }
  i++;
  if (i <= 63)
    goto loop;
  index = *(s16 *)(work + 0x154) - 1;
  Sound_Play(158);
  index >>= 1;
  {
    s32 x = gData_020019ec[index].position[0];
    /* FAKEMATCH(temporary): position[0] read first and position[1] read
     * through a pointer from a separate byte offset; as
     * gData_020019ec[index].position[1] in the call, 4 of 63 instructions
     * differ in tla-en. */
    u32 offset = index * sizeof(struct IsletCaveEntry) +
                 offsetof(struct IsletCaveEntry, position);
    const u16 *position = (const u16 *)((const u8 *)gData_020019ec + offset);

    MapGrid_AnimateCells(gData_020019ec[index].value, x, position[1]);
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -4);
  Event_Wait(6);
  Import_EventSetExitRequest(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

extern u16 gData_0200134c[], gData_0200135c[], gData_02001388[],
    gData_020013b4[];
extern u8 gData_02001454[];

s32 Scene_Initialize(void) {
  s32 scene, dim;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 521;
  Func_0200131c(0);
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov675Room1) {
    Func_020008c8();
    Palette_SetColors(gData_0200134c, gData_0200135c, gData_02001388,
                  gData_020013b4);
  } else if (scene == (s32)&SceneId_Ov675Room3) {
    Actor_SetSingleChildFlags(Actor_GetById(9), 0);
    if (gMainRoutineWork.entrance == 2) {
      Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
      GameFlag_SetBit(0x9cf);
    }
  } else if (scene == (s32)&SceneId_Ov675Room2) {
    if (GameFlag_Test(85) || GameFlag_Test(165)) {
      GameFlag_SetBit(0x230);
    } else if (!GameFlag_Test(0x230)) {
      Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
      Actor_SetMovementTypeById(8, gData_02001454);
    }
  } else {
    goto other;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov675Room2)
    goto fade;
other:
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov675Room3) {
  fade:
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov675Room2)
      dim = (gMainRoutineWork.entrance - 3) / 2;
    else
      dim = 3;
    Func_02000580(0x400 - (dim << 7));
    Func_0200133c();
    BattleEffect_StartBufferInterpolation(1);
    Event_WaitTransitionFrames();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Readies object id: when the flag is set, places it at (x, z) in cells;
/// marks it, then calls Map_CopyCellAttributeRect with the cell to the left of
/// it.
void Func_020007dc(s32 id, s32 a, s32 b, s32 x, s32 z, s32 unused, s32 flag) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    if (GameFlag_Test(flag))
      Actor_SetHorizontalPositionWithTerrain(id, x << 16, z << 16);
    Actor_SetSpritePriority(id, 3);
    object->priorityFlags |= 2;
    MapGrid_CopyCellAttributes(a, b, 3, 1, (object->x >> 20) - 1,
                               object->z >> 20);
  }
}

/// As Func_020007dc, with the cell above the object.
void Func_02000848(s32 id, s32 a, s32 bArg, s32 x, s32 z, s32 unused,
                   s32 flag) {
  struct Actor *object;

  if (Actor_GetById(id) != 0) {
    Actor_SetSpritePriority(id, 3);
    if (GameFlag_Test(flag))
      Actor_SetHorizontalPositionWithTerrain(id, x << 16, z << 16);
    object = Actor_GetById(id);
    object->priorityFlags |= 2;
    MapGrid_CopyCellAttributes(a, bArg, 1, 3, object->x >> 20,
                               (object->z >> 20) - 1);
  }
}

void Func_020008b4(s32 arg0) {
  Actor_SetModeById(arg0, 1);
  Actor_SetModeById(arg0, 2);
  return;
}

void Func_020008c8(void) {
  ((void (*)())Func_02000848)(8, 27, 0, 136, 0x1f8, 20, 0x280);
  ((void (*)())Func_02000848)(9, 28, 0, 136, 232, 21, 0x281);
  ((void (*)())Func_02000848)(10, 29, 0, 0x358, 152, 22, 0x282);
  ((void (*)())Func_02000848)(11, 30, 0, 0x318, 152, 23, 0x283);
  ((void (*)())Func_02000848)(12, 31, 0, 0x358, 0x178, 24, 0x284);
  ((void (*)())Func_02000848)(13, 32, 0, 0x318, 0x178, 25, 0x285);
  ((void (*)())Func_020007dc)(14, 33, 0, 0x338, 120, 26, 0x286);
  return;
}

void Func_020009a4(void) {
  Func_020008b4(8);
  return;
}

void Func_020009b0(void) {
  Func_020008b4(9);
  return;
}

void Func_020009bc(void) {
  Func_020008b4(10);
  return;
}

void Func_020009c8(void) {
  Func_020008b4(11);
  return;
}

void Func_020009d4(void) {
  Func_020008b4(12);
  return;
}

void Func_020009e0(void) {
  Func_020008b4(13);
  return;
}

void Func_020009ec(void) {
  Func_020008b4(14);
  return;
}

/* VIEW(islet_cave): tag struct FieldActor as struct FieldActor__islet_cave; field_event.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/islet_cave.c:519: dereferencing pointer to incomplete type. */
struct FieldActor__islet_cave {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 speed;
  s32 acceleration;
};

void Func_02001214(struct FieldActor__islet_cave *actor, s32 mode);

void IsletCave_RideLog(s32 actorId, s32 dx, s32 dz) {
  struct FieldActor__islet_cave *leader;
  struct FieldActor__islet_cave *actor;
  s32 x;
  s32 z;

  leader = ((struct FieldActor__islet_cave *)Actor_GetById(gMainRoutineWork.currentOwner));
  actor = ((struct FieldActor__islet_cave *)Actor_GetById(actorId));
  Event_Start();
  Event_ResolvePendingActions(0);
  x = ((leader->x + (dx << 16)) & 0xfff00000) + 0x80000;
  z = ((leader->z + (dz << 16)) & 0xfff00000) + 0x80000;
  leader->speed = 0x10000;
  leader->acceleration = 0x8000;
  Actor_SetMoveTarget(leader, x, leader->y, z);
  Func_02001214(leader, 27);
  x = ((actor->x + (dx << 16)) & 0xfff00000) + 0x80000;
  z = ((actor->z + (dz << 16)) & 0xfff00000) + 0x80000;
  actor->speed = 0x10000;
  actor->acceleration = 0x8000;
  Actor_SetMoveTarget(actor, x, actor->y, z);
  if (dx < 0 || dz < 0) {
    Func_02001214(actor, 4);
  } else {
    Func_02001214(actor, 3);
  }
  Sound_Play(226);
  Script_WaitForEventTimeout(leader);
  Func_02001214(actor, 2);
  Sound_Play(298);
  Event_End();
}

/// Rides log `logId` `dx` pixels across the water and moves its cells with it,
/// then sets or clears its flag (`flagCall`) to say where it lies. The
/// name is ours.
#define DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(name, logId, dx, flagCall, flag)    \
  void name(void) {                                                            \
    s32 fromX = Actor_GetById(logId)->x;                                       \
    s32 fromZ = Actor_GetById(logId)->z;                                       \
                                                                               \
    IsletCave_RideLog(logId, dx, 0);                                           \
    wait(2);                                                                   \
    Map_CopyCellAttributeRect(fromX >> 20, (fromZ >> 20) - 1, 1, 3,            \
                              Actor_GetById(logId)->x >> 20,                   \
                              (fromZ >> 20) - 1);                              \
    Map_CopyCellAttributeRect(0, 0, 1, 3, fromX >> 20, (fromZ >> 20) - 1);     \
    flagCall(flag);                                                            \
  }

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000aec, 8, 32, GameFlag_ClearBit, 0x280)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000b4c, 8, -32, GameFlag_SetBit, 0x280)

void Func_02000bb0(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(9);
  value1 = object0->x;
  object2 = Actor_GetById(9);
  value3 = object2->z;
  IsletCave_RideLog(9, 112, 0);
  IsletCave_RideLog(9, 96, 0);
  wait(2);
  object4 = Actor_GetById(9);
  Map_CopyCellAttributeRect((value1 >> 20), ((value3 >> 20) - 1), 1, 3,
                            (object4->x >> 20), ((value3 >> 20) - 1));
  Map_CopyCellAttributeRect(0, 0, 1, 3, (value1 >> 20), ((value3 >> 20) - 1));
  GameFlag_ClearBit(0x281);
  return;
}

void Func_02000c1c(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(9);
  value1 = object0->x;
  object2 = Actor_GetById(9);
  value3 = object2->z;
  IsletCave_RideLog(9, -112, 0);
  IsletCave_RideLog(9, -96, 0);
  wait(2);
  object4 = Actor_GetById(9);
  Map_CopyCellAttributeRect((value1 >> 20), ((value3 >> 20) - 1), 1, 3,
                            (object4->x >> 20), ((value3 >> 20) - 1));
  Map_CopyCellAttributeRect(0, 0, 1, 3, (value1 >> 20), ((value3 >> 20) - 1));
  GameFlag_SetBit(0x281);
  return;
}

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000c8c, 10, 80, GameFlag_SetBit, 0x282)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000cf0, 10, -80, GameFlag_ClearBit, 0x282)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000d54, 11, 80, GameFlag_ClearBit, 0x283)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000db8, 11, -80, GameFlag_ClearBit, 0x283)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000e1c, 12, 80, GameFlag_SetBit, 0x284)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000e7c, 12, -80, GameFlag_ClearBit, 0x284)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000ee0, 13, 80, GameFlag_ClearBit, 0x285)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000f44, 13, -80, GameFlag_SetBit, 0x285)

void Func_02000fa8(void) {
  struct Actor *object0;
  s32 value1;
  s32 v5;
  struct Actor *object2;
  s32 v6;
  s32 v8;
  struct Actor *object3;

  object0 = Actor_GetById(14);
  value1 = object0->x;
  v5 = value1;
  object2 = Actor_GetById(14);
  v6 = 112;
  v8 = object2->z;
  v6 = -v6;
  v8 = (v8 >> 20);
  IsletCave_RideLog(14, 0, v6);
  IsletCave_RideLog(14, 0, v6);
  IsletCave_RideLog(14, 0, -128);
  wait(2);
  object3 = Actor_GetById(14);
  v5 = (v5 >> 20);
  v5 = (v5 - 1);
  MapGrid_CopyCellAttributes(v5, v8, 3, 1, v5, (object3->z >> 20));
  Map_CopyCellAttributeRect(0, 0, 3, 1, v5, v8);
  GameFlag_SetBit(0x286);
  return;
}

void Func_02001034(void) {
  struct Actor *object0;
  s32 value1;
  s32 v5;
  struct Actor *object2;
  s32 value3;
  s32 v6;
  struct Actor *object4;

  object0 = Actor_GetById(14);
  value1 = object0->x;
  v5 = value1;
  object2 = Actor_GetById(14);
  value3 = object2->z;
  v6 = value3;
  IsletCave_RideLog(14, 0, 112);
  IsletCave_RideLog(14, 0, 112);
  IsletCave_RideLog(14, 0, 128);
  wait(2);
  object4 = Actor_GetById(14);
  v5 = (v5 >> 20);
  v5 = (v5 - 1);
  v6 = (v6 >> 20);
  MapGrid_CopyCellAttributes(v5, v6, 3, 1, v5, (object4->z >> 20));
  Map_CopyCellAttributeRect(0, 0, 3, 1, v5, v6);
  GameFlag_ClearBit(0x286);
  return;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/* VIEW(islet_cave): routine Field_RequestBattleFromEncounterTable as s32 Field_RequestBattleFromEncounterTable(); module/field/event_commands.h has void Field_RequestBattleFromEncounterTable(s32 row, s32 column); tried as the definition's void Field_RequestBattleFromEncounterTable(s32 row, s32 column), which the object already declares, Func_020010ac moves: 1 of 64 instructions in tla-ja. */
extern s32 Field_RequestBattleFromEncounterTable__islet_cave() __asm__("Field_RequestBattleFromEncounterTable");

void Func_020010ac(void) {
  s32 t12, cue;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(8, 2);
  Event_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(0x3c);
  Event_Wait(60);
  FieldMessage_ShowCentered((s32)MsgSummonGuardianOfWind, 0, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(0x3c);
  Event_Wait(60);
  t12 = ((s32)Actor_GetById(FIELD_AT(&gMainRoutineWork, s32, 532)));
  if (t12 != 0) {
    Actor_ResetAndSetPosition(8, FIELD_AT(t12, s16, 10),
                              FIELD_AT(t12, s16, 18));
  }
  Event_Wait(20);
  Event_SetAfterBattleDestination((s32)&SceneId_Ov675Room3, 2);
  FIELD_AT(&gMainRoutineWork, u8, 587) = 2;
  cue = 0x67;
  /* FAKEMATCH(barrier): the asm on the first argument loads it before the
   * second, as the native does; as Field_RequestBattleFromEncounterTable(0x67, 2), 2 of 64
   * instructions differ in tla-en. */
  __asm__ volatile("" : "+r"(cue));
  Field_RequestBattleFromEncounterTable__islet_cave(cue, 2);
  Event_End();
}

void Func_0200115c(void) {
  if (!Event_GetFlag(0x230)) {
    struct Actor *object = Actor_GetById(9);
    struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);

    Event_SetFlag(0x230);
    Actor_SetPosition(9, PIXELS(296), PIXELS(160));
    object->speedLimit = 0x18000;
    object->velocityY = PIXELS(15);
    Actor_MoveToAndWait(9, 344, ((owner->z >> 19) << 3) + 24);
    Actor_Stop(8);
    object->speedLimit = 0x10000;
    Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 9, 0);
  }
}
