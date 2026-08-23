#include "colosso_log_rolling_stage.h"

typedef struct StageActor {
    u8 filler00[6];
    u16 attributes;
    s32 x;
    u8 filler0C[4];
    s32 z;
} StageActor;

extern s16 Data_02000240[];

extern StageActor *Func_0200507e(s32);
extern void Func_02004ffe(s32, s32, s32, s32);

void ColossoLogRollingStage_ShowActorPositionMessage(void)
{
    StageActor *actor;
    s16 *table;
    s32 x;
    s32 z;
    s32 message_id;

    table = Data_02000240;
    actor = Func_0200507e(*(s32 *)&table[250]);
    x = actor->x >> 20;
    message_id = 23;
    z = actor->z >> 20;
    if (x == 81 && z == 12) {
        if ((actor->attributes & 0xE000) == 0x4000) {
            message_id = 253;
        }
        Func_02004ffe(0, x << 20, z << 20, message_id);
    }
}
