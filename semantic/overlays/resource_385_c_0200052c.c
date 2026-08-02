typedef signed int s32;

/* Complete actor-12 two-flag dialogue scene through its three-word pool. */
extern void Func_0808a018(void);
extern void Func_0808a148(s32 actor, s32 mode, s32 value);
extern void Func_080770c8(s32 flag);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a020(void);

void Func_0200052c(void)
{
    Func_0808a018();
    Func_0808a148(12, 0, 2);
    Func_080770c8(0x306);
    Func_080770c8(0x868);
    Func_0808a170(0x1caf);
    Func_0808a180(12, 0);
    Func_0808a020();
}
