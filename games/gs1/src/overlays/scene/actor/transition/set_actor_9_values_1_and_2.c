#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/transition/set_actor_9_values_1_and_2.h"

void Scene_SetActor9Values1And2(void)
{
    Actor_Apply(9, 1);
    Actor_Apply2(9, 2);
}
