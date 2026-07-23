typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

void Func_08009020(void *, s32);
s32 Func_08077220(s16, s32);

extern u8 *Data_03001f2c;

void Func_080b11c4(s32 enabled, s32 selected, s32 value)
{
    u8 *state;
    u8 *halfBase;
    s32 *item;
    s32 index;
    s32 offset;
    s16 id;

    state = Data_03001f2c;
    if (enabled != 0) {
        index = 0;
        if (index < *(s8 *)(state + 0x3a7)) {
            halfBase = state + 2;
            offset = 0x36c;
            item = (s32 *)(state + 0x114);
            do {
                if (index == selected)
                    Func_08009020((void *)*item, 30);
                else
                    Func_08009020((void *)*item, 1);
                item[16] = 0x10000;
                id = *(s16 *)(halfBase + offset);
                if (Func_08077220(id, value) == 0)
                    item[16] = 0xcccc;
                index++;
                offset += 2;
                item++;
            } while (index < *(s8 *)(state + 0x3a7));
        }
    }
}
