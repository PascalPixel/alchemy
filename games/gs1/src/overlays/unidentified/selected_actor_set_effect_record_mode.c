#include "types.h"

/* Shared 22-byte head leaf proved identical for this overlay family. */
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
