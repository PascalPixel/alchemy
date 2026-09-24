#include "TYPES.H"
#include "SCENE.H"

s16 *BattleAction_FindDescriptor(s16 action);
void *Resource_GetMetadataRecordFar(s16 id);

/* Snap an effect object onto the object it is linked to, raised by the
   linked descriptor's height. */
s32 BattleFx_CopyLinkedObjectPosition(void *obj)
{
    void *link;

    link = FIELD_AT_OFFSET(obj, void **, 0x68);
    if (link != NULL) {
        FIELD_AT_OFFSET(obj, s8 *, 0x55) = 0;
        FIELD_AT_OFFSET(obj, s32 *, 8) = FIELD_AT_OFFSET(link, s32 *, 8);
        FIELD_AT_OFFSET(obj, s32 *, 0xc) = FIELD_AT_OFFSET(link, s32 *, 0xc)
            + (FIELD_AT_OFFSET(Resource_GetMetadataRecordFar(*BattleAction_FindDescriptor(FIELD_AT_OFFSET(obj, s16 *, 0x66))), s8 *, 8) << 16)
            + 0x80000;
        FIELD_AT_OFFSET(obj, s32 *, 0x14) = FIELD_AT_OFFSET(link, s32 *, 0x14);
        FIELD_AT_OFFSET(obj, s32 *, 0x10) = FIELD_AT_OFFSET(link, s32 *, 0x10);
    }
    return 0;
}
