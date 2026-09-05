#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000a6;
extern u8 Data_020099c4[];
extern u8 Data_020097b4[];

s32 SceneData_SelectTable99C4Or97B4ByStateA6(void) {
    if (Data_02000240[224] == (s32)&Value_000000a6) {
        return (s32)Data_020099c4;
    }
    return (s32)Data_020097b4;
}
