#include "types.h"
#include "field_event.h"

extern const u8 gData_02003620[];

struct FieldActor *Func_020029ec(s32 type, s32 x, s32 y, s32 z);
void Func_020029dc(struct FieldActor *object, s32 mode);
void Func_020029e4(struct FieldActor *object, const void *script);
struct FieldActor *Object_GetById(s32 id);
void Func_02002bc4(s32 cue);
void Battle_WaitMode0(s32 frames);

void Func_02000c40(s32 x, s32 y, s32 z, s32 angle, s32 frames);

void Func_02000c90(s32 firstId, s32 secondId) {
  struct FieldActor *first = Object_GetById(firstId);
  struct FieldActor *second = Object_GetById(secondId);
  s32 rise = 0x1c0000;
  s32 y;

  y = first->y;
  /* FAKEMATCH: the barrier holds r3 until the height is read, so the angle is
   * started after it as the native has; plain, the angle is started first (one
   * reordered row). */
  __asm__("" : "+l"(y) : : "r3");
  Func_02000c40(first->x, y + rise, first->z, 0x1000, 15);
  Func_02002bc4(151);
  Battle_WaitMode0(15);
  y = second->y;
  /* FAKEMATCH: the same barrier for the second angle. */
  __asm__("" : "+l"(y) : : "r3");
  Func_02000c40(second->x, y + rise, second->z, 0xf000, 15);
  Func_02002bc4(151);
  Battle_WaitMode0(15);
  Func_02000c40(first->x, first->y + rise, first->z, 0x1000, 15);
  Func_02002bc4(151);
  Battle_WaitMode0(15);
  Func_02000c40(second->x, second->y + rise, second->z, 0xf000, 15);
  Func_02002bc4(151);
  Battle_WaitMode0(15);
}
