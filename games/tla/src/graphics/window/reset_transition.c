#include "types.h"

struct ChannelTransition {
    u8 unknown_00[28];
    u16 transition;
};

void Func_08039754(struct ChannelTransition *channel)
{
    channel->transition = 2;
}
