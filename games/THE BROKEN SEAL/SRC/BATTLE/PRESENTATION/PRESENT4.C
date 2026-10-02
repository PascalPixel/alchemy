#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_MSG.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_INTRO.H"
#include "INVENTORY.H"
#include "DMA.H"
#include "SCENE.H"
#include "BATTLE_WORK.H"

extern u8 gLinkStatus[];
extern u8 gCameraWork[];

struct SceneCameraTransfer {
    s32 x;
    s32 y;
    s32 z;
};

#define LINK_STAT (*(u16 *)gLinkStatus)
#define REG_SIOCNT (*(volatile u32 *)0x04000128)
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);

void UiWork_ClearValueNameTablesFar(void);
void UiWork_PushValueSlotFar(s32, s32);
void UiText_ShowMessageAndWaitCoreFar(s32);
void UiWork_FinalizeSharedSlotFar(void);
void BattlePresentation_WaitForAdvance(void);

extern s8 BattleParty_CenterOrderOffsets[];
s32 BattleParty_PrepareActiveOwners(u16 *out);

void BattlePresentation_UpdateCamera(void)
{
    void **slot = (void **)((u32)&gCameraWork);
    struct BattleCamera *state = slot[0];
    struct BattlePresentationTransition *transition = slot[32];
    struct BattleSession *work = slot[-3];
    struct SceneCameraTransfer local;
    s32 *pos;
    s16 delta;
    u32 id;

    if (work->two_sided != 0) {
        if ((LINK_STAT & 3) != 3) {
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
    struct BattleSession *battle;
    s16 *enemy;
    s32 announced;

    battle = gBattleWork;
    UiWork_ClearValueNameTablesFar();
    BattleParty_ListPresentEnemies(enemies);

    announced = 0;
    if (enemy_count != 0) {
        enemy = enemies;
        do {
            UiWork_PushValueSlotFar((u16)*enemy++, 1);
            if (announced == enemy_count - 1)
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgLastEnemyAppeared);
            else
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgEnemyAppeared);
            announced++;
            BattlePresentation_WaitForAdvance();
        } while (announced != enemy_count);
    }

    UiWork_FinalizeSharedSlotFar();
    if (battle->encounter_mode == BATTLE_ENCOUNTER_PARTY_FIRST) {
        UiWork_ClearValueNameTablesFar();
        UiWork_PushValueSlotFar(0, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgPartyStrikesFirst);
        BattlePresentation_WaitForAdvance();
    } else if (battle->encounter_mode == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
        UiWork_ClearValueNameTablesFar();
        UiWork_PushValueSlotFar(0, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgPartySurprised);
        BattlePresentation_WaitForAdvance();
    }
}

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

    state = (u8 *)gBattleWork;
    count = BattleParty_PrepareActiveOwners(buf);
    for (i = 0; i < count; i++) {
        *(u16 *)(state + 88 + i * 2) = buf[i];
    }
    offset = count * 2 + 88;
    *(u16 *)(state + offset) = 0xFF;

    count = BattleParty_ListPresentEnemies(buf);
    kind = ((struct BattleSession *)state)->unknown_042;
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
        index = (BattleParty_CenterOrderOffsets[i] + count / 2) * 2 + 100;
        out = (u16 *)(state + 2);
        *(u16 *)((u8 *)out + index) = buf[i];
    }
done:
    out = (u16 *)(state + 2);
    last = count * 2 + 100;
    *(u16 *)((u8 *)out + last) = 0xFF;
}

/* Copies the eight-word tile pattern one row down in VRAM, then clears the
   five words at 0x0600028c with the IWRAM word clear. */
s32 BattlePresentation_InitializeTilePattern(void)
{
    Dma_Set((const void *)0x06000290, (void *)0x06000280, 0x80000008,
            (volatile u32 *)0x040000d4);
    return Iwram_ClearWords((void *)0x0600028c, 20);
}

/* Battle presentation keeps its own copy of the BG0 vertical offset reset. */
void BattlePresentation_ClearBg0VerticalOffset(void)
{
    u32 zero = 0;

    *(volatile u16 *)0x04000012 = zero;
}

void Party_ReservedNoOp5B14(void)
{
}
