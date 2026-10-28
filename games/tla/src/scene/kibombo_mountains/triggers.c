#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02001144(s32 a, s32 b);
void Func_0200114c(void);
void Func_0200106c(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
void Func_0200103c(struct Actor *object, s32 value);
void Func_02000da8(s32 value);
void Func_02000e28(s32 value);

void Func_02000f40(void) { Func_02000da8(11); }

void Func_02000f4c(void) { Func_02000da8(11); }

void Func_02000f58(void) {
  struct SequenceParentWork *party = &gSequenceParentWork;

  /* FAKEMATCH: ⚓️ loads gSequenceParentWork before it builds the offset */
  asm volatile("" : "+l"(party));
  if (party->renderMode != 3)
    Func_02000da8(12);
}

void Func_02000f78(void) { Func_02000da8(12); }

void Func_02000f84(void) { Func_02000da8(8); }

void Func_02000f90(void) { Func_02000da8(8); }

void Func_02000f9c(void) {
  if (!GameFlag_Test(514) && !GameFlag_Test(513))
    Func_02000e28(10);
}

void Func_02000fc4(void) {
  if (!GameFlag_Test(512))
    Func_02000da8(9);
}

void Func_02000fdc(void) {
  if (!GameFlag_Test(512))
    Func_02000da8(9);
}

void Func_02000ff4(void) {
  if (!GameFlag_Test(515) && !GameFlag_Test(514))
    Func_02000e28(10);
}
