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

extern StageActor *Func_020054fa(s32);
extern void Func_020055e2(s32, s32);
extern void Func_020053e2(StageActor *, s32);
extern void Func_02005428(StageActor *, s32, s32, s32);
extern void Func_02005436(StageActor *);

void ColossoLogRollingStage_OffsetActiveActor(void)
{
    StageActor *actor;
    s16 *table;
    s32 *slot;
    s32 z;

    table = Data_02000240;
    slot = (s32 *)&table[250];
    actor = Func_020054fa(*slot);
    actor->move_rate_z = 0x10000;
    actor->move_rate_x = 0x20000;
    Func_020055e2(*slot, 258);
    Func_020053e2(actor, 5);
    z = actor->z & 0xFFF00000;
    Func_02005428(actor, actor->x, actor->y, z + 0x180000);
    Func_02005436(actor);
}
