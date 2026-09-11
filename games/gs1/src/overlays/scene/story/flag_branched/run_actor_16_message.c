/* Contiguous unnamed leaf-owner run for resource_38d. */

#include "types.h"
#include "scene.h"

    Story_Do(0x1750);
    if (Story_Check(0x302) != 0) {
        Story_unk2_2(0x1768);
    }
    Story_Apply(15, 0);
    Story_unk3_2(0x302);
    Story_Run();
}

#include "types.h"

void Dialogue_RunActor16Message1769(void)
{
    Story_unk2_4();
    Story_unk4_2(0x1769);
    Story_Apply2(16, 0);
    Story_unk3_4();
}
