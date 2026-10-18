// Draft, differ 75 of 106: the field inspect handler. Ids 242 to 247 show a
// fixed pair of messages; any other id looks up its group 3 descriptor, shows
// its "checked" message and runs its handler (a ROM address) or shows its
// message (0x004xxxxx) when its condition holds. No ☀️ twin found. Left:
// register choice (the native keeps id in r5 and the index in r6, then the
// descriptor in r5 and the message index in r6; this draft swaps them) and
// block order: the native tests 0xf000000, then 0xfff00000 == 0x400000
// inline before the handler path, with the message path after it; written
// as if / else if (here) the second test follows the handler path, and the
// nested if-not form differs 81. The packed word is signed (asrs).
#include "types.h"
#include "event_work.h"
#include "flag_call.h"
#include "party_state.h"

void *Func_080ccd78(s32 group, s32 id);
s32 GameFlag_IsConditionActive(s32 condition);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 flags);

extern const u8 Data_080eff22[];

// Unnamed messages, given draft names: the "checked the ..." openers from
// 3524, their results from 3567, and the plain search of the ground.
extern char MsgCheckedBase[];
extern char MsgCheckResultBase[];
extern char MsgCheckedGround[];
extern char MsgFoundNothingOnGround[];
extern char MsgFoundNothing[];

struct InspectDescriptor {
  s32 packed;
  u16 flags;
  s16 condition;
  s32 handler;
};

#define FLAG_INSPECT_SHOWN 322

s32 Func_080cd808(s32 id) {
  struct InspectDescriptor *desc;
  s32 message;
  s32 condition;
  u32 handler;

  message = id - 242;
  if ((u32)message <= 5) {
    s32 index;

    EventRuntime_PrepareCurrentObject();
    index = Data_080eff22[message];
    UiText_ShowPositionedMessageAndWaitFar((s32)MsgCheckedBase + index, 1);
    UiText_ShowPositionedMessageAndWaitFar((s32)MsgCheckResultBase + index, 1);
    return 0;
  }
  desc = Func_080ccd78(3, id);
  if (desc != NULL) {
    condition = desc->condition;
    message = (desc->packed >> 4) & 31;
    if (!(desc->flags & 0x400) && message != 0) {
      EventRuntime_PrepareCurrentObject();
      UiText_ShowPositionedMessageAndWaitFar((s32)MsgCheckedBase + message, 1);
      GameFlag_SetBit(FLAG_INSPECT_SHOWN);
    } else {
      GameFlag_ClearBit(FLAG_INSPECT_SHOWN);
    }
    handler = desc->handler;
    if (handler & 0xf000000) {
      if (GameFlag_IsConditionActive(condition))
        ((void (*)(s32))desc->handler)(gPartyState.currentOwner);
      if (GameFlag_Test(FLAG_INSPECT_SHOWN))
        UiText_ShowPositionedMessageAndWaitFar(
            (s32)MsgCheckResultBase + message, 1);
    } else if ((handler & 0xfff00000) == 0x400000) {
      UiText_ShowPositionedMessageAndWaitFar(
          GameFlag_IsConditionActive(condition) ? (u16)desc->handler
                                                : (s32)MsgFoundNothing,
          1);
    }
  } else {
    UiText_ShowPositionedMessageAndWaitFar((s32)MsgCheckedGround, 1);
    UiText_ShowPositionedMessageAndWaitFar((s32)MsgFoundNothingOnGround, 1);
  }
  GameFlag_ClearBit(FLAG_INSPECT_SHOWN);
  return 0;
}
