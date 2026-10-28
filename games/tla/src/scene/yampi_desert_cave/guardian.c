#include "types.h"
#include "call.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern const u8 MsgIAmThe[];
extern u8 gAdventure[];
extern char SceneId_Ov664Room4;

s32 Func_02001870();
s32 Func_02001930();
s32 Actor_StartRepeatedMotion();
s32 FieldEvent_Wait();
s32 Func_020018f8();
s32 Func_020018f0();
s32 Func_02001900();
s32 Func_02001860();
s32 Actor_GetById();
s32 Actor_ResetAndSetPosition();
void Func_020018e0(s32 a, s32 b);

/// The guardian speaks, then the party leaves for overlay 664's fourth room.
/// Overlay 6af's Func_02000a8c is the same scene for actor 8.
void Func_0200130c(void) {
  s32 leader;

  Func_02001870();
  Call1(Func_02001930, 0);
  Call2(Actor_StartRepeatedMotion, 9, 2);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_020018f8, 0x10000, 0);
  Call2(Func_020018f0, 0x10003, 1);
  Call1(Func_02001900, 60);
  Call1(FieldEvent_Wait, 60);
  Call3(Func_02001860, (s32)MsgIAmThe, 0, 6);
  Call2(Func_020018f0, 0x10000, 0);
  Call1(Func_02001900, 60);
  Call1(FieldEvent_Wait, 60);
  leader = Value1(Actor_GetById, FIELD_AT(gAdventure, s32, 0x214));
  if (leader != 0)
    Call3(Actor_ResetAndSetPosition, 9, FIELD_AT(leader, s16, 10),
          FIELD_AT(leader, s16, 18));
  Call1(FieldEvent_Wait, 20);
  /* FAKEMATCH: a one-pass block around the first two stores keeps the call's
   * first argument ahead of the scene store's address (alchemy permute);
   * without it the argument follows the store (differ 4). */
  do {
    FIELD_AT(gAdventure, u16, 0x254) = 0x19cd;
    FIELD_AT(gAdventure, u8, 0x24b) = 2;
  } while (0);
  FIELD_AT(gAdventure, u16, 0x1f6) = (s32)&SceneId_Ov664Room4;
  Func_020018e0(0x67, 1);
}
