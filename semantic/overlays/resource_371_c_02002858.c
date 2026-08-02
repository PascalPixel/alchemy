typedef signed int s32;

extern void *Data_0200e79c;
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a090(s32 actor, s32 x, s32 y);
extern void Func_0808a0d0(s32 actor, s32 x, s32 y);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a180(void *scene, s32 mode);
extern void Func_0808a3d8(void);
extern void Func_0808a3e0(void);

/* Open dialogue 0x2643, then restore actor zero's scene position. */
void Func_02002858(void)
{
    Func_0808a018();
    Func_0808a3d8();
    Func_0808a170(0x2643);
    Func_0808a180(Data_0200e79c, 0);
    Func_0808a3e0();
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a0d0(0, 0x1778, 0x0d48);
    Func_0808a020();
}
