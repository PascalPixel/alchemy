typedef unsigned char u8; typedef signed int s32;
extern s32 Func_080770c0(s32); extern void Func_0808a090(s32,s32,s32); extern u8 *Func_0808a080(s32);
extern void Func_0808a100(s32,s32); extern void Func_0808a0d8(s32,s32,s32);
extern void Func_0808a010(s32); extern void Func_0808a248(s32);
void Func_02000c10(void)
{
    if (Func_080770c0(0x202) != 0 && Func_080770c0(0x201) == 0) {
        Func_0808a090(0, 0x3333, 0x1999);
        Func_0808a080(0)[0x55] = 0;
        Func_0808a100(0, 2);
        Func_0808a0d8(0, 2, -8);
        Func_0808a010(13);
        Func_0808a248(12);
    }
}
