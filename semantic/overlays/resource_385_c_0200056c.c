typedef signed int s32;

/* Complete actor-13 dialogue wrapper through its one-word pool. */
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a150(s32 actor, s32 mode, s32 value);
extern void Func_020004b4(s32 actor);

void Func_0200056c(void)
{
    Func_0808a170(0x1cb0);
    Func_0808a150(13, 0, 2);
    Func_020004b4(13);
}
