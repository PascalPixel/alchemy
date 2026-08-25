#include "types.h"

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

struct EffectActorSlot *Func_080b7dd0(s32);

#define BattlePresentation_SetActorRecordMode Func_080c0f98

void BattlePresentation_SetActorRecordMode(s32 actor_id, s32 mode)
{
    struct EffectActorSlot *slot = Func_080b7dd0(actor_id);
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
