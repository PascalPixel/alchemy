#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "call.h"
#include "event_inline.h"
extern char SceneId_Ov6ACRoom6;
extern char SceneId_Ov6ACRoom1;

void Sound_WaitForBgmRestore(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001324(s32 x, s32 z, s32 c, s32 d, s32 e, s32 f);
void Func_0200154c(void *a, void *b, void *c, void *d);
void Func_02002e6c(void);
void Func_020031e0(void);
void Func_020031e8(void);
void Func_02003228(void);
void Func_02003278(s32 a);
void Func_02003290(void);

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
  Event_SetFlag(0x112);
  Call4((void (*)())Func_0200154c, (s32)gData_0200338c, (s32)gData_0200339c,
        (s32)gData_020033c8, (s32)gData_020033f4);
  if (scene != (s32)&SceneId_Ov6ACRoom6 && scene == (s32)&SceneId_Ov6ACRoom1) {
    Call1((void (*)())Func_02003278, 0);
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x204;
    if ((u32)(entrance - 1) <= 1 || entrance == 8 || entrance == 10 ||
        entrance == 12 || entrance == 14) {
      s32 unset = Event_GetFlag(0xa31);

      if (unset == 0 && Event_GetFlag(0xa32) &&
          Event_GetFlag(0xa33) &&
          Event_GetFlag(0xa34) &&
          Event_GetFlag(0xa35)) {
        struct Actor *actor;

        Func_020031e0();
        FieldEvent_ResolvePendingActions(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Event_PlaySound(78);
        FieldCamera_SetSpeed(0x20000, 0x4000);
        FieldCamera_MoveTo(0xe00000, 0x400000, 0xd80000, 1);
        Func_02003228();
        FieldEvent_Wait(20);
        PaletteFade_ApplySource(0x10000, 0);
        PaletteFade_ApplyTarget(0x10003, 0);
        PaletteFade_Interpolate(20);
        System_Wait(20);
        /* FAKEMATCH(temporary): entrance reused for the palette address, which
         * holds its register; as a plain store, 38 of 324 instructions differ
         * in tla-en. */
        entrance = 0x500021e;
        *(u16 *)entrance = 0x7fff;
        System_Wait(1);
        FieldMessage_ShowCentered((s32)MsgYouHaveProven, 0, 4);
        PaletteFade_ApplyTarget(0x10000, 0);
        PaletteFade_Interpolate(20);
        System_Wait(20);
        FieldCamera_SetSpeed(0x10000, 0x2000);
        FieldCamera_MoveTo(0xe00000, 0x100000, 0xd80000, 1);
        Func_02003228();
        FieldEvent_Wait(40);
        actor = Actor_Get(16);
        actor->motionFlags = unset;
        actor->y = 0x100000;
        actor->terrainHeight = 0x100000;
        ((u8 *)actor)[98] = 18;
        actor->unk66 = 0x4000;
        actor->watchedObject = (struct Actor *)unset;
        Func_02002e6c();
        System_Wait(40);
        MapGrid_SetShake(0x20000, 0x20000, 0x10000);
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
        MapGrid_SetShake(-1, -1, 0xe666);
        MapGrid_CopyCellAttributes(30, 12, 4, 3, 12, 12);
        FieldEvent_Wait(40);
        Event_SetFlag(0xa31);
        Event_PlaySound(80);
        Sound_WaitForBgmRestore();
        Func_02003290();
        Func_020031e8();
      }
    }
  }
  return 0;
}
