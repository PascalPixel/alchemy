#include "types.h"
#include "call.h"
#include "system/main_routine_work.h"
#include "ram_buffer.h"
s32 Animation_SetStateFlags();
s32 Camera_MoveWithinMapBounds();
s32 Event_PrepareObjectAndApplyValue();
s32 FieldEvent_Wait();
s32 Func_02000610();
s32 Func_02000640();
s32 Func_020035ec();
s32 Func_0200361c();
s32 Func_02003648();
s32 Func_02005574();
s32 Func_0200557c();
s32 Func_02005594();
s32 Func_0200559c();
s32 Func_020055a4();
s32 Func_020055ac();
s32 Func_020055c4();
void Func_020055cc();
s32 Func_020055ec();
s32 Func_0200567c();
s32 Func_02005684();
s32 Func_020056dc();
s32 Func_020056ec();
s32 Func_020056f4();
s32 Func_02005704();
void Func_02005714();
s32 Func_02005724();
void Func_02005734();
s32 Func_0200573c();
s32 Func_02005744();
s32 Func_0200575c();
s32 Func_0200576c();
s32 Func_0200578c();
void Func_02005794();
s32 Func_020057dc();
s32 GameFlag_SetBit();
s32 Motion_SetModeAndWaitAnimation();
s32 Motion_SetVarCbAndRefresh();
s32 Actor_SetSingleChildFlags();
s32 Actor_ArmCallback();
s32 Actor_SetMovementTypeById();
s32 Actor_SetSpritePriority();
s32 Actor_SetPositionAndReset();
s32 Actor_SetSpeedParameters();
s32 Actor_GetById();
s32 Actor_SetMovementTypeAndWaitById();
s32 Actor_SetModeById();
s32 Actor_SetPartAttribute();
s32 Actor_SetPositionAndResetMotion();
s32 PartyInventory_Remove();
s32 HeapSlot_AllocFar();
s32 HeapSlot_Release();
s32 Task_Add();
s32 Task_RemoveFar();
s32 FieldMessage_Show();
s32 VramCache_Load();
s32 wait();
extern u8 Data_02006b68[];
extern u8 Data_02006ccc[];
extern u8 Func_0200326c[];
extern u8 Func_02003370[];
extern u8 MsgTheBeaconIs[];
extern u8 gData_020060b4[];
extern u8 gData_020060fc[];
extern u8 gData_02006120[];
extern u8 gData_02006150[];

#define OWNER gMainRoutineWork.currentOwner

/// A halfword whose store may alias any read after it.
union StoredHalfword {
  u16 value;
  s32 word;
};

struct OverlayActor {
  u8 unk0[8];
  s32 x, y, z, terrainHeight, scaleX, scaleY;
  u8 unk20[3];
  u8 priorityFlags;
  u8 unk24[12];
  s32 speedLimit, acceleration;
  u8 unk38[16];
  s32 gravity;
  s32 unk4c;
  s32 animation;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[6];
  u8 unk5c;
  u8 unk5d[15];
  s32 update;
};
#define A(p) ((struct OverlayActor *)(p))

struct OverlaySprite {
  u8 unk0[5];
  u8 unk5_0 : 5;
  u8 unk5_5 : 1;
  u8 unk5_6 : 2;
  u8 unk6[3];
  u8 unk9_0 : 2;
  u8 unk9_2 : 2;
  u8 unk9_4 : 4;
  u8 unkA[6];
  u8 vramBlock;
  u8 unk11[10];
  u8 unk1b;
};

/// The heap slots read as words.
struct HeapWords {
  u8 unk00[108];
  s32 eventWord;
};

/// The event work's step counter.
struct AerieEventWork {
  u8 unk000[452];
  union {
    union StoredHalfword step;
    u16 stepRead;
  };
};

void Func_02003658(void) {
  struct AerieEventWork *work, *later;
  struct OverlaySprite *sprite;
  s32 created, zero, firstBeam, object, tiles, glowTask, message;
  s32 anchor, y, x, z, spawned, block;
  s32 beam, beam2, piece;
  s32 piece2, piece3;
  s32 owner, actor;

  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Func_0200567c, 28, 0, 0);
  Call3((void (*)())Func_0200567c, 29, 0, 0);
  Call3((void (*)())Func_0200567c, 30, 0, 0);
  Call2((void (*)())Func_02005724, 0x19999, SPLIT_CONSTANT(0x3300, 51));
  A((Value0((s32 (*)())Func_0200573c)))->motionFlags = 0;
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x2000000, 0x600000, 0x1380000,
        1);
  Func_02005734();
  Call1((void (*)())wait, 1);
  *(u16 *)0x4000052 = 0xc08;
  *(u16 *)0x4000050 = 0x3f10;
  Call6((void (*)())Func_020055ac, 18, 40, 18, 16, 23, 10);
  Call1((void (*)())Func_02003648, 7);
  Call1((void (*)())Func_02003648, 5);
  Call1((void (*)())Func_02003648, 6);
  firstBeam = 23;
  zero = 0;
  Call1((void (*)())Func_02003648, 19);
  Call1((void (*)())Func_02003648, 0);
  Call1((void (*)())Func_02003648, 3);
  Call1((void (*)())Func_02003648, 2);
  Call1((void (*)())Func_02003648, 1);
  Call2((void (*)())Event_PrepareObjectAndApplyValue, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, (OWNER), 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, (OWNER), 0x200, 0x118);
  Call2((void (*)())Func_02005704, (OWNER), 0xc000);
  Call2((void (*)())Motion_SetVarCbAndRefresh, (OWNER), 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call1((void (*)())PartyInventory_Remove, 222);
  anchor = Value1((s32 (*)())Actor_GetById, 4);
  y = A(anchor)->y;
  x = A(anchor)->x;
  y += 0x80000;
  z = A(anchor)->z;
  spawned = Func_02005574(0x1e9, x, y, z);
  created = spawned;
  object = spawned;
  if (created == 0)
    goto L_0200b8ac;
  sprite = (struct OverlaySprite *)A(object)->animation;
  sprite->unk5_5 = 0;
  sprite->unk1b = zero;
  sprite->unk9_4 = 0;
  sprite->unk9_2 = 2;
  A(object)->priorityFlags &= 254;
  A(object)->motionFlags = zero;
  A(object)->unk5c = 1;
  A(object)->priorityFlags |= 2;
  Call2((void (*)())Actor_SetSingleChildFlags, created, 0);
  A(object)->speedLimit = SPLIT_CONSTANT(0x6600, 102);
  A(object)->acceleration = SPLIT_CONSTANT(0xc00, 204);
  A(object)->gravity = 0x8000;
  block = Value2((s32 (*)())HeapSlot_AllocFar, 68, 0x608);
  Call1((void (*)())Func_020055ec, 247);
  tiles = (block + 0x400);
  Call3((void (*)())VramCache_Load, sprite->vramBlock, 128, tiles);
  Call1((void (*)())HeapSlot_Release, 68);
  A(object)->motionFlags = zero;
  Call4((void (*)())Func_02005594, object, 0x2000000, 0x800000, 0x1180000);
  Call1((void (*)())Func_0200559c, object);
  A(object)->motionFlags = 4;
  Call1((void (*)())FieldEvent_Wait, 60);
  A(object)->motionFlags = zero;
  Call4((void (*)())Func_02005594, object, 0x2000000, 0x800000, 0x1080000);
  Call1((void (*)())Func_0200559c, object);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call4((void (*)())Func_02005594, object, 0x2000000, 0x500000, 0xfc0000);
  Call1((void (*)())Func_0200559c, object);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call1((void (*)())Func_0200557c, object);
L_0200b8ac:
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Actor_SetSpritePriority, 23, 2);
  Call2((void (*)())Actor_SetSpritePriority, 24, 2);
  Call2((void (*)())Actor_SetSpritePriority, 25, 2);
  Call2((void (*)())Actor_SetSpritePriority, 26, 2);
  Call2((void (*)())Actor_SetSpritePriority, 27, 2);
  Call1((void (*)())Func_020057dc, 141);
  Call3((void (*)())Func_020055c4, 0x10000, 0x10000, 0x10000);
  Call2((void (*)())Actor_SetPartAttribute,
        (Value1((s32 (*)())Actor_GetById, 23)), 7);
  beam = Value1((s32 (*)())Actor_GetById, 23);
  A(beam)->scaleX = 0x1999;
  A(beam)->scaleY = 0xffff0000;
  A(beam)->motionFlags = zero;
  Call4((void (*)())Actor_SetPositionAndResetMotion, beam, 0x2000000, 0x400000,
        0xcc0000);
  Call2((void (*)())Actor_SetPartAttribute,
        (Value1((s32 (*)())Actor_GetById, 24)), 7);
  beam2 = Value1((s32 (*)())Actor_GetById, 24);
  A(beam2)->scaleX = 0x1999;
  A(beam2)->scaleY = 0xffff0000;
  A(beam2)->motionFlags = zero;
  Call4((void (*)())Actor_SetPositionAndResetMotion, beam2, 0x2000000, 0x600000,
        0xcc0000);
  Value2((s32 (*)())Task_Add, (s32)Func_0200326c, 0x480);
  Call3((void (*)())Actor_ArmCallback, 7, 0xe000, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0xe000, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0xe000, 0);
  Call3((void (*)())Actor_ArmCallback, 19, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 0, 0xa000, 0);
  Call3((void (*)())Actor_ArmCallback, 3, 0xa000, 0);
  Call3((void (*)())Actor_ArmCallback, 1, 0xa000, 0);
  Call3((void (*)())Actor_ArmCallback, 1, 0xa000, 80);
  Call2((void (*)())Actor_SetMovementTypeById, 23, (s32)gData_020060b4);
  Call2((void (*)())Actor_SetMovementTypeById, 24, (s32)gData_020060b4);
  Call1((void (*)())Func_020057dc, 145);
  Call3((void (*)())Func_020055c4, 0x60000, 0x60000, 0x10000);
  Call2((void (*)())Func_0200575c, 0x4063ff, 0);
  Call1((void (*)())Func_0200576c, 16);
  Call1((void (*)())wait, 20);
  Call2((void (*)())Func_0200575c, SPLIT_CONSTANT(0x7f00, 255), 0);
  Call1((void (*)())Func_0200576c, 24);
  Call1((void (*)())wait, 60);
  Call1((void (*)())Func_020057dc, 141);
  Call1((void (*)())GameFlag_SetBit, 0x200);
  A((Value1((s32 (*)())Actor_GetById, 23)))->y = 0xffc80000;
  A((Value1((s32 (*)())Actor_GetById, 24)))->y = 0xffe80000;
  Call2((void (*)())Actor_SetPartAttribute,
        (Value1((s32 (*)())Actor_GetById, 25)), 7);
  piece = Value1((s32 (*)())Actor_GetById, 25);
  A(piece)->scaleY = 0xffff0000;
  A(piece)->scaleX = (A((Value1((s32 (*)())Actor_GetById, 23)))->scaleX);
  A(piece)->motionFlags = zero;
  Call4((void (*)())Actor_SetPositionAndResetMotion, piece, 0x2000000, 0x80000,
        0xcc0000);
  Call2((void (*)())Actor_SetPartAttribute,
        (Value1((s32 (*)())Actor_GetById, 26)), 7);
  piece2 = Value1((s32 (*)())Actor_GetById, 26);
  A(piece2)->scaleY = 0xffff0000;
  A(piece2)->scaleX = (A((Value1((s32 (*)())Actor_GetById, 23)))->scaleX);
  A(piece2)->motionFlags = zero;
  Call4((void (*)())Actor_SetPositionAndResetMotion, piece2, 0x2000000,
        0x280000, 0xcc0000);
  Call2((void (*)())Actor_SetPartAttribute,
        (Value1((s32 (*)())Actor_GetById, 27)), 7);
  piece3 = Value1((s32 (*)())Actor_GetById, 27);
  A(piece3)->scaleY = 0xffff0000;
  A(piece3)->scaleX = (A((Value1((s32 (*)())Actor_GetById, 23)))->scaleX);
  A(piece3)->motionFlags = zero;
  Call4((void (*)())Actor_SetPositionAndResetMotion, piece3, 0x2000000,
        0x480000, 0xcc0000);
  Call6((void (*)())Func_020055a4, 106, 36, 87, 7, 17, firstBeam);
  Call6((void (*)())Func_020055a4, 42, 98, 23, 69, 17, firstBeam);
  Call6((void (*)())Func_020055ac, 0, 40, 18, 20, firstBeam, 10);
  Call1((void (*)())Func_020035ec, 7);
  Call1((void (*)())Func_020035ec, 5);
  Call1((void (*)())Func_020035ec, 6);
  glowTask = (s32)Func_0200326c;
  Call1((void (*)())Func_020035ec, 20);
  Call1((void (*)())Func_020035ec, 21);
  Call1((void (*)())Func_020035ec, 19);
  Call1((void (*)())Func_0200361c, 2);
  Call1((void (*)())Func_0200361c, 1);
  Call1((void (*)())Func_0200361c, 22);
  Call1((void (*)())Func_0200361c, 0);
  Call1((void (*)())Func_0200361c, 3);
  owner = OWNER;
  {
    s32 speed = 0xe000;
    Call4((void (*)())Func_02005684, owner, 0x1e40000, 0x1380000, speed);
  }
  Call2((void (*)())Actor_SetSpritePriority, (OWNER), 1);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Func_020055c4, 0x40000, 0x40000, 0x10000);
  Call2((void (*)())Func_0200575c, 0x4063ff, 0);
  Call1((void (*)())Func_0200576c, 120);
  Call2((void (*)())Actor_SetMovementTypeById, 23, (s32)gData_020060fc);
  Call2((void (*)())Actor_SetMovementTypeById, 24, (s32)gData_020060fc);
  Call2((void (*)())Actor_SetMovementTypeById, 25, (s32)gData_020060fc);
  Call2((void (*)())Actor_SetMovementTypeById, 26, (s32)gData_020060fc);
  Call2((void (*)())Actor_SetMovementTypeById, 27, (s32)gData_020060fc);
  Call1((void (*)())wait, 120);
  Call3((void (*)())Func_020055c4, 0x30000, 0x30000, 0x10000);
  Call2((void (*)())Func_0200575c, 0x203210, 0);
  Call1((void (*)())Func_0200576c, 120);
  Call1((void (*)())wait, 120);
  Call3((void (*)())Func_020055c4, 0x20000, 0x20000, 0x10000);
  Call2((void (*)())Func_0200575c, 0x10000, 0);
  Call1((void (*)())Func_0200576c, 120);
  Call1((void (*)())wait, 120);
  Call3((void (*)())Func_020055c4, 0x10000, 0x10000, 0x10000);
  Call1((void (*)())Func_020057dc, 78);
  A((Value1((s32 (*)())Actor_GetById, 23)))->scaleY = SPLIT_CONSTANT(0x500, 30);
  Call2((void (*)())Actor_SetMovementTypeById, 24, (s32)gData_02006120);
  Call2((void (*)())Actor_SetMovementTypeById, 25, (s32)gData_02006120);
  Call2((void (*)())Actor_SetMovementTypeById, 26, (s32)gData_02006120);
  Call2((void (*)())Actor_SetMovementTypeAndWaitById, 27, (s32)gData_02006120);
  Call1((void (*)())Func_020057dc, 0x186);
  Call2((void (*)())Actor_SetPartAttribute,
        (Value1((s32 (*)())Actor_GetById, 23)), 15);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Actor_SetMovementTypeAndWaitById, 23, (s32)gData_02006150);
  Call1((void (*)())Task_RemoveFar, glowTask);
  actor = Value1((s32 (*)())Actor_GetById, 8);
  A(actor)->motionFlags = zero;
  Call2((void (*)())Animation_SetStateFlags, actor, 1);
  Call2((void (*)())Actor_SetPartAttribute,
        (Value1((s32 (*)())Actor_GetById, 8)), 7);
  Call2((void (*)())Actor_SetSpritePriority, 8, 2);
  *(s32 *)(s32)Data_02006ccc = zero;
  *(s32 *)(s32)Data_02006b68 = 240;
  Value2((s32 (*)())Task_Add, (s32)Func_02003370, 0x480);
L_0200bcea:
  Call1((void (*)())wait, 1);
  if ((u32)(*(s32 *)(s32)Data_02006ccc) <= (u32)100)
    goto L_0200bcea;
  Call1((void (*)())Func_020057dc, 72);
  Call1((void (*)())FieldEvent_Wait, 120);
  Call3((void (*)())Func_02005714, 3, 0x109, 40);
  Call1((void (*)())Func_020056dc, (s32)MsgTheBeaconIs);
  Call2((void (*)())Func_020056f4, 3, 0);
  Func_02005714(2, 0x105, 40);
  Call2((void (*)())Func_020056f4, 2, 0);
  Call2((void (*)())Func_020056f4, 6, 0);
  Func_02005714(7, 0x109, 20);
  Call2((void (*)())Func_020056f4, 7, 0);
  Func_02005714(1, 0x103, 40);
  Call2((void (*)())Func_020056f4, 1, 0);
  Call3((void (*)())Actor_ArmCallback, 7, 0, 0);
  Call3((void (*)())Actor_ArmCallback, (OWNER), 0, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 19, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 3, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 0, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 1, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 2, 0x8000, 20);
  Call2((void (*)())Actor_SetModeById, 7, 3);
  Call2((void (*)())Actor_SetModeById, (OWNER), 3);
  Call2((void (*)())Actor_SetModeById, 5, 3);
  Call2((void (*)())Actor_SetModeById, 6, 3);
  Call2((void (*)())Actor_SetModeById, 19, 3);
  Call2((void (*)())Actor_SetModeById, 3, 3);
  Call2((void (*)())Actor_SetModeById, 0, 3);
  Call2((void (*)())Actor_SetModeById, 1, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 2, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Call3((void (*)())Actor_ArmCallback, (OWNER), 0x4000, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 19, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 3, 0x4000, 0);
  Call3((void (*)())Actor_ArmCallback, 1, 0xe000, 0);
  Call2((void (*)())Func_02005704, 0, 0);
  Call3((void (*)())Func_020056ec, SPLIT_CONSTANT(0x4000, 5), 0, 10);
  Call3((void (*)())Func_020055c4, -1, -1, 0xe666);
  Func_020055cc();
  Call2((void (*)())Func_0200578c, 23, 24);
  Call2((void (*)())FieldMessage_Show, ((s32)MsgTheBeaconIs + 6), 1);
  Func_02005794();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_0200578c, 23, 24);
  Call2((void (*)())FieldMessage_Show, ((s32)MsgTheBeaconIs + 7), 1);
  Func_02005794();
  work = Ram_HeapSlots->eventWork;
  work->step.value = work->stepRead + 2;
  Call1((void (*)())Func_02000610, (OWNER));
  Call1((void (*)())Func_02000610, 7);
  Call1((void (*)())Func_02000610, 5);
  Call1((void (*)())Func_02000610, 6);
  Call1((void (*)())Func_02000610, 19);
  Call1((void (*)())Func_02000610, 1);
  Call1((void (*)())Func_02000610, 2);
  Call1((void (*)())Func_02000610, 3);
  Call3((void (*)())Actor_ArmCallback, 0, 0xc000, 40);
  Call3((void (*)())Actor_ArmCallback, 0, 0x4000, 40);
  Call2((void (*)())Func_02005704, 0, 0x4000);
  Call3((void (*)())Func_020056ec, 0x6000, 0, 10);
  Call2((void (*)())Func_0200578c, 23, 24);
  message = ((s32)MsgTheBeaconIs + 9);
  Call2((void (*)())FieldMessage_Show, message, 1);
  Func_02005794();
  Call1((void (*)())Func_02000640, 3);
  Call2((void (*)())Func_02005704, SPLIT_CONSTANT(0x6000, 3), 0x4000);
  A((Value1((s32 (*)())Actor_GetById, 3)))->update = 0;
  // Read as a word, so the store to the actor before it may alias.
  later =
      (struct AerieEventWork *)((struct HeapWords *)Ram_HeapSlots)->eventWord;
  later->stepRead = later->stepRead + 1;
  Func_02005714(3, 0x100, 20);
  Call3((void (*)())Func_020056ec, 3, 0, 20);
  Call1((void (*)())Func_020057dc, 201);
  Call2((void (*)())Func_0200575c, SPLIT_CONSTANT(0x7f00, 255), 0);
  Call1((void (*)())Func_0200576c, 20);
  Call1((void (*)())wait, 20);
  Call1((void (*)())Func_02005744, 3);
  return;
}
