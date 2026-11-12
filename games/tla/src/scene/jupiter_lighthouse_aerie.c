/// @file
/// The scene code of overlay 0x6a4 that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_aerie/.

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
#include "event_inline.h"
#include "actor.h"
#include "main_routine_work.h"
#include "jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

union ActorCoordinate {
  s32 value;
  u16 half;
};

void Func_0200105c(struct Actor *object);

void Func_02000054(s32 mode) {
  struct Actor *left = Actor_Get(9);
  struct Actor *right = Actor_Get(10);
  struct Actor *rider = Actor_Get(23);
  u8 *sprite = rider->animation;
  s32 i;

  FieldActor_SetPositionAndResetMotion(rider, rider->x, rider->y, rider->z);
  if (mode == 0 || mode == 2) {
    Event_PlaySound(556);
    for (i = 0; i <= 63; i++) {
      left->y += 0x1999;
      rider->y += 0x7ae;
      // Through a union, so this store may alias the sprite read after it.
      ((union ActorCoordinate *)&rider->x)->value += -0xa3d;
      *(u16 *)(sprite + 18) += 32;
      System_Wait(1);
    }
  } else {
    Event_PlaySound(556);
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
  Event_PlaySound(522);
  for (i = 255; i >= 0; i--) {
    Actor_Get(9)->y += 0x1999;
    Actor_Get(10)->y += 0x1999;
    rider->y += 0x1999;
    System_Wait(1);
  }
  Event_PlaySound(390);
  FieldEvent_Wait(40);
  Event_PlaySound(80);
  FieldEvent_Wait(10);
  Sound_WaitForJingle();
  left->motionFlags = 4;
  right->motionFlags = 4;
  rider->motionFlags = 4;
}

struct JupiterLighthouseAerieObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

void Func_02001070(struct JupiterLighthouseAerieObject *object);
void Func_020017a8(s32 a, s32 b, s32 c);
void Func_02002a78(s32 x, s32 z, s32 a, s32 b);
void Func_02002a88(void);
void Func_02002a90(void);
void Func_02002af0(void);

void Func_020001b4(void) {
  Event_SetFlag(0xa0a);
  Func_02002a88();
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
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldEvent_Wait(40);
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(9)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(10)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(11)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(12)));
  Func_02002af0();
  Func_02002a90();
}

void Func_02000264(void) {
  Event_SetFlag(0xa0b);
  Func_02002a88();
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
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldEvent_Wait(40);
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(9)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(10)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(11)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(12)));
  Func_02002af0();
  Func_02002a90();
}

void Func_02000314(s32 unused, s32 actor) {
  struct JupiterLighthouseAerieObject *object =
      ((struct JupiterLighthouseAerieObject *)Actor_Get(actor));

  Func_02002a78(object->x, object->z, 0, 4);
}

// Jupiter Lighthouse aerie's two pushed objects: object 13 on cell (6, 12) or
// object 14 on cell (57, 12) sets its flag (2570, 2571), falls into place
// (waited on for at most 30 frames), copies cells (Func_0802b450,
// Map_CopyCellAttributeRect) with sounds 134 and 167 and runs its part
// (Func_020001b4, Func_02000264); off its cell it only marks the cell it stands
// on (Func_02002a78). With both flags set, flag 2591 is set. The fall wait is
// an if around a do-while.

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
      Event_PlaySound(134);
      MapGrid_CopyCellsLayered(66, 39, 1, 1, 70, 10);
      MapGrid_CopyCellsLayered(5, 75, 1, 1, 6, 75);
      MapGrid_CopyCellAttributes(5, 12, 1, 1, 6, 12);
      FieldEvent_Wait(30);
      Event_PlaySound(167);
      MapGrid_CopyCellsLayered(70, 39, 1, 1, 71, 10);
      FieldEvent_Wait(20);
      Func_020001b4();
    } else {
      Func_02002a78(object->x, object->z, 0, 6);
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
      Event_PlaySound(134);
      MapGrid_CopyCellsLayered(68, 39, 1, 1, 121, 10);
      MapGrid_CopyCellsLayered(58, 75, 1, 1, 57, 75);
      MapGrid_CopyCellAttributes(58, 12, 1, 1, 57, 12);
      FieldEvent_Wait(30);
      Event_PlaySound(167);
      MapGrid_CopyCellsLayered(72, 39, 1, 1, 120, 10);
      FieldEvent_Wait(20);
      Func_02000264();
    } else {
      Func_02002a78(object->x, object->z, 0, 6);
    }
  }
  if (Event_GetFlag(2570) && Event_GetFlag(2571))
    Event_SetFlag(2591);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Func_02000538(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 513;
  if ((u16)(gMainRoutineWork.entrance - 3) <= 1)
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_02000858(0);
  Func_02002b28(15, 1);
  Func_02002b28(16, 1);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  Actor_Get(17)->priorityFlags |= 128;
  Actor_Get(18)->priorityFlags |= 128;
  Actor_Get(19)->priorityFlags |= 128;
  Actor_Get(20)->priorityFlags |= 128;
  Actor_Get(21)->priorityFlags |= 128;
  Actor_Get(22)->priorityFlags |= 128;
  Func_02000e80();
  return 0;
}

void Func_02001cb4(void);
void Func_02001dbc(s32 actor, s32 mode);

/// The room's setup: actors 13 and 14 by flags 0xa0a and 0xa0b (with the map
/// cells copied when set), then the terrain under each.
s32 Func_02000604(void) {
  struct Actor *object;

  Scene_SetArrivalFlags(0);
  Func_02001cb4();
  if (Event_GetFlag(0xa0a)) {
    Actor_SetPosition(13, 0, 0);
    MapGrid_CopyCellsLayered(66, 39, 1, 1, 70, 10);
    MapGrid_CopyCellsLayered(5, 75, 1, 1, 6, 75);
    MapGrid_CopyCellAttributes(5, 12, 1, 1, 6, 12);
    MapGrid_CopyCellsLayered(70, 39, 1, 1, 71, 10);
  } else {
    Func_02001dbc(13, 3);
    Actor_Get(13)->priorityFlags |= 32;
  }
  object = Actor_Get(13);
  object->motionFlags = 0;
  {
    s32 last = 6;

    Func_02002a78(object->x, object->z, 0, last);
  }
  if (Event_GetFlag(0xa0b)) {
    Actor_SetPosition(14, 0, 0);
    MapGrid_CopyCellsLayered(68, 39, 1, 1, 121, 10);
    MapGrid_CopyCellsLayered(58, 75, 1, 1, 57, 75);
    MapGrid_CopyCellAttributes(58, 12, 1, 1, 57, 12);
    MapGrid_CopyCellsLayered(72, 39, 1, 1, 120, 10);
  } else {
    Func_02001dbc(14, 5);
    Actor_Get(14)->priorityFlags |= 32;
  }
  object = Actor_Get(14);
  object->motionFlags = 0;
  {
    s32 last = 6;

    Func_02002a78(object->x, object->z, 0, last);
  }
  BYTE_AT(Actor_Get(11), 0x62) = 1;
  BYTE_AT(Actor_Get(12), 0x62) = 1;
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "field_event.h"

void Func_02000790(s32 value) { gData_02003318 = value; }

void Func_0200079c(void) {
  u16 colors[16];
  s32 phase = (gData_02003314 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02003318 != 0) {
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
    /* FAKEMATCH(temporary): the slot and the table's address held in
     * locals; as colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_02003340 +
     * offset), 2 of 83 instructions differ in tla-en. */
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_02003340;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_02003314++;
}

extern u16 gData_02003360[];
extern u16 gData_02003364[];

void Func_02000858(s32 mode) {
  gData_02003318 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02003340, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02003360, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_02003314 = zero;
    Task_Add(Func_0200079c, 1152);
  } else {
    Dma_Set(gData_02003364, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02002b58();

void Func_020008dc(s32 id) {
  struct FieldActor *object;
  s32 height;

  object = ((struct FieldActor *)Actor_Get(id));
  object->motionFlags = 4;
  height = object->terrainHeight;
  WORD_AT(object, 68) = 0;
  object->y = height + 0x200000;
  {
    u8 *flags = &object->priorityFlags;
    /* FAKEMATCH(temporary): a local holding 128, so the bit is built in the
     * register it is ORed into; as *flags |= 128, 4 of 38 instructions differ
     * in tla-en. */
    s32 bit = 128;

    *flags = bit | *flags;
  }
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_02002a78(object->x, object->z, 0, (height >> 19) + 6);
  Func_02002b58(object->x, object->z, 0, 128);
}

#include "frame.h"
#include "field_sprite.h"
#include "event_work.h"

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z,
                                                   s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_02000930(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Func_02002a88();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->unkd46 = 0;
  actor->motionFlags = 4;
  Actor_SetSingleChildFlags(actor, 0);
  while (*((u8 *)linked + 0x63) != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->angle = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->unkd44 = 1;
  work->unkd46 = 1;
  facing = linked->angle;
  actor->x = (((linked->x >> 19) & ~1) + (facing >> 13) - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - ((facing >> 13) & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSingleChildFlags(actor, 1);
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
  Event_PlaySound(127);
  i = 0;
  while (i <= 59 && actor->velocityY != 0) {
    i++;
    System_Wait(1);
  }
  Actor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unkd46 = 1;
  work->exit = 0;
  Func_02002a90();
}

s32 Func_02000b1c(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

extern const s32 gData_0200331c[];
extern s32 gData_0200333c;

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
s32 Func_02002b50(s32 x, s32 z, s32 layer);
void Func_02002b20(s32 *position);
s32 Math_Cosine(s32 angle);
void Func_02002a00(struct Actor *object, const s32 *script);

void Func_02000b48(struct Actor *self) {
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
  if ((++*(u16 *)&self->action & 31) == 31)
    Event_PlaySound(231);
  {
    s8 *to = self->animation;
    s8 *from = leader->animation;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (Func_02002b50(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Func_02002b20(position);
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
        if (Func_02000b1c(&leader->x, &self->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->angle = self->angle;
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
        s32 cosine = Math_Cosine(self->angle);
        s32 speed = self->speedLimit;
        /* FAKEMATCH(dead): the routine address taken inside a once-only
         * block, so the speed loads first; as a plain call, 2 of 197
         * instructions differ in tla-en. */
        do {
          multiply = Iwram_MulQ16;
        } while (0);
        self->x += multiply(cosine, speed);
      }
      self->z += Iwram_MulQ16(Math_Sin(self->angle), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Func_02002a00(self, gData_0200331c);
    Event_PlaySound(228);
    gData_0200333c = zero;
  }
}
