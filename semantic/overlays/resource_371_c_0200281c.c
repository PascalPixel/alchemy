typedef signed int s32;

extern void *Data_0200e79c;
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a148(s32 actor, s32 target, s32 frames);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a180(void *scene, s32 mode);
extern void Func_0808a1b8(s32 actor, s32 value, s32 frames);

/* Present dialogue 0x2642 while actor 55 crosses the foreground. */
void Func_0200281c(void)
{
    Func_0808a018();
    Func_0808a148(55, 0, 0);
    Func_0808a170(0x2642);
    Func_0808a180(Data_0200e79c, 0);
    Func_0808a1b8(55, 0x3000, 0);
    Func_0808a020();
}
