// Draft, differ N of M: XXX. Left:
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern const u8 MsgVeryWellShow[];
extern const u8 MsgShallIGive[];
extern const u8 MsgPlacedOnThe[];
extern const u8 MsgAreYouPrepared[];
void Func_02001138();
void Func_02001148();
void FieldEvent_WaitFrames();
void Motion_SetModeAndWaitAnimation();
s32 Func_020011a0();
s32 UiText_OpenMessageAtObject();
s32 Func_02001188();
s32 Event_AskYesNoWithActorAnimation();
s32 Unit_Get();
s32 Func_020010b8();
s32 Object_GetById();
void Func_020010a0();
void Func_02001108();
void Func_020010b0();
void UiText_ShowPositionedMessageAndWait();
void Func_02001170();
s32 Func_02001168();
s32 Func_02001178();
void Func_020011a8();
void Object_SetModeById();
void ObjectMotion_WaitForAnimationChange();
void ObjectMotion_ResetAndSetPositionInMode2();
void ObjectMotion_Launch();
s32 Func_02001160();
void Func_02000920();
s32 GameFlag_Test();
void Func_020007b4();
void Func_02000760(void);
void Func_02000838();
void Func_02000878(void);
void Party_AdjustCoins();

void Func_02000390(void) {
	.ifdef TLA_EDITION_JA
    sub sp, #8
  Call1(Func_02001138, 0x1ec1);
  Call2(Func_02001148, 10, 0);
  Call1(FieldEvent_WaitFrames, 30);
.L_ja_020083a8:
  Call2(Motion_SetModeAndWaitAnimation, gPartyState.currentOwner, 3); // ret
    cmp Value2(Func_020011a0, sp+4, ?), -0x1
    bne .L_ja_020083ea
  Call1(Func_02001138, 0x1ec4);
  Value2(UiText_OpenMessageAtObject, 10, 0);
    cmp Value2(Event_AskYesNoWithActorAnimation, Value0(Func_02001188), 0), 1
    bne .L_ja_020083a8
    b .L_ja_02008496
.L_ja_020083ea:
  Value1(Func_020010b8, ldrh(Value1(Unit_Get, ldr(?+4))+0+[(ldr(?+0) << 1)+216]+0));
  Call4(Func_020010a0, Value1(Object_GetById, 18), ldrh(ret(Unit_Get)+0+[(ldr(?+0) << 1)+216]+0), ?, (ldr(?+0) << 1)+216); /* XXX arity */
  Call3(Func_02001108, 18, 0x2a00000, 0x680000);
  Call1(FieldEvent_WaitFrames, 60);
  Call2(Func_020010b0, ldrh(ret(Unit_Get)+0+[(ldr(?+0) << 1)+216]+0), 2);
  Call2(UiText_ShowPositionedMessageAndWait, 0x1ebb, 1);
  Call1(Func_02001138, 0x1ec2);
  Value2(UiText_OpenMessageAtObject, 10, 0);
    cmp Value2(Event_AskYesNoWithActorAnimation, Value0(Func_02001188), 0), 1
    bne .L_ja_020084a4
  Call3(Func_02001108, 18, 0, 0);
  Call2(Func_020010b0, ldrh(ret(Unit_Get)+0+[(ldr(?+0) << 1)+216]+0), 2);
  Call2(UiText_ShowPositionedMessageAndWait, 0x1ec3, 1); // ret
  Call1(Func_02001138, 0x1ec4);
  Value2(UiText_OpenMessageAtObject, 10, 0);
    cmp Value2(Event_AskYesNoWithActorAnimation, Value0(Func_02001188), 0), 1
    bne .L_ja_020083a8
.L_ja_02008496:
  Call1(Func_02001138, ?);
  Call2(Func_02001148, 10, 0);
    b .L_ja_02008756
.L_ja_020084a4:
  Call1(Func_02001138, 0x1ec6);
  Call2(Func_02001148, 10, 0);
  Call2(Func_02001170, 0x10000, 0);
  Call1(FieldEvent_WaitFrames, 30);
  Value2(Func_02001168, 0x40250d, 1);
  Value1(Func_02001178, 30);
  Call1(FieldEvent_WaitFrames, 120);
  Call1(Func_020011a8, 11);
  Call2(Object_SetModeById, 10, 5);
  Call1(ObjectMotion_WaitForAnimationChange, 10);
  Value1(Object_GetById, 10); // ret
    ands r3, r2
    strb [ret(Object_GetById)+90 +0] = ?   ; r3
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 10, 664, 0x60);
  Call2(Object_SetModeById, 10, 5);
  Call1(FieldEvent_WaitFrames, 30);
  Value1(Object_GetById, 10); // ret
    ands r3, r2
    strb [ret(Object_GetById)+90 +0] = ?   ; r3
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 10, 680, 0x60);
  Call2(Object_SetModeById, 10, 5);
  Call3(ObjectMotion_Launch, 10, 4, 90);
  Value1(Object_GetById, 10); // ret
    ands r3, r2
    strb [ret(Object_GetById)+90 +0] = ?   ; r3
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 10, 664, 0x60);
  Call2(Object_SetModeById, 10, 5);
  Call1(FieldEvent_WaitFrames, 30);
  Value1(Object_GetById, 10); // ret
    ands r3, r2
    strb [ret(Object_GetById)+90 +0] = ?   ; r3
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 10, 680, 0x60);
  Call2(Object_SetModeById, 10, 5);
  Call3(ObjectMotion_Launch, 10, 4, 90);
  Value1(Object_GetById, 10); // ret
    ands r5, r3
    strb [ret(Object_GetById)+90 +0] = ?   ; r5
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 10, 0x2a0, 0x60);
  Call2(Object_SetModeById, 10, 5);
  Call3(ObjectMotion_Launch, 10, 4, 30);
  Call3(ObjectMotion_Launch, 10, 4, 30);
  Call2(Object_SetModeById, 10, 5);
  Call2(Object_SetModeById, 10, 0);
  Call1(Func_020011a8, 11);
  Value2(Func_02001168, 0x10000, 1);
  Value1(Func_02001178, 30);
  Call3(Func_02001108, 20, 0x2a00000, 0x680000);
  Call1(Func_020011a8, 0x90);
  Call2(Object_SetModeById, 20, 2);
  Call1(ObjectMotion_WaitForAnimationChange, 20);
  Call3(Func_02001108, 20, 0, 0);
  Call1(FieldEvent_WaitFrames, 4);
  Call3(Func_02001108, 19, 0x2a00000, 0x680000);
  Call2(Object_SetModeById, 19, 4);
  Call1(ObjectMotion_WaitForAnimationChange, 19);
  Call1(FieldEvent_WaitFrames, 15);
  Call3(Func_02001108, 19, 0, 0);
  Call1(FieldEvent_WaitFrames, 30);
  Value3(Func_02001160, gPartyState.currentOwner, 258, 0);
  Call1(FieldEvent_WaitFrames, 60);
  Call1(Func_020011a8, 10);
  Value3(Func_02001160, 10, 0x100, 60);
  Call2(Func_02001148, 10, 0);
    cmp (ldrh(Value1(Func_020010b8, ldrh(Value1(Unit_Get, ldr(?+4))+0+[(ldr(?+0) << 1)+216]+0))+0+[(ldr(?+0) << 1)+216]+0)+-0x1b7 << 16), 0x20000
    bhi .L_ja_020086ba
  Call4(Func_02000920, ldrh(ret(Unit_Get)+0+[(ldr(?+0) << 1)+216]+0), 0x20000, ret(Func_020010b8)+0, (ldrh(ret(Unit_Get)+0+[(ldr(?+0) << 1)+216]+0)+-0x1b7 << 16)); /* XXX arity */
    b .L_ja_02008744
.L_ja_020086ba:
    cmp ?, 0x1460000
    bne .L_ja_020086f8
  Call1(Func_02001138, 0x1edf);
  Call2(Func_02001148, 10, 0);
    cmp Value1(GameFlag_Test, 0x940), 0
    bne .L_ja_020086e6
    b .L_ja_020086ea
.L_ja_020086e6:
.L_ja_020086ea:
  Call1(Func_02001138, ?);
  Call2(Func_02001148, 10, 0);
    b .L_ja_02008744
.L_ja_020086f8:
    cmp ldrb(?+2)+0, 6
    bne .L_ja_02008706
  Call3(Func_020007b4, ldrb(?+2), ?, ldrb(?+2)+0); /* XXX arity */
    b .L_ja_02008744
.L_ja_02008706:
    cmp ?, 1
    bne .L_ja_02008710
  Func_02000760(); /* XXX arity */
    b .L_ja_02008744
.L_ja_02008710:
    cmp ?, 0x3000000
    bhi .L_ja_02008740
  Call4(Func_02000838, ?, ?, 0x3000000, ?); /* XXX arity */
    b .L_ja_02008744
.L_ja_02008740:
  Func_02000878(); /* XXX arity */
.L_ja_02008744:
  Call3(Func_02001108, 18, 0, 0);
  Call1(Party_AdjustCoins, -0x14);
.L_ja_02008756:
    movs r0, r0
	.else
    sub sp, #8
  Call1(Func_02001138, MsgVeryWellShow);
  Call2(Func_02001148, 10, 0);
  Call1(FieldEvent_WaitFrames, 30);
.L_020083a8:
  Call2(Motion_SetModeAndWaitAnimation, gPartyState.currentOwner, 3); // ret
    cmp Value2(Func_020011a0, sp+4, ?), -0x1
    bne .L_020083ea
  Call1(Func_02001138, MsgShallIGive+0);
  Value2(UiText_OpenMessageAtObject, 10, 0);
    cmp Value2(Event_AskYesNoWithActorAnimation, Value0(Func_02001188), 0), 1
    bne .L_020083a8
    b .L_02008496
.L_020083ea:
  Value1(Func_020010b8, ldrh(Value1(Unit_Get, ldr(?+4))+0+[(ldr(?+0) << 1)+216]+0));
  Call4(Func_020010a0, Value1(Object_GetById, 18), ldrh(ret(Unit_Get)+0+[(ldr(?+0) << 1)+216]+0), ?, (ldr(?+0) << 1)+216); /* XXX arity */
  Call3(Func_02001108, 18, 0x2a00000, 0x680000);
  Call1(FieldEvent_WaitFrames, 60);
  Call2(Func_020010b0, ldrh(ret(Unit_Get)+0+[(ldr(?+0) << 1)+216]+0), 2);
  Call2(UiText_ShowPositionedMessageAndWait, MsgPlacedOnThe, 1);
  Call1(Func_02001138, MsgAreYouPrepared+0);
  Value2(UiText_OpenMessageAtObject, 10, 0);
    cmp Value2(Event_AskYesNoWithActorAnimation, Value0(Func_02001188), 0), 1
    bne .L_020084a4
  Call3(Func_02001108, 18, 0, 0);
  Call2(Func_020010b0, ldrh(ret(Unit_Get)+0+[(ldr(?+0) << 1)+216]+0), 2);
  Call2(UiText_ShowPositionedMessageAndWait, MsgAreYouPrepared+1, 1); // ret
  Call1(Func_02001138, MsgAreYouPrepared+2);
  Value2(UiText_OpenMessageAtObject, 10, 0);
    cmp Value2(Event_AskYesNoWithActorAnimation, Value0(Func_02001188), 0), 1
    bne .L_020083a8
.L_02008496:
  Call1(Func_02001138, ?);
  Call2(Func_02001148, 10, 0);
    b .L_0200875a
.L_020084a4:
  Call1(Func_02001138, MsgAreYouPrepared+4);
  Call2(Func_02001148, 10, 0);
  Call2(Func_02001170, 0x10000, 0);
  Call1(FieldEvent_WaitFrames, 30);
  Value2(Func_02001168, 0x40250d, 1);
  Value1(Func_02001178, 30);
  Call1(FieldEvent_WaitFrames, 120);
  Call1(Func_020011a8, 11);
  Call2(Object_SetModeById, 10, 5);
  Call1(ObjectMotion_WaitForAnimationChange, 10);
  Value1(Object_GetById, 10); // ret
    ands r3, r2
    strb [ret(Object_GetById)+90 +0] = ?   ; r3
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 10, 664, 0x60);
  Call2(Object_SetModeById, 10, 5);
  Call1(FieldEvent_WaitFrames, 30);
  Value1(Object_GetById, 10); // ret
    ands r3, r2
    strb [ret(Object_GetById)+90 +0] = ?   ; r3
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 10, 680, 0x60);
  Call2(Object_SetModeById, 10, 5);
  Call3(ObjectMotion_Launch, 10, 4, 90);
  Value1(Object_GetById, 10); // ret
    ands r3, r2
    strb [ret(Object_GetById)+90 +0] = ?   ; r3
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 10, 664, 0x60);
  Call2(Object_SetModeById, 10, 5);
  Call1(FieldEvent_WaitFrames, 30);
  Value1(Object_GetById, 10); // ret
    ands r3, r2
    strb [ret(Object_GetById)+90 +0] = ?   ; r3
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 10, 680, 0x60);
  Call2(Object_SetModeById, 10, 5);
  Call3(ObjectMotion_Launch, 10, 4, 90);
  Value1(Object_GetById, 10); // ret
    ands r5, r3
    strb [ret(Object_GetById)+90 +0] = ?   ; r5
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 10, 0x2a0, 0x60);
  Call2(Object_SetModeById, 10, 5);
  Call3(ObjectMotion_Launch, 10, 4, 30);
  Call3(ObjectMotion_Launch, 10, 4, 30);
  Call2(Object_SetModeById, 10, 5);
  Call2(Object_SetModeById, 10, 0);
  Call1(Func_020011a8, 11);
  Value2(Func_02001168, 0x10000, 1);
  Value1(Func_02001178, 30);
  Call3(Func_02001108, 20, 0x2a00000, 0x680000);
  Call1(Func_020011a8, 0x90);
  Call2(Object_SetModeById, 20, 2);
  Call1(ObjectMotion_WaitForAnimationChange, 20);
  Call3(Func_02001108, 20, 0, 0);
  Call1(FieldEvent_WaitFrames, 4);
  Call3(Func_02001108, 19, 0x2a00000, 0x680000);
  Call2(Object_SetModeById, 19, 4);
  Call1(ObjectMotion_WaitForAnimationChange, 19);
  Call1(FieldEvent_WaitFrames, 15);
  Call3(Func_02001108, 19, 0, 0);
  Call1(FieldEvent_WaitFrames, 30);
  Value3(Func_02001160, gPartyState.currentOwner, 258, 0);
  Call1(FieldEvent_WaitFrames, 60);
  Call1(Func_020011a8, 10);
  Value3(Func_02001160, 10, 0x100, 60);
  Call2(Func_02001148, 10, 0);
  Value1(Func_020010b8, ldrh(Value1(Unit_Get, ldr(?+4))+0+[(ldr(?+0) << 1)+216]+0));
    ands r2, r3
    cmp ?, 0x20000
    bhi .L_020086c0
  Call4(Func_02000920, ret(Func_020010b8), 0x20000, ?, ?); /* XXX arity */
    b .L_02008748
.L_020086c0:
    cmp ?, 326
    bne .L_020086fc
  Call1(Func_02001138, MsgAreYouPrepared+0+29);
  Call2(Func_02001148, 10, 0);
    cmp Value1(GameFlag_Test, 0x940), 0
    bne .L_020086ea
    b .L_020086ee
.L_020086ea:
.L_020086ee:
  Call1(Func_02001138, ?);
  Call2(Func_02001148, 10, 0);
    b .L_02008748
.L_020086fc:
    cmp ldrb(?+2)+0, 6
    bne .L_0200870a
  Call3(Func_020007b4, ldrb(?+2), ?, ldrb(?+2)+0); /* XXX arity */
    b .L_02008748
.L_0200870a:
    cmp ?, 1
    bne .L_02008714
  Func_02000760(); /* XXX arity */
    b .L_02008748
.L_02008714:
    cmp ?, 0x3000000
    bhi .L_02008744
  Call4(Func_02000838, ?, 0x3000000, ?, ?); /* XXX arity */
    b .L_02008748
.L_02008744:
  Func_02000878(); /* XXX arity */
.L_02008748:
  Call3(Func_02001108, 18, 0, 0);
  Call1(Party_AdjustCoins, -0x14);
.L_0200875a:
	.endif
}
