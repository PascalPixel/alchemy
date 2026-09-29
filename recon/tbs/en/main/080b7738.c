/* Draft, not exact (2026-09-28): 420 of 428 bytes, 104 aligned edits (was
   432 bytes / 142 edits). Battle: cycle the status icons of every actor
   (state 9 while the object is hidden) and set the sprite priority of each
   side from the camera angle.
   The icon loop is an if/goto loop: loop.c leaves it alone, so it indexes
   ids[i * 2] afresh each pass exactly as the reference does (the for, while
   and do/while spellings are all strength-reduced). Taking each child record
   with *records++ gives the reference's ldmia walk.
   Open: the reference keeps both priorities and two hoisted masks on the
   stack (frame 44 against 32 here) and does not reverse the two side loops
   (it compares i against the count, unfolded at i = 0); here loop.c's second
   pass reverses them into a count-down.
   2026-09-29 (alchemy permute scorer): the draft scored 5064 (44
   register-only, 4 stack-only, 21 operand, 10 reordered, 17 inserted, 21
   deleted). This body is the permuter's best after a 300-second search
   (45,000 candidates): 2968 (39 register-only, 3 stack-only, 6 operand, 9
   reordered, 11 inserted, 10 deleted). Many of its 73 rewrites are noise;
   the 44-byte frame (40 here) and the unreversed side loops remain.
 */
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
    s32 near_priority;
    struct IconContext *context;
    register s32 far_priority;
    struct SpriteRecord **records;
    s32 i;
    s32 count;
    s32 j;
    s32 state;
    s32 tmp8;

    i = 0;
    BattleParty_ListActorIds(3, ids);
    if (0xff != ids[i]) {
    loop:
        slot = GetBattleObjectSlot(ids[i]);
        if (slot != 0) {
            struct BattleStatusIconRecord *tmp7;
            struct ActorObject *tmp9;
            tmp9 = slot->object;
            tmp7 = (struct BattleStatusIconRecord *)slot;
            BattleStatusIcon_Cycle(tmp7);
            object = tmp9;
            if (slot->icon_effect != 0) {
                if (context = GetMotionRecord(object, 0)) {
                    s32 tmp;
                    u8 tmp5;
                    s32 tmp6;
                    state = 0;
                    if (object->hidden)
                        state = 9;
                    tmp5 = slot[0].icon_effect->state;
                    if (tmp6 = (tmp = tmp5 != state) != 0) {
                        slot->icon_effect->state = state;
                        context->dirty = 1;
                    }
                }
            }
        }
        if (13 >= ++i && ids[i] != 0xff)
            goto loop;
    }
    if (0 <= gCameraWork->angle) {
        near_priority = 1;
        far_priority = 2;
    } else {
        far_priority = 1;
        near_priority = 2;
    }
    count = BattleParty_ListActorIds(1, ids);
    for (i = 0; i < count; i++) {
        s32 tmp4;
        s32 tmp2;
        slot = GetBattleObjectSlot(ids[i]);
        tmp2 = slot == 0;
        tmp4 = tmp2;
        if (tmp4)
            continue;
        object = slot->object;
        switch (object->record_kind & 15) {
        case 1:
            ((struct SpriteRecord *)object->records)->priority = near_priority;
            break;
        case 2:
            records = object->records;
            j = 3;
            if (0 <= j) {
                while (1) {
                    struct SpriteRecord *record = records++[0];
                    if (0 != record)
                        record->priority = near_priority;
                    j--;
                    if (0 > j)
                        break;
                }
            }
            break;
        }
    }
    tmp8 = BattleParty_ListActorIds(2, ids);
    count = tmp8;
    for (i = 0; i < count; i++) {
        s32 tmp3;
        slot = GetBattleObjectSlot(ids[i]);
        tmp3 = slot == 0;
        if (tmp3)
            continue;
        object = slot->object;
        switch (object->record_kind & 15) {
        case 1:
            ((struct SpriteRecord *)object->records)->priority = far_priority;
            break;
        case 2:
            records = object->records;
            j = 3;
            if (j >= 0) {
                while (1) {
                    struct SpriteRecord *record = *records++;
                    if (0 != record)
                        record->priority = far_priority;
                    --j;
                    if (0 > j)
                        break;
                }
            }
            break;
        }
    }
}
