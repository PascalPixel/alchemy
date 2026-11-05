// Draft, differ 14 of 255 (tla-en, 2026-11-05). Func_020002f4 of overlay 6ab,
// rewritten readable from the translator's map, nested for its one exit.
// Left: the leader's position block: the follower's offset through r3 and the
// leader through r1 natively, r1 and r3 here, with one untagged once-only
// block that holds x first.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

struct Overlay6abEventWork {
  u8 unk000[0x1ac];
  s32 unk1ac;
  u8 unk1b0[0x1c];
  struct Actor *unk1cc;
};

s32 Func_02000714();
s32 Func_02000ae4();
s32 Func_02000d88();
s32 Func_02000db8();
s32 Func_02001d7c();
s32 Func_02001df0();
s32 Func_02003e04();
s32 Func_020041fc();
s32 Func_0200423c();
s32 Func_02004244();
s32 Func_0200432c();
s32 Func_0200443c();
s32 GameFlag_GetByte();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 wait();
struct Actor *Actor_GetById(s32 id);
extern s32 Data_02004cec;
extern char SceneId_Ov6abRoom1;

s32 Func_020002f4(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;

  ((struct Overlay6abEventWork *)Ram_HeapSlots->eventWork)->unk1ac = 0x204;
  Data_02004cec = 0;
  Call1((void (*)())GameFlag_SetBit, 0x112);
  Func_02001d7c();
  Call4((void (*)())Func_02001df0, 0, 0, 0, 88);
  if (scene == (s32)&SceneId_Ov6abRoom1) {
  Call1((void (*)())Func_0200443c, 0);
  if (Value1((s32 (*)())GameFlag_Test, 0xa30)) {
    Call6((void (*)())Func_0200423c, 65, 65, 9, 21, 38, 24);
    Call6((void (*)())Func_02004244, 65, 65, 9, 21, 38, 24);
    Call6((void (*)())Func_02004244, 76, 64, 9, 22, 38, 87);
    Call6((void (*)())Func_02004244, 76, 64, 9, 22, 102, 23);
  }
  if (entrance == 11) {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
    struct Overlay6abEventWork *work = Ram_HeapSlots->eventWork;
    s32 column = GameFlag_GetByte(0x340);
    s32 row = GameFlag_GetByte(0x348);
    struct Actor **follower;
    struct Actor *other;
    s32 x, z, base;

    x = (column << 20) + 0x80000;
    do {} while (0);
    base = -0x1280000;
    follower = &work->unk1cc;
    z = (row << 20) + base;
    other = *follower;
    leader->z = z;
    leader->x = x;
    other->x = x;
    other->z = leader->z;
    Func_020041fc();
    if (!Value1((s32 (*)())GameFlag_Test, 0x109))
      Func_02003e04();
  }
  if ((u32)(entrance - 9) <= 1) {
    if (!Value1((s32 (*)())GameFlag_Test, 0x109))
      Func_02003e04();
  }
  if (entrance == 1 || entrance == 20) {
  if (!Value1((s32 (*)())GameFlag_Test, 0xa30)) {
    Call2((void (*)())Actor_SetSpritePriority, 11, 3);
    Call2((void (*)())Actor_SetSpritePriority, 12, 3);
    Actor_GetById(12)->scaleX = 0xffff0000;
  }
  if (Value1((s32 (*)())GameFlag_Test, 0xa4b)) {
    Func_02000d88();
    Call3((void (*)())Func_0200432c, 10, 0, 0);
  } else if (Value1((s32 (*)())GameFlag_Test, 0xa4a)) {
    Call1((void (*)())GameFlag_SetBit, 0x233);
  } else {
    Call3((void (*)())Func_0200432c, 10, 0, 0);
  }
  if (Value1((s32 (*)())GameFlag_Test, 0xa3b)) {
    if (!Value1((s32 (*)())GameFlag_Test, 0xa30)) {
      Call3((void (*)())Func_0200432c, 11, 0, 0);
      Call3((void (*)())Func_0200432c, 12, 0, 0);
    }
    Func_02000d88();
    Call1((void (*)())wait, 10);
    if (!Value1((s32 (*)())GameFlag_Test, 0xa3a)) {
      Call1((void (*)())Func_02000ae4, 0);
      if (!Value1((s32 (*)())GameFlag_Test, 0x109)) {
        Call1((void (*)())Func_02000db8, 13);
        Call1((void (*)())Func_02000db8, 14);
      }
    }
    if (Actor_GetById(13)->x > 0x1900000) {
      Call6((void (*)())Func_0200423c, 39, 22, 2, 1, 43, 13);
      Call6((void (*)())Func_0200423c, 31, 21, 2, 2, 40, 12);
    }
  } else if (entrance == 20) {
    Func_02000714();
  }
  }
  }
  return 0;
}
