#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/script/fx_entry_step.h"

extern u8 gWork[];

void Scene_RunScript(void)
{
    u32 i;
    s32 record;

    Script_Run3();
    Script_Run(-1, -1, -1, 0);
    Script_Run4(1);
    Script_Run5(20, 0, 0);
    Script_Run6(22, 0, 0);
    Script_Run7(24, 0, 0);
    Script_Run8(25, 0, 0);
    Script_Run9(26, 0, 0);
    Script_Run10(27, 0, 0);
    Script_Run11(0, 0, 0);
    Script_Run12(23, 0, 0);
    record = Script_Check(23);
    {
        s32 shown = 0x3000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_Place(21, 0xe80000, 0x28a0000);
    record = Script_Check2(21);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_Run2(0xe80000, -1, 0x27c0000, 0);
    Script_Run13();
    Script_Run14(1);
    Script_Run15(23, 21);
}
