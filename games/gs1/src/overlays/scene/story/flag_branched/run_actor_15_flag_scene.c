#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Dialogue_RunActor15Flag303Scene(void)
{
    u32 i;
    s32 record;

    Story_Run();
    Story_Do(0x1756);
    if (Story_Check(0x303) != 0) {
        Story_unk2_2(0x176c);
    }
    Story_unk2_4(15, 0);
    Story_unk3_2(0x303);
    Story_unk3_4();
}
