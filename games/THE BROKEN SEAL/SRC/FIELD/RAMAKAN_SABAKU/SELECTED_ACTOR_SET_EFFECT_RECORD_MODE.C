#include "TYPES.H"

/*
 * Ramakan desert: set the mode on the selected actor's effect record. The
 * 22-byte head of the record is proved identical for this overlay family, so
 * the mode bitfield lands at offset 9.
 */

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

#define SetEffectRecordMode Func_02000030

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}
