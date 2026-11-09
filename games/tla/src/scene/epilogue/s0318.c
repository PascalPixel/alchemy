#include "edition.h"

#include "types.h"
#include "call.h"
#include "bg_scroll.h"
#include "system/main_routine_work.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "event_inline.h"
s32 Func_02000098();
s32 Func_02000244();
s32 Func_020007c4();
s32 Func_020007cc();
s32 Func_02000824();
s32 Func_02000844();
s32 Func_0200084c();
s32 Func_02000854();
s32 Func_0200085c();
s32 Func_02000864();
s32 WindowSystem_InitField();
extern u8 MsgFinallyWeVe[];
extern s32 gData_02000a28;
extern u8 gRenderOamEnabled;
extern char ResourceId_EndingCliffDayPicture,
    ResourceId_EndingCliffSunsetPicture, ResourceId_EndingVillagersPictureA,
    ResourceId_EndingEmbracePicture, ResourceId_EndingReunionPicture,
    ResourceId_EndingSunsetPlainPicture, ResourceId_EndingTheEndPicture;
extern char SceneId_Ov64a;

#if LANGUAGE == LANGUAGE_JAPANESE
#define TILES_FLAG_OFFSET 0x118c
#else
#define TILES_FLAG_OFFSET 0x138c
#endif

#define Ram_Input FIXED_ADDRESS((struct InputState *), 0x03001150, gInput)

s32 Func_02000318(void) {
  struct MainRoutineWork *work;
  u8 *eventMode;
  s32 saved;
  s32 zero, one, value;
  s32 count;
  void *p;
  u8 *actor;
  u8 *tiles;
  struct BgScroll *scroll;

  work = &gMainRoutineWork;
  eventMode = &work->eventMode;
  saved = *eventMode;
  actor = (u8 *)Actor_Get(work->currentOwner);
  zero = 0;
  actor[85] = zero;
  one = 1;
  *eventMode = one;
  Func_02000844();
  WindowSystem_InitField();
  tiles = Ram_HeapSlots->windowTiles;
  tiles[6] = one;
  tiles[TILES_FLAG_OFFSET] = one;
  gData_02000a28 = (s32)MsgFinallyWeVe;
  Func_02000098();
  FieldEvent_Wait(30);
  Call3((void (*)())Func_02000244, 5, 0, 40);
  Call3((void (*)())Func_02000244, 1, 1, 40);
  Call3((void (*)())Func_02000244, 6, 0, 40);
  Call3((void (*)())Func_02000244, 2, 1, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Func_02000244, 2, 1, 40);
#endif
  Call3((void (*)())Func_02000244, 3, 0, 30);
  Call3((void (*)())Func_02000244, 7, 1, 30);
  Call3((void (*)())Func_02000244, 56, 0, 40);
  Call3((void (*)())Func_02000244, 60, 1, 30);
  Call3((void (*)())Func_02000244, 61, 1, 40);
  Call3((void (*)())Func_02000244, 0, 0, 40);
  Call3((void (*)())Func_02000244, 62, 1, 30);
  Call3((void (*)())Func_02000244, 0, 0, 30);
  Call3((void (*)())Func_02000244, 0, 0, 30);
  Call3((void (*)())Func_02000244, 5, 1, 40);
  Call3((void (*)())Func_02000244, 4, 0, 30);
  Func_02000244(1, 1, 30);
  Event_PlaySound(67);
  scroll = gBgScroll;
  scroll[3].y = 16;
  scroll[2].y = 16;
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_EndingCliffDayPicture,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(100);
  Call3((void (*)())Func_02000244, 60, 2, 30);
  Call3((void (*)())Func_02000244, 61, 2, 40);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Func_02000244, 5, 2, 40);
#endif
  Func_02000244(5, 2, 40);
  Func_02000854(120);
  System_Wait(1);
  Func_0200085c();
  FieldEvent_Wait(30);
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_EndingCliffSunsetPicture,
        (s32)Ram_MapCellBuffer, 120);
  FieldEvent_Wait(120);
  Call3((void (*)())Func_02000244, 5, 2, 30);
  Call3((void (*)())Func_02000244, 7, 2, 30);
  Call3((void (*)())Func_02000244, 56, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Func_02000244, 4, 2, 30);
#endif
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Call3((void (*)())Func_02000244, 6, 2, 30);
  Call3((void (*)())Func_02000244, 5, 2, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Func_02000244, 5, 2, 30);
#endif
  Call3((void (*)())Func_02000244, 3, 2, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Func_02000244, 1, 2, 30);
#endif
  Func_02000244(1, 2, 30);
  Event_PlaySound(78);
  Call3((void (*)())Func_02000244, 0x8040, 2, 30);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 0x8041, 2, 30);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 0x8045, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 30);
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_EndingVillagersPictureA,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  Event_PlaySound(73);
  FieldEvent_Wait(30);
  Call3((void (*)())Func_02000244, 64, 2, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Func_02000244, 64, 2, 30);
#endif
  Call3((void (*)())Func_02000244, 65, 2, 30);
  Call3((void (*)())Func_02000244, 67, 2, 30);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 70, 2, 30);
  Call3((void (*)())Func_02000244, 68, 2, 30);
  Call3((void (*)())Func_02000244, 56, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Func_02000244(56, 2, 40);
  Func_02000854(200);
  System_Wait(1);
  Func_0200085c();
  FieldEvent_Wait(30);
  Call3((void (*)())Func_02000244, 62, 2, 20);
  Call3((void (*)())Func_02000244, 0, 2, 40);
  Call3((void (*)())Func_02000244, 66, 2, 20);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 66, 2, 40);
  Call3((void (*)())Func_0200084c, 0, (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  scroll[3].y = zero;
  scroll[2].y = zero;
  Func_02000864(0);
  Func_0200084c((s32)&ResourceId_EndingEmbracePicture, (s32)Ram_MapCellBuffer,
                0);
  FieldEvent_Wait(60);
  goto L_02008692;
L_0200868c:
  System_Wait(1);
L_02008692:
  /* FAKEMATCH(temporary): p reused for the input and main work addresses; as
   * gInput and gMainRoutineWork read directly, 8 of 426 instructions differ in
   * tla-en. */
  p = Ram_Input;
  if (((struct InputState *)p)->pressed == 0)
    goto L_0200868c;
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_EndingReunionPicture,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(120);
  Func_02000854(120);
  System_Wait(1);
  Func_0200085c();
  FieldEvent_Wait(180);
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_EndingSunsetPlainPicture,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(300);
  Func_0200084c((s32)&ResourceId_EndingTheEndPicture, (s32)Ram_MapCellBuffer,
                0);
  FieldEvent_Wait(180);
  Event_PlaySound(78);
  p = &gMainRoutineWork;
  ((struct MainRoutineWork *)p)->eventMode = saved;
  FieldEvent_Wait(300);
  count = 0;
  if (Ram_Input->pressed != 0)
    goto L_0200871e;
L_02008704:
  System_Wait(1);
  count++;
  if (count > 10799)
    goto L_0200871e;
  if (Ram_Input->pressed == 0)
    goto L_02008704;
L_0200871e:
  Func_0200084c(0, (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  value = 0;
  /* FAKEMATCH(one-pass): the block round the store; as a plain store, 2 of 426
   * instructions differ in tla-en. */
  do {
    REG_BLDCNT = value;
  } while (0);
  value = 0x1040;
  REG_DISPCNT = value;
  Func_020007cc();
  Func_020007c4();
  gRenderOamEnabled = 1;
  Event_ClearFlag(0x17c);
  Call2((void (*)())Func_02000824, (s32)&SceneId_Ov64a, 2);
  return 0;
}
