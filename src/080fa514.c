typedef unsigned char u8;
typedef signed int s32;

void Func_080fa68c(u8 *);

static __inline__ s32 masked_080fa514(s32 mask, s32 value)
{
    return mask & value;
}

void Func_080fa514(u8 *state)
{
    s32 count = state[8];
    u8 *track = *(u8 **)(state + 44);

    if (count > 0) {
        s32 high_mask = 0x80;

        do {
            register s32 low_mask;
            s32 flags = track[0];

            if (masked_080fa514(high_mask, flags) != 0) {
                low_mask = 0x40;

                if (masked_080fa514(low_mask, flags) != 0) {
                    Func_080fa68c(track);
                    track[0] = high_mask;
                    track[15] = 2;
                    track[19] = low_mask;
                    track[25] = 22;
                    track[36] = 1;
                }
            }
            count--;
            track += 80;
        } while (count > 0);
    }
}
