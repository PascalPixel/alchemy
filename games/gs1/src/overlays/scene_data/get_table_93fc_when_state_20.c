#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000020;
extern u8 Data_020093fc[];

s32 SceneData_GetTable93FCWhenState20(void) {
    if (Data_02000240[224] == (s32)&Value_00000020) {
        return (s32)Data_020093fc;
    }
    return 0;
}
