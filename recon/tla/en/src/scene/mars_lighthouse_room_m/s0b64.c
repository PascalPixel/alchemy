#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "call.h"
extern char SceneId_Ov6ACRoom6;
extern char SceneId_Ov6ACRoom1;

void Sound_WaitForBgmRestore(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_Wait(s32 frames);
void Func_02001324(s32 x, s32 z, s32 c, s32 d, s32 e, s32 f);
void Func_0200154c(void *a, void *b, void *c, void *d);
void Func_02002e6c(void);
void Func_02003170(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003198(s32 a, s32 b, s32 c);
void Func_020031c0(const void *message, s32 a, s32 b);
void Func_020031e0(void);
void Func_020031e8(void);
void Func_02003218(s32 a, s32 b);
void Func_02003228(void);
void Func_02003248(s32 a, s32 b);
void Func_02003250(s32 a, s32 b);
void Func_02003258(s32 frames);
void Func_02003278(s32 a);
void Func_02003280(s32 a);
void Func_02003290(void);
void Func_020032b8(s32 a);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);

extern u8 MsgYouHaveProven[];
extern u8 gData_0200338c[];
extern u8 gData_0200339c[];
extern u8 gData_020033c8[];
extern u8 gData_020033f4[];

s32 Func_02000b64(void) {
  s32 entrance;
  s32 scene;

  scene = gMainRoutineWork.scene;
  entrance = gMainRoutineWork.entrance;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x100;
  Call1((void (*)())GameFlag_SetBit, 0x112);
  Call4((void (*)())Func_0200154c, (s32)gData_0200338c, (s32)gData_0200339c,
        (s32)gData_020033c8, (s32)gData_020033f4);
  if (scene != (s32)&SceneId_Ov6ACRoom6 && scene == (s32)&SceneId_Ov6ACRoom1) {
    Call1((void (*)())Func_02003278, 0);
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x204;
    if ((u32)(entrance - 1) <= 1 || entrance == 8 || entrance == 10 ||
        entrance == 12 || entrance == 14) {
      s32 unset = Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa00, 49));

      if (unset == 0 && Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa00, 50)) &&
          Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa00, 51)) &&
          Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa00, 52)) &&
          Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa00, 53))) {
        struct Actor *actor;

        Func_020031e0();
        Call1((void (*)())Func_02003280, 0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Call1((void (*)())Func_020032b8, 78);
        Call2((void (*)())Func_02003218, 0x20000, 0x4000);
        Call4((void (*)())Camera_MoveWithinMapBounds, 0xe00000, 0x400000,
              0xd80000, 1);
        Func_02003228();
        Call1((void (*)())FieldEvent_Wait, 20);
        Call2((void (*)())Func_02003250, 0x10000, 0);
        Call2((void (*)())Func_02003248, SPLIT_CONSTANT(0x10000, 3), 0);
        Call1((void (*)())Func_02003258, 20);
        Call1((void (*)())wait, 20);
        /* FAKEMATCH(temporary): entrance reused for the palette address, which
         * holds its register; as a plain store, 38 of 324 instructions differ
         * in tla-en. */
        entrance = 0x500021e;
        *(u16 *)entrance = 0x7fff;
        Call1((void (*)())wait, 1);
        Call3((void (*)())Func_020031c0, (s32)MsgYouHaveProven, 0, 4);
        Call2((void (*)())Func_02003248, 0x10000, 0);
        Call1((void (*)())Func_02003258, 20);
        Call1((void (*)())wait, 20);
        Call2((void (*)())Func_02003218, 0x10000, 0x2000);
        Call4((void (*)())Camera_MoveWithinMapBounds, 0xe00000, 0x100000,
              0xd80000, 1);
        Func_02003228();
        Call1((void (*)())FieldEvent_Wait, 40);
        actor = Actor_GetById(16);
        actor->motionFlags = unset;
        actor->y = 0x100000;
        actor->terrainHeight = 0x100000;
        ((u8 *)actor)[98] = 18;
        actor->unk66 = 0x4000;
        actor->watchedObject = (struct Actor *)unset;
        Func_02002e6c();
        Call1((void (*)())wait, 40);
        Call3((void (*)())Func_02003198, 0x20000, 0x20000, 0x10000);
        Func_02001324(216, 216, 65, 65, 13, 13);
        Func_02001324(232, 216, 66, 65, 14, 13);
        Func_02001324(232, 232, 66, 66, 14, 14);
        Func_02001324(216, 200, 65, 64, 13, 12);
        Func_02001324(248, 200, 67, 64, 15, 12);
        Func_02001324(248, 232, 67, 66, 15, 14);
        Func_02001324(216, 232, 65, 66, 13, 14);
        Func_02001324(200, 216, 64, 65, 12, 13);
        Func_02001324(232, 200, 66, 64, 14, 12);
        Func_02001324(248, 216, 67, 65, 15, 13);
        Func_02001324(200, 232, 64, 66, 12, 14);
        Func_02001324(200, 200, 64, 64, 12, 12);
        Call3((void (*)())Func_02003198, -1, -1, SPLIT_CONSTANT(0xe600, 102));
        Func_02003170(30, 12, 4, 3, 12, 12);
        FieldEvent_Wait(40);
        Call1((void (*)())GameFlag_SetBit, SPLIT_CONSTANT(0xa00, 49));
        Func_020032b8(80);
        Sound_WaitForBgmRestore();
        Func_02003290();
        Func_020031e8();
      }
    }
  }
  return 0;
}
