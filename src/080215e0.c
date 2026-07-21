typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08002dd8(s32);
void Func_080053e8(s32, void *);
extern s32 Data_08031864[];

void Func_080215e0(s32 arg0, s32 arg1) {
    register s32 value;
    void *temp_r6;

    temp_r6 = (void *)Func_080048f4(14, 0x400);
    if ((value = Data_08031864[arg0], arg1 <= 0x5F)) {
        Func_080053e8(value, temp_r6);
        Func_08003fa4(arg1, 0x200, temp_r6);
        Func_08002dd8(14);
    }
}
