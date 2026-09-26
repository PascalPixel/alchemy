/* Draft, not exact (2026-09-26): 828 of 832 bytes, 189 differing halfwords.
   Scene ownership, slot records and resource lifetime replace raw offsets.
   The 44-byte frame and early pool now line up. Remaining: chained store
   order, loop/pointer lifetime, four missing bytes and later pools.
   The original 832-byte model differed in 305 halfwords. */
#include "TYPES.H"
#include "EFFECT_0809B11C.H"
#include "BATTLE_EFFECT_RUNTIME.H"

struct BurstPosition { s32 x, y, z; };

struct BurstResource {
    u8 unknown_00[28];
    u8 id;
};

struct BurstObject {
    void *script;
    s16 step;
    u16 heading;
    struct BurstPosition pos;
    s32 terrain_height;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[8];
    s32 velocity_z;
    s32 unknown_2c;
    s32 speed_limit;
    s32 acceleration;
    u8 unknown_38[24];
    struct BurstResource *sprite;
    u8 unknown_54;
    u8 mode;
};

struct BurstScene {
    s32 angle;
    struct BurstPosition pos;
    struct BurstObject *main_object;
    struct BurstObject *child;
    u8 unknown_18[8];
    s8 use_main_object_origin;
    u8 unknown_21[19];
    s8 enlarge_child;
    s8 preserve_child_motion;
    u8 unknown_36[34];
    struct EffectSlot slots[12];
};

void WaitFrames(s32 frames);
void Resource_ResetEntry(s32 slot);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct BurstPosition *pos);
void Object_SetMode(struct BurstObject *object, s32 mode);
void ObjectDispatch_InitializeFar(struct BurstObject *object, s32 script);
void Object_SetPosition(struct BurstObject *object, s32 x, s32 y, s32 z);
s32 Object_CheckMovementCollision(struct BurstObject *object, struct BurstPosition *pos);
void Animation_ApplyChildValuesFar(struct BurstObject *object, s32 value);
void ObjectGroup_SetChildValueUnlessFifteenFar(s32 object, s32 value);
s32 Func_08009250(struct BurstObject *object, struct BurstPosition *pos);
s32 BattleFx_FindMatchingEvent(s32 flags, s32 group, s32 *context);
s32 BattleFx_RunEventAction(void *event, s32 object, s32 context);
struct BurstResource *Object_ReplaceResourceEntry(struct BurstResource *sprite, struct BurstResource *resource);
struct BurstObject *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void BattleEffect_InitializeSharedScene(void);
void BattleFx_PrepareBufferInterpolation(void);
void Camera_WorldToScreen(struct BurstPosition *pos);
void EffectSlot_Initialize(struct EffectSlot *slot, s32 kind, s32 x, s32 z);
void BattleFx_UpdateRadialBurst(struct EffectSlot *slot);
void Audio_PlayCue(s32 cue);

/* Twelve screen-space particles precede the main burst and its three
   copies. All copies share one resource entry until their scripts finish. */
void RunBattleEffect04(void)
{
    struct BurstObject *child;
    s32 event_context;
    struct BurstObject *spawned[4];
    struct BurstPosition pos;
    struct BurstObject **spawn_start;
    struct BurstObject **write;
    struct BurstObject **read;
    struct BurstObject *object;
    struct BurstObject *copy;
    struct BurstObject *target;
    struct BurstScene *scene;
    struct EffectSlot *slot;
    struct BurstResource *resource;
    s32 event;
    s32 scale;
    s32 index;
    u8 resource_id;
    u16 zero;

    scene = *(struct BurstScene **)0x03001f30;
    child = scene->child;
    BattleEffect_InitializeSharedScene();
    Audio_PlayCue(0x82);
    slot = scene->slots;
    index = 11;
    do {
        target = scene->main_object;
        pos.x = target->pos.x;
        pos.y = target->pos.y + 0x100000;
        pos.z = target->pos.z;
        Camera_WorldToScreen(&pos);
        EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
        EffectSlot_SetCallback(slot, BattleFx_UpdateRadialBurst);
        EffectSlot_SetObjectMode(slot, 7);
        ObjectGroup_SetChildValueUnlessFifteenFar((s32)slot->object, 9);
        slot->scale_y = slot->scale_x = 0xb333;
        WaitFrames(2);
        index--;
        slot++;
    } while (index >= 0);

    target = scene->main_object;
    pos.x = target->pos.x;
    pos.y = target->pos.y + 0x100000;
    pos.z = target->pos.z;
    Vector_AddPolarOffset(0x80000, scene->angle, &pos);
    object = Object_Spawn(0xd7, pos.x, pos.y, pos.z);
    if (object == NULL) {
        BattleFx_PrepareBufferInterpolation();
        return;
    }
    object->scale_y = object->scale_x = 0x4000;
    object->heading = scene->angle;
    object->speed_limit = object->acceleration = 0x40000;
    zero = 0;
    object->mode = zero;
    Object_SetMode(object, 5);
    Animation_ApplyChildValuesFar(object, 3);
    if (object->scale_x < 0x10000) {
        do {
            scale = object->scale_x + 0x500;
            object->scale_y = object->scale_x = scale;
            WaitFrames(1);
        } while (object->scale_x <= 0xffff);
    }
    WaitFrames(3);
    spawn_start = spawned;
    resource = NULL;
    index = 2;
    write = &spawned[2];
    do {
        copy = Object_Spawn(0xd7, object->pos.x, object->pos.y, object->pos.z);
        *write-- = copy;
        if (copy != NULL) {
            copy->scale_y = copy->scale_x = 0xf000;
            copy->heading = scene->angle;
            copy->speed_limit = copy->acceleration = 0x40000;
            copy->mode = 0;
            Object_SetMode(copy, 5);
            Animation_ApplyChildValuesFar(copy, 2);
            resource = Object_ReplaceResourceEntry(copy->sprite, resource);
        }
        index--;
    } while (index >= 0);
    resource_id = resource->id;
    if (scene->use_main_object_origin != 0) {
        target = scene->main_object;
        pos.x = target->pos.x;
        pos.y = target->pos.y + 0x100000;
        pos.z = target->pos.z;
        Vector_AddPolarOffset(0x380000, scene->angle, &pos);
    } else {
        pos.x = scene->pos.x;
        pos.y = scene->pos.y + 0x100000;
        pos.z = scene->pos.z;
    }
    Object_SetPosition(object, pos.x, pos.y, pos.z);
    ObjectDispatch_InitializeFar(object, 0x0809f12c);
    read = spawn_start;
    index = 2;
    do {
        copy = *read++;
        if (copy != NULL) {
            WaitFrames(3);
            Object_SetPosition(copy, pos.x, pos.y, pos.z);
            ObjectDispatch_InitializeFar(copy, 0x0809f0b4);
        }
        index--;
    } while (index >= 0);
    index = 0;
    while (object->script != NULL) {
        WaitFrames(1);
        index++;
        if (index > 59)
            break;
    }
    if (child != NULL && scene->preserve_child_motion == 0) {
        if (scene->enlarge_child != 0)
            child->velocity_z = 0x80000;
        pos.x = child->pos.x;
        pos.y = child->pos.y;
        pos.z = child->pos.z;
        Vector_AddPolarOffset(0x100000, scene->angle, &pos);
        if (Object_CheckMovementCollision(child, &pos) == 0 && Func_08009250(child, &pos) == 0) {
            child->acceleration = child->speed_limit = 0x10000;
            Object_SetPosition(child, pos.x, pos.y, pos.z);
        }
    }
    event = BattleFx_FindMatchingEvent(0x50000005, 4, &event_context);
    if (event != 0)
        BattleFx_RunEventAction((void *)event, Data_02000240.object_id, event_context);
    WaitFrames(10);
    BattleFx_PrepareBufferInterpolation();
    WaitFrames(20);
    if (resource_id != 96)
        Resource_ResetEntry(resource_id);
}
