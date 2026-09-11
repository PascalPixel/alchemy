#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/script/fx_mid_step.h"

extern u8 gWork[];

void Scene_RunScript(void)
{
    u32 i;
    s32 record;

    Script_Run();
    Script_Do(0x200d160);
    Script_Run2(1);
    Script_Place(0, 0xe80000, 0x27c0000);
    Script_Run3(0, 15);
    record = Script_Check(0);
    Script_Run4(record, 0);
    Script_Run5(1);
    Script_Run6(0, 0);
    Script_Run7();
    Script_Run8(1);
    Script_Run9(22);
    Script_Run10(21);
    Script_Run11(1);
    Script_Run12(22, 0, 0);
    Script_Run13(21, 0, 0);
    Script_Run14(20, 0, 0);
    record = Script_Check2(20);
    {
        s32 shown = 0x3000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_Place2(23, 0xe80000, 0x28a0000);
    record = Script_Check3(23);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_Run15(1);
    Script_Run16(20, 23);
}
