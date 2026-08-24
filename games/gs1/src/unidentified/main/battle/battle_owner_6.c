#include "types.h"
#include "battle_msg.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Runtime_GetObject(s32);
s32 Func_080b8808(u32 arg0);
void Func_08015118(void);
void UiText_DrawQuantity(s32, s32);
void UiText_ShowMessageAndWait(s32);

s32 Func_080b8888(s16 *script) {
    s32 object_id;
    s32 result;
    void *object;

    object_id = *script;
    object = Runtime_GetObject(object_id);
    if (Func_080b8808(object_id) < 0) {
        return -1;
    }
    result = 0;
    if (FIELD(object, s16 *, 0x38) <= 0) {
        return result;
    }
    Func_08015118();
    UiText_DrawQuantity(object_id, 1);
    UiText_ShowMessageAndWait((s32)&Value_00000816);
    return 0;
}
