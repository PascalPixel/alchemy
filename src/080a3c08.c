typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 *Data_03001f2c;
extern s32 Data_03001e40;
s32 Func_08077218(u16, s32);
void Func_08009020(s32, s32);

void Func_080a3c08(void) {
    u8 *state;
    s8 i;

    state = Data_03001f2c;
    if ((Data_03001e40 & 31) == 0 && state[0x219] != 0) {
        i = 0;
        do {
            s32 halfwordOffset = i * 2 + 520;

            if (Func_08077218(*(u16 *)(state + halfwordOffset),
                              *(u16 *)(state + 376) & 0x1FF) != 0) {
                s32 objectOffset = i * 4 + 276;
                s32 object = *(s32 *)(state + objectOffset);
                Func_08009020(object, 3);
            } else {
                s32 objectOffset = i * 4 + 276;
                s32 object = *(s32 *)(state + objectOffset);
                Func_08009020(object, 1);
            }
            i++;
        } while (i < state[0x219]);
    }
}
