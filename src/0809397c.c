typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

s32 Func_080072f0(s32, s32, s32, s32);
s32 Func_080022ec(s32, s32);
s32 Func_08009150(s32, s32, s32, s32);
s32 Func_08009080(s32, s32);

s32 Func_0809397c(s32 arg0)
{
    s32 squareRoot = 0x030001d8;
    s32 object;
    void *target;
    s32 deltaX;
    s32 deltaY;
    s32 cellX;
    s32 cellY;
    s32 newX;
    s32 distance;

    object = arg0;
    target = *(void **)(object + 0x68);
    if (target != 0) {
        deltaX = *(s32 *)(target + 8) - *(s32 *)(object + 8);
        if (deltaX < 0)
            deltaX += 0xffff;
        cellX = deltaX >> 16;
        deltaY = *(s32 *)(target + 0x10) - *(s32 *)(object + 0x10);
        if (deltaY < 0)
            deltaY += 0xffff;
        cellY = deltaY >> 16;
        distance = Func_080072f0(cellX * cellX + cellY * cellY,
                                 (s32)target, cellY, squareRoot);
        arg0 = *(s16 *)(object + 0x64);
        if (distance >= arg0) {
            newX = *(s32 *)(object + 8) +
                Func_080022ec(cellX << 20, arg0);
            Func_08009150(object, newX, *(s32 *)(object + 0x0c),
                          *(s32 *)(object + 0x10) +
                              Func_080022ec(cellY << 20, arg0));
            Func_08009080(object, 2);
        } else {
            Func_08009080(object, 1);
        }
    }
    return 1;
}
