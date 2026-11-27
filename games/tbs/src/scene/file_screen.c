// The File Screen overlay, the first screen after the title.
//
// It holds the Golden Sun logo, the start menu (New Game, Continue, Copy and Erase), the door to
// the Battle Arena and the sending of a cleared game's data to The Lost Age, by link cable or as a
// password. Two secret button codes on the screen (scFileScreenCheckButtonCodes) name more of the
// party.

#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "resource_ids.h"
#include "debug/debug.h"
#include "field/script.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/title.h"
#include "sound/sound_driver_symbols.h"
#include "game/system_call.h"
#include "window/text_font.h"
#include "text/msg_ids.h"

Sint32             scInit(void);
extern Uint32      FileScreenExits[];
extern ACTOR_PLACE FileScreenActorPlacements[];
extern CUE         FileScreenCues[];
extern Sint16      FileScreenBlendTick;
extern Sint16      FileScreenCodeUnlocked;
extern Sint16      FileScreenExtraCodeUnlocked;
extern Sint16      FileScreenCodeProgress;
extern Sint16      FileScreenExtraCodeProgress;
extern Sint16      FileScreenCodeHeld;
extern Sint32      FileScreenExtraCodeHeld;
extern Uint16      FileScreenPasswordStackItems[];

// The password buffer: a Gold password is 260 symbols (scFileScreenEncodePassword) in every
// edition, which the Japanese buffer holds exactly. XXX (no evidence): why the other editions
// reserve 320; nothing in this game writes or reads past 260, so the bytes show only the space.
#define FILE_SCREEN_PASSWORD_BUFFER_SIZE WHEN(LARGER_PASSWORD_BUFFER, 320, 260)

// A cable transfer's window ("Save", "Preparing data" and "B: Quit"), in tiles.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH     \
    || LANGUAGE == LANGUAGE_ITALIAN
#    define FILE_SCREEN_LINK_WINDOW_X     4
#    define FILE_SCREEN_LINK_WINDOW_WIDTH 22
#else
#    define FILE_SCREEN_LINK_WINDOW_X     6
#    define FILE_SCREEN_LINK_WINDOW_WIDTH 18
#endif

// The first letter of the greeting a cable transfer waits for, before V, T and S (scInit).
#define FILE_SCREEN_LINK_LANGUAGE_TAG BY_LANGUAGE('C', 'U', 'E', 'S', 'F', 'I')

// The window that shows "Sending data", or the transfer's error, in tiles.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH     \
    || LANGUAGE == LANGUAGE_ITALIAN
#    define FILE_SCREEN_SENDING_WINDOW_X     4
#    define FILE_SCREEN_SENDING_WINDOW_WIDTH 22
#else
#    define FILE_SCREEN_SENDING_WINDOW_X     5
#    define FILE_SCREEN_SENDING_WINDOW_WIDTH 20
#endif

// The window of the password's "Password" heading, in tiles.
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#    define FILE_SCREEN_PASSWORD_HEADING_X     8
#    define FILE_SCREEN_PASSWORD_HEADING_WIDTH 13
#else
#    define FILE_SCREEN_PASSWORD_HEADING_X     10
#    define FILE_SCREEN_PASSWORD_HEADING_WIDTH 10
#endif

// The x of a one-page password's help line, "B: Return", in pixels.
#define FILE_SCREEN_PASSWORD_RETURN_X JAPANESE_OR(0, 80)

// The help window of a password of several pages ("Left & Right: Change
// Page  B: Return"), in tiles.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH     \
    || LANGUAGE == LANGUAGE_ITALIAN
#    define FILE_SCREEN_PASSWORD_HELP_X     0
#    define FILE_SCREEN_PASSWORD_HELP_WIDTH 30
#elif LANGUAGE == LANGUAGE_JAPANESE
#    define FILE_SCREEN_PASSWORD_HELP_X     3
#    define FILE_SCREEN_PASSWORD_HELP_WIDTH 24
#else
#    define FILE_SCREEN_PASSWORD_HELP_X     1
#    define FILE_SCREEN_PASSWORD_HELP_WIDTH 28
#endif

#define PASSWORD_SYMBOLS_PER_PAGE 50   // five rows of ten
#define PASSWORD_SYMBOLS_PER_ROW  10   // a page's row
#define PASSWORD_SYMBOL_MASK      0x3f // six bits: 64 symbols

// The bytes each password level packs, its checksum's two included, as The Lost Age's password
// screen decodes them.
#define PASSWORD_GOLD_BYTES       175
#define PASSWORD_SILVER_BYTES     41
#define PASSWORD_BRONZE_BYTES     11
// The checksum's two bytes at the end of each level's (scFileScreenCalculatePasswordCrc16).
#define PASSWORD_CRC_BYTES        2
#define PASSWORD_CRC_SEED         0xffff
#define PASSWORD_CRC_POLYNOMIAL   0x1021
#define PASSWORD_CRC_TOP_BIT      0x8000 // of its 16 bits

// Where a password's parts start among its bytes: the four levels and the Djinn from the first, the
// story flags, then Silver's and Bronze's Psynergy items, which Gold has not; the stats rows after
// those, two party members to a row; and Gold's alone, the party's items, 15 a member in nine bits
// each, their quantities, five bits for each of the 23 items that stack
// (FileScreenPasswordStackItems), and the coins, high byte first.
#define PASSWORD_FLAGS_BYTE       7
#define PASSWORD_KEY_ITEMS_BYTE   8
#define PASSWORD_STATS_ROW_BYTES  15
#define PASSWORD_ITEMS_BYTE       39
#define PASSWORD_QUANTITIES_BYTE  107
#define PASSWORD_COINS_BYTE       165
#define PASSWORD_STACK_ITEMS      23
// The party members a password packs (FileScreenPasswordMembers), its story flags
// (FileScreenPasswordFlags) and Psynergy items (FileScreenPasswordKeyItems).
#define PASSWORD_MEMBERS          4
#define PASSWORD_FLAGS            6
#define PASSWORD_KEY_ITEMS        8

extern Uint32 FileScreenPasswordMembers[PASSWORD_MEMBERS];
extern Uint16 FileScreenPasswordFlags[PASSWORD_FLAGS];
extern Uint16 FileScreenPasswordKeyItems[PASSWORD_KEY_ITEMS];

// A party member's base stats in the password, two words of fields: HP (11 bits), PP (11) and
// Attack (10); then Defense (10), Agility (10) and Luck (8) over four bits of 0.
#define PASSWORD_HP_SHIFT      21
#define PASSWORD_PP_SHIFT      10
#define PASSWORD_DEFENSE_SHIFT 22
#define PASSWORD_AGILITY_SHIFT 12
#define PASSWORD_LUCK_SHIFT    4

// The password's symbols in the font: in the Japanese edition kana in their
// order (charmap_ja.tsv's codes), あ to そ, た to ん and を, then with the
// voiced mark が to ぞ, だ, で, ど and ば to ぼ; in the others capitals
// without I and O, the digits 2 to 9, small letters without l and o, then ! ? # & $ % + and =.
#if LANGUAGE == LANGUAGE_JAPANESE
#    define SYMBOL_TA 15
#    define SYMBOL_WO 45
#    define SYMBOL_GA 46
#    define SYMBOL_DA 56
#    define SYMBOL_BA 59
#else
#    define SYMBOL_DIGITS        24
#    define SYMBOL_SMALL_LETTERS 32
#    define SYMBOL_SIGNS         56
#endif

// Sending by cable gives up after this long, almost three hours, and with a communication error
// once the other console goes unheard this long.
#define LINK_SEND_TICKS   600000
#define LINK_SILENT_TICKS 10

// UB: the second code's held flag, a word, is read and written as its low halfword.
#define EXTRA_CODE_HELD                                                                            \
    WHEN(FIX_TYPE_PUNNING, FileScreenExtraCodeHeld, (*(Sint16 *)&FileScreenExtraCodeHeld))

static Sint32 scFileScreenPackPasswordData(Sint32 unused, Sint32 mode, Uint8 *out);
static Sint32 scFileScreenEncodePassword(Uint8 *data, Sint32 size, Uint8 *out);
static Uint16 scFileScreenCalculatePasswordCrc16(Uint32 byteCount, Uint8 *data);

// The File Screen overlay's scene tables, for the scene loader.
Uint8        *scGetEntrances(void)
{
    return (Uint8 *)ScreenEntrances;
}

// The exit regions entry: the File Screen has no exit boxes.
Sint32 scGetExitRegions(void)
{
    return 0;
}

Uint8 *scGetExits(void)
{
    return (Uint8 *)FileScreenExits;
}

Uint8 *scGetActorPlacements(void)
{
    return (Uint8 *)FileScreenActorPlacements;
}

Uint8 *scGetCues(void)
{
    return (Uint8 *)FileScreenCues;
}

// Loads the Golden Sun logo onto BG2 as scTitleLoadBackground (title.c) does, and stops the
// leader's motion.
static void scFileScreenLoadBackground(void)
{
    Uint8     *resource;
    Sint32     resourceId;
    Uint16    *tilemap;
    Uint32     x;
    Uint32     y;
    Uint16     tile;
    BG_SCROLL *scroll;
    Sint32     offscreenTile = OFFSCREEN_TILE;

    resourceId               = RESOURCE_GOLDEN_SUN_LOGO;
    syScreenFadeToBlack(0);
    REG_BG2CNT        = LOGO_BGCNT;
    BgScrollWork[2].y = 0;
    resource          = syResourceGetTableEntry(resourceId);
    syDMAXCopy(resource, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_COLORS / 2), REG_DMA3);
    resource += LOGO_PALETTE_COLORS * 2;
    syFieldDecompressTaggedLZ(resource, RamMapGridMetatileBuffer);
    syDMAXCopy((void *)RamMapGridMetatileBuffer,
               LOGO_TILES,
               DMA_COPY_WORDS(SCREEN_TILE_BYTES / 4),
               REG_DMA3);
    // The logo's map, each row's two columns past the screen's edge, which never show.
    tilemap = LOGO_MAP;
    tile    = LOGO_FIRST_TILE;
    for (y = 0; y < DISPLAY_TILE_HEIGHT; y++)
    {
        for (x = 0; x < DISPLAY_TILE_WIDTH; x++)
            *tilemap++ = tile++;
        *tilemap++ = offscreenTile;
        *tilemap++ = offscreenTile;
    }
    scroll = BgScrollWork;
    for (y = 0; y <= 3; y++)
    {
        scroll->y = 0;
        scroll->x = 0;
        scroll++;
    }
    syDMAXCopy(BgScrollWork, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
    ((MAP_GRID_SCROLL_STATE *)MapGridState[0])->flags           = DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
    fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlags = 0;
}

// The event that fades BG0 and the sprites in over the logo, a step of the blend every second tick.
static void scFileScreenFadeInBlend(void)
{
    Uint16 level;

    level = (Uint16)++FileScreenBlendTick >> 1;
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT,
                        BLDCNT_TGT1_BG0 | BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1
                            | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_BD,
                        IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH(
        (Uint32)&REG_BLDALPHA, BLEND_ALPHA(level, BLEND_WEIGHT_MAX - level), IO_WRITE_HALFWORD);
    if (level >= BLEND_WEIGHT_MAX)
        syCloseEvent(scFileScreenFadeInBlend);
}

// clang-format off
// The first hidden code, entered on the File Screen and ended by 0.
//
// It is Select three times. With it a new game names Garet, Ivan and Mia as well as Isaac (scInit).
const Uint16 FileScreenButtonCode[] = {
    KEY_SELECT, KEY_SELECT, KEY_SELECT, 0,
};

// The second, longer code.
//
// With it a new game also names Felix, Jenna and Sheba.
const Uint16 FileScreenExtraButtonCode[] = {
    KEY_UP, KEY_DOWN, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT,
    KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_UP, KEY_SELECT, 0,
};
// clang-format on

// The event that follows the two hidden codes each tick while the File Screen runs: each held key
// that matches the next key of a code advances it, a wrong key starts it again, and finishing a
// code unlocks it with a chime.
static void scFileScreenCheckButtonCodes(void)
{
    if (FileScreenCodeUnlocked == 0)
    {
        if (FileScreenCodeHeld != 0)
        {
            if (PerData == 0)
                FileScreenCodeHeld = 0;
        }
        else if (PerData != 0)
        {
            if (PerData == FileScreenButtonCode[FileScreenCodeProgress])
            {
                FileScreenCodeProgress++;
                FileScreenCodeHeld = 1;
                if (FileScreenButtonCode[FileScreenCodeProgress] == 0)
                {
                    FileScreenCodeUnlocked = 1;
                    sySoundPlay(SE_CHIME);
                }
            }
            else
            {
                FileScreenCodeProgress = 0;
            }
        }
    }
    if (FileScreenExtraCodeUnlocked == 0)
    {
        if (EXTRA_CODE_HELD != 0)
        {
            if (PerData == 0)
                EXTRA_CODE_HELD = 0;
        }
        else if (PerData != 0)
        {
            if (PerData == FileScreenExtraButtonCode[FileScreenExtraCodeProgress])
            {
                FileScreenExtraCodeProgress++;
                EXTRA_CODE_HELD = 1;
                if (FileScreenExtraButtonCode[FileScreenExtraCodeProgress] == 0)
                {
                    FileScreenExtraCodeUnlocked = 1;
                    sySoundPlay(SE_CHIME);
                }
            }
            else
            {
                FileScreenExtraCodeProgress = 0;
            }
        }
    }
}

// The character a password symbol stands for, in the font's codes, with the voiced mark after it
// where it has one.
static void scFileScreenGetPasswordCharacter(Sint32 metatile, Uint8 *text)
{
    text[1] = 0;
    text[2] = 0;
#if LANGUAGE != LANGUAGE_JAPANESE
    if (metatile <= 'H' - 'A')
        text[0] = metatile + 'A';
    else if (metatile <= 'N' - 'A' - 1)
        text[0] = metatile + 'A' + 1;
    else if (metatile <= 'Z' - 'A' - 2)
        text[0] = metatile + 'A' + 2;
    else if (metatile < SYMBOL_SMALL_LETTERS)
        text[0] = metatile - SYMBOL_DIGITS + '2';
    else if (metatile <= SYMBOL_SMALL_LETTERS + 'k' - 'a')
        text[0] = metatile - SYMBOL_SMALL_LETTERS + 'a';
    else if (metatile <= SYMBOL_SMALL_LETTERS + 'n' - 'a' - 1)
        text[0] = metatile - SYMBOL_SMALL_LETTERS + 'a' + 1;
    else if (metatile <= SYMBOL_SMALL_LETTERS + 'z' - 'a' - 2)
        text[0] = metatile - SYMBOL_SMALL_LETTERS + 'a' + 2;
    else if (metatile == SYMBOL_SIGNS)
        text[0] = '!';
    else if (metatile == SYMBOL_SIGNS + 1)
        text[0] = '?';
    else if (metatile == SYMBOL_SIGNS + 2)
        text[0] = '#';
    else if (metatile == SYMBOL_SIGNS + 3)
        text[0] = '&';
    else if (metatile == SYMBOL_SIGNS + 4)
        text[0] = '$';
    else if (metatile == SYMBOL_SIGNS + 5)
        text[0] = '%';
    else if (metatile == SYMBOL_SIGNS + 6)
        text[0] = '+';
    else
        text[0] = '=';
#else
    if (metatile < SYMBOL_TA)
        text[0] = metatile + KANA_A;
    else if (metatile < SYMBOL_WO)
        text[0] = metatile - SYMBOL_TA + KANA_TA;
    else if (metatile == SYMBOL_WO)
        text[0] = KANA_WO;
    else
    {
        if (metatile < SYMBOL_DA)
            text[0] = metatile - SYMBOL_GA + KANA_KA;
        else if (metatile == SYMBOL_DA)
            text[0] = KANA_TA;
        else if (metatile == SYMBOL_DA + 1)
            text[0] = KANA_TE;
        else if (metatile == SYMBOL_DA + 2)
            text[0] = KANA_TO;
        else
            text[0] = metatile - SYMBOL_BA + KANA_HA;
        text[1] = TEXT_MARK_VOICED;
    }
#endif
}

// Nonzero (-1) when the save was made where Retreat is blocked, unless in a town or on the world
// map: such a save cannot restart at the Sanctum, by L and Start held as it loads or as a corrupted
// save is recovered.
static Sint32 scFileScreenIsRetreatBlocked(void)
{
    if (gmFlagGet(FLAG_RETREAT_BLOCKED) == 0)
    {
        return 0;
    }
    if (MainRoutineStateWork.sceneType == SCENE_TYPE_TOWN)
    {
        return 0;
    }
    if (MainRoutineStateWork.next == SCENE_WORLD_MAP)
    {
        return 0;
    }
    return -1;
}

// The loaded file's scene state as Continue reads it: the play time it holds must match the file's
// own, or the data is corrupted.
typedef struct FILE_SCREEN_LOADED_SCENE_STATE
{
    // SceneLocalState, then InventorySnapshotWork
    Uint8  mapAndInventory[256];
    // The play time in ticks, which mnSaveBuildFileSummary copies here when saving.
    Sint32 playTime;
} FILE_SCREEN_LOADED_SCENE_STATE;

// A console's link payload as the cable transfer reads it from SerialPeerPayloads.
typedef struct SAVE_LINK_GREETING
{
    // The greeting scInit waits for: FILE_SCREEN_LINK_LANGUAGE_TAG, then V, T and S.
    //
    // scInit sets every console's to "0000" before it waits.
    Uint16 code[4];
    Uint8  reserved[16]; // the rest of the 24 bytes; not read
} SAVE_LINK_GREETING;

// The overlay's initialize entry: the File Screen.
//
// Returns 0 once a destination is set.
Sint32 scInit(void)
{
    Sint32  pageCount;
    Sint32  encodedSize;
    WINDOW *pageWindow;
    Sint32  glyphColumn;
    WINDOW *linkWindow;
    Sint32  passwordColumn;
    Sint32  tileValue;
    // index, limit and result serve each step in turn: the yes/no answer, the name being entered,
    // the link's greeting, the page to redraw.
    Sint32  limit;
    Sint32  index;
    Sint32  result;
    Sint32  linkMsg;
    Sint32  peerIndex;
    Bool    linkConnectionOk;
    Sint32  pageGlyphIndex;
    Sint32  passwordHeadingMsg;
    Sint32  pageIndex;
    Bool    blendStarted;
    WINDOW *helpWindow;
    WINDOW *headingWindow;
    Uint8   saveData[200];
    Uint8   password[FILE_SCREEN_PASSWORD_BUFFER_SIZE];
    Uint8   glyph[4];

    blendStarted = FALSE;
    scFileScreenLoadBackground();
    sySetTickEvent(scFileScreenCheckButtonCodes, EVENT_PASS_TICK);
    FieldStateWork->transition = 0;
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    // FILE_SCREEN_FROM_TITLE, after the credits (the title): save the cleared game, then
    // WORLD_MAP_FROM_FILE_SCREEN. Cancelling the file choice asks again.
    if (MainRoutineStateWork.entrance == FILE_SCREEN_FROM_TITLE)
    {
    ask_to_save:
        // "Save your adventure?"
        mnFieldMessageShow(MsgSaveAdventureQuestion, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN);
        index = fdYesNoAsk(1);
        mnMessageCloseFinishedWindows();
        if (index == YES)
        {
            // "You cannot resume your adventure with this data."
            mnFieldMessageShow(MsgSaveAdventureQuestion + 1, FIELD_MESSAGE_ZOOM);
            // Marks the file cleared (BUP_FILE_SUMMARY's sendFlag): its data can be sent, and the
            // message says it cannot be continued.
            MainRoutineStateWork.sendFlag = 1;
            index                         = mnSaveToChosenFile();
            if (index == -1)
            {
                goto ask_to_save;
            }
        }
        fdScriptWait(60);
        fdScriptFinishScreenTransition();
        sySoundPlay(SOUND_FADE_OUT_BGM);
        fdScriptWait(300);
        fdScriptSetDestination(SCENE_WORLD_MAP, WORLD_MAP_FROM_FILE_SCREEN);
        return 0;
    }
    SleepDisabled = 0;
menu_top:;
    result = mnSaveScanFileSummaries();
    // No backup memory: in debug mode, say so and turn idle sleep on.
    if (result < 0)
    {
        if (DebugMode != 0)
        {
            MainRoutineStateWork.idleSleepEnabled = 1;
            IdleSleepEnabled                      = 1;
            // "Unable to detect backup memory."
            mnFieldMessageShowCentered(MsgNoBackupMemory, CENTERED_MESSAGE_WINDOW, 8);
        }
    }
    // No save file: fade the menu layers in, once, and start a new game.
    if (result == 0)
    {
        if (blendStarted == 0)
        {
            sySynch(30);
            sySetTickEvent(scFileScreenFadeInBlend, EVENT_PASS_TICK);
            sySynch(1);
            blendStarted = TRUE;
        }
    }
    // The start menu's choice (FILE_ACTION); any other shows the menu again.
    if (result > 0)
    {
        result = mnFileListSelectAction();
    }
    else
    {
        result = SAVE_ACTION_NEW_GAME;
    }
    if (result == SAVE_ACTION_NEW_GAME)
    {
        // Hidden button codes expand the list of names offered for a new game: Isaac alone, with
        // the first code to Mia, with the second to Sheba. B on the first name goes back to the
        // menu.
        gmMainRoutineStateInitDefaults();
        mnWindowColorApply(MainRoutineStateWork.windowHue, MainRoutineStateWork.windowBrightness);
        limit = ISAAC + 1;
        index = ISAAC;
        for (;;)
        {
            sySynch(6);
            result = mnNameEntryEditMemberName(index);
            if (result == -1)
            {
                if (index == ISAAC)
                {
                    goto menu_top;
                }
                index--;
                continue;
            }
            if (FileScreenCodeUnlocked != 0)
            {
                limit = MIA + 1;
            }
            if (FileScreenExtraCodeUnlocked != 0)
            {
                limit = SHEBA + 1;
            }
            if (++index >= limit)
            {
                break;
            }
        }
        gmPartyStartChildhood();
        MainRoutineStateWork.next     = SCENE_COTTAGE_ROOMS_B;
        MainRoutineStateWork.entrance = COTTAGE_ROOMS_B_FROM_VALE_PLAZA_SOUTHEAST;
        goto leave;
    }
    if (result == SAVE_ACTION_CONTINUE)
    {
        MAIN_ROUTINE_STATE *state;

        result = mnSaveChooseAndLoadFile(FILE_LIST_CONTINUE);
        if (result == -1)
        {
            goto menu_top;
        }
        fdScriptSetFlag(FLAG_KEEP_PARTY_POSITION);
        state = &MainRoutineStateWork;
        mnWindowColorApply(state->windowHue, state->windowBrightness);
        if (state->buildStamp != gmBuildStampGetTime())
        {
            state->next     = state->restartScene;
            state->entrance = state->restartEntrance;
            gmFlagClear(FLAG_KEEP_PARTY_POSITION);
        }
        else if ((PerData & (KEY_L | KEY_START)) == (KEY_L | KEY_START))
        {
            if (scFileScreenIsRetreatBlocked() != 0)
            {
                // "Unfortunately, your data could not be recovered."
                mnFieldMessageShow(MsgSaveRecoveryFailed, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
                goto menu_top;
            }
            state->next     = state->restartScene;
            state->entrance = state->restartEntrance;
            gmFlagClear(FLAG_KEEP_PARTY_POSITION);
            gmFlagSet(FLAG_SAVE_RESTARTED_AT_SANCTUM);
        }
        else if (state->playTime != ((FILE_SCREEN_LOADED_SCENE_STATE *)SceneLocalState)->playTime)
        {
            // "Some data is corrupted and cannot be recovered."
            mnFieldMessageShow(MsgSaveDataCorrupted, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
            // "Do you wish to try to recover from a Sanctum?"
            mnFieldMessageShow(MsgSaveDataCorrupted + 1,
                               FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN | FIELD_MESSAGE_LOWER);
            if (mnYesNoMenuRun(1, 0, 0, YES) != YES)
            {
                mnMessageCloseFinishedWindows();
                goto menu_top;
            }
            mnMessageCloseFinishedWindows();
            if (scFileScreenIsRetreatBlocked() != 0)
            {
                // "Unfortunately, your data could not be recovered."
                mnFieldMessageShow(MsgSaveDataCorrupted + 2,
                                   FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
                goto menu_top;
            }
            state->next     = state->restartScene;
            state->entrance = state->restartEntrance;
            gmFlagClear(FLAG_KEEP_PARTY_POSITION);
            gmFlagSet(FLAG_SAVE_RECOVERED_AT_SANCTUM);
        }
        gmFlagClear(FLAG_MENU_OPEN);
        goto leave;
    }
    if (result == SAVE_ACTION_COPY)
    {
        mnSaveCopyFile();
        goto menu_top;
    }
    if (result == SAVE_ACTION_ERASE)
    {
        mnSaveEraseSelectedFile();
        goto menu_top;
    }
    // Battle: go to the Battle Arena with a file. One saved while the Colosso held the party's
    // equipment gets it back, and its party is reset to Isaac, Garet, Ivan and Mia, in order.
    if (result == SAVE_ACTION_BATTLE)
    {
        result = mnSaveChooseAndLoadFile(FILE_LIST_BATTLE);
        if (result == -1)
        {
            goto menu_top;
        }
        MainRoutineStateWork.leaderActorId = 0;
        if (fdScriptGetFlag(FLAG_COLOSSO_HOLDS_EQUIPMENT) != 0)
        {
            gmInventorySnapshotRestore();
            gmPartyRemoveMember(ISAAC);
            gmPartyRemoveMember(GARET);
            gmPartyRemoveMember(IVAN);
            gmPartyRemoveMember(MIA);
            gmPartyAddMember(ISAAC);
            gmPartyAddMember(GARET);
            gmPartyAddMember(IVAN);
            gmPartyAddMember(MIA);
        }
        mnWindowColorApply(MainRoutineStateWork.windowHue, MainRoutineStateWork.windowBrightness);
        gmFlagClear(FLAG_KEEP_PARTY_POSITION);
        gmFlagClear(FLAG_MENU_OPEN);
        gmFlagSet(FLAG_IN_BATTLE_ARENA);
        SleepDisabled = 1;
        fdScriptSetDestination(SCENE_BATTLE_ARENA, BATTLE_ARENA_FROM_FILE_SCREEN);
        goto leave;
    }
    else
    {
        if (result != SAVE_ACTION_SEND)
        {
            goto menu_top;
        }
    link_load:;
        result = mnSaveChooseAndLoadFile(FILE_LIST_CLEARED);
        if (result == -1)
        {
            goto menu_top;
        }
        mnWindowColorApply(MainRoutineStateWork.windowHue, MainRoutineStateWork.windowBrightness);
    link_select:;
        result = mnIconMenuChooseSendOption(SEND_OPTION_TRANSFER_METHOD);
        if (result == -1)
        {
            goto link_load;
        }
        // Link transfer waits for the other game's edition-specific greeting in the partner's
        // payload (SAVE_LINK_GREETING); B gives up.
        if (result == TRANSFER_METHOD_CABLE)
        {
            linkWindow = mnWindowCreate(FILE_SCREEN_LINK_WINDOW_X,
                                        5,
                                        FILE_SCREEN_LINK_WINDOW_WIDTH,
                                        8,
                                        WINDOW_FLAG_INSTANT);
            linkMsg    = MsgClearSaveLabel;

            // "Save", "Preparing data" and "B: Quit"
            mnTextDrawMessage(linkMsg, linkWindow, 0, 4);
            mnTextDrawMessage(linkMsg + 1, linkWindow, 0, 16);
            mnTextDrawMessage(linkMsg + 3, linkWindow, 0, 36);
            sySerialInit();
            sySynch(10);
            {
                Uint16 *sync = (Uint16 *)SerialTransfer.payload;

                sync[0]      = '0';
                sync[1]      = '0';
                sync[2]      = '0';
                sync[3]      = '0';
            }
            index = 0;
            limit = LINK_STATUS_HEARD_BOTH;
            for (peerIndex = 0; peerIndex != 4; peerIndex++)
            {
                Uint16 *greeting =
                    ((SAVE_LINK_GREETING *)SerialPeerPayloads[peerIndex].payload)->code;

                greeting[0] = '0';
                greeting[1] = '0';
                greeting[2] = '0';
                greeting[3] = '0';
            }
            for (;;)
            {
                if ((PerPush & KEY_B) != 0)
                {
                    sySoundPlay(SE_MENU_CANCEL);
                    break;
                }
                if ((limit & LinkStatusWork) == limit)
                {
                    Uint32  control = REG_SIOCNT;
                    Uint32  peer    = SIO_ID_OF(control) ^ 1;
                    Uint16 *received =
                        ((SAVE_LINK_GREETING *)SerialPeerPayloads[peer].payload)->code;

                    if (received[0] == FILE_SCREEN_LINK_LANGUAGE_TAG)
                    {
                        if (received[1] == 'V')
                        {
                            if (received[2] == 'T')
                            {
                                if (received[3] == 'S')
                                {
                                    index = 1;
                                    break;
                                }
                            }
                        }
                    }
                }
                sySynch(1);
            }
            mnWindowClose(linkWindow, 2);
            if (index == 0)
            {
                goto link_select;
            }
            linkWindow = mnWindowCreate(FILE_SCREEN_SENDING_WINDOW_X,
                                        10,
                                        FILE_SCREEN_SENDING_WINDOW_WIDTH,
                                        4,
                                        WINDOW_FLAG_INSTANT);

            // "Sending data"
            mnTextDrawMessage(MsgClearSendingData, linkWindow, 0, 4);
            sySynch(10);
            sySerialStartSend(BupBuffer, 4100);
            sySynch(10);
            limit = 0;
            {
                Sint32 requiredLinkMask = LINK_STATUS_HEARD_BOTH;

                linkConnectionOk        = TRUE;
                for (index = 0; index < LINK_SEND_TICKS; index++)
                {
                    limit++;
                    if ((requiredLinkMask & LinkStatusWork) == requiredLinkMask)
                    {
                        limit = 0;
                    }
                    if (limit == LINK_SILENT_TICKS)
                    {
                        linkConnectionOk = FALSE;
                        break;
                    }
                    if (SerialSendSource == 0)
                    {
                        break;
                    }
                    sySynch(1);
                }
                if (linkConnectionOk == 0)
                {
                    mnWindowSpritePrepareForRedraw(linkWindow);
                    // "Communication Error"
                    mnTextDrawMessage(MsgClearCommunicationError, linkWindow, 0, 4);
                    do
                    {
                        sySynch(1);
                    } while ((PerPush & KEY_A) == 0);
                }
                sySynch(10);
                sySerialRemoveIRQHandlers();
                sySynch(10);
            }
#if SENDING_WINDOW_WAITS_A_TICK_AFTER_CLOSING
            mnWindowClose(linkWindow, 2);
            sySynch(1);
#else
            mnWindowSpritePrepareForRedraw(linkWindow);
            mnWindowClose(linkWindow, 2);
#endif
            goto menu_top;
        }
        if (result == TRANSFER_METHOD_PASSWORD)
        {
        // The password: the party's data packed, its checksum after it, then turned into symbols.
        // The level is Gold, Silver or Bronze (enum PASSWORD_LEVEL): 260, 61 or 16 symbols, shown
        // PASSWORD_SYMBOLS_PER_PAGE to a page.
        page_select:;
            result = mnIconMenuChooseSendOption(SEND_OPTION_PASSWORD_LEVEL);

            if (result == -1)
            {
                goto link_select;
            }
            {
                Sint16 sum;

                encodedSize           = scFileScreenPackPasswordData(0, result, saveData);
                sum                   = scFileScreenCalculatePasswordCrc16(encodedSize, saveData);
                saveData[encodedSize] = (Uint16)sum >> 8;
                saveData[encodedSize + 1]  = sum;
                encodedSize               += PASSWORD_CRC_BYTES;
                encodedSize = scFileScreenEncodePassword(saveData, encodedSize, password);
            }
            syWindowLoadGraphics();
            pageWindow         = mnWindowCreate(5, 4, 20, 12, WINDOW_FLAG_INSTANT);
            pageIndex          = 0;
            pageCount          = encodedSize / PASSWORD_SYMBOLS_PER_PAGE + 1;
            headingWindow      = mnWindowCreate(FILE_SCREEN_PASSWORD_HEADING_X,
                                           0,
                                           FILE_SCREEN_PASSWORD_HEADING_WIDTH,
                                           4,
                                           WINDOW_FLAG_INSTANT);
            passwordHeadingMsg = MsgClearPasswordLabel;
            // "Password"
            mnTextDrawMessage(passwordHeadingMsg, headingWindow, 6, 4);
            index = 1;
            // The help line below: "B: Return" for one page, "Left & Right: Change
            // Page  B: Return" for more.
            if (pageCount == 1)
            {
                helpWindow = mnWindowCreate(5, 16, 20, 3, WINDOW_FLAG_INSTANT);
                mnTextDrawMessageAsTiles(
                    passwordHeadingMsg - 2, helpWindow, FILE_SCREEN_PASSWORD_RETURN_X, 0);
            }
            else
            {
                helpWindow = mnWindowCreate(FILE_SCREEN_PASSWORD_HELP_X,
                                            16,
                                            FILE_SCREEN_PASSWORD_HELP_WIDTH,
                                            3,
                                            WINDOW_FLAG_INSTANT);
                mnTextDrawMessageAsTiles(passwordHeadingMsg - 1, helpWindow, 0, 0);
            }
            mnGraphicsExpandVRAMTilesByColorTable(
                (Uint16 *)(BG_CHAR_BLOCK(0) + PAGE_DIGITS_TILE * TILE_SIZE_4BPP));
            mnWindowSpritePrepareForRedraw(pageWindow);
            // A turns the page, and on the last returns to the menu; Left and Right turn back and
            // forth, wrapping round; B goes back to the level choice. index is 1 when the page must
            // be drawn again.
            for (;;)
            {
                mnPageArrowsDraw(PAGE_ARROW_VRAM);
                if ((PerPush & KEY_B) != 0)
                {
                    sySoundPlay(SE_MENU_CANCEL);
                    mnWindowSpritePrepareForRedraw(pageWindow);
                    mnWindowClose(pageWindow, 2);
                    mnWindowClose(helpWindow, 2);
                    mnWindowClose(headingWindow, 2);
                    goto page_select;
                }
                if ((PerPush & KEY_A) != 0)
                {
                    pageIndex++;
                    index = 1;
                    if (pageIndex == pageCount)
                    {
                        sySoundPlay(SE_MENU_CONFIRM);
                        mnWindowSpritePrepareForRedraw(pageWindow);
                        mnWindowClose(pageWindow, 2);
                        mnWindowClose(helpWindow, 2);
                        mnWindowClose(headingWindow, 2);
                        sySynch(1);
                        goto menu_top;
                    }
                    sySoundPlay(SE_MENU_CURSOR_MOVE);
                }
                else if ((PerPush & KEY_LEFT) != 0 && pageCount > 1)
                {
                    // Left and Right turn the page, wrapping round.
                    sySoundPlay(SE_MENU_CURSOR_MOVE);
                    pageIndex = (pageIndex + pageCount - 1) % pageCount;
                    index     = 1;
                }
                else if ((PerPush & KEY_RIGHT) != 0 && pageCount > 1)
                {
                    sySoundPlay(SE_MENU_CURSOR_MOVE);
                    pageIndex = (pageIndex + pageCount + 1) % pageCount;
                    index     = 1;
                }
                if (index == 1)
                {
                    mnWindowSpritePrepareForRedraw(pageWindow);
                    index = 0;
                    limit = 2;
                    do
                    {
                        mnWindowDrawDividerLine(pageWindow, 0, limit, 18, limit);
                        limit = limit + 2;
                        index++;
                    } while (index != 4);
                    // The page numbers between the arrows, the page shown lit.
                    if (pageCount > 1)
                    {
                        index = 0;
                        if (pageCount != 0)
                        {
                            Uint32 mode = TILEMAP_ENTRY_PLAIN;

                            do
                            {
                                tileValue = index + PAGE_NUMBER_TILE;
                                if (index == pageIndex)
                                {
                                    tileValue = index + PAGE_NUMBER_LIT_TILE;
                                }
                                mnWindowSetTilemapEntry(
                                    pageWindow, tileValue, index + 18 - pageCount, -1, mode);
                                index++;
                            } while (index != pageCount);
                        }
                        mnWindowSetTilemapEntry(
                            pageWindow, PAGE_ARROW_LEFT, 17 - pageCount, -1, TILEMAP_ENTRY_PLAIN);
                        mnWindowSetTilemapEntry(
                            pageWindow, PAGE_ARROW_RIGHT, 18, -1, TILEMAP_ENTRY_PLAIN);
                        mnWindowMarkTilemapRow((WIN_STATE *)WindowStateWork[0], pageWindow->y);
                    }
                    // The page's symbols, each row split into two groups of five: rows 16 pixels
                    // apart, the second five of a row 10 pixels further right.
                    result = pageIndex * PASSWORD_SYMBOLS_PER_PAGE + PASSWORD_SYMBOLS_PER_PAGE;
                    if (result > encodedSize)
                    {
                        result = encodedSize;
                    }
                    for (index = pageIndex * PASSWORD_SYMBOLS_PER_PAGE, pageGlyphIndex = 0;
                         index != result;
                         index++, pageGlyphIndex++)
                    {
                        // FAKEMATCH(dead): a once-only block round the draw
                        // * call.
                        do
                        {
                            scFileScreenGetPasswordCharacter(password[index] & PASSWORD_SYMBOL_MASK,
                                                             glyph);
                        } while (0);
                        passwordColumn = index % PASSWORD_SYMBOLS_PER_ROW;
                        if (passwordColumn > PASSWORD_SYMBOLS_PER_ROW / 2 - 1)
                        {
                            Sint32 row;

                            glyphColumn = pageGlyphIndex % PASSWORD_SYMBOLS_PER_ROW;
                            row         = pageGlyphIndex / PASSWORD_SYMBOLS_PER_ROW;
                            mnTextDrawString(
                                glyph, pageWindow, glyphColumn * 12 + 18, (row << 4) + 2);
                        }
                        else
                        {
                            Sint32 row;

                            glyphColumn = pageGlyphIndex % PASSWORD_SYMBOLS_PER_ROW;
                            row         = pageGlyphIndex / PASSWORD_SYMBOLS_PER_ROW;
                            mnTextDrawString(
                                glyph, pageWindow, glyphColumn * 12 + 8, (row << 4) + 2);
                        }
                    }
                    index = 0;
                }
                sySynch(1);
            }
        }
        sySynch(300);
        goto menu_top;
    }
leave:;
    FieldStateWork->exit = EXIT_KEEP_DESTINATION;
    fdScriptWait(30);
    sySoundPlay(SOUND_FADE_OUT_BGM);
    fdScriptFinishScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptWait(60);
    return 0;
}

// The grid of the window tilemap the two unused functions below fill: 16 by 8 entries; the second's
// tiles start at TILE_GRID_FIRST_TILE.
#define TILE_GRID_COLUMNS    16
#define TILE_GRID_ROWS       8
#define TILE_GRID_FIRST_TILE 32

// Unused: unpacks the tiles into a scratch buffer, then fills a grid of the window tilemap, in VRAM
// and in its shadow, at the window's metatile: each metatile's tile the row times the window's
// width plus the column.
UNUSED static void scFileScreenFillTileGridByWindowWidth(WINDOW     *window,
                                                         const void *compressedTiles)
{
    Uint16 *shadow;
    Uint16 *vram;
    void   *decodedTiles;
    Sint32  row;
    Sint32  column;
    Sint32  metatile;

    shadow       = ((WIN_STATE *)WindowStateWork[0])->tilemap;
    decodedTiles = syHeapAllocateEWRAM(768);
    syFieldDecompressTaggedLZ(compressedTiles, decodedTiles);
    metatile  = window->y * BG_SCREEN_WIDTH + window->x;
    vram      = (Uint16 *)WINDOW_TILEMAP_VRAM + metatile;
    shadow   += metatile;
    for (row = 0; row < TILE_GRID_ROWS; row++)
    {
        for (column = 0; column < TILE_GRID_COLUMNS; column++)
        {
            Sint16 tile = WINDOW_TILE(window->width * row + column);

            *vram++     = tile;
            *shadow++   = tile;
        }
        vram   += BG_SCREEN_WIDTH - TILE_GRID_COLUMNS;
        shadow += BG_SCREEN_WIDTH - TILE_GRID_COLUMNS;
    }
    syHeapRelease(decodedTiles);
}

// Unused: fills a grid of the window tilemap with the tiles from TILE_GRID_FIRST_TILE on,
// TILE_GRID_COLUMNS to a row.
UNUSED static void scFileScreenFillTileGridByGridWidth(const WINDOW *window)
{
    Sint16 *mirror = ((WIN_STATE *)WindowStateWork[0])->tilemap;
    Sint16 *buf    = syHeapAllocateEWRAM(768);
    Sint16 *vram;
    Sint32  metatile;
    Sint32  row;
    Sint32  base;

    metatile  = window->y * BG_SCREEN_WIDTH + window->x;
    vram      = (Sint16 *)WINDOW_TILEMAP_VRAM + metatile;
    mirror   += metatile;
    row       = 0;
    base      = 0;
    do
    {
        Sint32 tile = base + TILE_GRID_FIRST_TILE;
        Sint32 column;

        for (column = 0; column < TILE_GRID_COLUMNS; column++)
        {
            Sint16 value = WINDOW_TILE(tile);

            *vram        = value;
            tile++;
            *mirror = value;
            vram++;
            mirror++;
        }
        row++;
        vram   += BG_SCREEN_WIDTH - TILE_GRID_COLUMNS;
        mirror += BG_SCREEN_WIDTH - TILE_GRID_COLUMNS;
        base   += TILE_GRID_COLUMNS;
    } while (row < TILE_GRID_ROWS);
    syHeapRelease(buf);
}

// A party member's base stats, which the password clamps and packs.
//
// They are UNIT's from baseHp to baseLuck, at the same offsets.
typedef struct PASSWORD_STATS
{
    Sint16 baseHp; // clamped to 0 to UNIT_CHARACTER_HP_MAX; 11 bits in the
    // password
    Sint16 basePp;      // clamped to 0 to UNIT_CHARACTER_HP_MAX; 11 bits
    Sint16 hpGauge;     // Unit's; not read here
    Sint16 ppGauge;     // Unit's; not read here
    Uint16 baseAttack;  // clamped to UNIT_STAT_MAX; 10 bits
    Uint16 baseDefense; // clamped to UNIT_STAT_MAX; 10 bits
    Uint16 baseAgility; // clamped to UNIT_STAT_MAX; 10 bits
    Uint8  baseLuck;    // clamped to UNIT_LUCK_MAX; 8 bits, the top one always 0
} PASSWORD_STATS;

// A party member as the password reads them: their level, base stats, inventory and Djinn, a view
// of UNIT at its offsets.
typedef struct PASSWORD_MEMBER
{
    Uint8          name[15];                   // Unit's; not read here
    Uint8          level;                      // clamped to 1 to UNIT_LEVEL_MAX; 7 bits
    PASSWORD_STATS stats;
    Uint8          regenToActions[184];        // Unit's baseHpRegen to psynergy
    Uint16         inventory[INVENTORY_SLOTS]; // an INVENTORY_ENCODING each
    Uint8          padding[2];                 // Unit's paddingBeforeDjinn
    // the Djinn held, a bit per Djinni, by element
    Uint32         djinnHeld[ELEMENT_COUNT];
} PASSWORD_MEMBER;

// Packs the four heroes' data for a password of level mode (enum PASSWORD_LEVEL), its checksum's
// bytes still to come; any other mode clears PASSWORD_BRONZE_BYTES. It clamps each party member's
// base stats and level in place first, and returns the byte count.
static Sint32 scFileScreenPackPasswordData(Sint32 unused, Sint32 mode, Uint8 *out)
{
    Sint32 length;
    Uint32 levelBits;
    Uint32 djinnBits;
    Uint8  keyItemBits;
    Uint8  flagBits;
    Uint32 packedStats[PASSWORD_MEMBERS * 2];
    Sint32 i;
    Sint32 outputIndex;
    Sint32 bitOffset;
    Sint32 j;
    Sint32 k;
    Sint32 n;

    length = PASSWORD_BRONZE_BYTES;
    switch (mode)
    {
        case PASSWORD_LEVEL_GOLD  : length = PASSWORD_GOLD_BYTES - PASSWORD_CRC_BYTES; break;
        case PASSWORD_LEVEL_SILVER: length = PASSWORD_SILVER_BYTES - PASSWORD_CRC_BYTES; break;
        case PASSWORD_LEVEL_BRONZE: length = PASSWORD_BRONZE_BYTES - PASSWORD_CRC_BYTES; break;
    }
    for (i = 0; i != length; i++)
    {
        out[i] = 0;
    }

    levelBits   = 0;
    djinnBits   = 0;
    keyItemBits = 0;
    flagBits    = 0;
    for (i = 0; i != PASSWORD_MEMBERS * 2; i++)
    {
        packedStats[i] = 0;
    }

    for (i = 0; i != PASSWORD_FLAGS; i++)
    {
        if (gmFlagGet(FileScreenPasswordFlags[i]))
        {
            flagBits |= 1 << i;
        }
    }

    for (i = 0; i != PASSWORD_MEMBERS; i++)
    {
        PASSWORD_MEMBER *member = (PASSWORD_MEMBER *)gmUnitGet(FileScreenPasswordMembers[i]);
        PASSWORD_STATS  *stats  = &member->stats;

        if (stats->baseHp > UNIT_CHARACTER_HP_MAX)
        {
            stats->baseHp = UNIT_CHARACTER_HP_MAX;
        }
        if (stats->baseHp < 0)
        {
            stats->baseHp = 0;
        }
        if (stats->basePp > UNIT_CHARACTER_HP_MAX)
        {
            stats->basePp = UNIT_CHARACTER_HP_MAX;
        }
        if (stats->basePp < 0)
        {
            stats->basePp = 0;
        }
        if (stats->baseAttack > UNIT_STAT_MAX)
        {
            stats->baseAttack = UNIT_STAT_MAX;
        }
        if (stats->baseDefense > UNIT_STAT_MAX)
        {
            stats->baseDefense = UNIT_STAT_MAX;
        }
        if (stats->baseAgility > UNIT_STAT_MAX)
        {
            stats->baseAgility = UNIT_STAT_MAX;
        }
        if (stats->baseLuck > UNIT_LUCK_MAX)
        {
            stats->baseLuck = UNIT_LUCK_MAX;
        }
        // Shift as unsigned: the permitted HP and Defense maxima exceed the signed 32-bit range
        // after packing, while their bit patterns are valid.
        packedStats[i * 2] = (Uint32)stats->baseHp << PASSWORD_HP_SHIFT
                           | stats->basePp << PASSWORD_PP_SHIFT | stats->baseAttack;
        packedStats[i * 2 + 1] = (Uint32)stats->baseDefense << PASSWORD_DEFENSE_SHIFT
                               | stats->baseAgility << PASSWORD_AGILITY_SHIFT
                               | stats->baseLuck << PASSWORD_LUCK_SHIFT;
        if (member->level > UNIT_LEVEL_MAX)
        {
            member->level = UNIT_LEVEL_MAX;
        }
        if (member->level == 0)
        {
            member->level = 1;
        }
        levelBits |= member->level << (i * 7);
        for (j = 0; j != ELEMENT_COUNT; j++)
        {
            djinnBits += member->djinnHeld[j] << (j * 7);
        }
        // Which of the eight Psynergy items (FileScreenPasswordKeyItems) any party member carries.
        for (j = 0; j != INVENTORY_SLOTS; j++)
        {
            Uint16 item = member->inventory[j] & INVENTORY_ITEM_ID_MASK;

            for (k = 0; k != PASSWORD_KEY_ITEMS; k++)
            {
                if (item == FileScreenPasswordKeyItems[k])
                {
                    keyItemBits |= 1 << k;
                }
            }
        }
    }

    // The largest password also carries inventories, stack counts and coins.
    if (mode == PASSWORD_LEVEL_GOLD)
    {
        outputIndex = PASSWORD_ITEMS_BYTE;
        bitOffset   = 0;
        for (i = 0; i != PASSWORD_MEMBERS; i++)
        {
            PASSWORD_MEMBER *member = (PASSWORD_MEMBER *)gmUnitGet(FileScreenPasswordMembers[i]);

            for (n = 0; n != INVENTORY_SLOTS; n++)
            {
                Uint16 item;

                // gmItemGetDirect only looks the definition up; its result is not used.
                gmItemGetDirect(member->inventory[n]);
                item                  = member->inventory[n] & INVENTORY_ITEM_ID_MASK;
                out[outputIndex]     += item >> (bitOffset + 1);
                out[outputIndex + 1] += item << (7 - bitOffset);
                bitOffset++;
                outputIndex++;
                if (bitOffset == 7)
                {
                    bitOffset = 0;
                    outputIndex++;
                }
            }
        }

        outputIndex = PASSWORD_QUANTITIES_BYTE;
        bitOffset   = -1;
        for (i = 0; i != PASSWORD_MEMBERS; i++)
        {
            PASSWORD_MEMBER *member = (PASSWORD_MEMBER *)gmUnitGet(FileScreenPasswordMembers[i]);
            Sint32           m;

            for (m = 0; m != PASSWORD_STACK_ITEMS; m++)
            {
                UNUSED Uint16 quantity;

                j = 0;
                for (n = 0; n != INVENTORY_SLOTS; n++)
                {
                    if ((member->inventory[n] & INVENTORY_ITEM_ID_MASK)
                        == FileScreenPasswordStackItems[m])
                    {
                        j = (member->inventory[n] & INVENTORY_QUANTITY_MASK)
                         >> INVENTORY_QUANTITY_SHIFT;
                    }
                }
                // FAKEMATCH(dead): a halfword copy of the quantity nothing
                // * reads.
                quantity = j;
                if (bitOffset < 0)
                {
                    out[outputIndex] += (Uint16)j >> -bitOffset;
                    outputIndex++;
                    bitOffset += 8;
                }
                out[outputIndex] += (Uint16)j << bitOffset;
                bitOffset        -= 5;
                if (bitOffset == -5)
                {
                    outputIndex++;
                    bitOffset = 3;
                }
            }
        }
        out[PASSWORD_COINS_BYTE]     = MainRoutineStateWork.holdings.coins >> 16;
        out[PASSWORD_COINS_BYTE + 1] = MainRoutineStateWork.holdings.coins >> 8;
        out[PASSWORD_COINS_BYTE + 2] = MainRoutineStateWork.holdings.coins;
    }

    // Silver and Gold carry the stats rows. Each packs two party members in
    // PASSWORD_STATS_ROW_BYTES: each one's first word and the top 28 bits of its second, whose low
    // four bits are always 0.
    if (mode != PASSWORD_LEVEL_BRONZE)
    {
        Sint32 statsOffset = PASSWORD_KEY_ITEMS_BYTE + (mode != PASSWORD_LEVEL_GOLD);

        for (i = 0; i != PASSWORD_MEMBERS / 2; i++)
        {
            out[statsOffset]       = packedStats[i * 4] >> 24;
            out[statsOffset + 1]   = packedStats[i * 4] >> 16;
            out[statsOffset + 2]   = packedStats[i * 4] >> 8;
            out[statsOffset + 3]   = packedStats[i * 4];
            out[statsOffset + 4]   = packedStats[i * 4 + 1] >> 24;
            out[statsOffset + 5]   = packedStats[i * 4 + 1] >> 16;
            out[statsOffset + 6]   = packedStats[i * 4 + 1] >> 8;
            out[statsOffset + 7]   = packedStats[i * 4 + 1];
            out[statsOffset + 7]  |= packedStats[i * 4 + 2] >> 28;
            out[statsOffset + 8]   = packedStats[i * 4 + 2] >> 20;
            out[statsOffset + 9]   = packedStats[i * 4 + 2] >> 12;
            out[statsOffset + 10]  = packedStats[i * 4 + 2] >> 4;
            out[statsOffset + 11]  = packedStats[i * 4 + 2] << 4;
            out[statsOffset + 11] |= packedStats[i * 4 + 3] >> 28;
            out[statsOffset + 12]  = packedStats[i * 4 + 3] >> 20;
            out[statsOffset + 13]  = packedStats[i * 4 + 3] >> 12;
            out[statsOffset + 14]  = packedStats[i * 4 + 3] >> 4;
            statsOffset           += PASSWORD_STATS_ROW_BYTES;
        }
    }

    // The first seven bytes: the four levels, seven bits each, low byte first, and the Djinn held,
    // seven bits an element; byte 3 holds the levels' last four bits above the Djinn's first four.
    out[0]                   = levelBits;
    out[1]                   = levelBits >> 8;
    out[2]                   = levelBits >> 16;
    out[3]                   = (levelBits >> 20 & 0xf0) | (djinnBits & 0x0f);
    out[4]                   = djinnBits >> 4;
    out[5]                   = djinnBits >> 12;
    out[6]                   = djinnBits >> 20;
    out[PASSWORD_FLAGS_BYTE] = flagBits;
    if (mode != PASSWORD_LEVEL_GOLD)
    {
        out[PASSWORD_KEY_ITEMS_BYTE] = keyItemBits;
    }
    return length;
}

// Encodes size bytes of data as password symbols in out and returns how many there are: every byte
// but the last XORed with the last, six bits a symbol, a checksum symbol after every nine, and each
// symbol plus its place.
static Sint32 scFileScreenEncodePassword(Uint8 *data, Sint32 size, Uint8 *out)
{
    Sint32 xorKey;
    Sint32 symbolCount;
    Sint32 i;
    Sint32 j;
    Sint32 inputBit;
    Sint32 inputIndex;
    Sint32 symbol;
    Sint32 groupSum;
    Sint32 groupSymbols;
    Sint32 srcBit;

    xorKey      = data[size - 1];
    symbolCount = 0;
    for (i = 0; i != size - 1; i++)
        data[i] ^= xorKey;
    inputBit     = 0;
    inputIndex   = 0;
    groupSum     = 0;
    groupSymbols = 0;
    do
    {
        symbol = 0;
        for (j = 0; j != 6; j++)
        {
            srcBit = (data[inputIndex] >> (7 - inputBit)) & 1;
            inputBit++;
            if (inputBit == 8)
            {
                inputBit = 0;
                inputIndex++;
            }
            symbol |= srcBit << (5 - j);
            if (inputIndex == size)
                break;
        }
        groupSymbols++;
        out[symbolCount++]  = symbol;
        groupSum           += symbol;
        if (groupSymbols == PASSWORD_SYMBOLS_PER_ROW - 1)
        {
            out[symbolCount++] = groupSum & PASSWORD_SYMBOL_MASK;
            groupSum           = 0;
            groupSymbols       = 0;
        }
    } while (inputIndex != size);
    for (i = 0; i != symbolCount; i++)
        out[i] = (out[i] + i) & PASSWORD_SYMBOL_MASK;
    return symbolCount;
}

// The password's checksum: over byteCount bytes of data, shifted a bit at a time from
// PASSWORD_CRC_SEED, PASSWORD_CRC_POLYNOMIAL subtracted when the top bit falls out, and the result
// complemented. The Lost Age's password screen checks it against a decoded password's last
// PASSWORD_CRC_BYTES.
static Uint16 scFileScreenCalculatePasswordCrc16(Uint32 byteCount, Uint8 *data)
{
    Uint32 crc;
    Uint32 byteIndex;
    Uint32 bitIndex;

    crc = PASSWORD_CRC_SEED;
    for (byteIndex = 0; byteIndex != byteCount; byteIndex++, data++)
    {
        crc ^= *data << 8;
        for (bitIndex = 0; bitIndex != 8; bitIndex++)
        {
            if ((crc & PASSWORD_CRC_TOP_BIT) != 0)
            {
                crc = (crc << 1) - PASSWORD_CRC_POLYNOMIAL;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return ~crc;
}

#include "shared/screen_entrances.c"

Uint32 FileScreenExits[] = {
    SCENE_EXITS_END,
};

ACTOR_PLACE FileScreenActorPlacements[] = {
    {SCENE_TABLE_END},
};

CUE FileScreenCues[] = {
    {CUE_TABLE_END},
};

// Ticks since scFileScreenFadeInBlend began; the blend level is half of it.
Sint16 FileScreenBlendTick         = 0;

// Set once FileScreenButtonCode has been entered; nothing here clears it.
Sint16 FileScreenCodeUnlocked      = 0;

// Set once FileScreenExtraButtonCode has been entered.
Sint16 FileScreenExtraCodeUnlocked = 0;

// The index in FileScreenButtonCode of the key to press next.
Sint16 FileScreenCodeProgress      = 0;

// The index in FileScreenExtraButtonCode of the key to press next.
Sint16 FileScreenExtraCodeProgress = 0;

// Set while the first code's last correct key is still held.
Sint16 FileScreenCodeHeld          = 0;

// The same for the second code: a word, of which only the low halfword is used.
Sint32 FileScreenExtraCodeHeld     = 0;

// clang-format off
// The party members the password packs: Isaac, Garet, Ivan and Mia.
Uint32 FileScreenPasswordMembers[PASSWORD_MEMBERS] = {
    ISAAC, GARET, IVAN, MIA,
};

// The six story flags of the password's PASSWORD_FLAGS_BYTE, bits 0 to 5.
ALIGNED(4) Uint16 FileScreenPasswordFlags[PASSWORD_FLAGS] = {
    FLAG_DODONPA_LOCKED_UP, FLAG_WON_COLOSSO,
    FLAG_HSU_RESCUED_WITHOUT_PARTY, FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED,
    FLAG_VISITED_SICK_DORA, FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES,
};

// The eight Psynergy items, ITEM_ORB_OF_FORCE to ITEM_CATCH_BEADS, which Silver and Bronze record in PASSWORD_KEY_ITEMS_BYTE.
ALIGNED(4) Uint16 FileScreenPasswordKeyItems[PASSWORD_KEY_ITEMS] = {
    ITEM_ORB_OF_FORCE, ITEM_DOUSE_DROP,
    ITEM_FROST_JEWEL, ITEM_LIFTING_GEM,
    ITEM_HALT_GEM, ITEM_CLOAK_BALL,
    ITEM_CARRY_STONE, ITEM_CATCH_BEADS,
};

// The items whose quantity a Gold password keeps for each party member.
//
// They are Herb, Nut, Vial, Potion, Psy Crystal, Antidote, Elixir, Water of Life, Power Bread, Cookie, Apple, Hard Nut, Mint, Lucky Pepper, Smoke Bomb, Sleep Bomb, Game Ticket, Lucky Medal, Sacred Feather, Oil Drop, Weasel's Claw, Bramble Seed and Crystal Powder.
Uint16 FileScreenPasswordStackItems[PASSWORD_STACK_ITEMS] = {
    ITEM_HERB, ITEM_NUT, ITEM_VIAL, ITEM_POTION, ITEM_PSY_CRYSTAL,
    ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_WATER_OF_LIFE, ITEM_POWER_BREAD,
    ITEM_COOKIE, ITEM_APPLE, ITEM_HARD_NUT, ITEM_MINT, ITEM_LUCKY_PEPPER,
    ITEM_SMOKE_BOMB, ITEM_SLEEP_BOMB, ITEM_GAME_TICKET, ITEM_LUCKY_MEDAL,
    ITEM_SACRED_FEATHER, ITEM_OIL_DROP, ITEM_WEASELS_CLAW, ITEM_BRAMBLE_SEED, ITEM_CRYSTAL_POWDER,
};
// clang-format on
