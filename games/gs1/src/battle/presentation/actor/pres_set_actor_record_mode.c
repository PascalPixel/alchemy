#include "types.h"
#include "scene.h"

/* battle/presentation/actor/set_record_mode.c */
struct EffectActorRecord {
    u8 reserved_00[5];
    u8 reserved_a : 2;
    u8 mode_a : 2;
    u8 upper_a : 4;
    u8 reserved_06[11];
    u8 reserved_b : 2;
    u8 mode_b : 2;
    u8 upper_b : 4;
};

struct EffectActorObject {
    u8 reserved_00[80];
    struct EffectActorRecord *records;
    u8 kind;
};

struct EffectActorSlot {
    struct EffectActorObject *object;
};

struct EffectActorSlot *Actor_Run(s32);

void BattlePres_SetActorRecordMode(s32 actor_id, s32 mode)
{
    struct EffectActorSlot *slot = Actor_Run(actor_id);
    struct EffectActorObject *object;
    struct EffectActorRecord *record;
    struct EffectActorRecord **records;
    s32 i;

    if (slot == 0)
        return;
    object = slot->object;
    if (object == 0)
        return;
    switch (object->kind & 15) {
    case 1:
    {
        record = object->records;
        record->mode_a = mode;
        record->mode_b = mode;
        break;
    }
    case 2:
    {
        records = (struct EffectActorRecord **)object->records;
        i = 0;
        do {
            record = *records++;
            if (record == 0)
                break;
            record->mode_a = mode;
            record->mode_b = mode;
            i++;
        } while (i <= 3);
        break;
    }
    }
}

/* battle/presentation/actor/set_other_modes.c */
void BattlePres_SetOtherActorRecordModes(s32 excluded)
{
    s16 values[14];
    s32 count = Actor_Apply(3, values);
    s32 index;

    for (index = 0; index < count; index++) {
        if (values[index] != excluded)
            Actor_Apply2(values[index], 1);
    }
}

/* battle/presentation/actor/clear_modes.c */
void BattlePres_ClearAllActorRecordModes(void)
{
    s16 values[14];
    s32 count = Actor_Apply(3, values);
    s32 index;

    for (index = 0; index < count; index++)
        Actor_Apply2(values[index], 0);
}
