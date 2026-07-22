typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

void Func_08006b84(u32 source, u8 *destination, s32 size)
{
    const u8 *cursor = (const u8 *)source;
    u8 *out = destination;
    s32 index = size - 1;

    if (size != 0) {
        do {
            source = *cursor;
            *out = source;
            cursor++;
            out++;
            index--;
        } while (index != -1);
    }
}
