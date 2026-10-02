/* DRAFT (score 2343), rewritten 2026-10-02: 74 instructions differ, from two
 * causes.
 * 1. The ROM keeps both sprite priorities on the stack (sp+8, sp+12) and masks
 *    each with 3 before shifting it into the record; here the near one stays in
 *    r5 and, both being known to be 1 or 2, the mask is dropped. As a two-word
 *    array they are on the stack and masked, but then the second case reloads
 *    the word where the ROM reuses the masked value it holds in r11.
 * 2. In the icon scan the ROM leaves the motion record in r0 and loads the icon
 *    effect into r2; here they are the other way round.
 * Settled: the icon scan is not strength-reduced in the ROM (kept as a goto
 * loop, tagged); the slot is a variable of each loop, not of the function;
 * the four-record loops are ascending loops the compiler reverses. */
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
    s32 near_priority;
    s32 far_priority;
    struct ActorObject *object;
    s32 i;
    s32 j;
    s32 count;

    BattleParty_ListActorIds(3, ids);
    /* FAKEMATCH: the scan is rotated by hand with a goto, which keeps the loop
     * pass off it; as a for loop it becomes a pointer walk. */
    i = 0;
    if (ids[i] != 0xff) {
again:
        {
            struct ActorSlot *slot = GetBattleObjectSlot(ids[i]);

            if (slot != 0) {
                object = slot->object;
                BattleStatusIcon_Cycle((struct BattleStatusIconRecord *)slot);
                if (slot->icon_effect != 0) {
                    struct IconContext *context = GetMotionRecord(object, 0);

                    if (context != 0) {
                        struct IconEffect *effect;
                        s32 state = 0;

                        if (object->hidden != 0)
                            state = 9;
                        effect = slot->icon_effect;
                        if (effect->state != state) {
                            effect->state = state;
                            context->dirty = 1;
                        }
                    }
                }
            }
        }
        i++;
        if (i <= 13 && ids[i] != 0xff)
            goto again;
    }
    if (gCameraWork->angle >= 0) {
        near_priority = 1;
        far_priority = 2;
    } else if (gCameraWork->angle < 0) {
        near_priority = 2;
        far_priority = 1;
    }
    count = BattleParty_ListActorIds(1, ids);
    for (i = 0; i < count; i++) {
        struct ActorSlot *slot = GetBattleObjectSlot(ids[i]);

        if (slot != 0) {
            object = slot->object;
            switch (object->record_kind & 15) {
            case 1:
                ((struct SpriteRecord *)object->records)->priority = near_priority;
                break;
            case 2:
                for (j = 0; j < 4; j++) {
                    struct SpriteRecord *record = ((struct SpriteRecord **)object->records)[j];

                    if (record != 0)
                        record->priority = near_priority;
                }
                break;
            }
        }
    }
    count = BattleParty_ListActorIds(2, ids);
    for (i = 0; i < count; i++) {
        struct ActorSlot *slot = GetBattleObjectSlot(ids[i]);

        if (slot != 0) {
            object = slot->object;
            switch (object->record_kind & 15) {
            case 1:
                ((struct SpriteRecord *)object->records)->priority = far_priority;
                break;
            case 2:
                for (j = 0; j < 4; j++) {
                    struct SpriteRecord *record = ((struct SpriteRecord **)object->records)[j];

                    if (record != 0)
                        record->priority = far_priority;
                }
                break;
            }
        }
    }
}
