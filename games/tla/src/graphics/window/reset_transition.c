#include "types.h"

struct ChannelTransition {
  u8 unk00[28];
  u16 transition;
};

void Func_08039754(struct ChannelTransition *channel) {
  channel->transition = 2;
}
