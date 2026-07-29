typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

extern u8 *Data_03001f2c;
void Func_080ae7fc(s8 *);

s32 Func_080ae778(s32 decrease, s32 increase) {
    s8 values[16];
    u8 *state;
    volatile u8 *countPointer;
    s32 countAddress;
    u8 count;
    u8 i;
    u8 j;
    u8 innerCount;
    s32 valid;

    state = Data_03001f2c;
    Func_080ae7fc(values);
    values[decrease]--;
    values[increase]++;

    countAddress = (s32)state + 0x219;
    countPointer = (volatile u8 *)countAddress;
    valid = 1;
    count = *countPointer;
    i = 0;
    if (i < count) {
        do {
            j = i;
            if (i < count) {
                innerCount = *countPointer;
                do {
                    j++;
                    if (j >= innerCount)
                        break;
                    if ((u32)(values[i] - values[j] + 1) > 2) {
                        valid = 0;
                        break;
                    }
                } while (1);
            }
            i++;
        } while (i < count);
    }
    return valid;
}
