#include "types.h"
#include "call.h"
#include "actor.h"
#include "field/event_work.h"
#include "scene_ids.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"

/// The native reads these event work halfwords signed (ldrsh).

struct MagmaRockEffectConfig {
  s32 kind;
  s32 variant;
  s32 unk08;
  s32 unk0c;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  u8 unk20[4];
  void (*unk24)(void);
};

extern u32 gFrameCount;
extern u32 gData_02003d50[];
extern u8 gData_02003d9e[];
extern u8 gData_02003db2[];
extern u8 gData_02003dc4[];
extern u8 gData_02003dd6[];
extern u8 gData_02003dee[];

u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectConfig *config);
struct Actor *Actor_GetById(s32 id);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Func_02003a54(struct Actor *object, s32 a);
void Func_02003c4c(struct Actor *object, s32 a);
s32 Func_020010cc(s32 x, s32 z, const u8 *data);
void Func_020011b8(void);
void Func_020015e4(void);
void Func_02001528(void);

struct AnimationEntry;

/// The sprite as ResourceObject_Create fills it, laid out as resource_obj.c
/// declares it.
struct ResourceSprite {
  u32 imageAttributes[3];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 unk11 : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  u32 shadowAttributes[3];
  struct AnimationEntry *entries[4];
};

void Func_02001738(struct Actor *object) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  // The parts' actor type, held from the top: global allocation leaves it in
  // no register and reload rebuilds 0x15f in r0 at each call, as the native
  // does; as the literal, its pseudo takes r0 before the first call's
  // arguments are built (differ 155).
  s32 type = 0x15f;

  if ((gFrameCount & 15) == 0)
    Func_02003c4c(object, 136);
  Actor_SetPartAttribute(object, 10);
  if (object->action <= 31) {
    u32 phase = gFrameCount & 3;

    if (phase == 0) {
      struct MagmaRockEffectConfig config;
      s32 dx;
      s32 dz;

      config.id = 403;
      config.variant = 10;
      config.kind = 2;
      config.unk24 = Func_020011b8;
      config.data = gData_02003d50;
      dx = Random16Far() & 15;
      dz = Random16Far() & 15;
      Func_0200015c(object->x + 0x10000, object->y + 0x180000, object->z,
                    (dx - 8) << 14, 0, (dz - 8) << 14, 0x1330000, &config);
    }
  } else if (object->action == 33) {
    struct Actor *parts[4];
    struct Actor *fifthPart;
    s32 id;
    s32 i;

    Func_02003c4c(object, 144);
    parts[0] = Import_ActorCreate(type, object->x + 0x10000,
                                  object->y + 0x200000, object->z);
    for (i = 1; i != 4; i++) {
      struct ResourceSprite *first = parts[0]->animation;
      struct Actor *part = Import_ActorCreate(type, 0, 0, 0);
      struct ResourceSprite *sprite;

      parts[i] = part;
      sprite = part->animation;
      sprite->slot = first->slot;
      sprite->retainCacheEntry = 1;
      part->x = parts[0]->x;
      part->y = parts[0]->y;
      part->z = parts[0]->z;
      part->motionFlags = 0;
      Actor_SetSingleChildFlags(part, 0);
      Func_02003a54(parts[i], 3);
      /* FAKEMATCH(dead): part set again to parts[i], which it holds; as
       * parts[i]-> for the uses below, 65 of 294 instructions differ in
       * tla-en. */
      part = parts[i];
      // Byte 98 lies in actor.h's callback. Written as a field, the value is
      // computed before its address, as the native does (through a u8 *,
      // differ 34).
      part->unk5c[6] = 3 - i;
      part->watchedObject = object;
      part->update = Func_02001528;
      Actor_SetPartAttribute(part, 3);
    }
    {
      struct Actor *part = parts[0];

      part->motionFlags = 0;
      Actor_SetSingleChildFlags(part, 0);
      Func_02003a54(parts[0], 3);
      Scene_SetActorSpritePriority(parts[0], 3);
      /* FAKEMATCH(dead): part set again to parts[0], which it holds; as
       * parts[0]-> for the uses below, 45 of 294 instructions differ in
       * tla-en. */
      part = parts[0];
      part->unk5c[6] = 3;
      part->watchedObject = object;
      part->update = Func_02001528;
      Actor_SetPartAttribute(part, 3);
    }
    id = 0;
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
      id = Func_020010cc(object->x, object->z, gData_02003d9e);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
      // The native loads x and z once, ahead of the entrance tests.
      s16 entrance = gMainRoutineWork.entrance;
      s32 x = object->x;
      s32 z = object->z;

      if (entrance <= 3)
        id = Func_020010cc(x, z, gData_02003db2);
      else if (entrance <= 8)
        id = Func_020010cc(x, z, gData_02003dc4);
      else if (entrance <= 12)
        id = Func_020010cc(x, z, gData_02003dd6);
      else
        id = Func_020010cc(x, z, gData_02003dee);
    }
    fifthPart = Actor_GetById(id);
    fifthPart->unk5c[6] = 4;
    fifthPart->watchedObject = object;
    fifthPart->update = Func_020015e4;
    object->action++;
  } else if (object->action > 364) {
    Actor_SetPartAttribute(object, 0);
    object->update = NULL;
  }
  // screenStatus read unsigned: as work->screenStatus, 39 of 294 instructions
  // differ in tla-en.
  if (work->unk166 == 0 && *(s16 *)&work->unk15a == 0 &&
      *(s16 *)&work->unk15c[2] == 0 && *(s16 *)work->unk168 == 0 &&
      (u16)work->screenStatus != 0)
    object->action++;
}
