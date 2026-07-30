#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_08002df0(void *);
void *Func_08004938(s32);
void Func_080072f0(void *, const void *, s32, const void *);
void Func_080030f8(s32);
void Func_08015068(void *, s32, s32, s32, s32);
void Func_08015080(s32, void *, s32, s32);
void Func_08015090(void *, void *, s32, s32);
void Func_080150b0(s32, s32, void *, s32, s32);
void *Func_080150d8(s32, s32, s32, void *, s32, s32);
void Func_08015278(void *);
void *Func_08077008(u16);
void Func_08077010(u16);
void *Func_08077018(u16);
void Func_08077050(u16, s32);
s32 Func_08077218(u16, u16);
s32 Func_080a10d0(void *, s32, s32, s32, s32, s32);
void Func_080a153c(void *, void *);
void Func_080a15f0(void *, void *, void *);
void Func_080a8b10(u8 *, s32, u16);

/*
 * Draw the character detail panel selected by mode.  The high byte of mode
 * suppresses creation of the panel's auxiliary window.
 */
void Func_080a112c(s32 unused, u16 character_id, s32 slot, s32 mode)
{
    u8 *state = *(u8 **)0x03001f2c;
    u8 *character = Func_08077008(character_id);
    u16 selected_value = FIELD(character, u16, 0xd8 + slot * 2);
    void *metadata = Func_08077018(selected_value & 0x1ff);
    void *window;
    void *auxiliary = 0;
    u8 summary[5];
    s32 shown = 0;

    (void)unused;

    if ((mode & 0x100) == 0)
        auxiliary = (void *)Func_080a10d0(
            state + 0x24, 0, 5, 13, 12, 0x102);

    window = FIELD(state, void *, 0x24);

    if ((mode & 0x100) == 0) {
        if (auxiliary == 0) {
            Func_080030f8(1);
            Func_08015068(window, 0, 0, 88, 32);
        }

        Func_08015090(character, window, 32, 0);
        Func_080a8b10(summary, 1, character_id);

        if (summary[1] != 0) {
            Func_08015080(0xbd6, window, 32, 8);
            shown = 1;
        }
        if (summary[2] != 0) {
            Func_08015080(0xbd7, window, 32, shown * 8 + 8);
            shown++;
        }
        if (summary[3] != 0) {
            Func_08015080(0xbd8, window, 32, shown * 8 + 8);
            shown++;
        }
        if (summary[4] != 0) {
            Func_08015080(0xbd9, window, 32, shown * 8 + 8);
            shown++;
        }

        if (shown <= 1) {
            Func_08015090((void *)0x080af20c, window, 40, 16);
            Func_080150b0(FIELD(character, u8, 0x0f), 4, window, 56, 16);
        }
    }

    if (auxiliary == 0) {
        Func_080030f8(1);
        Func_08015068(window, 0, 32, 88, 80);
    }
    Func_08015278(window);

    if ((mode & 0x100) == 0)
        FIELD(state, void *, 0x17c) =
            Func_080150d8(character_id, 0, 0, window, 0, 0);

    switch (mode & 0xff) {
    case 0: {
        s32 icon = FIELD(character, u8, 0x129) + 0xbd6;
        s32 value = FIELD(character, s32, 0x124);

        Func_08015080(icon, window, 0, 32);
        Func_080a153c(character, window);
        Func_08015080(0xbd7, window, 0, 64);
        Func_080150b0(value, 8, window, 24, 72);
        break;
    }

    case 2:
    case 3:
        if (Func_08077218(character_id, selected_value) == 0) {
            Func_08015080(0xb0e, window, 0, 48);
        } else {
            void *snapshot = Func_08004938(0x14c);

            Func_080072f0(snapshot, character, 0x14c, (void *)0x03001388);
            if (FIELD(state, s8, 0x25c) != 0) {
                FIELD(character, u16, 0xd8 + slot * 2) &= 0xfdff;
            } else {
                Func_08077050(character_id, slot);
            }
            Func_08077010(character_id);
            Func_080a15f0(character, snapshot, window);
            Func_080072f0(character, snapshot, 0x14c, (void *)0x03001388);
            Func_08002df0(snapshot);
        }
        break;

    case 4: {
        u16 item_id = FIELD(metadata, u16, 40);
        s32 equipped = 0;
        s32 i;

        for (i = 0; i < 32; i++) {
            if ((FIELD(character, u16, 88 + i * 4) & 0x3fff) == item_id) {
                equipped = 1;
                break;
            }
        }

        if (equipped)
            Func_08015080(item_id + 0x741, window, 0, 48);
        else
            Func_08015080(item_id + 0x333, window, 0, 48);
        Func_08015080(equipped ? 0xb23 : 0xb22, window, 0, 56);
        break;
    }

    case 6:
        Func_08015080(FIELD(character, u8, 0x129) + 0xbd6, window, 0, 32);
        Func_080a153c(character, window);
        break;

    case 8:
        Func_08015080(0xb1c, window, 0, 40);
        Func_080150b0(FIELD(character, u16, 60), 3, window, 64, 40);
        Func_08015080(0xb1d, window, 0, 48);
        Func_080150b0(FIELD(character, u16, 62), 3, window, 64, 48);
        Func_08015080(0xb20, window, 0, 56);
        Func_080150b0(FIELD(character, u16, 64), 3, window, 64, 56);
        Func_08015080(0xb1f, window, 0, 64);
        Func_080150b0(FIELD(character, u8, 66), 3, window, 64, 64);
        break;

    default:
        break;
    }
}
