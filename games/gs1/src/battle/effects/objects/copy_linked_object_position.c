#include "metadata_lookup.h"
#include "types.h"
#include "scene.h"
#include "abi/battle/effects/objects/copy_linked_object_position.h"


s16 *Battle_Run(s16);

s32 BattleFx_CopyLinkedObjectPosition(void *obj)
{
    void *link;

    link = FIELD_AT_OFFSET(obj, void **, 0x68);
    if (link != NULL) {
        FIELD_AT_OFFSET(obj, s8 *, 0x55) = 0;
        FIELD_AT_OFFSET(obj, s32 *, 8) = (s32)FIELD_AT_OFFSET(link, s32 *, 8);
        FIELD_AT_OFFSET(obj, s32 *, 0xC) = (s32)(FIELD_AT_OFFSET(link, s32 *, 0xC) + (FIELD_AT_OFFSET(Battle_Run2(*Battle_Run(FIELD_AT_OFFSET(obj, s16 *, 0x66))), s8 *, 8) << 0x10) + 0x80000);
        FIELD_AT_OFFSET(obj, s32 *, 0x14) = (s32)FIELD_AT_OFFSET(link, s32 *, 0x14);
        FIELD_AT_OFFSET(obj, s32 *, 0x10) = (s32)FIELD_AT_OFFSET(link, s32 *, 0x10);
    }
    return 0;
}
