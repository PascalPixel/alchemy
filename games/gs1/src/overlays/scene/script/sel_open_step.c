#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/script/sel_open_step.h"

extern u8 gWork[];

void Scene_RunSel(s32 a0)
{
    u32 i;
    s32 record;

    Script_Run();
    Script_Run2(228);
    Script_Place(0, 0x6666, 0x3333);
    Script_Run3(0, 2);
    Script_Place2(0, 0, -8);
    record = Script_Check(0);
    Script_Run4(record, 0);
    Script_Run5(8);
    Script_Run6(0, ((a0 << 19) + 0x80000), 0);
    Script_Run7(30);
}
