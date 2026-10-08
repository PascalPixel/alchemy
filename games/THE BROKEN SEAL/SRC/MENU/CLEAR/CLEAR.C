#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "GLOBAL_CELLS.H"
#include "EDITION.H"
#include "TBS_EDITION.H"
#include "TEXT_FONT.H"
#include "TYPES.H"
#include "SCENE.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "FIELD_EVENT.H"
#include "RESOURCE_IDS.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "FIELD_SCENE.H"
#include "SERIAL_RUNTIME.H"

extern u16 gBgScroll[];

static __inline__ void DecodeBackground(const u8 *res)
{
    /* FAKEMATCH: an inline call wrapper keeps the decode's source in r4 and reloads the cell buffer's address after it, as the game does. */
    Resource_DecodeType01(res, (void *)Ram_MapCellBuffer);
}

struct ClearWork {
    u8 unknown_00[20];
    u16 mode;
};

#define DMA3 (REG_DMA3)

static __inline__ void RestoreInterrupts(u32 saved)
{
    REG_IME = saved;
}

#define QUEUE_WRITE(address, value)                                         \
    do {                                                                    \
        volatile u16 *ime;                                                  \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do { \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
            ime = &REG_IME;                                           \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = 0x20000;                                         \
        }                                                                   \
        RestoreInterrupts(saved);                                          \
    } while (0)

extern volatile u32 gKeysHeld;

u8 *SceneData_GetScriptTable(void)
{
    return Clear_ScriptTable;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Clear_MessageTable;
}

u8 *SceneData_GetActorTable(void)
{
    return Clear_ActorTable;
}

u8 *SceneData_GetEffectTable(void)
{
    return Clear_EffectTable;
}

void Clear_LoadBackground(void)
{
    u8 *res;
    s32 id;
    u16 *map;
    u32 x;
    u32 y;
    s32 tile;
    struct ScrollPair *scroll;
    s32 blank;
    u16 zero;

    id = (s32)&ResourceId_GoldenSunLogo;
    Engine_BlendSetDarkenTarget16(0);
    REG_BG2CNT = 0x681;
    gBgScroll[5] = 0;
    blank = 0x1ff;
    res = Engine_ResourceGetTableEntry(id);
    Dma_Set(res, (void *)0x05000000, 0x84000070, DMA3);
    res += 0x1c0;
    DecodeBackground(res);
    Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800, 0x84002580, DMA3);
    map = (u16 *)0x06003000;
    tile = 0x1a0;
    y = 0;
col:
    {
        x = 0;
    row:
        {
            s32 old = tile;

            /* FAKEMATCH: keep the signed tile wrap in the high half. */
            tile = ((old << 16) + 0x10000) >> 16;
            *map++ = old;
        }
        if (++x <= 29)
            goto row;
        *map++ = blank;
        *map++ = blank;
    }
    if (++y <= 19)
        goto col;
    scroll = (struct ScrollPair *)gBgScroll;
    for (y = 0; y <= 3; y++) {
        scroll->y = 0;
        scroll->x = 0;
        scroll++;
    }
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, DMA3);
    ((struct ClearWork *)gMapWork[0])->mode = 0x1400;
    {
        struct FieldActor *leader;

        leader = Object_GetById(gGameState.selected_actor);
        /* FAKEMATCH: a halfword zero keeps the reference's short-reach pool. */
        zero = 0;
        leader->motion_flags = zero;
    }
}

void Clear_UpdateBlend(void)
{
    /* FAKEMATCH: the one-pass IME read keeps the saved copy before masking; the count cast preserves the queue's original publication order. */
    struct IoWriteQueue *q;
    s32 frame;
    s32 level;

    frame = Clear_BlendFrame + 1;
    Clear_BlendFrame = frame;
    q = &gIoWriteQueue;
    level = (u16)frame >> 1;
    QUEUE_WRITE(0x4000050, 0x2e51);
    QUEUE_WRITE(0x4000052, ((16 - (u16)level) << 8) | (u16)level);
    if ((u16)level > 15)
        Engine_TaskRemoveCallback(Clear_UpdateBlend);
}

void Clear_CheckButtonCodes(void)
{
    /* FAKEMATCH: volatile keys make each test read them again, as the game does. */
    if (Clear_CodeUnlocked == 0) {
        if (Clear_CodeHeld != 0) {
            if (gKeysHeld == 0)
                Clear_CodeHeld = 0;
        } else if (gKeysHeld != 0) {
            if (gKeysHeld == Clear_CodeSequence[Clear_CodeProgress]) {
                Clear_CodeProgress++;
                Clear_CodeHeld = 1;
                if (Clear_CodeSequence[Clear_CodeProgress] == 0) {
                    Clear_CodeUnlocked = 1;
                    Engine_AudioPlayCue(110);
                }
            } else {
                Clear_CodeProgress = 0;
            }
        }
    }
    if (Clear_ExtraCodeUnlocked == 0) {
        if (Clear_ExtraCodeHeld != 0) {
            if (gKeysHeld == 0)
                Clear_ExtraCodeHeld = 0;
        } else if (gKeysHeld != 0) {
            if (gKeysHeld == Clear_ExtraCodeSequence[Clear_ExtraCodeProgress]) {
                Clear_ExtraCodeProgress++;
                Clear_ExtraCodeHeld = 1;
                if (Clear_ExtraCodeSequence[Clear_ExtraCodeProgress] == 0) {
                    Clear_ExtraCodeUnlocked = 1;
                    Engine_AudioPlayCue(110);
                }
            } else {
                Clear_ExtraCodeProgress = 0;
            }
        }
    }
}

void Clear_NameEntryCharacter(s32 cell, u8 *text)
{
    text[1] = 0;
    text[2] = 0;
#if EDITION_INTERNATIONAL
    if (cell <= 7)
        text[0] = cell + 'A';
    else if (cell <= 12)
        text[0] = cell + 'A' + 1;
    else if (cell <= 23)
        text[0] = cell + 'A' + 2;
    else if (cell <= 31)
        text[0] = cell + 26;
    else if (cell <= 42)
        text[0] = cell + 'A';
    else if (cell <= 44)
        text[0] = cell + 'A' + 1;
    else if (cell <= 55)
        text[0] = cell + 'A' + 2;
    else if (cell == 56)
        text[0] = '!';
    else if (cell == 57)
        text[0] = '?';
    else if (cell == 58)
        text[0] = '#';
    else if (cell == 59)
        text[0] = '&';
    else if (cell == 60)
        text[0] = '$';
    else if (cell == 61)
        text[0] = '%';
    else if (cell == 62)
        text[0] = '+';
    else
        text[0] = '=';
#else
    if (cell <= 14)
        text[0] = cell - 111;
    else if (cell <= 44)
        text[0] = cell - 47;
    else if (cell == 45)
        text[0] = 0x86;
    else {
        if (cell <= 55)
            text[0] = cell + 104;
        else if (cell == 56)
            text[0] = 0xe0;
        else if (cell == 57)
            text[0] = 0xe3;
        else if (cell == 58)
            text[0] = 0xe4;
        else
            text[0] = cell - 81;
        text[1] = TEXT_MARK_DAKUTEN;
    }
#endif
}

s32 Scene_GetModeMask(void)
{
    s32 mode;

    if (Engine_GameFlagIsSet(324) == 0) {
        return 0;
    }
    if (gGameState.scene_variant == 2) {
        return 0;
    }
    mode = gGameState.scene ^ (s32)&SceneId_WorldMap;
    return -((u32)(-mode | mode) >> 31);
}

struct SaveMenuSceneState {
    u8 unknown_000[0x100];
    s32 word_100;
};
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void GameFlag_ClearBit(s32 flag);
void UiWork_FinalizePendingCore(void);
s32 Scene_GetModeMask(void);
s32 Menu_RunConfirmSelection(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern u8 MsgNoBackupMemory[];
extern u8 MsgSaveAdventureQuestion[];
extern u8 MsgSaveDataCorrupted[];
extern u8 MsgSaveRecoveryFailed[];
extern u8 MsgClearCommunicationError[];
extern u8 MsgClearSendingData[];
extern u8 MsgClearPasswordLabel[];
extern u8 MsgClearSaveLabel[];

extern volatile s32 gKeyState;
extern volatile u16 gLinkStatus;
void RenderOutput_PrepareForRedraw(s32 window);

extern u8 gSaveBuffer[];
extern u8 gSleepDisabled;
extern u8 gDebugMode;
extern u8 gOptionMirror;
extern s16 Clear_CodeUnlocked;
extern s16 Clear_ExtraCodeUnlocked;
extern u8 *gWindowWork[];
void Clear_UpdateBlend(void);
void Clear_CheckButtonCodes(void);
struct SaveLinkGreeting {
    u16 code[4];
    u8 reserved[16];
};

extern struct SaveLinkGreeting gLinkPeerSignatures[4];
s32 Clear_EncodePassword();
void Clear_LoadBackground();
void Clear_NameEntryCharacter();
s32 Clear_PackPasswordData();
void Engine_AudioPlayCue(s32);
s32 Engine_GameFlagIsSet();
void Engine_EventCloseScreen();
void Event_SetDestination();
void Engine_EventOpenScreen(void);
void Engine_EventWaitForScreen();
void GameState_InitDefaults();
void Graphics_ExpandVramTilesByColorTable();
void InventorySnapshot_Restore(void);
void Link_DrawShiftedTilePair();
s32 Menu_SelectResourceLayout(s32);
s32 Menu_SelectSaveSlotAction();
s32 NameEntry_EditOwnerName();
s32 Object_CallSpawnRoutineAtOrigin(s32);
void PaletteGlow_Update();
void Party_AddActiveOwner();
void Party_RemoveActiveOwner(s32);
void Party_SetFlag32AndRefreshMembers();
s32 Runtime_GetBuildStampTime();
void SaveState_CopySlotPair();
void SaveState_DeleteSelectedSlot();
s32 SaveState_LoadRecordIntoWork();
s32 SaveState_ScanRecordFlags();
s32 Save_WriteSelectedSlot();
s32 SceneData_GetBufferCrc16();
s32 Engine_TaskAddCallback();
s32 SerialRuntime_BeginTransferA();
void SerialRuntime_Initialize();
void SerialRuntime_RemoveIrqHandlers();
void UiText_DrawCharacterAtOffset();
void UiText_DrawResource();
void UiText_DrawString();
void UiText_ShowCenteredMessage();
void UiWindow_DrawDividerLine();
void Ui_LoadWindowGraphics();
void Engine_TaskWait();
s32 __divsi3(s32, s32);
s32 __modsi3();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    /* FAKEMATCH: a forwarding call keeps 0x109 out of a register for SaveMenu_Run's later flag clears (differ 85 called directly). */
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    /* FAKEMATCH: a forwarding call for SaveMenu_Run's 0x952 flag test (differ 10 called directly). */
    return f(a0);
}

s32 SaveMenu_Run(void)
{
    s32 page_count;
    s32 rec;
    s32 encoded_size;
    s32 page_window;
    s32 rec5;
    s32 rec7;
    s32 record;
    s32 value;
    s32 v5;
    s32 v7;
    s32 count;
    s32 v6;
    s32 link_message;
    s32 v1;
    s32 v3;
    s32 v4;
    s32 n;
    s32 page_message;
    s32 page;
    s32 x;
    s32 started;
    s32 help_window;
    s32 heading_window;
    u8 save_data[200];
#if EDITION_INTERNATIONAL
    u8 password[320];
#else
    u8 password[260];
#endif
    u8 glyph[4];

    started = 0;
    Clear_LoadBackground();
    Engine_TaskAddCallback((s32)Clear_CheckButtonCodes, 0xc80);
    *(s32 *)((*(u8 **)&gEventWork + 0x1c0)) = started;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    if (gGameState.entrance == 2) {
    L_ask:
        UiText_ShowPositionedMessageAndWait((s32)MsgSaveAdventureQuestion, 5);
        v7 = Object_CallSpawnRoutineAtOrigin(1);
        UiWork_FinalizePendingCore();
        if (v7 == 0) {
            UiText_ShowPositionedMessageAndWait(((s32)MsgSaveAdventureQuestion + 1), 1);
            gGameState.unknown_20f = 1;
            v7 = Save_WriteSelectedSlot();
            if (v7 == -1) {
                goto L_ask;
            }
        }
        ((s32 (*)(s32))Engine_EventWait)(60);
        Engine_EventCloseScreen();
        Engine_AudioPlayCue(17);
        ((s32 (*)(s32))Engine_EventWait)(0x12c);
        Event_SetDestination((s32)&SceneId_WorldMap, 72);
        return 0;
    } else {
        gSleepDisabled = started;
    }
    L_020004a8:;
    v6 = SaveState_ScanRecordFlags();
    if (v6 < 0) {
        if (gDebugMode != 0) {
            gGameState.saved_options = 1;
            gOptionMirror = 1;
            UiText_ShowCenteredMessage((s32)MsgNoBackupMemory, 1, 8);
        }
    }
    if (v6 == 0) {
        if (started == 0) {
            Engine_TaskWait(30);
            Engine_TaskAddCallback((s32)Clear_UpdateBlend, 0xc80);
            Engine_TaskWait(1);
            started = 1;
        }
    }
    if (v6 > 0) {
        v6 = Menu_SelectSaveSlotAction();
    } else {
        v6 = 0;
    }
    if (v6 != 0) {
        goto L_0200057a;
    }
    GameState_InitDefaults();
    PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
    v5 = 1;
    v7 = 0;
    L_02000522:;
    Engine_TaskWait(6);
    v6 = NameEntry_EditOwnerName(v7);
    if (v6 == -1) {
        if (v7 == 0) {
            goto L_020004a8;
        }
        v7--;
        goto L_02000522;
    }
    if (Clear_CodeUnlocked != 0) {
        v5 = 4;
    }
    if (Clear_ExtraCodeUnlocked != 0) {
        v5 = 7;
    }
    v7 = (v7 + 1);
    if (v7 < v5) {
        goto L_02000522;
    }
    Party_SetFlag32AndRefreshMembers();
    gGameState.scene = (s32)&SceneId_HaidiaBabi;
    gGameState.entrance = 20;
    goto L_02000c96;
    L_0200057a:;
    if (v6 != 1) {
    } else {
        struct GameState *state;

        v6 = SaveState_LoadRecordIntoWork(1);
        if (v6 == -1) {
            goto L_020004a8;
        }
        /* FAKEMATCH: called directly, CSE keeps 0x109 in r7 for the later clears; the native derives them from 0x1c2 (522 vs 573). */
        Call1((void (*)())Engine_GameFlagSet, 0x109);
        state = &gGameState;
        PaletteGlow_Update(state->palette_glow[0], state->palette_glow[1]);
        if (state->build_stamp != Runtime_GetBuildStampTime()) {
            state->scene = state->saved_scene;
            state->entrance = state->saved_entrance;
            GameFlag_ClearBit(0x109);
        } else if ((gKeysHeld & 0x208) == 0x208) {
            if (Scene_GetModeMask() != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveRecoveryFailed, 9);
                goto L_020004a8;
            }
            state->scene = state->saved_scene;
            state->entrance = state->saved_entrance;
            GameFlag_ClearBit(0x109);
            ((void (*)(s32 flag))Engine_GameFlagSet)(0x13e);
        } else if (state->play_time != ((struct SaveMenuSceneState *)gSceneState)->word_100) {
            UiText_ShowPositionedMessageAndWait((s32)MsgSaveDataCorrupted, 9);
            UiText_ShowPositionedMessageAndWait((s32)MsgSaveDataCorrupted + 1, 13);
            if (Menu_RunConfirmSelection(1, 0, 0, 0) != 0) {
                UiWork_FinalizePendingCore();
                goto L_020004a8;
            }
            UiWork_FinalizePendingCore();
            if (Scene_GetModeMask() != 0) {
                /* FAKEMATCH: MsgSaveRecoveryFailed as MsgSaveDataCorrupted + 2, the native adds 2 to the register holding message 4 (186 by name). */
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveDataCorrupted + 2, 9);
                goto L_020004a8;
            }
            state->scene = state->saved_scene;
            state->entrance = state->saved_entrance;
            GameFlag_ClearBit(0x109);
            ((void (*)(s32 flag))Engine_GameFlagSet)(0x13f);
        }
        GameFlag_ClearBit(0x106);
        goto L_02000c96;
    }
    if (v6 == 2) {
        SaveState_CopySlotPair();
        goto L_020004a8;
    }
    if (v6 == 3) {
        SaveState_DeleteSelectedSlot();
        goto L_020004a8;
    }
    if (v6 == 4) {
        v6 = SaveState_LoadRecordIntoWork(4);
        if (v6 == -1) {
            goto L_020004a8;
        }
        gGameState.selected_actor = 0;
        /* FAKEMATCH: called directly, the flag test differs by 10 instructions (2026-10-07); through the wrapper it matches. */
        if (Value1(Engine_GameFlagIsSet, 0x952) != 0) {
            InventorySnapshot_Restore();
            Party_RemoveActiveOwner(0);
            Party_RemoveActiveOwner(1);
            Party_RemoveActiveOwner(2);
            Party_RemoveActiveOwner(3);
            Party_AddActiveOwner(0);
            Party_AddActiveOwner(1);
            Party_AddActiveOwner(2);
            Party_AddActiveOwner(3);
        }
        PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
        GameFlag_ClearBit(0x109);
        GameFlag_ClearBit(0x106);
        ((void (*)(s32 flag))Engine_GameFlagSet)(0x17e);
        gSleepDisabled = 1;
        Event_SetDestination((s32)&SceneId_LinkLobby, 1);
        goto L_02000c96;
    } else {
        if (v6 != 5) {
            goto L_020004a8;
        }
        L_020007b8:;
        v6 = SaveState_LoadRecordIntoWork(5);
        if (v6 == -1) {
            goto L_020004a8;
        }
        PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
        L_020007dc:;
        v6 = Menu_SelectResourceLayout(0);
        if (v6 == -1) {
            goto L_020007b8;
        }
        if (v6 != 1) {
            goto L_020009cc;
        }
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        v6 = UiWindow_Create(4, 5, 22, 8, 2);
#else
        v6 = UiWindow_Create(6, 5, 18, 8, 2);
#endif
        link_message = (s32)MsgClearSaveLabel;

        UiText_DrawResource(link_message, v6, 0, 4);
        UiText_DrawResource((link_message + 1), v6, 0, 16);
        UiText_DrawResource((link_message + 3), v6, 0, 36);
        SerialRuntime_Initialize();
        Engine_TaskWait(10);
        {
            u16 *sync = (u16 *)gSerialTransfer.payload;

            sync[0] = 0x30;
            sync[1] = 0x30;
            sync[2] = 0x30;
            sync[3] = 0x30;
        }
        v7 = 0;
        v5 = 3;
        for (v1 = 0; v1 != 4; v1++) {
            u16 *greeting = gLinkPeerSignatures[v1].code;

            greeting[0] = 0x30;
            greeting[1] = 0x30;
            greeting[2] = 0x30;
            greeting[3] = 0x30;
        }
        while ((gKeyState & KEY_B) == 0) {
            if ((v5 & gLinkStatus) == v5) {
                u32 control = REG_SIOCNT;
                u32 peer = ((control << 26) >> 30) ^ 1;
                u16 *received = gLinkPeerSignatures[peer].code;

#if defined(TBS_EDITION_DE)
                if (received[0] == 69) {
#elif defined(TBS_EDITION_FR)
                if (received[0] == 70) {
#elif defined(TBS_EDITION_ES)
                if (received[0] == 83) {
#elif defined(TBS_EDITION_IT)
                if (received[0] == 73) {
#elif !EDITION_INTERNATIONAL
                if (received[0] == 67) {
#else
                if (received[0] == 85) {
#endif
                    if (received[1] == 86) {
                        if (received[2] == 84) {
                            if (received[3] == 83) {
                                v7 = 1;
                                goto L_020008e0;
                            }
                        }
                    }
                }
            }
            Engine_TaskWait(1);
        }
        Engine_AudioPlayCue(113);
        L_020008e0:;
        UiWork_Finalize(v6, 2);
        if (v7 == 0) {
            goto L_020007dc;
        }
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        v6 = UiWindow_Create(4, 10, 22, 4, 2);
#else
        v6 = UiWindow_Create(5, 10, 20, 4, 2);
#endif

        UiText_DrawResource((s32)MsgClearSendingData, v6, 0, 4);
        Engine_TaskWait(10);
        SerialRuntime_BeginTransferA((s32)gSaveBuffer, 0x1004);
        Engine_TaskWait(10);
        v5 = 0;
        v1 = 3;
        v4 = 1;
        for (v7 = 0; v7 <= 0x927bf; v7++) {
            v5 = (v5 + 1);
            if ((v1 & gLinkStatus) == v1) {
                v5 = 0;
            }
            if (v5 == 10) {
                v4 = 0;
                break;
            }
            if (gSerialSendSource == 0) {
                break;
            }
            Engine_TaskWait(1);
        }
    }
    L_0200095e:;
    if (v4 == 0) {
        RenderOutput_PrepareForRedraw(v6);
        UiText_DrawResource((s32)MsgClearCommunicationError, v6, 0, 4);
        v5 = 1;
        do {
            Engine_TaskWait(1);
        } while ((gKeyState & v5) == 0);
    }
    Engine_TaskWait(10);
        SerialRuntime_RemoveIrqHandlers();
    Engine_TaskWait(10);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    UiWork_Finalize(v6, 2);
    Engine_TaskWait(1);
#else
    RenderOutput_PrepareForRedraw(v6);
    UiWork_Finalize(v6, 2);
#endif
    goto L_020004a8;
    L_020009cc:;
    if (v6 == 0) {
        L_020009d2:;
        v6 = Menu_SelectResourceLayout(1);

        if (v6 == -1) {
            goto L_020007dc;
        }
        {
            u8 *data = save_data;
            s16 sum;

            encoded_size = Clear_PackPasswordData(0, v6, data);
            sum = SceneData_GetBufferCrc16(encoded_size, data);
            save_data[encoded_size] = (u16)sum >> 8;
            save_data[encoded_size + 1] = sum;
            encoded_size += 2;
            encoded_size = Clear_EncodePassword(data, encoded_size, password);
        }
        v6 = 2;
        Ui_LoadWindowGraphics();
        page_window = UiWindow_Create(5, 4, 20, 12, v6);
        page = 0;
        page_count = (__divsi3(encoded_size, 50) + 1);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        record = UiWindow_Create(8, 0, 13, 4, v6);
#else
        record = UiWindow_Create(10, 0, 10, 4, v6);
#endif
        page_message = (s32)MsgClearPasswordLabel;
        heading_window = record;
        UiText_DrawResource(page_message, heading_window, 6, 4);
        v7 = 1;
        if (page_count == 1) {
            record = UiWindow_Create(5, 16, 20, 3, v6);
            help_window = record;
#if EDITION_INTERNATIONAL
            UiText_DrawCharacterAtOffset(page_message - 2, help_window, 80, 0);
#else
            UiText_DrawCharacterAtOffset(page_message - 2, help_window, 0, 0);
#endif
        } else {
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            record = UiWindow_Create(0, 16, 30, 3, v6);
#elif !EDITION_INTERNATIONAL
            record = UiWindow_Create(3, 16, 24, 3, v6);
#else
            record = UiWindow_Create(1, 16, 28, 3, v6);
#endif
            help_window = record;
            UiText_DrawCharacterAtOffset(page_message - 1, help_window, 0, 0);
        }
        Graphics_ExpandVramTilesByColorTable(0x6006000);
        RenderOutput_PrepareForRedraw(page_window);
        for (;;) {
        Link_DrawShiftedTilePair(0x6002500);
        if ((gKeyState & KEY_B) != 0) {
            Engine_AudioPlayCue(113);
            RenderOutput_PrepareForRedraw(page_window);
            UiWork_Finalize(page_window, 2);
            UiWork_Finalize(help_window, 2);
            UiWork_Finalize(heading_window, 2);
            goto L_020009d2;
        }
        if ((gKeyState & KEY_A) != 0) {
            page = (page + 1);
            v7 = 1;
            if (page == page_count) {
                Engine_AudioPlayCue(112);
                RenderOutput_PrepareForRedraw(page_window);
                UiWork_Finalize(page_window, 2);
                UiWork_Finalize(help_window, 2);
                UiWork_Finalize(heading_window, 2);
                Engine_TaskWait(1);
                goto L_020004a8;
            }
            Engine_AudioPlayCue(111);
        } else {
            if ((gKeyState & KEY_LEFT) != 0) {
                if (page_count <= 1) {
                    goto L_02000b30;
                }
                Engine_AudioPlayCue(111);
                rec5 = page + page_count;
                rec5--;
            } else {
                L_02000b30:;
                if ((gKeyState & KEY_RIGHT) == 0) {
                    goto L_02000b58;
                }
                if (page_count <= 1) {
                    goto L_02000b58;
                }
                Engine_AudioPlayCue(111);
                rec5 = page + page_count;
                rec5++;
            }
            rec5 = __modsi3(rec5, page_count);
            v7 = 1;
            page = rec5;
        }
        L_02000b58:;
        if (v7 != 1) {
            v5 = page_message;
        } else {
            RenderOutput_PrepareForRedraw(page_window);
            v7 = 0;
            v5 = 2;
            do {
                UiWindow_DrawDividerLine(page_window, 0, v5, 18, v5);
                v5 = (v5 + 2);
                v7 = (v7 + 1);
            } while (v7 != 4);
            if (page_count > 1) {
                    v7 = 0;
                if (page_count != 0) {
                    v6 = 0;
                    do {
                        value = v7 + 0xf301;
                        if (v7 == page) {
                            value = v7 + 0xf30b;
                        }
                        UiWindow_SetTilemapEntry(page_window, value, v7 + 18 - page_count, -1, v6);
                        v7 = (v7 + 1);
                    } while (v7 != page_count);
                }
                v5 = 0;
                UiWindow_SetTilemapEntry(page_window, 0xf128, (17 - (s32)page_count), -1, 0);
                UiWindow_SetTilemapEntry(page_window, 0xf129, 18, -1, 0);
                gWindowWork[0][RENDER_DIRTY_OFS] |= (2 << ((u32)*(u16 *)(page_window + 14) >> 2));
            }
            v6 = page * 50 + 50;
            if (v6 > encoded_size) {
                v6 = encoded_size;
            }
            for (v7 = page * 50, n = 0; v7 != v6; v7++, n++) {
                /* FAKEMATCH: the loop note ends the CSE path, which otherwise carries the glyph address into both draw calls for gcse to hoist out of the loop; the native rebuilds sp + 24 at each call */
                do {
                    Clear_NameEntryCharacter(password[v7] & 63, glyph);
                } while (0);
                record = __modsi3(v7, 10);
                if (record > 4) {
                    s32 row;

                    rec7 = __modsi3(n, 10);
                    row = __divsi3(n, 10);
                    UiText_DrawString(glyph, page_window, rec7 * 12 + 18, (row << 4) + 2);
                } else {
                    s32 row;

                    rec7 = __modsi3(n, 10);
                    row = __divsi3(n, 10);
                    UiText_DrawString(glyph, page_window, rec7 * 12 + 8, (row << 4) + 2);
                }
            }
            v7 = 0;
        }
        Engine_TaskWait(1);
        }
    }
    Engine_TaskWait(0x12c);
    goto L_020004a8;
    L_02000c96:;
    /* FAKEMATCH: stored as a plain constant, 0x3e7 is a halfword pool load that forces a pool before the epilogue (differ 25); through an s32 local it joins the one pool after the function. */
    v3 = 0x3e7;
    *(u16 *)((*(u8 **)&gEventWork + 0x170)) = v3;
    ((s32 (*)(s32))Engine_EventWait)(30);
    Engine_AudioPlayCue(17);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    ((s32 (*)(s32))Engine_EventWait)(60);
    L_Return:;
    return 0;
}
