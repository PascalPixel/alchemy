#include "types.h"
#include "scene.h"
#include "abi/battle/presentation/cam/update.h"
#include "global_cells.h"

struct SceneCameraState {
    u8 filler0[12];
    s32 field0c;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 *field1c;
    s32 field20;
    u8 filler24[16];
    u16 field34;
    u16 field36;
};

struct SceneCameraTransfer {
    s32 first;
    s32 second;
    s32 third;
};

struct BattlePresentationTransition {
    s32 battle_value;
    s32 timer;
    u8 reserved08[12];
    s32 flag;
};

struct LinkWork {
    u8 pad0[0x44];
    u8 enabled;
    u8 pad1[0x0b];
    u8 side;
    u8 miss;
    u8 paused;
};

#define LINK_STAT (*(u16 *)0x03001f64)
#define REG_SIOCNT (*(volatile u32 *)0x04000128)

void Battle_Run(void)
{
    void **slot = (void **)ADDR_03001E80;
    struct SceneCameraState *state = slot[0];
    struct BattlePresentationTransition *transition = slot[32];
    struct LinkWork *work = slot[-3];
    struct SceneCameraTransfer local;
    s32 *pos;
    s16 delta;
    u32 id;

    if (work->enabled != 0) {
        if ((LINK_STAT & 3) != 3) {
            work->miss++;
            if (work->miss > 24) {
                work->paused = 1;
            }
        } else {
            id = (REG_SIOCNT << 0x1A) >> 0x1E;
            if (work->side != id) {
                work->paused = 1;
            }
            work->miss = 0;
        }
    }

    if (transition->timer != 0) {
        delta = transition->battle_value - state->field36;
        delta /= 16;
        state->field36 += delta;
        transition->timer--;
    }

    pos = &state->field0c;
    if (state->field1c != 0) {
        pos = state->field1c;
    }

    Battle_Run2();
    Battle_Do(pos);
    Battle_Do2((s16)state->field36);
    Battle_Do3((s16)state->field34);

    local.first = 0;
    local.second = 0;
    local.third = state->field20;
    ((void (*)(struct SceneCameraTransfer *, struct SceneCameraState *))0x03000250)(&local, state);

    if (transition->flag == 0) {
        Battle_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}
