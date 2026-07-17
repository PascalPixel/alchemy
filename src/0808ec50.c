typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080030f8(s32);
s32 Func_08009080(s32, s32);
s32 Func_0808ec14(void);
void Func_080f9010(s32);

void Func_0808ec50(void) {
    s32 t = Func_0808ec14();
    if (t != -1) {
        s32 b = *(s32 *)0x03001EBC;
        s32 o = t * 8;
        s32 v = *(s32 *)(b + o + 0x11C);
        if (v) {
            Func_08009080(v, 5);
        }
        Func_080f9010(0x7D);
        Func_080030f8(0xC);
    }
}
