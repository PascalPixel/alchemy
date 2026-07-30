typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u16 Data_02000240[];
extern volatile u16 Data_02000462;

void Func_0801c8a0(s32 *first, s32 *second, u16 (*table)[2]) {
    u32 color;
    u32 low;
    u32 high;
    s32 index;
    u16 (*cursor)[2];

    *first = 0;
    *second = 0;

    color = Data_02000240[272];
    low = color & 0x3FF;
    high = color >> 10;
    index = 0;
    cursor = table;
    do {
        if ((*cursor)[1] == low &&
            (*cursor)[0] == high) {
            *first = index;
            break;
        }
        index++;
        cursor++;
    } while (index <= 447);

    index = 0;
    cursor = table;
    do {
        u32 current = Data_02000462;
        if ((*cursor)[1] == (current & 0x3FF) &&
            (*cursor)[0] == (current >> 10)) {
            *second = index;
            break;
        }
        index++;
        cursor++;
    } while (index <= 447);
}
