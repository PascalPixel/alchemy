typedef signed int s32;

/* Complete actor-8 flag-setting dialogue scene through its two-word pool. */
extern void Func_0808a018(void);
extern void Func_0808a148(s32 actor, s32 mode, s32 value);
extern void Func_080770c8(s32 flag);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a020(void);

void Func_020004d8(void)
{
    Func_0808a018();
    Func_0808a148(8, 0, 2);
    Func_080770c8(0x305);
    Func_0808a170(0x1cab);
    Func_0808a180(8, 0);
    Func_0808a020();
}
