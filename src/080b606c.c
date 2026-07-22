typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/* Preserve independent live ranges while producing constants zero and three. */
#define LIVE_CONSTANT(seed, offset) (((u32)(seed) | ~(u32)(seed)) + (offset))

u8 *Func_080b606c(u32 unused0, u32 unused1, u16 *source)
{
    u8 buffer[5];
    u32 replacementValue = '_';
    u8 *base = (u8 *)((u32)buffer + LIVE_CONSTANT(source, 1));
    u8 *replacement = base;
    u8 *destination = base;
    s32 remaining = LIVE_CONSTANT(source, 4);

    do {
        u32 value = *source;

        *destination = value;
        source++;
        destination++;
        if ((u8)value == 0) {
            *replacement = replacementValue;
        }
        replacement++;
        remaining--;
    } while (remaining >= 0);
    base[4] = 0;
    return destination;
}
