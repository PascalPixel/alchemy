#include "types.h"

struct OwnerProgressState {
    u8 unknown_000[0x0f];
    u8 level;
    u8 unknown_010[0x114];
    u32 experience;
};

struct StatusMenuContext {
    u8 unknown_000[0x21a];
    u8 owner_id;
};

struct OwnerProgressState *Func_08077008(s32 owner_id);
u32 Func_08077258(s32 owner_id, s32 level);
void Func_08015120(u32 quantity, s32 style);
void *Func_08004938(s32 size);
void Func_08015030(s32 message_id, void *buffer, s32 length);
void Func_08015258(void *buffer, void *destination, s32 offset, s32 terminator);
void Func_08002df0(void *buffer);

void StatusMenu_ShowOwnerProgressMessage(
    void *destination,
    s32 message_variant,
    s32 preserve_variant)
{
    struct StatusMenuContext *menu =
        *(struct StatusMenuContext **)0x03001f2c;
    void *buffer;

    if (preserve_variant == 0 && message_variant > 3) {
        message_variant++;
    }
    if (message_variant == 1) {
        struct OwnerProgressState *owner = Func_08077008(menu->owner_id);

        if (owner->level == 99) {
            message_variant = 8;
        } else {
            u64 remaining = Func_08077258(menu->owner_id, owner->level + 1) -
                owner->experience;
            Func_08015120(remaining, 5);
        }
    }

    buffer = Func_08004938(0x100);
    Func_08015030(message_variant + 0x0be6, buffer, 0x80);
    Func_08015258(buffer, destination, 0, -1);
    Func_08002df0(buffer);
}
