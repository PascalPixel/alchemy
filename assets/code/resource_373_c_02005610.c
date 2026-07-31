typedef signed int s32;

typedef s32 (*IwramSqrt02005610)(s32);

s32 Func_02005610(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx * dx;
    s32 dysq = dy * dy;
    s32 dzsq = dz * dz;

    return ((IwramSqrt02005610) 0x030001D8)(dxsq + dysq + dzsq);
}
