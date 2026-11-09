#include "types.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern const u8 MsgIAmThe[];
extern u8 gMainRoutineWork[];
extern char SceneId_Ov664Room4;

s32 Func_02001870();
void Func_020018e0(s32 a, s32 b);

/// The guardian speaks, then the party leaves for overlay 664's fourth room.
/// Overlay 6af's Func_02000a8c is the same scene for actor 8.
void Func_0200130c(void) {
  s32 leader;

  Func_02001870();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(9, 2);
  Event_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(60);
  Event_Wait(60);
  FieldMessage_ShowCentered((s32)MsgIAmThe, 0, 6);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(60);
  Event_Wait(60);
  leader = ((s32)Actor_GetById(FIELD_AT(gMainRoutineWork, s32, 0x214)));
  if (leader != 0)
    Actor_ResetAndSetPosition(9, FIELD_AT(leader, s16, 10),
                              FIELD_AT(leader, s16, 18));
  Event_Wait(20);
  /* FAKEMATCH: a one-pass block around the first two stores keeps the call's
   * first argument ahead of the scene store's address (alchemy permute);
   * without it the argument follows the store (differ 4). */
  do {
    FIELD_AT(gMainRoutineWork, u16, 0x254) = 0x19cd;
    FIELD_AT(gMainRoutineWork, u8, 0x24b) = 2;
  } while (0);
  FIELD_AT(gMainRoutineWork, u16, 0x1f6) = (s32)&SceneId_Ov664Room4;
  Func_020018e0(0x67, 1);
}
