// Matches: differ 0 (tla-en, 2026-11-08)
// Func_020002f4 of overlay 6ab, rewritten readable from the translator's map,
// nested for its one exit; differ 0 in all six editions (2026-11-08). The
// leader's position block settled in plain C: the leader's x is stored before
// z is computed. Reload hands out r1, r2 and r3 in turn, so the leader's
// reload has to come between x's 0x80000 and z's constant for the other
// actor's 460 to land in r3 and the leader in r1, as in the listing; the
// earlier drafts stored both of the leader's coordinates after z and swapped
// them. Two tagged devices: GameFlag_GetByte declared returning s32 (as u8,
// three added zero-extensions), and the Call6 wrapper; every other call is
// direct, with its prototype.
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

void Func_02000714(void);
void Func_02000ae4(s32 a);
void Func_02000d88(void);
void Func_02000db8(s32 actor);
void Func_02001d7c(void);
void Func_02001df0(s32 a, s32 b, s32 c, s32 d);
void Func_02003e04(void);
void Func_020041fc(void);
void Func_0200423c();
void Func_02004244();
void Func_0200432c(s32 actor, s32 a, s32 b);
void Func_0200443c(s32 a);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
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
      /* FAKEMATCH(wrapper): Func_0200423c and Func_02004244 called through
       * Call6, here and below; as direct calls, 12 of 255 instructions differ
       * in tla-en. */
      Call6((void (*)())Func_0200423c, 65, 65, 9, 21, 38, 24);
      Call6((void (*)())Func_02004244, 65, 65, 9, 21, 38, 24);
      Call6((void (*)())Func_02004244, 76, 64, 9, 22, 38, 87);
      Call6((void (*)())Func_02004244, 76, 64, 9, 22, 102, 23);
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
        Actor_SetSpritePriority(11, 3);
        Actor_SetSpritePriority(12, 3);
        Actor_GetById(12)->scaleX = 0xffff0000;
      }
      if (GameFlag_Test(0xa4b)) {
        Func_02000d88();
        Func_0200432c(10, 0, 0);
      } else if (GameFlag_Test(0xa4a)) {
        GameFlag_SetBit(0x233);
      } else {
        Func_0200432c(10, 0, 0);
      }
      if (GameFlag_Test(0xa3b)) {
        if (!GameFlag_Test(0xa30)) {
          Func_0200432c(11, 0, 0);
          Func_0200432c(12, 0, 0);
        }
        Func_02000d88();
        wait(10);
        if (!GameFlag_Test(0xa3a)) {
          Func_02000ae4(0);
          if (!GameFlag_Test(0x109)) {
            Func_02000db8(13);
            Func_02000db8(14);
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
