typedef unsigned char u8;
typedef signed int s32;

struct State_0807961c {
    u8 padding[0x1F8];
    u8 active[8];
};

extern struct State_0807961c Data_02000240;
s32 Func_080795fc(void);
void Func_08079358(s32);

s32 Func_0807961c(s32 value) {
    s32 count = Func_080795fc();
    s32 index;

    Func_08079358(value);
    index = 0;
    while (index < count) {
        if (Data_02000240.active[index] == value) {
            return count;
        }
        index++;
    }
    Data_02000240.active[index] = value;
    return count + 1;
}
