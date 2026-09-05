#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000ad;
extern u8 Data_0200b81c[];

s32 SceneData_SelectTableB81cByWord224(void) {
    if (Data_02000240[224] == (s32)&Value_000000ad) {
        return (s32)Data_0200b81c;
    }
    return 0;
}
