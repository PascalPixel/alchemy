/* NONMATCHING: 3 halfwords. Reload loads the 0x0be6 message base into r3; the
 * reference reloads it into r0, the call's own argument register. Spellings
 * of the sum (temporary, nested call, split add, base variable) move nothing.
 */
#include "TYPES.H"

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

struct OwnerProgressState *Owner_GetStateFar(s32 owner_id);
u32 Owner_GetLevelThresholdFar(s32 owner_id, s32 level);
void UiWork_PushValueSlotFar(u32 quantity, s32 style);
void *Runtime_BumpAllocate(s32 size);
void Func_08015030(s32 message_id, void *buffer, s32 length);
void UiText_RenderWideStringAtOffsetFar(void *buffer, void *destination, s32 offset, s32 terminator);
void Runtime_BumpFree(void *buffer);

/* The message id base is a link-time symbol, loaded from the literal pool. */
extern u8 Value_00000be6;


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
        struct OwnerProgressState *owner = Owner_GetStateFar(menu->owner_id);

        if (owner->level == 99) {
            message_variant = 8;
        } else {
            u64 remaining = Owner_GetLevelThresholdFar(menu->owner_id, owner->level + 1) -
                owner->experience;
            UiWork_PushValueSlotFar(remaining, 5);
        }
    }

    buffer = Runtime_BumpAllocate(0x100);
    Func_08015030(message_variant + (s32)&Value_00000be6, buffer, 0x80);
    UiText_RenderWideStringAtOffsetFar(buffer, destination, 0, -1);
    Runtime_BumpFree(buffer);
}
