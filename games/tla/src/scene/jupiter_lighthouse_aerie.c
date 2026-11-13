/// @file
#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "field/event_inline.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "event_work.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "system/frame.h"
#include "field_sprite.h"
#include "call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "effect_sprite.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "gamepak/far_call/system.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "graphics/resource.h"
#include "shared/cracked_tile.h"
#include "field/cell_target.h"
#include "system/io_write.h"
#include "shared/tables.h"
#include "module/field/field_effect.h"
#include "objcmd.h"

extern s32 gPaletteCycleFrame;
extern s32 gPaletteCyclePausesInEvents;
extern u16 gPaletteCycleColors[];
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

struct EffectOptions;
struct EffectSprite;
struct LightOam;

void LightSprites_SetPriority(s32 value);

void Func_0200105c(struct Actor *object);

void Func_020017a8(s32 a, s32 b, s32 side);
void LightSprites_InitB(void);
void LightSprites_Add(s32 id, s32 value);

void MapGrid_SetAttributeBAt();

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

extern char ResourceId_GoldenTrident;

void EffectSprite_Init(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                       u32 attributes, s32 tile);

void Func_02001080(void);

extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void Oam_AddSpriteAtDepthClamped(struct LightOam *oam, s32 depth);

extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

struct CellTarget;
struct CellTargetEntry;

void JupiterLighthouse_LoadPalettesZero(s32 mode);

void JupiterLighthouse_PlaceRaisedActors(void);

void Leader_CenterInCell(void);

void CrackedTile_Crack(struct CellTarget *work, struct CellTargetEntry *entry);

void CrackedTile_Fall(struct CellTarget *work, struct CellTargetEntry *entry);

void CrackedTile_DropIn(void);

struct BeamSpark;
struct BeamSprite;
struct FieldEffect;

void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void Func_02002b38(struct BeamSprite *sprite);
void EffectSprite_DrawAtWorldPosition(struct BeamSprite *sprite,
                                      struct BeamSpark *spark);
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(s32 mode) {
  struct Actor *left = Actor_Get(9);
  struct Actor *right = Actor_Get(10);
  struct Actor *rider = Actor_Get(23);
  u8 *sprite = rider->sprite;
  s32 i;

  FieldActor_SetPositionAndResetMotion(rider, rider->x, rider->y, rider->z);
  if (mode == 0 || mode == 2) {
    Event_PlaySound(SE_556);
    for (i = 0; i <= 63; i++) {
      left->y += 0x1999;
      rider->y += 0x7ae;
      // Through a union, so this store may alias the sprite read after it.
      ((union ActorCoordinate *)&rider->x)->value += -0xa3d;
      *(u16 *)(sprite + 18) += 32;
      System_Wait(1);
    }
  } else {
    Event_PlaySound(SE_556);
    for (i = 0; i <= 63; i++) {
      right->y += 0x1999;
      rider->y += 0x7ae;
      rider->x += 0xa3d;
      *(u16 *)(sprite + 18) += 0xffe0;
      System_Wait(1);
    }
  }
  if ((u32)(mode - 2) > 1)
    return;
  FieldEvent_Wait(30);
  Func_0200105c(left);
  Func_0200105c(right);
  Event_PlaySound(SE_522);
  for (i = 255; i >= 0; i--) {
    Actor_Get(9)->y += 0x1999;
    Actor_Get(10)->y += 0x1999;
    rider->y += 0x1999;
    System_Wait(1);
  }
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  FieldEvent_Wait(40);
  Event_PlaySound(MUS_080);
  FieldEvent_Wait(10);
  Sound_WaitForJingle();
  left->motionFlags = 4;
  right->motionFlags = 4;
  rider->motionFlags = 4;
}

void Func_020001b4(void) {
  Event_SetFlag(0xa0a);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Func_020017a8(11, 9, 0);
  FieldEvent_Wait(40);
  if (Event_GetFlag(0xa0b) == 0) {
    Func_02000054(0);
    FieldEvent_Wait(40);
  } else {
    Func_02000054(2);
  }
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldEvent_Wait(40);
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(9)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(10)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(11)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(12)));
  FieldCamera_WaitForArrival();
  Event_End();
}

void Func_02000264(void) {
  Event_SetFlag(0xa0b);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Func_020017a8(12, 10, 1);
  FieldEvent_Wait(40);
  if (Event_GetFlag(0xa0a) == 0) {
    Func_02000054(1);
    FieldEvent_Wait(40);
  } else {
    Func_02000054(3);
  }
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldEvent_Wait(40);
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(9)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(10)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(11)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(12)));
  FieldCamera_WaitForArrival();
  Event_End();
}

void Func_02000314(s32 unused, s32 actor) {
  struct JupiterLighthouseAerieObject *object =
      ((struct JupiterLighthouseAerieObject *)Actor_Get(actor));

  MapGrid_SetCellHeight(object->x, object->z, 0, 4);
}

// Jupiter Lighthouse aerie's two pushed objects: object 13 on cell (6, 12) or
// object 14 on cell (57, 12) sets its flag (2570, 2571), falls into place
// (waited on for at most 30 frames), copies cells
// (MapGrid_CopyCellPictureCellsRect, Map_CopyCellAttributeRect) with sounds 134
// and 167 and runs its part (Func_020001b4, Func_02000264); off its cell it
// only marks the cell it stands on (MapGrid_SetCellHeight). With both flags
// set, flag 2591 is set. The fall wait is an if around a do-while.

void Func_0200032c(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 i;

  if (id == 13) {
    if (object->x >> 20 == 6 && *(s32 *)&object->z >> 20 == 12) {
      Event_SetFlag(2570);
      object->motionFlags = 3;
      System_Wait(2);
      i = 0;
      if (object->y > object->terrainHeight || object->velocityY != 0) {
        do {
          System_Wait(1);
          i++;
          if (i > 29)
            break;
        } while (object->y > object->terrainHeight || object->velocityY != 0);
      }
      object->motionFlags = 0;
      FieldActor_Stop(gMainRoutineWork.currentOwner);
      Actor_SetPosition(id, 0, 0);
      Event_PlaySound(SE_HEAVY_IMPACT);
      MapGrid_CopyCellsLayered(66, 39, 1, 1, 70, 10);
      MapGrid_CopyCellsLayered(5, 75, 1, 1, 6, 75);
      MapGrid_CopyCellAttributes(5, 12, 1, 1, 6, 12);
      FieldEvent_Wait(30);
      Event_PlaySound(SE_MAP_TRANSITION);
      MapGrid_CopyCellsLayered(70, 39, 1, 1, 71, 10);
      FieldEvent_Wait(20);
      Func_020001b4();
    } else {
      MapGrid_SetCellHeight(object->x, object->z, 0, 6);
    }
  }
  if (id == 14) {
    if (object->x >> 20 == 57 && *(s32 *)&object->z >> 20 == 12) {
      Event_SetFlag(2571);
      object->motionFlags = 3;
      System_Wait(2);
      i = 0;
      if (object->y > object->terrainHeight || object->velocityY != 0) {
        do {
          System_Wait(1);
          i++;
          if (i > 29)
            break;
        } while (object->y > object->terrainHeight || object->velocityY != 0);
      }
      object->motionFlags = 0;
      FieldActor_Stop(gMainRoutineWork.currentOwner);
      Actor_SetPosition(id, 0, 0);
      Event_PlaySound(SE_HEAVY_IMPACT);
      MapGrid_CopyCellsLayered(68, 39, 1, 1, 121, 10);
      MapGrid_CopyCellsLayered(58, 75, 1, 1, 57, 75);
      MapGrid_CopyCellAttributes(58, 12, 1, 1, 57, 12);
      FieldEvent_Wait(30);
      Event_PlaySound(SE_MAP_TRANSITION);
      MapGrid_CopyCellsLayered(72, 39, 1, 1, 120, 10);
      FieldEvent_Wait(20);
      Func_02000264();
    } else {
      MapGrid_SetCellHeight(object->x, object->z, 0, 6);
    }
  }
  if (Event_GetFlag(2570) && Event_GetFlag(2571))
    Event_SetFlag(2591);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 513;
  if ((u16)(gMainRoutineWork.entrance - 3) <= 1)
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  JupiterLighthouse_LoadPalettesZero(0);
  Func_02002b28(15, 1);
  Func_02002b28(16, 1);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  Actor_Get(17)->priorityFlags |= 128;
  Actor_Get(18)->priorityFlags |= 128;
  Actor_Get(19)->priorityFlags |= 128;
  Actor_Get(20)->priorityFlags |= 128;
  Actor_Get(21)->priorityFlags |= 128;
  Actor_Get(22)->priorityFlags |= 128;
  JupiterLighthouse_PlaceRaisedActors();
  return 0;
}

/// The room's setup: actors 13 and 14 by flags 0xa0a and 0xa0b (with the map
/// cells copied when set), then the terrain under each.
s32 Scene_PrepareMap(void) {
  struct Actor *object;

  Scene_SetArrivalFlags(0);
  LightSprites_InitB();
  if (Event_GetFlag(0xa0a)) {
    Actor_SetPosition(13, 0, 0);
    MapGrid_CopyCellsLayered(66, 39, 1, 1, 70, 10);
    MapGrid_CopyCellsLayered(5, 75, 1, 1, 6, 75);
    MapGrid_CopyCellAttributes(5, 12, 1, 1, 6, 12);
    MapGrid_CopyCellsLayered(70, 39, 1, 1, 71, 10);
  } else {
    LightSprites_Add(13, 3);
    Actor_Get(13)->priorityFlags |= 32;
  }
  object = Actor_Get(13);
  object->motionFlags = 0;
  {
    s32 last = 6;

    MapGrid_SetCellHeight(object->x, object->z, 0, last);
  }
  if (Event_GetFlag(0xa0b)) {
    Actor_SetPosition(14, 0, 0);
    MapGrid_CopyCellsLayered(68, 39, 1, 1, 121, 10);
    MapGrid_CopyCellsLayered(58, 75, 1, 1, 57, 75);
    MapGrid_CopyCellAttributes(58, 12, 1, 1, 57, 12);
    MapGrid_CopyCellsLayered(72, 39, 1, 1, 120, 10);
  } else {
    LightSprites_Add(14, 5);
    Actor_Get(14)->priorityFlags |= 32;
  }
  object = Actor_Get(14);
  object->motionFlags = 0;
  {
    s32 last = 6;

    MapGrid_SetCellHeight(object->x, object->z, 0, last);
  }
  BYTE_AT(Actor_Get(11), 0x62) = 1;
  BYTE_AT(Actor_Get(12), 0x62) = 1;
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

void PaletteCycle_Step(void) {
  u16 colors[16];
  s32 phase = (gPaletteCycleFrame / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gPaletteCyclePausesInEvents != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  i = 0;
  offset = 4;
next:
  p = colors;
  {
    /* FAKEMATCH(temporary): the slot and the table's address held in locals. */
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gPaletteCycleColors;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gPaletteCycleFrame++;
}

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES_ZERO(
    JupiterLighthouse_LoadPalettesZero, gPaletteCyclePausesInEvents,
    gPaletteCycleColors, gPaletteCycleUnlitPalette, gPaletteCycleFrame,
    PaletteCycle_Step, gPaletteCycleUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, MapGrid_SetCellHeight,
                             MapGrid_SetAttributeBAt)

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z,
                                                   s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_02000930(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->unkd46 = 0;
  actor->motionFlags = 4;
  Actor_SetSpriteFlags(actor, 0);
  while (*((u8 *)linked + 0x63) != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->facing = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->unkd44 = 1;
  work->unkd46 = 1;
  facing = linked->facing;
  actor->x = (((linked->x >> 19) & ~1) + (facing >> 13) - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - ((facing >> 13) & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSpriteFlags(actor, 1);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  shadow = FieldCamera_GetActor();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  FieldActor_SetMoveTarget(
      shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z), actor->z);
  i = 0;
  while (i <= 59 && actor->y > actor->terrainHeight + 0x100000) {
    i++;
    System_Wait(1);
  }
  Event_PlaySound(SE_LANDING_THUD);
  i = 0;
  while (i <= 59 && actor->velocityY != 0) {
    i++;
    System_Wait(1);
  }
  FieldActor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unkd46 = 1;
  work->exit = 0;
  Event_End();
}

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern s32 gData_0200331c[];
extern s32 gData_0200333c;

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

void Whirlwind_Update(struct Actor *self) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(SE_WATER_LEVEL);
  {
    s8 *to = self->sprite;
    s8 *from = leader->sprite;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (MapGrid_GetCellBehavior(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Camera_WorldToScreen(position);
    distance = position[0];
    if (distance > 0x1100000)
      goto stop;
    if (distance < -0x200000)
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y - self->y;
        if (distance < 0)
          distance = self->y - leader->y;
        near = 0;
        if (Position_FlatDistance(&leader->x, &self->x) <= 12 &&
            distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->facing = self->facing;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          {
            s32 delay = 200;
            *(u16 *)(work + 362) = delay;
          }
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      {
        s32 (*multiply)(s32, s32);
        s32 cosine = Trig_Cos(self->facing);
        s32 speed = self->speedLimit;
        /* FAKEMATCH(dead): the routine address taken inside a once-only block.
         */
        do {
          multiply = Iwram_MulQ16;
        } while (0);
        self->x += multiply(cosine, speed);
      }
      self->z += Iwram_MulQ16(Math_Sin(self->facing), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Actor_StartScript(self, gData_0200331c);
    Event_PlaySound(SE_SPARKLING_PASSAGE);
    gData_0200333c = zero;
  }
}

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Import_ActorSelectAnimation,
                       Whirlwind_Update, FieldEffect_Spawn)

s32 Func_02000de0(struct Actor *object) {
  struct EffectOptions spark;
  s32 frame = gFrameCount & 7;

  if (frame == 0) {
    s32 x;
    s32 y;
    s32 speed;

    spark.priority = 3;
    spark.startScaleX = 0xb333;
    spark.startScaleY = 0xb333;
    spark.palette = 14;
    x = object->x + (((((u32)Random16() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (((u32)Random16() * 20) >> 16)) << 16);
    speed = (s32)(((((u32)Random16() * 5) >> 16) << 16) + 0x50000) / 10;
    FieldEffect_Spawn(x, y, object->z, 0, speed, frame, 0xb0000, &spark);
  }
  return 0;
}

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

void Func_02001010(struct Actor *object) {
  u8 *fill = &gDecodeFillByte;
  u8 *step = &BYTE_AT(object, 99);
  s32 phase = *step & 3;

  *fill = 2;
  switch (phase) {
  case 1:
    Actor_SetSpriteColorMode(object, 13);
    break;
  case 0:
  case 2:
    Actor_SetSpriteColorMode(object, 4);
    break;
  case 3:
    Actor_SetSpriteColorMode(object, 4);
    break;
  }
  (*step)++;
}

void Func_0200105c(struct Actor *object) {
  BYTE_AT(object, 99) = 0;
  WORD_AT(object, 108) = (s32)Func_02001010;
}

void Func_02001070(struct Actor *object) {
  WORD_AT(object, 108) = 0;
  Actor_SetSpriteColorMode(object, 4);
}

struct BeamWork {
  u8 tiles[0x200];
  struct BeamSprite sprites[32];
  struct BeamSpark sparks[32];
  struct BeamSprite large;
  u8 unkaa8[0x1c];
  s32 from[3];
  s32 to[3];
  s32 carried[3];
  s32 pointA[3];
  s32 pointB[3];
  s16 step;
  s16 frame;
  s16 largeEntry;
  u16 tile;
  s8 duration;
  s8 loop;
  s8 still;
  s8 side;
  struct Actor *first;
  struct Actor *second;
};

#define LERP(from, to, t, n) ((from) + ((to) - (from)) * (t) / (n))

void Func_02001080(void) {
  struct BeamWork *work = Ram_HeapSlots->menuRuntime;
  struct Actor *carrier =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 460);
  s32 t = work->frame;
  s32 position[3];
  s32 a[3];
  s32 b[3];
  s32 aside;
  s32 scale;
  s32 *pos;
  struct BeamSpark *spark;
  struct BeamSprite *sprite;
  struct BeamSprite *large;
  s32 i;

  switch (work->step) {
  case 0:
    work->duration = 30;
    aside = work->side == 0 ? 0xa0000 : -0xa0000;
    a[0] = work->from[0] - aside;
    a[1] = work->from[1] + 0x180000;
    a[2] = work->from[2];
    b[0] = work->from[0];
    b[1] = work->from[1];
    b[2] = work->from[2];
    work->pointA[0] = LERP(a[0], b[0], t, work->duration);
    work->pointA[1] = LERP(a[1], b[1], t, work->duration);
    work->pointA[2] = LERP(a[2], b[2], t, work->duration);
    a[1] = work->from[1] - 0x180000;
    work->pointB[0] = LERP(a[0], b[0], t, work->duration);
    work->pointB[1] = LERP(a[1], b[1], t, work->duration);
    work->pointB[2] = LERP(a[2], b[2], t, work->duration);
    if (work->frame == work->duration) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 1:
    work->duration = 60;
    aside = work->side == 0 ? 0x1e0000 : -0x1e0000;
    a[0] = work->from[0];
    a[1] = work->from[1];
    a[2] = work->from[2];
    b[0] = work->from[0] - aside;
    b[1] = work->from[1];
    b[2] = work->from[2];
    work->pointA[0] = LERP(a[0], b[0], t, work->duration);
    work->pointA[1] = LERP(a[1], b[1], t, work->duration);
    work->pointA[2] = LERP(a[2], b[2], t, work->duration);
    work->pointB[0] = work->pointA[0];
    work->pointB[1] = work->pointA[1];
    work->pointB[2] = work->pointA[2];
    large = &work->large;
    large->scaleY = large->scaleX = (work->frame << 16) / 60;
    if (work->frame == work->duration) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 2:
    if (work->frame == 0)
      work->still = 1;
    aside = work->side == 0 ? 0x8000 : -0x8000;
    work->pointA[0] += aside;
    work->pointB[0] += aside;
    if (work->frame == 14) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 3:
    if (t == 0)
      Sound_Play(SE_PSYNERGY_SPARKLE);
    work->carried[0] = ((work->from[0] >> 2) +
                        ((work->to[0] >> 2) - (work->from[0] >> 2)) * t / 96)
                       << 2;
    work->carried[1] = LERP(work->from[1], work->to[1], t, 96);
    work->carried[2] = LERP(work->from[2], work->to[2], t, 96);
    Actor_SetPositionAndResetMotion(carrier, work->carried[0], work->carried[1],
                                    work->carried[2]);
    aside = work->side == 0 ? 0x100000 : -0x100000;
    work->pointA[0] = work->carried[0] - aside;
    work->pointA[1] = work->carried[1];
    work->pointA[2] = work->carried[2];
    work->pointB[0] = work->pointA[0];
    work->pointB[1] = work->pointA[1];
    work->pointB[2] = work->pointA[2];
    if (work->frame == 96) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 4:
    if (work->frame == 0) {
      Sound_Play(SE_SHIMMER);
      Func_0200105c(work->second);
    }
    large = &work->large;
    large->scaleY = large->scaleX = large->scaleX - 0x800;
    if (large->scaleX <= 0x3332) {
      large->scaleX = 0;
      work->step++;
      work->frame = -1;
    }
    break;
  case 5:
    if (work->frame == 0)
      work->loop = 0;
    if (work->frame == 16)
      work->step = 999;
    break;
  }
  large = &work->large;
  scale = large->scaleX;
  pos = position;
  if (work->side == 0)
    large->scaleX = scale < 0 ? -scale : scale;
  else
    large->scaleX = -(scale < 0 ? -scale : scale);
  pos[0] = work->carried[0];
  pos[1] = work->carried[1];
  pos[2] = work->carried[2];
  Camera_WorldToScreen(pos);
  large->x = pos[0];
  large->z = pos[2];
  Func_02002b38(large);
  for (i = 0, spark = work->sparks, sprite = work->sprites; i <= 7;
       i++, sprite++, spark++) {
    if (spark->age == 0) {
      if (i & 1) {
        spark->position[0] = work->pointA[0];
        spark->position[1] = work->pointA[1];
        spark->position[2] = work->pointA[2];
      } else {
        spark->position[0] = work->pointB[0];
        spark->position[1] = work->pointB[1];
        spark->position[2] = work->pointB[2];
      }
      if (work->still == 0) {
        spark->position[1] += Trig_Sin(Random16()) << 3;
        spark->drift = 0x4ccc;
      } else {
        spark->position[1] += Trig_Sin(Random16()) << 1;
        spark->position[0] += Trig_Cos(Random16()) * 10;
        spark->drift = (Random16() - 0x8000) << 1;
      }
    }
    if (spark->age <= 15) {
      sprite->tile = work->tile + (spark->age & 3) * 2;
      EffectSprite_DrawAtWorldPosition(sprite, spark);
      spark->position[1] -= spark->drift;
    }
    spark->age++;
    if (work->loop != 0 && spark->age == 16)
      spark->age = 0;
  }
  work->frame++;
}
#undef LERP

/* VIEW(s17a8): tag struct BeamWork as struct BeamWork__s17a8;
 * games/tla/src/scene/jupiter_lighthouse_aerie/s0d08 has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/jupiter_lighthouse_aerie.c:857: structure has no
 * member named `smallTile'. */
struct BeamWork__s17a8 {
  u8 tiles[0x200];
  struct EffectSprite sprites[32];
  struct BeamSpark sparks[32];
  struct EffectSprite large;
  u8 unkaa8[0x1c];
  s32 from[3];
  s32 to[3];
  s32 start[3];
  u8 unkae8[0x18];
  s16 step;
  s16 unkb02;
  u16 largeTile;
  u16 smallTile;
  u8 unkb08;
  u8 unkb09;
  u8 unkb0a;
  u8 side;
  struct Actor *first;
  struct Actor *second;
};

void Actor_StartIdle(struct Actor *object);

static __inline__ void SetUp(struct EffectSprite *sprite, s32 pivotX,
                             s32 pivotY, u32 attributes, s32 tile) {
  /* FAKEMATCH(wrapper): the setup forwarded inline. */
  EffectSprite_Init(sprite, pivotX, pivotY, attributes, tile);
}

void Func_020017a8(s32 a, s32 b, s32 side) {
  struct BeamWork__s17a8 *work =
      HeapSlot_Alloc(220, sizeof(struct BeamWork__s17a8));
  struct Actor *carrier =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 460);
  struct EffectSprite *sprite;
  struct EffectSprite *large;
  struct BeamSpark *spark;
  s32 smallEntry;
  s32 largeEntry;
  s32 smallTile;
  s32 largeTile;
  s32 i;
  s32 age;

  work->first = Actor_Get(a);
  work->second = Actor_Get(b);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade),
                      work);
  smallEntry = VramCache_FindFreeEntry();
  smallTile = Vram_Load(smallEntry, 256, work);
  work->smallTile = smallTile;
  spark = work->sparks;
  age = 0;
  sprite = work->sprites;
  i = 31;
  for (; i >= 0; age -= 2, i--, sprite++, spark++) {
    EffectSprite_Init(sprite, 4, 4, 0, smallTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    *(u16 *)&sprite->depth = 240;
    spark->age = age;
  }
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_GoldenTrident),
                      work);
  largeEntry = VramCache_FindFreeEntry();
  {
    /* FAKEMATCH(temporary): sprite also holds the size. */
    sprite = (struct EffectSprite *)512;
    largeTile = Vram_Load(largeEntry, (s32)sprite, work);
  }
  work->largeTile = largeTile;
  large = &work->large;
  SetUp(large, 30, 7, 0x80004000, largeTile);
  *(u16 *)&large->depth = 240;
  large->oam.priority = 1;
  large->oam.palette256 = 1;
  large->oam.palette = 0;
  large->scaleX = 0;
  large->scaleY = 0;
  if (side == 0) {
    work->from[0] = 152 << 16;
    work->from[1] = 0x200000;
    work->from[2] = 164 << 16;
    work->to[0] = 235 << 17;
    work->to[1] = 0x200000;
    work->to[2] = 164 << 16;
  } else {
    work->from[0] = 218 << 18;
    work->from[1] = 0x200000;
    work->from[2] = 164 << 16;
    work->to[0] = 0x2260000;
    work->to[1] = 0x200000;
    work->to[2] = 164 << 16;
  }
  work->start[0] = work->from[0];
  work->start[1] = work->from[1];
  work->start[2] = work->from[2];
  carrier->speedLimit = 0x8000;
  carrier->acceleration = 0x8000;
  Actor_StartIdle(carrier);
  FieldActor_SetMoveTarget(carrier, work->start[0], work->start[1],
                           work->start[2]);
  Event_PlaySound(SE_SWIRL);
  Func_0200105c(work->first);
  FieldEvent_Wait(40);
  work->side = side;
  work->step = 0;
  work->unkb02 = 0;
  work->unkb09 = 1;
  work->unkb0a = 0;
  Task_AddFrame(Func_02001080, 0x480);
  while (work->step != 999)
    System_Wait(1);
  FieldTask_Remove(Func_02001080);
  VramCache_FreeEntry(largeEntry);
  VramCache_FreeEntry(smallEntry);
  HeapSlot_Release(220);
}

extern s16 gLightSpriteTile;

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOams,
                          gLightSpriteCount, gLightSpriteActors,
                          gLightSpritesLarge, gLightSpritePriority,
                          BattleEffect_GetObjectPriority,
                          Oam_AddSpriteAtDepthClamped)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount,
                         gLightSpriteActors)

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

#undef LERP
// @file
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Import_ActorSelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Import_ActorSelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define CELL_TABLE gData_02003488
#define CELL_SPRITE_DRAW CrackedTiles_Update
/* VIEW(s1e78): routine Oam_AddSpriteAtDepthClamped as cell_sprite.c declares
 * it; the joined file has another; tried as the definition's void
 * Oam_AddSpriteAtDepthClamped(void *entry, s32 slot), which the object already
 * declares, it does not compile in tla-ja:
 * games/tla/src/field/map/cell_sprite.c:20: conflicting types for
 * `Oam_AddSpriteAtDepthClamped'. */
void Oam_AddSpriteAtDepthClamped__s1e78(void *entry, s32 priority) __asm__(
    "Oam_AddSpriteAtDepthClamped");
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped__s1e78
#include "../field/map/cell_sprite.c"

DEFINE_CRACKED_TILES_FIND(CrackedTiles_Find, gData_02003488,
                          CrackedTiles_Update)

extern s32 gPaletteCycleFrame;
extern s32 gPaletteCyclePausesInEvents;
extern u16 gPaletteCycleColors[];
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

struct CellTargetEntry;

/* VIEW(s24a8): tag struct CellTarget as struct CellTarget__s24a8; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * CrackedTile_Fall moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s24a8 {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
};

void Leader_CenterInCell(void);

/* VIEW(s24a8): routine CrackedTile_Fall as this part defines it; the scene's
 * module header declares it with the joined file's CellTarget. */
void CrackedTile_Fall__s24a8(
    struct CellTarget__s24a8 *table,
    struct CellTargetEntry *entry) __asm__("CrackedTile_Fall");
DEFINE_CRACKED_TILE_FALL(CrackedTile_Fall__s24a8, CellTarget__s24a8,
                         Leader_CenterInCell)

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
#undef Actor_StartScript
// @file
extern s32 gPaletteCycleFrame;
extern s32 gPaletteCyclePausesInEvents;
extern u16 gPaletteCycleColors[];
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

DEFINE_CRACKED_TILE_DROP_IN(CrackedTile_DropIn, FieldEffect_Spawn)

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gData_02003488, CrackedTile_Crack,
                          CrackedTile_Fall)

void Func_02002888(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct CellTarget *work = &gData_02003488;
  s32 x = FieldGameFlag_GetByte(work->flagByte);
  s32 z = FieldGameFlag_GetByte(work->flagByte + 8);

  if (x != 0 || z != 0) {
    struct Actor *shadow;

    GameFlag_SetByte(work->flagByte, 0);
    GameFlag_SetByte(work->flagByte + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    shadow = *(struct Actor **)(event + 460);
    shadow->x = object->x;
    shadow->z = object->z;
    Map_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = value;
  }
}

extern u32 gFieldEffect_ShortScript[];

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 560, 96, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_SCENE_EXITS_2(264, 259, 1, 259, 2, 259, 3, 262, 4)

struct ActorPlacement gActorPlacements[] = {
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 1},
    {339, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(256), FACING_WEST, 2, 0},
    {339, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(256), FACING_EAST, 2, 0},
    {340, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(200), FACING_EAST, 2, 0},
    {340, CONDITION_ALWAYS, 1, PIXELS(888), 0, PIXELS(200), FACING_WEST, 2, 0},
    {428, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {428, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {302, CONDITION_ALWAYS, 7, PIXELS(312), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 0},
    {302, CONDITION_ALWAYS, 7, PIXELS(344), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {302, CONDITION_ALWAYS, 7, PIXELS(376), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {302, CONDITION_ALWAYS, 7, PIXELS(648), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {302, CONDITION_ALWAYS, 7, PIXELS(680), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {302, CONDITION_ALWAYS, 7, PIXELS(712), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {425, CONDITION_ALWAYS, 11, PIXELS(512), 0, PIXELS(248), FACING_EAST, 2, 0},
    {425, CONDITION_ALWAYS, 11, PIXELS(512), 0, PIXELS(248), FACING_EAST, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13, CONDITION_ALWAYS, EVENT(Func_02000314)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, EVENT(Func_0200032c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14, CONDITION_ALWAYS, EVENT(Func_02000314)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(Func_0200032c)},
    {8, -1, CONDITION_ALWAYS, EVENT(Func_02000314)},
    {9, -1, CONDITION_ALWAYS, EVENT(Func_0200032c)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gData_0200331c[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
#undef CrackedTile_Fall
