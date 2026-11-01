// Matches: differ 0 (tla-en, 2026-11-01)
// Overlay 6b3's closing sequence: it sets eventMode to 1 and restores it, runs
// the two-party dialogue through Func_02000244, and waits for a key twice, the
// second time for at most 10800 frames. The map buffer, gInput and
// gMainRoutineWork reload at each use because they are constant addresses
// here: as symbols CSE and cse2 merge them and they take callee-saved
// registers (a differ of 65). Ram_Input is new: on adoption it goes in
// system/ram_buffer.h beside the others (O2). The address loads for the first
// two key tests still merge as constants, so one tagged local (p) is reused
// between them; a while loop over the read hoists it instead (differ 66).
// Devices, both tagged in the function: temporary and one-pass. Inline setters
// for the two register stores left differ 2. Placeholders: the pooled ids 121
// to 128 (ResourceId_Ov6b3_N, whose kind is a guess from the buffer argument)
// and 1 (Id_Ov6b3_1) need defining as symbols on adoption, and tiles names only
// the heap slot it reads. The other languages are unmeasured.
#include "types.h"
#include "call.h"
#include "bg_scroll.h"
#include "system/main_routine_work.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
s32 FieldEvent_Wait();
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
s32 Func_0200086c();
s32 GameFlag_ClearBit();
s32 Actor_GetById();
s32 WindowSystem_InitField();
s32 wait();
extern u8 MsgFinallyWeVe[];
extern s32 gData_02000a28;
extern u8 gRenderOamEnabled;
extern char ResourceId_Ov6b3_121, ResourceId_Ov6b3_122, ResourceId_Ov6b3_123,
    ResourceId_Ov6b3_125, ResourceId_Ov6b3_126, ResourceId_Ov6b3_127,
    ResourceId_Ov6b3_128;
extern char Id_Ov6b3_1;

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
  actor = (u8 *)Value1((s32 (*)())Actor_GetById, work->currentOwner);
  zero = 0;
  actor[85] = zero;
  one = 1;
  *eventMode = one;
  Func_02000844();
  WindowSystem_InitField();
  tiles = Ram_HeapSlots->windowTiles;
  tiles[6] = one;
  tiles[0x138c] = one;
  gData_02000a28 = (s32)MsgFinallyWeVe;
  Func_02000098();
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_02000244, 5, 0, 40);
  Call3((void (*)())Func_02000244, 1, 1, 40);
  Call3((void (*)())Func_02000244, 6, 0, 40);
  Call3((void (*)())Func_02000244, 2, 1, 40);
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
  Call3((void (*)())Func_02000244, 1, 1, 30);
  Call1((void (*)())Func_0200086c, 67);
  scroll = gBgScroll;
  scroll[3].y = 16;
  scroll[2].y = 16;
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_Ov6b3_121, (s32)Ram_MapCellBuffer, 0);
  Call1((void (*)())FieldEvent_Wait, 100);
  Call3((void (*)())Func_02000244, 60, 2, 30);
  Call3((void (*)())Func_02000244, 61, 2, 40);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Call3((void (*)())Func_02000244, 5, 2, 40);
  Call1((void (*)())Func_02000854, 120);
  Call1((void (*)())wait, 1);
  Func_0200085c();
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_Ov6b3_122, (s32)Ram_MapCellBuffer, 120);
  Call1((void (*)())FieldEvent_Wait, 120);
  Call3((void (*)())Func_02000244, 5, 2, 30);
  Call3((void (*)())Func_02000244, 7, 2, 30);
  Call3((void (*)())Func_02000244, 56, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 30);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Call3((void (*)())Func_02000244, 6, 2, 30);
  Call3((void (*)())Func_02000244, 5, 2, 30);
  Call3((void (*)())Func_02000244, 3, 2, 40);
  Call3((void (*)())Func_02000244, 1, 2, 30);
  Call1((void (*)())Func_0200086c, 78);
  Call3((void (*)())Func_02000244, 0x8040, 2, 30);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 0x8041, 2, 30);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 0x8045, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 30);
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_Ov6b3_123, (s32)Ram_MapCellBuffer, 0);
  Call1((void (*)())FieldEvent_Wait, 60);
  Call1((void (*)())Func_0200086c, 73);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_02000244, 64, 2, 30);
  Call3((void (*)())Func_02000244, 65, 2, 30);
  Call3((void (*)())Func_02000244, 67, 2, 30);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 70, 2, 30);
  Call3((void (*)())Func_02000244, 68, 2, 30);
  Call3((void (*)())Func_02000244, 56, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Call3((void (*)())Func_02000244, 56, 2, 40);
  Call1((void (*)())Func_02000854, 200);
  Call1((void (*)())wait, 1);
  Func_0200085c();
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_02000244, 62, 2, 20);
  Call3((void (*)())Func_02000244, 0, 2, 40);
  Call3((void (*)())Func_02000244, 66, 2, 20);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 66, 2, 40);
  Call3((void (*)())Func_0200084c, 0, (s32)Ram_MapCellBuffer, 0);
  Call1((void (*)())FieldEvent_Wait, 60);
  scroll[3].y = zero;
  scroll[2].y = zero;
  Call1((void (*)())Func_02000864, 0);
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_Ov6b3_125, (s32)Ram_MapCellBuffer, 0);
  Call1((void (*)())FieldEvent_Wait, 60);
  goto L_02008692;
L_0200868c:
  Call1((void (*)())wait, 1);
L_02008692:
  /* FAKEMATCH(temporary): p reused for the input and main work addresses; as gInput and gMainRoutineWork read directly, 27 of 426 instructions differ in tla-en. */
  p = Ram_Input;
  if (((struct InputState *)p)->pressed == 0)
    goto L_0200868c;
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_Ov6b3_126, (s32)Ram_MapCellBuffer, 0);
  Call1((void (*)())FieldEvent_Wait, 120);
  Call1((void (*)())Func_02000854, 120);
  Call1((void (*)())wait, 1);
  Func_0200085c();
  Call1((void (*)())FieldEvent_Wait, 180);
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_Ov6b3_127, (s32)Ram_MapCellBuffer, 0);
  Call1((void (*)())FieldEvent_Wait, 300);
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_Ov6b3_128, (s32)Ram_MapCellBuffer, 0);
  Call1((void (*)())FieldEvent_Wait, 180);
  Call1((void (*)())Func_0200086c, 78);
  p = &gMainRoutineWork;
  ((struct MainRoutineWork *)p)->eventMode = saved;
  Call1((void (*)())FieldEvent_Wait, 300);
  count = 0;
  if (Ram_Input->pressed != 0)
    goto L_0200871e;
L_02008704:
  Call1((void (*)())wait, 1);
  count++;
  if (count > 10799)
    goto L_0200871e;
  if (Ram_Input->pressed == 0)
    goto L_02008704;
L_0200871e:
  Call3((void (*)())Func_0200084c, 0, (s32)Ram_MapCellBuffer, 0);
  Call1((void (*)())FieldEvent_Wait, 60);
  value = 0;
  /* FAKEMATCH(one-pass): the block round the store; as a plain store, 2 of 426 instructions differ in tla-en. */
  do {
    REG_BLDCNT = value;
  } while (0);
  value = 0x1040;
  REG_DISPCNT = value;
  Func_020007cc();
  Func_020007c4();
  gRenderOamEnabled = 1;
  Call1((void (*)())GameFlag_ClearBit, 0x17c);
  Call2((void (*)())Func_02000824, (s32)&Id_Ov6b3_1, 2);
  return 0;
}
