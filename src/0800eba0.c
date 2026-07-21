typedef signed int s32;

s32 Func_0800eba0(s32 *a, s32 arg1, s32 *b, s32 arg3) {
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 radius = arg1 + arg3;
    if (!(dx > 0x400000) && !(dz > 0x400000) &&
        (dx * dx + dy * dy + dz * dz) < radius * radius)
        return 0;
    return -1;
}
