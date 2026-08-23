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

extern s16 Data_02000240[];

extern StageActor *Func_02005556(s32);
extern void Func_02005440(StageActor *, s32);
extern void Func_02005486(StageActor *, s32, s32, s32);
extern void Func_02005494(StageActor *);
extern void Func_0200566e(s32, s32);
extern void Func_02005620(s32, s32, s32);

void ColossoLogRollingStage_ClampAndOffsetActiveActor(void)
{
    StageActor *actor;
    s16 *table;
    s32 *slot;
    s32 z;

    table = Data_02000240;
    slot = (s32 *)&table[250];
    actor = Func_02005556(*slot);
    if (actor->x > 0x2980000) {
        actor->x = 0x2980000;
    }
    actor->move_rate_z = 0x10000;
    actor->move_rate_x = 0x20000;
    Func_02005440(actor, 5);
    z = actor->z & 0xFFF00000;
    Func_02005486(actor, actor->x, actor->y, z + 0xC0000);
    Func_02005494(actor);
    Func_0200566e(*slot, 258);
    Func_02005620(*slot, 6, 0);
}
