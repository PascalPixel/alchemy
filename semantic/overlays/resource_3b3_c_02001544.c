typedef signed int s32;

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern s32 Func_02000758(s32 result[6]);
extern void Func_020008ec(s32, s32, s32, s32, s32, s32);

/*
 * Complete 52-byte placement-query wrapper.  This is the exact reviewed
 * control-flow homolog of resource_392:0x020009f8, with this overlay's local
 * producer and consumer independently resolved at 0x02000758/0x020008ec.
 */
void Func_02001544(void)
{
    s32 result[6];

    Func_0808a018();
    if (Func_02000758(result) != 0)
        Func_020008ec(result[0], result[1], result[2], result[3],
                      result[4], result[5]);
    Func_0808a020();
}
