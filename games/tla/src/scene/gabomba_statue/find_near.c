#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

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
struct Orbiter *Object_GetById(s32 id);
s32 Func_02005834(s32 value);
void Func_020020f0(s32 value);
void GameFlag_SetBit(s32 flag);
void WaitTicks(s32 ticks);

#define DIFF_WITHIN(a, b, limit)                                               \
  ((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))

void Func_02000550(void) {
  struct Orbiter *actor = Object_GetById(9);
  u16 zero = 0;
  s32 direction;
  s32 i;

  /* FAKEMATCH: taking actor's address keeps GCSE from hoisting the phase's
   * address out of the loop, which the native rebuilds on every pass (differ 25
   * without it). */
  (void)&actor;
  if (Func_02005834(0) == 1) {
    direction = gData_02005af0;
    Func_020020f0(0);
    GameFlag_SetBit(515);
    /* FAKEMATCH: a u16 local stores its 0 from a register (movs, strh); the
     * constant would load from the pool (differ 11). */
    *(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x166) = zero;
    i = 0;
  loop:
    actor->phase += gData_02006b38[i] * direction;
    WaitTicks(1);
    if (++i <= 24)
      goto loop;
  }
}

struct Orbiter *Func_020005b4(struct Orbiter *origin, s32 id) {
  s32 i;

  for (i = 8; i < 64; i++) {
    struct Orbiter *object = Object_GetById(i);

    if (object != NULL && object->kind == 1 && object->animation[10] != NULL &&
        *(s16 *)object->animation[10] == id) {
      if (DIFF_WITHIN(object->x, origin->x, 0x17ffff) &&
          DIFF_WITHIN(object->z, origin->z, 0x17ffff))
        return object;
    }
  }
  return NULL;
}
