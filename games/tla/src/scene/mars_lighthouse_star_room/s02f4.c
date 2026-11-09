#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_inline.h"

struct Overlay6abEventWork {
  u8 unk000[0x1ac];
  s32 unk1ac;
  u8 unk1b0[0x1c];
  struct Actor *unk1cc;
};

void Func_02000714(void);
void Func_02000ae4(s32 a);
void Func_02000d88(void);
void Func_02000db8(s32 actor);
void Func_02001d7c(void);
void Func_02001df0(s32 a, s32 b, s32 c, s32 d);
void Func_02003e04(void);
void Func_020041fc(void);
void Func_0200443c(s32 a);
extern struct Actor *Data_02004cec;
extern char SceneId_Ov6abRoom1;

s32 Func_020002f4(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;

  ((struct Overlay6abEventWork *)Ram_HeapSlots->eventWork)->unk1ac = 0x204;
  Data_02004cec = NULL;
  GameFlag_SetBit(0x112);
  Func_02001d7c();
  Func_02001df0(0, 0, 0, 88);
  if (scene == (s32)&SceneId_Ov6abRoom1) {
    Func_0200443c(0);
    if (GameFlag_Test(0xa30)) {
      /* FAKEMATCH(wrapper): Map_CopyCellAttributeRect and Func_0802b450 called through
       * Call6, here and below; as direct calls, 12 of 255 instructions differ
       * in tla-en. */
      MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
      MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
      MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
      MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
    }
    if (entrance == 11) {
      struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
      struct Overlay6abEventWork *work = Ram_HeapSlots->eventWork;
      /* FAKEMATCH(signature): GameFlag_GetByte declared returning s32, where
       * its definition returns u8; as u8, 17 of 255 instructions differ in
       * tla-en. */
      s32 column = GameFlag_GetByte(0x340);
      s32 row = GameFlag_GetByte(0x348);
      struct Actor *other;
      s32 x;

      x = (column << 20) + 0x80000;
      leader->x = x;
      leader->z = (row << 20) - 0x1280000;
      other = work->unk1cc;
      other->x = x;
      other->z = leader->z;
      Func_020041fc();
      if (!GameFlag_Test(0x109))
        Func_02003e04();
    }
    if (entrance == 9 || entrance == 10) {
      if (!GameFlag_Test(0x109))
        Func_02003e04();
    }
    if (entrance == 1 || entrance == 20) {
      if (!GameFlag_Test(0xa30)) {
        FieldActor_SetSpritePriority(11, 3);
        FieldActor_SetSpritePriority(12, 3);
        Actor_GetById(12)->scaleX = 0xffff0000;
      }
      if (GameFlag_Test(0xa4b)) {
        Func_02000d88();
        Actor_SetPosition(10, 0, 0);
      } else if (GameFlag_Test(0xa4a)) {
        GameFlag_SetBit(0x233);
      } else {
        Actor_SetPosition(10, 0, 0);
      }
      if (GameFlag_Test(0xa3b)) {
        if (!GameFlag_Test(0xa30)) {
          Actor_SetPosition(11, 0, 0);
          Actor_SetPosition(12, 0, 0);
        }
        Func_02000d88();
        System_Wait(10);
        if (!GameFlag_Test(0xa3a)) {
          Func_02000ae4(0);
          if (!GameFlag_Test(0x109)) {
            Func_02000db8(13);
            Func_02000db8(14);
          }
        }
        if (Actor_GetById(13)->x > 0x1900000) {
          MapGrid_CopyCellAttributes(39, 22, 2, 1, 43, 13);
          MapGrid_CopyCellAttributes(31, 21, 2, 2, 40, 12);
        }
      } else if (entrance == 20) {
        Func_02000714();
      }
    }
  }
  return 0;
}
