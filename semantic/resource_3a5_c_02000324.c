typedef unsigned char u8; typedef signed int s32;
void Func_080000c0();
void Func_02000324(u8 *o, s32 limit)
{
    s32 frames=60;
    while (frames-- != 0) { Func_080000c0(1); if (*(s32 *)(o+12)<=limit) break; }
}
