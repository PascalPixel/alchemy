#include "BATTLE_EFFECT_RUNTIME.H"

struct BattleEffectVisual {
    u8 unknown_00[9];
    u8 flags;
    u8 unknown_0a[28];
    u8 value_26;
};

struct BattleEffectResource {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[60];
    struct BattleEffectVisual *visual;
};

struct BattleEffectLinkedObject {
    u8 unknown_00[80];
    struct BattleEffectVisual *visual;
    u8 value_54;
    u8 value_55;
    u8 unknown_56[14];
    u16 counter;
    u16 resource_id;
    struct BattleEffectResource *resource;
    void (*callback)(void);
};

struct BattleEffectLinkedObject *Object_CreateFar(
    s32 kind,
    s32 x,
    s32 y,
    s32 z);
void ObjectDispatch_InitializeFar(
    struct BattleEffectLinkedObject *object,
    const void *configuration);
void Object_SetMode(struct BattleEffectLinkedObject *object, s32 mode);
void Audio_PlayCue(s32 cue);
void Battle_WaitMode0(s32 state);
void BattleFx_CopyLinkedObjectPosition(void);
extern const u8 BattleFx_LinkedObjectScript[];

void BattleFx_SpawnLinked(
    s32 resource_id,
    s32 flags,
    s32 state)
{
    struct BattleEffectResource *resource;

    if ((flags & 0xff) == 6) {
        Audio_PlayCue(110);
    }

    resource = ObjectTable_Get(resource_id);
    if (resource != 0) {
        struct BattleEffectLinkedObject *object =
            Object_CreateFar(21, resource->x, resource->y, resource->z);

        if (object != 0) {
            ObjectDispatch_InitializeFar(object, BattleFx_LinkedObjectScript);
            Object_SetMode(object, flags & 15);
            object->value_55 = 0;
            object->counter = 0;
            object->resource_id = resource_id;
            object->callback = BattleFx_CopyLinkedObjectPosition;
            object->visual->value_26 = 0;
            object->resource = resource;

            if ((flags & 0x100) != 0) {
                s32 mask = 13;
                u8 visual_flags = object->visual->flags;

                mask = -mask;
                mask &= visual_flags;
                mask |= 4;
                object->visual->flags = mask;
            } else {
                s32 copied_flags = 12;
                u8 source_flags = resource->visual->flags;
                u8 flags;
                s32 clear_mask = 13;

                copied_flags &= source_flags;
                flags = object->visual->flags;
                clear_mask = -clear_mask;
                clear_mask &= flags;
                clear_mask |= copied_flags;
                object->visual->flags = clear_mask;
            }
        }
        Battle_WaitMode0(state);
    }
}
