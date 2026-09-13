#include "types.h"
#include "scene.h"

extern s32 gCell[];
extern char Value_00000923;
extern char Value_00000925;
void UiText_DrawQuantity(s32 arg0, s32 arg1);
void UiText_DrawMessage(void *arg0, s32 arg1);

void FieldEvent_ShowStatusMessage(void)
{
    gCell[145] = 0;
    if (*(s8 *)&gCell[146] == 0) {
        UiText_DrawQuantity(0x96, 4);
        UiText_DrawMessage(&Value_00000923, 1);
        return;
    }
    UiText_DrawQuantity(0xEC, 2);
    UiText_DrawMessage(&Value_00000925, 1);
}
