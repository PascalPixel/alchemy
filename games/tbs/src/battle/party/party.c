#include "types.h"
#include "serial_runtime.h"
#include "btlpty.h"
#include "btlwork.h"
#include "iwram_call.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "btlsys.h"
#include "btltype.h"
#include "party_state.h"
#include "battle_command.h"
#include "resource.h"
#include "mem.h"
#include "btldisp.h"
#include "dma.h"
#include "io_reg.h"
#include "game_state.h"
#include "heap_state.h"
#include "flag_ids.h"
#include "callback_scheduler.h"
#include "menu_list.h"
#include "audio_engine_symbols.h"
#include "ui.h"
#include "btlcalc.h"
#include "text/msg_ids.h"

s32 BattleParty_AssignMemberSlots(void)
{
    u16 active_members[8];
    struct BtlWork *battle_state = BtlWk;
    s32 party_size = BattleParty_PrepareActiveOwners(active_members);
    s32 member_slot;
    s32 unit_id;

    for (member_slot = 0; member_slot < party_size; member_slot++) {
        unit_id = active_members[member_slot];
        battle_state->owner_slots[unit_id] = member_slot - 128;
    }
}

s32 Battle_ClearSerialTransferPayload(void)
{
    return Iwram_ClearWords(gSerialTransfer.payload, 16);
}

s32 GameFlag_TestFar(s32 flag);

#define BattleParty_ReconcileDjinnRecovery ((void (*)(void))BattleParty_ReconcileDjinnRecovery)
#define BattleSummon_UpdateAvailability ((void (*)(void))BattleSummon_UpdateAvailability)
#define Party_ReservedNoOp5B14 ((void (*)(s32))Party_ReservedNoOp5B14)
#define Runtime_GetRemainingIwram ((void (*)(void))Runtime_GetRemainingIwram)
#define Runtime_GetRemainingEwram ((void (*)(void))Runtime_GetRemainingEwram)
#define Battle_ProcessRoundEnd ((void (*)(void))Battle_ProcessRoundEnd)
#define BattlePresentation_WaitForPromptAt ((void (*)(s32,s32))BattlePresentation_WaitForPromptAt)
#define BattleParty_ResetActiveRuntimeFields ((void (*)(void))BattleParty_ResetActiveRuntimeFields)

#define DMA_FILL32 0x85000000

#define Dma3Fill(cell, dest, bytes)                                           \
    {                                                                         \
        (cell) = 0;                                                           \
        Dma_Set((void *)&(cell), (dest), DMA_FILL32 | ((bytes) >> 2),         \
                REG_DMA3);                                                    \
    }

extern s32 gBattleRandomSeed;

#define Battle_SetStart(mode)                                                 \
    do {                                                                      \
        gGameState.battle_start = (mode);                                     \
    } while (0)
#define BattleRandom_ClearSeed()                                              \
    do {                                                                      \
        gBattleRandomSeed = 0;                                                \
    } while (0)

extern volatile u16 gLinkStatus;
extern u8 gActorSpriteSlots[];
extern u8 gCpuLoadDisplayEnabled;

void GameFlag_SetBitFar(s32 id);
s32 *Trade_GetOfferStateFar(s32 index);
void PtyStDrawFar(s32 mode);
void BattleMotion_DestroyAllSlotObjects(void);
s32 UiText_OpenMessageWindowFar(s32 id, s32 a, s32 b, s32 c);
void UiWork_FinalizeFar(s32 handle, s32 mode);
void UiWork_PushValueSlotFar(s32 a, s32 b);
void UiText_ShowMessageAndWaitCoreFar(s32 id);
void BattlePresentation_WaitForAdvance(void);

s32 Battle_RunEncounter(s32 enemy_group)
{
    struct BattleCamera *camera;
    s32 *target;
    struct BtlWork *battle;
    u8 *owner_states;
    struct BattlePresentationTransition *transition;
    struct BattleDisplayWork *display;
    s32 object;
    u8 *dest;
    s32 count;
    s32 i;
    s32 result;
    s32 handle;
    s32 wait;
    u32 pos;
    u32 actor;
    s16 cue;
    u8 unused[48];
    volatile u32 fill;

    camera = Runtime_AllocateBlock(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
    battle = Runtime_AllocateBlock(HEAP_SLOT_BATTLE, sizeof(struct BtlWork));
    owner_states = Runtime_AllocateBlock(54, 0x7c8);
    transition = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_BACKGROUND, 32);
    Runtime_AllocateBlock(11, 0x280);
    target = camera->pos;
    /* FAKEMATCH: needed to match: the setup's zero held in result, a global pseudo, so global allocation gives it r7 as listed */
    result = 0;
    Iwram_ClearWords(owner_states, 0x7c8);
    Scheduler_ResetTaskTable();
    transition->frames = result;
    transition->target_yaw = 0x2000;
    transition->flag = 1;
    transition->unknown_18 = result;
    transition->unknown_1c = result;
    REG_DISPCNT = 1;
    GameFlag_SetBitFar(0x103);
    GameFlag_SetBitFar(0x169);
    Render_ResetTransformState();

    Dma3Fill(fill, camera, sizeof(struct BattleCamera));
    Dma3Fill(fill, battle, sizeof(struct BtlWork));
    battle->prompt_slot = -1;
    battle->enm_grp = enemy_group;
    display = Runtime_AllocateBlock(37, sizeof(struct BattleDisplayWork));
    Dma3Fill(fill, display, sizeof(struct BattleDisplayWork));
    battle->background = Event_GetSpecialValueFar();
    Runtime_AllocateBlock(4, 0xe00);
    Runtime_AllocateBlock(3, 0x600);
    ObjectSystem_InitializeFar(4);
    if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0)
        UiWork_InitializeFar(1);
    else
        UiWork_InitializeFar(0);

    target[0] = 0;
    target[1] = 0x400000;
    target[2] = 0;
    camera->eye[0] = 0;
    camera->eye[1] = 0xb40000;
    camera->eye[2] = 0x400000;
    camera->yaw = 0x2800;
    camera->pitch = 0x5000;
    camera->distance = 0x1000000;
    object = BattleFormation_BuildEnemyList(battle->enm_grp);

    if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0) {
        battle->two_sided = 1;
        Battle_SetStart(4);
    }
    if (battle->two_sided != 0) {
        BattleRandom_ClearSeed();
        for (wait = 0;;) {
            if ((gLinkStatus & 3) == 3)
                break;
            WaitFrames(1);
            wait++;
            if (wait > 24) {
                battle->link_paused = 1;
                break;
            }
        }
        battle->link_side = (REG_SIOCNT << 26) >> 30;
        dest = gBattleOwnerStates;
        for (pos = 0; pos <= 0x7c7; pos++)
            *dest++ = gActorSpriteSlots[pos];
        object = GameFlag_GetByteFar(0x3f0);
        BattleParty_AssignMemberSlots();
        battle->battle_type = 0;
    }
    Scheduler_AddOrUpdateCallback(BattlePresentation_UpdateCamera, 0xc7f);

    cue = gGameState.scene_cue;
    if (cue != 0) {
        AudioCommand_PlayFar(cue);
        if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0) {
            AudioCommand_PlayFar(55);
            Sound_LoadPresetParameters(4);
        }
    } else {
        AudioCommand_PlayFar(51);
        AudioCommand_PlayFar(76);
    }

    BattleParty_CollectUnitList();
    BattleUnit_RefreshPlacement();
    BattleParty_ReconcileDjinnRecovery();
    BattleSummon_UpdateAvailability();
    if (*Trade_GetOfferStateFar(0) != 0)
        battle->pty_st = 3;
    else
        battle->pty_st = 1;
    UiWindow_CreateWithLayoutBoundsFar(9);
    Camera_InitDefaultTransform();
    BtlObjPut();
    BattlePresentation_InitializeWorkAndResetState();
    BattleBackground_Load(1, battle->background, 0);
    BattleCamera_SetRange(0xa00000, 0x500000, 0, 0, 0x20000);
    BattlePres_SetupTransitionScene(0, 0, 0, 190);
    Party_ReservedNoOp5B14(1);
    REG_BLDCNT = 0;
    Summon_ClearWorkFields();
    battle->prompt_slot = Resource_LoadIntoFreeSlot(128);
    battle->encounter_mode = 0;
    if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0) {
        battle->encounter_mode = 1;
    } else if (gGameState.battle_start == 0) {
        if ((BtlRndFar() & 15) == 0)
            battle->encounter_mode = 1;
        else if ((BtlRndFar() & 31) == 0)
            battle->encounter_mode = 2;
    }
    BattlePres_RunEntryTransition(object, enemy_group);
    transition->flag = 0;
    gCpuLoadDisplayEnabled = 0;
    Scheduler_AddOrUpdateCallback(BattleActor_UpdateSpritePriorities, 0xc80);

    for (;;) {
        Battle_ReservedNoOp9B2C();
        BattleSummon_UpdateAvailability();
        if (*Trade_GetOfferStateFar(0) != 0)
            battle->pty_st = 3;
        else
            battle->pty_st = 1;
        transition->target_yaw = 0x2800;
        transition->frames = 60;
        PtyStDrawFar(battle->pty_st);
        Iwram_ClearWords(battle->actions, 0x140);
        Resource_ResetEntry(battle->prompt_slot);
        if (GameFlag_TestFar(FLAG_AUTO_BATTLE) == 0) {
            Runtime_GetRemainingIwram();
            Runtime_GetRemainingEwram();
            count = BattlePresentation_BuildActions(battle->actions);
            Runtime_GetRemainingIwram();
            Runtime_GetRemainingEwram();
        } else {
            count = BattlePresentation_BuildSortedUnitEntries(battle->actions);
        }
        battle->prompt_slot = Resource_LoadIntoFreeSlot(128);
        PtyStDrawFar(battle->pty_st);
        if (count < 0)
            goto aborted;

        for (i = 0; i < count; i++) {
            actor = battle->actions[i].unit_id;
            Runtime_GetRemainingIwram();
            Runtime_GetRemainingEwram();
            if (GameFlag_TestFar(FLAG_AUTO_BATTLE) == 0) {
                if (BattlePresentation_DispatchAction(&battle->actions[i], i == 0 ? 10 : 0) == 1)
                    goto interrupted;
            } else {
                if (BattlePres_RunAction(&battle->actions[i]) == 1)
                    goto interrupted;
            }
            Runtime_GetRemainingIwram();
            Runtime_GetRemainingEwram();
            if (BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, 0) == 0)
                goto party_lost;
            if (BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, 0) == 0) {
                if (actor <= 7 && battle->spoils.defeated == 1)
                    battle->defeat_state = 3;
                goto resolved;
            }
            if (BattlePres_SyncTurn() < 0)
                goto aborted;
        }

        battle->encounter_mode = 0;
        Battle_ReservedNoOpF674();
        Battle_ProcessRoundEnd();
        BattleMotion_DestroyAllSlotObjects();
        if (battle->two_sided != 0) {
            if (BattlePres_SyncTurn() < 0)
                goto aborted;
        } else {
            WaitFrames(20);
        }
        if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) == 0)
            continue;

        handle = UiText_OpenMessageWindowFar((s32)MsgTutorialUnleashed, 0, 4, 1);
        while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
        UiWork_FinalizeFar(handle, 1);
        WaitFrames(1);
        handle = UiText_OpenMessageWindowFar((s32)MsgTutorialStandby, 10, 4, 1);
        BattlePresentation_WaitForPromptAt(92, 24);
        UiWork_FinalizeFar(handle, 1);
        WaitFrames(1);
    }

resolved:
    Battle_ClearSerialTransferPayload();
    if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) == 0) {
        if (battle->two_sided != 0)
            AudioCommand_PlayFar(58);
        if (battle->spoils.defeated != 0) {
            AudioCommand_PlayFar(58);
            if (battle->defeat_state <= 1) {
                BtlUnitSetFar(128, battle->enemy_classes[battle->first_defeated], 26);
                UiWork_ClearValueNameTablesFar();
                UiWork_PushValueSlotFar(128, 1);
                UiText_ShowMessageAndWaitCoreFar(battle->defeat_state + (s32)MsgDowned);
                BattlePresentation_WaitForAdvance();
            }
        }
        Battle_AwardSpoils();
    }
    AudioCommand_PlayFar(17);
    Blend_SetDarkenTarget16(30);
    Blend_WaitForTransition();
    result = battle->spoils.defeated;
    goto finished;

aborted:
    Battle_ClearSerialTransferPayload();
    Scheduler_EnableCallbacks(0);
    REG_DISPCNT = 1;
    result = battle->spoils.defeated;
    GameFlag_SetBitFar(0x3e8);
    goto finished;

party_lost:
    Battle_ClearSerialTransferPayload();
    AudioCommand_PlayFar(59);
    UiWork_ClearValueNameTablesFar();
    UiWork_PushValueSlotFar(gGameState.active_owners[0], 1);
    if (BattleParty_PrepareActiveOwners(0) == 1)
        UiText_ShowMessageAndWaitCoreFar((s32)MsgGoesDownInDefeat);
    else
        UiText_ShowMessageAndWaitCoreFar((s32)MsgPartyDefeated);
    BattlePresentation_WaitForAdvance();
    AudioCommand_PlayFar(17);
    Blend_SetDarkenTarget16(30);
    result = -1;
    Blend_WaitForTransition();
    goto finished;

interrupted:
    AudioCommand_PlayFar(17);
    Blend_SetDarkenTarget16(30);
    Blend_WaitForTransition();
    result = 0x3e7;

finished:
    BattleParty_ResetActiveRuntimeFields();
    Battle_ReservedNoOpF674();
    BattlePlacement_UpdateTimedEntries();
    Battle_SetStart(0);
    Scheduler_RemoveCallback(BattleActor_UpdateSpritePriorities);
    Runtime_ReleaseHeapBlock10();
    return result;
}

s32 Party_CountActiveOwnersFar(void);

s32 BattleParty_PrepareActiveOwners(u16 *owners)
{
    s32 limit;
    s32 count;
    s32 index;

    limit = 4;
    if (BtlWk->two_sided != 0)
        limit = 3;

    count = Party_CountActiveOwnersFar();
    if (count > limit)
        count = limit;

    for (index = 0; index < count; index++) {
        s32 owner = gGameState.active_owners[index];

        if (owners != 0)
            *owners++ = owner;
        UnitGetFar(owner)->side = BATTLE_UNIT_PARTY;
    }

    if (owners != 0)
        *owners = BATTLE_UNIT_LIST_END;
    return count;
}

s32 BtlEnmList(s16 *unit_ids)
{
    s16 *output;
    s32 battle_result;
    s32 id;
    s32 entry_limit;
    s32 entry_count;

    output = unit_ids;
    entry_count = 0;
    entry_limit = 6;
    battle_result = 0;
    if (output != NULL) {
        if (GameFlag_TestFar(0x16C) != 0) {
            entry_limit = 3;
        }
        id = 0x80;
        entry_limit += 0x80;
        for (; id < entry_limit; id += 1) {
            if (UnitGetFar(id)->side != 0) {
                *output = id;
                entry_count += 1;
                output += 1;
            }
        }
        *output = BATTLE_UNIT_LIST_END;
        battle_result = entry_count;
    }
    return battle_result;
}

s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids)
{
    u16 active_members[8];
    u16 *base;
    s32 remaining;
    s32 enemy_limit;
    u16 *output;
    s32 living_count;
    s32 enemy_capacity;
    u16 *member;
    s32 unit_id;
    s16 hp;
    s32 active_count;
    struct BtlUnit *unit;

    output = unit_ids;
    living_count = 0;
    enemy_capacity = 6;
    if (GameFlag_TestFar(0x16C) != 0) {
        enemy_capacity = 3;
    }
    if (side_mask & BATTLE_SIDE_PARTY) {
        base = active_members;
        active_count = BattleParty_PrepareActiveOwners(base);
        if (living_count < active_count) {
            member = base;
            remaining = active_count;
            do {
                unit_id = *member;
                member += 1;
                hp = UnitGetFar(unit_id)->hp;
                if (hp > 0) {
                    if (output != NULL) {
                        *output = unit_id;
                        output += 1;
                    }
                    living_count += 1;
                }
                remaining -= 1;
            } while (remaining != 0);
        }
    }
    if (side_mask & BATTLE_SIDE_ENEMIES) {
        remaining = 0x80;
        enemy_limit = enemy_capacity + 0x80;
        if (remaining < enemy_limit) {
            do {
                unit = UnitGetFar(remaining);
                if ((unit->side != 0) && (unit->hp > 0)) {
                    if (output != NULL) {
                        *output = remaining;
                        output += 1;
                    }
                    living_count += 1;
                }
                remaining += 1;
            } while (remaining < enemy_limit);
        }
    }
    if (output != NULL) {
        *output = BATTLE_UNIT_LIST_END;
    }
    return living_count;
}

s32 BattleParty_ListActorIds(s32 groups, u16 *dst)
{
    struct BtlWork *order;
    s32 count;
    s32 i;

    order = BtlWk;
    count = 0;
    if (groups & BATTLE_SIDE_PARTY) {
        for (i = 0; order->party_units[i] != BATTLE_UNIT_LIST_END; i++) {
            if (order->party_units[i] != BATTLE_UNIT_REMOVED) {
                if (dst != NULL)
                    *dst++ = order->party_units[i];
                count++;
            }
        }
    }
    if (groups & BATTLE_SIDE_ENEMIES) {
        for (i = 0; order->enm_no[i] != BATTLE_UNIT_LIST_END; i++) {
            if (order->enm_no[i] != BATTLE_UNIT_REMOVED) {
                if (dst != NULL)
                    *dst++ = order->enm_no[i];
                count++;
            }
        }
    }
    if (dst != NULL)
        *dst = BATTLE_UNIT_LIST_END;
    return count;
}
