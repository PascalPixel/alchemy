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

s32 Func_08009140();
void *Func_0808ba1c();

void Func_080920a0(void) {
    void *temp_r0;
    register u8 *field asm("r1");
    register u32 old asm("r2");
    register u32 value asm("r3");

    temp_r0 = Func_0808ba1c();
    if (temp_r0 != NULL) {
        field = (u8 *)temp_r0 + 0x5A;
        old = *field;
        value = 1;
        value |= old;
        *field = value;
        Func_08009140();
    }
}
