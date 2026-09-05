#include "types.h"

typedef struct SceneActor {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} SceneActor;

SceneActor *Func_02006fd4(void);
SceneActor *Func_020070da(void);

#define IsSceneActorWithinTriggerBox Func_02001980
s32 IsSceneActorWithinTriggerBox(void)
{
    SceneActor *scene_actor = Func_02006fd4();
    SceneActor *player = Func_020070da();
    s32 actor_x = scene_actor->x / 0x100000;
    s32 actor_z = scene_actor->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance > 7 || z_distance > 5) {
        return 0;
    }
    return 1;
}
