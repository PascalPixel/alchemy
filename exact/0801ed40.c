#include "types.h"

s32 Func_080770c0(s32);
s32 Func_08019d2c(s32);
void Func_0801a4fc(s32, s32, s32 *, s32 *, s32, s32);

void Func_0801ed40(u32 slot, s32 character, s32 value)
{
    s32 result;
    s32 current;
    u32 character_id;
    u8 *state;
    s32 offset;

    state = *(u8 **)0x03001e8c;

    if (Func_080770c0(0x20) != 0) {
        if (character == 0)
            character = 0x12;
        if (character == 1)
            character = 0x13;
    }

    character_id = Func_08019d2c(character);
    if (character_id != -1U) {
        if (slot > 1U) {
            if (*(u16 *)(state + 0x12ee) == character_id) {
                slot = 1;
            } else if (*(u16 *)(state + 0x12ec) == character_id) {
                slot = 0;
            } else {
                return;
            }
        }
        offset = 0x12f0 + slot * 2;
        current = *(u16 *)(state + offset);
        Func_0801a4fc(character_id, value, &current, &result, slot + 0xe, 1);
    }
}
