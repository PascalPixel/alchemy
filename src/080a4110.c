typedef signed int s32;

extern s32 Data_080af2e4[];

s32 Func_080a4110(s32 column, s32 row)
{
    if (column > 2 || row > 2 || column < 0 || row < 0)
        return 0;
    return Data_080af2e4[row * 3 + column];
}
