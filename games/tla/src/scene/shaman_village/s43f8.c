#include "shaman_village.h"
#include "input.h"
#include "io_reg.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_020048ac(s32 mode);
void Func_02002878(void);
void Func_020043c8(void);
void Func_02004394(void);

struct IoReg16 {
  u16 value;
};

void Func_020043f8(void) {
  if (!GameFlag_Test(0x109)) {
    if (GameFlag_Test(0x9fe)) {
      Actor_SetPosition(16, PIXELS(104), PIXELS(200));
      Actor_SetPosition(17, PIXELS(136), PIXELS(200));
    } else if (GameFlag_Test(0x94f) && !GameFlag_Test(0x9e1)) {
      Actor_SetPosition(15, PIXELS(120), PIXELS(208));
      Actor_SetPosition(16, PIXELS(104), PIXELS(212));
      Actor_SetHorizontalPositionWithTerrain(17, PIXELS(136), PIXELS(212));
      wait(1);
    }
  }
  if (GameFlag_Test(0x202))
    MapGrid_CopyCellsTo(43, 40, 7, 6, 1, 2);
  if (GameFlag_Test(0x94d)) {
    s32 width = 1;
    s32 height = 2;

    Map_CopyCellPictureIndicesRect(48, 40, 77, 21, width, height);
    Map_CopyCellPictureIndicesRect(48, 40, 81, 25, width, height);
  }
  Func_02004a44();
  Func_02004a4c(0, 0x204, 19, 20);
  if (GameFlag_Test(0x205))
    Func_02002878();
  {
    /* FAKEMATCH(pin): the value pinned to r3 loads from the pool before the
     * register address starts, as the native does; as REG_BLDALPHA = 0xc08,
     * one row is reordered in tla-en. */
    register u16 alpha __asm__("r3") = 0xc08;

    REG_BLDALPHA = alpha;
  }
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  {
    s32 priority = 0x480;

    /* FAKEMATCH(barrier): the empty asm holds r0 until the priority is
     * complete, so the callback is loaded last as the native has; as
     * Task_Add(Func_020043c8, 0x480), it comes one row early in tla-en. */
    __asm__("" : "+l"(priority) : : "r0");
    Task_Add(Func_020043c8, priority);
  }
  {
    s32 priority = 0x480;

    Task_Add(Func_02004394, priority);
  }
}

void Func_02004534(void) {
  u32 i;

  ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      521;
  ((struct ShamanVillageObject *)Actor_GetById(12))->collisionFlags = 8;
  if (GameFlag_Test(0x9fe)) {
    /* FAKEMATCH(temporary): the local holding one constant starts the facing
     * before the other arguments, as the native has; as the plain argument
     * 0x3000 it is started third (one reordered row). */
    s32 angle = 0x3000;

    Actor_SetPositionAndFacing(8, PIXELS(88), PIXELS(92), angle);
    Actor_SetPosition(11, PIXELS(156), PIXELS(72));
  } else {
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
  }
  if ((gMainRoutineWork.entrance == 30 || gMainRoutineWork.entrance == 31) &&
      !GameFlag_Test(0x109)) {
    Func_020048ac(1);
    Func_020048c4();
    Event_ResolvePendingActions(0);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 41);
    Actor_SetSingleChildFlags(((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner)),
                                       0);
    wait(1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    i = 0;
    goto test_stand;
  next_stand:
    i++;
  test_stand:
    if (i < 60) {
      wait(1);
      if (gInput.held == 0)
        goto next_stand;
    }
    Actor_SetModeById(gMainRoutineWork.currentOwner, 42);
    Event_Wait(10);
    i = 0;
    goto test_turn;
  next_turn:
    i++;
  test_turn:
    if (i < 30) {
      wait(1);
      if (gInput.held == 0)
        goto next_turn;
    }
    Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 813, 120);
    Actor_SetSingleChildFlags(((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner)),
                                       1);
    Func_020048cc();
    if (gMainRoutineWork.entrance == 30) {
      GameFlag_SetBit(0x9fe);
      GameFlag_SetBit(0x94d);
    }
  }
}
