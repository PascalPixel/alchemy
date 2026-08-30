#include "types.h"

void UiWork_ResetChannelTransition(void *work)
{
  int transition_mode;
  int reset_mode;
  u8 *channel_transition;
  channel_transition = ((u8 *) work) + 0x1C;
  reset_mode = 2;
  transition_mode = reset_mode;
  *((s16 *) channel_transition) = transition_mode;
}
