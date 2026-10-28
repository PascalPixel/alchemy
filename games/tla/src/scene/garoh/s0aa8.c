#include "garoh.h"
#include "adventure.h"
#include "event_work.h"
#include "call.h"

extern char SceneId_Ov65FRoom1;
extern char SceneId_Ov65FRoom2;
extern char SceneId_Ov65FRoom3;
extern char SceneId_Ov65FRoom4;
extern char SceneId_Ov65FRoom5;
extern char SceneId_Ov660Room3;

void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void Event_StartScreenTransition(void);
void Func_02000a68(void);
void Func_02000be4(void);
void Func_02000d30(void);
void Func_02000e00(void);
void Func_02005214(s32 id, s32 x, s32 z, s32 angle);
void Func_020052f4(s32 value);
void Func_0200538c(void);

s32 Func_02000aa8(void) {
  struct EventWork *work;

  if (gAdventure.scene == (s32)&SceneId_Ov65FRoom4) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  } else if (gAdventure.scene == (s32)&SceneId_Ov65FRoom5) {
    ObjectMotion_SetActionVariant(13, 1);
    ObjectMotion_SetActionVariant(14, 1);
    if (GameFlag_Test(0x91f)) {
      /* FAKEMATCH(temporary): the angle in its own local; as Value4(...,
       * 0x4000), 4 of 128 instructions differ in tla-en. */
      s32 angle = 0x4000;

      Value4((s32 (*)())Func_02005214, 10, 0x1480000, 0xa20000, angle);
    }
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
    Func_020052f4(0);
    if (gAdventure.entrance == 6) {
      work = Ram_HeapSlots->eventWork;
      work->screenEffect = 0x209;
      GameFlag_ClearBit(0x120);
      GameFlag_ClearBit(0x121);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
    }
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
    if (gAdventure.scene == (s32)&SceneId_Ov65FRoom1) {
      GameFlag_SetBit(0x919);
      Func_02000be4();
    } else if (gAdventure.scene == (s32)&SceneId_Ov65FRoom2) {
      Func_02000d30();
    } else if (gAdventure.scene == (s32)&SceneId_Ov65FRoom3) {
      Func_02000e00();
      if (GameFlag_Test(0x919)) {
        gAdventure.savedScene = (s32)&SceneId_Ov660Room3;
        gAdventure.savedEntrance = 2;
      }
    }
  }
  return 0;
}
