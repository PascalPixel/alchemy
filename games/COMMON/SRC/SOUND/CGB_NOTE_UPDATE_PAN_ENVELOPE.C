#include "TYPES.H"

struct SoundNote {
    u8 unk00[2];
    u8 out_right;
    u8 out_left;
    u8 unk04[2];
    u8 sustain;
    u8 unk07[3];
    u8 peak;
    u8 unk0b[0x0e];
    u8 sustain_level;
    u8 unk1a;
    u8 pan_bits;
    u8 channel_bits;
};

void CgbNote_UpdatePanEnvelope(struct SoundNote *note)
{
    u32 right = note->out_right;
    u32 left = note->out_left;

    if ((u8)right >= (u8)left) {
        if ((u8)right / 2 >= (u8)left) {
            note->pan_bits = 0x0F;
            goto one_side;
        }
    } else if ((u8)left / 2 >= (u8)right) {
        note->pan_bits = 0xF0;
        goto one_side;
    }
    note->pan_bits = 0xFF;
    note->peak = (u32)(note->out_right + note->out_left) >> 4;
    goto scale;
one_side:
    note->peak = (u32)(note->out_right + note->out_left) >> 4;
    if (note->peak > 15)
        note->peak = 15;
scale:
    note->sustain_level = (note->peak * note->sustain + 15) >> 4;
    note->pan_bits &= note->channel_bits;
}
