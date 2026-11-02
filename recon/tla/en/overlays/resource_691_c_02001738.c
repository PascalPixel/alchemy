// Draft, differ 118 of 294, skeleton gap 18 (tla-en, 2026-11-01)
//
// The code is the native's; what is left is one allocation question and the
// reload and scheduling that follow from it. The native keeps the event work
// in r9 and the action pointer in fp, and it recomputes object + 100 at the
// closing object->action++. GCSE's PRE deletes that recomputation here (insn
// dump: "redundant insn ... reaching reg"), so the pointer has 6 refs like the
// event work, a shorter life, and outranks it: set after the second call, as
// the native does, the pair swaps (differ 136 with the body below). The local
// `action`, set at its declaration, lengthens the pointer's life enough to put
// the pair right, but the set sits before the first test (native: after
// Actor_SetPartAttribute), which brings an extra reload there and rotates the
// scratch registers of everything after it: r2 for the 15 (native r0), r1 and
// r4 for the ldrsh offsets (native r2, then r4, r0, r1, r2), r4 for the order's
// 3 (native r3). What would settle it: why PRE leaves the closing increment
// alone. The expression is available on every path, gcse.c's only ways out are
// a set of object between or a different expression, and neither shows in the
// listing. A second copy of object (a twice-set local) is cprop'd away and is a
// device anyway.
// Also open: the native moves `str r0, [r7, r5]` past the sprite bytes' strb
// and ldrb. Through struct fields rather than u8 * the store is free of the
// char alias and drifts as it does (struct with a keep bit at byte 17, as drafts
// 6ab and 6ac: differ 122 to 126); that is probably the real type, but the
// measure is worse until the pointer settles. Tried with no gain: storing
// parts[i] after sprite[16] or sprite[17] (the shift takes first's r5, differ
// 159 to 255), a slot pointer (adds r5, r7, r5), an offset local, parts[i]
// throughout (reloads each use). Removing `last` gives differ 158.
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
#include "actor.h"
#include "field/event_work.h"
#include "scene_ids.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
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
  s16 *action = &object->action;
  /* FAKEMATCH(temporary): the last part's order held in a variable from the
   * top, which spills and is reloaded as the constant at each use; as the
   * literal 3 in both stores, CSE keeps the loop's 3 in a register across the
   * calls after the loop and 158 of 294 instructions differ in tla-en. */
  s32 last = 3;

  if ((gFrameCount & 15) == 0)
    Func_02003c4c(object, 136);
  Actor_SetPartAttribute(object, 10);
  if (*action <= 31) {
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
  } else if (*action == 33) {
    struct Actor *parts[4];
    struct Actor *helper;
    s32 id;
    s32 i;

    Func_02003c4c(object, 144);
    parts[0] = Import_ActorCreate(0x15f, object->x + 0x10000,
                                  object->y + 0x200000, object->z);
    i = 1;
    // A goto loop, as the native: as a do-while loop.c strength-reduces
    // parts[i] to a walking pointer (ldmia) and spills more (differ 293).
  next:
  {
    u8 *first = parts[0]->animation;
    struct Actor *part = Import_ActorCreate(0x15f, 0, 0, 0);
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
    BYTE_AT(part, 98) = last - i;
    part->watchedObject = object;
    part->update = Func_02001528;
    Actor_SetPartAttribute(part, 3);
  }
    if (++i != 4)
      goto next;
    {
      struct Actor *part = parts[0];

      part->motionFlags = 0;
      Actor_SetSingleChildFlags(part, 0);
      Func_02003a54(parts[0], 3);
      Scene_SetActorSpritePriority(parts[0], 3);
      part = parts[0];
      BYTE_AT(part, 98) = last;
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
    BYTE_AT(helper, 98) = 4;
    helper->watchedObject = object;
    helper->update = Func_020015e4;
    (*action)++;
  } else if (*action > 364) {
    Actor_SetPartAttribute(object, 0);
    object->update = NULL;
  }
  if (work->unk166 == 0 && S16_AT(work, 0x15a) == 0 &&
      S16_AT(work, 0x15e) == 0 && S16_AT(work, 0x168) == 0 &&
      (u16)work->screenStatus != 0)
    object->action++;
}
