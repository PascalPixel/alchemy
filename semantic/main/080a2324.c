typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_080a17c4(void *);

void Func_080a2324(s32 arg0, s32 arg1, s32 unused,
                   s32 value, s32 start) {
    void *state;
    void **cursor;
    void **clear_cursor;
    void *object;
    s32 index;
    s32 temp_end;
    s32 offset;
    s32 entry_offset;
    s32 position;
    s32 status;

    state = *(void **)0x03001F2C;
    status = 0xD;
    clear_cursor = (void **)((u8 *)state + 0x48);
    index = 0x1F;
    do {
        object = *clear_cursor++;
        if (object != 0)
            FIELD(object, s8, 5) = status;
        index--;
    } while (index >= 0);

    index = arg1;
    temp_end = arg0 + index;
    if (index < temp_end) {
        offset = index * 4;
        entry_offset = offset;
        entry_offset += 0x48;
        object = *(void **)((u8 *)state + entry_offset);
        if (object != 0 &&
            index <= (s32)(FIELD(state, u8, 0x218) - 1)) {
            cursor = (void **)((u8 *)state + offset + 0x48);
            position = start;
            do {
                FIELD(object, s16, 6) = value;
                FIELD(object, s16, 8) = position;
                Func_080a17c4(object);
                index++;
                FIELD(object, s8, 5) = 1;
                position += 0x10;
                if (index >= temp_end)
                    break;
                cursor++;
                object = *cursor;
                if (object == 0)
                    break;
            } while (index <=
                     (s32)(FIELD(state, u8, 0x218) - 1));
        }
    }
}
