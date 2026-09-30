#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_EFFECT_RUNTIME.H"

extern u8 gCam[];
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
s32 WaitFrames(s32 frames);

s16 *BattleAction_FindDescriptor(s16 action);
void *Resource_GetMetadataRecordFar(s16 id);

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
extern const u8 BattleFx_LinkedObjectScript[];

s32 BattleFx_CopyLinkedObjectPosition(void *obj);

/* Wait (at most 300 frames) for the display work's pending flag at +0x358 to clear. */
void Event_WaitForDisplayField358Clear(void)
{
    s32 frames;
    u8 *work = *(u8 **)((u32)&gCam);

    if (*(s16 *)((u8 *)Runtime_AllocateBlock(0x1b, 0xccc) + 0x19e) == 3) {
        frames = 0;
        if (*(s16 *)(work + 0x358) != 0) {
            do {
                WaitFrames(1);
                frames++;
            } while (frames <= 0x12b && *(s16 *)(work + 0x358) != 0);
        }
    }
}

/* Snap an effect object onto the object it is linked to, raised by the
   linked descriptor's height. */
s32 BattleFx_CopyLinkedObjectPosition(void *obj)
{
    void *link;

    link = FIELD_AT_OFFSET(obj, void **, 0x68);
    if (link != NULL) {
        FIELD_AT_OFFSET(obj, s8 *, 0x55) = 0;
        FIELD_AT_OFFSET(obj, s32 *, 8) = FIELD_AT_OFFSET(link, s32 *, 8);
        FIELD_AT_OFFSET(obj, s32 *, 0xc) = FIELD_AT_OFFSET(link, s32 *, 0xc)
            + (FIELD_AT_OFFSET(Resource_GetMetadataRecordFar(*BattleAction_FindDescriptor(FIELD_AT_OFFSET(obj, s16 *, 0x66))), s8 *, 8) << 16)
            + 0x80000;
        FIELD_AT_OFFSET(obj, s32 *, 0x14) = FIELD_AT_OFFSET(link, s32 *, 0x14);
        FIELD_AT_OFFSET(obj, s32 *, 0x10) = FIELD_AT_OFFSET(link, s32 *, 0x10);
    }
    return 0;
}

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
