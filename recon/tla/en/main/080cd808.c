// Draft, differ 22 of 106 (59 before the gotos below), from the listing: the
// search command. Collision codes 242 to 247 show their pair of messages from
// Data_080eff22; otherwise the search event (kind 3) of the cell runs: its
// "checked the ..." message, then its message or callback, or the matching
// "found nothing" message. The messages are linker symbols the .po files do
// not name yet: MsgSearchStart is message 3524 in English (3985 in German,
// Spanish, French and Italian, 3525 in Japanese) and the base of the "X
// checked the Y..." lines, MsgSearchEmpty 3567 (4028) the base of the
// "found nothing" lines, MsgButFoundNothing 3615 (4076). The native lays the
// callback out before the message, both after the two tests, which the
// gotos give (as if and else, 51; the tests the other way round, 26).
// Left: the id and its index (id - 242) take r5 and r6 in the native, r1
// and r5 here, and the second message sum is formed in r0 there (adds r0,
// r5, r0) and in r5 here, then copied; and the native loads the event's
// target halfword before the kind's mask. Tried: the index inline in the
// test or the load, the message as u8 or u32, the sums the other way round
// (all 22).
#include "types.h"
#include "adventure.h"

struct Trigger {
  s32 flags;
  u8 target;
  u8 unk05;
  s16 condition;
  u32 result;
};

extern const u8 Data_080eff22[];
extern char MsgSearchStart[];
extern char MsgSearchEmpty[];
extern char MsgButFoundNothing[];
void Leader_StopMoving(void);
void FieldMessage_ShowFar(s32 message, s32 flags);
struct Trigger *Func_080ccd78(s32 kind, s32 target);
s32 GameFlag_IsConditionActive(s32 condition);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 GameFlag_Test(s32 flag);

s32 Func_080cd808(s32 id) {
  struct Trigger *event;
  s32 index;
  s32 kind;
  s32 condition;
  u32 result;

  index = id - 242;
  if ((u32)index <= 5) {
    s32 message;

    Leader_StopMoving();
    message = Data_080eff22[index];
    FieldMessage_ShowFar(message + (s32)MsgSearchStart, 1);
    FieldMessage_ShowFar(message + (s32)MsgSearchEmpty, 1);
    return 0;
  }
  event = Func_080ccd78(3, id);
  if (event != NULL) {
    condition = event->condition;
    kind = (event->flags >> 4) & 31;
    if (!(*(u16 *)&event->target & 0x400) && kind != 0) {
      Leader_StopMoving();
      FieldMessage_ShowFar(kind + (s32)MsgSearchStart, 1);
      GameFlag_SetBit(0x142);
    } else {
      GameFlag_ClearBit(0x142);
    }
    result = event->result;
    if ((result & 0xf000000) == 0) {
      if ((result & 0xfff00000) == 0x400000)
        goto message;
      goto done;
    }
    if (GameFlag_IsConditionActive(condition))
      ((void (*)(s32))event->result)(gAdventure.currentOwner);
    if (GameFlag_Test(0x142))
      FieldMessage_ShowFar(kind + (s32)MsgSearchEmpty, 1);
    goto done;
  message:
    if (GameFlag_IsConditionActive(condition))
      FieldMessage_ShowFar((u16)event->result, 1);
    else
      FieldMessage_ShowFar((s32)MsgButFoundNothing, 1);
  } else {
    FieldMessage_ShowFar((s32)MsgSearchStart + 5, 1);
    FieldMessage_ShowFar((s32)MsgSearchEmpty + 5, 1);
  }
done:
  GameFlag_ClearBit(0x142);
  return 0;
}
