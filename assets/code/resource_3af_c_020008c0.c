typedef unsigned char u8;
typedef signed int s32;

void Func_020008c0(u8 *o) {
    if (*(s32 *)(o + 24) > 0x10000) {
        *(s32 *)(o + 24) += 0xFFFFF800;
        *(s32 *)(o + 28) += 0xFFFFF800;
    }
}
