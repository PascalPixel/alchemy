#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s8 Func_080022fc(s32, s32);
void Func_080030f8(s32);
void Func_08015080(s32, u8 *, s32, s32);
u8 *Func_080150d0(s32, s32, u8 *, s32, s32);
u8 *Func_08077008(u16);
s32 Func_080770c0(s32);
s32 Func_080a10d0(u8 *, s32, s32, s32, s32, s32);
void Func_080a112c(u8 *, u16, s32, s32);
void Func_080a1a40(s32, s32);
void Func_080a1e38(u8 *, u8);
void Func_080a33d4(u8 *, u8 *);
void Func_080a38a8(u16);
s32 Func_080a3d6c(u16);
s8 Func_080a3ddc(u8 *, u8 *, s32);
void Func_080a3e88(u16, s32);
void Func_080f9010(s32);

/*
 * Present the party-character selector and return the chosen character ID,
 * zero while the external event closes it, or -1 for cancellation.
 */
s32 Func_080a35f8(const u16 *character_ids)
{
    u8 *state = *(u8 **)0x03001f2c;
    s8 selection = FIELD(state, s8, 0x1c);
    s32 count = FIELD(state, s8, 0x1e);
    s32 result = 0;
    s32 page = 0;
    s32 alternate_mode = 0;
    s32 dirty = 1;
    u8 *character = Func_08077008(character_ids[selection]);

    if (Func_080a10d0(state + 0x20, 0x0d, 3, 0x11, 0x0a, 2) != 0)
        Func_080a33d4(state, FIELD(state, u8 *, 0x20));

    if (Func_080a10d0(state + 0x28, 0x0d, 0x0d, 0x11, 4, 2) != 0) {
        u8 *object =
            Func_080150d0(2, 0, FIELD(state, u8 *, 0x28), 0, result);
        FIELD(state, u8 *, 0x21c) = object;
        FIELD(object, s8, 5) = 0x0d;
    }

    Func_08015080(0x0b87, FIELD(state, u8 *, 0x28), 0, 0);
    Func_08015080(0x0b88, FIELD(state, u8 *, 0x28), 0, 8);
    FIELD(FIELD(state, u8 *, 0x14), s8, 5) = 1;

    for (;;) {
        s32 index;
        u16 character_id;

        if (Func_080770c0(0x150) != 0)
            break;

        selection = Func_080022fc(count + selection, count);
        index = selection;
        character_id = character_ids[index];
        Func_080a1a40(index * 0x18 - 0x0a, 0x10);

        if (dirty != 0) {
            s32 i;
            u8 *handle = FIELD(state, u8 *, 0x24);

            page = 0;
            dirty = 0;
            character = Func_08077008(character_id);
            if (alternate_mode != 0) {
                FIELD(state, s8, 0x218) = Func_080a3ddc(
                    Func_08077008(character_id), state + 0x1c8, 0);
                Func_080a38a8(character_id);
                Func_080a112c(handle, character_id, 0, 8);
            } else {
                Func_080a3e88(character_id, 0);
                Func_080a112c(handle, character_id, 0, 0);
            }

            for (i = 0; i < 4; i++)
                FIELD(state, u16, 0x144 + i * 2) = 0x1e;
            FIELD(state, u16, 0x144 + index * 2) = 0x1a;
        }

        Func_080030f8(1);
        if ((*(u32 *)0x03001c94 & 1) != 0) {
            if ((*(u32 *)0x03001ae8 & 0x200) != 0) {
                page = (page + 4) % 4;
                Func_080a1e38(character + 0xd8, (u8)page);
                page = (u8)(page + 1);
                Func_080a3e88(character_id, 0);
                Func_080f9010(0x70);
            } else if (Func_080a3d6c(character_id) != 0) {
                Func_080f9010(0x70);
                result = character_id;
                break;
            } else {
                Func_080f9010(0x72);
            }
        }

        if ((*(u32 *)0x03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            result = -1;
            break;
        }

        if ((*(u32 *)0x03001c94 & 0x100) != 0) {
            alternate_mode = 1;
            dirty = 1;
        }
        if ((*(u32 *)0x03001ae8 & 0x100) == 0 && alternate_mode == 1) {
            alternate_mode = 0;
            dirty = 1;
        }
        if ((*(u32 *)0x03001b04 & 0x20) != 0) {
            Func_080f9010(0x6f);
            selection--;
            dirty = 1;
        }
        if ((*(u32 *)0x03001b04 & 0x10) != 0) {
            Func_080f9010(0x6f);
            selection++;
            dirty = 1;
        }
    }

    FIELD(state, s8, 0x1c) = selection;
    FIELD(state, s32, 8) = character_ids[selection];
    FIELD(state, s8, 0x21a) = (s8)character_ids[selection];
    return result;
}
