#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s8 Func_080022fc(s32, s32);
void Func_08002df0(void *);
void Func_080030f8(s32);
u16 *Func_08004938(s32);
u8 *Func_080150d0(s32, s32, u8 *, s32, s32);
void Func_08015270(u8 *);
void Func_08015278(u8 *);
u8 *Func_08077008(u16);
s32 Func_080770c0(s32);
void Func_080770d0(s32);
s32 Func_080a10d0(u8 *, s32, s32, s32, s32, s32);
void Func_080a112c(u8 *, u16, s32, s32);
void Func_080a1804(u8 *, u16);
void Func_080a1a40(s32, s32);
void Func_080a23c0(u8 *);
void Func_080a33d4(u8 *, u8 *);
void Func_080a6384(u16);
void Func_080a6614(u8 *, u16);
u8 Func_080a68ec(u8 *, u16 *, s32);
void Func_080f9010(s32);

/*
 * Present the character action selector and return the selected action, or -1
 * for cancellation.  The historical caller's second register is not an input.
 */
s32 Func_080a60d4(const u16 *actions)
{
    u8 *state = *(u8 **)0x03001f2c;
    s32 count = FIELD(state, s8, 0x1e);
    s8 selection = FIELD(state, s8, 0x1c);
    s32 result = 0;
    s32 dirty = 1;
    s32 detail_ready = 0;
    u8 *character;

    FIELD(state, u8, 0x268) = 0;
    character = Func_08077008(actions[selection]);

    if (Func_080a10d0(state + 0x20, 0x0d, 3, 0x11, 0x0a, 2) != 0)
        Func_080a33d4(state, FIELD(state, u8 *, 0x20));

    if (Func_080a10d0(state + 0x28, 0x0d, 0x0d, 0x11, 4, 2) != 0) {
        u8 *object =
            Func_080150d0(2, 0, FIELD(state, u8 *, 0x28), 0, result);
        FIELD(state, u8 *, 0x21c) = object;
        FIELD(object, s8, 5) = 0x0d;
    }

    for (;;) {
        s32 index;
        u16 action;

        if (Func_080770c0(0x150) != 0)
            break;

        if (dirty != 0) {
            s32 i;
            u8 *handle;

            dirty = 0;
            selection = Func_080022fc(count + selection, count);
            index = selection;
            action = actions[index];
            handle = FIELD(state, u8 *, 0x24);
            character = Func_08077008(action);
            Func_080a6384(action);
            Func_080a112c(handle, action, 0, 0);
            Func_080a6614(FIELD(state, u8 *, 0x28), action);
            Func_080a1804(state, action);

            for (i = 0; i < 4; i++)
                FIELD(state, u16, 0x144 + i * 2) = 0x1e;
            FIELD(state, u16, 0x144 + index * 2) = 0x1a;

            if (Func_080770c0(0x151) == 0 && detail_ready == 0) {
                Func_08015278(FIELD(state, u8 *, 0x2c));
                Func_08015270(FIELD(state, u8 *, 0x2c));
                Func_080a23c0(FIELD(state, u8 *, 0x2c));
                detail_ready = 1;
            } else {
                Func_080770d0(0x151);
            }
        }

        index = selection;
        action = actions[index];
        Func_080a1a40(index * 0x18 - 0x0a, 0x10);
        Func_080030f8(1);

        if ((*(u32 *)0x03001c94 & 1) != 0) {
            if (FIELD(state, u8, 0x218) != 0) {
                Func_080f9010(0x70);
                result = action;
                break;
            }
            Func_080f9010(0x72);
        }

        if ((*(u32 *)0x03001c94 & 0x300) != 0) {
            u16 *buffer;
            s8 accepted;

            result = action;
            if ((*(u32 *)0x03001c94 & 0x200) != 0)
                FIELD(state, u8, 0x268) = 1;
            else
                FIELD(state, u8, 0x268) = 2;

            buffer = Func_08004938(0x40);
            accepted = (s8)Func_080a68ec(character, buffer, 1);
            Func_08002df0(buffer);
            if (accepted != 0) {
                Func_080f9010(0x70);
                break;
            }
            FIELD(state, u8, 0x268) = 0;
            Func_080f9010(0x72);
        }

        if ((*(u32 *)0x03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            result = -1;
            break;
        }
        if ((*(u32 *)0x03001b04 & 0x20) != 0) {
            Func_080f9010(0x6f);
            dirty = 1;
            selection--;
        }
        if ((*(u32 *)0x03001b04 & 0x10) != 0) {
            Func_080f9010(0x6f);
            dirty = 1;
            selection++;
        }
    }

    FIELD(state, s8, 0x1c) = selection;
    FIELD(state, s32, 8) = actions[selection];
    FIELD(state, s8, 0x21a) = (s8)actions[selection];
    return result;
}
