#include "types.h"

struct State_08078ee8 {
    u8 text[15];
    u8 pad_00f[5];
    s16 scale_y;
    s16 scale_x;
    u8 pad_018[0xc0];
    s16 values[15];
    u8 pad_0f6[0x32];
    u8 record_index;
};

struct Record_08078ee8 {
    u8 pad_000[0x96];
    u8 kind;
    u8 pad_097;
    u16 values[13];
};

extern s32 Data_0807b690[];
extern u8 Value_00000066;

struct State_08078ee8 *Func_08077394(s32);
void Func_08015020(s32, u16 *);
struct Record_08078ee8 *Func_08078ed8(s32);
s32 Func_08078588(s32, s32);
void Func_08078708(s32, s32);
void Func_08079ae8(s32);
void Func_080792fc(s32, s32);
void Func_08077428(s32);

void Func_08078ee8(void)
{
    volatile u16 text[16];
    s32 index;
    s32 *owner;

    owner = Data_0807b690;
    index = 0;
    do {
        struct State_08078ee8 *state = Func_08077394(index);
        s32 character = 0;
        s32 offset = 0;

        Func_08015020(index + (u32)&Value_00000066, (u16 *)text);
        state->text[0] = text[0];
        if (text[0] != 0) {
            u8 *destination = state->text;
            do {
                character++;
                offset += 2;
                if (character > 13)
                    break;
                destination++;
                *destination = *(volatile u16 *)((u8 *)text + offset);
            } while (*(volatile u16 *)((u8 *)text + offset) != 0);
        }
        state->text[14] = 0;
        index++;
    } while (index <= 7);

    while (*owner != -1) {
        struct State_08078ee8 *state = Func_08077394(*owner);

        if (state != 0) {
            struct Record_08078ee8 *record;
            s16 *value;
            s32 count;

            state->record_index = *owner;
            record = Func_08078ed8(state->record_index);

            value = &state->values[14];
            count = 14;
            do {
                count--;
                *value-- = 0;
            } while (count >= 0);

            count = 0;
            do {
                s32 mapped = Func_08078588(
                    *owner, record->values[count] & 0x1ff);
                Func_08078708(*owner, mapped);
                count++;
            } while ((u32)count <= 12);

            Func_08079ae8(*owner);
            state->scale_x = 0x4000;
            state->scale_y = 0x4000;
            Func_080792fc(*owner, record->kind);
            Func_08077428(*owner);
        }
        owner++;
    }
}
