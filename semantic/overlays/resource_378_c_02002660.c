typedef unsigned char u8;
typedef signed int s32;

/* Resource 378 object reset at 0x02002660 (28 bytes including alignment). */

extern u8 *Func_0808a080();
extern void Func_0808a0f0();

void Func_02002660(void)
{
    u8 *state = Func_0808a080(14);
    *(s32 *)(state + 108) = 0;
    Func_0808a0f0(14, 0, 0);
}
