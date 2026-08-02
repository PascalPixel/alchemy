typedef signed short s16; typedef signed int s32;
extern void Func_0808a170(s32); extern s32 Func_080770c0(s32); extern void Func_0808a180(s32,s32);
void Func_02000980(void)
{
    if (*(s16 *)(0x02000240 + 588) != 0)
        Func_0808a170(0x2412);
    else if (Func_080770c0(0x941) != 0)
        Func_0808a170(0x24dd);
    else
        Func_0808a170(0x1bb6);
    Func_0808a180(9, 0);
}
