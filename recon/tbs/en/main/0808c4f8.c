#include "RUNTIME_MEM.H"
/* DRAFT of Func_0808c4f8, the field mode's entry and outer loop: sets up the
   scene, then alternates between walking (one Field_ProcessStep a frame)
   and serving the requests the field work block's slots post, until one
   posts an exit code.
   Not exact: 2424 of 2428 bytes, every call, branch and store in place.
   Remaining: (1) the reference hoists the game state's base out of the loop
   into r9 and loads the leader's address at each use; here the leader's
   address takes r9, which also puts r0 among the reload registers;
   (2) the reference clears the fill word from r7 with one more stack slot;
   (3) it compares the map with a pooled 1 (a halfword compare). */
#include "TYPES.H"
#include "IO_REG.H"
#include "DMA.H"
#include "GAME_STATE.H"
#include "RAM_BUFFER.H"

struct FieldActor {
    u8 unknown_00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[14];
    u8 turn;
};

struct FieldWork {
    u8 unknown_000[0x10];
    s32 view;
    struct FieldActor *actors[86];
    s16 action;                 /* 0x16c */
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
    s16 count;                  /* 0x184 */
    s16 survived;
    s16 quantities[9];          /* 0x188 */
    s16 item_argument;          /* 0x19a */
    u8 unknown_19c[2];
    s16 mode;                   /* 0x19e */
    u8 unknown_1a0[8];
    s32 unknown_1a8;
    u8 unknown_1ac[4];
    s32 unknown_1b0;
    s32 unknown_1b4;
    u8 unknown_1b8[4];
    u8 *cells;                  /* 0x1bc */
    s32 transition;             /* 0x1c0 */
    u16 unknown_1c4;
    u16 transition_active;      /* 0x1c6 */
    s32 transition_frames;      /* 0x1c8 */
    u8 unknown_1cc[0xaea];
    u16 busy;                   /* 0xcb6 */
    u8 unknown_cb8[8];
    u16 unknown_cc0;
    u8 unknown_cc2[6];
    u16 unknown_cc8;
};

struct OwnerVitals {
    u8 unknown_000[20];
    s16 hp_ratio;
    s16 pp_ratio;
    u8 unknown_018[28];
    s16 max_hp;
    s16 max_pp;
    s16 hp;
    s16 pp;
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

/* Four cells of the game state are reached by their own addresses. */
#define STATE_LEADER (*(s32 *)&gGameState.selected_actor)
#define STATE_TURN (*(u16 *)&gGameState.turn)
#define STATE_ENTRANCE (*(s16 *)&gGameState.entrance)
#define STATE_238 (*(s32 *)&gGameState.unknown_238)

void WaitFrames(s32 frames);
void Scheduler_ResetTaskTable(void);
void ObjectSystem_InitializeFar(s32 mode);
void Object_SetMode(struct FieldActor *actor, s32 mode);
void ObjectDispatch_InitFromTable2Far(struct FieldActor *actor);
void ObjectDispatch_InitFromTable3Far(struct FieldActor *actor);
void ObjectDispatch_InitFromTable1Far(struct FieldActor *actor);
void ObjectDispatch_InitFromTable0Far(struct FieldActor *actor);
void Map_LoadLayeredSceneFar(void);
void Map_InitializePerspectiveSceneFar(void);
void Map_ApplyWorkOriginAndSpanFar(void);
void Map_UpdateCurrentTileBlockFar(void);
void WorldMap_LoadGraphicsFar(s32 x, s32 z);
void Object_ResetMotion(struct FieldActor *actor);
void FarCall_WindowTable(void);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void UiText_DrawQuantity(s32 quantity, s32 mode);
void Menu_RunTopSelectionFar(void);
void UiTimedNotice_CreateFar(s32 scene);
void UiTimedNotice_CloseIfActiveFar(void);
void Menu_RunSelectionFar(void);
void Menu_RunSelectionWithCursorObjectFar(void);
void Menu_RunWorkspaceResultLoopFar(s32 mode);
struct OwnerVitals *Owner_GetStateFar(s32 owner);
s32 GameFlag_IsSet(s32 flag);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void GameFlag_RefreshLureCapFar(void);
void BattleFx_SelectLocationRule(s32 rule);
void Party_ResolveTablePair(void);
void ObjectTable_ResetForObject(void);
void ObjectTable_Snapshot(void);
void ObjectTable_Restore(void);
void BattleFx_ResetCounters(void);
s32 BattleFx_SumCounters(void);
void Field_ProcessStep(s32 turn, s32 x, s32 y, s32 z);
void Battle_SetObjectFlag5bWhenMode3(void);
void Battle_ClearObjectFlag5bWhenMode3(void);
s32 Object_GetTriggerTileAheadOfCurrent(void);
void BattleMap_ApplyEntranceView(void);
void Debug_RunPaletteEditor(void);
s32 BattleFx_FindDescriptorWithOverride(s32 object);
void BattleFx_RunDescriptorAction(s32 descriptor);
void BattleFx_RunKind6DescriptorAction(s32 event);
void BattleAction_RunDescriptor(s32 action);
void Battle_DispatchInputEvent(s32 input);
void Field_RunTileAction(s32 tile);
s32 BattleEffect_SelectNearbyObject(s32 actor);
void Battle_ResetEffectCounter(void);
void BattleCommand_ExecuteSelectedItem(s32 item, s32 argument);
void BattleCommand_ExecuteSelectedAction(s32 command);
void Battle_PlaceMapMarkers(void);
void DisplayTransition_Start(s32 mode, s32 frames);
void DisplayTransition_Finish(s32 mode, s32 frames);
void BattleEffect_InitializeBuffers(void);
void BattleFx_ApplyColorToTargetBuffer(s32 color, s32 mode);
void Battle_InitializeRenderObject(void);
s32 Party_CheckMemberValueTotal(s32 item);
void PartyInventory_GiveItem(s32 item, s32 owner);
void Scene_FadeColorFromWhite(s32 scene);
s32 ObjectEffect_RunPendingFlagEvent(void);
void FieldObject_PlaceSceneActors(void);
void Djinn_ResolvePendingEvent();
void BattleFx_ScheduleCallbackWhenValue24cSet(void);
void Map_ShowWorldMap(s16 slot);
void BattleFx_RunVisibilityTransition(void);
void AudioCommand_PlayFar(s32 cue);
s32 AudioCommand_GetSecondaryStateByteFar(void);

extern struct OverlayEntries gOverlayArea;
extern u8 ResourceBlockOwners[];
extern volatile u32 gKeysHeld;
extern struct MenuCtrl *gMenuCtrlWork;
extern volatile u8 gDebugMode;

static __inline__ s32 Vitals_Ratio(s32 value, s32 max)
{
    s32 ratio = (value << 14) / max;
    s32 clamped = 0x4000;

    if (ratio <= 0x4000) {
        clamped = 0;
        if (ratio >= 0)
            clamped = ratio;
    }
    return clamped;
}

s32 Func_0808c4f8(void)
{
    volatile s32 zero;
    struct FieldWork *work;
    struct FieldActor *actor;
    s32 mode;
    s32 result;
    s32 code;

    work = Runtime_AllocateBlock(27, sizeof(struct FieldWork));
    zero = 0;
    Dma_Set(&zero, work, 0x85000000 | (sizeof(struct FieldWork) / 4), (volatile u32 *)0x040000d4);
    GameFlag_ClearBitFar(0x103);
    gGameState.default_scene = gGameState.scene;
    gGameState.default_entrance = gGameState.entrance;
    gGameState.next_scene = -1;
    gGameState.next_entrance = -1;
    gGameState.defeat_scene = -1;
    gGameState.defeat_entrance = -1;
    Scheduler_ResetTaskTable();
    Djinn_ResolvePendingEvent(0);
    if ((s16)gGameState.map == 1) {
        Map_InitializePerspectiveSceneFar();
        gGameState.perspective_scene = 1;
        mode = 3;
        BattleFx_SelectLocationRule(0);
    } else {
        Map_LoadLayeredSceneFar();
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
    if (gGameState.unknown_23e == 2) {
        work->unknown_1b0 = 0;
        GameFlag_SetBitFar(0x144);
    }
    work->cells = Ram_MapCellBuffer;
    if (GameFlag_IsSet(0x109) == 0) {
        Party_ResolveTablePair();
        gGameState.unknown_22c = 16;
        gGameState.unknown_22e = 0;
        gGameState.unknown_230 = 1;
        gGameState.unknown_24a = -1;
        gGameState.cloaked = 0;
    }
    BattleFx_ScheduleCallbackWhenValue24cSet();
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
            *(u16 *)0x05000000 = 0;
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
        actor = work->actors[STATE_LEADER];
        Object_ResetMotion(actor);
        gGameState.x = actor->x;
        gGameState.y = 0;
        gGameState.z = actor->z;
        gGameState.heading = actor->angle;
        STATE_TURN = actor->turn;

        while (BattleFx_SumCounters() != 0) {
            code = work->exit;
            if (code != 0) {
                work->exit = 0;
                result = code;
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
                        UiText_ShowPositionedMessageAndWaitFar(0x91a, 1);
                    }
                    if (work->survived == 0) {
                        struct OwnerVitals *vitals;

                        if (STATE_LEADER == 0) {
                            if (GameFlag_IsSet(32) != 0)
                                Object_SetMode(actor, 21);
                            else
                                Object_SetMode(actor, 37);
                        } else {
                            Object_SetMode(actor, 19);
                        }
                        AudioCommand_PlayFar(59);
                        UiText_ShowPositionedMessageAndWaitFar(0x91b, 1);
                        vitals = Owner_GetStateFar(STATE_LEADER);
                        vitals->hp = 1;
                        vitals->hp_ratio = Vitals_Ratio(vitals->hp, vitals->max_hp);
                        if (vitals->hp_ratio == 0 && vitals->hp != 0)
                            vitals->hp_ratio = 1;
                        vitals->pp_ratio = Vitals_Ratio(vitals->pp, vitals->max_pp);
                        if (vitals->pp_ratio == 0 && vitals->pp != 0)
                            vitals->pp_ratio = 1;
                        gGameState.scene = gGameState.saved_scene;
                        STATE_ENTRANCE = gGameState.saved_entrance;
                        Battle_ClearObjectFlag5bWhenMode3();
                        result = 999;
                        goto done;
                    }
                    Battle_ClearObjectFlag5bWhenMode3();
                } else if (work->event == -888) {
                    Battle_InitializeRenderObject();
                    Map_ShowWorldMap(27);
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
                STATE_ENTRANCE = work->scene;
                work->exit = 999;
                Scene_FadeColorFromWhite(work->scene);
                work->unknown_1a8 = 0;
                work->scene = 0;
                STATE_238 = 0;
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
                s32 found;

                UiTimedNotice_CloseIfActiveFar();
                object = BattleEffect_SelectNearbyObject(STATE_LEADER);
                found = 0;
                if (object != -1) {
                    found = BattleFx_FindDescriptorWithOverride(object);
                    if (found != 0)
                        found = 1;
                }
                if (found != 0) {
                    work->descriptor = object | 0x1000;
                    work->menu = 0;
                } else {
                    s32 tile = Object_GetTriggerTileAheadOfCurrent();

                    if (tile != 0) {
                        work->tile = tile;
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
                        UiText_ShowPositionedMessageAndWaitFar(0xc2f, 1);
                    }
                }
                Battle_ClearObjectFlag5bWhenMode3();
                GameFlag_ClearBitFar(0x106);
                work->start = 0;
            }
        }

        GameFlag_ClearBitFar(0x104);
        actor = work->actors[STATE_LEADER];
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
            actor = work->actors[STATE_LEADER];
            if (gDebugMode == 0 || GameFlag_IsSet(0x163) == 0)
                Field_ProcessStep(actor->turn, actor->x, actor->y, actor->z);
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
