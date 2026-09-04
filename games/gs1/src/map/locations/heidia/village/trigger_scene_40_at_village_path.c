#include "types.h"

typedef struct SceneActor {
    u8 pad0[6];
    u16 dir;        /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} SceneActor;

extern u8 *Data_03001ebc;

SceneActor *Func_0200709a(s32);
s32 Func_02007064(s32);
void Func_02002920(void);

#define TriggerScene40AtVillagePath Func_02001a44
void TriggerScene40AtVillagePath(void)
{
    SceneActor *player = Func_0200709a(0);

    if (Func_02007064(856) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 16 && player_z > 55 && player_z <= 58
            && (player->dir == 0xc000 || player->dir == 0x4000)) {
            s16 *q = (s16 *)(Data_03001ebc + 364);
            s32 v = 40;

            *q = v;
            Func_02002920();
        }
    }
}
