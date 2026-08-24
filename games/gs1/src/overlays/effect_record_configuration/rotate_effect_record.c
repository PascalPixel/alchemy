#include "types.h"

typedef struct {
    u8 pad_to_angle[30];
    u16 angle;
} EffectRecord;

typedef struct {
    u8 pad_to_record[80];
    EffectRecord *record;
} EffectWork;

#define RotateEffectRecord Func_020004c4

void RotateEffectRecord(EffectWork *work)
{
    EffectRecord *record = work->record;

    record->angle -= 0x800;
}
