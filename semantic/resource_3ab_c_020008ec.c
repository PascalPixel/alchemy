typedef signed short s16; typedef signed int s32;
extern void Func_0808a170(s32); extern s32 Func_080770c0(s32);
extern void Func_0808a1e8(s32,s32,s32); extern void Func_0808a180(s32,s32);
extern void Func_0808a130(s32,s32); extern void Func_080770c8(s32);
void Func_020008ec(void)
{
    if (*(s16 *)(0x02000240 + 588) != 0) {
        Func_0808a170(0x2411);
    } else if (Func_080770c0(0x941) != 0 && Func_080770c0(0x94d) == 0) {
        Func_0808a1e8(8, 0x101, 60);
        Func_0808a170(0x24db);
        Func_0808a180(8, 0);
        Func_0808a130(8, 1);
        Func_0808a170(0x24dc);
        Func_080770c8(0x9af);
    } else {
        Func_0808a170(0x1bb5);
    }
    Func_0808a180(8, 0);
}
