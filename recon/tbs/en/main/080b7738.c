#include "BATTLE_PRESENTATION.H"
#include "BATTLE_PARTY.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"
#include "TYPES.H"
#include "BATTLE_STATUS_ICON.H"

void Func_080b7738(void)
{
    u16 ids[14];
    s32 priority[2];
    s32 i;
    s32 j;
    s32 count;

    BattleParty_ListActorIds(BATTLE_SIDE_BOTH, ids);
    /* FAKEMATCH: the scan is rotated by hand with a goto, which keeps the loop
     * pass off it; as a for loop it becomes a pointer walk. */
    i = 0;
    if (ids[i] != BATTLE_UNIT_LIST_END) {
again:
        {
            struct BattleObjectSlot *slot = GetBattleObjectSlot(ids[i]);

            if (slot != 0) {
                struct MotionObject *object = slot->object;

                BattleStatusIcon_Cycle(slot);
                if (slot->effect_entry != 0) {
                    struct AnimationObject *context = GetMotionRecord(object, 0);

                    if (context != 0) {
                        struct SpriteEntry *effect;
                        s32 state = 0;

                        if (object->y != 0)
                            state = 9;
                        effect = slot->effect_entry;
                        if (effect->priority != state) {
                            effect->priority = state;
                            context->dirty = 1;
                        }
                    }
                }
            }
        }
        i++;
        if (i <= 13 && ids[i] != BATTLE_UNIT_LIST_END)
            goto again;
    }
    if ((s16)gCameraWork->yaw >= 0) {
        priority[0] = 1;
        priority[1] = 2;
    } else {
        priority[0] = 2;
        priority[1] = 1;
    }
    {
        s32 value;

        count = BattleParty_ListActorIds(BATTLE_SIDE_PARTY, ids);
        value = priority[0];
        for (i = 0; i < count; i++) {
            struct BattleObjectSlot *slot = GetBattleObjectSlot(ids[i]);

            if (slot != 0) {
                struct MotionObject *object = slot->object;

                switch (object->record_storage_kind & 15) {
                case 1:
                {
                    struct AnimationObject *record = object->records;
                    record->part[0].priority = value;
                    break;
                }
                case 2:
                    for (j = 0; j < 4; j++) {
                        struct AnimationObject *record = ((struct AnimationObject **)object->records)[j];

                        if (record != 0) {
                            record->part[0].priority = value;
                        }
                    }
                    break;
                }
            }
        }
    }
    {
        s32 value;

        count = BattleParty_ListActorIds(BATTLE_SIDE_ENEMIES, ids);
        value = priority[1];
        for (i = 0; i < count; i++) {
            struct BattleObjectSlot *slot = GetBattleObjectSlot(ids[i]);

            if (slot != 0) {
                struct MotionObject *object = slot->object;

                switch (object->record_storage_kind & 15) {
                case 1:
                {
                    struct AnimationObject *record = object->records;
                    record->part[0].priority = value;
                    break;
                }
                case 2:
                    for (j = 0; j < 4; j++) {
                        struct AnimationObject *record = ((struct AnimationObject **)object->records)[j];

                        if (record != 0) {
                            record->part[0].priority = value;
                        }
                    }
                    break;
                }
            }
        }
    }
}
