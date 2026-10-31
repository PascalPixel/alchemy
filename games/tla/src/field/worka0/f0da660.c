#include "worka0.h"
#include "actor.h"
#include "system.h"

extern s32 gData_080f394c;
s32 Trig_Sin(s32 angle);
void Func_080da6e0(s32 arg, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                   s32 wave);

void Func_080da660(s32 arg, s32 firstId, s32 secondId) {
  struct Actor **work;
  struct Actor *first;
  struct Actor *second;
  s32 index;

  /* FAKEMATCH(one-pass): the empty block stops the scheduler moving the heap
   * slot address above the third argument's spill; as straight-line code, 2
   * of 57 instructions differ in tla-en. */
  do {
  } while (0);
  work = (struct Actor **)WORK_A0;
  /* FAKEMATCH(temporary): arg copied into index after the block, so its copy
   * to r10 follows the slot address; as arg passed directly, 2 of 57
   * instructions differ in tla-en. */
  index = arg;
  /* FAKEMATCH(dead): the dead store keeps GCSE from propagating arg back into
   * index's uses; without it, 2 of 57 instructions differ in tla-en. */
  arg = 0;
  first = ObjectTable_Get(firstId);
  second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    work[0] = first;
    work[1] = second;
    Func_080da6e0(index, first->x, first->y + 0x80000, first->z, second->x,
                  second->y + 0x80000, second->z,
                  Trig_Sin(gData_080f394c) << 2);
    gData_080f394c += 0x1000;
  }
}
