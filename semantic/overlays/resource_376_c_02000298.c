typedef signed int s32;
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 actor, s32 mode);

/* Complete 36-byte actor-0x800b dialogue owner through its two-word pool. */
void Func_02000298(void)
{
    Func_0808a018();
    Func_0808a170(0x1c40);
    Func_0808a180(0x800b, 0);
    Func_0808a020();
}
