#include "types.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Func_08077008(s32);
s32 Func_080b8808(u32 arg0);
void Func_08015118(void);
void Func_08015120(s32, s32);
void Func_080151c8(s32);

s32 Func_080b8888(s16 *script) {
    s32 object_id;
    s32 result;
    void *object;

    object_id = *script;
    object = Func_08077008(object_id);
    if (Func_080b8808(object_id) < 0) {
        return -1;
    }
    result = 0;
    if (FIELD(object, s16 *, 0x38) <= 0) {
        return result;
    }
    Func_08015118();
    Func_08015120(object_id, 1);
    Func_080151c8(0x816);
    return 0;
}
