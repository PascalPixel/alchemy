#include "types.h"

void Audio_ClearChannelChain(void *);

struct AudioChannelNode {
    u8 flags;
    u8 pad[0x33];
    struct AudioChannelNode *next;
};

struct MusicTrackFinishContext {
    u8 active;
    u8 pad[0x1f];
    struct AudioChannelNode *head;
};

void MusicTrack_Finish(
    void *unused,
    struct MusicTrackFinishContext *context)
{
    struct AudioChannelNode *channel;

    channel = context->head;
    while (channel != 0) {
        u8 flags;

        flags = channel->flags;
        if (flags & 0xc7) {
            flags |= 0x40;
            channel->flags = flags;
        }
        Audio_ClearChannelChain(channel);
        channel = channel->next;
    }
    context->active = 0;
}
