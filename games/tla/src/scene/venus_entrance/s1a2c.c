#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_020028b4();
s32 Func_020029cc();
s32 Func_0200293c();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_ArmCallback();
s32 FieldEvent_WaitFrames();
s32 Func_0200294c();
s32 Object_SetModeById();
s32 Func_02002964();
s32 ObjectMotion_Launch();
s32 Motion_SetModeAndWaitAnimation();
s32 ObjectMotion_SetSpeedParameters();
s32 ObjectMotion_CommitPositionAndActivate();
s32 ObjectMotion_OffsetPositionAndReset();
s32 Party_SetFields1eeAnd1f0();
s32 Party_SetFields1f2And1f4();
s32 Func_020029a4();
void Func_020028bc(void);
extern u8 MsgWhyDoI[];
extern char SceneId_VenusEntrance;
extern u8 gPartyState[];

void Func_02001a2c(void) {
  Func_020028b4();
  Call1(Func_020029cc, 0);
  Call1(Func_0200293c, (s32)MsgWhyDoI);
  Call3(ObjectMotion_SetPositionAndReset, 5, 0x48, 0x10a);
  Call3(ObjectMotion_ArmCallback, 5, 0x8000, 0);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(Func_0200294c, 0x16, 0, 0xa);
  Call2(Object_SetModeById, 0x16, 1);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(Func_02002964, 0x16, 0x101, 0x32);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(ObjectMotion_ArmCallback, 0x16, 0, 0);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call3(ObjectMotion_Launch, 0x16, 6, 0xf);
  Call3(ObjectMotion_Launch, 0x16, 6, 0x23);
  Call3(ObjectMotion_ArmCallback, 0x16, 0xb000, 0);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call2(Object_SetModeById, 0x16, 0xc);
  Call3(Func_0200294c, 0x16, 0, 0);
  Call2(Object_SetModeById, 0x16, 1);
  Func_02002964(0x16, 0x100, 0x32);
  Call3(ObjectMotion_ArmCallback, 0x16, 0, 0);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call3(Func_0200294c, 0x16, 0, 0xa);
  Call2(Motion_SetModeAndWaitAnimation, 0x16, 3);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call3(ObjectMotion_SetSpeedParameters, 0x16, 0x10000, 0x8000);
  Call3(ObjectMotion_CommitPositionAndActivate, 0x16, 0x10, 0);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(Func_0200294c, 0x16, 0, 0xa);
  Call3(ObjectMotion_SetSpeedParameters, 0x16, 0x20000, 0x10000);
  Call3(ObjectMotion_Launch, 0x16, 6, 0xf);
  Call3(ObjectMotion_Launch, 0x16, 6, 0x19);
  Call3(ObjectMotion_Launch, 0x16, 6, 0);
  Call3(ObjectMotion_OffsetPositionAndReset, 0x16, 0x1a, 0);
  Call1(FieldEvent_WaitFrames, 8);
  Call2(Object_SetModeById, 0x16, 1);
  Call2(Party_SetFields1eeAnd1f0, (s32)&SceneId_VenusEntrance, 0x63);
  Call2(Party_SetFields1f2And1f4, (s32)&SceneId_VenusEntrance, 0x62);
  {
    u8 *party = (u8 *)gPartyState;
    /* FAKEMATCH: the native schedule loads the party state address before it
     * builds the field offset. */
    __asm__ volatile("" : "+l"(party));
    FIELD_AT(party, u8, 587) = 2;
  }
  Call2(Func_020029a4, 9, 0);
  Func_020028bc();
}
