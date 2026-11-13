// ---- games/tla/src/field/map/cell_set.c ----
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

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

// ---- games/tla/src/scene/tundaria_tower.c ----
#include "../../scene/tundaria_tower/tundaria_tower.h"

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Func_02003030(s32 x, s32 z, s32 terrain, s32 height);

/// Sets the cell under the object by the flag its action names: with the flag
/// set, bit 7 of the layer 0 attribute and the high byte of the layer 2
/// attribute are cleared and the sprite goes to priority 3; without it they are
/// set and the sprite goes to priority 2. Func_02003030 then gets the cell, the
/// object's terrain and the terrain height in half cells, 4 lower or higher.
void Func_02000338(struct Actor *object) {
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 attr0 = MapCell_GetAttributes(0, x, z);
  s32 attr2 = MapCell_GetAttributes(2, x, z);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;

  if (GameFlag_Test(object->timer)) {
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
  Func_02003030(x << 20, z << 20, object->terrainId, height);
}

#include "field_event.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

void Func_02002f80();

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

  Actor_SetSingleChildFlags(object, 0);
  Func_02002f80(object, (u16)animation + GameFlag_Test((u16)flag));
  object->motionFlags = 0;
  object->collisionFlags &= 0xfe;
  object->timer = flag;
  Func_02000338(object);
  row += 3;
  if (row[0] != 0xffff)
    goto loop;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

#include "scene_ids.h"

extern const u8 gData_020033d0[];
extern const u8 gData_020033f0[];

const struct ActorPlacement *Func_02000488(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom6)
    return (const struct ActorPlacement *)gData_020033d0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom7)
    return (const struct ActorPlacement *)gData_020033f0;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02003618[];
extern const u8 gData_02003648[];
extern const u8 gData_02003660[];
extern const u8 gData_02003690[];
extern const u8 gData_02003708[];
extern const u8 gData_020037c8[];

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
  Map_CopyCellPictureIndicesRect(a0, a1, a2, a3, a4, a5);
}

void Func_02000548(void) {
  Func_02003048();
  TundariaTower_02002fe8(19, 55, 7, 8, 24, 46);
  TundariaTower_02002fc0(24, 102, 24, 110, 7, 8);
  GameFlag_SetBit(2370);
  Func_02003050();
}

void Func_0200058c(void) {
  Func_02003048();
  TundariaTower_02002fe8(37, 55, 7, 8, 32, 46);
  TundariaTower_02002fc0(32, 102, 32, 110, 7, 8);
  GameFlag_SetBit(2371);
  Func_02003050();
}

#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
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

  Func_02003048();
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
  Func_02003050();
}

#include "call.h"
#include "field_effect.h"

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

    options.rotation = (Random16Far() & 15) << 12;
    velocityX = Random16Far();
    // One constant for the loop's life: the listing keeps 15 in a register.
    mask = 15;
    velocityX &= mask;
    velocityX -= 8;
    velocityX <<= 14;
    velocityY = Random16Far() & mask;
    velocityY += 8;
    velocityY <<= 14;
    x = position->x - ((Random16Far() & 31) << 16) + 0xf0000;
    Func_020000b8(x, position->y + ((Random16Far() & mask) << 16), position->z,
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
    Map_CopyCellPictureIndicesRect(31, 85, 20, 85, 3, 4);
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
      /* FAKEMATCH(temporary): a local holding the mask builds it before the
       * random draw; as Random16Far() & 15, 7 of 46 instructions differ in
       * tla-en. */
      s32 mask = 15;

      dx = mask & Random16Far();
    }
    dy = Random16Far() & 7;
    params.type = 673;
    Func_020000b8(object->x + ((dx - 8) << 16), object->y + ((dy - 8) << 16),
                  object->z, 0, 0x8000, phase, 0x100000, &params);
  }
}

void Func_020008a8(void) {
  struct Actor *object = Actor_GetById(8);

  Func_02003048();
  Event_ResolvePendingActions(0);
  GameFlag_SetBit(0x944);
  Call6((void (*)())Map_CopyCellAttributeRect, 11, 22, 3, 2, 20, 22);
  object->gravity = 0x8000;
  object->motionFlags = 3;
  OverlayObject_WaitUntilIdle(object);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(68, PIXELS(344), PIXELS(372));
  Actor_GetById(68)->update = (void (*)(void))Func_02000840;
  Func_02003050();
}

void Func_02000920(void) {
  Func_02003048();
  Event_ResolvePendingActions(0);
  FieldMessage_Show(MsgThereSSomething, 1);
  if (GameFlag_Test(0x949)) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    work->unk15a = 1;
  }
  Func_02003050();
}

void Func_0200189c(void);

void Func_0200095c(void) { Func_0200189c(); }

extern const u8 gData_02003370[];

void Func_02002f90(struct Actor *object, const void *script);

/// Actor 8 is placed and given its script while the screen flashes, and three
/// map cells change.
void Func_02000964(void) {
  struct Actor *object = Actor_GetById(8);

  Func_02003048();
  Event_ResolvePendingActions(0);
  TundariaTower_FadeFrom(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(2);
  Sound_Play(132);
  TundariaTower_SetWork(0x50000, 0x70000, 0x10000);
  TundariaTower_SetWork(-1, -1, 0xe666);
  Event_Wait(2);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(2);
  object->actionFlags = 0;
  object->update = 0;
  Actor_SetDestination(8, 424, 536);
  Func_02002f90(object, gData_02003370);
  Actor_WaitForMoveById(8);
  GameFlag_SetBit(0x200);
  Actor_WatchAndSetScript(8, gMainRoutineWork.currentOwner);
  MapCell_SetAttributes(0, 21, 33, 0x3200);
  MapCell_SetAttributes(0, 22, 33, 0x3200);
  MapCell_SetAttributes(0, 26, 33, 0xff00);
  Func_02003050();
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
    options.rotation = (Random16Far() & 15) << 12;
    velocityX = ((Random16Far() & 15) - 8) << 13;
    Func_020000b8(object->x, object->y - ((arg1 * 3) << 16) + 0x100000,
                  object->z, velocityX, 0, 0, 0x5a0000, &options);
    object->scaleY -= 7936;
  }
  if (arg0 == 1) {
    object->scaleY = 0x10000;
    GameFlag_SetBit(object->timer);
    Func_02000338(Actor_GetById(effectWork[13]));
    Map_CopyCellPictureIndicesRect(67, 23, (object->x >> 20) + 64,
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

  Func_02003048();
  Event_ResolvePendingActions(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  leader->facing = 0x4000;
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Event_Wait(30);
  Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
  Motion_SetVarCbAndRefresh(gMainRoutineWork.currentOwner, 2);
  Event_Wait(20);
  leader->facing = 0xc000;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 5);
  Actor_SetActionById(gMainRoutineWork.currentOwner, 24);
  Event_Wait(40);
  Func_02003030(0x1300000, 0x3900000, 0, -12);
  Actor_GetById(gMainRoutineWork.currentOwner)->bounce = 0;
  Import_EventSetExitRequest(11);
  Func_02003050();
}

void Func_02000bfc(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->scene == (s32)&SceneId_Ov68fRoom6) {
    struct Actor *object;

    Func_02003170(gData_02003264);
    object = *(struct Actor **)((u8 *)work + 0xcbc);
    object->timer = ((object->x >> 20) << 8) | ((object->z >> 20) & 0xff);
  } else if (party->scene == (s32)&SceneId_Ov68fRoom7) {
    Func_02003170(gData_0200326e);
  }
}

void Func_02003178(void);

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
  Func_02003048();
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
    Camera_FollowActor(actor, 1);
    Script_WaitForEventTimeout(object);
    wait(1);
  }
  object->x = ((object->x >> 19) | 1) << 19;
  object->z = ((object->z >> 19) | 1) << 19;
  Func_02003178();
  Func_02003050();
}

#include "io_reg.h"

/// Off layer 2's terrain height, the event's object runs cue 188 on terrain 2
/// with motion flags 3. In room 6, column 27 sets flag 0x947, and column 25
/// with that flag set sets 0x946.
void Func_02000d2c(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object;

  object = *(struct Actor **)(event + 0xcbc);

  Func_02003048();
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
  Func_02003178();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom6) {
    /* FAKEMATCH(temporary): the copy makes each column test reload X; as
     * object->x, 10 of 76 instructions differ in tla-en. */
    struct Actor *o = object;

    if (o->x >> 19 == 27)
      GameFlag_SetBit(0x947);
    if (o->x >> 19 == 25 && GameFlag_Test(0x947))
      GameFlag_SetBit(0x946);
  }
  Func_02003050();
}

void Func_02002fc8(void);

void Func_02000de4(void) {
  Func_02002fc8();
  GameFlag_ClearBit(0x212);
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    Map_CopyCellPictureIndicesRect(39, 100, 53, 100, 3, 3);
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

void Func_02002fd0(void);

void Func_02000e78(void) {
  Func_02002fd0();
  GameFlag_SetBit(0x212);
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    Map_CopyCellPictureIndicesRect(43, 100, 53, 100, 3, 3);
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
    Map_CopyCellPictureIndicesRect(18, 97, 6, 97, 3, 4);
    Map_CopyCellPictureIndicesRect(82, 34, 70, 34, 3, 2);
    Call6((void (*)())Map_CopyCellAttributeRect, 7, 36, 1, 1, 7, 35);
    GameFlag_SetBit(0x94b);
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
    Task_RemoveFar(Func_0200137c);
  }
  if (mode == 1) {
    Map_CopyCellPictureIndicesRect(13, 26, 13, 16, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 13, 26, 3, 3, 13, 16);
    GameFlag_SetBit(0x948);
  }
  if (mode == 2)
    Func_02002d44(0xe80000, 0x100000, 0x1180000, 2);
  if (mode == 3)
    Func_02002d44(0xe80000, 0x100000, 0x1180000, 30);
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

struct ReflectionSprite {
  u8 unk00[2];
  u16 unk02;
  u8 unk04[12];
  s32 unk10;
  u8 unk14;
};

struct ReflectionAnimation {
  u8 unk00[0x28];
  struct ReflectionSprite *sprite;
};

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
    if (GameFlag_Test(0x212)) {
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
