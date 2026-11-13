/// @file
/// Scene code of overlay 0x6ac, in ROM order.

#include "../shared/mars_lighthouse.h"
#include "../mars_lighthouse_room_m.h"
#include "event_inline.h"
#include "module/field/field.h"
#include "module/scene/dehkan_plateau.h"
#include "module/scene/mars_lighthouse_room_m.h"
#include "module/scene/scene.h"

void Func_02002118(void) {
  struct MarsLighthouseRoomMRideWork *work =
      (struct MarsLighthouseRoomMRideWork *)(gInventorySnapshot + 0x188);
  struct Actor *object;
  struct MarsLighthouseRoomMAnimation *animation;

  if (!Event_GetFlag(0x109))
    Iwram_ClearWordsEntry(work, 116);
  object = (struct Actor *)FieldActor_Create(365, 1, 0, 0);
  object->unk5c[0] = 2;
  Func_02003118(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}

#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"

struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
  s32 d;
};

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

struct RideSlot {
  u16 value;
  u8 unk02[2];
};

struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[12];
  void *handle;
};

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
#include "main_routine_work.h"

DEFINE_RIDE_RELEASE_HANDLE(Func_0200216c, Func_02003130)

void Func_02002180(s16 value) {
  ((struct RideWork *)(gInventorySnapshot + 0x188))->value0e = value;
}

#include "io_reg.h"
#include "field.h"

DEFINE_BEAM_START(Func_0200218c, Func_02001f94)

s32 Func_020022c0(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a;
}

void Func_020022d0(s32 index, s16 value) {
  ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a = value;
}

s32 Func_020022e0(void) {
  return ((struct RideWork *)(gInventorySnapshot + 0x188))->counter;
}

DEFINE_RIDE_ADVANCE_COUNTER(Func_020022ec)

DEFINE_RIDE_SET_ENTRY(Func_02002314)

void Func_02002334(s32 index, s16 value) {
  ((struct RideWork *)(gInventorySnapshot + 0x188))->entries[index].c = value;
}

s32 Func_02002344(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->b;
}

DEFINE_RIDE_SET_SLOT(Func_02002354)

#include "battle_unit.h"

struct MarsLighthouseEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
  u16 downCount;
  u16 standingCount;
  u16 downOwners[8];
};

struct BattleUnit *Unit_Get(s32 owner);

DEFINE_PARTY_TAKE_FALL_DAMAGE(Func_02002368)

#include "../../field/field_effect/advance_spin.c"

DEFINE_ACTOR_BLINK(Func_020024bc, 10, 4, 0)

#include "version.h"

#include "call.h"
#include "vector3.h"
#include "fixed_math.h"

s32 Func_02003168(s32 terrain, s32 x, s32 z);
/* VIEW(s2118): routine Func_02003180 as s32 Func_02003180(struct Actor *actor,
 * struct Vector3 *position); module/scene/scene.h has s32 Func_02003180(s32
 * layer, s32 shape, s32 height); tried as the rest of the object declares it,
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_m/s2118.c:292: too few arguments to
 * function `Func_02003180'. */
extern s32
Func_02003180__s2118(struct Actor *actor,
                     struct Vector3 *position) __asm__("Func_02003180");

#define SHARED_F02002148_NAME Func_020024e4
#define SHARED_F02002148_ID2 Func_02002368
#define SHARED_F02002148_ID3 Func_020024bc
#define SHARED_F02002148_ID4 Func_02003118
#define SHARED_F02002148_ID5 Func_02003168
#define SHARED_F02002148_ID6 Func_02003180__s2118
#define SHARED_F02002148_ID7 Import_FieldCheckSpheresOverlap
#include "../shared/func_02002148.c"
#undef SHARED_F02002148_NAME
#undef SHARED_F02002148_ID2
#undef SHARED_F02002148_ID3
#undef SHARED_F02002148_ID4
#undef SHARED_F02002148_ID5
#undef SHARED_F02002148_ID6
#undef SHARED_F02002148_ID7

void Func_020026f0(void) {
  if (((struct RideWork *)(gInventorySnapshot + 0x188))->active)
    Func_020024e4();
}
