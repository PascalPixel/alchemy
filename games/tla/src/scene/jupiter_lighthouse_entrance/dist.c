#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "irq.h"
#include "event_work.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "call.h"
#include "field_effect.h"
#include "field_coordinate.h"
#include "field_event.h"
#include "task.h"
#include "frame.h"

s32 Func_020005b8(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

extern const s32 gData_0200219c[];
extern s32 gData_020021bc;

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
s32 Func_02001b2c(s32 x, s32 z, s32 layer);
void Func_02001b14(s32 *position);
s32 Math_Cosine(s32 angle);
void Func_02001a0c(struct Actor *object, const s32 *script);

void Func_020005e4(struct Actor *self) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (work->unk166 != 0 || *(s16 *)&work->unk15a != 0 ||
      *(s16 *)&work->unk15c[2] != 0 || *(s16 *)work->unk168 != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->action & 31) == 31)
    Event_PlaySound(231);
  ((struct FieldSprite *)self->animation)->priority =
      ((struct FieldSprite *)leader->animation)->priority;
  if (Func_02001b2c(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Func_02001b14(position);
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
        if (Func_020005b8(&leader->x, &self->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->angle = self->angle;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          work->unk16a = 200;
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      // IwramMulQ16 reached from IwramIrqMain rather than at its fixed address:
      // the address is a linked word read after the cosine, as the native
      // reads it after the speed (at Iwram_MulQ16 it is built before).
      self->x += ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulQ16Offset))(
          Math_Cosine(self->angle), self->speedLimit);
      self->z += ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulQ16Offset))(
          Math_Sin(self->angle), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;

    /* FAKEMATCH(temporary): one word zero clears the ride byte, then the
     * update; as self->update = 0; *((u8 *)self + 0x63) = 0;, 4 of 197
     * instructions differ in tla-en (byte first, 15). */
    *((u8 *)self + 0x63) = zero;
    self->update = (void *)zero;
  }
  Func_02001a0c(self, gData_0200219c);
  Event_PlaySound(228);
  gData_020021bc = 0;
}

struct GustObject {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x0f];
  u8 priorityFlags;
  u8 unk24[0x0c];
  s32 unk30;
  u8 unk34[0x21];
  u8 motionFlags;
  u8 unk56[0x0c];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u8 unk66[6];
  void (*update)(struct GustObject *object);
};

void Func_020019fc(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct GustObject *object, u32 flags);
void Func_02000eac(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags, const struct EffectOptions *options);

struct GustObject *Func_020007a4(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(222);
  position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Cosine(source->angle));
  position.z = source->z + Iwram_MulQ16(0x80000, Math_Sin(source->angle));
  object = ((struct GustObject *)FieldActor_Create(280, position.x, source->y,
                                                    position.z));
  Func_020019fc(object, 2);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_020005e4;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_02000eac(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}

struct SceneEffectSource {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct SceneEffectParams {
  s32 unk00;
  s32 mode;
  s32 color1;
  s32 color2;
  u8 unk10[0x18];
};

s32 Func_0200087c(struct SceneEffectSource *object) {
  struct SceneEffectParams params;
  s32 phase, x, y, speed;

  phase = gFrameCount & 7;
  if (phase == 0) {
    params.unk00 = 3;
    params.color1 = 0xb333;
    params.color2 = 0xb333;
    params.mode = 14;
    x = object->x + (((s32)((u32)(Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Random16Far() * 20) >> 16)) << 16);
    speed = ((((s32)((u32)(Random16Far() * 5) >> 16)) << 16) + 0x50000) / 10;
    Func_02000eac(x, y, object->z, 0, speed, phase, 0xb0000, &params);
  }
  return 0;
}

/// struct Actor as this function reads it, its coordinates as
/// FieldCoordinate.
struct PositionedActor {
  u8 unk00[8];
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  s32 terrainHeight;
  s32 scaleX;
  u8 unk1c[0x34];
  struct FieldSprite *animation;
  u8 animationKind;
  u8 motionFlags;
};

void Animation_ApplyChildValues(struct PositionedActor *object, s32 value);

void Func_0200091c(void) {
  struct PositionedActor *actor9 = ((struct PositionedActor *)Actor_Get(9));
  struct PositionedActor *actor10 =
      ((struct PositionedActor *)Actor_Get(10));
  struct PositionedActor *actor23 =
      ((struct PositionedActor *)Actor_Get(23));
  struct FieldSprite *sprite = actor23->animation;

  Actor_SetPosition(24, 0x2000000, 0xf80000);
  Actor_SetPosition(23, 0x2000000, 0xf80000);
  Actor_SetPosition(9, 0x1d80000, 0x1000000);
  Actor_SetPosition(10, 0x2280000, 0x1000000);
  ((struct PositionedActor *)Actor_Get(24))->scaleX = 0;
  actor23->motionFlags = 0;
  actor23->y.fixed = actor23->terrainHeight;
  actor9->motionFlags = 0;
  actor9->y.fixed = actor9->terrainHeight;
  actor10->motionFlags = 0;
  actor10->y.fixed = actor10->terrainHeight;
  if (Event_GetFlag(0xa0a)) {
    actor9->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(9)), 4);
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(11)),
                               4);
  }
  if (Event_GetFlag(0xa0b)) {
    actor10->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(10)),
                               4);
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(12)),
                               4);
  }
  if (Event_GetFlag(0xa0a) && Event_GetFlag(0xa0b)) {
    actor9->y.fixed += 0x199900;
    actor10->y.fixed += 0x199900;
    {
      s32 step = 0x1b8480;

      actor23->y.fixed += step;
    }
    actor9->motionFlags = 4;
    actor10->motionFlags = 4;
    actor23->motionFlags = 4;
  }
}

// Queues the lit actors' sprites: for each listed actor with a position, projects it
// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
// 64 one when the big switch is set, at its animation picture and depth.
#include "map.h"
#include "vram_tab.h"

struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

extern s16 gData_02002200;
extern s16 gData_02002202;
extern struct FieldActor *gData_02002204[];
extern struct LightOam Data_02002244[];
extern s16 gData_02002304;
extern s16 gData_02002306;

s32 Func_02001b24(struct FieldActor *actor);
void Func_020019d4(struct LightOam *oam, s32 depth);

void Func_02000aac(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02002200].offset >> 5;
  viewY = map->origin[1];
  oam = Data_02002244;

  for (i = 0; i < gData_02002202; i++) {
    struct FieldActor *actor = gData_02002204[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_02002304;
      if (big == 0) {
        s32 x, sy;
        frame = (s16)actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
        ox = x - 8;
        oy = sy - 16;
        if ((u32)(x + 7) > 334 || oy <= -16 || oy > 239)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0x40002000;
        *word = (tile + frame * 8) | 0x800;
        if (gData_02002306 == -1)
          oam->priority = Func_02001b24(actor);
        else
          oam->priority = gData_02002306;
        Func_020019d4(oam, depth);
        oam++;
      } else {
        s32 x, sy;
        frame = (s16)actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
        ox = x - 8;
        oy = sy - 64;
        if ((u32)(x + 23) > 350 || oy <= -64 || oy > 175)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0xc000a000;
        *word = (tile + frame * 8) | 0x800;
        if (gData_02002306 == -1)
          oam->priority = Func_02001b24(actor);
        else
          oam->priority = gData_02002306;
        Func_020019d4(oam, depth);
        oam++;
      }
    }
  }
}

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];

void *Heap_Alloc(s32 size);
void Func_020019bc(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02000c68(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002204, 64);
  Func_020019bc(gField_CachedTilesA, buffer);
  gData_02002200 = Resource_FindFreeEntry();
  VramCache_Load(gData_02002200, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000aac, TASK_PASS_VBLANK);
  gData_02002202 = 0;
  gData_02002304 = 0;
  gData_02002306 = -1;
}

void Func_02000ce8(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_02002204, 64);
  Func_020019bc(gField_CachedTilesB, buffer);
  gData_02002200 = Resource_FindFreeEntry();
  Vram_Load(gData_02002200, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000aac, TASK_PASS_VBLANK);
  gData_02002202 = 0;
  gData_02002304 = 0;
  gData_02002306 = -1;
}

void Func_02000d68(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002204, 64);
  Func_020019bc(gField_CachedTilesC, buffer);
  gData_02002200 = Resource_FindFreeEntry();
  Vram_Load(gData_02002200, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000aac, TASK_PASS_VBLANK);
  gData_02002202 = 0;
  gData_02002304 = 1;
  gData_02002306 = -1;
}

void Func_02000df0(s32 id, s32 value) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_02002202;
    gData_02002204[*count] = object;
    (*count)++;
  }
}

void Func_02000e20(s32 value) { gData_02002306 = value; }

#include "../../field/object/set_sprite_priority.c"
