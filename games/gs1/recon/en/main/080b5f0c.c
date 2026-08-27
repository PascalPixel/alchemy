#include "types.h"

extern void *Data_03001388;

void *Func_08004970(s32);
s32 Func_080b6a60(void *);
void *Func_08077008(s32);
void Func_080072f0(void *dst, void *src, s32 size, const void *fmt);
s32 Func_080063bc(void *);
void Func_08006458(void);
void Func_080030f8(s32);
void Func_08002df0(void *);
void *Func_08077000(s32);

s32 Func_080b5f0c(void)
{
    u8 *table;
    u8 *buffer;
    u16 sp_names[8];
    s32 count;
    s32 named_count;
    s32 i;
    s32 j;
    s32 result;

    buffer = Func_08004970(340);
    table = Data_03001388;

    for (j = 7; j >= 0; j--) {
        table[72 + j] = 0xff;
    }

    count = Func_080b6a60(sp_names);
    named_count = 0;

    if (0 < count) {
        i = 0;
        do {
            void *object = Func_08077008(((u16 *)((u8 *)sp_names + i))[0]);
            Func_080072f0(buffer, object, 340, 0);
            buffer[298] = 2;

            table[((u16 *)((u8 *)sp_names + i))[0] + 72] = (u8)(named_count - 128);

            result = Func_080063bc(buffer);
            if (result == -1) {
                break;
            }
            Func_08006458();
            named_count++;
            Func_080030f8(2);
            i += 2;
        } while (named_count < count);
    }

    named_count = 0;
    while (named_count <= 2) {
        buffer[298] = 0;
        result = Func_080063bc(buffer);
        if (result == -1) {
            break;
        }
        Func_08006458();
        Func_080030f8(2);
        named_count++;
    }

    Func_08002df0(buffer);
    buffer = Func_08004970(320);
    {
        void *unit = Func_08077000(0);
        Func_080072f0(buffer, unit, 320, 0);
    }

    {
        s32 count2 = *(s32 *)(buffer + 264);
        u8 *entry = buffer + 8;
        for (i = 0; i < count2; i++) {
            entry[2] = table[entry[2] + 72];
            count2 = *(s32 *)(buffer + 264);
            entry += 4;
        }
    }

    result = Func_080063bc(buffer);
    if (result != -1) {
        Func_08006458();
        Func_080030f8(1);
        Func_080030f8(2);
    }

    Func_08002df0(buffer);
    return named_count;
}
