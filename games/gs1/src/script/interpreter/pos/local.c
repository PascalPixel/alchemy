#include "object_commands.h"
#include "types.h"
#include "scene.h"
#include "abi/script/interpreter/pos/local.h"

s32 Script_ApplyLocalOffsetPosition(u8 *arg0)
{
    s32 offset[3];

    Script_Run(
        arg0,
        *(s32 *)(arg0 + 8) + offset[0],
        *(s32 *)(arg0 + 12) + offset[1],
        *(s32 *)(arg0 + 16) + offset[2]
    );
    *(u16 *)(arg0 + 4) += 3;
    return 1;
}
