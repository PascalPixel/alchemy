typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;

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

void Func_080fadf0(struct CgbChannel *chan)
{
    u32 r = chan->right_volume;
    u32 l = chan->left_volume;

    if ((u8) r >= (u8) l)
    {
        if (((u8) r >> 1) >= (u8) l)
        {
            chan->pan = 0x0F;
            goto clamp;
        }
    }
    else
    {
        if (((u8) l >> 1) >= (u8) r)
        {
            chan->pan = 0xF0;
            goto clamp;
        }
    }

    chan->pan = 0xFF;
    chan->envelope_goal =
        (u32) (chan->right_volume + chan->left_volume) >> 4;
    goto done;

clamp:
    chan->envelope_goal =
        (u32) (chan->right_volume + chan->left_volume) >> 4;
    if (chan->envelope_goal > 15)
        chan->envelope_goal = 15;

done:
    chan->sustain_goal =
        (chan->envelope_goal * chan->sustain + 15) >> 4;
    chan->pan &= chan->pan_mask;
}
