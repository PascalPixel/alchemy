#include "scene.h"
#include "abi/effect_runtime/set_current_position.h"
#include "effect_runtime.h"
#include "global_cells.h"


s32 EffectRuntime_FindSlotByObject();

void EffectRuntime_SetCurrentPosition(s32 unused, s32 x, s32 y)
{
    s32 slot;
    s32 angle;
    s32 objectX;
    s32 objectY;
    void *slotData;
    void *slotBase;
    void *object;

    objectY = y;
    objectX = x;
    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        slotBase = *(s32 *)ADDR_03001EBC + (slot * 8);
        slotData = slotBase + 0x11C;
        object = FIELD_AT_OFFSET(slotBase, void **, 0x11C);
        if (object != NULL) {
            if (objectX == -1) {
                objectX = (FIELD_AT_OFFSET(slotData, u8 *, 6) << 0x14) + 0x80000;
            }
            if (objectY == -1) {
                objectY = (FIELD_AT_OFFSET(slotData, u8 *, 7) << 0x14) + 0x80000;
            }
            FIELD_AT_OFFSET(object, s32 *, 8) = objectX;
            FIELD_AT_OFFSET(object, s32 *, 0x10) = objectY;
            angle = Sys_Place(0, objectX, objectY);
            FIELD_AT_OFFSET(object, s32 *, 0x14) = angle;
            FIELD_AT_OFFSET(object, s32 *, 0xC) = angle;
        }
    }
}
