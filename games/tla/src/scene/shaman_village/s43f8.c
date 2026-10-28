#include "shaman_village.h"
#include "input.h"
#include "io_reg.h"

void Func_020048ac(s32 mode);
void Func_02002878(void);
void Func_020043c8(void);
void Func_02004394(void);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);

struct IoReg16 {
  u16 value;
};

void Func_020043f8(void) {
  if (!GameFlag_Test(0x109)) {
    if (GameFlag_Test(0x9fe)) {
      ShamanVillage_ObjPlace(16, 0x680000, 0xc80000);
      ShamanVillage_ObjPlace(17, 0x880000, 0xc80000);
    } else if (GameFlag_Test(0x94f) && !GameFlag_Test(0x9e1)) {
      ShamanVillage_ObjPlace(15, 0x780000, 0xd00000);
      ShamanVillage_ObjPlace(16, 0x680000, 0xd40000);
      ShamanVillage_ObjPlace(17, 0x880000, 0xd40000);
      WaitFrames(1);
    }
  }
  if (GameFlag_Test(0x202))
    ShamanVillage_MapCopy(43, 40, 7, 6, 1, 2);
  if (GameFlag_Test(0x94d)) {
    s32 width = 1;
    s32 height = 2;

    ShamanVillage_MapCopy(48, 40, 77, 21, width, height);
    ShamanVillage_MapCopy(48, 40, 81, 25, width, height);
  }
  Func_02004a44();
  Func_02004a4c(0, 0x204, 19, 20);
  if (GameFlag_Test(0x205))
    Func_02002878();
  {
    /* FAKEMATCH: the value pinned to r3 loads from the pool before the register
     * address starts, as the native does; as a plain store, one row is
     * reordered in tla-en. */
    register u16 alpha __asm__("r3") = 0xc08;

    /* FAKEMATCH: the barrier of the pin above: it holds r2 until the value is
     * loaded. */
    __asm__("" : "+l"(alpha) : : "r2");
    REG_BLDALPHA = alpha;
  }
  REG_BLDCNT = 0x3f10;
  {
    s32 priority = 0x480;

    /* FAKEMATCH: the barrier holds r0 until the priority is complete, so the
     * callback is loaded last as the native has; through the inline alone it
     * comes one row early in tla-en. */
    __asm__("" : "+l"(priority) : : "r0");
    Scheduler_AddOrUpdateCallback(Func_020043c8, priority);
  }
  {
    s32 priority = 0x480;

    Scheduler_AddOrUpdateCallback(Func_02004394, priority);
  }
}

void Func_02004534(void) {
  u32 i;

  ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      521;
  Object_GetById(12)->collisionFlags = 8;
  if (GameFlag_Test(0x9fe)) {
    /* FAKEMATCH: the local starts the facing before the other arguments, as the
     * native has; as a plain argument it is started third (one reordered row).
     */
    s32 angle = 0x3000;

    ShamanVillage_ObjPut(8, 0x580000, 0x5c0000, angle);
    ShamanVillage_ObjPlace(11, 0x9c0000, 0x480000);
  } else {
    Func_02004934(17, 0, 0);
    Func_02004934(18, 0, 0);
    Func_02004934(19, 0, 0);
  }
  if ((gAdventure.entrance == 30 || gAdventure.entrance == 31) &&
      !GameFlag_Test(0x109)) {
    Func_020048ac(1);
    Func_020048c4();
    Func_02004a24(0);
    Object_SetModeById(gAdventure.currentOwner, 41);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gAdventure.currentOwner),
                                       0);
    WaitFrames(1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    FieldEvent_WaitFrames(20);
    i = 0;
    goto test_stand;
  next_stand:
    i++;
  test_stand:
    if (i < 60) {
      WaitFrames(1);
      if (gInput.held == 0)
        goto next_stand;
    }
    Object_SetModeById(gAdventure.currentOwner, 42);
    FieldEvent_WaitFrames(10);
    i = 0;
    goto test_turn;
  next_turn:
    i++;
  test_turn:
    if (i < 30) {
      WaitFrames(1);
      if (gInput.held == 0)
        goto next_turn;
    }
    ObjectMotion_Launch(gAdventure.currentOwner, 4, 0);
    Object_SetModeById(gAdventure.currentOwner, 1);
    ObjectMotion_SetSpeedParameters(gAdventure.currentOwner, 0x19999, 0xcccc);
    ObjectMotion_SetPositionAndCommit(gAdventure.currentOwner, 813, 120);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gAdventure.currentOwner),
                                       1);
    Func_020048cc();
    if (gAdventure.entrance == 30) {
      GameFlag_SetBit(0x9fe);
      GameFlag_SetBit(0x94d);
    }
  }
}
