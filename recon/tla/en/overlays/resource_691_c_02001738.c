// Near miss, differ 8 of 294 (tla-en, 2026-11-06): one scheduling choice in the
// loop. The native moves `str r0, [r7, r5]` (parts[i] = part) past the sprite
// bytes' first[16] load, sprite[16] store and sprite[17] load; here it stays
// ahead of them, held by the char alias. Through a struct (a vramBlock byte
// and a keepEntry bit at 17, as drafts 6ab and 6ac) the store does drift, but
// so does the sprite[17] load and parts[0]'s reload (differ 12; FieldSprite
// from field_sprite.h, differ 14); storing parts[i] after the sprite bytes
// gives differ 237.
//
// The earlier reload rotation (differ 113) came from three expansions, each
// now fixed in plain C: the type 0x15f as a literal kept a pseudo in r0 across
// the first Import_ActorCreate's argument loads, so no insn's find_reg picked
// r0 and r0 never entered spill_regs; a local `last = 3` from the top was
// CSE'd into gFrameCount & 3, which then reloaded the 3 instead of giving it a
// register; and the order stored through a u8 * computed its address before
// its value. With the type held in a local and the order stored as a field the
// rotation is the native's, and the old tagged `last` device is gone
// (the literal 3 now measures the same).
//
// The update of an object that calls Func_02003c4c(object, 136) every 16
// frames and sets its part attribute 10 each frame. While its action counter is at most 31
// it throws effect 403 every fourth frame; at 33 it calls Func_02003c4c with
// 144, builds four parts (type 0x15f) above it, each sharing the first part's
// sprite, ordered 3 down to 0 and taking Func_02001528, then hands the object
// that Func_020010cc finds in Ov691Room1, or in Ov691Room2 by entrance, to
// Func_020015e4; past 364 it clears its update. The counter advances only while
// event work 0x166, 0x15a, 0x15e and 0x168 are 0 and screenStatus is not.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "field/event_work.h"
#include "scene_ids.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"

/// The native reads these event work halfwords signed (ldrsh).
#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

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
      // The native passes the zero phase for the fifth argument.
      Func_0200015c(object->x + 0x10000, object->y + 0x180000, object->z,
                    (dx - 8) << 14, phase, (dz - 8) << 14, 0x1330000,
                    &config);
    }
  } else if (object->action == 33) {
    struct Actor *parts[4];
    struct Actor *helper;
    s32 id;
    s32 i;

    Func_02003c4c(object, 144);
    parts[0] = Import_ActorCreate(type, object->x + 0x10000,
                                  object->y + 0x200000, object->z);
    for (i = 1; i != 4; i++) {
      u8 *first = parts[0]->animation;
      struct Actor *part = Import_ActorCreate(type, 0, 0, 0);
      u8 *sprite;

      parts[i] = part;
      sprite = part->animation;
      sprite[16] = first[16];
      sprite[17] |= 1;
      part->x = parts[0]->x;
      part->y = parts[0]->y;
      part->z = parts[0]->z;
      part->motionFlags = 0;
      Actor_SetSingleChildFlags(part, 0);
      Func_02003a54(parts[i], 3);
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
    helper = Actor_GetById(id);
    helper->unk5c[6] = 4;
    helper->watchedObject = object;
    helper->update = Func_020015e4;
    object->action++;
  } else if (object->action > 364) {
    Actor_SetPartAttribute(object, 0);
    object->update = NULL;
  }
  if (work->unk166 == 0 && S16_AT(work, 0x15a) == 0 &&
      S16_AT(work, 0x15e) == 0 && S16_AT(work, 0x168) == 0 &&
      (u16)work->screenStatus != 0)
    object->action++;
}
