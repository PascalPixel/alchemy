#include "resource_384.h"

extern s16 Data_02000240[];
extern u8 Data_020084a0[];
extern u8 Data_020083ec[];

#define DefaultSceneScript     Data_020083ec
#define SpecialStepSceneScript Data_020084a0

u8 *Func_0200006c(void)
{
    if (Data_02000240[225] == RESOURCE384_SPECIAL_STEP) {
        return SpecialStepSceneScript;
    }
    return DefaultSceneScript;
}
