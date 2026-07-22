typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

extern u8 Data_03001810[];

/* Keep the 128-quarter scan bound live independently from the run counter. */
#define SCAN_QUARTERS(seed) (((u32)(seed) | ~(u32)(seed)) + 129)

s32 Func_08003ed4(void)
{
    u8 *cursor = Data_03001810;
    s32 current = 0;
    s32 remaining = SCAN_QUARTERS(cursor);
    s32 longest = 0;

    remaining <<= 2;
    do {
        if (*cursor++ != 0xff) {
            current = 0;
        } else {
            current++;
            if (longest < current) {
                longest = current;
            }
        }
        remaining--;
    } while (remaining != 0);
    return longest;
}
