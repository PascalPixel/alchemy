
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
#include "event_work.h"
#include "system/frame.h"
#include "field/event.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "module/field/field_effect.h"
#include "system/input.h"
#include "version.h"
#include "shared/map_cells.h"
#include "module/field/object.h"
#include "shared/effects.h"
#include "shared/tables.h"
#include "field_sprite.h"
#include "gamepak/far_call/system.h"
#include "field/particle.h"

void Func_02000280(void) {
  GameFlag_Set(513);
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
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom6)
    return (const struct ActorPlacement *)gData_020033d0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom7)
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
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom2)
    return (const struct Trigger *)gData_02003618;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom3)
    return (const struct Trigger *)gData_02003648;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom4)
    return (const struct Trigger *)gData_02003660;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom5)
    return (const struct Trigger *)gData_02003690;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom6)
    return (const struct Trigger *)gData_02003708;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom7)
    return (const struct Trigger *)gData_020037c8;
  return (const struct Trigger *)gData_02003618;
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void TundariaTower_02002fe8(s32 a0, s32 a1, s32 a2, s32 a3,
                                          s32 a4, s32 a5) {
  Map_CopyCellAttributeRect(a0, a1, a2, a3, a4, a5);
}

static inline void TundariaTower_02002fc0(s32 a0, s32 a1, s32 a2, s32 a3,
                                          s32 a4, s32 a5) {
  MapGrid_CopyCellPictureIndicesRect(a0, a1, a2, a3, a4, a5);
}

void Func_02000548(void) {
  Event_Start();
  TundariaTower_02002fe8(19, 55, 7, 8, 24, 46);
  TundariaTower_02002fc0(24, 102, 24, 110, 7, 8);
  GameFlag_Set(2370);
  Event_End();
}

void Func_0200058c(void) {
  Event_Start();
  TundariaTower_02002fe8(37, 55, 7, 8, 32, 46);
  TundariaTower_02002fc0(32, 102, 32, 110, 7, 8);
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
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  s16 *choice;
  s32 x = 37;
  s32 *owner;
  s32 one;
  s32 height;
  s32 cell;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  choice = (s16 *)(work + 340);
  if (*choice == 3)
    x = 25;
  Event_PlaySound(158);
  one = 1;
  height = 2;
  MapGrid_CopyCellsTo(30, 38, x, 49, one, height);
  FieldEvent_Wait(10);
  MapGrid_CopyCellsTo(32, 38, x, 49, one, height);
  FieldEvent_Wait(10);
  owner = (s32 *)&gMainRoutineWork;
  owner += 133;
  Actor_SetSpeed(*owner, 0x8000, 0x4000);
  ((struct Actor *)((s32)Actor_GetById(*owner)))->motionFlags = 0;
  cell = (((struct Actor *)((s32)Actor_GetById(*owner)))->x >> 19) | one;
  Actor_WalkToAndWait(*owner, cell << 3,
                      *(s16 *)((u8 *)((s32)Actor_GetById(*owner)) + 18) | one);
  Event_PlaySound(123);
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
    // One constant for the loop's life: the listing keeps 15 in a register.
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
    position.x = 0x15b0000;
    position.y = 0;
    position.z = 0x1780000;
    Func_02000708(&position, 1, 0xcccc);
    break;
  case 3:
    MapGrid_CopyCellPictureIndicesRect(31, 85, 20, 85, 3, 4);
    position.x = 0x15b0000;
    position.y = 0;
    position.z = 0x1780000;
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
  Call6((void (*)())Map_CopyCellAttributeRect, 11, 22, 3, 2, 20, 22);
  object->gravity = 0x8000;
  object->motionFlags = 3;
  OverlayObject_WaitUntilIdle(object);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(68, PIXELS(344), PIXELS(372));
  Actor_GetById(68)->update = (void (*)(void))Func_02000840;
  Event_End();
}

void Func_02000920(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show(MsgThereSSomething, 1);
  if (GameFlag_Get(0x949)) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    work->unk15a = 1;
  }
  Event_End();
}

void Func_0200095c(void) { Func_0200189c(); }

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
  Sound_Play(132);
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
  GameFlag_Set(0x200);
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
    object->scaleY = 0x10000;
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
  Call4((void (*)())FieldCamera_MoveWithinMapBounds, -1, -1, -1, 0);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  leader->facing = 0x4000;
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Event_Wait(30);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x102);
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
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->scene == (s32)&SceneId_Ov68fRoom6) {
    struct Actor *object;

    Actor_LiftTargetBlock(gData_02003264);
    object = *(struct Actor **)((u8 *)work + 0xcbc);
    object->timer = ((object->x >> 20) << 8) | ((object->z >> 20) & 0xff);
  } else if (party->scene == (s32)&SceneId_Ov68fRoom7) {
    Actor_LiftTargetBlock(gData_0200326e);
  }
}

/// The event's object keeps stepping a cell at a time in the direction it was
/// pushed (its cell against the one Func_02000bfc stored in its action) while
/// the next cell's layer 1 attribute is 50 and its layer 2 attribute is not
/// 255, then settles on the middle of its cell.
void Func_02000c64(s32 unused, s32 actorArg) {
  u8 *work = Ram_HeapSlots->eventWork;
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
  object->speedLimit = 0x8000;
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
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object;

  object = *(struct Actor **)(event + 0xcbc);

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
    object->terrainId = 2;
    object->motionFlags = 3;
    OverlayObject_WaitUntilIdle(object);
    Sound_Play(188);
    OverlayObject_WaitUntilIdle(object);
    object->motionFlags = 0;
    object->terrainId = 0;
  }
  Actor_PlaceMovedBlock();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom6) {
    /* FAKEMATCH(temporary): the copy makes each column test reload X; as
     * object->x, 10 of 76 instructions differ in tla-en. */
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
  GameFlag_Clear(0x212);
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    MapGrid_CopyCellPictureIndicesRect(39, 100, 53, 100, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 43, 41, 3, 3, 53, 37);
    if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 20 == 38) {
      s16 angle = Actor_GetById(gMainRoutineWork.currentOwner)->facing;
      s32 owner = gMainRoutineWork.currentOwner;

      if (angle < 0)
        Actor_SetHorizontalPositionWithTerrain(owner, PIXELS(872), PIXELS(600));
      else
        Actor_SetHorizontalPositionWithTerrain(owner, PIXELS(872), PIXELS(632));
    }
  }
}

void Func_02000e78(void) {
  BlendScript_Suspend();
  GameFlag_Set(0x212);
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    MapGrid_CopyCellPictureIndicesRect(43, 100, 53, 100, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 43, 37, 3, 3, 53, 37);
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
    Call6((void (*)())Map_CopyCellAttributeRect, 7, 36, 1, 1, 7, 35);
    GameFlag_Set(0x94b);
    break;
  case 2:
    position.x = 0x780000;
    position.y = 0;
    position.z = 0x2380000;
    Func_02000708(&position, 1, 0xcccc);
    break;
  case 3:
    position.x = 0x780000;
    position.y = 0;
    position.z = 0x2380000;
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
    Call6((void (*)())Map_CopyCellAttributeRect, 13, 26, 3, 3, 13, 16);
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
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom1)
    return gData_020037f8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom2)
    return gData_0200381c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom3)
    return gData_0200393c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom4)
    return gData_020039e4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom5)
    return gData_02003a5c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom6)
    return gData_02003b28;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom7)
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
    if (GameFlag_Get(0x212)) {
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

// ---- games/tla/src/scene/tundaria_tower/s11e0.c ----
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

/// What The Lost Age's code declares for the files that use it, written once.

struct Actor;
struct Particle;
struct ParticleSprite;
union FieldObject;

struct TundariaTowerEventWork {
  u8 unk000[428];
  s32 screenEffect;
};

void Func_02001868(void);

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

extern s16 gData_02003272[];
extern s16 gData_020032b2[];

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

static inline void TundariaTower_CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_020011e0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  s32 angle;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  TundariaTower_CamBounds(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(204);
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
  Sound_Play(188);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
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

  ((struct TundariaTowerEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x204;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov68fRoom1) {
  } else if (scene == (s32)&SceneId_Ov68fRoom2) {
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
        object->terrainHeight = PIXELS(8);
        object->y = PIXELS(8);
        Actor_SetItemIcon(object, 0x1b9);
        if (GameFlag_Get(0x944)) {
          MapGrid_CopyCellsTo(31, 85, 20, 85, 3, 4);
          MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
          Actor_SetPosition(68, PIXELS(344), PIXELS(372));
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
  } else if (scene == (s32)&SceneId_Ov68fRoom4) {
    switch (gMainRoutineWork.entrance) {
    case 6:
    case 7:
      if (!GameFlag_Get(100)) {
        if (!GameFlag_Get(0x200)) {
          Actor_SetPosition(8, PIXELS(344), PIXELS(536));
          MapCell_SetAttributes(0, 21, 33, 0x3300);
          MapCell_SetAttributes(0, 22, 33, 0xff00);
        } else if (!GameFlag_Get(0x201)) {
          MapCell_SetAttributes(0, 26, 33, 0xff00);
        }
        Actor_WatchAndSetScript(8, gMainRoutineWork.currentOwner);
      }
      if (!GameFlag_Get(0x109) && gMainRoutineWork.entrance == 7)
        Func_020011e0();
      break;
    }
  } else if (scene == (s32)&SceneId_Ov68fRoom5) {
    Func_02000404(gData_02003340);
  } else if (scene == (s32)&SceneId_Ov68fRoom6) {
    switch (gMainRoutineWork.entrance) {
    case 3:
      object = FieldActor_Create(0x11e, PIXELS(472), 0, PIXELS(200));
      if (object != NULL) {
        Call2((void (*)())Scene_SetActorSpritePriority, (s32)object, 1);
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
        Actor_SetPosition(13, PIXELS(216), PIXELS(136));
        if (GameFlag_Get(0x946))
          Actor_SetPosition(12, PIXELS(200), PIXELS(136));
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
      if (!GameFlag_Get(0x109))
        Func_020011e0();
      break;
    }
  } else if (scene == (s32)&SceneId_Ov68fRoom7) {
    GameFlag_Set(0x144);
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

/* VIEW(s11e0): variable gInput as extern u32 gInput; input.h has extern struct
 * InputState gInput; tried as the rest of the object declares it, it does not
 * compile in tla-ja: games/tla/src/scene/tundaria_tower.c:995: invalid operands
 * to binary >>. */
extern u32 gInput__s11e0 __asm__("gInput");
void Actor_SelectAnimation(struct Actor *object, s32 mode);

void Func_020017dc(struct Actor *object) {
  s32 angle;
  s32 turn;

  if (object->timer != 0) {
    object->timer--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (GameFlag_Get(0x106))
    angle = -1;
  else
    angle = gData_02003272[(gInput__s11e0 >> 4) & 15];
  if (angle == -1) {
    Actor_SelectAnimation(object, 9);
    return;
  }
  turn = (s16)(angle - object->facing);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->facing += turn;
  Actor_SelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}

void Func_02001868(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (work->actionCounter != 0) {
    Call1((void (*)())GameFlag_Set, 0x106);
    Leader_StopMoving();
    Event_ResolveAllPendingActions();
    GameFlag_Clear(0x106);
  }
}

void Func_0200189c(void) {
  struct Actor *object;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 x;
  s32 z;
  u8 *layer;
  s32 radius;
  s32 position[3];
  s32 dx, dz;
  s32 cell;
  s16 angle;
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *p;
  s32 *q;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
retry:
  Func_02001868();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  p = position;
  p[0] = x = (object->x & 0xfff00000) + 0x80000;
  p[1] = object->y;
  p[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetZ = z;
  targetX = x;
  layer = &object->terrainId;
  startCell = MapGrid_GetCellBehaviorAt(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_020032b2[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff)
      return;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse's hoist of the
   * position's address; as no asm, 2 of 304 instructions differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  Vector_AddPolarOffset(0x100000, angle, position);
  cell = MapGrid_GetCellBehaviorAt(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000) {
    object->facing = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  p[0] = x;
  p[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->timer = 0;
  Actor_SetMoveTarget(object, x, object->y, z);
  Actor_SelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  Actor_WaitForMove(object);
  object->update = Func_020017dc;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    if (Value4((s32 (*)())Field_CheckSpheresOverlap, (s32)&entry->x, radius,
               (s32)position, entry->radius - 2) >= 0)
      goto blocked;
  }
  j++;
  flags += sizeof(*entry);
  entry++;
  if (j <= 63)
    goto overlap;
  targetX = q[0];
  targetZ = q[2];
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  Actor_SetMoveTarget(object, q[0], q[1], q[2]);
  Actor_WaitForMove(object);
  if (cell != startCell)
    goto moved;
walk:
  /* FAKEMATCH(barrier): as the one above; as no asm, 126 of 304 instructions
   * differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  Call3((void (*)())Vector_AddPolarOffset, 0x100000, angle, (s32)position);
  q = position;
  cell = MapGrid_GetCellBehaviorAt(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Actor_SetMoveTarget(object, targetX, object->y, targetZ);
  Actor_WaitForMove(object);
  wait(2);
  goto retry;
moved:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Actor_SelectAnimation(object, 1);
}

// ---- scene/tundaria_tower/s1b48.c ----

#if TLA_PATCH_1

#include "types.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "system/input.h"
#include "call.h"

/* VIEW(s1b48): variable gData_02003cd0 as volatile s16 gData_02003cd0;
 * scene/shared/tundaria_tower.h has u16 gData_02003cd0; tried as the 2
 * declarations' extern volatile s16 gData_02003cd0, it does not compile in
 * tla-en: games/tla/src/scene/tundaria_tower.c:1157: conflicting types for
 * `gData_02003cd0'. */
extern volatile s16 gData_02003cd0__s1b48 __asm__("gData_02003cd0");
extern s16 gData_020032b2[];
void Func_020017dc(struct Actor *object);
void Func_02001868(void);
s32 MapGrid_GetCellBehaviorAt(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Actor_WaitForMove(struct Actor *actor);
s32 Actor_HasArrived(struct Actor *object);
s32 Field_CheckSpheresOverlap(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
void wait(s32 frames);

void Func_02001b48(void) {
  struct Actor *object;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 x;
  s32 z;
  u8 *layer;
  s32 radius;
  s32 position[3];
  s32 dx, dz;
  s32 cell;
  s16 angle;
  s32 i;
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *q;
  s32 *p;

  /* FAKEMATCH(volatile): gData_02003cd0 read as a volatile s16, reread after
   * each test and for lsls #16; as a plain u16, 54 of 347 instructions differ
   * in tla-en. */
  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gData_02003cd0__s1b48 = 0;
retry:
  Func_02001868();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  p = position;
  p[0] = x = (object->x & 0xfff00000) + 0x80000;
  p[1] = object->y;
  p[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetZ = z;
  targetX = x;
  layer = &object->terrainId;
  startCell = MapGrid_GetCellBehaviorAt(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_020032b2[(gInput.held >> 4) & 15];
    // -1 for no direction, tested as its halfword.
    if ((u16)angle == 0xffff)
      goto end;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  /* FAKEMATCH(wrapper): the polar step goes through Call3; as a direct call, 2
   * of 347 instructions differ in tla-en. */
  Call3((void (*)())Vector_AddPolarOffset, 0x100000, angle, (s32)position);
  cell = MapGrid_GetCellBehaviorAt(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000) {
    object->facing = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  p[0] = x;
  p[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->timer = 0;
  Actor_SetMoveTarget(object, x, object->y, z);
  Actor_SelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  for (i = 0; i <= 179 && Actor_HasArrived(object) == 0; i++) {
    if (gData_02003cd0__s1b48 != 0)
      goto end;
    wait(1);
  }
  object->update = (void (*)(void))Func_020017dc;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  if (gData_02003cd0__s1b48 != 0)
    goto end;
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    /* FAKEMATCH(wrapper): the overlap call goes through Value4; as a direct
     * call, 2 of 347 instructions differ in tla-en. */
    if (Value4((s32 (*)())Field_CheckSpheresOverlap, (s32)&entry->x, radius,
               (s32)position, entry->radius - 2) >= 0)
      goto blocked;
  }
  j++;
  flags += sizeof(*entry);
  entry++;
  if (j <= 63)
    goto overlap;
  targetX = q[0];
  targetZ = q[2];
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  Actor_SetMoveTarget(object, q[0], q[1], q[2]);
  i = 0;
  goto test;
next:
  wait(1);
  i++;
test:
  if (i <= 179 && Actor_HasArrived(object) == 0 && gData_02003cd0__s1b48 == 0)
    goto next;
  if (gData_02003cd0__s1b48 != 0)
    goto end;
  if (cell != startCell)
    goto changed;
walk:
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse hoisting the
   * position's address; as no asm, 94 of 347 instructions differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  /* FAKEMATCH(wrapper): the polar step goes through Call3; as a direct call, 6
   * of 347 instructions differ in tla-en. */
  Call3((void (*)())Vector_AddPolarOffset, 0x100000, angle, (s32)position);
  q = position;
  cell = MapGrid_GetCellBehaviorAt(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Actor_SetMoveTarget(object, targetX, object->y, targetZ);
  Actor_WaitForMove(object);
  wait(2);
  goto retry;
changed:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Actor_SelectAnimation(object, 1);
}

#else

// The Japanese release's older form.

#include "types.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "system/input.h"
#include "call.h"

extern volatile s16 gData_02003cd0__s1b48 __asm__("gData_02003cd0");
extern s16 gData_020032b2[];
void Func_020017dc(struct Actor *object);
void Func_02001868(void);
s32 MapGrid_GetCellBehaviorAt(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
s32 Actor_HasArrived(struct Actor *object);
s32 Field_CheckSpheresOverlap(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
void wait(s32 frames);

void Func_02001b48(void) {
  struct Actor *object;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 x;
  s32 z;
  u8 *layer;
  s32 radius;
  s32 position[3];
  s32 dx, dz;
  s32 cell;
  s16 angle;
  s32 i;
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *q;
  s32 *p;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gData_02003cd0__s1b48 = 0;
retry:
  Func_02001868();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  p = position;
  p[0] = x = (object->x & 0xfff00000) + 0x80000;
  p[1] = object->y;
  p[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetZ = z;
  targetX = x;
  layer = &object->terrainId;
  startCell = MapGrid_GetCellBehaviorAt(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_020032b2[(gInput.held >> 4) & 15];
    // -1 for no direction, tested as its halfword.
    if ((u16)angle == 0xffff)
      return;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  /* FAKEMATCH(barrier): an empty asm clobbering sfp keeps the first step's
   * position address from reusing p; as no asm, 2 of 332 instructions differ
   * in tla-ja. */
  __asm__("" : : : SFP_CLOBBER);
  Vector_AddPolarOffset(0x100000, angle, position);
  cell = MapGrid_GetCellBehaviorAt(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000) {
    object->facing = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  p[0] = x;
  p[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->timer = 0;
  Actor_SetMoveTarget(object, x, object->y, z);
  Actor_SelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  Actor_WaitForMove(object);
  object->update = (void (*)(void))Func_020017dc;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  if (gData_02003cd0__s1b48 != 0)
    goto blocked;
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    /* FAKEMATCH(wrapper): the overlap call goes through Value4; as a direct
     * call, 2 of 332 instructions differ in tla-ja. */
    if (Value4((s32 (*)())Field_CheckSpheresOverlap, (s32)&entry->x, radius,
               (s32)position, entry->radius - 2) >= 0)
      goto blocked;
  }
  j++;
  flags += sizeof(*entry);
  entry++;
  if (j <= 63)
    goto overlap;
  targetX = q[0];
  targetZ = q[2];
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  i = 0;
  Actor_SetMoveTarget(object, q[0], q[1], q[2]);
  goto test;
next:
  wait(1);
  i++;
test:
  if (i <= 179 && Actor_HasArrived(object) == 0 && gData_02003cd0__s1b48 == 0)
    goto next;
  if (gData_02003cd0__s1b48 != 0)
    goto end;
  if (cell != startCell)
    goto changed;
walk:
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse hoisting the
   * position's address; as no asm, 128 of 332 instructions differ in tla-ja. */
  __asm__("" : : : SFP_CLOBBER);
  /* FAKEMATCH(wrapper): the polar step goes through Call3; as a direct call, 6
   * of 332 instructions differ in tla-ja. */
  Call3((void (*)())Vector_AddPolarOffset, 0x100000, angle, (s32)position);
  q = position;
  cell = MapGrid_GetCellBehaviorAt(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Actor_SetMoveTarget(object, targetX, object->y, targetZ);
  Actor_WaitForMove(object);
  wait(2);
  goto retry;
changed:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Actor_SelectAnimation(object, 1);
}

#endif

// ---- scene/tundaria_tower/events_f.c ----

s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

void Func_02001e5c(s32 value) { gData_02003cd0 = value; }

void Actor_SelectAnimation(struct Actor *object, s32 mode);

void Func_02001e68(void) {
  struct Actor *object;
  s16 angle;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 radius;
  s32 position[3];
  s32 *p;
  s32 x;
  s32 z;
  u8 *layer;
  s32 cell;
  s32 *q;
  struct Actor *entry;
  s32 j;
  u8 *flags;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  angle = (object->facing + 0x2000) & 0xc000;
retry:
  Func_02001868();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  p = position;
  p[0] = x = (object->x & 0xfff00000) + 0x80000;
  p[1] = object->y;
  p[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetX = x;
  targetZ = z;
  layer = &object->terrainId;
  startCell = MapGrid_GetCellBehaviorAt(*layer, x, z);
  Vector_AddPolarOffset(0x100000, angle, p);
  cell = MapGrid_GetCellBehaviorAt(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000) {
    object->facing = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  p[0] = x;
  p[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->timer = 0;
  Actor_SelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  object->update = Func_020017dc;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    if (Value4((s32 (*)())Field_CheckSpheresOverlap, (s32)&entry->x, radius,
               (s32)position, entry->radius - 2) >= 0)
      goto blocked;
  }
  j++;
  flags += sizeof(*entry);
  entry++;
  if (j <= 63)
    goto overlap;
  targetX = q[0];
  targetZ = q[2];
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  Actor_SetMoveTarget(object, q[0], q[1], q[2]);
  Actor_WaitForMove(object);
  if (cell != startCell)
    goto moved;
walk:
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse hoisting the
   * position's address; as no asm, 43 of 246 instructions differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  Call3((void (*)())Vector_AddPolarOffset, 0x100000, angle, (s32)position);
  q = position;
  cell = MapGrid_GetCellBehaviorAt(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Actor_SetMoveTarget(object, targetX, object->y, targetZ);
  Actor_WaitForMove(object);
  wait(2);
  goto retry;
moved:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Actor_SelectAnimation(object, 1);
}

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
  // A goto loop: as a do-while loop, 42 of 61 instructions differ in tla-en.
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
        object->scaleY = 0x10000;
      }
      return;
    }
  }
  list += 3;
  if (*list != 0xffff)
    goto top;
}

DEFINE_ACTOR_SET_CELL_OPEN(Actor_SetCellOpen, Actor_GetCellIndex,
                           Actor_SelectAnimation)

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
      Sound_Play(125);
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
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_020029d0(0);
        wait(8);
        Actor_SelectAnimation(trap, 2);
        Func_02002ca8(-2, trap->terrainId);
        Func_020029d0(1);
        wait(16);
        Func_02002ca8(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(trap, 0, 0, 0);
        wait(2);
        Sound_Play(188);
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
    Sound_Play(185);
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
    Sound_Play(188);
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
      Sound_Play(125);
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
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_020029d0(0);
        wait(8);
        Actor_SelectAnimation(partner, 2);
        Func_02002ca8(-2, trap->terrainId);
        Func_020029d0(1);
        wait(16);
        Func_02002ca8(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *motion = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        wait(2);
        Sound_Play(188);
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

// ---- scene/tundaria_tower/s29d0.c ----

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
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)Import_ActorCreate(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
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
  // Default: without it the set in the loop is hoisted.
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

DEFINE_ACTOR_SET_CELL_SHAPE(Actor_SetCellShape, MapGrid_GetTerrainKind,
                            MapCollision_FindOrAddTag)

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
 * it does not compile in tla-ja: games/tla/src/scene/tundaria_tower.c:2168:
 * structure has no member named `blend'. */
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
DEFINE_ROCKS_SPAWN(Rocks_Spawn)

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

DEFINE_G_DATA_02002276(gData_02003272)

s16 gData_020032b2[] = {
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
    SCENE_EXITS(206),
    SCENE_EXIT_IF(1, 2, 46, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 207, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(207),
    SCENE_EXIT_IF(1, 206, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 207, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 207, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 207, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 211, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 207, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(7, 207, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(8, 207, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(9, 207, 18, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(10, 208, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(11, 207, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(12, 211, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(13, 207, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(14, 207, 17, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(15, 207, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(16, 207, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(17, 207, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(18, 207, 9, EXIT_FLAG_NONE),
    SCENE_EXITS(208),
    SCENE_EXIT_IF(1, 208, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 208, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 208, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 208, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 209, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 208, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(7, 208, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(8, 208, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(9, 207, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(10, 208, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(11, 208, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(12, 208, 8, EXIT_FLAG_NONE),
    SCENE_EXITS(209),
    SCENE_EXIT_IF(1, 210, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 208, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 210, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 210, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 209, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 209, 5, EXIT_FLAG_NONE),
    SCENE_EXITS(210),
    SCENE_EXIT_IF(1, 210, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 209, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 210, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 210, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 209, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 209, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(7, 211, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(8, 210, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(9, 211, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(10, 211, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(11, 209, 7, EXIT_FLAG_NONE),
    SCENE_EXITS(211),
    SCENE_EXIT_IF(1, 210, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 210, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 212, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 207, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 207, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 212, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(7, 211, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(8, 211, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(9, 210, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(10, 211, 10, EXIT_FLAG_NONE),
    SCENE_EXITS(212),
    SCENE_EXIT_IF(1, 211, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 211, 3, EXIT_FLAG_NONE),
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
    {245, 0x64, 7, PIXELS(424), 0, PIXELS(536), FACING_WEST, 0, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_02003690(gData_02003690, 406, 1, 2, 406, 406, 3, 406, 4)

DEFINE_G_DATA_020013D8(gData_02003708, 406, 1, 2, 3, 272, 4, 5, 6, 7)

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
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_020005d0)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_020005d0)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {33, 10, CONDITION_ALWAYS, 10},
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
    {33, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {49, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020039e4[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {33, 3, CONDITION_ALWAYS, 3},
    {33, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {TRIGGER_STEP, 51, 0x200, EVENT(Func_02000964)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003a5c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {33, 4, CONDITION_ALWAYS, 4},
    {49, 5, CONDITION_ALWAYS, 5},
    {49, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {49, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {33, 10, CONDITION_ALWAYS, 10},
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
    {49, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(Func_02000f00)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200,
     EVENT(Func_02000b34)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201,
     EVENT(Func_02000b34)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 10, 0x202,
     EVENT(Func_02000b34)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02000bfc)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000d2c)},
    {0x80008c15, 11, CONDITION_ALWAYS, EVENT(Func_02000fac)},
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
    {0x80008c15, 14, CONDITION_ALWAYS, EVENT(Func_02000fac)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14,
     CONDITION_ALWAYS, EVENT(Func_02000bfc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS), 14,
     CONDITION_ALWAYS, EVENT(Func_02000c64)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Func_02000e78)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Func_02000de4)},
    {0x80009705, 52, CONDITION_ALWAYS, EVENT(Func_02000fac)},
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
