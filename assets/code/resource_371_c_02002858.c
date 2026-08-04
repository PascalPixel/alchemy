typedef signed int s32;

extern void *Data_0200e79c;
extern void Func_02006b38(void);
extern void Func_02006b74(void);
extern void Func_02006b8e(s32 actor, s32 x, s32 y);
extern void Func_02006bd0(s32 actor, s32 x, s32 y);
extern void Func_02006c0a(s32 dialogue);
extern void Func_02006c24(void *scene, s32 mode);
extern void Func_02006cbc(void);
extern void Func_02006cd8(void);

/* Open dialogue 0x2643, then restore actor zero's scene position. */
void Func_02002858(void)
{
    Func_02006b38();
    Func_02006cbc();
    Func_02006c0a(0x2643);
    Func_02006c24(Data_0200e79c, 0);
    Func_02006cd8();
    Func_02006b8e(0, 0x10000, 0x8000);
    Func_02006bd0(0, 0x1778, 0x0d48);
    Func_02006b74();
}
