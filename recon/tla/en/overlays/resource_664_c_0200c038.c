/*
 * Drafts of Func_02004038 and Func_02004084 (overlay 664, 0200c038 and
 * 0200c084), a ten-step glide toward the object at 104.
 * Func_02004038: differ 2, the target's y is loaded one instruction early.
 * Func_02004084: differ 2, the count's byte is stored after its zero
 * extension, where ⚓️ stores it first. `return --count != 0` on the byte
 * itself branches instead (differ 12).
 */
#include "types.h"
#include "field_event.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

s32 Func_02004038(struct FieldActor *object)
{
    struct FieldActor *target = (struct FieldActor *)WORD_AT(object, 104);

    WORD_AT(object, 68) = (target->x - object->x) / 10;
    WORD_AT(object, 76) = (target->y - object->y + 0x100000) / 10;
    WORD_AT(object, 72) = (target->z - object->z) / 10;
    BYTE_AT(object, 98) = 10;
    return 0;
}

s32 Func_02004084(struct FieldActor *object)
{
    u8 count;

    object->x += WORD_AT(object, 68);
    object->y += WORD_AT(object, 76);
    object->z += WORD_AT(object, 72);
    count = BYTE_AT(object, 98) - 1;
    BYTE_AT(object, 98) = count;
    return count != 0;
}
