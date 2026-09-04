#include "types.h"

#define SceneData_GetTable97dc Func_02000f28

/* Complete in-image scene-descriptor getter through its literal pool. */
extern const u8 Data_020097dc[];

const u8 *SceneData_GetTable97dc(void)
{
    return Data_020097dc;
}
