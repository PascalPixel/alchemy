/// @file
/// The Islet Cave, in the rooms of this overlay: logs the party rides across
/// its water (IsletCave_RideLog), the Venus Djinni Meld and the Mercury Djinni
/// Serac at the end of their rides (IsletCave_FightMeld, IsletCave_FightSerac),
/// and the cycling colours of its water (IsletCave_StartSlowPaletteCycle).

#include "flags.h"
#include "field/field.h"
#include "system/main_routine_work.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "module/scene/garoh.h"
#include "module/scene/scene.h"
#include "system/task.h"
#include "actor.h"
#include "scene_tables.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_ids.h"
#include "stddef.h"
#include "shared/tables.h"
#include "objcmd.h"

void Func_02000328(void);

s32 List_CountEntries(u16 *list);

void PaletteCycle_Fade(void);

void Func_020008c8(void);

void IsletCave_FightMeld(void);

void IsletCave_FightSerac(void);

void Func_02000508(void);

void Func_02000544(void);

void Func_020005fc(void);

void Func_020009a4(void);

void Func_020009b0(void);

void Func_020009bc(void);

void Func_020009c8(void);

void Func_020009d4(void);

void Func_020009e0(void);

void Func_020009ec(void);
void Func_02000aec(void);
void Func_02000bb0(void);
void Func_02000c1c(void);

void Func_02000c8c(void);

void Func_02000d54(void);
void Func_02000db8(void);
void Func_02000e1c(void);
void Func_02000e7c(void);
void Func_02000ee0(void);
void Func_02000f44(void);
void Func_02000fa8(void);
void Func_02001034(void);
void Func_020010ac(void);

void Func_0200115c(void);

extern u32 gData_020013fc[];

extern s32 gData_02001418[];
extern s16 gData_020019d4[];

extern u16 *gData_02001a24;
extern u16 *Data_02001a2c;
extern u16 gData_02001a14;
extern u16 gData_02001a20;
extern u16 gData_02001a30;
extern u16 gData_02001a34;
void Teleport_PlaySparks(void);
void FieldEffect_StartBlankOverlay(s32 mode);
void BattleEffect_PackTargetBuffer(void);

extern u16 *gPaletteColors;
extern u16 *gPaletteCycleSlots;

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
void Palette_SetColors(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_02001a24 = slots;
  gPaletteColors = colours;
  Data_02001a2c = a;
  gPaletteCycleSlots = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      SET_RGB(((u16 *)BG_PLTT)[index], red, green, blue);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    Func_02000328();
}

extern u16 gPaletteCycleTimer;

void IsletCave_StartSlowPaletteCycle(void) {
  gData_02001a34 = 0;
  gData_02001a30 = List_CountEntries(gData_02001a24);
  gData_02001a20 = 15;
  gPaletteCycleTimer = 16;
  gData_02001a14 = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

void IsletCave_StopPaletteCycle(void) {
  gFieldWork->transition = 0x100;
  Task_Remove(PaletteCycle_Fade);
}

void IsletCave_StartFastPaletteCycle(void) {
  gData_02001a34 = 0;
  gData_02001a30 = List_CountEntries(gData_02001a24);
  gData_02001a20 = 2;
  gPaletteCycleTimer = 2;
  gData_02001a14 = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

#include "shared/list_count_entries.h"

void PaletteCycle_Fade(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gPaletteCycleTimer != 0) {
    if (--gPaletteCycleTimer == 0) {
      step = 1;
      gPaletteCycleTimer = gData_02001a20;
      if (gData_02001a20 > 2)
        gData_02001a20--;
    }
  }
  if (step == 0)
    return;
  src = Data_02001a2c;
  for (i = 0; i < gData_02001a30; i++) {
    dst = &BG_PLTT[gPaletteCycleSlots[gData_02001a34 * gData_02001a30 + i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    SET_RGB(*dst, red, green, blue);
  }
  dst = &BG_PLTT[gData_02001a24[gData_02001a30]];
  if (gData_02001a14 != 0)
    src = gPaletteColors + gData_02001a30 * 3;
  else
    src = Data_02001a2c + gData_02001a30 * 3;
  red = *src++;
  green = *src++;
  blue = *src++;
  SET_RGB(*dst, red, green, blue);
  gData_02001a14 ^= 1;
  if (++gData_02001a34 == gData_02001a30)
    gData_02001a34 = 0;
}

void Func_02000328(void) {
  struct IsletCaveEventWork *work = Ram_HeapSlots->fieldWork;
  s32 saved;

  saved = work->value1ac;
  BattleEffect_ApplyColorToTargetBuffer(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  ((struct IsletCaveEventWork *)Ram_HeapSlots->fieldWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  IsletCave_StartFastPaletteCycle();
  Teleport_PlayWhirl();
  wait(40);
  IsletCave_StopPaletteCycle();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  PaletteFade_StartInterpolation(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

#include "text/msg_ids.h"

void IsletCave_FightMeld(void) {
  Field_RequestDjinnBattle(15, 12);
  return;
}

void IsletCave_FightSerac(void) {
  Field_RequestDjinnBattle(9, 37);
  return;
}

s32 IsletCave_ClearScriptedActorSpriteFlags(s32 arg0) {
  Actor_SetSpriteFlags(arg0, 0);
  return 0;
}

s32 Func_020003d4(struct Actor *object) {
  u8 *phase = (u8 *)object + 0x62;
  s32 step = *phase & 63;
  s16 *animation = object->sprite;

  object->motionFlags = 3;
  if (step < 32) {
    if (step == 0)
      object->velocityY = px(1);
    animation[9] = Trig_Cos(step << 12) / 128;
  }
  (*phase)++;
  return -1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gIsletCave_Placements[];
extern struct ActorPlacement gIsletCave_Room1Placements[];
extern struct ActorPlacement gIsletCave_Room2Placements[];
extern struct Trigger gIsletCave_Triggers[];
extern struct Trigger gIsletCave_Room2Triggers[];
extern struct Trigger gIsletCave_Room3Triggers[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_IsletCave1)
    return gIsletCave_Room1Placements;
  if (scene == (s32)&SceneId_IsletCave2)
    return gIsletCave_Room2Placements;
  return gIsletCave_Placements;
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_IsletCave2)
    return gIsletCave_Room2Triggers;
  if (scene == (s32)&SceneId_IsletCave3)
    return gIsletCave_Room3Triggers;
  return gIsletCave_Triggers;
}

void Func_020004ac(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 effect = work->transition;

  IsletCave_StartFastPaletteCycle();
  Teleport_PlayWhirl();
  wait(40);
  IsletCave_StopPaletteCycle();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  PaletteFade_StartInterpolation(16);
  wait(16);

  gMainRoutineWork.unk2c2 = 0;
  Actor_ConfigureSpriteColor(gMainRoutineWork.currentOwner, 0);
  work->transition = effect;
}

#define DEFINE_ISLET_CAVE_ARRIVE_AT(name, n1, n2)                              \
  void name(void) {                                                            \
    IsletCave_StartSlowPaletteCycle();                                         \
    Teleport_PlaySparks();                                                     \
                                                                               \
    gMainRoutineWork.unk2c2 = 1;                                               \
    Event_Wait(40);                                                            \
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,      \
                                           px(n1), px(n2));                    \
    Func_020004ac();                                                           \
  }

DEFINE_ISLET_CAVE_ARRIVE_AT(Func_02000508, 232, 136)

DEFINE_ISLET_CAVE_ARRIVE_AT(Func_02000544, 240, 480)

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
     * nothing. */
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

extern struct MapGridCellAnimation gData_020019ec[];

void Func_020005fc(void) {
  u8 *work = Ram_HeapSlots->fieldWork;
  u32 i;
  u32 index;

  Event_Start();
  Event_ResolvePendingActions(0);
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
  index = ((struct FieldWork *)work)->stepTrigger - 1;
  Sound_Play(SE_DOOR_OPEN);
  index >>= 1;
  {
    s32 x = gData_020019ec[index].x;
    /* FAKEMATCH(temporary): x read first and y read through a pointer to x from
     * a separate byte offset. */
    u32 offset = index * sizeof(struct MapGridCellAnimation) +
                 offsetof(struct MapGridCellAnimation, x);
    const u16 *position = (const u16 *)((const u8 *)gData_020019ec + offset);

    MapGrid_AnimateCells(gData_020019ec[index].steps, x, position[1]);
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -4);
  Event_Wait(6);
  Import_EventSetExitRequest(((struct FieldWork *)work)->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

extern u16 gData_0200134c[];
extern u16 gData_0200135c[];
extern u16 gData_02001388[];
extern u16 gData_020013b4[];
extern s32 gData_02001454[];

s32 Scene_Initialize(void) {
  s32 scene, dim;

  gFieldWork->transition = 521;
  FieldEffect_StartBlankOverlay(0);
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_IsletCave1) {
    Func_020008c8();
    Palette_SetColors(gData_0200134c, gData_0200135c, gData_02001388,
                      gData_020013b4);
  } else if (scene == (s32)&SceneId_IsletCave3) {
    Actor_SetSpriteFlags(Actor_GetById(9), 0);
    if (gMainRoutineWork.entrance == 2) {
      Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
      GameFlag_Set(0x9cf);
    }
  } else if (scene == (s32)&SceneId_IsletCave2) {
    if (GameFlag_Get(85) || GameFlag_Get(165)) {
      GameFlag_Set(FLAG_SCENE_TEMP_30);
    } else if (!GameFlag_Get(FLAG_SCENE_TEMP_30)) {
      Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
      Actor_SetMovementTypeById(8, gData_02001454);
    }
  } else {
    goto other;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_IsletCave2)
    goto fade;
other:
  if (gMainRoutineWork.scene == (s32)&SceneId_IsletCave3) {
  fade:
    if (gMainRoutineWork.scene == (s32)&SceneId_IsletCave2)
      dim = (gMainRoutineWork.entrance - 3) / 2;
    else
      dim = 3;
    Func_02000580(0x400 - (dim << 7));
    BattleEffect_PackTargetBuffer();
    PaletteFade_StartInterpolation(1);
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
    if (GameFlag_Get(flag))
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
    if (GameFlag_Get(flag))
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

/* VIEW(islet_cave): tag struct FieldActor as struct FieldActor__islet_cave;
 * field_event.h has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/scene/islet_cave.c:545: dereferencing pointer to incomplete
 * type. */
struct FieldActor__islet_cave {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 speed;
  s32 acceleration;
};

void Actor_SelectAnimation(struct FieldActor__islet_cave *actor, s32 mode);

void IsletCave_RideLog(s32 actorId, s32 dx, s32 dz) {
  struct FieldActor__islet_cave *leader;
  struct FieldActor__islet_cave *actor;
  s32 x;
  s32 z;

  leader = ((struct FieldActor__islet_cave *)Actor_GetById(
      gMainRoutineWork.currentOwner));
  actor = ((struct FieldActor__islet_cave *)Actor_GetById(actorId));
  Event_Start();
  Event_ResolvePendingActions(0);
  x = ((leader->x + (dx << 16)) & 0xfff00000) + 0x80000;
  z = ((leader->z + (dz << 16)) & 0xfff00000) + 0x80000;
  leader->speed = 0x10000;
  leader->acceleration = px(1) / 2;
  Actor_SetMoveTarget(leader, x, leader->y, z);
  Actor_SelectAnimation(leader, 27);
  x = ((actor->x + (dx << 16)) & 0xfff00000) + 0x80000;
  z = ((actor->z + (dz << 16)) & 0xfff00000) + 0x80000;
  actor->speed = 0x10000;
  actor->acceleration = px(1) / 2;
  Actor_SetMoveTarget(actor, x, actor->y, z);
  if (dx < 0 || dz < 0) {
    Actor_SelectAnimation(actor, 4);
  } else {
    Actor_SelectAnimation(actor, 3);
  }
  Sound_Play(SE_LOG_PUSH);
  Actor_WaitForMove(leader);
  Actor_SelectAnimation(actor, 2);
  Sound_Play(SE_298);
  Event_End();
}

/// Rides log `logId` `dx` pixels across the water and moves its cells with it,
/// then sets or clears its flag (`flagCall`) to say where it lies. The
/// name is ours.
#define DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(name, logId, dx, flagCall, flag)     \
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

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000aec, 8, 32, GameFlag_Clear, 0x280)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000b4c, 8, -32, GameFlag_Set, 0x280)

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
  GameFlag_Clear(FLAG_SCENE_TEMP_81);
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
  GameFlag_Set(FLAG_SCENE_TEMP_81);
  return;
}

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000c8c, 10, 80, GameFlag_Set, 0x282)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000cf0, 10, -80, GameFlag_Clear, 0x282)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000d54, 11, 80, GameFlag_Clear, 0x283)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000db8, 11, -80, GameFlag_Clear, 0x283)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000e1c, 12, 80, GameFlag_Set, 0x284)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000e7c, 12, -80, GameFlag_Clear, 0x284)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000ee0, 13, 80, GameFlag_Clear, 0x285)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(Func_02000f44, 13, -80, GameFlag_Set, 0x285)

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
  GameFlag_Set(FLAG_SCENE_TEMP_86);
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
  GameFlag_Clear(FLAG_SCENE_TEMP_86);
  return;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020010ac(void) {
  s32 cue;
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(8, 2);
  Event_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  EventMessage_ShowCentered((s32)MsgSummonGuardianOfWind, 0, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object != 0) {
    Actor_SetDestinationById(8, FIELD_AT((u8 *)object, s16, 10),
                             FIELD_AT((u8 *)object, s16, 18));
  }
  Event_Wait(20);
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_IsletCave3, 2);
  gMainRoutineWork.battleRule24b = 2;
  cue = 0x67;
  /* FAKEMATCH(barrier): the asm on the first argument loads it before the
   * second. */
  __asm__ volatile("" : "+r"(cue));
  Field_RequestBattleFromEncounterTable(cue, 2);
  Event_End();
}

void Func_0200115c(void) {
  if (!Event_GetFlag(FLAG_SCENE_TEMP_30)) {
    struct Actor *object = Actor_GetById(9);
    struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);

    Event_SetFlag(FLAG_SCENE_TEMP_30);
    Actor_SetPosition(9, 296, 160);
    object->speedLimit = px(3) / 2;
    object->velocityY = px(15);
    Actor_MoveToAndWait(9, 344, ((owner->z >> 19) << 3) + 24);
    Actor_Stop(8);
    object->speedLimit = px(1);
    Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 9, 0);
  }
}

#undef DEFINE_ISLET_CAVE_ARRIVE_AT
#undef DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS
#undef FIELD_AT

void Func_02000b4c(void);
void Func_02000cf0(void);

// clang-format off
u16 gData_0200134c[] = {9, 10, 11, 12, 13, 14, 15, 65535};

u16 gData_0200135c[] = {23, 25, 22, 23, 25, 22, 23, 25, 22, 23, 25, 22, 23, 25, 22, 23, 25, 22, 23, 25, 22, 65535};

u16 gData_02001388[] = {30, 27, 31, 22, 21, 31, 20, 18, 31, 13, 11, 27, 11, 8, 24, 8, 5, 21, 31, 31, 31, 65535};

u16 gData_020013b4[] = {                                                               
      14, 9, 10, 11, 12, 13, 13, 14, 9, 10, 11, 12, 12, 13, 14, 9, 10, 11,  
      11, 12, 13, 14, 9, 10, 10, 11, 12, 13, 14, 9, 9, 10, 11, 12, 13, 14,  
  };

u32 gData_020013fc[] = {
    SC_SET(SW_SCALE_Y, 0x14ccc),
    SC_SET(SW_SCALE_X, 0x14ccc),
    SC_END,
};

s32 gData_02001418[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, px(-4)),
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_CALL(IsletCave_ClearScriptedActorSpriteFlags),
    SC_END,
};

s32 gData_02001454[] = {
    SC_CALL(Func_020003d4),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(IsletCave1),
    SCENE_EXIT_UNLESS(1, Islets5, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, IsletCave1, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, IsletCave1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, IsletCave1, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, IsletCave1, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, IsletCave1, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, IsletCave1, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, IsletCave2, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, IsletCave1, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, IsletCave1, 9, EXIT_FLAG_NONE),
    SCENE_EXITS(IsletCave2),
    SCENE_EXIT_UNLESS(1, IsletCave1, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, IsletCave2, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, IsletCave2, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, IsletCave2, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, IsletCave2, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, IsletCave2, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, IsletCave2, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, IsletCave2, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, IsletCave2, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, IsletCave3, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(IsletCave3),
    SCENE_EXIT_UNLESS(1, IsletCave2, 10, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gIsletCave_Placements[] = {
    ACTOR_PLACEMENT(170, 0x9cf, (s32)gData_020013fc, 184, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, (s32)gData_02001418, 152, 0, 144, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, (s32)gData_02001418, 216, 0, 144, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, (s32)gData_02001418, 136, 0, 176, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, (s32)gData_02001418, 232, 0, 176, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, (s32)gData_02001418, 152, 0, 224, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, (s32)gData_02001418, 216, 0, 224, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, (s32)gData_02001418, 136, 0, 256, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, (s32)gData_02001418, 232, 0, 256, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, (s32)gData_02001418, 136, 0, 304, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, (s32)gData_02001418, 232, 0, 304, FACING_NORTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIsletCave_Room1Placements[] = {
    ACTOR_PLACEMENT(298, CONDITION_ALWAYS, 7, 168, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(298, CONDITION_ALWAYS, 7, 344, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(298, CONDITION_ALWAYS, 7, 776, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(298, CONDITION_ALWAYS, 7, 872, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(298, CONDITION_ALWAYS, 7, 776, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(298, CONDITION_ALWAYS, 7, 872, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(297, CONDITION_ALWAYS, 7, 824, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(243, 0x3c, MOVEMENT_TYPE_STAND, 824, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIsletCave_Room2Placements[] = {
    ACTOR_PLACEMENT(252, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(244, 0x55, MOVEMENT_TYPE_STAND, 336, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gIsletCave_Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 20, CONDITION_ALWAYS, EVENT(Func_02000aec)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 20, CONDITION_ALWAYS, EVENT(Func_02000b4c)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT(Func_02000bb0)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT(Func_02000c1c)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 22, CONDITION_ALWAYS, EVENT(Func_02000c8c)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 22, CONDITION_ALWAYS, EVENT(Func_02000cf0)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 23, CONDITION_ALWAYS, EVENT(Func_02000d54)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT(Func_02000db8)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 24, CONDITION_ALWAYS, EVENT(Func_02000e1c)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT(Func_02000e7c)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT(Func_02000ee0)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT(Func_02000f44)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT(Func_02000fa8)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT(Func_02001034)},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Func_020009a4)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(Func_020009b0)},
    {TRIGGER_STEP, 22, CONDITION_ALWAYS, EVENT(Func_020009bc)},
    {TRIGGER_STEP, 23, CONDITION_ALWAYS, EVENT(Func_020009c8)},
    {TRIGGER_STEP, 24, CONDITION_ALWAYS, EVENT(Func_020009d4)},
    {TRIGGER_STEP, 25, CONDITION_ALWAYS, EVENT(Func_020009e0)},
    {TRIGGER_STEP, 26, CONDITION_ALWAYS, EVENT(Func_020009ec)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(IsletCave_FightMeld)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 50, CONDITION_ALWAYS, EVENT(Func_02000508)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 51, CONDITION_ALWAYS, EVENT(Func_02000544)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIsletCave_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Func_020005fc)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(Func_020005fc)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Func_020005fc)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Func_020005fc)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(Func_020005fc)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 8, 0x230, EVENT(Func_0200115c)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(IsletCave_FightSerac)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIsletCave_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 10, 0x9cf, EVENT(Func_020010ac)},
    {TRIGGER_TABLE_END},
};

s16 gData_020019d4[] = {
    72, 4, 2, 2, 2, 73, 4, 2,
    2, 2, -1, 0,
};
// clang-format on

struct MapGridCellAnimation gData_020019ec[] = {
    {gData_020019d4, 72, 4},  {gData_020019d4, 84, 4},  {gData_020019d4, 96, 4},
    {gData_020019d4, 107, 4}, {gData_020019d4, 118, 4},
};
