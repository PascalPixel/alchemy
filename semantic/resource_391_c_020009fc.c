/* Advance the actor's +100 counter by a bounded random amount. */
typedef int s32; typedef unsigned int u32; typedef signed short s16;
typedef unsigned short u16; typedef unsigned char u8;
u32 Func_080000f8(void); void Func_0808a160(u8 *object, s32 mode);
s32 Func_020009fc(u8 *object)
{
    u16 *counter = (u16 *)(object + 100);
    *counter = (u16)(*counter + ((Func_080000f8() * 100) >> 16));
    if ((s16)*counter > 1000) Func_0808a160(object, 7);
    else Func_0808a160(object, 10);
    if (*(s16 *)counter > 1200) *counter = 0;
    return 1;
}
