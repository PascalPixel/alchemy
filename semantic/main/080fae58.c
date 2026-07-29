#include "types.h"

struct CgbChannel_080fae58 {
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
    u8 unknown_0e[2];
    u8 gate_time;
    u8 midi_key;
    u8 velocity;
    u8 priority;
    u8 rhythm_pan;
    u8 unknown_15[3];
    u8 unknown_18;
    s8 sustain_goal;
    u8 control;
    u8 pan;
    u8 pan_mask;
    u8 modify;
    u8 length;
    u8 sweep;
    u32 frequency;
    const u8 *wave;
    const u8 *current_wave;
    void *track;
    struct CgbChannel_080fae58 *previous;
    struct CgbChannel_080fae58 *next;
    u8 unknown_38[8];
};

struct SoundInfo_080fae58 {
    u8 unknown_00[10];
    u8 envelope_clock;
    u8 unknown_0b[0x11];
    struct CgbChannel_080fae58 *cgb_channels;
};

extern struct SoundInfo_080fae58 *Data_03007ff0;

void Func_080fada0(u8 channel);
void Func_080fadf0(struct CgbChannel_080fae58 *channel);

#define CHANNEL_ON       0x80
#define CHANNEL_STOP     0x40
#define CHANNEL_ECHO     0x04
#define ENVELOPE_MASK    0x03
#define ENVELOPE_ATTACK  0x03
#define ENVELOPE_DECAY   0x02
#define ENVELOPE_SUSTAIN 0x01
#define ENVELOPE_RELEASE 0x00
#define MODIFY_VOLUME    0x01
#define MODIFY_PITCH     0x02

/*
 * Advance the four programmable-sound channels and publish their envelope,
 * pitch, pan, and trigger state to the GBA sound registers.
 *
 * The channel layout and high-level control flow were adapted from the public
 * pret/pokeemerald src/m4a.c:CgbSound reconstruction at commit
 * 83df84e40623b79281f2397faa611cbf044170bd; all addresses, branches, and
 * field offsets were then verified against this ROM owner.
 */
void Func_080fae58(void)
{
    struct SoundInfo_080fae58 *sound = Data_03007ff0;
    struct CgbChannel_080fae58 *channel;
    s32 channel_number;

    if (sound->envelope_clock != 0)
        sound->envelope_clock--;
    else
        sound->envelope_clock = 14;

    channel = sound->cgb_channels;
    for (channel_number = 1; channel_number <= 4;
         channel_number++, channel++) {
        volatile u8 *reg0;
        volatile u8 *reg1;
        volatile u8 *reg2;
        volatile u8 *reg3;
        volatile u8 *reg4;
        s32 previous_clock;
        u8 envelope_control;

        if ((channel->status & 0xc7) == 0)
            continue;

        switch (channel_number) {
        case 1:
            reg0 = (volatile u8 *)0x04000060;
            reg1 = (volatile u8 *)0x04000062;
            reg2 = (volatile u8 *)0x04000063;
            reg3 = (volatile u8 *)0x04000064;
            reg4 = (volatile u8 *)0x04000065;
            break;
        case 2:
            reg0 = (volatile u8 *)0x04000061;
            reg1 = (volatile u8 *)0x04000068;
            reg2 = (volatile u8 *)0x04000069;
            reg3 = (volatile u8 *)0x0400006c;
            reg4 = (volatile u8 *)0x0400006d;
            break;
        case 3:
            reg0 = (volatile u8 *)0x04000070;
            reg1 = (volatile u8 *)0x04000072;
            reg2 = (volatile u8 *)0x04000073;
            reg3 = (volatile u8 *)0x04000074;
            reg4 = (volatile u8 *)0x04000075;
            break;
        default:
            reg0 = (volatile u8 *)0x04000071;
            reg1 = (volatile u8 *)0x04000078;
            reg2 = (volatile u8 *)0x04000079;
            reg3 = (volatile u8 *)0x0400007c;
            reg4 = (volatile u8 *)0x0400007d;
            break;
        }

        previous_clock = sound->envelope_clock;
        envelope_control = *reg2;

        if ((channel->status & CHANNEL_ON) != 0) {
            if ((channel->status & CHANNEL_STOP) != 0)
                goto oscillator_off;

            channel->status = ENVELOPE_ATTACK;
            channel->modify = MODIFY_PITCH | MODIFY_VOLUME;
            Func_080fadf0(channel);

            switch (channel_number) {
            case 1:
                *reg0 = channel->sweep;
                /* fall through */
            case 2:
                *reg1 =
                    channel->length + ((u32)channel->wave << 6);
                goto initialize_envelope;
            case 3:
                if (channel->wave != channel->current_wave) {
                    const u32 *source = (const u32 *)channel->wave;
                    volatile u32 *wave_ram =
                        (volatile u32 *)0x04000090;

                    *reg0 = 0x40;
                    wave_ram[0] = source[0];
                    wave_ram[1] = source[1];
                    wave_ram[2] = source[2];
                    wave_ram[3] = source[3];
                    channel->current_wave = channel->wave;
                }
                *reg0 = 0;
                *reg1 = channel->length;
                if (channel->length != 0)
                    channel->control = 0xc0;
                else
                    channel->control = 0x80;
                break;
            default:
                *reg1 = channel->length;
                *reg3 = (u32)channel->wave << 3;
initialize_envelope:
                envelope_control = channel->attack + 8;
                if (channel->length != 0)
                    channel->control = 0x40;
                else
                    channel->control = 0;
                break;
            }

            channel->envelope_counter = channel->attack;
            if (channel->attack != 0) {
                channel->envelope_volume = 0;
                goto envelope_step_complete;
            }
            goto envelope_decay_start;
        }

        if ((channel->status & CHANNEL_ECHO) != 0) {
            channel->pseudo_echo_length--;
            if ((s8)channel->pseudo_echo_length <= 0)
                goto oscillator_off;
            goto envelope_complete;
        }

        if ((channel->status & CHANNEL_STOP) != 0 &&
            (channel->status & ENVELOPE_MASK) != 0) {
            channel->status &= (u8)~ENVELOPE_MASK;
            channel->envelope_counter = channel->release;
            if (channel->release != 0) {
                channel->modify |= MODIFY_VOLUME;
                if (channel_number != 3)
                    envelope_control = channel->release;
                goto envelope_step_complete;
            }
            goto envelope_echo_start;
        }

envelope_step_repeat:
        if (channel->envelope_counter == 0) {
            if (channel_number == 3)
                channel->modify |= MODIFY_VOLUME;

            Func_080fadf0(channel);
            switch (channel->status & ENVELOPE_MASK) {
            case ENVELOPE_RELEASE:
                channel->envelope_volume--;
                if ((s8)channel->envelope_volume <= 0) {
envelope_echo_start:
                    channel->envelope_volume =
                        (channel->envelope_goal *
                             channel->pseudo_echo_volume +
                         0xff) >>
                        8;
                    if (channel->envelope_volume == 0)
                        goto oscillator_off;

                    channel->status |= CHANNEL_ECHO;
                    channel->modify |= MODIFY_VOLUME;
                    if (channel_number != 3)
                        envelope_control = 8;
                    goto envelope_complete;
                }
                channel->envelope_counter = channel->release;
                break;

            case ENVELOPE_SUSTAIN:
envelope_sustain:
                channel->envelope_volume = channel->sustain_goal;
                channel->envelope_counter = 7;
                break;

            case ENVELOPE_DECAY:
                channel->envelope_volume--;
                if ((s8)channel->envelope_volume <=
                    channel->sustain_goal) {
envelope_sustain_start:
                    if (channel->sustain == 0) {
                        channel->status &= (u8)~ENVELOPE_MASK;
                        goto envelope_echo_start;
                    }

                    channel->status--;
                    channel->modify |= MODIFY_VOLUME;
                    if (channel_number != 3)
                        envelope_control = 8;
                    goto envelope_sustain;
                }
                channel->envelope_counter = channel->decay;
                break;

            default:
                channel->envelope_volume++;
                if (channel->envelope_volume >=
                    channel->envelope_goal) {
envelope_decay_start:
                    channel->status--;
                    channel->envelope_counter = channel->decay;
                    if (channel->envelope_counter != 0) {
                        channel->modify |= MODIFY_VOLUME;
                        channel->envelope_volume =
                            channel->envelope_goal;
                        if (channel_number != 3)
                            envelope_control =
                                channel->decay;
                    } else {
                        goto envelope_sustain_start;
                    }
                } else {
                    channel->envelope_counter =
                        channel->attack;
                }
                break;
            }
        }

envelope_step_complete:
        channel->envelope_counter--;
        if (previous_clock == 0) {
            previous_clock--;
            goto envelope_step_repeat;
        }

envelope_complete:
        if ((channel->modify & MODIFY_PITCH) != 0) {
            if (channel_number < 4 &&
                (channel->type & 8) != 0) {
                u8 bias = *(volatile u8 *)0x04000089;

                if (bias <= 0x3f)
                    channel->frequency =
                        (channel->frequency + 2) & 0x7fc;
                else if (bias <= 0x7f)
                    channel->frequency =
                        (channel->frequency + 1) & 0x7fe;
            }

            if (channel_number != 4)
                *reg3 = (u8)channel->frequency;
            else
                *reg3 = (*reg3 & 8) | (u8)channel->frequency;

            channel->control =
                (channel->control & 0xc0) +
                (u8)(channel->frequency >> 8);
            *reg4 = channel->control;
        }

        if ((channel->modify & MODIFY_VOLUME) != 0) {
            volatile u8 *pan = (volatile u8 *)0x04000081;

            *pan = (*pan & (u8)~channel->pan_mask) |
                   channel->pan;
            if (channel_number == 3) {
                *reg2 =
                    ((const u8 *)0x080fba04)
                        [channel->envelope_volume];
                if ((channel->control & 0x80) != 0) {
                    *reg0 = 0x80;
                    *reg4 = channel->control;
                    channel->control &= 0x7f;
                }
            } else {
                *reg2 =
                    (channel->envelope_volume << 4) |
                    (envelope_control & 0x0f);
                *reg4 = channel->control | 0x80;
                if (channel_number == 1 &&
                    (*reg0 & 8) == 0)
                    *reg4 = channel->control | 0x80;
            }
        }

        channel->modify = 0;
        continue;

oscillator_off:
        Func_080fada0((u8)channel_number);
        channel->status = 0;
        channel->modify = 0;
    }
}
