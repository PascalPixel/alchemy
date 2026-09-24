/* Draft, not exact (2026-09-24): candidate=432 reference=428 differing_halfwords=193.
   Battle: cycle the status icons of every actor (state 9 while the object
   is hidden) and set the sprite priority of each side from the camera
   angle. Open: the reference keeps both priorities and two hoisted masks
   on the stack (sp 44), indexes the icon loop without strength reduction,
   and compares the side loops against the count without folding i = 0. */
#include "TYPES.H"
#include "BATTLE_STATUS_ICON.H"

struct SpriteRecord {
    u8 unknown_00[9];
    u8 unknown_09_0 : 2;
    u8 priority : 2;
    u8 unknown_09_4 : 4;
};

struct ActorObject {
    u8 unknown_00[0x0c];
    s32 hidden;
    u8 unknown_10[0x40];
    void *records;
    u8 record_kind;
};

struct IconEffect {
    u8 unknown_00[6];
    u8 state;
};

struct IconContext {
    u8 unknown_00[0x25];
    u8 dirty;
};

struct ActorSlot {
    struct ActorObject *object;
    u8 unknown_04[0x20];
    struct IconEffect *icon_effect;
};

struct CameraWork {
    u8 unknown_00[0x36];
    s16 angle;
};

extern struct CameraWork *gCameraWork;

s32 BattleParty_ListActorIds(s32 groups, u16 *ids);
struct ActorSlot *GetBattleObjectSlot(s32 object_id);
struct IconContext *GetMotionRecord(struct ActorObject *object, s32 record_index);

void Func_080b7738(void)
{
    u16 ids[14];
    struct ActorSlot *slot;
    struct ActorObject *object;
    struct IconContext *context;
    struct SpriteRecord **records;
    s32 near_priority;
    s32 far_priority;
    s32 count;
    s32 state;
    s32 i;
    s32 j;

    BattleParty_ListActorIds(3, ids);
    for (i = 0; i <= 13 && ids[i] != 0xff; i++) {
        slot = GetBattleObjectSlot(ids[i]);
        if (slot == 0)
            continue;
        object = slot->object;
        BattleStatusIcon_Cycle((struct BattleStatusIconRecord *)slot);
        if (slot->icon_effect == 0)
            continue;
        context = GetMotionRecord(object, 0);
        if (context == 0)
            continue;
        state = 0;
        if (object->hidden != 0)
            state = 9;
        if (slot->icon_effect->state != state) {
            slot->icon_effect->state = state;
            context->dirty = 1;
        }
    }

    if (gCameraWork->angle >= 0) {
        near_priority = 1;
        far_priority = 2;
    } else {
        near_priority = 2;
        far_priority = 1;
    }

    count = BattleParty_ListActorIds(1, ids);
    for (i = 0; i < count; i++) {
        slot = GetBattleObjectSlot(ids[i]);
        if (slot == 0)
            continue;
        object = slot->object;
        switch (object->record_kind & 15) {
        case 1:
            ((struct SpriteRecord *)object->records)->priority = near_priority;
            break;
        case 2:
            records = object->records;
            for (j = 3; j >= 0; j--) {
                if (*records != 0)
                    (*records)->priority = near_priority;
                records++;
            }
            break;
        }
    }

    count = BattleParty_ListActorIds(2, ids);
    for (i = 0; i < count; i++) {
        slot = GetBattleObjectSlot(ids[i]);
        if (slot == 0)
            continue;
        object = slot->object;
        switch (object->record_kind & 15) {
        case 1:
            ((struct SpriteRecord *)object->records)->priority = far_priority;
            break;
        case 2:
            records = object->records;
            for (j = 3; j >= 0; j--) {
                if (*records != 0)
                    (*records)->priority = far_priority;
                records++;
            }
            break;
        }
    }
}
