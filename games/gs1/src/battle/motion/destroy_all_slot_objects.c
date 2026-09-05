#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Object_Destroy(s32);
void *GetBattleObjectSlot(s32 arg0);

void *BattleMotion_DestroyAllSlotObjects(void) {
    s32 no;
    s32 i;
    void *slot;

    i = 0;
    do {
        no = i + 0x78;
        if (i <= 7) {
            no = i;
        }
        slot = GetBattleObjectSlot(no);
        if ((slot != NULL) && (FIELD_AT_OFFSET(slot, s16 *, 0x28) != 0)) {
            Object_Destroy(FIELD_AT_OFFSET(slot, s32 *, 0));
            FIELD_AT_OFFSET(slot, s32 *, 0) = 0;
            FIELD_AT_OFFSET(slot, s16 *, 0x28) = 0;
        }
        i += 1;
    } while (i <= 0xD);
}
