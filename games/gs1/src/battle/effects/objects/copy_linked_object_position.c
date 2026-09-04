#include "metadata_lookup.h"
#include "types.h"

#define BattleEffect_CopyLinkedObjectPosition Func_0809376c

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s16 *Func_0808d394(s16);

s32 BattleEffect_CopyLinkedObjectPosition(void *obj) {
    void *link;

    link = FIELD_AT_OFFSET(obj, void **, 0x68);
    if (link != NULL) {
        FIELD_AT_OFFSET(obj, s8 *, 0x55) = 0;
        FIELD_AT_OFFSET(obj, s32 *, 8) = (s32) FIELD_AT_OFFSET(link, s32 *, 8);
        FIELD_AT_OFFSET(obj, s32 *, 0xC) = (s32) (FIELD_AT_OFFSET(link, s32 *, 0xC) + (FIELD_AT_OFFSET(Func_08185000(*Func_0808d394(FIELD_AT_OFFSET(obj, s16 *, 0x66))), s8 *, 8) << 0x10) + 0x80000);
        FIELD_AT_OFFSET(obj, s32 *, 0x14) = (s32) FIELD_AT_OFFSET(link, s32 *, 0x14);
        FIELD_AT_OFFSET(obj, s32 *, 0x10) = (s32) FIELD_AT_OFFSET(link, s32 *, 0x10);
    }
    return 0;
}
