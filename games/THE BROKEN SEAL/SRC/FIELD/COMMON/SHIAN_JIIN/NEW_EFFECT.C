#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "TEMPLE.H"

void SetEffectMode(struct EffectWork *work, u32 mode)
{
    work->rec->f1 = mode;
}

void *NewEffectObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *overlay_object;
    void *object_record;
    s32 flags_mask;

    overlay_object = Object_Create(fourth, first, second, third);
    if (overlay_object != NULL) {
        object_record = FIELD_AT_OFFSET(overlay_object, void *, 0x50);
        flags_mask = -0xD;
        FIELD_AT_OFFSET(object_record, u8, 9) = (u8)(flags_mask & FIELD_AT_OFFSET(object_record, u8, 9));
        FIELD_AT_OFFSET(overlay_object, u8, 0x55) = 0;
        FIELD_AT_OFFSET(overlay_object, u8, 0x59) = 8;
        Engine_ActorSetSpriteFlags(overlay_object, 0);
        ObjectGroup_SetChildValue(overlay_object, 0xE);
        Engine_ObjectSetBlendMode(overlay_object, 1);
        return overlay_object;
    }
    return NULL;
}

void *NewFlippedEffectObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *result = Object_Create(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        Engine_ActorSetSpriteFlags(result, 0);
        ObjectGroup_SetChildValue(result, 15);
        result[0x23] = (result[0x23] & 0xfe) | 2;
        return result;
    }
    return NULL;
}
