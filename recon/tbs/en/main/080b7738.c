/* DRAFT (score 1175), rewritten 2026-10-02: same frame and same shape as the
 * ROM, 63 instructions differ, all register choices.
 * 1. In the two priority loops the ROM holds the object in r5 and, inside the
 *    four-record loop, reuses r5 for the shifted priority; here the object is
 *    in r0. With one object variable for the whole function the object is in
 *    r5, but the shifted priority is then set before the record list is read,
 *    conflicts with it and pushes the loop counter out of r7.
 * 2. In the icon scan the ROM leaves the motion record in r0 and loads the icon
 *    effect into r2; here they are the other way round, and the slot and the
 *    object trade r5 and r6.
 * Settled: the priorities are a two-word array copied into a variable for
 * each loop (which is why the ROM masks them with 3 and shares the masked
 * value between the two cases); the icon scan is not strength-reduced (kept
 * as a goto loop, tagged); the slot belongs to each loop; the four-record
 * loops are ascending loops the compiler reverses. Walking the record list
 * with a pointer lets the compiler reverse the outer loop, which the ROM
 * does not. */
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
    s32 priority[2];
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
                struct ActorObject *object = slot->object;

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
        priority[0] = 1;
        priority[1] = 2;
    } else {
        priority[0] = 2;
        priority[1] = 1;
    }
    {
        s32 value;

        count = BattleParty_ListActorIds(1, ids);
        value = priority[0];
        for (i = 0; i < count; i++) {
            struct ActorSlot *slot = GetBattleObjectSlot(ids[i]);

            if (slot != 0) {
                struct ActorObject *object = slot->object;

                switch (object->record_kind & 15) {
                case 1:
                    ((struct SpriteRecord *)object->records)->priority = value;
                    break;
                case 2:
                    for (j = 0; j < 4; j++) {
                        struct SpriteRecord *record = ((struct SpriteRecord **)object->records)[j];

                        if (record != 0)
                            record->priority = value;
                    }
                    break;
                }
            }
        }
    }
    {
        s32 value;

        count = BattleParty_ListActorIds(2, ids);
        value = priority[1];
        for (i = 0; i < count; i++) {
            struct ActorSlot *slot = GetBattleObjectSlot(ids[i]);

            if (slot != 0) {
                struct ActorObject *object = slot->object;

                switch (object->record_kind & 15) {
                case 1:
                    ((struct SpriteRecord *)object->records)->priority = value;
                    break;
                case 2:
                    for (j = 0; j < 4; j++) {
                        struct SpriteRecord *record = ((struct SpriteRecord **)object->records)[j];

                        if (record != 0)
                            record->priority = value;
                    }
                    break;
                }
            }
        }
    }
}
