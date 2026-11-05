// Draft, differ 149 of 216, skeleton gap 10 and 7 (tla-en, 2026-11-01)
// All control flow is here; what is left is allocation. Two faults remain.
// 1. j goes to r2 with caller-saves (13 refs against 3 calls, 4 * 3 < 13), so
// the listing's r10, the eighth save and the r8 push are missing. No loop
// shape changed the count: life counts loop depth from the CFG, so a goto loop
// weighs the same. It needs 12 refs or a fourth call crossed.
// 2. The first loop's load at its head is dropped: the test's *p is reused
// through a copy, where the listing reloads with ldr r0, [r5].
// struct s is a stand-in: as plain locals count and speed win hi registers
// and list is spilled (gap 32 and 22). The listing's r2/r1 rotation looks like
// reload's, so they were probably plain locals beaten for hi registers.
// The goto loops stop loop.c: as for loops the pool load and 0 hoist out and i
// is strength-reduced, which the listing does not do. The outer loop stays a
// for: as a goto, CSE merges 0x17fff with 0x18000.
#include "types.h"
#include "call.h"
struct SceneActor {
  u8 pad0[16];
  s32 unk_10;
  u8 pad14[35 - 20];
  u8 unk_23;
  u8 pad24[64 - 36];
  s32 unk_40;
  u8 pad44[85 - 68];
  u8 unk_55;
};
struct SceneWork {
  u8 pad0[12];
  s32 unk_0c;
};
struct FieldWork {
  u8 pad0[0x178];
  struct SceneWork unk_178;
};
struct Iwram {
  u8 pad0[32];
  struct FieldWork *field;
};
void Camera_FollowActor(s32 id, s32 flag);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02005884(s32, s32, s32);
void Func_020059f4(s32);
void Actor_SetSpritePriority(u32, s32);
struct SceneActor *Actor_GetById(s32);
void wait(s32);
extern s32 Data_02006a1c;
extern s32 gData_020059fc[];
extern s32 gData_02005a0c[];
extern s32 gData_02005a14[];
extern s32 gData_02005a1c[];
extern s32 gData_020069dc[];

void Func_02003a6c(s32 mode) {
  struct { u32 count; s32 speed; } s;
  s32 *list;
  s32 *p;
  s32 *q;
  u32 i;
  u32 j;
  struct SceneWork *work;

  work = &((struct Iwram *)0x3000000)->field->unk_178;
  s.speed = 0x1999;
  s.count = 0;
  Data_02006a1c = work->unk_0c;
  switch (mode) {
  case 0:
  case 3:
    list = gData_020059fc;
    break;
  case 1:
    list = gData_02005a0c;
    break;
  case 2:
    list = gData_02005a14;
    break;
  default:
    list = gData_02005a1c;
    break;
  }

  i = 0;
  if (list[0] != 64) {
    p = list;
  L0:
    Actor_GetById(*p)->unk_55 = 0;
    Actor_SetSpritePriority(*p++, 1);
    s.count++;
    i++;
    if (i > 4)
      goto L7;
    if (*p != 64)
      goto L0;
  L7:;
  }

  i = 0;
L1:
  Actor_GetById(gData_020069dc[i])->unk_23 |= 32;
  Actor_SetSpritePriority(gData_020069dc[i], 0);
  i++;
  if (i <= 5)
    goto L1;

  Func_020059f4(223);

  for (i = 0; i <= 227; i++) {
    work->unk_0c -= s.speed;
    j = 0;
    if (j < s.count) {
      q = list;
    L4:
      Actor_GetById(*q)->unk_10 += s.speed;
      Actor_GetById(*q)->unk_40 = Actor_GetById(*q++)->unk_10;
      /* FAKEMATCH(barrier): an empty asm holds the order around it; as no asm,
       * 10 of 216 instructions differ in tla-en. */
      asm("");
      j++;
      if (j < s.count)
        goto L4;
    }
    if ((i & 3) == 3)
      s.speed += 0x1999;
    if (s.speed > 0x17fff)
      s.speed = 0x18000;
    wait(1);
    if (mode == 0) {
      if (i == 40)
        Camera_MoveWithinMapBounds(128 << 18, 208 << 15, 152 << 17, 1);
      if (i == 120)
        break;
    } else if (mode == 1) {
      if (i == 40)
        Camera_MoveWithinMapBounds(128 << 18, 208 << 15, 176 << 17, 1);
      if (i == 120)
        break;
    } else if (mode == 3) {
      if (i == 40)
        Camera_FollowActor(0, 1);
      if (i == 120)
        break;
    }
  }

  i = 0;
L2:
  Actor_GetById(gData_020069dc[i])->unk_23 &= 223;
  Actor_SetSpritePriority(gData_020069dc[i], 1);
  i++;
  if (i <= 5)
    goto L2;

  i = 0;
  if (i < s.count) {
    p = list;
  L6:
    Func_02005884(*p, 0, 0);
    /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
     * order around it; as the plain statement, 12 of 216 instructions differ
     * in tla-en. */
    do {
      Actor_GetById(*p++)->unk_55 = 0;
    } while (0);
    /* FAKEMATCH(barrier): an empty asm holds the order around it; as no asm,
     * 8 of 216 instructions differ in tla-en. */
    asm("");
    i++;
    if (i < s.count)
      goto L6;
  }
}
