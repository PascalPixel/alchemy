#include "types.h"
#include "object_runtime.h"

s32 GameFlag_Test(s32 flag);
void WorldMap_UpdateBobbingMarker(void);

s32 StoryActor_Initialize(struct ObjectRuntime *actor)
{
    if (GameFlag_Test(0x37) != 0) {
        return 0;
    }
    if (GameFlag_Test(0x16E) != 0) {
        return 0;
    }
    actor->update = WorldMap_UpdateBobbingMarker;
    actor->motion_flags = 0;
    actor->action = 0;
    actor->unknown_66 = 0;
    actor->scale_x = 0x8000;
    actor->scale_y = 0x8000;
    return 0;
}
