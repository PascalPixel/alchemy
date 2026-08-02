typedef unsigned short u16;
typedef unsigned int u32;

/*
 * Complete 48-byte prologue-less alternate-affine builder.  It copies the
 * live three-word transform, then offsets the high half of every word by 192.
 * The return at 0x020015f0 owns alignment and its two-word pool through
 * 0x020015fb.
 */
void Func_020015cc(void)
{
    const u32 *source = (const u32 *)0x03001ad4;
    u32 *alternate = (u32 *)0x0200b72c;
    u16 *halves = (u16 *)alternate;

    alternate[0] = source[0];
    alternate[1] = source[1];
    alternate[2] = source[2];
    halves[1] += 192;
    halves[3] += 192;
    halves[5] += 192;
}
