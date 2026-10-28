// Func_02000064, Func_020000e0, Func_02000d08 and Func_02000d3c load scene ids
// 0xea and 0xeb from the literal pool: they wait for SceneId_ names in a ⚓️
// scenes.s table (O3). Func_02000c14 differ 10: the native reloads
// gFrameCount's address for the second test instead of keeping it in r1.
#include "types.h"
#include "sequence_parent_work.h"
#include "field_event.h"
#include "field_sprite.h"

extern u8 gData_02001194[];
extern u8 gData_020011ac[];
extern u8 gData_020012cc[];
extern u8 gData_0200147c[];
extern u8 gData_0200159c[];
extern u8 gData_0200165c[];
extern u8 gData_0200171c[];
extern u8 gData_02001728[];
extern u8 gData_020018f0[];
extern u32 gFrameCount;

void Func_02001110(s32 a, s32 b, s32 c);
void GameFlag_ClearBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void Func_0200097c(void);
void Func_02000dc4(void);
void Func_02000ed0(void);
struct FieldActor *Actor_GetById(s32 id);
void Actor_SetSpritePriority(u32, s32);
void Func_02000ff0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

u8 *Func_02000064(void) {
  s16 scene = gSequenceParentWork.scene;

  if (scene == 0xea) {
    if (GameFlag_Test(0x9b2))
      return gData_0200147c;
    if (GameFlag_Test(0x9d0))
      return gData_020012cc;
    return gData_020011ac;
  }
  if (scene == 0xeb) {
    if (GameFlag_Test(0x9d0))
      return gData_0200159c;
    return gData_0200165c;
  }
  return gData_02001194;
}

u8 *Func_020000e0(void) {
  s16 scene = gSequenceParentWork.scene;

  if (scene == 0xea)
    return gData_02001728;
  if (scene == 0xeb)
    return gData_020018f0;
  return gData_0200171c;
}

void Func_02000c14(void) {
  s32 i;
  s16 first;

  if ((gFrameCount & 7) == 0) {
    first = ((volatile u16 *)0x05000160)[9];
    i = 9;
  rotate_a:
    ((volatile u16 *)0x05000160)[i] = ((volatile u16 *)0x05000160)[i - 1];
    i--;
    if (i != 4)
      goto rotate_a;
    ((volatile u16 *)0x05000160)[4] = first;
  }
  if ((gFrameCount & 7) == 0) {
    first = ((volatile u16 *)0x05000160)[15];
    i = 15;
  rotate_b:
    ((volatile u16 *)0x05000160)[i] = ((volatile u16 *)0x05000160)[i - 1];
    i--;
    if (i != 12)
      goto rotate_b;
    ((volatile u16 *)0x05000160)[12] = first;
  }
}

s32 Func_02000d08(void) {
  s16 scene = gSequenceParentWork.scene;

  if (scene == 0xea)
    Func_02000dc4();
  else if (scene == 0xeb)
    Func_02000ed0();
  return 0;
}

s32 Func_02000d3c(void) {
  if (gSequenceParentWork.scene == 0xea && GameFlag_Test(0x9d0)) {
    Func_02000ff0(54, 47, 4, 15, 10, 4);
    Func_02000ff0(54, 38, 73, 7, 10, 4);
    Func_02000ff0(54, 43, 21, 55, 10, 3);
    Func_02000ff0(65, 43, 21, 20, 10, 3);
    Func_02000ff0(4, 47, 4, 50, 10, 3);
  }
  return 0;
}
