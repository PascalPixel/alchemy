/* Allocates battle work, plays the opening, and runs rounds to completion.
   2026-09-29 alchemy permute (seed 1, 3 jobs, 10 minutes): 28,144
   candidates; the best scored 3922 against 4257 (83 register-only, 2
   stack-only, 49 operand, 18 reordered, 5 inserted, 9 deleted) after 12
   rewrites (introduce a temporary, reorder local declarations, change loop
   form, pointer arithmetic or indexing), none of them kept. Separately, the
   callees now use the build's names where the address has exactly one,
   which alone takes the draft to 3757; four callees have none. */

#include "DMA.H"

struct BattleTimerWork {
    s32 value;                  /* 0x00 */
    s32 frames;                 /* 0x04 */
    u8 unk_08[12];              /* 0x08 */
    s32 field_14;               /* 0x14 */
    s32 field_18;               /* 0x18 */
    s32 field_1c;               /* 0x1c */
};

struct BattleSceneVector {
    s32 field_00;
    s32 field_04;
    s32 field_08;
};

struct BattleSceneWork {
    s32 field_00;               /* 0x00 */
    s32 field_04;               /* 0x04 */
    s32 field_08;               /* 0x08 */
    struct BattleSceneVector sub_0c; /* 0x0c */
    u8 unk_18[8];               /* 0x18 */
    s32 field_20;               /* 0x20 */
    u8 unk_24[16];              /* 0x24 */
    u16 field_34;               /* 0x34 */
    u16 field_36;               /* 0x36 */
    u8 unk_38[20];              /* 0x38 .. 0x4b */
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
    do {                                                                      \
        (cell) = 0;                                                           \
        Dma_Set((void *)&(cell), (dest), DMA_FILL32 | ((bytes) >> 2),         \
                (volatile u32 *)0x040000d4);                                  \
    } while (0)

extern u8 gCell[];
extern s32 gBattleRandomSeed;
extern volatile u16 gLinkStatus;
extern u8 *gBattleOwnerStates;
extern u8 gActorSpriteSlots[];
extern u8 Data_03001f58;
void BattlePresentation_UpdateCamera(void);
void Func_080b7738(void);

/* The relocated IWRAM block clear this owner reaches through __call_via_r3. */
#include "IWRAM_CALL.H"

void *Runtime_AllocateBlock(s32 tag, s32 size);
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
void BattleParty_AssignMemberSlots(void);
s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 order);
void AudioCommand_PlayFar(s32 cue);
void Sound_LoadPresetParameters(s32 value);
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
s32 Resource_LoadIntoFreeSlot(s32 size);
s32 BattleRandom16Far(void);
void Func_080c02a4(s32 object, s32 arg);
void Battle_ReservedNoOp9B2C(void);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
void Resource_ResetEntry(s32 entry);
void Runtime_GetRemainingIwram(void);
void Runtime_GetRemainingEwram(void);
s32 BattlePresentation_BuildActions(struct BattleActionSlot *slots);
s32 BattlePresentation_BuildSortedUnitEntries(struct BattleActionSlot *slots);
s32 BattlePresentation_DispatchAction(struct BattleActionSlot *slot, s32 delay);
s32 BattlePres_RunAction(struct BattleActionSlot *slot);
s32 BattleParty_ListLivingUnits(s32 side, s32 mode);
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
s32 BattleParty_PrepareActiveOwners(s32 side);
void BattleParty_ResetActiveRuntimeFields(void);
void BattlePlacement_UpdateTimedEntries(void);
void Scheduler_RemoveCallback(s32 callback);
void Runtime_ReleaseHeapBlock10(void);

s32 Battle_RunEncounter(s32 arg)
{
    struct BattleSceneWork *scene;
    struct BattleSceneVector *cam;
    struct BattleEncounterWork *work;
    struct BattleTimerWork *timer;
    u8 *buf;
    u8 *src;
    u8 *dst;
    s32 object;
    s32 cnt;
    s32 i;
    s32 delay;
    s32 handle;
    s32 ret;
    s32 wait;
    u32 pos;
    s32 off;
    u32 actor;
    s16 cue;
    u8 unused[48];
    s32 fill;

    scene = (struct BattleSceneWork *)Runtime_AllocateBlock(12, 76);
    work = (struct BattleEncounterWork *)Runtime_AllocateBlock(9, 0x82c);
    buf = (u8 *)Runtime_AllocateBlock(54, 0x7c8);
    timer = (struct BattleTimerWork *)Runtime_AllocateBlock(44, 32);
    Runtime_AllocateBlock(11, 0x280);
    cam = &scene->sub_0c;
    /* __call_via_r3 -> the IWRAM block clear at 0x03000164. */
    Iwram_ClearWords(buf, 0x7c8);
    Scheduler_ResetTaskTable();
    timer->frames = 0;
    timer->value = 0x2000;
    timer->field_14 = 1;
    timer->field_18 = 0;
    timer->field_1c = 0;
    *(u16 *)0x04000000 = 1;
    GameFlag_SetBitFar(0x103);
    GameFlag_SetBitFar(0x169);
    Render_ResetTransformState();

    Dma3Fill(fill, scene, 76);
    Dma3Fill(fill, work, 0x82c);
    work->field_54 = -1;
    work->field_00 = arg;
    dst = (u8 *)Runtime_AllocateBlock(37, 12);
    Dma3Fill(fill, dst, 12);
    work->field_648 = (u16)Event_GetSpecialValueFar();
    Runtime_AllocateBlock(4, 0xe00);
    Runtime_AllocateBlock(3, 0x600);
    ObjectSystem_InitializeFar(4);
    if (GameFlag_IsSet(0x16e) != 0)
        UiWork_InitializeFar(1);
    else
        UiWork_InitializeFar(0);

    cam->field_04 = 0x400000;
    cam->field_00 = 0;
    cam->field_08 = 0;
    scene->field_04 = 0xb40000;
    scene->field_08 = 0x400000;
    scene->field_00 = 0;
    scene->field_36 = 0x2800;
    scene->field_34 = 0x5000;
    scene->field_20 = 0x1000000;
    object = BattleFormation_BuildEnemyList(work->field_00);

    if (GameFlag_IsSet(0x16c) != 0) {
        work->field_44 = 1;
        gCell[0x22b] = 4;
    }
    if (work->field_44 != 0) {
        gBattleRandomSeed = 0;
        wait = 0;
        do {
            if ((gLinkStatus & 3) == 3)
                goto linked;
            wait++;
            WaitFrames(1);
        } while (wait <= 24);
        work->field_52 = 1;
    linked:
        work->field_50 = (u8)((*(u32 *)0x04000128 << 26) >> 30);
        src = gActorSpriteSlots;
        dst = gBattleOwnerStates;
        pos = 0;
        do {
            pos++;
            *dst = *src;
            src++;
            dst++;
        } while (pos <= 0x7c7);
        object = GameFlag_GetByteFar(0x3f0);
        BattleParty_AssignMemberSlots();
        work->field_42 = 0;
    }
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_UpdateCamera, 0xc7f);

    {
        off = 494;
        cue = *(s16 *)(gCell + off);
    }
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
    Func_080c02a4(object, arg);
    timer->field_14 = 0;
    Data_03001f58 = 0;
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
                delay = 10;
                if (i != 0)
                    delay = 0;
                if (BattlePresentation_DispatchAction(&work->actions[i], delay) == 1)
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

        handle = UiText_OpenMessageWindowFar(0xc47, 0, 4, 1);
        while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
        UiWork_FinalizeFar(handle, 1);
        WaitFrames(1);
        handle = UiText_OpenMessageWindowFar(0xc48, 10, 4, 1);
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
                UiText_ShowMessageAndWaitCoreFar(work->field_3e + 0x838);
                BattlePresentation_WaitForAdvance();
            }
        }
        Battle_AwardSpoils();
    }
    AudioCommand_PlayFar(17);
    Blend_SetDarkenTarget16(30);
    Blend_WaitForTransition();
    ret = work->field_538;
    goto finished;

aborted:
    Battle_ApplyValueToWork2224();
    Scheduler_EnableCallbacks(0);
    *(u16 *)0x04000000 = 1;
    ret = work->field_538;
    GameFlag_SetBitFar(0x3e8);
    goto finished;

party_lost:
    Battle_ApplyValueToWork2224();
    AudioCommand_PlayFar(59);
    UiWork_ClearValueNameTablesFar();
    off = 504;
    src = gCell + off;
    UiWork_PushValueSlotFar(*src, 1);
    if (BattleParty_PrepareActiveOwners(0) == 1)
        UiText_ShowMessageAndWaitCoreFar(0x83d);
    else
        UiText_ShowMessageAndWaitCoreFar(0x837);
    BattlePresentation_WaitForAdvance();
    AudioCommand_PlayFar(17);
    Blend_SetDarkenTarget16(30);
    ret = -1;
    Blend_WaitForTransition();
    goto finished;

interrupted:
    AudioCommand_PlayFar(17);
    Blend_SetDarkenTarget16(30);
    Blend_WaitForTransition();
    ret = 0x3e7;

finished:
    BattleParty_ResetActiveRuntimeFields();
    Battle_ReservedNoOpF674();
    BattlePlacement_UpdateTimedEntries();
    gCell[0x22b] = 0;
    Scheduler_RemoveCallback((s32)Func_080b7738);
    Runtime_ReleaseHeapBlock10();
    return ret;
}
