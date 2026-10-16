// Draft, differ 47 of 63: ported from Func_020005b4 (find_near.c). The native
// rebuilds the vector's stack address (mov r4, sp) inside the loop and keeps
// only i and the id in r5 and r6; here GCSE's PRE carries the first block's
// address pseudo across the loop in a callee-saved register (r5), which shifts
// i and id to r6 and r7. Tried: a struct vector, a pointer local inside the
// loop (or in the innermost block), i set before the call, taking the address
// of i, id or origin, permuter 4 minutes (no gain). Func_02000550 shows the
// same habit; a goto loop with (void)&actor stopped it there. Also tried:
// (void)& of the position, the object, i, origin and id (47 to 52); the loop's
// pointer pinned by an empty asm (40: it copies the hoisted register); a goto
// loop (71, where <= 0x17ffff becomes < 0x180000). The gcse dump shows PRE
// making the loop's (sfp - 12) redundant with the one before the call.
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
void WaitFrames(s32 frames);

#define DIFF_WITHIN(a, b, limit)                                               \
  ((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);

struct Orbiter *Func_02000624(struct Orbiter *origin, s32 id) {
  s32 position[3];
  s32 i;

  position[0] = origin->x;
  position[2] = origin->z;
  Vector_AddPolarOffsetFar(0x100000, origin->facing, position);
  for (i = 8; i < 64; i++) {
    struct Orbiter *object = Object_GetById(i);

    if (object != NULL && object->kind == 1 && object->animation[10] != NULL &&
        *(s16 *)object->animation[10] == id) {
      if (DIFF_WITHIN(object->x, position[0], 0x17ffff) &&
          DIFF_WITHIN(object->z, position[2], 0x17ffff))
        return object;
    }
  }
  return NULL;
}
