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

void Func_080c0f98(s32 actor_id, s32 mode)
{
    struct EffectActorSlot *slot = Func_080b7dd0(actor_id);
    struct EffectActorObject *object;

    if (slot == 0)
        return;
    object = slot->object;
    if (object == 0)
        return;
    switch (object->kind & 15) {
    case 1:
    {
        register struct EffectActorRecord *record = object->records;
        record->mode_a = mode;
        record->mode_b = mode;
        break;
    }
    case 2:
    {
        register struct EffectActorRecord **records =
            (struct EffectActorRecord **)object->records;
        s32 i = 0;
        do {
            register struct EffectActorRecord *record = *records++;
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
