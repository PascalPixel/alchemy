#include "types.h"
#include "scene.h"
#include "abi/battle/presentation/misc/msg_field38.h"
#include "battle_msg.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Runtime_GetObject(s32);

void UiText_DrawQuantity(s32, s32);
void UiText_ShowMessageAndWait(s32);

s32 BattlePres_ShowMessageWhenField38Positive(s16 *script)
{
    s32 object_id;
    s32 result;
    void *object;

    object_id = *script;
    object = Runtime_GetObject(object_id);
    if (Battle_Check(object_id) < 0) {
        return -1;
    }
    result = 0;
    if (FIELD(object, s16 *, 0x38) <= 0) {
        return result;
    }
    Battle_Run();
    UiText_DrawQuantity(object_id, 1);
    UiText_ShowMessageAndWait((s32)&Value_00000816);
    return 0;
}
