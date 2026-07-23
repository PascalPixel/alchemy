typedef unsigned char u8;
typedef signed int s32;

void Func_080fa68c(u8 *);

static __inline__ s32 Masked_080fa514(s32 mask, s32 value)
{
    return mask & value;
}

void Func_080fa514(u8 *arg0)
{
    s32 count = arg0[8];
    u8 *entry = *(u8 **)(arg0 + 44);

    if (count > 0) {
        s32 high_mask = 0x80;

        do {
            register s32 low_mask;
            s32 flags = entry[0];

            if (Masked_080fa514(high_mask, flags) != 0) {
                low_mask = 0x40;

                if (Masked_080fa514(low_mask, flags) != 0) {
                    Func_080fa68c(entry);
                    entry[0] = high_mask;
                    entry[15] = 2;
                    entry[19] = low_mask;
                    entry[25] = 22;
                    entry[36] = 1;
                }
            }
            count--;
            entry += 80;
        } while (count > 0);
    }
}
