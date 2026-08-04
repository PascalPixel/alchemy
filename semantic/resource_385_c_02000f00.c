typedef signed int s32;

/* Complete paired scene-resource enable wrapper through its two-word pool. */
extern void Func_0808a018(void);
extern void Func_08015040(s32 resource, s32 enabled);
extern void Func_0808a020(void);

void Func_02000f00(void)
{
    Func_0808a018();
    Func_08015040(0x947, 1);
    Func_08015040(0x29dc, 1);
    Func_0808a020();
}
