#include "TYPES.H"

/* Encode a password: mask every byte with the last, repack the bits as six-bit
 * symbols with a checksum symbol after every nine, then offset each symbol by
 * its position. Returns the symbol count. */
s32 Clear_EncodePassword(u8 *data, s32 size, u8 *out)
{
    u8 *src;
    s32 key;
    s32 count;
    s32 i;
    s32 j;
    s32 bit;
    s32 index;
    s32 value;
    s32 sum;
    s32 group;
    s32 b;

    key = data[size - 1];
    count = 0;
    for (i = 0; i != size - 1; i++)
        data[i] ^= key;
    bit = 0;
    index = 0;
    sum = 0;
    group = 0;
    do {
        value = 0;
        /* FAKEMATCH: the source address is added as integers, index first,
         * for the reference operand order. */
        for (j = 0, src = (u8 *)(index + (s32)data); j != 6; j++) {
            b = (*src >> (7 - bit)) & 1;
            bit++;
            if (bit == 8) {
                bit = 0;
                src++;
                index++;
            }
            value |= b << (5 - j);
            if (index == size)
                break;
        }
        group++;
        out[count++] = value;
        sum += value;
        if (group == 9) {
            out[count++] = sum & 63;
            sum = 0;
            group = 0;
        }
    } while (index != size);
    for (i = 0; i != count; i++)
        out[i] = (out[i] + i) & 63;
    return count;
}
