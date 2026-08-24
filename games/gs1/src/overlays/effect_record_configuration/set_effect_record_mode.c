#include "types.h"

/*
 * resource_38a owner at 0x02000030, 22 bytes. Leaf, no frame.
 *
 * Sets the two-bit mode field of the record the caller's +80 pointer names.
 * The bitfield assignment preserves the reference's full-width mask shape.
 */

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;                /* +9, bits 2..3 */
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record; /* +80 */
};

#define SetEffectRecordMode Func_02000030

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}
