#include "types.h"

typedef struct SceneActor {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} SceneActor;

SceneActor *Func_02006e70(void);
SceneActor *Func_02006e78(s32);

#define IsSceneActorVerticallyNearPlayer Func_0200181c

s32 IsSceneActorVerticallyNearPlayer(void)
{
    SceneActor *scene_actor = Func_02006e70();
    SceneActor *player = Func_02006e78(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 z_distance = actor_z - player_z;

    if (z_distance >= -6 && z_distance <= 6 && actor_x - 1 < player_x && actor_x + 1 > player_x) {
        return 1;
    }
    return 0;
}
