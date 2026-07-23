typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

u8 *Func_08077008(s32);
s32 Func_08077228(s32, s32);
s32 Func_080b6e7c(s32);
extern u16 Data_080c2a1c[];
extern u16 Data_080c2a2a[];
extern u16 Data_080c2a38[];
extern u16 Data_080c2a46[];
extern u16 Data_080c2a54[];

s32 Func_080b6eb4(s32 id) {
    u8 *state;
    s32 entry;
    s32 result;

    state = Func_08077008(id);
    entry = Func_08077228(id, 1);
    result = 0;
    if (entry >= 0) {
        s32 entryOffset;
        s32 selection;

        entryOffset = entry * 2 + 216;
        selection = Func_080b6e7c(*(u16 *)(state + entryOffset) & 0x1FF);
        switch (state[296]) {
        case 0:
            result = Data_080c2a1c[selection];
            break;
        case 1:
            result = Data_080c2a2a[selection];
            break;
        case 2:
            result = Data_080c2a38[selection];
            break;
        case 3:
            result = Data_080c2a46[selection];
            break;
        case 4:
            break;
        case 5:
            result = Data_080c2a54[selection];
            break;
        }
    }
    return result;
}
