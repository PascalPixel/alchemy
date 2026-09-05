#include "types.h"

#define SceneState_InitStateWordsAndSlots Func_02001084

extern s32 Data_0200bb68;
extern s32 Data_0200bb00;
extern s32 Data_0200bb6c;
extern s32 Data_0200bb70;
extern s32 Data_0200bac0[];

void SceneState_InitStateWordsAndSlots(void) {
    s32 *p;
    u32 i;

    Data_0200bb68 = 63;
    Data_0200bb00 = 0;
    Data_0200bb6c = 0;
    Data_0200bb70 = 120;
    p = Data_0200bac0;
    for (i = 0; i < 16; i++) {
        *p++ = 0;
    }
}
