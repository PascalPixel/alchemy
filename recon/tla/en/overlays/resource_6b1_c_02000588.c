// Matches: differ 0 (tla-en, 2026-11-02)
// Func_02000588 for every edition: games/tla/src/scene/
// anemos_inner_sanctum_entrance/s0588.c with its LANGUAGE guard moved from the
// whole file to the centring MoveTo call, which Japanese lacks. This is what
// lets the Japanese form leave its listing section under TLA_EDITION_JA.
// Measured: differ 0 in tla-en, de, es, fr and it (157 of 157). tla-ja reads
// differ 1 (145 of 146) and that one is not a miss: the listing has no label for
// Func_02000710, so its bx lr counts inside this symbol. The draft's own
// Func_02000710 is that instruction. Checked by hand in tla-ja: the pool is the
// listing's order (gMainRoutineWork, gData_02000eec, -0x80000, Func_020004b4,
// -0x100000) and .text is 0x170, the listing's 0x02008588 to 0x020086f8.
// Left: nothing to match. The MoveTo wrapper is the adopted file's device,
// re-measured here at 6 of 157 in tla-en and no effect in tla-ja.
#include "edition.h"
#include "../../../../games/tla/src/scene/anemos_inner_sanctum_entrance/anemos_inner_sanctum_entrance.h"

extern const s8 gData_02000eec[];

void Func_02000388(void);
void Func_020004b4(void);
void Func_02000c74(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02000c7c(void);
s32 Func_02000c8c(s32 layer, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 a);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
u32 Random16Far(void);

static __inline__ void MoveTo(struct Actor *object, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): MoveTo forwarded inline; as direct calls to
   * Func_02000c74, 6 of 157 instructions differ in tla-en. */
  Func_02000c74(object, x, y, z);
}

void Func_02000588(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 shift = 0x1300000;
  s32 i;

  if (object->z >> 20 <= 9)
    shift = -shift;
  Func_02000d44(0);
  if (!GameFlag_Test(0x200))
    return;
  if (Func_02000c8c(object->terrainId, object->x, object->z) != 77)
    return;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  MoveTo(object, (object->x & 0xfff00000) + 0x80000, object->y,
         (object->z & 0xfff00000) + 0x80000);
#endif
  {
    s32 id = gMainRoutineWork.currentOwner;

    if (shift > 0)
      Actor_ArmCallback(id, 0xc000, 0);
    else
      Actor_ArmCallback(id, 0x4000, 0);
  }
  wait(10);
  i = 0;
next:
  {
    object = Actor_GetById(gData_02000eec[i]);
    object->motionFlags = 0;
    object->y += 0x8000;
    object->acceleration = 0x1000;
    object->speedLimit = 0x20000;
    Actor_SetSingleChildFlags(object, 1);
    Func_02000c74(object, object->x, 0xfff80000, object->z);
    object->update = Func_020004b4;
    Func_02000d64(152);
    wait((Random16Far() * 4 >> 16) + 3);
  }
  if (++i <= 6)
    goto next;
  wait(40);
  Func_02000d64(198);
  i = 9;
lower:
  {
    object = Actor_GetById(i);
    {
      s32 z = object->z - shift;

      Func_02000c74(object, object->x, 0xfff80000, z);
    }
  }
  if (++i <= 14)
    goto lower;
  Actor_GetById(9);
  Func_02000c7c();
  wait(40);
  i = 9;
land:
  {
    object = Actor_GetById(i);
    object->z = (object->z & 0xfff00000) + 0x80000;
    object->motionFlags = 3;
    object->update = 0;
  }
  if (++i <= 14)
    goto land;
  wait(3);
  Func_02000d64(188);
  wait(10);
  if (!GameFlag_Test(0xa9f) && shift > 0)
    Func_02000388();
}

void Func_02000710(void) {}
