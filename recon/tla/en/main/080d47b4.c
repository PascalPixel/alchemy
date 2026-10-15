/* Near miss of Func_080d47b4, differ 8 of 78 (was 28): The Broken Seal's BattleFx_SpawnLinked
 * with this game's object kind (488), the visual's byte at 26 and the object returned. Left,
 * instruction order: the native loads the visual (ldr r0, [r5, #80]) before it starts the
 * 0x100 mask, stores the resource before the visual's byte, and in the copy branch reads the
 * new object's flags before the source's. */
#include "objdisp.h"

struct BattleEffectVisual {
    u8 unk00[9];
    u8 flags;
    u8 unk0a[16];
    u8 value26;
};

struct BattleEffectResource {
    u8 unk00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unk14[60];
    struct BattleEffectVisual *visual;
};

struct BattleEffectLinkedObject {
    u8 unk00[80];
    struct BattleEffectVisual *visual;
    u8 value54;
    u8 value55;
    u8 unk56[14];
    u16 counter;
    u16 resourceId;
    struct BattleEffectResource *resource;
    void (*callback)(void);
};

struct BattleEffectLinkedObject *Object_CreateFar(
    s32 kind,
    s32 x,
    s32 y,
    s32 z);
void Object_SetMode(struct BattleEffectLinkedObject *object, s32 mode);
void Audio_PlayCue(s32 cue);
void EventRuntime_Wait(s32 state);
void BattleFx_CopyLinkedObjectPosition(void);
extern const u8 BattleFx_LinkedObjectScript[];

struct BattleEffectLinkedObject *BattleFx_SpawnLinked(
    s32 resourceId,
    s32 flags,
    s32 state)
{
    struct BattleEffectResource *resource;
    struct BattleEffectLinkedObject *object;

    if ((flags & 0xff) == 6) {
        Audio_PlayCue(110);
    }

    resource = ObjectTable_Get(resourceId);
    if (resource != 0) {
        object =
            Object_CreateFar(488, resource->x, resource->y, resource->z);

        if (object != 0) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_LinkedObjectScript);
            Object_SetMode(object, flags & 15);
            object->value55 = 0;
            object->counter = 0;
            object->resourceId = resourceId;
            object->callback = BattleFx_CopyLinkedObjectPosition;
            object->visual->value26 = 0;
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
        EventRuntime_Wait(state);
    }
    return object;
}
