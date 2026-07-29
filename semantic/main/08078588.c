typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

u8 *Func_08077394(s32);
u8 *Func_08078414(s32);

s32 Func_08078588(s32 owner, s32 target)
{
    u8 *object;
    u8 *cursor;
    s32 index;
    s32 offset;
    u32 value;
    u32 count;
    u32 mask;

    object = Func_08077394(owner);
    if ((Func_08078414(target)[3] & 0x10) != 0) {
        index = 0;
        mask = 0x1ff;
        if ((FIELD(object, u16, 216) ^ target) & mask) {
            cursor = object + 216;
            do {
                index++;
                if (index > 14)
                    break;
                cursor += 2;
            } while (((FIELD(cursor, u16, 0) ^ target) & mask) != 0);
        }

        if (index != 15) {
            offset = index * 2 + 216;
            value = FIELD(object, u16, offset);
            count = (value >> 11) + 1;
            if (count <= 29) {
                FIELD(object, u16, offset) =
                    (value & 0x7ff) | (count << 11);
                return index;
            }
            return -1;
        }
    }

    cursor = object + 216;
    index = 0;
    offset = 216;
    while (1) {
        value = FIELD(cursor, volatile u16, 0);
        cursor += 2;
        if (value == 0) {
            FIELD(object, u16, offset) = target;
            return index;
        }
        index++;
        offset += 2;
        if (index > 14)
            return -1;
    }
}
