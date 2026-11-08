// Differ 34 of 1038 (tla-en, 2026-11-08, -mtune=gba). Rechecked 2026-11-08: the
// held struct sizes the frame (without it 38; a volatile word in its place 34);
// W_Func_02005714 is needed (direct 146, Call3 36); zero or mask as u32, u8, s16
// or u16 (34, mask s16 83), declared in either order (34); zero set before the
// mask (61); the last or as scratch | one (34) or with its result in one or
// scratch (79 to 82); a prototype for Camera_MoveWithinMapBounds (34); the
// permuter, 6 minutes (4920 candidates), found nothing better.
// Near miss, differ 34 of 1038 (tla-en, 2026-11-06): the slots and the
// mask/zero register choice, as below. Left: the slots (14 rows):
// the one spilled actor at sp+4 natively with four empty bytes below it, here
// at sp with the four above it: a local always sits above reload's slots, and
// a real stack struct for it takes the frame's address in a register. Rank
// (16 rows): mask (6 refs, 620 insns) against the zero (4 refs, 390),
// local-alloc's order; the native has mask first, r8, and a pin on the zero
// (r10) reaches those rows. Both Camera_MoveWithinMapBounds calls: the native
// shifts r0 first; here r0 comes last. The last flag set ors into r3 where
// the native ors into r2. The FIELD_AT and t names are the translator's and
// need replacing before adoption. Both long-lived zeros (r8 early, the
// one beside the mask) are the 0 a FIELD_AT byte store builds as (field & 0) |
// value; plain pointer-cast stores lose them (differ 84), and a u8:1 bitfield
// at 90 builds the mask as -2 with no zero (differ 78). Tried without gain: wrapper
// calls made direct near the mask lengthen its life (626, 34); u8 or u16 mask
// 72; arrays, unions and an 8-byte struct for the spilled actor stay a pseudo
// (frame 4, 38). Second pass (2026-11-06): the two direct
// Camera_MoveWithinMapBounds calls through Call4, unchanged (34).
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Camera_MoveWithinMapBounds();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_Wait();
s32 Func_020053e4();
s32 Func_020054e4();
s32 Func_0200560c();
s32 Func_0200567c();
s32 Func_02005684();
s32 Func_020056dc();
s32 Func_020056f4();
s32 Func_02005704();
s32 Func_02005714();
s32 Func_0200571c();
s32 Func_02005724();
s32 Func_02005734();
s32 Func_0200573c();
s32 Func_02005744();
s32 Func_0200575c();
s32 Func_02005764();
s32 Func_0200576c();
s32 Func_02005784();
s32 Func_020057dc();
s32 Motion_SetModeAndWaitAnimation();
s32 Motion_SetVarCbAndRefresh();
s32 Actor_ArmCallback();
s32 Actor_SetMovementTypeById();
s32 Actor_Jump();
s32 Actor_SetSpritePriority();
s32 Actor_SetPositionAndReset();
s32 Actor_SetSpeedParameters();
s32 Actor_StartRepeatedMotion();
s32 Actor_GetById();
s32 Actor_SetMovementTypeAndWaitById();
s32 Actor_SetModeById();
s32 Actor_SetPartAttribute();
s32 Random16Far();
s32 wait();
extern u8 Func_0200008c[];
extern u8 Func_02001eb0[];
extern u8 Func_02001ee4[];
extern u8 MsgWhoAreThey[];
extern u8 gData_02005f08[];
extern u8 gData_02005f50[];
extern u8 gData_02005f98[];
extern u8 gData_02005fdc[];
extern u8 gData_02006004[];
extern u8 gData_0200602c[];
extern u8 gData_02006070[];
extern u8 gData_02006b64[];
extern u8 gData_02006b6c[];
extern u8 gData_02006b70[];
extern u8 gData_02006c20[];
extern u8 gMainRoutineWork[];

static __inline__ void W_Func_02005714(s32 a0, s32 a1, s32 a2) {
  Func_02005714(a0, a1, a2);
}

void Func_02001f10(void) {
  struct {
    u8 bytes[4];
  } held;
  s32 lastArg, spill, zero, scratch;
  s32 owner, actor, mask, owner2, one, t1;
  s32 t2, t16, t34;
  s32 t85, t86, t87;
  s32 t158, t162;
  s32 t166, t170;
  s32 t244, t248;

  actor = Value1(Actor_GetById, 17);
  t1 = Value1(Actor_GetById, 30);
  t2 = Value1(Actor_GetById, 31);
  spill = (Value1(Actor_GetById, 32));
  Func_0200560c();
  Call1(Func_02005784, 0);
  Call2(Actor_SetModeById, 17, 2);
  FIELD_AT(actor, u8, 85) = 0;
  scratch = FIELD_AT(actor, s32, 8);
  FIELD_AT(actor, s32, 8) = (scratch - 0x40000);
  scratch = FIELD_AT(actor, s32, 16);
  FIELD_AT(actor, s32, 16) = (scratch + 0x100000);
  Call2(Actor_SetModeById, 28, 2);
  Call2(Actor_SetModeById, 29, 3);
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call1(wait, 1);
  Call2(Actor_SetModeById, 30, 7);
  Call2(Actor_SetModeById, 31, 7);
  Call2(Actor_SetModeById, 32, 6);
  owner = (s32)gMainRoutineWork;
  owner = (owner + 0x214);
  t16 = *(s32 *)owner;
  lastArg = 0xc000;
  Call4(Func_02005684, t16, 0x1f80000, 0x1400000, lastArg);
  Call1(wait, 1);
  Call2(Func_02005764, 0, 0);
  Call2(Func_0200575c, 0, 0);
  Call1(Func_0200576c, 1);
  Call2(Actor_SetPartAttribute, (Value1(Actor_GetById, 30)), 15);
  Call2(Actor_SetPartAttribute, (Value1(Actor_GetById, 31)), 15);
  Call2(Actor_SetPartAttribute, (Value1(Actor_GetById, 32)), 15);
  Call1(wait, 1);
  Call2(Actor_SetSpritePriority, 21, 1);
  Call2(Actor_SetSpritePriority, 20, 1);
  Call2(Actor_SetSpritePriority, 19, 1);
  Call2(Actor_SetSpritePriority, 26, 1);
  Call2(Actor_SetSpritePriority, 25, 1);
  t34 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t34, s32, 428) = 0x204;
  FIELD_AT(t34, s32, 436) = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1(Func_020057dc, 0x186);
  Call2(Func_0200575c, 0x10003, 1);
  Call2(Func_0200575c, 0x10000, 2);
  Call1(Func_0200576c, 16);
  Call1(wait, 16);
  Call1(FieldEvent_Wait, 80);
  *(s32 *)(s32)gData_02006b64 = 0;
  *(s32 *)(s32)gData_02006b6c = 4;
  FIELD_AT((Value1(Actor_GetById, 17)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT((Value1(Actor_GetById, 27)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT((Value1(Actor_GetById, 28)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT((Value1(Actor_GetById, 29)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT(t1, s32, 108) = (s32)Func_02001ee4;
  FIELD_AT(t2, s32, 108) = (s32)Func_02001ee4;
  FIELD_AT((spill), s32, 108) = (s32)Func_02001ee4;
  Call1(Func_020057dc, 0x253);
  Call1(wait, 10);
  Call1(Func_020057dc, 0x253);
  Call1(wait, 10);
  Call1(Func_020057dc, 0x253);
  Call1(wait, 10);
  Call1(Func_020057dc, 0x253);
  Call1(wait, 10);
  Call1(Func_020057dc, 0x253);
  Call1(wait, 40);
  *(s32 *)(s32)gData_02006b6c = 3;
  Call1(wait, 30);
  *(s32 *)(s32)gData_02006b6c = 2;
  Call1(wait, 20);
  Call3(Func_0200567c, 17, 0, 0);
  Call3(Func_0200567c, 27, 0, 0);
  Call3(Func_0200567c, 28, 0, 0);
  Call3(Func_0200567c, 29, 0, 0);
  FIELD_AT((Value1(Actor_GetById, 17)), s32, 108) = 0;
  FIELD_AT((Value1(Actor_GetById, 27)), s32, 108) = 0;
  FIELD_AT((Value1(Actor_GetById, 28)), s32, 108) = 0;
  FIELD_AT((Value1(Actor_GetById, 29)), s32, 108) = 0;
  FIELD_AT(t1, s32, 108) = 0;
  FIELD_AT(t2, s32, 108) = 0;
  FIELD_AT((spill), s32, 108) = 0;
  Call1(wait, 1);
  Call1(Func_020057dc, 103);
  Call2(Actor_SetPartAttribute, (Value1(Actor_GetById, 30)), 9);
  Call2(Actor_SetPartAttribute, (Value1(Actor_GetById, 31)), 9);
  Call2(Actor_SetPartAttribute, (Value1(Actor_GetById, 32)), 9);
  Call1(FieldEvent_Wait, 20);
  Call2(Actor_SetPartAttribute, (Value1(Actor_GetById, 30)), 0);
  Call2(Actor_SetPartAttribute, (Value1(Actor_GetById, 31)), 0);
  Call2(Actor_SetPartAttribute, (Value1(Actor_GetById, 32)), 0);
  Call1(FieldEvent_Wait, 20);
  t85 = Value0(Random16Far);
  FIELD_AT(t1, u8, 98) = (((u32)(((t85 << 2) + t85) << 4) >> 16) + 40);
  t86 = Value0(Random16Far);
  FIELD_AT(t2, u8, 98) = (((u32)(((t86 << 2) + t86) << 4) >> 16) + 40);
  t87 = Value0(Random16Far);
  FIELD_AT((spill), u8, 98) = (((u32)(((t87 << 2) + t87) << 4) >> 16) + 40);
  FIELD_AT(t1, s32, 108) = (s32)Func_0200008c;
  FIELD_AT(t2, s32, 108) = (s32)Func_0200008c;
  FIELD_AT((spill), s32, 108) = (s32)Func_0200008c;
  Call1(FieldEvent_Wait, 20);
  Call2(Func_0200575c, 0x10000, 1);
  Call1(Func_0200576c, 40);
  Call1(wait, 80);
  Call1(Func_020056dc, (s32)MsgWhoAreThey);
  Call2(Func_02005704, 26, 0x8000);
  Call2(Func_020056f4, 26, 0);
  Call2(Func_020056f4, 19, 0);
  Call3(Actor_ArmCallback, 22, 0x2000, 0);
  Call3(Actor_ArmCallback, (*(s32 *)owner), 0x2000, 0);
  Call3(Actor_ArmCallback, 23, 0x4000, 0);
  Call3(Actor_ArmCallback, 24, 0x4000, 0);
  Call3(Actor_ArmCallback, 21, 0x2000, 0);
  Call3(Actor_ArmCallback, 20, 0x2000, 0);
  Call3(Actor_ArmCallback, 26, 0x4000, 0);
  Call3(Actor_ArmCallback, 25, 0x4000, 0);
  Call2(Func_02005724, 0x19999, 0x3333);
  FIELD_AT((Value0(Func_0200573c)), u8, 85) = 0;
  Camera_MoveWithinMapBounds(0x2000000, 0x500000, 0x1540000, 1);
  Func_02005734();
  Func_02005714(19, 0x109, 20);
  Call2(Func_020056f4, 19, 0);
  Call1(Func_020057dc, 8);
  Call2(Func_0200571c, 24, 0x102);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_020056f4, 24, 0);
  Call2(Func_020056f4, 25, 0);
  Call2(Motion_SetModeAndWaitAnimation, 19, 3);
  Call2(Func_020056f4, 19, 0);
  Call2(Func_0200571c, 26, 0x102);
  Call2(Func_020056f4, 0x101a, 0);
  Call2(Actor_SetModeById, 19, 4);
  Call2(Func_020056f4, 19, 0);
  Func_02005714(23, 0x103, 0);
  Call3(Actor_SetSpeedParameters, 23, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndReset, 23, 0x200, 0x148);
  Call2(Func_02005704, 23, 0x2000);
  Call2(Func_020056f4, 0x1017, 0);
  Call2(Func_02005704, 20, 0x8000);
  Call2(Func_020056f4, 0xa014, 0);
  Func_02005714(19, 0x100, 20);
  Call2(Actor_SetSpritePriority, 21, 2);
  FIELD_AT((Value1(Actor_GetById, 20)), u8, 98) = 0;
  FIELD_AT((Value1(Actor_GetById, 21)), u8, 98) = 0;
  Call3(Actor_SetSpeedParameters, 21, 0xcccc, 0x6666);
  Call3(Actor_SetSpeedParameters, 20, 0xcccc, 0x6666);
  Call2(Actor_SetMovementTypeById, 21, (s32)gData_02005f08);
  Call2(Actor_SetMovementTypeById, 20, (s32)gData_02005f50);
  Call3(Actor_SetSpeedParameters, 19, 0x26666, 0x13333);
  Call3(Actor_SetPositionAndReset, 19, 0x21c, 0x154);
  Call3(Actor_Jump, 19, 4, 10);
  Call2(Func_020056f4, 19, 0);
  actor = Value1(Actor_GetById, 21);
L_0200a47a:
  Call1(wait, 1);
  scratch = actor;
  scratch = (scratch + 98);
  scratch = FIELD_AT(scratch, u8, 0);
  if (scratch == 0)
    goto L_0200a47a;
  Call3(Actor_ArmCallback, 20, 0x2000, 0);
  Call2(Func_02005704, 21, 0x2000);
  Call2(Func_020056f4, 21, 0);
  Call2(Actor_SetSpritePriority, 20, 2);
  Call2(Actor_SetSpritePriority, 22, 2);
  W_Func_02005714(20, 0x100, 0);
  Call3(Actor_SetSpeedParameters, 20, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndReset, 20, 0x1ea, 0x128);
  Call3(Actor_ArmCallback, 21, 0xe000, 20);
  Call3(Actor_ArmCallback, 20, 0xa000, 20);
  Call3(Actor_ArmCallback, 20, 0xe000, 20);
  Call3(Actor_SetSpeedParameters, 20, 0x3333, 0x1999);
  t158 = Value1(Actor_GetById, 20);
  mask = 254;
  FIELD_AT(t158, u8, 90) = (mask & (FIELD_AT(t158, u8, 90)));
  zero = 0;
  Call3(Actor_SetPositionAndReset, 20, 0x1e8, 0x12c);
  Call1(FieldEvent_Wait, 1);
  t162 = Value1(Actor_GetById, 20);
  one = 1;
  FIELD_AT(t162, u8, 90) = ((FIELD_AT(t162, u8, 90)) | one);
  Call1(FieldEvent_Wait, 20);
  Call2(Motion_SetVarCbAndRefresh, 20, 2);
  t166 = Value1(Actor_GetById, 20);
  FIELD_AT(t166, u8, 90) = (mask & (FIELD_AT(t166, u8, 90)));
  Call3(Actor_SetPositionAndReset, 20, 0x1e6, 0x134);
  Call1(FieldEvent_Wait, 1);
  t170 = Value1(Actor_GetById, 20);
  FIELD_AT(t170, u8, 90) = ((FIELD_AT(t170, u8, 90)) | one);
  Call2(Func_02005724, 0xcccc, 0x1999);
  Camera_MoveWithinMapBounds(0x2000000, 0x600000, 0x1400000, 1);
  Func_02005734();
  Call3(Actor_ArmCallback, 21, 0, 40);
  Call3(Actor_ArmCallback, 21, 0xe000, 20);
  Call2(Func_02005704, 21, 0);
  Call2(Func_020056f4, 0x8015, 0);
  Call3(Actor_ArmCallback, 22, 0xc000, 0);
  owner2 = (s32)gMainRoutineWork;
  owner2 = (owner2 + 0x214);
  Call3(Actor_ArmCallback, (*(s32 *)owner2), 0xa000, 0);
  Call3(Actor_ArmCallback, 23, 0xa000, 0);
  Call3(Actor_ArmCallback, 24, 0xa000, 0);
  Call3(Actor_ArmCallback, 26, 0xa000, 0);
  Call3(Actor_ArmCallback, 25, 0xa000, 0);
  Call2(Func_020056f4, 0x8014, 0);
  actor = Value1(Actor_GetById, 32);
  FIELD_AT(actor, s32, 108) = zero;
  Call1(wait, 1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  Call2(Motion_SetVarCbAndRefresh, 32, 2);
  Call2(Func_020056f4, 32, 0);
  Func_02005714(23, 0x100, 0);
  Call3(Actor_ArmCallback, 23, 0xe000, 40);
  Call3(Actor_ArmCallback, 25, 0x8000, 0);
  Func_02005714(25, 0x101, 20);
  Call2(Func_020056f4, 25, 0);
  Call3(Actor_ArmCallback, 23, 0, 40);
  Call2(Func_02005704, 23, 0xc000);
  Call2(Actor_StartRepeatedMotion, 32, 3);
  Call2(Func_020056f4, 32, 0);
  Call3(Actor_ArmCallback, 24, 0xc000, 40);
  Call2(Func_02005704, 24, 0x6000);
  Call2(Func_020056f4, 0x4018, 0);
  Call2(Func_02005704, 21, 0x2000);
  Call2(Func_0200571c, 21, 0x102);
  Call2(Func_020056f4, 21, 0);
  Call3(Actor_ArmCallback, (*(s32 *)owner2), 0xc000, 0);
  Call3(Actor_ArmCallback, 24, 0xc000, 0);
  Call3(Actor_ArmCallback, 26, 0xc000, 0);
  Call3(Actor_ArmCallback, 25, 0xc000, 0);
  actor = Value1(Actor_GetById, 30);
  FIELD_AT(actor, s32, 108) = zero;
  Call1(wait, 1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  actor = Value1(Actor_GetById, 31);
  FIELD_AT(actor, s32, 108) = zero;
  Call1(wait, 1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  Call1(FieldEvent_Wait, 10);
  Call2(Motion_SetVarCbAndRefresh, 31, 1);
  Call2(Func_020056f4, 0x201f, 0);
  Call2(Motion_SetVarCbAndRefresh, 30, 2);
  Call2(Func_020056f4, 30, 0);
  Call3(Actor_Jump, 20, 2, 10);
  Call3(Actor_Jump, 20, 4, 10);
  Call2(Func_020056f4, 0x8014, 0);
  Call3(Actor_SetSpeedParameters, 20, 0x26666, 0x13333);
  Call3(Actor_SetSpeedParameters, (*(s32 *)owner2), 0x26666, 0x13333);
  Call3(Actor_SetSpeedParameters, 23, 0x26666, 0x13333);
  Call3(Actor_SetSpeedParameters, 24, 0x26666, 0x13333);
  Call2(Actor_SetMovementTypeById, 20, (s32)gData_02005f98);
  Call2(Actor_SetMovementTypeById, (*(s32 *)owner2), (s32)gData_02005fdc);
  Call2(Actor_SetMovementTypeById, 23, (s32)gData_02006004);
  Call2(Actor_SetMovementTypeAndWaitById, 24, (s32)gData_0200602c);
  Call1(FieldEvent_Wait, 20);
  Call3(Actor_SetSpeedParameters, 22, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndReset, 22, 0x210, 0x140);
  Call3(Actor_ArmCallback, 22, 0x2000, 0);
  Call2(Func_02005704, 25, 0xa000);
  Call2(Func_020056f4, 0x5016, 0);
  Call3(Actor_SetSpeedParameters, 25, 0x19999, 0xcccc);
  Call2(Actor_SetMovementTypeById, 25, (s32)gData_02006070);
  Call3(Actor_SetPositionAndReset, 22, 0x1e6, 0x134);
  Call2(Func_02005704, 22, 0xa000);
  Call3(Actor_SetSpeedParameters, 21, 0x26666, 0x13333);
  t244 = Value1(Actor_GetById, 21);
  scratch = FIELD_AT(t244, u8, 90);
  mask = (mask & scratch);
  FIELD_AT(t244, u8, 90) = (u8)mask;
  Call3(Actor_SetPositionAndReset, 21, 0x1da, 0x13c);
  Call1(FieldEvent_Wait, 1);
  t248 = Value1(Actor_GetById, 21);
  scratch = FIELD_AT(t248, u8, 90);
  FIELD_AT(t248, u8, 90) = (one | scratch);
  Call3(Actor_ArmCallback, 21, 0xe000, 0);
  Call3(Actor_SetPositionAndReset, 22, 0x1ce, 0x118);
  Call2(Func_02005704, 22, 0);
  Call2(Actor_SetSpritePriority, 22, 1);
  Call2(Func_020053e4, (Value1(Actor_GetById, 22)), (s32)gData_02006c20);
  Call2(Func_020053e4, (Value1(Actor_GetById, 25)), (s32)gData_02006b70);
  Call1(FieldEvent_Wait, 40);
  Call2(Motion_SetModeAndWaitAnimation, 19, 4);
  Call2(Func_020056f4, 19, 0);
  Call1(Func_020057dc, 78);
  Call2(Func_0200575c, 0, 0);
  Call1(Func_0200576c, 120);
  Call1(wait, 120);
  Call1(Actor_GetById, 22);
  Func_020054e4();
  Call1(Actor_GetById, 25);
  Func_020054e4();
  Call1(FieldEvent_Wait, 80);
  Call1(Func_02005744, 2);
  return;
}
