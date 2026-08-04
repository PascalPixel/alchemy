typedef signed int s32;

extern void *Data_0200e79c;
extern void Func_02006afc(void);
extern void Func_02006b2e(void);
extern void Func_02006bb6(s32 actor, s32 target, s32 frames);
extern void Func_02006bd4(s32 dialogue);
extern void Func_02006bee(void *scene, s32 mode);
extern void Func_02006c0a(s32 actor, s32 value, s32 frames);

/* Present dialogue 0x2642 while actor 55 crosses the foreground. */
void Func_0200281c(void)
{
    Func_02006afc();
    Func_02006bb6(55, 0, 0);
    Func_02006bd4(0x2642);
    Func_02006bee(Data_0200e79c, 0);
    Func_02006c0a(55, 0x3000, 0);
    Func_02006b2e();
}
