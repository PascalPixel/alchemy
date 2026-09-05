#include "types.h"

extern u8 *Data_0200b328;
extern u8 *Data_03001ee0;

void SceneState_ClearWorkWord24(void) {
    if (Data_0200b328 != 0) {
        *(s32 *)(Data_03001ee0 + 24) = 0;
    }
}
