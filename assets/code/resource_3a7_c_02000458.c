typedef signed int s32;

extern s32 *Func_0200158e(s32);

s32 Func_02000458(void)
{
    s32 *record = Func_0200158e(10);
    s32 depth = record[4];
    s32 across;
    s32 depthCell;
    s32 acrossCell;

    if (depth < 0) {
        depth += 0x000FFFFF;
    }
    across = record[2];
    depthCell = depth >> 20;
    if (across < 0) {
        across += 0x000FFFFF;
    }
    acrossCell = across >> 20;
    if (acrossCell == 16 && depthCell == 12) {
        return 1;
    }
    return 0;
}
