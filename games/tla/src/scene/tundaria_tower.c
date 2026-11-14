
#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Func_02002f90
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect/spawn.c"

#undef EFFECT_SET_SCRIPT

#include "types.h"
#include "flags.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "map.h"
#include "field/field.h"
#include "module/field/map.h"
#include "module/scene/magma_rock.h"
#include "flag_call.h"
#include "shared/tundaria_tower.h"
#include "field_event.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "system/iwram_call.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "call.h"
#include "system/field_effect.h"
#include "system/io_reg.h"
#include "field/field_sequence_event.h"
#include "system/frame.h"
#include "field/event.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "module/field/field_effect.h"
#include "system/input.h"
#include "version.h"
#include "module/field/object.h"
#include "shared/tables.h"
#include "field_sprite.h"
#include "gamepak/far_call/system.h"
#include "field/particle.h"

void Func_02000280(void) {
  GameFlag_Set(FLAG_SCENE_TEMP_01);
  Event_RequestDjinnBattle(8, 52);
}

#include "../field/object/wait_until_idle.c"

#include "../field/map/cell_get.c"

extern u8 gMapCellBuffer[];

/// Replaces a cell's collision code and attributes, returning the old value.
s32 MapCell_SetAttributes(u32 layer, s32 x, s32 z, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = (u8 *)map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (z << 7)) << 2;
  old = (cell[2] << 8) | cell[3];
  cell[2] = value >> 8;
  cell[3] = value;
  return old;
}

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void MapGrid_SetCellHeight(s32 x, s32 z, s32 terrain, s32 height);

/// Sets the cell under the object by the flag its action names: with the flag
/// set, bit 7 of the layer 0 attribute and the high byte of the layer 2
/// attribute are cleared and the sprite goes to priority 3; without it they are
/// set and the sprite goes to priority 2. MapGrid_SetCellHeight then gets the
/// cell, the object's terrain and the terrain height in half cells, 4 lower or
/// higher.
void Func_02000338(struct Actor *object) {
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 attr0 = MapCell_GetAttributes(0, x, z);
  s32 attr2 = MapCell_GetAttributes(2, x, z);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;

  if (GameFlag_Get(object->timer)) {
    attr2 &= 0xffff00ff;
    attr0 &= ~128;
    if ((object->terrainHeight >> 19) != height)
      height -= 4;
    Scene_SetActorSpritePriority(object, 3);
  } else {
    attr2 = (attr2 & 0xff) | 0xff00;
    attr0 |= 128;
    height += 4;
    Scene_SetActorSpritePriority(object, 2);
  }
  MapCell_SetAttributes(0, x, z, attr0);
  MapCell_SetAttributes(2, x, z, attr2);
  MapGrid_SetCellHeight(x << 20, z << 20, object->terrainId, height);
}

void Actor_SelectAnimation();

void Func_02000404(u16 *table) {
  u16 *row;
  struct FieldActor *object;
  s16 flag;
  s16 animation;

  if (table[0] == 0xffff)
    return;
  row = table;
loop:
  object = ((struct FieldActor *)Actor_GetById(row[0]));
  animation = row[1];
  flag = row[2];

  Actor_SetSpriteFlags(object, 0);
  Actor_SelectAnimation(object, (u16)animation + GameFlag_Get((u16)flag));
  object->motionFlags = 0;
  object->collisionFlags &= 0xfe;
  object->timer = flag;
  Func_02000338(object);
  row += 3;
  if (row[0] != 0xffff)
    goto loop;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern u32 gData_020033d0[];
extern u32 gData_020033f0[];

const struct ActorPlacement *Func_02000488(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower6)
    return (const struct ActorPlacement *)gData_020033d0;
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower7)
    return (const struct ActorPlacement *)gData_020033f0;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gData_02003618[];
extern struct ActorPlacement gData_02003648[];
extern struct ActorPlacement gData_02003660[];
extern struct ActorPlacement gData_02003690[];
extern struct ActorPlacement gData_02003708[];
extern struct ActorPlacement gData_020037c8[];

const struct Trigger *Func_020004c8(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower2)
    return (const struct Trigger *)gData_02003618;
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower3)
    return (const struct Trigger *)gData_02003648;
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower4)
    return (const struct Trigger *)gData_02003660;
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower5)
    return (const struct Trigger *)gData_02003690;
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower6)
    return (const struct Trigger *)gData_02003708;
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower7)
    return (const struct Trigger *)gData_020037c8;
  return (const struct Trigger *)gData_02003618;
}

void Func_02000548(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(19, 55, 7, 8, 24, 46);
  MapGrid_CopyCellsTo(24, 102, 24, 110, 7, 8);
  GameFlag_Set(2370);
  Event_End();
}

void Func_0200058c(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(37, 55, 7, 8, 32, 46);
  MapGrid_CopyCellsTo(32, 102, 32, 110, 7, 8);
  GameFlag_Set(2371);
  Event_End();
}

#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020005d0(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s16 *choice;
  s32 x = 37;
  s32 *owner;
  s32 one;
  s32 height;
  s32 cell;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  choice = &work->stepTrigger;
  if (*choice == 3)
    x = 25;
  Event_PlaySound(SE_DOOR_OPEN);
  one = 1;
  height = 2;
  MapGrid_CopyCellsTo(30, 38, x, 49, one, height);
  FieldEvent_Wait(10);
  MapGrid_CopyCellsTo(32, 38, x, 49, one, height);
  FieldEvent_Wait(10);
  owner = (s32 *)&gMainRoutineWork;
  owner += 133;
  Actor_SetSpeed(*owner, 0.5);
  ((struct Actor *)((s32)Actor_GetById(*owner)))->motionFlags = 0;
  cell = (((struct Actor *)((s32)Actor_GetById(*owner)))->x >> 19) | one;
  Actor_WalkToAndWait(*owner, cell << 3,
                      *(s16 *)((u8 *)((s32)Actor_GetById(*owner)) + 18) | one);
  Event_PlaySound(SE_MAP_EXIT);
  cell = ((struct Actor *)((s32)Actor_GetById(*owner)))->x;
  Actor_WalkTo(*owner, (cell >> 19) << 3,
               (*(s16 *)((u8 *)((s32)Actor_GetById(*owner)) + 18) | one) - 16);
  FieldEvent_Wait(5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WaitForMoveById(*owner);
  Import_EventSetExitRequest(*choice);
  Event_End();
}

void Func_020006d4(union FieldObject *object) {
  object->effect.x += object->effect.velocity.x;
  object->effect.y += object->effect.velocity.y;
  object->effect.z += object->effect.velocity.z;
  object->effect.scaleX += object->effect.scaleRateX;
  object->effect.scaleY += object->effect.scaleRateY;
  object->effect.velocity.y -= 0x4ccc;
}

// Scatters count sparks (effect type 279) around a position: each starts at
// the given scale, shrinks toward 0xcccc, turns to a random sixteenth of a
// turn and flies off with a random velocity through the overlay's effect
// spawner, falling under Func_020006d4.

/// Scatters count sparks (effect type 279) around a position: each starts at
/// the given scale, shrinks toward 0xcccc, turns to a random sixteenth of a
/// turn and flies off with a random velocity through the overlay's effect
/// spawner, falling under Func_020006d4.
void Func_02000708(struct TundariaTowerPosition *position, s32 count,
                   s32 scale) {
  struct EffectOptions options;
  s32 i;
  s32 velocityX;
  s32 velocityY;
  s32 x;

  options.priority = 1;
  options.type = 279;
  options.targetScaleY = 0xcccc;
  options.targetScaleX = 0xcccc;
  options.startScaleY = scale;
  options.startScaleX = scale;
  options.update = Func_020006d4;
  for (i = 0; i != count; i++) {
    s32 mask;

    options.rotation = (Random16() & 15) << 12;
    velocityX = Random16();
    mask = 15;
    velocityX &= mask;
    velocityX -= 8;
    velocityX <<= 14;
    velocityY = Random16() & mask;
    velocityY += 8;
    velocityY <<= 14;
    x = position->x - ((Random16() & 31) << 16) + 0xf0000;
    FieldEffect_Spawn(x, position->y + ((Random16() & mask) << 16), position->z,
                      velocityX, velocityY, 0, 0x15e0000, &options);
  }
}

void Func_020007dc(s32 step) {
  struct TundariaTowerPosition position;

  switch (step) {
  case 1:
    break;
  case 2:
    position.x = px(347);
    position.y = 0;
    position.z = px(376);
    Func_02000708(&position, 1, 0xcccc);
    break;
  case 3:
    MapGrid_CopyCellPictureIndicesRect(31, 85, 20, 85, 3, 4);
    position.x = px(347);
    position.y = 0;
    position.z = px(376);
    Func_02000708(&position, 8, 0x18000);
    break;
  }
}

void Func_02000840(struct Actor *object) {
  struct EffectOptions params;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    {
      // The mask, named where it is declared, is built before the draw.
      s32 mask = 15;

      dx = mask & Random16();
    }
    dy = Random16() & 7;
    params.type = 673;
    FieldEffect_Spawn(object->x + ((dx - 8) << 16),
                      object->y + ((dy - 8) << 16), object->z, 0, 0x8000, phase,
                      0x100000, &params);
  }
}

void Func_020008a8(void) {
  struct Actor *object = Actor_GetById(8);

  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_Set(0x944);
  MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
  object->gravity = px(1) / 2;
  object->motionFlags = 3;
  OverlayObject_WaitUntilIdle(object);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(68, px(344), px(372));
  Actor_GetById(68)->update = (void (*)(void))Func_02000840;
  Event_End();
}

void Func_02000920(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show(MsgThereSSomething, 1);
  if (GameFlag_Get(0x949)) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    work->fieldMenu = 1;
  }
  Event_End();
}

void Func_0200095c(void) { TundariaTower_SlideOnIce(); }

extern u32 gData_02003370[];

/// Actor 8 is placed and given its script while the screen flashes, and three
/// map cells change.
void Func_02000964(void) {
  struct Actor *object = Actor_GetById(8);

  Event_Start();
  Event_ResolvePendingActions(0);
  TundariaTower_FadeFrom(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  PaletteFade_StartInterpolation(2);
  Sound_Play(SE_WATER);
  TundariaTower_SetWork(0x50000, 0x70000, 0x10000);
  TundariaTower_SetWork(-1, -1, 0xe666);
  Event_Wait(2);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(2);
  object->actionFlags = 0;
  object->update = 0;
  Actor_SetDestination(8, 424, 536);
  Func_02002f90(object, gData_02003370);
  Actor_WaitForMoveById(8);
  GameFlag_Set(FLAG_SCENE_TEMP_00);
  Actor_WatchAndSetScript(8, gMainRoutineWork.currentOwner);
  MapCell_SetAttributes(0, 21, 33, 0x3200);
  MapCell_SetAttributes(0, 22, 33, 0x3200);
  MapCell_SetAttributes(0, 26, 33, 0xff00);
  Event_End();
}

void Func_02000a48(s32 arg0, s32 arg1) {
  struct EffectOptions options;
  s16 *effectWork = Ram_HeapSlots->effectWork;
  struct Actor *object = Actor_GetById(effectWork[13]);
  s32 velocityX;

  if (arg1 < 8) {
    options.type = 279;
    options.startScaleY = 0x10000;
    options.startScaleX = 0x10000;
    options.priority = 0;
    options.targetScaleY = 0x8000;
    options.targetScaleX = 0x8000;
    options.rotation = (Random16() & 15) << 12;
    velocityX = ((Random16() & 15) - 8) << 13;
    FieldEffect_Spawn(object->x, object->y - ((arg1 * 3) << 16) + 0x100000,
                      object->z, velocityX, 0, 0, 0x5a0000, &options);
    object->scaleY -= 7936;
  }
  if (arg0 == 1) {
    object->scaleY = Q16_ONE;
    GameFlag_Set(object->timer);
    Func_02000338(Actor_GetById(effectWork[13]));
    MapGrid_CopyCellPictureIndicesRect(67, 23, (object->x >> 20) + 64,
                                       object->z >> 20, 1, 1);
    object->z = 0;
    object->y = 0;
    object->x = 0;
  }
}

void Func_02000b34(s32 arg0, s32 arg1) { Func_02000a48(arg0, arg1); }

/// The leader turns to angle 0x4000 and takes mode 22, shows emote 0x102, then
/// turns to 0xc000 and takes mode 5 and action 24.
void Func_02000b3c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveToFixed(-1, -1, -1, 0);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  leader->facing = 0x4000;
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Event_Wait(30);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Motion_SetVarCbAndRefresh(gMainRoutineWork.currentOwner, 2);
  Event_Wait(20);
  leader->facing = 0xc000;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 5);
  Actor_SetActionById(gMainRoutineWork.currentOwner, 24);
  Event_Wait(40);
  MapGrid_SetCellHeight(0x1300000, 0x3900000, 0, -12);
  Actor_GetById(gMainRoutineWork.currentOwner)->bounce = 0;
  Import_EventSetExitRequest(11);
  Event_End();
}

void Func_02000bfc(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->scene == (s32)&SceneId_TundariaTower6) {
    struct Actor *object;

    Actor_LiftTargetBlock(gData_02003264);
    object = *(struct Actor **)((u8 *)work + 0xcbc);
    object->timer = ((object->x >> 20) << 8) | ((object->z >> 20) & 0xff);
  } else if (party->scene == (s32)&SceneId_TundariaTower7) {
    Actor_LiftTargetBlock(gData_0200326e);
  }
}

/// The event's object keeps stepping a cell at a time in the direction it was
/// pushed (its cell against the one Func_02000bfc stored in its action) while
/// the next cell's layer 1 attribute is 50 and its layer 2 attribute is not
/// 255, then settles on the middle of its cell.
void Func_02000c64(s32 unused, s32 actorArg) {
  u8 *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;
  u16 actor;
  s32 dx;
  s32 dz;

  actor = actorArg;
  object = *(struct Actor **)(work + 0xcbc);
  Event_Start();
  Event_ResolvePendingActions(0);
  dx = (object->x >> 20) - (object->timer >> 8);
  dz = (object->z >> 20) - (object->timer & 0xff);
  object->speedLimit = px(1) / 2;
  while ((MapCell_GetAttributes(1, (object->x >> 20) + dx,
                                (object->z >> 20) + dz) >>
          8) == 50 &&
         (MapCell_GetAttributes(2, (object->x >> 20) + dx,
                                (object->z >> 20) + dz) >>
          8) != 255) {
    Actor_SetMoveTarget(object, (dx << 20) + object->x, object->y,
                        (dz << 20) + object->z);
    FieldCamera_FollowActor(actor, 1);
    Actor_WaitForMove(object);
    wait(1);
  }
  object->x = ((object->x >> 19) | 1) << 19;
  object->z = ((object->z >> 19) | 1) << 19;
  Actor_PlaceMovedBlock();
  Event_End();
}

/// Off layer 2's terrain height, the event's object runs cue 188 on terrain 2
/// with motion flags 3. In room 6, column 27 sets flag 0x947, and column 25
/// with that flag set sets 0x946.
void Func_02000d2c(void) {
  u8 *event = Ram_HeapSlots->fieldWork;
  struct Actor *object;

  object = *(struct Actor **)(event + 0xcbc);

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
    object->terrainId = 2;
    object->motionFlags = 3;
    OverlayObject_WaitUntilIdle(object);
    Sound_Play(SE_THUMP);
    OverlayObject_WaitUntilIdle(object);
    object->motionFlags = 0;
    object->terrainId = 0;
  }
  Actor_PlaceMovedBlock();
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower6) {
    /* FAKEMATCH(temporary): the copy makes each column test reload X. */
    struct Actor *o = object;

    if (o->x >> 19 == 27)
      GameFlag_Set(0x947);
    if (o->x >> 19 == 25 && GameFlag_Get(0x947))
      GameFlag_Set(0x946);
  }
  Event_End();
}

void Func_02000de4(void) {
  BlendScript_Resume();
  GameFlag_Clear(FLAG_SCENE_TEMP_12);
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    MapGrid_CopyCellPictureIndicesRect(39, 100, 53, 100, 3, 3);
    MapGrid_CopyCellAttributes(43, 41, 3, 3, 53, 37);
    if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 20 == 38) {
      s16 angle = Actor_GetById(gMainRoutineWork.currentOwner)->facing;
      s32 owner = gMainRoutineWork.currentOwner;

      if (angle < 0)
        Actor_SetHorizontalPositionWithTerrain(owner, px(872), px(600));
      else
        Actor_SetHorizontalPositionWithTerrain(owner, px(872), px(632));
    }
  }
}

void Func_02000e78(void) {
  BlendScript_Suspend();
  GameFlag_Set(FLAG_SCENE_TEMP_12);
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    MapGrid_CopyCellPictureIndicesRect(43, 100, 53, 100, 3, 3);
    MapGrid_CopyCellAttributes(43, 37, 3, 3, 53, 37);
    if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 20 > 38)
      MapCell_SetAttributes(0, 54, 37, 0x1f00);
    else
      MapCell_SetAttributes(0, 54, 39, 0x1f00);
  }
}

void Func_02000f00(void) { Event_ResolvePendingActions(0); }

void Func_02000f0c(s32 step) {
  struct TundariaTowerPosition position;

  switch (step) {
  case 1:
    MapGrid_CopyCellPictureIndicesRect(18, 97, 6, 97, 3, 4);
    MapGrid_CopyCellPictureIndicesRect(82, 34, 70, 34, 3, 2);
    MapGrid_CopyCellAttributes(7, 36, 1, 1, 7, 35);
    GameFlag_Set(0x94b);
    break;
  case 2:
    position.x = px(120);
    position.y = 0;
    position.z = px(568);
    Func_02000708(&position, 1, 0xcccc);
    break;
  case 3:
    position.x = px(120);
    position.y = 0;
    position.z = px(568);
    Func_02000708(&position, 8, 0x18000);
    break;
  }
}

void Func_02000fac(void) { Event_ResolvePendingActions(0); }

void Func_02000fb8(s32 mode, s32 cue) {
  if (cue == 0) {
    Func_02002e00();
    Task_Remove(Func_0200137c);
  }
  if (mode == 1) {
    MapGrid_CopyCellPictureIndicesRect(13, 26, 13, 16, 3, 3);
    MapGrid_CopyCellAttributes(13, 26, 3, 3, 13, 16);
    GameFlag_Set(0x948);
  }
  if (mode == 2)
    Rocks_Spawn(0xe80000, 0x100000, 0x1180000, 2);
  if (mode == 3)
    Rocks_Spawn(0xe80000, 0x100000, 0x1180000, 30);
  if (cue == 0x3e7)
    Func_02002ebc();
}

const struct Trigger *Func_02001044(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower1)
    return gData_020037f8;
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower2)
    return gData_0200381c;
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower3)
    return gData_0200393c;
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower4)
    return gData_020039e4;
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower5)
    return gData_02003a5c;
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower6)
    return gData_02003b28;
  if (gMainRoutineWork.scene == (s32)&SceneId_TundariaTower7)
    return gData_02003c90;
  return gData_0200381c;
}

void Func_020010d4(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct ReflectionAnimation *leaderAnimation = leader->sprite;
  struct ReflectionAnimation *animation = object->sprite;

  if (leader->z <= object->unk4c) {
    object->z = 0;
    object->x = 0;
    REG_BG2CNT &= 0xfffc;
    REG_BG2CNT |= 1;
  } else {
    REG_BG2CNT &= 0xfffc;
    REG_BG2CNT |= 2;
    if (GameFlag_Get(FLAG_SCENE_TEMP_12)) {
      object->z = 0;
      object->x = 0;
    } else {
      struct ReflectionSprite *sprite = animation->sprite;
      struct ReflectionSprite *source;

      object->x = leader->x;
      object->y = leader->y;
      object->z = object->unk4c - (leader->z - object->unk4c);
      object->facing = ~leader->facing;
      source = leaderAnimation->sprite;
      sprite->unk10 = source->unk10;
      sprite->unk02 = source->unk02;
      sprite->unk14 = source->unk14;
    }
  }
}
#undef FIELD_AT

#include "../field/field_effect/advance_motion.c"

/* VIEW(events_f): routines Func_020010d4, Func_02000404, Func_020007dc,
 * Func_02000f0c and Func_02000fb8 as module/scene/tundaria_tower.h declares
 * them; the scene before defines them otherwise. */
void Func_020010d4__events_f(void) __asm__("Func_020010d4");
void Func_02000404__events_f(const u8 *data) __asm__("Func_02000404");
void Func_020007dc__events_f(void) __asm__("Func_020007dc");
void Func_02000f0c__events_f(void) __asm__("Func_02000f0c");
void Func_02000fb8__events_f(void) __asm__("Func_02000fb8");
#define Func_020010d4 Func_020010d4__events_f
#define Func_02000404 Func_02000404__events_f
#define Func_020007dc Func_020007dc__events_f
#define Func_02000f0c Func_02000f0c__events_f
#define Func_02000fb8 Func_02000fb8__events_f

struct Actor;
struct Particle;
struct ParticleSprite;
union FieldObject;

void TundariaTower_StopLeader(void);

s32 Field_CheckSpheresOverlap(s32 *a, s32 aRadius, s32 *b, s32 bRadius);

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

s32 Actor_GetCellIndex(struct Actor *object);
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 Field_TryJumpForward(void);
struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

void Func_020029d0(s32 spread);
void Func_02002ca8(s32 a, s32 b);

void Func_02002b04(void);

void Actor_MoveToObject(s32 id, s32 target);
void Map_CenterOnFollowedPoint(void);

extern union FieldObject *Data_02003ccc;

void Func_020010d4(void);
void Func_02000548(void);
void Func_0200058c(void);
void Func_02000404(const u8 *data);

extern s16 gTundariaTower_SlideAngles[];
extern s16 gTundariaTower_SlideAnglesByKey[];

extern u32 gData_020032d4[];

extern u32 gData_02003304[];

void EffectSprite_DrawAtWorldPosition(struct ParticleSprite *sprite,
                                      struct Particle *particle);
void BattleEffect_StepVector3(struct Particle *particle, s32 unk1, s32 step);

void Func_020007dc(void);
void Func_02000920(void);
void Func_0200095c(void);
void Func_02000964(void);
void Func_02000b34(s32 arg0, s32 arg1);
void Func_02000d2c(void);
void Func_02000de4(void);
void Func_02000e78(void);
void Func_02000f00(void);
void Func_02000f0c(void);
void Func_02000fac(void);
void Func_02000fb8(void);

extern u32 gData_020031b0[];
extern u32 gData_020031ec[];
extern u32 gData_02003228[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void Func_020011e0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  s32 angle;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveToFixed(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  leader->y = px(130);
  leader->gravity = px(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = 3;
  Event_Wait(24);
  options.palette = 7;
  options.update = (void *)FieldEffect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  i = 0;
  do {
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14 >> 16) - 0x6000;
    velocity[2] += ((Random16() * 3) << 13 >> 16) - 0x3000;
    FieldEffect_Spawn(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                      velocity[2], 0x01090001, &options);
    i++;
  } while (i <= 16);
  Sound_Play(SE_THUMP);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = px(1);
  leader->bounce = Q16_ONE / 4;
  gFieldWork->transition = 0x204;
  Event_End();
}

void Func_0200137c(void) {
  if ((Actor_GetById(gMainRoutineWork.currentOwner)->y >> 19) != 0)
    MapCell_SetAttributes(0, 14, 17, 0);
  else
    MapCell_SetAttributes(0, 14, 17, 0x3200);
}

extern s16 gData_02003340[];
extern s16 gData_0200335a[];
void Actor_PlaceBlocks(const s16 *data);

s32 Scene_Initialize(void) {
  struct Actor *object;
  s32 scene;
  s32 flag;

  gFieldWork->transition = 0x204;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_TundariaTower1) {
  } else if (scene == (s32)&SceneId_TundariaTower2) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
      if (GameFlag_Get(0x942))
        Func_02000548();
      if (GameFlag_Get(0x943))
        Func_0200058c();
      break;
    case 11:
      flag = GameFlag_Get(0x945);
      if (flag == 0) {
        object = Actor_GetById(8);
        Actor_SetSpriteFlags(Actor_GetById(8), 0);
        object->collisionFlags = 0;
        object->motionFlags = 0;
        object->terrainHeight = px(8);
        object->y = px(8);
        Actor_SetItemIcon(object, 441);
        if (GameFlag_Get(0x944)) {
          MapGrid_CopyCellsTo(31, 85, 20, 85, 3, 4);
          MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
          Actor_SetPosition(68, 344, 372);
          Actor_GetById(68)->update = (void (*)(void))Func_02000840;
          Actor_SetPosition(8, 0, 0);
        }
      } else if (GameFlag_Get(0x944)) {
        MapGrid_CopyCellsTo(31, 85, 20, 85, 3, 4);
        MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
        Actor_SetPosition(8, 0, 0);
      }
      break;
    }
  } else if (scene == (s32)&SceneId_TundariaTower4) {
    switch (gMainRoutineWork.entrance) {
    case 6:
    case 7:
      if (!GameFlag_Get(100)) {
        if (!GameFlag_Get(FLAG_SCENE_TEMP_00)) {
          Actor_SetPosition(8, 344, 536);
          MapCell_SetAttributes(0, 21, 33, 0x3300);
          MapCell_SetAttributes(0, 22, 33, 0xff00);
        } else if (!GameFlag_Get(FLAG_SCENE_TEMP_01)) {
          MapCell_SetAttributes(0, 26, 33, 0xff00);
        }
        Actor_WatchAndSetScript(8, gMainRoutineWork.currentOwner);
      }
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
          gMainRoutineWork.entrance == 7)
        Func_020011e0();
      break;
    }
  } else if (scene == (s32)&SceneId_TundariaTower5) {
    Func_02000404(gData_02003340);
  } else if (scene == (s32)&SceneId_TundariaTower6) {
    switch (gMainRoutineWork.entrance) {
    case 3:
      object = FieldActor_Create(0x11e, px(472), 0, px(200));
      if (object != NULL) {
        Scene_SetActorSpritePriority((s32)object, 1);
        object->motionFlags = 0;
        Actor_SetSpriteFlags(object, 0);
        Actor_SetSpriteColor(object, 14);
        Actor_SetBlendMode(object, 1);
      }
      break;
    case 1:
    case 2:
      Func_02000404(gData_0200335a);
      if (GameFlag_Get(0x947)) {
        Actor_SetPosition(13, 216, 136);
        if (GameFlag_Get(0x946))
          Actor_SetPosition(12, 200, 136);
      }
      Actor_PlaceBlocks(gData_02003264);
      Actor_SetSpritePriority(12, 2);
      break;
    case 4:
    case 5:
      if (GameFlag_Get(0x94b)) {
        MapGrid_CopyCellsTo(18, 97, 6, 97, 3, 4);
        MapGrid_CopyCellsTo(82, 34, 70, 34, 3, 2);
        MapGrid_CopyCellAttributes(7, 36, 1, 1, 7, 35);
      }
      {
        union FieldObject **slot = &Data_02003ccc;
        struct Actor *effect = ((struct Actor *)Import_ActorCreate(4, 0, 0, 0));

        effect->unk4c = 144 << 18;
        effect->update = Func_020010d4;
        *slot = (union FieldObject *)effect;
        effect->motionFlags = 0;
        Scene_SetActorSpritePriority(effect, 3);
      }
      break;
    case 6:
    case 7: {
      union FieldObject **slot = &Data_02003ccc;
      struct Actor *effect = ((struct Actor *)Import_ActorCreate(4, 0, 0, 0));

      effect->unk4c = 156 << 18;
      effect->update = Func_020010d4;
      *slot = (union FieldObject *)effect;
      effect->motionFlags = 0;
      Scene_SetActorSpritePriority(effect, 3);
    } break;
    case 10:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        Func_020011e0();
      break;
    }
  } else if (scene == (s32)&SceneId_TundariaTower7) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    if (GameFlag_Get(0x948)) {
      MapGrid_CopyCellsTo(13, 26, 13, 16, 3, 3);
      MapGrid_CopyCellAttributes(13, 26, 3, 3, 13, 16);
    } else {
      Task_Add(Func_0200137c, 0x480);
    }
    Actor_PlaceBlocks(gData_0200326e);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Actor_SelectAnimation(struct Actor *object, s32 mode);

#include "shared/actor_steer_by_pad.h"

void TundariaTower_StopLeader(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (work->busy != 0) {
    Event_SetFlag(0x106);
    Leader_StopMoving();
    Event_ResolveAllPendingActions();
    GameFlag_Clear(FLAG_MENU_OPEN);
  }
}

#include "shared/leader_slide_on_ice.h"

extern s16 gTundariaTower_SlideAnglesByKey[];
void TundariaTower_KeepSliding(struct Actor *object);
void TundariaTower_StopLeader(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
s32 Actor_HasArrived(struct Actor *object);
s32 Field_CheckSpheresOverlap(s32 *a, s32 aRadius, s32 *b, s32 bRadius);

#define SLIDE_FROM_EDGE TundariaTower_SlideOnIceFromEdge
#define SLIDE_STOPPED gTundariaTower_SlideStopped
#define SLIDE_STOP_LEADER TundariaTower_StopLeader
#define SLIDE_ANGLES gTundariaTower_SlideAnglesByKey
#define SLIDE_SET_ANIMATION Actor_SelectAnimation
#define SLIDE_HAS_ARRIVED Actor_HasArrived
#define SLIDE_KEEP_SLIDING TundariaTower_KeepSliding
#define SLIDE_SPHERES_OVERLAP Field_CheckSpheresOverlap
#include "shared/slide_on_ice_from_edge.c"
#undef SLIDE_FROM_EDGE
#undef SLIDE_STOPPED
#undef SLIDE_STOP_LEADER
#undef SLIDE_ANGLES
#undef SLIDE_SET_ANIMATION
#undef SLIDE_HAS_ARRIVED
#undef SLIDE_KEEP_SLIDING
#undef SLIDE_SPHERES_OVERLAP

s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

void TundariaTower_SetSlideStopped(s32 value) {
  gTundariaTower_SlideStopped = value;
}

void Actor_SelectAnimation(struct Actor *object, s32 mode);

#include "shared/leader_slide_on_ice_ahead.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Func_02002094(u16 *table) {
  u16 *row;
  s32 id;
  struct Actor *object;
  s16 x, z;

  if (table[0] == 0xffff)
    return;
  row = table;
loop:
  id = row[0];
  object = Actor_GetById(id);
  x = row[1];
  z = row[2];

  object->collisionFlags &= 0xfe;
  Actor_SetSpriteFlags(object, 0);
  Actor_SetCellOpen(id, (u16)x, (u16)z, BYTE_AT(object, 34));
  row += 3;
  if (row[0] != 0xffff)
    goto loop;
}

// Finds the current target's table row and shrinks it.
void Func_02002100(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  if (*list == 0xffff)
    return;
top:
  {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_GetById(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        GameFlag_Set(flag);
        Actor_SetCellOpen(id, a, flag, object->terrainId);
        object->scaleY = Q16_ONE;
      }
      return;
    }
  }
  list += 3;
  if (*list != 0xffff)
    goto top;
}

#include "shared/actor_set_cell_open.h"

// On entering, sets each listed pillar from its two flags (pushed: moved to its
// place; else its cell raised and its shape redrawn), then lifts the leader
// onto the ground under it.

// On entering, sets each listed pillar from its two flags (pushed: moved to
// its place; else its cell raised and its shape redrawn), then lifts the
// leader onto the ground under it.
void Func_0200222c(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  struct Actor *leader;
  s32 ground;

  if (*table == 0xffff)
    goto done;
next:
  id = table[0];
  object = Actor_GetById(id);
  flag = table[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = Ram_MapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  if (GameFlag_Get(nextFlag)) {
    Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Actor_GetCellIndex(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = MapGrid_GetTerrainKind(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Actor_DisableById(id);
    cell->attributeB = 128;
    *slot =
        MapCollision_FindOrAddTag(object->terrainId, shape, (height >> 19) + 4);
    if (GameFlag_Get(flag))
      Actor_SelectAnimation(object, 0);
  }
  table += 2;
  if (*table != 0xffff)
    goto next;
done:
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  ground = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < ground) {
    leader->terrainHeight = ground;
    leader->y = ground;
  }
}

void Func_02002354(const u16 *table) {
  struct Actor *leader;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0)
    goto test;
  return;
next:
  id = table[0];
  if (Actor_GetById(id) != trap) {
    table += 2;
    goto test;
  }
  {
    flag = table[1];
    Event_Start();
    if (!GameFlag_Get(flag)) {
      Sound_Play(SE_CHEST_CLOSE);
      Actor_SetSpriteColor(Actor_GetById(id), 7);
      wait(2);
      Actor_SelectAnimation(trap, 0);
      Actor_SetSpriteColor(Actor_GetById(id), 0);
      wait(2);
      Actor_SetSpriteColor(Actor_GetById(id), 7);
      wait(4);
      Actor_SetSpriteColor(Actor_GetById(id), 0);
      Func_020029d0(0);
      GameFlag_Set(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Get(second)) {
        leader->motionFlags = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Func_020029d0(0);
        wait(8);
        Actor_SelectAnimation(trap, 2);
        Func_02002ca8(-2, trap->terrainId);
        Func_020029d0(1);
        wait(16);
        Func_02002ca8(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(trap, 0, 0, 0);
        wait(2);
        Sound_Play(SE_THUMP);
        Func_02002b04();
        wait(20);
        GameFlag_Set(second);
      }
    }
    Event_End();
    return;
  }
test:
  if (*table != 0xffff)
    goto next;
}

void Func_020024e8(u16 *row, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 flag;
  s32 nextFlag;

  if (*row == 0xffff)
    return;
loop:
  if (*row != id) {
    row += 2;
    goto test;
  }
  flag = row[1];
  Event_Start();
  nextFlag = flag + 1;
  if (!GameFlag_Get(nextFlag)) {
    Sound_Play(SE_PUSH);
    TundariaTower_SetWork(0x10000, 0x40000, 0x10000);
    Func_020029d0(0);
    wait(8);
    Actor_SelectAnimation(object, 2);
    Actor_SetCellShape(-4, object->terrainId, id);
    Func_020029d0(1);
    wait(16);
    TundariaTower_SetWork(-1, -1, 0xe666);
    wait(8);
    Actor_SetPositionAndResetMotion(object, 0, 0, 0);
    wait(2);
    Sound_Play(SE_THUMP);
    Func_02002b04();
    wait(20);
    GameFlag_Set(flag);
    GameFlag_Set(nextFlag);
  }
  Event_End();
  return;
test:
  if (*row != 0xffff)
    goto loop;
}

void Func_020025c8(s32 id) {
  Actor_GetById(id)->unk5c[0] = 3;
  Actor_DisableById(id);
}

// Places the listed actors on their cells.

// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
// it copy actor 9's visual attributes.
void Func_020025e0(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  u8 *grid = gMapShapeGrid; // the shape grid, read once

  Actor_MoveToObject(8, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(10, gMainRoutineWork.currentOwner);
  wait(1);
  Func_020025c8(8);
  Func_020025c8(9);
  Func_020025c8(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  wait(1);
  if (*list == 0xffff)
    goto done;
next:
  id = list[0];
  object = Actor_GetById(id);
  flag = list[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->unk5c[0] = 3;
  if (GameFlag_Get(nextFlag)) {
    Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Actor_GetCellIndex(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = MapGrid_GetTerrainKind(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Actor_DisableById(id);
    cell->attributeB = 128;
    *slot =
        MapCollision_FindOrAddTag(object->terrainId, shape, (height >> 19) + 4);
    if (GameFlag_Get(flag))
      Actor_CopyGraphics(id, 9);
  }
  list += 2;
  if (*list != 0xffff)
    goto next;
done:
  wait(10);
}

void Func_02002748(struct Actor *object, struct Actor *target) {
  Actor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  Actor_SetPositionAndResetMotion(target, 0, 0, 0);
}

void Func_02002764(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;
  u8 *motion;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  partner = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0) {
    goto test;
  next:
    id = table[0];
    if (Actor_GetById(id) != trap) {
      table += 2;
      goto test;
    }
    flag = table[1];
    Event_Start();
    if (!GameFlag_Get(flag)) {
      Actor_SelectAnimation(partner, 1);
      Func_02002748(partner, trap);
      wait(1);
      Sound_Play(SE_CHEST_CLOSE);
      Actor_SetSpriteColor(Actor_GetById(8), 7);
      wait(2);
      Actor_SelectAnimation(partner, 0);
      Actor_CopyGraphics(id, 9);
      Actor_SetSpriteColor(Actor_GetById(8), 0);
      wait(2);
      Actor_SetSpriteColor(Actor_GetById(8), 7);
      wait(4);
      Actor_SetSpriteColor(Actor_GetById(8), 0);
      Func_020029d0(0);
      Func_02002748(trap, partner);
      wait(1);
      GameFlag_Set(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Get(second)) {
        Actor_SelectAnimation(partner, 0);
        Func_02002748(partner, trap);
        motion = &leader->motionFlags;
        wait(1);
        *motion = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Func_020029d0(0);
        wait(8);
        Actor_SelectAnimation(partner, 2);
        Func_02002ca8(-2, trap->terrainId);
        Func_020029d0(1);
        wait(16);
        Func_02002ca8(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        *motion = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        wait(2);
        Sound_Play(SE_THUMP);
        Func_02002b04();
        wait(20);
        GameFlag_Set(second);
      }
    }
    Event_End();
    return;
  test:
    if (*table != 0xffff)
      goto next;
  }
}

#include "../field/field_effect/advance_motion_priority.c"

s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);
void *Sprite_ShareVramCacheEntry(void *src, void *alt);

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void Func_020029d0(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local. */
      s32 y = leader->y - (((Random16() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)Import_ActorCreate(
          285, (i << 17) + leader->x - px(8), y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Func_02002f90(effect, gData_020032d4);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Trig_Cos(i << 13), 0x13333);
        effect->velocity.z = Trig_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

void Func_02002b04(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 state;
  u32 i;
  s32 zero = 0;

  if (leader != NULL) {
    state = 0;
    for (i = 0; i < 17; i++) {
      struct FieldEffect *effect = ((struct FieldEffect *)Import_ActorCreate(
          285, leader->x, leader->y, leader->z));

      if (effect != NULL) {
        s32 angle;

        state = Sprite_ShareVramCacheEntry(effect->sprite, state);
        zero = 0;
        effect->motionFlags = zero;
        effect->collisionFlags = zero;
        Actor_SetSpriteFlags(effect, 0);
        Actor_SelectAnimation(effect, 2);
        Func_02002f90(effect, gData_02003304);
        angle = i << 12;
        effect->velocity.x = Trig_Cos(angle);
        effect->velocity.y = zero;
        effect->velocity.z = Trig_Sin(angle);
        effect->velocity.x += effect->velocity.x >> 1;
        effect->velocity.x += ((Random16() * 3) << 14 >> 16) - 0x6000;
        effect->velocity.z += ((Random16() * 3) << 13 >> 16) - 0x3000;
        effect->spin = ((Random16() << 12) >> 16) - 0x800;
        effect->scaleRateX = zero;
        effect->scaleRateY = zero;
        effect->update = Effect_AdvanceMotionAtLeaderPriority;
        effect->sprite->priority =
            ((struct FieldSprite *)leader->sprite)->priority;
      }
    }
  }
}

#include "shared/actor_set_cell_shape.h"

void Func_02002ca8(s32 a, s32 b) {
  Actor_SetCellShape(a, b, gMainRoutineWork.currentOwner);
}

void Func_02002cc0(void) {
  struct ParticleWork *work = Ram_HeapSlots->menuRuntime;
  struct Particle *particle = work->particles;
  struct ParticleSprite *sprite = work->sprites;
  s32 i;
  s32 step = -0x8000;

  i = 63;
next:
  if ((u32)particle->age <= 19) {
    u16 *base = &work->tile;
    s32 mask = 7;
    s32 tile = (u16)((*base + ((particle->age >> 2) & mask) * 8) & 0x3ff);
    s32 attr = sprite->attr;

    sprite->attr = (attr & ~0x3ff) | tile;
    EffectSprite_DrawAtWorldPosition(sprite, particle);
    BattleEffect_StepVector3(particle, 63, step);
    particle->age++;
  }
  i--;
  sprite++;
  particle++;
  if (i >= 0)
    goto next;
}

// A falling rock's sprite.
/* VIEW(s29d0): tag struct RockSprite as struct RockSprite__s29d0; module/tla.h
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja (tundaria_tower.c):
 * games/tla/src/scene/tundaria_tower.c:1586: structure has no member named
 * `blend'. */
struct RockSprite__s29d0 {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 blend : 1;
  u8 unk05_6 : 2;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x14];
  u16 unk1e;
  u8 unk20[8];
};

// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite__s29d0 sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

// Spawns count rocks around (x, y, z), each pushed out at a random angle and
// thrown up and out along it.
#include "shared/rocks_spawn.h"

// The falling rocks' setup, and Func_02002ebc, which stops their task and
// frees their picture and heap slot.

// The falling rocks' setup: loads their picture, sets up 64 small sprites,
// marks each rock idle and starts the task that spawns them.
void Func_02002e00(void) {
  struct RockWork *work = HeapSlot_Alloc(220, sizeof(struct RockWork));
  s32 resource;
  s32 tile;
  struct Rock *rock;
  struct RockSprite__s29d0 *sprite;
  s32 i;

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_FallingRock),
                      work);
  resource = VramCache_FindFreeEntry();
  tile = Vram_Load(resource, sizeof(work->tiles), work);
  work->tileBase = tile;
  work->resource = resource;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--) {
    EffectSprite_Init(sprite, 8, 8, 0x40000000, tile);
    sprite->blend = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
    sprite++;
    rock++;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  Task_AddFrame(Func_02002cc0, 0x480);
}

void Func_02002ebc(void) {
  struct TundariaTowerSparkWork *work = Ram_HeapSlots->menuRuntime;

  Task_Remove(Func_02002cc0);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(220);
}

void Func_02000280(void);
void Func_020005d0(void);
void Func_020008a8(void);
void Func_02000b3c(void);
void Func_02000bfc(void);

// clang-format off
u32 gData_020031b0[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020031ec[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003228[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gData_02003264[] = {
    11, 12, 13, 14, -1,
};

s16 gData_0200326e[] = {
    8, -1,
};

s16 gTundariaTower_SlideAngles[] = {                                                               
      -1,     0,      0x8000, -1,     0xc000, 0xe000, 0xa000, 0xc000,          
      0x4000, 0x2000, 0x6000, 0x4000, -1,     0,      0x8000, -1,              
      -1,     0,      0x8000, -1,     0xc000, -1,     -1,     0xc000,          
      0x4000, -1,     -1,     0x4000, -1,     0,      0x8000, -1,              
  };

s16 gTundariaTower_SlideAnglesByKey[] = {
    -1, 0, 0x8000, -1, 0xc000, 0, 0x8000, 0xc000,
    0x4000, 0, 0x8000, 0x4000, -1, 0, 0x8000, -1,
    0,
};

u32 gData_020032d4[] = {
    0, 33, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

u32 gData_02003304[] = {
    0, 44, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gData_020031b0,
    gData_020031ec,
    gData_02003228,
};

s16 gData_02003340[] = {
    8, 7, 512, 9, 7, 513, 10, 7,
    514, 11, 7, 515, -1,
};

s16 gData_0200335a[] = {
    8, 7, 512, 9, 7, 513, 10, 7,
    514, -1, 0,
};

u32 gData_02003370[] = {
    22, 3, 0, 0x80010000, 0, 1,
    23, 3, 0x1000, 13, 0xffff, 0x80010000,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_020033d0[] = {
    0x500330, 0x3400210, 0x2200060, 0x6ffff, 0xffff, 0,
    0, 0,
};

u32 gData_020033f0[] = {
    0xf00b0, 0xc000c0, 0xd00021, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(TundariaTower1),
    SCENE_EXIT_UNLESS(1, WorldMap, 46, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTower2, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTower2),
    SCENE_EXIT_UNLESS(1, TundariaTower1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTower2, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TundariaTower2, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TundariaTower2, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TundariaTower6, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TundariaTower2, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, TundariaTower2, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, TundariaTower2, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, TundariaTower2, 18, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, TundariaTower3, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, TundariaTower2, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, TundariaTower6, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, TundariaTower2, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, TundariaTower2, 17, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, TundariaTower2, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, TundariaTower2, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, TundariaTower2, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(18, TundariaTower2, 9, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTower3),
    SCENE_EXIT_UNLESS(1, TundariaTower3, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTower3, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TundariaTower3, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TundariaTower3, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TundariaTower4, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TundariaTower3, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, TundariaTower3, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, TundariaTower3, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, TundariaTower2, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, TundariaTower3, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, TundariaTower3, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, TundariaTower3, 8, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTower4),
    SCENE_EXIT_UNLESS(1, TundariaTower5, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTower3, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TundariaTower5, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TundariaTower5, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TundariaTower4, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TundariaTower4, 5, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTower5),
    SCENE_EXIT_UNLESS(1, TundariaTower5, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTower4, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TundariaTower5, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TundariaTower5, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TundariaTower4, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TundariaTower4, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, TundariaTower6, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, TundariaTower5, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, TundariaTower6, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, TundariaTower6, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, TundariaTower4, 7, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTower6),
    SCENE_EXIT_UNLESS(1, TundariaTower5, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTower5, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TundariaTower7, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TundariaTower2, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TundariaTower2, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TundariaTower7, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, TundariaTower6, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, TundariaTower6, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, TundariaTower5, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, TundariaTower6, 10, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTower7),
    SCENE_EXIT_UNLESS(1, TundariaTower6, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTower6, 3, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02003618[] = {
    {489, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_EAST, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003648[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003660[] = {
    ACTOR_PLACEMENT(245, 0x64, 7, 424, 0, 536, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003690[] = {                                             
      {406, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},                
      {406, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},                
      {406, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_SOUTH, 2, 2},                
      {406, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 2},                
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gData_02003708[] = {                                             
      {406, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},                
      {406, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},                
      {406, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_SOUTH, 2, 2},                
      {272, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 2},                
      {272, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_SOUTH, 2, 2},                
      {272, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_SOUTH, 2, 2},                
      {272, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_SOUTH, 2, 2},                
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gData_020037c8[] = {
    {272, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

struct Trigger gData_020037f8[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gData_0200381c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(Func_020005d0)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(Func_020005d0)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXAMINE, 52, CONDITION_ALWAYS, EVENT(Func_02000920)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 50, 0x942,
     EVENT(Func_02000548)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 51, 0x943,
     EVENT(Func_0200058c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 52, 0x944,
     EVENT(Func_020007dc)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 52,
     0x944, EVENT(Func_020008a8)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_0200393c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020039e4[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {TRIGGER_STEP, 51, 0x200, EVENT(Func_02000964)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003a5c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(Func_02000b3c)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200,
     EVENT(Func_02000b34)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201,
     EVENT(Func_02000b34)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 10, 0x202,
     EVENT(Func_02000b34)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 11, 0x203,
     EVENT(Func_02000b34)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003b28[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(Func_02000f00)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200,
     EVENT(Func_02000b34)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201,
     EVENT(Func_02000b34)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 10, 0x202,
     EVENT(Func_02000b34)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02000bfc)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_02000d2c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_FIRST), 11,
     CONDITION_ALWAYS, EVENT(Func_02000fac)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11,
     CONDITION_ALWAYS, EVENT(Func_02000bfc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS), 11,
     CONDITION_ALWAYS, EVENT(Func_02000c64)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     0x946, EVENT(Func_02000bfc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x946, EVENT(Func_02000d2c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     0x947, EVENT(Func_02000bfc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0x947, EVENT(Func_02000d2c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_FIRST), 14,
     CONDITION_ALWAYS, EVENT(Func_02000fac)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14,
     CONDITION_ALWAYS, EVENT(Func_02000bfc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS), 14,
     CONDITION_ALWAYS, EVENT(Func_02000c64)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Func_02000e78)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Func_02000de4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 52,
     CONDITION_ALWAYS, EVENT(Func_02000fac)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 52, 0x94b,
     EVENT(Func_02000f0c)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003c90[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(Func_02000bfc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS,
     EVENT(Func_02000d2c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50, 0x948,
     EVENT(Func_02000fb8)},
    {TRIGGER_TABLE_END},
};
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef FIELD_AT
#undef BYTE_AT
#undef Func_020010d4
#undef Func_02000404
#undef Func_020007dc
#undef Func_02000f0c
#undef Func_02000fb8
