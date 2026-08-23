#include "audio_engine_symbols.h"
#include "types.h"

struct CgbChannel
{
    u8 status;
    u8 type;
    u8 right_volume;
    u8 left_volume;
    u8 attack;
    u8 decay;
    u8 sustain;
    u8 release;
    u8 key;
    u8 envelope_volume;
    u8 envelope_goal;
    u8 envelope_counter;
    u8 pseudo_echo_volume;
    u8 pseudo_echo_length;
    u8 unknown0e[2];
    u8 gate_time;
    u8 midi_key;
    u8 velocity;
    u8 priority;
    u8 rhythm_pan;
    u8 unknown15[3];
    u8 unknown18;
    s8 sustain_goal;
    u8 unknown1a;
    u8 pan;
    u8 pan_mask;
    u8 modify;
    u8 length;
    u8 sweep;
    u32 frequency;
    const u8 *wave;
    const u8 *current_wave;
    struct MusicTrackState *track;
    struct CgbChannel *previous;
    struct CgbChannel *next;
    u8 unknown38[8];
};

void CgbChannel_UpdatePanEnvelope(struct CgbChannel *channel)
{
    u32 right = channel->right_volume;
    u32 left = channel->left_volume;

    if ((u8)right >= (u8)left)
    {
        if (((u8)right >> 1) >= (u8)left)
        {
            channel->pan = 0x0F;
            goto clamp;
        }
    }
    else
    {
        if (((u8)left >> 1) >= (u8)right)
        {
            channel->pan = 0xF0;
            goto clamp;
        }
    }

    channel->pan = 0xFF;
    channel->envelope_goal =
        (u32)(channel->right_volume + channel->left_volume) >> 4;
    goto done;

clamp:
    channel->envelope_goal =
        (u32)(channel->right_volume + channel->left_volume) >> 4;
    if (channel->envelope_goal > 15)
        channel->envelope_goal = 15;

done:
    channel->sustain_goal =
        (channel->envelope_goal * channel->sustain + 15) >> 4;
    channel->pan &= channel->pan_mask;
}
