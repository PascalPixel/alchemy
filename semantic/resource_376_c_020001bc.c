typedef signed int s32;
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a150(s32 actor, s32 target, s32 frames);
extern void Func_0808a170(s32 message);
extern void Func_0808a190(s32 actor, s32 mode);

/* Complete 44-byte actor-19 scene owner through its one-word pool. */
void Func_020001bc(void)
{
    Func_0808a018();
    Func_0808a170(0xf73);
    Func_0808a150(0, 19, 6);
    Func_0808a190(19, 0);
    Func_0808a020();
}
