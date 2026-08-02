typedef signed int s32;
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a060(s32 frames, s32 mode);
extern void Func_0808a168(s32 actor, s32 value, const void *script);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a188(s32 actor, s32 mode, s32 frames);
extern void Func_08015040(s32 message, s32 mode);

/* Complete 112-byte two-path scene owner through its four-word pool. */
void Func_020001e8(void)
{
    Func_0808a018();
    if (Func_080770c0(0x81b) != 0) {
        Func_0808a170(0x11a6);
        Func_0808a180(20, 0);
        Func_0808a168(20, 0x10000, (void *)0x020092fc);
    } else {
        s32 message = 0x11a4;
        Func_0808a170(message);
        Func_0808a188(20, 0, 20);
        Func_08015040(message + 1, 1);
        Func_0808a060(180, 0);
        Func_080770c8(0x81b);
    }
    Func_0808a020();
}
