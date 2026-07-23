typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_0800f9f4(s32 arg0) {
    u16 *destination;
    u16 *source;
    s32 count;
    s32 index;
    u32 temp;

    source = (u16 *)0x02010002;
    destination = (u16 *)0x02020000;
    temp = arg0 - 1;
    count = (s32)(temp + (temp >> 31)) >> 1;
    if (arg0 & 1) {
        switch (*(u8 *)0x02010001) {
        case 0: {
            index = 0;
            if (index < count) {
                do {
                    *destination = *source;
                    index++;
                    source++;
                    destination++;
                } while (index < count);
            }
            break;
        }
        case 1: {
            u32 previous;
            u8 *left;
            u8 *right;
            previous = 0;
            left = (u8 *)0x02010002;
            index = 0;
            right = left;
            right += count;
            if (index < count) {
                do {
                    s32 low = *right;
                    s32 high = *left << 8;
                    s32 value = (high | low) ^ previous;
                    index++;
                    *destination = value;
                    right++;
                    left++;
                    destination++;
                    previous = value;
                } while (index < count);
            }
            break;
        }
        case 2: {
            s32 previous = 0;
            if (count > 0) {
                index = count;
                do {
                    s32 value = *source ^ previous;
                    index--;
                    *destination = value;
                    source++;
                    destination++;
                    previous = value;
                } while (index != 0);
            }
            break;
        }
        }
    }
}
