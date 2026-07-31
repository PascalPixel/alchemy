typedef signed int s32;

typedef s32 (*IwramSqrt020002f0)(s32);

s32 Func_020002f0(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx * dx;
    s32 dysq = dy * dy;
    s32 dzsq = dz * dz;

    return ((IwramSqrt020002f0) 0x030001D8)(dxsq + dysq + dzsq);
}
