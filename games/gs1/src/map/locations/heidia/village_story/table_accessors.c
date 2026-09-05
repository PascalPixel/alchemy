/* Contiguous unnamed leaf-owner run for resource_39d. */

#include "types.h"

extern u8 Data_0200b9c8[];

void *SceneData_GetTableb9c8(void) {
    return Data_0200b9c8;
}

#include "types.h"

extern s16 Data_02000240[];
extern u8 Data_0200b9d4[];

void Func_020039f6(s32);

void *SceneData_GetTableB9d4AfterStateCheck(void) {
    if (Data_02000240[225] != 1) {
        Func_020039f6(0x253);
    }
    return Data_0200b9d4;
}

#include "types.h"

extern u8 Data_0200bbe4[];

void *SceneData_GetTablebbe4(void) {
    return Data_0200bbe4;
}
