typedef signed int s32;
typedef unsigned char u8;

void Func_080df9d0(u8 *source, u8 *destination, s32 stride) {
    s32 row = 0;
    s32 rowCount = 288;
    s32 sourceOffset = 0;

    do {
        s32 column;

        for (column = 0; column != 40; column++) {
            destination[sourceOffset / 2 + column / 2] = source[sourceOffset + column];
        }
        sourceOffset += stride;
        row++;
    } while (row != rowCount);
}
