typedef signed int s32;
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a150(s32 actor, s32 target, s32 frames);
extern void Func_0808a170(s32 message);
extern void Func_0808a190(s32 actor, s32 mode);

/* Complete 44-byte actor-15 scene owner through its one-word pool. */
void Func_02000190(void)
{
    Func_0808a018();
    Func_0808a170(0xf6d);
    Func_0808a150(0, 15, 6);
    Func_0808a190(15, 0);
    Func_0808a020();
}
