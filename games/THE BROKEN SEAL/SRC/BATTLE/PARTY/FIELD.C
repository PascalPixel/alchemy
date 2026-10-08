#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "FIELDOBJ.H"
#include "TYPES.H"
#include "OWNERVAL.H"
#include "SCENE.H"
#include "BTLTYPE.H"
#include "BTLSYS.H"
#include "PARTY_STATE.H"
#include "SOUND_IDS.H"
#include "FIXED_MATH.H"
#include "EVENT_RUNTIME.H"
#include "OBJECT_RUNTIME.H"
#include "ITEMGIFT.H"
#include "IO_REG.H"
#include "DMA.H"
#include "GAME_STATE.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"
#include "SCENE_IDS.H"
#include "BTLCALC.H"
#include "CALLBACK_SCHEDULER.H"
#include "DISPTRAN.H"
#include "text/MSG_IDS.H"

#define gOverlayArea (*(struct OverlayEntries *)&gOverlayArea)

#define Map_LoadAreaGraphicsFar ((s32 (*)(void))Map_LoadAreaGraphicsFar)
#define ObjectTable_ResetForObject ((void (*)(void))ObjectTable_ResetForObject)
#define BattleFx_RunDescriptorAction ((void (*)(s32))BattleFx_RunDescriptorAction)
#define BattleFx_RunKind6DescriptorAction ((void (*)(s32))BattleFx_RunKind6DescriptorAction)
#define BattleAction_RunDescriptor ((void (*)(s32))BattleAction_RunDescriptor)
#define Battle_DispatchInputEvent ((void (*)(s32))Battle_DispatchInputEvent)
#define Field_RunTileAction ((void (*)(s32))Field_RunTileAction)
#define BattleCommand_ExecuteSelectedItem ((void (*)(s32,s32))BattleCommand_ExecuteSelectedItem)
#define BattleCommand_ExecuteSelectedAction ((void (*)(s32))BattleCommand_ExecuteSelectedAction)
#define Scene_FadeColorFromWhite ((void (*)(s32))Scene_FadeColorFromWhite)

#define gMenuCtrlWork (*(struct MenuCtrl * *)&gMenuCtrlWork)

s32 Party_CountActiveOwnersFar(void);
void BattleFx_ApplyColorToSourceBuffer(s32 color, s32 mode);
void Audio_PlayCue(s32 cue);

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))


void BattleParty_ApplyDrain(s32 amount)
{
    s32 target_count = Party_CountActiveOwnersFar();
    s32 i;

    for (i = 0; i < target_count; i++)
        UnitEpAddFar(gGameState.active_owners[i], amount);
}

void BattleParty_ApplyHealthDelta(s32 amount, s32 scaled)
{
    s32 count;
    s32 i;
    s32 value;
    struct BtlUnit *unit;

    if (amount < 0) {
        BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
        BattleFx_StartBufferInterpolation(4);
        if (amount < -10)
            Audio_PlayCue(SOUND_HEAVY_IMPACT);
        else
            Audio_PlayCue(133);
    } else {
        Audio_PlayCue(SOUND_RECOVERY);
    }

    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++) {
        unit = UnitGetFar(gGameState.active_owners[i]);
        if (!scaled) {
            value = amount;
        } else {
            value = unit->max_hp * amount / 100;
            if (value == 0) {
                value = amount;
                if (value < 0)
                    value = -value;
            }
        }
        Owner_AdjustFirstValueFar(gGameState.active_owners[i], value);
    }
}

s32 BattleParty_ApplyStatusDamage(void)
{
    s32 result = 0;
    s32 count = Party_CountActiveOwnersFar();
    s32 i;

    for (i = 0; i < count; i++) {
        struct BtlUnit *unit = UnitGetFar(gGameState.active_owners[i]);
        s32 amount;

        switch (unit->doku) {
        case 1:
            amount = -((unit->max_hp + 10) / 20);
            if (amount == 0)
                amount = -1;
            if (result <= 0)
                result = 1;
            break;
        case 2:
            amount = -((unit->max_hp + 5) / 10);
            if (amount == 0)
                amount = -1;
            if (result <= 1)
                result = 2;
            break;
        default:
            amount = 0;
            break;
        }
        Owner_AdjustFirstValueFar(gGameState.active_owners[i], amount);
    }

    if (result != 0) {
        BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
        BattleFx_StartBufferInterpolation(4);
        Audio_PlayCue(133);
    }

    return result;
}

void Battle_SetObjectFlag5bWhenMode3(void)
{
    struct EventRuntime *work;
    void *blk;

    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    if (work->mode_19e == 3) {
        blk = Runtime_AllocateBlock(0x1F, 0x540);
        if ((blk != NULL) && (FIELD_AT_OFFSET(blk, s8 *, 0x53D) != 0)) {
            FIELD_AT_OFFSET(blk, s8 *, 0x53A) = 0;
            FIELD_AT_OFFSET(blk, s8 *, 0x53B) = 0;
            FIELD_AT_OFFSET(blk, s8 *, 0x53C) = 1;
            FIELD_AT_OFFSET(blk, s8 *, 0x53D) = 0;
        }
        ((struct ObjectRuntime *)((struct EventWork *)work)->view_center)->movement_state = 1;
        Map_ResumeAnimationFar();
    }
}

void Battle_ClearObjectFlag5bWhenMode3(void)
{
    struct EventRuntime *work;

    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    if (work->mode_19e == 3) {
        Map_LoadAreaGraphicsFar();
        ((struct ObjectRuntime *)((struct EventWork *)work)->view_center)->movement_state = 0;
    }
}

struct FieldWork {
    u8 unknown_000[0x10];
    s32 view;
    struct FieldActor *actors[86];
    s16 action;
    s16 input;
    s16 exit;
    s16 menu;
    s16 examine;
    s16 start;
    u16 descriptor;
    s16 tile;
    s16 scene;
    s16 command;
    s16 item;
    s16 event;
    s16 count;
    s16 survived;
    s16 quantities[9];
    s16 item_argument;
    u8 unknown_19c[2];
    s16 mode;
    u8 unknown_1a0[8];
    s32 unknown_1a8;
    u8 unknown_1ac[4];
    s32 unknown_1b0;
    s32 unknown_1b4;
    u8 unknown_1b8[4];
    u8 *cells;
    s32 transition;
    u16 unknown_1c4;
    u16 transition_active;
    s32 transition_frames;
    u8 unknown_1cc[0xaea];
    u16 busy;
    u8 unknown_cb8[8];
    u16 unknown_cc0;
    u8 unknown_cc2[6];
    u16 unknown_cc8;
};

struct OverlayEntries {
    void *unknown_00;
    s32 (*start)(void);
    void *unknown_08[5];
    s32 (*place)(void);
    void *unknown_20;
    s32 (*view)(void);
};

struct MenuCtrl {
    u8 unknown_00[4];
    u16 wait;
};

void WaitFrames(s32 frames);
void Object_SetMode(struct FieldActor *actor, s32 mode);
void Object_ResetMotion(struct FieldActor *actor);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void UiText_DrawQuantity(s32 quantity, s32 mode);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void ObjectTable_Snapshot(void);
void Battle_ResetEffectCounter(void);
void Djinn_ResolvePendingEvent();
void AudioCommand_PlayFar(s32 cue);

extern u8 ResourceBlockOwners[];
extern volatile u32 gKeysHeld;
extern volatile u8 gDebugMode;

static __inline__ s32 Vitals_Clamp(s32 ratio)
{
    s32 clamped;

    if (ratio > 0x4000)
        clamped = 0x4000;
    else if (ratio < 0)
        clamped = 0;
    else
        clamped = ratio;
    return clamped;
}

s32 Field_RunScene(s32 entrance)
{
    volatile s32 zero;
    struct FieldWork *work;
    struct FieldActor *actor;
    s32 mode;
    s32 result = 0;
    s32 code;

    work = Runtime_AllocateBlock(27, sizeof(struct FieldWork));
    zero = 0;
    Dma_Set(&zero, work, 0x85000000 | (sizeof(struct FieldWork) / 4), REG_DMA3);
    GameFlag_ClearBitFar(0x103);
    gGameState.default_scene = gGameState.scene;
    gGameState.default_entrance = gGameState.entrance;
    gGameState.next_scene = -1;
    gGameState.next_entrance = -1;
    gGameState.defeat_scene = -1;
    gGameState.defeat_entrance = -1;
    Scheduler_ResetTaskTable();
    Djinn_ResolvePendingEvent(0);
    if ((s16)gGameState.map == (s32)&SceneId_Clear) {
        Map_InitializePerspectiveSceneFar();
        gGameState.perspective_scene = 1;
        mode = 3;
        BattleFx_SelectLocationRule(0);
    } else {
        Map_LoadLayeredSceneFar((s16)gGameState.map);
        mode = 2;
        BattleFx_SelectLocationRule(1);
    }
    work->mode = mode;
    ObjectSystem_InitializeFar(mode);
    FarCall_WindowTable();
    BattleFx_ResetCounters();
    work->view = gOverlayArea.view();
    BattleMap_ApplyEntranceView();
    gOverlayArea.place();
    ObjectTable_ResetForObject();
    if (GameFlag_IsSet(0x109) != 0)
        ObjectTable_Restore();
    if (gGameState.pending_djinn_event != 0)
        FieldObject_PlaceSceneActors();
    if (mode == 3) {
        Map_UpdateCurrentTileBlockFar();
        WorldMap_LoadGraphicsFar(gGameState.x, gGameState.z);
    } else {
        Map_ApplyWorkOriginAndSpanFar();
    }
    Battle_PlaceMapMarkers();
    BattleEffect_InitializeBuffers();
    BattleFx_ApplyColorToTargetBuffer(0x10000, 0);
    work->transition = 0x100;
    work->transition_frames = 16;
    work->transition_active = 0;
    work->unknown_1b0 = 0x199;
    work->unknown_1b4 = 0;
    if (gGameState.scene_variant == 2) {
        work->unknown_1b0 = 0;
        GameFlag_SetBitFar(0x144);
    }
    work->cells = Ram_MapCellBuffer;
    if (GameFlag_IsSet(0x109) == 0) {
        Party_ResolveTablePair();
        gGameState.hazard_limit = 16;
        gGameState.hazard_mode = 0;
        gGameState.hazard_damage = 1;
        gGameState.paused_object = -1;
        gGameState.cloaked = 0;
    }
    BattleFx_ScheduleCloakedMapCheck();
    work->unknown_cc8 = 0xffff;
    gOverlayArea.start();
    code = work->exit;
    if (code != 0) {
        result = code;
        work->exit = 0;
        goto done;
    }
    GameFlag_ClearBitFar(0x109);
    if (BattleFx_SumCounters() == 0) {
        if (work->transition_active == 0) {
            DisplayTransition_Start(work->transition, work->transition_frames);
            work->transition_active = 1;
            PLTT_BACKDROP = 0;
            if (ObjectEffect_RunPendingFlagEvent() == 0)
                WaitFrames((work->transition_frames + 1) / 2);
        }
        if (GameFlag_IsSet(0x12f) != 0) {
            GameFlag_ClearBitFar(0x12f);
            UiTimedNotice_CreateFar(gGameState.scene);
        }
        if (gGameState.pending_djinn_event != 0) {
            Djinn_ResolvePendingEvent(gGameState.pending_djinn_event, 1);
            gGameState.pending_djinn_event = 0;
        }
        if (gGameState.pending_item != 0) {
            Battle_SetObjectFlag5bWhenMode3();
            if (Party_CheckMemberValueTotal(gGameState.pending_item) == 0)
                PartyInventory_GiveItem(gGameState.pending_item, 0);
            Battle_ClearObjectFlag5bWhenMode3();
            gGameState.pending_item = 0;
        }
    }

    for (;;) {
        GameFlag_SetBitFar(0x104);
        actor = work->actors[gGameState.selected_actor];
        Object_ResetMotion(actor);
        gGameState.x = actor->x.fixed;
        gGameState.y = 0;
        gGameState.z = actor->z.fixed;
        gGameState.heading = actor->facing;
        gGameState.turn = actor->terrain_id;

        while (BattleFx_SumCounters() != 0) {
            s32 next = work->exit;

            if (next != 0) {
                result = next;
                work->exit = 0;
                goto done;
            }
            if (work->event != 0) {
                work->busy = 1;
                if (work->event == -1) {
                    u32 i;

                    Battle_InitializeRenderObject();
                    Battle_SetObjectFlag5bWhenMode3();
                    for (i = 0; i < work->count; i++) {
                        Object_SetMode(actor, 22);
                        UiText_DrawQuantity(work->quantities[i], 1);
                        UiText_ShowPositionedMessageAndWaitFar((s32)MsgFieldStrengthExhausted, 1);
                    }
                    if (work->survived == 0) {
                        struct BtlUnit *vitals;

                        if (gGameState.selected_actor == 0) {
                            if (GameFlag_IsSet(32) != 0)
                                Object_SetMode(actor, 21);
                            else
                                Object_SetMode(actor, 37);
                        } else {
                            Object_SetMode(actor, 19);
                        }
                        AudioCommand_PlayFar(59);
                        UiText_ShowPositionedMessageAndWaitFar((s32)MsgFieldPartyAnnihilated, 1);
                        vitals = UnitGetFar(gGameState.selected_actor);
                        vitals->hp = 1;
                        /* FAKEMATCH: needed to match: the one-pass block keeps the reloaded 1 below the hp store */
                        do {
                            vitals->hp_gauge = Vitals_Clamp((vitals->hp << 14) / vitals->max_hp);
                            if (vitals->hp_gauge == 0 && vitals->hp != 0)
                                vitals->hp_gauge = 1;
                        } while (0);
                        vitals->ep_gauge = Vitals_Clamp((vitals->ep << 14) / vitals->max_ep);
                        if (vitals->ep_gauge == 0 && vitals->ep != 0)
                            vitals->ep_gauge = 1;
                        gGameState.scene = gGameState.saved_scene;
                        gGameState.entrance = gGameState.saved_entrance;
                        Battle_ClearObjectFlag5bWhenMode3();
                        result = 999;
                        goto done;
                    }
                    Battle_ClearObjectFlag5bWhenMode3();
                } else if (work->event == -888) {
                    Battle_InitializeRenderObject();
                    Map_ShowWorldMap((s32)&ResourceId_WorldMapPicture);
                } else if (work->event == -889) {
                    Battle_InitializeRenderObject();
                    BattleFx_RunVisibilityTransition();
                } else {
                    BattleFx_RunKind6DescriptorAction(work->event);
                }
                work->busy = 0;
                work->event = 0;
            } else if (work->scene != 0) {
                Battle_InitializeRenderObject();
                ObjectTable_Snapshot();
                gGameState.return_cue = -1;
                gGameState.scene = 510;
                gGameState.entrance = work->scene;
                work->exit = 999;
                Scene_FadeColorFromWhite(work->scene);
                work->unknown_1a8 = 0;
                work->scene = 0;
                gGameState.encounter_steps = 0;
            } else if (work->action != 0) {
                work->busy = 1;
                BattleAction_RunDescriptor(work->action);
                work->busy = 0;
                work->action = 0;
            } else if (work->input != 0) {
                Battle_DispatchInputEvent(work->input);
                work->input = 0;
            } else if (work->examine != 0) {
                s32 object;

                UiTimedNotice_CloseIfActiveFar();
                object = BattleEffect_SelectNearbyObject(gGameState.selected_actor);
                mode = 0;
                if (object != -1) {
                    mode = BattleFx_FindDescriptorWithOverride(object) != 0;
                }
                if (mode != 0) {
                    work->descriptor = object | 0x1000;
                    work->menu = 0;
                } else {
                    mode = Object_GetTriggerTileAheadOfCurrent();
                    if (mode != 0) {
                        work->tile = mode;
                        work->menu = 0;
                    } else {
                        work->menu = 1;
                    }
                }
                work->examine = 0;
            } else if (work->menu != 0) {
                UiTimedNotice_CloseIfActiveFar();
                Battle_InitializeRenderObject();
                AudioCommand_PlayFar(111);
                Battle_SetObjectFlag5bWhenMode3();
                GameFlag_SetBitFar(0x106);
                if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0 && (gKeysHeld & KEY_SELECT) != 0) {
                    Menu_RunSelectionWithCursorObjectFar();
                } else if (GameFlag_IsSet(0x107) != 0) {
                    work->event = 250;
                } else {
                    work->unknown_cc0 = 0;
                    Menu_RunTopSelectionFar();
                    work->unknown_cc0 = 1;
                }
                Battle_ClearObjectFlag5bWhenMode3();
                GameFlag_ClearBitFar(0x106);
                GameFlag_RefreshLureCapFar();
                work->menu = 0;
            } else if ((s16)work->descriptor != 0) {
                Battle_SetObjectFlag5bWhenMode3();
                BattleFx_RunDescriptorAction(work->descriptor & 0xfff);
                Battle_ClearObjectFlag5bWhenMode3();
                work->descriptor = 0;
            } else if (work->tile != 0) {
                Battle_SetObjectFlag5bWhenMode3();
                Field_RunTileAction(work->tile);
                Battle_ClearObjectFlag5bWhenMode3();
                work->tile = 0;
            } else if (work->command != 0) {
                UiTimedNotice_CloseIfActiveFar();
                Battle_SetObjectFlag5bWhenMode3();
                BattleCommand_ExecuteSelectedAction(work->command);
                Battle_ClearObjectFlag5bWhenMode3();
                work->command = 0;
            } else if (work->item != 0) {
                Battle_SetObjectFlag5bWhenMode3();
                BattleCommand_ExecuteSelectedItem(work->item, work->item_argument);
                Battle_ClearObjectFlag5bWhenMode3();
                work->item = 0;
            } else if (work->start != 0) {
                AudioCommand_PlayFar(111);
                Battle_InitializeRenderObject();
                Battle_SetObjectFlag5bWhenMode3();
                GameFlag_SetBitFar(0x106);
                if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0) {
                    Menu_RunSelectionFar();
                } else if (gDebugMode != 0 && (gKeysHeld & KEY_L) != 0) {
                    Debug_RunPaletteEditor();
                } else if (GameFlag_IsSet(0x107) != 0) {
                    work->event = 250;
                } else {
                    Battle_ResetEffectCounter();
                    UiTimedNotice_CloseIfActiveFar();
                    gGameState.return_cue = AudioCommand_GetSecondaryStateByteFar();
                    if (GameFlag_IsSet(0x17e) == 0) {
                        struct MenuCtrl *ctrl = gMenuCtrlWork;
                        u8 *owner = ResourceBlockOwners;
                        s32 free = 0;
                        s32 n = 512;

                        do {
                            if (*owner++ == 0xff)
                                free++;
                        } while (--n != 0);
                        if (free - 136 < 0) {
                            ctrl->wait = 1;
                            WaitFrames(1);
                        }
                        Menu_RunWorkspaceResultLoopFar(0);
                        ctrl->wait = 0;
                    } else {
                        UiText_ShowPositionedMessageAndWaitFar((s32)MsgFieldPauseUnavailable, 1);
                    }
                }
                Battle_ClearObjectFlag5bWhenMode3();
                GameFlag_ClearBitFar(0x106);
                work->start = 0;
            }
        }

        GameFlag_ClearBitFar(0x104);
        actor = work->actors[gGameState.selected_actor];
        if (actor != NULL) {
            if (gGameState.movement_mode == 2)
                ObjectDispatch_InitFromTable0Far(actor);
            else if (gGameState.movement_mode == 1)
                ObjectDispatch_InitFromTable1Far(actor);
            else if (work->mode == 3)
                ObjectDispatch_InitFromTable3Far(actor);
            else
                ObjectDispatch_InitFromTable2Far(actor);
        }
        do {
            WaitFrames(1);
            actor = work->actors[gGameState.selected_actor];
            if (gDebugMode == 0 || GameFlag_IsSet(0x163) == 0)
                Field_ProcessStep(actor->terrain_id, actor->x.fixed, actor->y.fixed, actor->z.fixed);
        } while (BattleFx_SumCounters() == 0);
    }

done:
    if (work->transition_active != 0) {
        DisplayTransition_Finish(work->transition, work->transition_frames);
        work->transition_active = 0;
        WaitFrames(work->transition_frames);
    }
    Runtime_ReleaseHeapBlock(27);
    return result;
}
