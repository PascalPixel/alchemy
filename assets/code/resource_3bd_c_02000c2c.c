typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern s16 Data_02000240;
extern u8 Value_00000097;

extern s32 Func_020048c0(s32);
extern void Func_02001770(void);
extern void Func_020049ba(s32, s32);
extern void Func_020049c2(s32, s32);
extern void Func_020049ca(s32, s32);
extern void Func_020049d2(s32, s32);
extern void Func_020049da(s32, s32);

void Func_02000c2c(void)
{
    u8 *ptr = *(u8 **)0x03001f30;
    s16 *table;

    if (Func_020048c0(0x200) != 0) {
        Func_02001770();
        ptr[0x34] = 1;
    }
    table = &Data_02000240;
    if (table[0xe0] == (s32)&Value_00000097) {
        Func_020049ba(16, 6);
        Func_020049c2(17, 6);
        Func_020049ca(18, 6);
        Func_020049d2(19, 6);
        Func_020049da(20, 6);
    }
}
