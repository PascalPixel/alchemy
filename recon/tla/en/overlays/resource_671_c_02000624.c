// Matches: differ 0 of 63 (tla-en, 2026-11-06).
// Ported from Func_020005b4 (find_near.c). Earlier drafts (22 of 63) kept a
// one-pass block and pinned the vector pointer to r4 against GCSE carrying its
// address across the loop; the native stores z before the call, and with that
// order and the facing in a local neither device is needed.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "main_routine_work.h"

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
struct Orbiter *Actor_GetById(s32 id);
s32 Func_02005834(s32 value);
void Func_020020f0(s32 value);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);

#define DIFF_WITHIN(a, b, limit)                                               \
  ((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);

struct Orbiter *Func_02000624(struct Orbiter *origin, s32 id) {
  s32 facing;
  s32 position[3];
  s32 i;

  position[0] = origin->x;
  position[2] = origin->z;
  /* FAKEMATCH(temporary): the facing read into a local before the call; as
   * origin->facing in the call, 9 of 63 instructions differ in tla-en. */
  facing = origin->facing;
  Vector_AddPolarOffsetFar(0x100000, facing, position);
  for (i = 8; i < 64; i++) {
    struct Orbiter *object = Actor_GetById(i);

    if (object != NULL && object->kind == 1 && object->animation[10] != NULL &&
        *(s16 *)object->animation[10] == id) {
      if (DIFF_WITHIN(object->x, position[0], 0x17ffff) &&
          DIFF_WITHIN(object->z, position[2], 0x17ffff))
        return object;
    }
  }
  return NULL;
}
