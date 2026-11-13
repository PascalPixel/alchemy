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
#include "item_icon.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_entrance.h"
#include "module/scene/scene.h"

s32 Func_020005b8(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

extern const s32 gData_0200219c[];
extern s32 gData_020021bc;

s32 Func_02001b2c(s32 x, s32 z, s32 layer);
/* VIEW(dist): routine Camera_WorldToScreen as void Camera_WorldToScreen(s32 *position); module/field/event_commands.h has void Camera_WorldToScreen(struct EffectVector *position). */
extern void Camera_WorldToScreen__dist(s32 *position) __asm__("Camera_WorldToScreen");
void Actor_StartScript(struct Actor *object, const s32 *script);

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
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(231);
  ((struct FieldSprite *)self->sprite)->priority =
      ((struct FieldSprite *)leader->sprite)->priority;
  if (Func_02001b2c(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Camera_WorldToScreen__dist(position);
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
          leader->facing = self->facing;
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
          Math_Cosine(self->facing), self->speedLimit);
      self->z += ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulQ16Offset))(
          Math_Sin(self->facing), self->speedLimit);
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
  Actor_StartScript(self, gData_0200219c);
  Event_PlaySound(228);
  gData_020021bc = 0;
}

/* VIEW(dist): routine Func_020019fc as void Func_020019fc(struct GustObject *object, s32 mode); module/scene/jupiter_lighthouse_entrance.h has void Func_020019fc(struct FieldEffect *effect, s32 mode). */
extern void Func_020019fc__dist(struct GustObject *object, s32 mode) __asm__("Func_020019fc");
/* VIEW(dist): routine Actor_SetSingleChildFlags as void Actor_SetSingleChildFlags(struct GustObject *object, u32 flags); field.h has void Actor_SetSingleChildFlags(struct Actor *object, u32 flags). */
extern void Actor_SetSingleChildFlags__dist(struct GustObject *object, u32 flags) __asm__("Actor_SetSingleChildFlags");

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
  Func_020019fc__dist(object, 2);
  Actor_SetSingleChildFlags__dist(object, 0);
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

/* VIEW(dist): tag struct SceneEffectParams as struct SceneEffectParams__dist; module/scene/scene.h has another definition. */
struct SceneEffectParams__dist {
  s32 unk00;
  s32 mode;
  s32 color1;
  s32 color2;
  u8 unk10[0x18];
};

s32 Func_0200087c(struct SceneEffectSource *object) {
  struct SceneEffectParams__dist params;
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

/* VIEW(dist): variable gData_02002200 as s16 gData_02002200; module/scene/apojii_islands.h has u32 gData_02002200[]. */
extern s16 gData_02002200__dist __asm__("gData_02002200");
/* VIEW(dist): variable gData_02002204 as struct Actor *gData_02002204[]; module/scene/jupiter_lighthouse_basement.h has u32 gData_02002204[]. */
extern struct Actor *gData_02002204__dist[] __asm__("gData_02002204");

/* VIEW(dist): routine Func_02001b24 as s32 Func_02001b24(struct Actor *actor); module/scene/magma_rock.h has void Func_02001b24(void). */
extern s32 Func_02001b24__dist(struct Actor *actor) __asm__("Func_02001b24");
void Func_020019d4(struct LightOam *oam, s32 depth);

void Func_02000aac(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02002200__dist].offset >> 5;
  viewY = map->origin[1];
  oam = Data_02002244;

  for (i = 0; i < gData_02002202; i++) {
    struct Actor *actor = gData_02002204__dist[i];
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
        frame = actor->timer;
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
          oam->priority = Func_02001b24__dist(actor);
        else
          oam->priority = gData_02002306;
        Func_020019d4(oam, depth);
        oam++;
      } else {
        s32 x, sy;
        frame = actor->timer;
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
          oam->priority = Func_02001b24__dist(actor);
        else
          oam->priority = gData_02002306;
        Func_020019d4(oam, depth);
        oam++;
      }
    }
  }
}

/* VIEW(dist): routine Resource_DecodeType01 as void Resource_DecodeType01(const void *source, void *destination); resource.h has s32 Resource_DecodeType01(const void *source, void *destination). */
extern void Resource_DecodeType01__dist(const void *source, void *destination) __asm__("Resource_DecodeType01");

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02000c68(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002204__dist, 64);
  Resource_DecodeType01__dist(gField_CachedTilesA, buffer);
  gData_02002200__dist = Resource_FindFreeEntry();
  VramCache_Load(gData_02002200__dist, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000aac, TASK_PASS_VBLANK);
  gData_02002202 = 0;
  gData_02002304 = 0;
  gData_02002306 = -1;
}

void Func_02000ce8(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_02002204__dist, 64);
  Resource_DecodeType01__dist(gField_CachedTilesB, buffer);
  gData_02002200__dist = Resource_FindFreeEntry();
  Vram_Load(gData_02002200__dist, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000aac, TASK_PASS_VBLANK);
  gData_02002202 = 0;
  gData_02002304 = 0;
  gData_02002306 = -1;
}

void Func_02000d68(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002204__dist, 64);
  Resource_DecodeType01__dist(gField_CachedTilesC, buffer);
  gData_02002200__dist = Resource_FindFreeEntry();
  Vram_Load(gData_02002200__dist, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000aac, TASK_PASS_VBLANK);
  gData_02002202 = 0;
  gData_02002304 = 1;
  gData_02002306 = -1;
}

void Func_02000df0(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_02002202;
    gData_02002204__dist[*count] = object;
    (*count)++;
  }
}

void Func_02000e20(s32 value) { gData_02002306 = value; }

#include "../../field/object/set_sprite_priority.c"
