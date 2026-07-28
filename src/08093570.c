#include "types.h"
#include "object_effect.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_080048f4(s32 arg0, s32 arg1);
void Func_08093570(void *target, s32 keep_current_position) {
    void *object;

    object = M2C_FIELD(Func_080048f4(0x1B, 0xCCC), void **, 0x1E0);
    if (target != NULL) {
        Func_080090e0(object, NULL);
        M2C_FIELD(object, void **, 0x68) = target;
        if (keep_current_position == 0) {
            M2C_FIELD(object, s32 *, 8) = M2C_FIELD(target, s32 *, 8);
            M2C_FIELD(object, s32 *, 0xC) = M2C_FIELD(target, s32 *, 0xC);
            M2C_FIELD(object, s32 *, 0x10) = M2C_FIELD(target, s32 *, 0x10);
        }
    }
}
