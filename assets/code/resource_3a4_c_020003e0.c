typedef signed int s32;
typedef unsigned char u8;

extern u8 Data_02000240[];
extern u8 Value_00000051;
extern void Func_0200409a(s32, s32);
extern void Func_02004092(s32, s32);

void Func_020003e0(void)
{
    Data_02000240[0x22b] = 3;
    Func_0200409a((s32)&Value_00000051, 99);
    Func_02004092(53, 2);
}
