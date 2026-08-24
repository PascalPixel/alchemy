#include "types.h"
#include "gs1_edition.h"

struct Work;

s32 Func_0808a5d0(s16, s16);
void Func_08016478(void *);
void UiText_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void UiText_DrawString(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0801e74c(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern u8 Value_0000099b[];
extern u8 Value_00000a07[];
extern u8 Data_08037428[];
void Func_08028ef0(struct Work *work, s16 first, const s16 *second) {
    s16 selected = first;
    s32 label = Func_0808a5d0(selected, *second) + (s32)Value_0000099b;
    Func_08016478(work);
    UiText_DrawNumber(selected, 3, (s32) work, 0, 14);
    UiText_DrawNumber(*second, 3, (s32) work, MENU_LABEL_X, 14);
    UiText_DrawString(Data_08037428, (s32) work, MENU_TEXT_X, 0);
    Func_0801e74c(selected + (s32)Value_00000a07, (s32)work, 0, 0);
    UiText_DrawString(Data_08037428, (s32) work, MENU_TEXT_X, 14);
    Func_0801e74c(label, (s32)work, MENU_LABEL_X, 0);
}
