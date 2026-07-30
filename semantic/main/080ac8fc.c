typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;

struct BitsetState_080ac8fc {
    u8 unknown_000[0xf8];
    u32 secondary[4];
    u32 primary[4];
};

extern struct BitsetState_080ac8fc *Func_08077008(s32 owner);

s32 Func_080ac8fc(u16 *output, s32 owner, s32 group_selector)
{
    struct BitsetState_080ac8fc *state = Func_08077008(owner);
    s32 count = 0;
    s32 bit;

    if (group_selector == -1) {
        u32 owner_bits = (u32)owner << 8;
        s32 group;

        for (group = 0; group < 4; group++) {
            u32 group_bits = (u32)group << 5;

            for (bit = 0; bit < 20; bit++) {
                u32 mask = 1U << bit;
                u32 encoded;

                if (state->primary[group] & mask) {
                    encoded = owner_bits | group_bits | (u32)bit | 0x8000;
                } else if (state->secondary[group] & mask) {
                    encoded = owner_bits | group_bits | (u32)bit;
                } else {
                    continue;
                }

                output[count++] = (u16)encoded;
            }
        }
    } else {
        u32 group_bits = (u32)group_selector << 5;

        for (bit = 0; bit < 20; bit++) {
            u32 mask = 1U << bit;
            u32 encoded;

            if (state->primary[group_selector] & mask) {
                encoded = group_bits | (u32)bit | 0x8000;
            } else if (state->secondary[group_selector] & mask) {
                encoded = group_bits | (u32)bit;
            } else {
                continue;
            }

            output[count++] = (u16)encoded;
        }
    }

    return count;
}
