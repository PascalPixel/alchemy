#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fixed_point_position.h"
#include "types.h"
#include "iwram_call.h"
#include "global_cells.h"
#include "btldisp.h"
#include "btlmes.h"
#include "btlpty.h"
#include "battle_intro.h"
#include "inventory.h"
#include "dma.h"
#include "scene.h"
#include "btlwork.h"
#include "io_reg.h"

#define SceneTransform_ApplyPosition ((void (*)(void*))SceneTransform_ApplyPosition)

extern volatile u16 gLinkStatus;

void UiWork_PushValueSlotFar(s32, s32);

void BattlePresentation_UpdateCamera(void)
{
    void **slot = (void **)&gCameraWork;
    struct BattleCamera *state = slot[0];
    struct BattlePresentationTransition *transition = slot[32];
    struct BtlWork *work = slot[-3];
    struct FixedPointPosition local;
    s32 *pos;
    s16 delta;
    u32 id;

    if (work->two_sided != 0) {
        if ((gLinkStatus & 3) != 3) {
            work->link_misses++;
            if (work->link_misses > 24) {
                work->link_paused = 1;
            }
        } else {
            id = (REG_SIOCNT << 0x1A) >> 0x1E;
            if (work->link_side != id) {
                work->link_paused = 1;
            }
            work->link_misses = 0;
        }
    }

    if (transition->frames != 0) {
        delta = transition->target_yaw - state->yaw;
        delta /= 16;
        state->yaw += delta;
        transition->frames--;
    }

    pos = state->pos;
    if (state->follow_pos != 0) {
        pos = state->follow_pos;
    }

    Render_ResetTransformState();
    SceneTransform_ApplyPosition(pos);
    SceneTransform_ApplyYaw((s16)state->yaw);
    SceneTransform_ApplyPitch((s16)state->pitch);

    local.x = 0;
    local.y = 0;
    local.z = state->distance;
    Iwram_TransformVector(&local.x, (s32 *)state);

    if (transition->flag == 0) {
        BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}

void BattleIntro_AnnounceEncounter(s32 enemy_count)
{
    s16 enemies[8];
    struct BtlWork *battle;
    s16 *enemy;
    s32 announced;

    battle = BtlWk;
    UiWork_ClearValueNameTablesFar();
    BtlEnmList(enemies);

    announced = 0;
    if (enemy_count != 0) {
        enemy = enemies;
        do {
            UiWork_PushValueSlotFar((u16)*enemy++, 1);
            if (announced == enemy_count - 1)
                UiText_ShowMessageAndWaitCoreFar((s32)MsgLastEnemyAppeared);
            else
                UiText_ShowMessageAndWaitCoreFar((s32)MsgEnemyAppeared);
            announced++;
            BattlePresentation_WaitForAdvance();
        } while (announced != enemy_count);
    }

    UiWork_FinalizeSharedSlotFar();
    if (battle->encounter_mode == BATTLE_ENCOUNTER_PARTY_FIRST) {
        UiWork_ClearValueNameTablesFar();
        UiWork_PushValueSlotFar(0, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgPartyStrikesFirst);
        BattlePresentation_WaitForAdvance();
    } else if (battle->encounter_mode == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
        UiWork_ClearValueNameTablesFar();
        UiWork_PushValueSlotFar(0, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgPartySurprised);
        BattlePresentation_WaitForAdvance();
    }
}

void BattleParty_CollectUnitList(void)
{
    u16 buf[14];
    struct BtlWork *battle;
    s32 count;
    s32 i;
    s32 index;
    s32 kind;

    battle = BtlWk;
    count = BattleParty_PrepareActiveOwners(buf);
    for (i = 0; i < count; i++)
        battle->party_units[i] = buf[i];
    battle->party_units[count] = 0xff;

    count = BtlEnmList(buf);
    kind = battle->battle_type;
    if (kind >= 0) {
        if (kind <= 1) {
            for (i = 0; i < count; i++)
                battle->enm_no[i] = buf[i];
            goto done;
        }
    }
    for (i = 0; i < count; i++) {
        index = BattleParty_CenterOrderOffsets[i] + count / 2;
        battle->enm_no[index] = buf[i];
    }
done:
    battle->enm_no[count] = 0xff;
}
