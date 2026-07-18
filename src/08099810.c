typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct Fields_08099810 {
    u8 filler[0x24c];
    s16 value;
};

extern struct Fields_08099810 Data_02000240;

void Func_080041d8(s32, s32);

void Func_08099810(void) {
    if (Data_02000240.value != 0) {
        Func_080041d8(0x08099679, 0xc80);
    }
}
