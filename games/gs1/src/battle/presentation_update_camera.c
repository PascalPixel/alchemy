#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "battle_msg.h"
#include "battle_party.h"
#include "battle_intro.h"
#include "inventory.h"

/* battle/presentation/cam/update.c */
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

    BattlePres_WaitForAdvance();
    Battle_Do(pos);
    SceneTransform_ApplyYaw((s16)state->field36);
    SceneTransform_ApplyPitch((s16)state->field34);

    local.first = 0;
    local.second = 0;
    local.third = state->field20;
    ((void (*)(struct SceneCameraTransfer *, struct SceneCameraState *))0x03000250)(&local, state);

    if (transition->flag == 0) {
        Battle_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}

/* battle/intro/announce_encounter.c */
extern u8 *gBattleWork;

void UiText_DrawQuantity(s32, s32);
void UiText_ShowMessageAndWait(s32);

void BattleIntro_AnnounceEncounter(s32 enemy_count)
{
    s16 enemies[8];
    u8 *battle_state;
    s16 *enemy;
    s32 announced;

    battle_state = gBattleWork;
    Battle_Run();
    BattleParty_ListPresentEnemies(enemies);

    announced = 0;
    if (enemy_count != 0) {
        enemy = enemies;
        do {
            UiText_DrawQuantity((u16)*enemy++, 1);
            if (announced == enemy_count - 1)
                UiText_ShowMessageAndWait((s32)&Value_00000811);
            else
                UiText_ShowMessageAndWait((s32)&Value_00000810);
            announced++;
            BattlePres_WaitForAdvance();
        } while (announced != enemy_count);
    }

    Battle_unk3_4();
    if (battle_state[69] == BATTLE_ENCOUNTER_PARTY_FIRST) {
        Battle_Run();
        UiText_DrawQuantity(0, 1);
        UiText_ShowMessageAndWait((s32)&Value_00000812);
        BattlePres_WaitForAdvance();
    } else if (battle_state[69] == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
        Battle_Run();
        UiText_DrawQuantity(0, 1);
        UiText_ShowMessageAndWait((s32)&Value_00000813);
        BattlePres_WaitForAdvance();
    }
}

/* battle/party/collect_unit_list.c */
extern u8 *gBattleWork;
extern s8 gRom[];

void BattleParty_CollectUnitList(void)
{
    u16 buf[14];
    u8 *state;
    s32 count;
    s32 i;
    s32 offset;
    s32 index;
    s32 last;
    s32 kind;
    u16 *out;

    state = gBattleWork;
    count = Battle_Check(buf);
    for (i = 0; i < count; i++) {
        *(u16 *)(state + 88 + i * 2) = buf[i];
    }
    offset = count * 2 + 88;
    *(u16 *)(state + offset) = 0xFF;

    count = BattleParty_ListPresentEnemies(buf);
    kind = state[66];
    if (kind >= 0) {
        if (kind <= 1) {
            for (i = 0; i < count; i++) {
                out = (u16 *)(state + 2);
                out[50 + i] = buf[i];
            }
            goto done;
        }
    }
    for (i = 0; i < count; i++) {
        index = (gRom[i] + count / 2) * 2 + 100;
        out = (u16 *)(state + 2);
        *(u16 *)((u8 *)out + index) = buf[i];
    }
done:
    out = (u16 *)(state + 2);
    last = count * 2 + 100;
    *(u16 *)((u8 *)out + last) = 0xFF;
}
