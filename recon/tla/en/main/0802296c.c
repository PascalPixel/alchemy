#include "TYPES.H"
#include "METADATA_LOOKUP.H"
extern u8 gMenuCtrlWork[];

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Animation_LookupValueByKey(s32 key);

void Animation_InitWorkFromMetadata(void *work)
{
    s32 value;
    s32 z;
    struct AnimationMetadata *info;

    if (work != NULL) {
        info = Resource_GetMetadataRecordFar(FIELD_AT_OFFSET(work, s16, 0));
        if (info->width != 0) {
            value = info->frames;
            if (value == 0) {
                value = Animation_LookupValueByKey(FIELD_AT_OFFSET(work, s16, 0));
            }
            FIELD_AT_OFFSET(work, u8, 4) = info->draw_kind;
            FIELD_AT_OFFSET(work, s32, 0x0c) = info->animation;
            FIELD_AT_OFFSET(work, s32, 8) = value;
            FIELD_AT_OFFSET(work, u8, 7) = info->frame_codec;
            z = 0;
            FIELD_AT_OFFSET(work, u8, 0x16) = 0xff;
            FIELD_AT_OFFSET(work, s32, 0x10) = z;
            FIELD_AT_OFFSET(work, u8, 0x14) = z;
        }
    }
}
