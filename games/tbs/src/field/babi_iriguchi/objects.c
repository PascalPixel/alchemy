#include "iriguchi.h"

void OverlayObject_SetOwnerMode(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);
    owner->mode = mode;
}

u8 *OverlayObject_SpawnConfiguredObject(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Engine_ObjectCreate(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);

        u32 masked = ~12;
        masked = masked & owner[9];
        owner[9] = (u8)masked;
        object[85] = 0;
        object[89] = 8;
        Engine_ActorSetSpriteFlags(object, 0);
        ObjectGroup_SetChildValue(object, 14);
        Engine_ObjectSetBlendMode(object, 1);
        result = object;
    } else {
        result = 0;
    }
    return result;
}

u8 *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Engine_ObjectCreate(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        u32 mask = ~12;
        u32 low;
        mask = mask & owner[9];
        owner[9] = (u8)(mask | 4);
        object[85] = 0;
        object[89] = 8;
        Engine_ActorSetSpriteFlags(object, 0);
        ObjectGroup_SetChildValue(object, 15);
        low = 0xfe;
        low = low & object[35];
        object[35] = (u8)(low | 2);
        result = object;
    } else {
        result = 0;
    }
    return result;
}
