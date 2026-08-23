#include "colosso_log_rolling_stage.h"

typedef struct ActorSpriteRef {
    u8 filler00[0x28];
    s16 *entry;
} ActorSpriteRef;

typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0x3c];
    ActorSpriteRef *sprite;
    u8 state;
} StageActor;

extern s16 Data_02000240[];

extern StageActor *Func_02007198(s32);
extern void Func_02007186(void);
extern StageActor *Func_020071a6(s32);
extern void Func_020072a0(s32);
extern void Func_020072b8(s32, s32);
extern void Func_02007210();
extern void Func_02007374(void);
extern void Func_02007380(void);
extern void Func_02007200(s32, s32);
extern void Func_0200735a(s32);
extern void Func_0200720a(s32);
extern void Func_02004e9a(s32);
extern void Func_020073ae(void);
extern void Func_020073c2(void);
extern void Func_02007274(void);

void ColossoLogRollingStage_SelectNearestObstacle(void)
{
    u8 *state;
    s16 *table;
    StageActor *target;
    StageActor *actor;
    s32 *frame;
    s32 active_slot;
    s32 slot;
    s32 best;
    s32 best_slot;
    s32 dx;
    s32 adx;
    s32 dz;
    s32 base;
    s32 z;

    state = *(u8 **)0x03001ebc;
    best_slot = 8;
    best = 0x100000;
    table = Data_02000240;
    active_slot = *(s32 *)&table[250];
    target = Func_02007198(active_slot);
    Func_02007186();
    for (slot = 8; slot <= 66; slot++) {
        actor = Func_020071a6(slot);
        if (actor == 0) {
            continue;
        }
        if (actor->state != 1) {
            continue;
        }
        if (*actor->sprite->entry != 165) {
            continue;
        }
        dx = (target->x - actor->x) / 65536;
        dz = (target->z - actor->z) / 65536;
        if (dz > 0) {
            continue;
        }
        adx = dx;
        if (adx < 0) {
            adx = -adx;
        }
        if (dz < 0) {
            dz = -dz;
        }
        if (adx + dz < best) {
            best_slot = slot;
            best = adx + dz;
        }
    }
    Func_020072a0(0x2085);
    Func_020072b8(best_slot, 0);
    frame = (s32 *)(state + 448);
    *frame = 0x200;
    *(s32 *)(state + 456) = 15;
    Func_02007210(20);
    Func_02007374();
    Func_02007380();
    base = active_slot * 16;
    Func_02007200(base + 880, target->x >> 20);
    z = target->z >> 20;
    Func_02007210(base + 888, z);
    active_slot = active_slot + 1;
    if (active_slot > 3) {
        Func_0200735a(10);
        Func_0200720a(282);
    } else {
        Func_02004e9a(active_slot);
        Func_020073ae();
        Func_020073c2();
        *frame = 0;
    }
    Func_02007274();
}
