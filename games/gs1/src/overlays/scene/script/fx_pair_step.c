#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/script/fx_pair_step.h"

extern u8 gWork[];

void Scene_RunScript(s32 a0, s32 a1)
{
    u32 i;
    s32 p10;
    s32 record;

    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x100;
    Script_Run2();
    Script_Run3();
    Script_Run4(20);
    Script_Run5();
    Script_Place(a0, 0xd80000, 0x24c0000);
    Script_Place2(a0, 0xcccc, 0x6666);
    Script_Place3(a0, 216, 0x258);
    Script_Place4(a0, 218, 0x25c);
    Script_Place5(a0, 234, 0x25c);
    Script_Place6(a0, 236, 0x26a);
    Script_Place7(a0, 0x5000, 20);
    Script_Run6(a0, 3);
    Script_Run7(20);
    Script_Run(a1, 0x5000);
    Script_Run8(a1, 4, 40);
    Script_Run9(a1, 2);
    Script_Do(0x1e39);
    Script_Run10(a1, 0, 20);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x202;
    Script_Run11();
    Script_Run12();
    Script_Run13(10);
    p10 = a0;
}
