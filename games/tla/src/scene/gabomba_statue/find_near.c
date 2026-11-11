#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct Orbiter {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[15];
  u8 flags23;
  u8 unk24[44];
  void **animation;
  u8 kind;
  u8 unk55[13];
  u8 phase;
  u8 unk63;
  s16 centerX;
  s16 centerZ;
  void *link;
};

extern s32 gData_02005af0;
extern const s8 gData_02006b38[];
extern const u8 gData_02006b20[];
extern const u8 gData_02005fd4[];
extern const u8 gData_02006004[];
extern const u8 gData_02006244[];
extern const u8 gData_020064cc[];
extern const u8 gData_020067e4[];
extern const u8 gData_02006898[];
extern const u8 gData_02006a90[];
s32 Func_02005834(s32 value);
void Func_020020f0(s32 value);

#define DIFF_WITHIN(a, b, limit)                                               \
  ((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))

void Func_02000550(void) {
  struct Orbiter *actor = ((struct Orbiter *)Actor_Get(9));
  u16 zero = 0;
  s32 direction;
  s32 i;

  if (Func_02005834(0) == 1) {
    direction = gData_02005af0;
    Func_020020f0(0);
    Event_SetFlag(515);
    /* FAKEMATCH(temporary): the 0 stored from a u16 local holding it, so it
     * is built in a register (movs, strh); as the constant 0 stored directly,
     * it loads from the pool and 11 instructions differ. */
    *(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x166) = zero;
    i = 0;
  loop:
    actor->phase += gData_02006b38[i] * direction;
    System_Wait(1);
    if (++i <= 24)
      goto loop;
  }
}

struct Orbiter *Func_020005b4(struct Orbiter *origin, s32 id) {
  s32 i;

  for (i = 8; i < 64; i++) {
    struct Orbiter *object = ((struct Orbiter *)Actor_Get(i));

    if (object != NULL && object->kind == 1 && object->animation[10] != NULL &&
        *(s16 *)object->animation[10] == id) {
      if (DIFF_WITHIN(object->x, origin->x, 0x17ffff) &&
          DIFF_WITHIN(object->z, origin->z, 0x17ffff))
        return object;
    }
  }
  return NULL;
}

/// As Func_020005b4, but around the point 16 pixels ahead of origin in its
/// facing.
struct Orbiter *Func_02000624(struct Orbiter *origin, s32 id) {
  s32 facing;
  s32 position[3];
  s32 i;

  position[0] = origin->x;
  position[2] = origin->z;
  /* FAKEMATCH(temporary): the facing read into a local before the call; as
   * origin->facing in the call, 9 of 63 instructions differ in tla-en. */
  facing = origin->facing;
  FieldVector_AddPolarOffset(0x100000, facing, position);
  for (i = 8; i < 64; i++) {
    struct Orbiter *object = ((struct Orbiter *)Actor_Get(i));

    if (object != NULL && object->kind == 1 && object->animation[10] != NULL &&
        *(s16 *)object->animation[10] == id) {
      if (DIFF_WITHIN(object->x, position[0], 0x17ffff) &&
          DIFF_WITHIN(object->z, position[2], 0x17ffff))
        return object;
    }
  }
  return NULL;
}
