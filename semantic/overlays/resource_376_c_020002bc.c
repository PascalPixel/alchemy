typedef unsigned char u8;
typedef signed int s32;
extern u8 *Data_03001ebc;
extern s32 Func_080770c0(s32 flagId);
extern void Func_0808a248(s32 value);
extern void Func_0808a2d0(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080f9010(s32 soundId);

/* Complete 76-byte shared numbered-scene owner through its two-word pool. */
void Func_020002bc(s32 value)
{
    if (Func_080770c0(0x834) != 0)
        Func_0808a2d0();
    Func_080f9010(123);
    *(s32 *)(Data_03001ebc + 448) = 521;
    *(s32 *)(Data_03001ebc + 456) = 16;
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(value);
}
