typedef unsigned char u8; typedef unsigned short u16; typedef signed int s32;
extern u8 *Data_03001ebc; extern u8 *Func_0808a080(s32); extern s32 Func_080770c0(s32); extern void Func_080770c8(s32);
void Func_02000ac4(void)
{
    s32 z = *(s32 *)(Func_0808a080(0) + 16);
    if (z < 0)
        z += 0xfffff;
    if (Func_080770c0(0x243) == 0 && (z >> 20) == 10) {
        Func_080770c8(0x243);
        *(u16 *)(*(u8 **)Data_03001ebc + 364) = 20;
    }
}
