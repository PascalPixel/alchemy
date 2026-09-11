#include "types.h"
#include "scene.h"

void Scene_RunActor11Transition(void)
{
    Actor_Run(241);
    Actor_Place(11, 0, 48);
    Actor_Do(0x121);
    Actor_unk2_4();
    Actor_unk2_2(2);
    Actor_unk3_4();
}
