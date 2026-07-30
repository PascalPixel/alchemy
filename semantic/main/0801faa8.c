typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

void *Func_08004970(s32);
s32 Func_080056cc(void);
void Func_0801776c(s32, s32);
s32 Func_08005a78(s16, void *);
void Func_080072f0(void *, void *, s32, void *);
s32 Func_08005920(s16, void *);
void Func_08005cf8(void);
void Func_08002df0(void *);

extern s16 Data_02002004;
extern u8 Data_020004e4;
extern u8 Data_02000000;
extern u8 Data_03001388;
extern u8 Value_0000000a;
extern u8 Value_0000000b;

s32 Func_0801faa8(void)
{
    void *buffer;
    s32 return_value;
    s32 result;
    s32 address;
    s16 *data;

    result = 0;
    buffer = Func_08004970(0x1000);
    data = &Data_02002004;
    return_value = *data;
    if (return_value != -1) {
        if (Func_080056cc() != 0) {
            result = 9;
            Func_0801776c((s32)&Value_0000000a, 1);
            goto negate_result;
        }
        if (Func_08005a78(*data, buffer) != 0) {
            Func_0801776c((s32)&Value_0000000b, 1);
            result = -2;
        }
        address = (s32)buffer + (s32)&Data_020004e4;
        Func_080072f0(
            (void *)(address - (s32)&Data_02000000),
            &Data_020004e4,
            16,
            &Data_03001388);
        if (Func_08005920(*data, buffer) != 0) {
            Func_0801776c((s32)&Value_0000000b, 1);
            result = 3;
negate_result:
            result = -result;
        }
        Func_08005cf8();
        Func_08002df0(buffer);
        return_value = result;
    }
    return return_value;
}
