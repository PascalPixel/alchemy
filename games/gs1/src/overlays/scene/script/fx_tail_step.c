#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/script/fx_tail_step.h"

extern u8 gWork[];

void Scene_RunScript(void)
{
    u32 i;
    s32 record;

    Script_Run(0xe80000, -1, 0x2a40000, 0);
    Script_Run2();
    Script_Place(0, 0xe80000, 0x2a40000);
    record = Script_Check(0);
    {
        s32 shown = 0x4000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_Run3(1);
}
