/*
 * Draft: Animation_SetWorkEntry does not yet match; 2 halfwords differ from ☀️'s C, first at +0x26 (str r2, [r5, #16]).
 * Links as recon/tla/raw/08022a24.s.
 */
#include "TYPES.H"
#include "METADATA_LOOKUP.H"


#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Animation_SetWorkEntry(void *work, s32 no)
{
    s32 hi;
    struct AnimationMetadata *info;
    s32 value;

    hi = 0x80 & no;
    if (FIELD_AT_OFFSET(work, s32, 0x0c) != 0) {
        info = Resource_GetMetadataRecordFar((s32)FIELD_AT_OFFSET(work, s16, 0));
        if (no < (s32)info->animation_count) {
            value = *(s32 *)((u8 *)FIELD_AT_OFFSET(work, s32, 0x0c) + (no * 4));
            FIELD_AT_OFFSET(work, u8, 4) = (u8)info->draw_kind;
            FIELD_AT_OFFSET(work, s32, 0x10) = value;
            FIELD_AT_OFFSET(work, s8, 0x15) = 0x10;
            if (hi == 0) {
                FIELD_AT_OFFSET(work, s8, 0x14) = hi;
                FIELD_AT_OFFSET(work, s16, 2) = (s16)hi;
            }
        }
    }
}
