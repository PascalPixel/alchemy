typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
s32 Func_0808a5d0(s16, s16);
void Func_08016478(void *);
void Func_0801e9a0(s32, s32, s32, s32, s32);
void Func_0801e858(u8 *, s32, s32, s32);
void Func_0801e74c(s32, void *, s32, s32);
extern u8 Value_0000099b[];
extern u8 Value_00000a07[];
extern u8 Data_08037428[];
void Func_08028ef0(void *arg0, s16 arg1, s16 *arg2) {
    s16 temp_r5 = arg1;
    s32 temp_sl = Func_0808a5d0(temp_r5, *arg2) + (s32)Value_0000099b;
    Func_08016478(arg0);
    Func_0801e9a0(temp_r5, 3, (s32) arg0, 0, 14);
    Func_0801e9a0(*arg2, 3, (s32) arg0, 0x52, 14);
    Func_0801e858(Data_08037428, (s32) arg0, 0x4A, 0);
    Func_0801e74c(temp_r5 + (s32)Value_00000a07, arg0, 0, 0);
    Func_0801e858(Data_08037428, (s32) arg0, 0x4A, 14);
    Func_0801e74c(temp_sl, arg0, 0x52, 0);
}
