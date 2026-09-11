#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/actor/offset.h"
#include "colosso_log_rolling_stage.h"

typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x1c];
    s32 move_rate_x;
    s32 move_rate_z;
} StageActor;

extern s16 gCell[];

extern StageActor *Actor_Run(s32);

void Colosso_OffsetActiveActor(void)
{
    StageActor *actor;
    s16 *table;
    s32 *slot;
    s32 z;

    table = gCell;
    slot = (s32 *)&table[250];
    actor = Actor_Run(*slot);
    actor->move_rate_z = 0x10000;
    actor->move_rate_x = 0x20000;
    Actor_Apply(*slot, 258);
    Actor_Apply2(actor, 5);
    z = actor->z & 0xFFF00000;
    Actor_SetMode(actor, actor->x, actor->y, z + 0x180000);
    Actor_Do(actor);
}
