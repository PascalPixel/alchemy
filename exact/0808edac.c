#include "effect_runtime.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080091a8(s32, s32, s32);
s32 Func_0808ec14();

void Func_0808edac(s32 unused, s32 x, s32 y) {
    s32 slot;
    s32 angle;
    s32 objectX;
    s32 objectY;
    void *slotData;
    void *slotBase;
    void *object;

    objectY = y;
    objectX = x;
    slot = Func_0808ec14();
    if (slot != -1) {
        slotBase = *(s32 *)0x03001EBC + (slot * 8);
        slotData = slotBase + 0x11C;
        object = M2C_FIELD(slotBase, void **, 0x11C);
        if (object != NULL) {
            if (objectX == -1) {
                objectX = (M2C_FIELD(slotData, u8 *, 6) << 0x14) + 0x80000;
            }
            if (objectY == -1) {
                objectY = (M2C_FIELD(slotData, u8 *, 7) << 0x14) + 0x80000;
            }
            M2C_FIELD(object, s32 *, 8) = objectX;
            M2C_FIELD(object, s32 *, 0x10) = objectY;
            angle = Func_080091a8(0, objectX, objectY);
            M2C_FIELD(object, s32 *, 0x14) = angle;
            M2C_FIELD(object, s32 *, 0xC) = angle;
        }
    }
}
