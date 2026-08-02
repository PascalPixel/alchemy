typedef signed int s32;

extern void Func_08009180(s32, s32, s32, s32, s32, s32);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern void Func_080000c0(s32 frames);

/* Publish the scene's upper prompt and lower dialogue panel. */
void Func_02003270(void)
{
    Func_08009180(0, 64, 11, 68, 3, 2);
    Func_080091c0(11, 10, 3, 2, 11, 8);
    Func_080000c0(1);
}
