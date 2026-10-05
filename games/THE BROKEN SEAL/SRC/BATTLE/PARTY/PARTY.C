#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_WORK.H"
#include "PARTY_STATE.H"
#include "BATTLE_PARTY.H"
#include "TYPES.H"
#include "BATTLE_COMMAND.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "BATTLE_PRESENTATION.H"
#include "DMA.H"
#include "IWRAM_CALL.H"

struct BattleTimerWork {
    s32 value;                  /* 0x00 */
    s32 frames;                 /* 0x04 */
    u8 unk_08[12];              /* 0x08 */
    s32 field_14;               /* 0x14 */
    s32 field_18;               /* 0x18 */
    s32 field_1c;               /* 0x1c */
};

/* One replayed presentation entry; halfword 0 is the acting unit id. */
struct BattleActionSlot {
    s16 h[8];
};

struct BattleEncounterWork {
    s32 field_00;                       /* 0x000 */
    u8 unk_04[12];                      /* 0x004 */
    u16 msg_ids[22];                    /* 0x010 */
    u16 field_3c;                       /* 0x03c */
    u16 field_3e;                       /* 0x03e */
    u8 unk_40;                          /* 0x040 */
    u8 field_41;                        /* 0x041 */
    u8 field_42;                        /* 0x042 */
    u8 unk_43;                          /* 0x043 */
    u8 field_44;                        /* 0x044 */
    u8 field_45;                        /* 0x045 */
    u8 unk_46[10];                      /* 0x046 */
    u8 field_50;                        /* 0x050 */
    u8 unk_51;                          /* 0x051 */
    u8 field_52;                        /* 0x052 */
    u8 unk_53;                          /* 0x053 */
    s32 field_54;                       /* 0x054 */
    u8 unk_58[0x294];                   /* 0x058 */
    struct BattleActionSlot actions[36]; /* 0x2ec */
    u8 unk_52c[12];                     /* 0x52c */
    s32 field_538;                      /* 0x538 */
    u8 unk_53c[0x10c];                  /* 0x53c */
    u16 field_648;                      /* 0x648 */
    u8 unk_64a[0x1e2];                  /* 0x64a .. 0x82b */
};

/* DMA3 fixed-source 32-bit fill of one freshly allocated block. */
#define DMA_FILL32 0x85000000

#define Dma3Fill(cell, dest, bytes)                                           \
    {                                                                         \
        (cell) = 0;                                                           \
        Dma_Set((void *)&(cell), (dest), DMA_FILL32 | ((bytes) >> 2),         \
                (volatile u32 *)0x040000d4);                                  \
    }

extern u8 gCell[];
extern s32 gBattleRandomSeed;

/* The battle mode byte (4 for a link battle) and the battle random seed. */
#define Battle_SetMode(mode)                                                  \
    do {                                                                      \
        gCell[0x22b] = (mode);                                                \
    } while (0)
#define BattleRandom_ClearSeed()                                              \
    do {                                                                      \
        gBattleRandomSeed = 0;                                                \
    } while (0)

extern u8 MsgDowned;
extern u8 MsgPartyDefeated;
extern u8 MsgGoesDownInDefeat;
extern u8 MsgTutorialUnleashed;
extern u8 MsgTutorialStandby;
extern volatile u16 gLinkStatus;
extern u8 *gBattleOwnerStates;
extern u8 gActorSpriteSlots[];
extern u8 gCpuLoadDisplayEnabled;
void BattlePresentation_UpdateCamera(void);
void Func_080b7738(void);

/* The relocated IWRAM block clear this owner reaches through __call_via_r3. */

void Scheduler_ResetTaskTable(void);
void Render_ResetTransformState(void);
void GameFlag_SetBitFar(s32 id);
s32 Event_GetSpecialValueFar(void);
void ObjectSystem_InitializeFar(s32 mode);
s32 GameFlag_IsSet(s32 flag);
void UiWork_InitializeFar(s32 mode);
s32 BattleFormation_BuildEnemyList(s32 value);
void WaitFrames(s32 frames);
s32 GameFlag_GetByteFar(s32 id);
s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 order);
void AudioCommand_PlayFar(s32 cue);
s32 Sound_LoadPresetParameters(s32 value);
void BattleParty_CollectUnitList(void);
void BattleUnit_RefreshPlacement(void);
void BattlePlacement_UpdateEntries(void);
void BattleSummon_UpdateAvailability(void);
s32 *Trade_GetOfferStateFar(s32 index);
void UiWindow_CreateWithLayoutBoundsFar(s32 value);
void Camera_InitDefaultTransform(void);
void BattleActor_CommitPlacement(void);
void BattlePresentation_InitializeWorkAndResetState(void);
void BattleBackground_Load(s32 a, s32 b, s32 c);
void BattleCamera_SetRange(s32 a, s32 b, s32 c, s32 d, s32 e);
void BattlePres_SetupTransitionScene(s32 a, s32 b, s32 c, s32 d);
void Party_ReservedNoOp5B14(s32 value);
void Summon_ClearWorkFields(void);
s32 BattleRandom16Far(void);
void BattlePres_RunEntryTransition(s32 object, s32 arg);
void Battle_ReservedNoOp9B2C(void);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
void Runtime_GetRemainingIwram(void);
void Runtime_GetRemainingEwram(void);
s32 BattlePresentation_BuildActions(struct BattleActionSlot *slots);
s32 BattlePresentation_BuildSortedUnitEntries(struct BattleActionSlot *slots);
s32 BattlePresentation_DispatchAction(struct BattleActionSlot *slot, s32 delay);
s32 BattlePres_RunAction(struct BattleActionSlot *slot);
s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids);
s32 BattlePres_SyncTurn(void);
void Battle_ReservedNoOpF674(void);
void Battle_ProcessRoundEnd(void);
void BattleMotion_DestroyAllSlotObjects(void);
s32 UiText_OpenMessageWindowFar(s32 id, s32 a, s32 b, s32 c);
s32 UiWork_IsCompleteFar(void);
void UiWork_FinalizeFar(s32 handle, s32 mode);
void BattlePresentation_WaitForPromptAt(s32 a, s32 b);
void Battle_ApplyValueToWork2224(void);
void BattleUnit_AssignFar(s32 a, s32 b, s32 c);
void UiWork_ClearValueNameTablesFar(void);
void UiWork_PushValueSlotFar(s32 a, s32 b);
void UiText_ShowMessageAndWaitCoreFar(s32 id);
void BattlePresentation_WaitForAdvance(void);
void Battle_AwardSpoils(void);
void Blend_SetDarkenTarget16(s32 value);
void Blend_WaitForTransition(void);
void Scheduler_EnableCallbacks(s32 value);
s32 BattleParty_PrepareActiveOwners(u16 *owners);
void BattleParty_ResetActiveRuntimeFields(void);
void Scheduler_RemoveCallback(s32 callback);
void Runtime_ReleaseHeapBlock10(void);

/* Allocates battle work, plays the opening, and runs rounds to completion. */
s32 Battle_RunEncounter(s32 arg)
{
    struct BattleCamera *scene;
    s32 *cam;
    struct BattleEncounterWork *work;
    u8 *buf;
    struct BattleTimerWork *timer;
    u8 *src;
    s32 object;
    u8 *dst;
    s32 cnt;
    s32 i;
    s32 result;
    s32 delay;
    s32 handle;
    s32 wait;
    u32 pos;
    s32 off;
    u32 actor;
    s16 cue;
    u8 unused[48];
    volatile u32 fill;

    scene = (struct BattleCamera *)Runtime_AllocateBlock(12, 76);
    work = (struct BattleEncounterWork *)Runtime_AllocateBlock(9, 0x82c);
    buf = (u8 *)Runtime_AllocateBlock(54, 0x7c8);
    timer = (struct BattleTimerWork *)Runtime_AllocateBlock(44, 32);
    Runtime_AllocateBlock(11, 0x280);
    cam = scene->pos;
    /* __call_via_r3 -> the IWRAM block clear at 0x03000164. */
    /* FAKEMATCH: the setup's zero held in result, a global pseudo, so global allocation gives it r7 as listed. */
    result = 0;
    Iwram_ClearWords(buf, 0x7c8);
    Scheduler_ResetTaskTable();
    timer->frames = result;
    timer->value = 0x2000;
    timer->field_14 = 1;
    timer->field_18 = result;
    timer->field_1c = result;
    *(u16 *)0x04000000 = 1;
    GameFlag_SetBitFar(0x103);
    GameFlag_SetBitFar(0x169);
    Render_ResetTransformState();

    Dma3Fill(fill, scene, 76);
    Dma3Fill(fill, work, 0x82c);
    work->field_54 = -1;
    work->field_00 = arg;
    src = (u8 *)Runtime_AllocateBlock(37, 12);
    Dma3Fill(fill, src, 12);
    work->field_648 = (u16)Event_GetSpecialValueFar();
    Runtime_AllocateBlock(4, 0xe00);
    Runtime_AllocateBlock(3, 0x600);
    ObjectSystem_InitializeFar(4);
    if (GameFlag_IsSet(0x16e) != 0)
        UiWork_InitializeFar(1);
    else
        UiWork_InitializeFar(0);

    cam[0] = 0;
    cam[1] = 0x400000;
    cam[2] = 0;
    ((s32 *)scene)[0] = 0;
    ((s32 *)scene)[1] = 0xb40000;
    ((s32 *)scene)[2] = 0x400000;
    scene->yaw = 0x2800;
    scene->pitch = 0x5000;
    scene->distance = 0x1000000;
    object = BattleFormation_BuildEnemyList(work->field_00);

    if (GameFlag_IsSet(0x16c) != 0) {
        work->field_44 = 1;
        Battle_SetMode(4);
    }
    if (work->field_44 != 0) {
        BattleRandom_ClearSeed();
        for (wait = 0;;) {
            if ((gLinkStatus & 3) == 3)
                break;
            WaitFrames(1);
            wait++;
            if (wait > 24) {
                work->field_52 = 1;
                break;
            }
        }
        work->field_50 = (u8)((*(u32 *)0x04000128 << 26) >> 30);
        dst = gBattleOwnerStates;
        for (pos = 0; pos <= 0x7c7; pos++)
            *dst++ = gActorSpriteSlots[pos];
        object = GameFlag_GetByteFar(0x3f0);
        BattleParty_AssignMemberSlots();
        work->field_42 = 0;
    }
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_UpdateCamera, 0xc7f);

    off = 494;
    cue = *(s16 *)(gCell + off);
    if (cue != 0) {
        AudioCommand_PlayFar(cue);
        if (GameFlag_IsSet(0x16c) != 0) {
            AudioCommand_PlayFar(55);
            Sound_LoadPresetParameters(4);
        }
    } else {
        AudioCommand_PlayFar(51);
        AudioCommand_PlayFar(76);
    }

    BattleParty_CollectUnitList();
    BattleUnit_RefreshPlacement();
    BattlePlacement_UpdateEntries();
    BattleSummon_UpdateAvailability();
    if (*Trade_GetOfferStateFar(0) != 0)
        work->field_41 = 3;
    else
        work->field_41 = 1;
    UiWindow_CreateWithLayoutBoundsFar(9);
    Camera_InitDefaultTransform();
    BattleActor_CommitPlacement();
    BattlePresentation_InitializeWorkAndResetState();
    BattleBackground_Load(1, work->field_648, 0);
    BattleCamera_SetRange(0xa00000, 0x500000, 0, 0, 0x20000);
    BattlePres_SetupTransitionScene(0, 0, 0, 190);
    Party_ReservedNoOp5B14(1);
    *(u16 *)0x04000050 = 0;
    Summon_ClearWorkFields();
    work->field_54 = Resource_LoadIntoFreeSlot(128);
    work->field_45 = 0;
    if (GameFlag_IsSet(0x16e) != 0) {
        work->field_45 = 1;
    } else if (gCell[0x22b] == 0) {
        if ((BattleRandom16Far() & 15) == 0)
            work->field_45 = 1;
        else if ((BattleRandom16Far() & 31) == 0)
            work->field_45 = 2;
    }
    BattlePres_RunEntryTransition(object, arg);
    timer->field_14 = 0;
    gCpuLoadDisplayEnabled = 0;
    Scheduler_AddOrUpdateCallback((s32)Func_080b7738, 0xc80);

    for (;;) {
        Battle_ReservedNoOp9B2C();
        BattleSummon_UpdateAvailability();
        if (*Trade_GetOfferStateFar(0) != 0)
            work->field_41 = 3;
        else
            work->field_41 = 1;
        timer->value = 0x2800;
        timer->frames = 60;
        UiWindow_DrawPartyStatusContentsFar(work->field_41);
        /* __call_via_r3: clear the twenty action slots. */
        Iwram_ClearWords(work->actions, 0x140);
        Resource_ResetEntry(work->field_54);
        if (GameFlag_IsSet(0x16a) == 0) {
            Runtime_GetRemainingIwram();
            Runtime_GetRemainingEwram();
            cnt = BattlePresentation_BuildActions(work->actions);
            Runtime_GetRemainingIwram();
            Runtime_GetRemainingEwram();
        } else {
            cnt = BattlePresentation_BuildSortedUnitEntries(work->actions);
        }
        work->field_54 = Resource_LoadIntoFreeSlot(128);
        UiWindow_DrawPartyStatusContentsFar(work->field_41);
        if (cnt < 0)
            goto aborted;

        for (i = 0; i < cnt; i++) {
            actor = work->actions[i].h[0];
            Runtime_GetRemainingIwram();
            Runtime_GetRemainingEwram();
            if (GameFlag_IsSet(0x16a) == 0) {
                if (BattlePresentation_DispatchAction(&work->actions[i], i == 0 ? 10 : 0) == 1)
                    goto interrupted;
            } else {
                if (BattlePres_RunAction(&work->actions[i]) == 1)
                    goto interrupted;
            }
            Runtime_GetRemainingIwram();
            Runtime_GetRemainingEwram();
            if (BattleParty_ListLivingUnits(1, 0) == 0)
                goto party_lost;
            if (BattleParty_ListLivingUnits(2, 0) == 0) {
                if (actor <= 7 && work->field_538 == 1)
                    work->field_3e = 3;
                goto resolved;
            }
            if (BattlePres_SyncTurn() < 0)
                goto aborted;
        }

        work->field_45 = 0;
        Battle_ReservedNoOpF674();
        Battle_ProcessRoundEnd();
        BattleMotion_DestroyAllSlotObjects();
        if (work->field_44 != 0) {
            if (BattlePres_SyncTurn() < 0)
                goto aborted;
        } else {
            WaitFrames(20);
        }
        if (GameFlag_IsSet(0x16e) == 0)
            continue;

        handle = UiText_OpenMessageWindowFar((s32)&MsgTutorialUnleashed, 0, 4, 1);
        while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
        UiWork_FinalizeFar(handle, 1);
        WaitFrames(1);
        handle = UiText_OpenMessageWindowFar((s32)&MsgTutorialStandby, 10, 4, 1);
        BattlePresentation_WaitForPromptAt(92, 24);
        UiWork_FinalizeFar(handle, 1);
        WaitFrames(1);
    }

resolved:
    Battle_ApplyValueToWork2224();
    if (GameFlag_IsSet(0x16e) == 0) {
        if (work->field_44 != 0)
            AudioCommand_PlayFar(58);
        if (work->field_538 != 0) {
            AudioCommand_PlayFar(58);
            if (work->field_3e <= 1) {
                BattleUnit_AssignFar(128, work->msg_ids[work->field_3c], 26);
                UiWork_ClearValueNameTablesFar();
                UiWork_PushValueSlotFar(128, 1);
                UiText_ShowMessageAndWaitCoreFar(work->field_3e + (s32)&MsgDowned);
                BattlePresentation_WaitForAdvance();
            }
        }
        Battle_AwardSpoils();
    }
    AudioCommand_PlayFar(17);
    Blend_SetDarkenTarget16(30);
    Blend_WaitForTransition();
    result = work->field_538;
    goto finished;

aborted:
    Battle_ApplyValueToWork2224();
    Scheduler_EnableCallbacks(0);
    *(u16 *)0x04000000 = 1;
    result = work->field_538;
    GameFlag_SetBitFar(0x3e8);
    goto finished;

party_lost:
    Battle_ApplyValueToWork2224();
    AudioCommand_PlayFar(59);
    UiWork_ClearValueNameTablesFar();
    UiWork_PushValueSlotFar(gCell[504], 1);
    if (BattleParty_PrepareActiveOwners(0) == 1)
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgGoesDownInDefeat);
    else
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgPartyDefeated);
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
    Battle_SetMode(0);
    Scheduler_RemoveCallback((s32)Func_080b7738);
    Runtime_ReleaseHeapBlock10();
    return result;
}

s32 Party_CountActiveOwnersFar(void);

/* Caps the active party at four owners, or three in the alternate battle mode,
 * optionally writes their identifiers with a 0xff terminator, marks each
 * selected battle unit with status 2, and returns the selected count. */
s32 BattleParty_PrepareActiveOwners(u16 *owners)
{
    s32 limit;
    s32 count;
    s32 index;

    limit = 4;
    if (gBattleWork->two_sided != 0)
        limit = 3;

    count = Party_CountActiveOwnersFar();
    if (count > limit)
        count = limit;

    for (index = 0; index < count; index++) {
        s32 owner = gGameState.active_owners[index];

        if (owners != 0)
            *owners++ = owner;
        Owner_GetStateFar(owner)->status_12a = BATTLE_UNIT_PARTY;
    }

    if (owners != 0)
        *owners = BATTLE_UNIT_LIST_END;
    return count;
}

s32 BattleParty_ListPresentEnemies(s16 *unit_ids)
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
            if (Owner_GetStateFar(id)->status_12a != 0) {
                *output = (s16)id;
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
    struct BattleUnit *unit;

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
                hp = Owner_GetStateFar(unit_id)->hp;
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
                unit = Owner_GetStateFar(remaining);
                if ((unit->status_12a != 0) && ((s32)unit->hp > 0)) {
                    if (output != NULL) {
                        *output = (u16)remaining;
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

/* Counts the units in the lists selected by groups, bit 0 for the
 * party and bit 1 for the enemies, skipping removed 254 entries.
 * When dst is given, the unit ids are also written there and terminated
 * with 255. */
s32 BattleParty_ListActorIds(s32 groups, u16 *dst)
{
    struct BattleSession *order;
    s32 count;
    s32 i;

    order = gBattleWork;
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
        for (i = 0; order->enemy_units[i] != BATTLE_UNIT_LIST_END; i++) {
            if (order->enemy_units[i] != BATTLE_UNIT_REMOVED) {
                if (dst != NULL)
                    *dst++ = order->enemy_units[i];
                count++;
            }
        }
    }
    if (dst != NULL)
        *dst = BATTLE_UNIT_LIST_END;
    return count;
}
