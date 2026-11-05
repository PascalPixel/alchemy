// Draft, differ 6 of 963, skeleton gap 3 (tla-en, 2026-11-01)
// Same 963 instructions; three pairs differ only by sched2 tie-breaks:
// movs #2 before ldrb at the bit 1 set on the actor's priorityFlags, movs r3
// before movs r2 in Func_02005684's arguments, mov r9 before ldr r0 ahead of
// the first Func_02000610 call. Each load in the listing stays behind the store
// before it, which looks like a memory conflict through the real types.
// Forward-dependence counts decide many ties here: a helper's void or value
// return changes the order at the next call, so those declarations are measured.
#include "types.h"
#include "call.h"
#include "system/main_routine_work.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
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
  u8 unkA[17];
  u8 unk1b;
};

void Func_02003658(void) {
  struct OverlaySprite *sprite;
  u8 *flags;
  s32 v_r0, v_r10, v_r11, v_r7, v_r5, v_r8, v_r6, t6, t20;
  s32 t22, t24, t26, t30, t31, t32, t33, t34, t41;
  s32 t43, t64, t66, t68, t70, t93, t94, t95, t97;
  s32 t98, t99, t101, t103, t104, t105, t107, t109, t110;
  s32 t111, t127, t129, t151, t157, t162, t164, t169, t183;
  s32 t193, t204, t222, t223, t224, t242, t243, t244;

  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Func_0200567c, 28, 0, 0);
  Call3((void (*)())Func_0200567c, 29, 0, 0);
  Call3((void (*)())Func_0200567c, 30, 0, 0);
  Call2((void (*)())Func_02005724, 0x19999, 0x3333);
  t6 = Value0((s32 (*)())Func_0200573c);
  A(t6)->motionFlags = 0;
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
  v_r11 = 23;
  v_r10 = 0;
  Call1((void (*)())Func_02003648, 19);
  Call1((void (*)())Func_02003648, 0);
  Call1((void (*)())Func_02003648, 3);
  Call1((void (*)())Func_02003648, 2);
  Call1((void (*)())Func_02003648, 1);
  Call2((void (*)())Event_PrepareObjectAndApplyValue, 0, 0);
  t20 = OWNER;
  Call3((void (*)())Actor_SetSpeedParameters, t20, 0xcccc, 0x6666);
  t22 = OWNER;
  Call3((void (*)())Actor_SetPositionAndReset, t22, 0x200, 0x118);
  t24 = OWNER;
  Call2((void (*)())Func_02005704, t24, 0xc000);
  t26 = OWNER;
  Call2((void (*)())Motion_SetVarCbAndRefresh, t26, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call1((void (*)())PartyInventory_Remove, 222);
  t30 = Value1((s32 (*)())Actor_GetById, 4);
  t31 = A(t30)->y;
  t32 = A(t30)->x;
  t31 += 0x80000;
  t33 = A(t30)->z;
  t34 = Func_02005574(0x1e9, t32, t31, t33);
  v_r0 = t34;
  v_r7 = t34;
  if (v_r0 == 0)
    goto L_0200b8ac;
  sprite = (struct OverlaySprite *)A(v_r7)->animation;
  sprite->unk5_5 = 0;
  sprite->unk1b = v_r10;
  sprite->unk9_4 = 0;
  sprite->unk9_2 = 2;
  A(v_r7)->priorityFlags &= 254;
  A(v_r7)->motionFlags = v_r10;
  A(v_r7)->unk5c = 1;
  A(v_r7)->priorityFlags |= 2;
  Call2((void (*)())Actor_SetSingleChildFlags, v_r0, 0);
  A(v_r7)->speedLimit = 0x6666;
  A(v_r7)->acceleration = 0xccc;
  A(v_r7)->gravity = 0x8000;
  t41 = Value2((s32 (*)())HeapSlot_AllocFar, 68, 0x608);
  Call1((void (*)())Func_020055ec, 247);
  v_r5 = (t41 + 0x400);
  t43 = FIELD_AT(sprite, u8, 16);
  Call3((void (*)())VramCache_Load, t43, 128, v_r5);
  Call1((void (*)())HeapSlot_Release, 68);
  A(v_r7)->motionFlags = v_r10;
  Call4((void (*)())Func_02005594, v_r7, 0x2000000, 0x800000, 0x1180000);
  Call1((void (*)())Func_0200559c, v_r7);
  A(v_r7)->motionFlags = 4;
  Call1((void (*)())FieldEvent_Wait, 60);
  A(v_r7)->motionFlags = v_r10;
  Call4((void (*)())Func_02005594, v_r7, 0x2000000, 0x800000, 0x1080000);
  Call1((void (*)())Func_0200559c, v_r7);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call4((void (*)())Func_02005594, v_r7, 0x2000000, 0x500000, 0xfc0000);
  Call1((void (*)())Func_0200559c, v_r7);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call1((void (*)())Func_0200557c, v_r7);
L_0200b8ac:
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Actor_SetSpritePriority, 23, 2);
  Call2((void (*)())Actor_SetSpritePriority, 24, 2);
  Call2((void (*)())Actor_SetSpritePriority, 25, 2);
  Call2((void (*)())Actor_SetSpritePriority, 26, 2);
  Call2((void (*)())Actor_SetSpritePriority, 27, 2);
  Call1((void (*)())Func_020057dc, 141);
  Call3((void (*)())Func_020055c4, 0x10000, 0x10000, 0x10000);
  t64 = Value1((s32 (*)())Actor_GetById, 23);
  Call2((void (*)())Actor_SetPartAttribute, t64, 7);
  t66 = Value1((s32 (*)())Actor_GetById, 23);
  A(t66)->scaleX = 0x1999;
  A(t66)->scaleY = 0xffff0000;
  A(t66)->motionFlags = v_r10;
  Call4((void (*)())Actor_SetPositionAndResetMotion, t66, 0x2000000, 0x400000,
        0xcc0000);
  t68 = Value1((s32 (*)())Actor_GetById, 24);
  Call2((void (*)())Actor_SetPartAttribute, t68, 7);
  t70 = Value1((s32 (*)())Actor_GetById, 24);
  A(t70)->scaleX = 0x1999;
  A(t70)->scaleY = 0xffff0000;
  A(t70)->motionFlags = v_r10;
  Call4((void (*)())Actor_SetPositionAndResetMotion, t70, 0x2000000, 0x600000,
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
  Call2((void (*)())Actor_SetMovementTypeById, 23,
        (s32)gData_020060b4);
  Call2((void (*)())Actor_SetMovementTypeById, 24,
        (s32)gData_020060b4);
  Call1((void (*)())Func_020057dc, 145);
  Call3((void (*)())Func_020055c4, 0x60000, 0x60000, 0x10000);
  Call2((void (*)())Func_0200575c, 0x4063ff, 0);
  Call1((void (*)())Func_0200576c, 16);
  Call1((void (*)())wait, 20);
  Call2((void (*)())Func_0200575c, 0x7fff, 0);
  Call1((void (*)())Func_0200576c, 24);
  Call1((void (*)())wait, 60);
  Call1((void (*)())Func_020057dc, 141);
  Call1((void (*)())GameFlag_SetBit, 0x200);
  t93 = Value1((s32 (*)())Actor_GetById, 23);
  A(t93)->y = 0xffc80000;
  t94 = Value1((s32 (*)())Actor_GetById, 24);
  A(t94)->y = 0xffe80000;
  t95 = Value1((s32 (*)())Actor_GetById, 25);
  Call2((void (*)())Actor_SetPartAttribute, t95, 7);
  t97 = Value1((s32 (*)())Actor_GetById, 25);
  A(t97)->scaleY = 0xffff0000;
  t98 = Value1((s32 (*)())Actor_GetById, 23);
  t99 = A(t98)->scaleX;
  A(t97)->scaleX = t99;
  A(t97)->motionFlags = v_r10;
  Call4((void (*)())Actor_SetPositionAndResetMotion, t97, 0x2000000, 0x80000,
        0xcc0000);
  t101 = Value1((s32 (*)())Actor_GetById, 26);
  Call2((void (*)())Actor_SetPartAttribute, t101, 7);
  t103 = Value1((s32 (*)())Actor_GetById, 26);
  A(t103)->scaleY = 0xffff0000;
  t104 = Value1((s32 (*)())Actor_GetById, 23);
  t105 = A(t104)->scaleX;
  A(t103)->scaleX = t105;
  A(t103)->motionFlags = v_r10;
  Call4((void (*)())Actor_SetPositionAndResetMotion, t103, 0x2000000, 0x280000,
        0xcc0000);
  t107 = Value1((s32 (*)())Actor_GetById, 27);
  Call2((void (*)())Actor_SetPartAttribute, t107, 7);
  t109 = Value1((s32 (*)())Actor_GetById, 27);
  A(t109)->scaleY = 0xffff0000;
  t110 = Value1((s32 (*)())Actor_GetById, 23);
  t111 = A(t110)->scaleX;
  A(t109)->scaleX = t111;
  A(t109)->motionFlags = v_r10;
  Call4((void (*)())Actor_SetPositionAndResetMotion, t109, 0x2000000, 0x480000,
        0xcc0000);
  Call6((void (*)())Func_020055a4, 106, 36, 87, 7, 17, v_r11);
  Call6((void (*)())Func_020055a4, 42, 98, 23, 69, 17, v_r11);
  Call6((void (*)())Func_020055ac, 0, 40, 18, 20, v_r11, 10);
  Call1((void (*)())Func_020035ec, 7);
  Call1((void (*)())Func_020035ec, 5);
  Call1((void (*)())Func_020035ec, 6);
  v_r8 = (s32)Func_0200326c;
  Call1((void (*)())Func_020035ec, 20);
  Call1((void (*)())Func_020035ec, 21);
  Call1((void (*)())Func_020035ec, 19);
  Call1((void (*)())Func_0200361c, 2);
  Call1((void (*)())Func_0200361c, 1);
  Call1((void (*)())Func_0200361c, 22);
  Call1((void (*)())Func_0200361c, 0);
  Call1((void (*)())Func_0200361c, 3);
  t127 = OWNER;
  {
    s32 speed = 0xe000;
    Call4((void (*)())Func_02005684, t127, 0x1e40000, 0x1380000, speed);
  }
  t129 = OWNER;
  Call2((void (*)())Actor_SetSpritePriority, t129, 1);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Func_020055c4, 0x40000, 0x40000, 0x10000);
  Call2((void (*)())Func_0200575c, 0x4063ff, 0);
  Call1((void (*)())Func_0200576c, 120);
  Call2((void (*)())Actor_SetMovementTypeById, 23,
        (s32)gData_020060fc);
  Call2((void (*)())Actor_SetMovementTypeById, 24,
        (s32)gData_020060fc);
  Call2((void (*)())Actor_SetMovementTypeById, 25,
        (s32)gData_020060fc);
  Call2((void (*)())Actor_SetMovementTypeById, 26,
        (s32)gData_020060fc);
  Call2((void (*)())Actor_SetMovementTypeById, 27,
        (s32)gData_020060fc);
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
  t151 = Value1((s32 (*)())Actor_GetById, 23);
  A(t151)->scaleY = 0x51e;
  Call2((void (*)())Actor_SetMovementTypeById, 24,
        (s32)gData_02006120);
  Call2((void (*)())Actor_SetMovementTypeById, 25,
        (s32)gData_02006120);
  Call2((void (*)())Actor_SetMovementTypeById, 26,
        (s32)gData_02006120);
  Call2((void (*)())Actor_SetMovementTypeAndWaitById, 27,
        (s32)gData_02006120);
  Call1((void (*)())Func_020057dc, 0x186);
  t157 = Value1((s32 (*)())Actor_GetById, 23);
  Call2((void (*)())Actor_SetPartAttribute, t157, 15);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Actor_SetMovementTypeAndWaitById, 23,
        (s32)gData_02006150);
  Call1((void (*)())Task_RemoveFar, v_r8);
  t162 = Value1((s32 (*)())Actor_GetById, 8);
  A(t162)->motionFlags = v_r10;
  Call2((void (*)())Animation_SetStateFlags, t162, 1);
  t164 = Value1((s32 (*)())Actor_GetById, 8);
  Call2((void (*)())Actor_SetPartAttribute, t164, 7);
  Call2((void (*)())Actor_SetSpritePriority, 8, 2);
  *(s32 *)(s32)Data_02006ccc = v_r10;
  *(s32 *)(s32)Data_02006b68 = 240;
  Value2((s32 (*)())Task_Add, (s32)Func_02003370, 0x480);
L_0200bcea:
  Call1((void (*)())wait, 1);
  t169 = *(s32 *)(s32)Data_02006ccc;
  if ((u32)t169 <= (u32)100)
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
  t183 = OWNER;
  Call3((void (*)())Actor_ArmCallback, t183, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 19, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 3, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 0, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 1, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 2, 0x8000, 20);
  Call2((void (*)())Actor_SetModeById, 7, 3);
  t193 = OWNER;
  Call2((void (*)())Actor_SetModeById, t193, 3);
  Call2((void (*)())Actor_SetModeById, 5, 3);
  Call2((void (*)())Actor_SetModeById, 6, 3);
  Call2((void (*)())Actor_SetModeById, 19, 3);
  Call2((void (*)())Actor_SetModeById, 3, 3);
  Call2((void (*)())Actor_SetModeById, 0, 3);
  Call2((void (*)())Actor_SetModeById, 1, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 2, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  t204 = OWNER;
  Call3((void (*)())Actor_ArmCallback, t204, 0x4000, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 19, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 3, 0x4000, 0);
  Call3((void (*)())Actor_ArmCallback, 1, 0xe000, 0);
  Call2((void (*)())Func_02005704, 0, 0);
  Call3((void (*)())Func_020056ec, 0x4005, 0, 10);
  Call3((void (*)())Func_020055c4, -1, -1, 0xe666);
  Func_020055cc();
  Call2((void (*)())Func_0200578c, 23, 24);
  Call2((void (*)())FieldMessage_Show,
        ((s32)MsgTheBeaconIs + 6), 1);
  Func_02005794();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_0200578c, 23, 24);
  Call2((void (*)())FieldMessage_Show,
        ((s32)MsgTheBeaconIs + 7), 1);
  Func_02005794();
  t222 = FIELD_AT(0x3000000, s32, 108);
  t223 = FIELD_AT(t222, u16, 452);
  FIELD_AT(t222, u16, 452) = (t223 + 2);
  t224 = OWNER;
  Call1((void (*)())Func_02000610, t224);
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
  v_r6 = ((s32)MsgTheBeaconIs + 9);
  Call2((void (*)())FieldMessage_Show, v_r6, 1);
  Func_02005794();
  Call1((void (*)())Func_02000640, 3);
  Call2((void (*)())Func_02005704, 0x6003, 0x4000);
  t242 = Value1((s32 (*)())Actor_GetById, 3);
  A(t242)->update = 0;
  t243 = FIELD_AT(0x3000000, s32, 108);
  t244 = FIELD_AT(t243, u16, 452);
  FIELD_AT(t243, u16, 452) = (t244 + 1);
  Func_02005714(3, 0x100, 20);
  Call3((void (*)())Func_020056ec, 3, 0, 20);
  Call1((void (*)())Func_020057dc, 201);
  Call2((void (*)())Func_0200575c, 0x7fff, 0);
  Call1((void (*)())Func_0200576c, 20);
  Call1((void (*)())wait, 20);
  Call1((void (*)())Func_02005744, 3);
  return;
}
