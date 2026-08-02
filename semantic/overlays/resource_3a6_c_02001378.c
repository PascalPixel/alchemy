typedef signed int s32;

/* Actor-8 presentation reset at 0x02001378, including alignment to 0x1390. */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a100(s32 actor, s32 mode);

void Func_02001378(void)
{
    Func_0808a018();
    Func_0808a100(8, 0);
    Func_0808a020();
}
