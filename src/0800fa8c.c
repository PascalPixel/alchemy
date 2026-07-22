typedef unsigned int u32;
typedef signed int s32;

extern u32 Data_02010000[];

void Func_0800fa8c(void)
{
    u32 *cursor = Data_02010000;
    u32 remaining = 128 << 7;
    u32 mask = 0xfff;
    s32 replacement = -1;

    do {
        u32 value = *cursor++;
        u32 low = value & mask;

        if (low == mask) {
            if (replacement != (s32)low) {
                replacement++;
            }
            value = value + replacement - low;
            cursor[-1] = value;
        }
        remaining--;
    } while (remaining != 0);
}
