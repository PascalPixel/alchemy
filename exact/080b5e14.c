#include "types.h"

void *Func_08004970(s32);
void Func_08002df0(void *);
void Func_08006488(void);
s32 Func_08006408(void);
void Func_080030f8(s32);
u8 *Func_08077008(s32);
void *Func_08077000(s32);
void Func_08015020(s32, u16 *);

s32 Func_080b5e14(void)
{
    u16 text[24];
    void *buffer;
    u8 *entry;
    s32 count;
    s32 index;
    s32 len;
    s32 i;

    buffer = Func_08004970(340);
    count = 0;
    index = 0;
    while (index <= 2) {
        entry = Func_08077008(index + 128);
        if (Func_08006408() == -1) {
            break;
        }
        Func_08006488();
        if (entry[298] != 0) {
            count += 1;
        }
        Func_080030f8(2);
        Func_08015020(0x80C, text);
        i = 0;
        if (text[i] != 0) {
            do {
                i += 1;
                if (i > 4) {
                    break;
                }
            } while (text[i] != 0);
        }
        len = i;
        for (i = 14; i >= len; i--) {
            entry[i] = entry[i - len];
        }
        for (i = 0; i < len; i++) {
            entry[i] = (u8)text[i];
        }
        entry[14] = 0;
        index += 1;
    }
    Func_08002df0(buffer);
    buffer = Func_08004970(320);
    Func_08077000(1);
    if (Func_08006408() != -1) {
        Func_08006488();
        Func_080030f8(2);
    }
    Func_08002df0(buffer);
    return count;
}
