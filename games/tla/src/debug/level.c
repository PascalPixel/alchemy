// The debug room Increase item and level, as The Broken Seal's: its actors raise the party's
// levels, give any item and show the game's icons and portraits. Through entrances 96 to 99 the
// same scene is instead one of four rooms of people in rows, each of whom says their name when
// talked to.

#include "debug/debug.h"
#include "field/field.h"
#include "menu/menu.h"
#include "field/script_inline.h"
#include "text/msg_ids.h"
#include "battle/battle_unit.h"
#include "game/character_ids.h"
#include "field/field_mode_script.h"
#include "window/screens.h"
#include "window/window.h"
#include "field/field_psynergy_state.h"

extern ACTOR_PLACE IncreaseItemAndLevelActorPlacements[];
extern ACTOR_PLACE IncreaseItemAndLevelFirstNamePlacements[];
extern ACTOR_PLACE IncreaseItemAndLevelSecondNamePlacements[];
extern ACTOR_PLACE IncreaseItemAndLevelThirdNamePlacements[];
extern ACTOR_PLACE IncreaseItemAndLevelFourthNamePlacements[];
extern CUE         IncreaseItemAndLevelNameCues[];
extern CUE         IncreaseItemAndLevelCues[];
extern char        IncreaseItemAndLevelLvLabel[];
extern char        IncreaseItemAndLevelItemPrompt[];
extern char        IncreaseItemAndLevelItemHelp[];
extern char        IncreaseItemAndLevelItemFull[];
void               dbIncreaseItemAndLevelRunMotionTest(void);
void               dbIncreaseItemAndLevelKeepPortraitBrowserInPlace(void);
void               dbIncreaseItemAndLevelOpenIconBrowser(void);
void               dbIncreaseItemAndLevelOpenPortraitBrowser(void);

// The entrances that open a room of names in place of the debug room: each has its own actor
// placements and the cues of IncreaseItemAndLevelNameCues.
enum
{
    ENTRANCE_NAMES_FOURTH = 96,
    ENTRANCE_NAMES_THIRD  = 97,
    ENTRANCE_NAMES_SECOND = 98,
    ENTRANCE_NAMES_FIRST  = 99
};

// Entrances up to ENTRANCE_DEBUG_ROOM_LAST open the debug room; those above ENTRANCE_NAMES_ABOVE
// take the rooms of names' cues.
enum
{
    ENTRANCE_DEBUG_ROOM_LAST = 89,
    ENTRANCE_NAMES_ABOVE     = 94
};

// The debug room's actors, numbering the rows of IncreaseItemAndLevelActorPlacements, as The Broken
// Seal's.
typedef enum INCREASE_ITEM_AND_LEVEL_ACTOR_ID
{
    // dbIncreaseItemAndLevelSelectItem
    ACTOR_ITEM_BROWSER = ACTOR_FIRST_PLACED,
    ACTOR_ICON_BROWSER, // dbIncreaseItemAndLevelOpenIconBrowser
                        // dbIncreaseItemAndLevelOpenPortraitBrowser; Force cast at it runs
                        // dbIncreaseItemAndLevelRunMotionTest.
    ACTOR_PORTRAIT_BROWSER,
    // dbIncreaseItemAndLevelRunLevelEditor, drawn larger than the others.
    ACTOR_LEVEL_EDITOR
} INCREASE_ITEM_AND_LEVEL_ACTOR_ID;

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return SceneEntrances;
}

const struct SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return 0;
}

const Uint32 *scGetExits(void)
{
    return SceneExits;
}

// The scene's actor placements: a room of names for its entrance, else the debug room's.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (MainRoutineStateWork.entrance == ENTRANCE_NAMES_FIRST)
        return IncreaseItemAndLevelFirstNamePlacements;
    if (MainRoutineStateWork.entrance == ENTRANCE_NAMES_SECOND)
        return IncreaseItemAndLevelSecondNamePlacements;
    if (MainRoutineStateWork.entrance == ENTRANCE_NAMES_THIRD)
        return IncreaseItemAndLevelThirdNamePlacements;
    if (MainRoutineStateWork.entrance == ENTRANCE_NAMES_FOURTH)
        return IncreaseItemAndLevelFourthNamePlacements;
    return IncreaseItemAndLevelActorPlacements;
}

// How much larger ACTOR_LEVEL_EDITOR is drawn in the debug room.
#define LEVEL_EDITOR_SCALE (toFIXED(17) / 10)

// Shows the name of the person talked to in a room of names: actor `actorId` says the message its
// room's list of names gives it, counted from the room's first name: "Master Poi" in the first
// room, "Susa" in the second, "Alex" in the third and "Weapons Shop" in the fourth.
void dbIncreaseItemAndLevelShowName(Sint32 actorId)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    if (MainRoutineStateWork.entrance == ENTRANCE_NAMES_FIRST)
        fdScriptSetMessageId(actorId - ACTOR_FIRST_PLACED + (Sint32)MsgNamesMasterPoi);
    else if (MainRoutineStateWork.entrance == ENTRANCE_NAMES_SECOND)
        fdScriptSetMessageId(actorId - ACTOR_FIRST_PLACED + (Sint32)MsgNamesSusa);
    else if (MainRoutineStateWork.entrance == ENTRANCE_NAMES_THIRD)
        fdScriptSetMessageId(actorId - ACTOR_FIRST_PLACED + (Sint32)MsgNamesAlex);
    else
        fdScriptSetMessageId(actorId - ACTOR_FIRST_PLACED + (Sint32)MsgNamesWeaponsShop);
    fdScriptShowMessageAndWait(actorId, 0);
    fdScriptEnd();
}

// Shows a party member's name when they are talked to in a room of names, the line of the list from
// MsgNamesIsaac at their character number, with the character as the message's actor argument. The
// list runs Isaac, Garet, Ivan, Mia, Jenna, Sheba, Piers with no Felix, so from JENNA on each says
// the next name: Jenna "Sheba", Sheba "Piers" and Piers "Alex".
void dbIncreaseItemAndLevelShowCharacterName(Sint32 character)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdScriptSetMessageId(character + (Sint32)MsgNamesIsaac);
    mnMessageArgumentPush(character, MESSAGE_ARGUMENT_ACTOR);
    fdScriptShowMessageAndWait(character, 0);
    // Garet's name is followed by Isaac saying the list's next line, "Ivan". XXX (no evidence):
    // why; the debug room's scripts cannot be recorded, and no text or code of either game says.
    if (character == GARET)
        fdScriptShowMessageAndWait(ISAAC, 0);
    fdScriptEnd();
}

// The scene's cues: the rooms of names' for any entrance above ENTRANCE_NAMES_ABOVE, else the debug
// room's.
const CUE *scGetCues(void)
{
    if (MainRoutineStateWork.entrance > ENTRANCE_NAMES_ABOVE)
        return IncreaseItemAndLevelNameCues;
    return IncreaseItemAndLevelCues;
}

// Raises one party member `amount` levels and works out their new stats.
void dbIncreaseItemAndLevelRaiseMemberLevel(Sint32 ownerId, Sint32 amount)
{
    gmUnitLevelUpTo(ownerId, syUnitGet(ownerId)->level + amount);
    gmUnitCalculate(ownerId);
}

// Raises every party member `amount` levels.
void dbIncreaseItemAndLevelRaisePartyLevels(Sint32 amount)
{
    Uint16 ownerIds[16];
    Sint32 ownerCount;
    Sint32 i;

    ownerCount = syFieldPartyListMembers(ownerIds);
    for (i = 0; i < ownerCount; i++)
        dbIncreaseItemAndLevelRaiseMemberLevel(ownerIds[i], amount);
}

// Displays the leader's level and changes the whole party's, as The Broken Seal's: talking to
// ACTOR_LEVEL_EDITOR shows `Raise everyone's level.` over the leader's name and level; Start or
// Select raises the whole party five levels to the Super Lucky jingle, A one level to the Lucky
// one. B closes the window and works out the stats of Isaac, Garet, Mia and Ivan again.
void dbIncreaseItemAndLevelRunLevelEditor(void)
{
    UNIT   *unit;
    WINDOW *window;
    Sint32  redraw;
    Sint32  msgId;

    unit   = syUnitGet(MainRoutineStateWork.leaderActorId);
    window = mnWindowCreate(0, 0, 30, 9, WINDOW_FLAG_INSTANT);

    msgId  = (Sint32)MsgRaiseEveryoneS;
    mnTextDrawMessage(msgId, window, 0, 0);
    mnTextDrawMessage(msgId + 1, window, 0, 16);
    msgId  += 2;
    redraw  = 1;
    mnTextDrawMessage(msgId, window, 0, 32);

    for (;;)
    {
        if (redraw != 0)
        {
            mnWindowSpriteRedrawSavedRect(window);
            mnTextDrawStringAtOffset(unit->name, window, 0, 48);
            mnTextDrawStringInWindow(IncreaseItemAndLevelLvLabel, window, 48, 48);
            redraw = 0;
            mnTextDrawNumberInWindow(unit->level, 0, window, 72, 48);
        }

        if ((PerWork.push & KEY_START) != 0 || (PerWork.push & KEY_SELECT) != 0)
        {
            dbIncreaseItemAndLevelRaisePartyLevels(5);
            sySoundPlay(MUS_SUPER_LUCKY);
            redraw = 1;
        }

        if ((PerWork.push & KEY_A) != 0)
        {
            dbIncreaseItemAndLevelRaisePartyLevels(1);
            sySoundPlay(MUS_LUCKY);
            redraw = 1;
        }

        if ((PerWork.push & KEY_B) != 0)
        {
            sySoundPlay(SE_MENU_CANCEL);
            mnWindowSpriteRedrawSavedRect(window);
            sySynch(1);
            mnWindowClose(window, 1);

            gmUnitCalculate(ISAAC);
            gmUnitCalculate(GARET);
            gmUnitCalculate(MIA);
            gmUnitCalculate(IVAN);
            return;
        }

        sySynch(1);
    }
}

// The browser's item numbers, 0 to 511, wrapping.
#define ITEM_BROWSER_ITEM_COUNT 512

// Browses nonempty item definitions and gives the selected item to the party, as The Broken Seal's:
// talking to ACTOR_ITEM_BROWSER. Up and Down step one number, Right and Left ten, R and L thirty,
// passing over numbers whose item has no icon; A gives the item to the first party member with
// room, and B, or A with every bag full, closes the browser.
void dbIncreaseItemAndLevelSelectItem(void)
{
    void  *window;
    void  *details;
    Sint32 selectedItemId;
    Sint32 redraw;
    Sint32 itemId;
    Sint32 entryCount;
    Sint8  seekDirection;

    entryCount    = ITEM_BROWSER_ITEM_COUNT;
    seekDirection = 0;
    sySoundPlay(SE_MENU_CONFIRM);
    selectedItemId = 1;
    redraw         = 1;
    window         = mnWindowCreate(0, 0, 30, 7, WINDOW_FLAG_INSTANT);
#if LANGUAGE < LANGUAGE_GERMAN
    details = mnWindowCreate(0, 8, 13, 10, WINDOW_FLAG_INSTANT);
#else
    details = mnWindowCreate(0, 9, 18, 10, WINDOW_FLAG_INSTANT);
#endif
    // Lend the highlight palette to the item icons, as The Broken Seal's: sprite palette 0 into it,
    // then the window background colour over its text background.
    syDMAXCopy((const void *)OBJ_PLTT,
               (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
               DMA_COPY_HALFWORDS(16),
               REG_DMA3);
    syDMAXCopy((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
               (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
               DMA_COPY_HALFWORDS(1),
               REG_DMA3);
    sySynch(1);
    while (1)
    {
        if (redraw)
        {
            redraw         = 0;
            // Wrap the number into 0 to 511 (a step back from 0 is 511).
            selectedItemId = (selectedItemId + ITEM_BROWSER_ITEM_COUNT) % entryCount;
            mnWindowSpriteRedrawSavedRect(window);
            mnWindowSpriteClearList(window);
            mnTextDrawStringInWindow(IncreaseItemAndLevelItemPrompt, window, 0, 0);
            mnTextDrawNumberAtOffset(selectedItemId, 0, window, 80, 0);
            if (gmPartyInventoryHasSpace())
            {
                mnTextDrawStringInWindow(IncreaseItemAndLevelItemHelp, window, 0, 32);
                itemId = selectedItemId & INVENTORY_ITEM_ID_MASK;
                gmItemGetDirect(itemId);
                mnTextDrawMessageAsTiles(itemId + (Sint32)MsgItemName, window, 120, 0);
                mnTextDrawMessage(itemId + (Sint32)MsgItemDescription, window, 0, 16);
                mnWindowSpriteRedrawSavedRect(details);
                mnItemMenuDrawItemDetails(details, selectedItemId);
            }
            else
            {
                mnTextDrawStringInWindow(IncreaseItemAndLevelItemFull, window, 0, 32);
            }
        }
        if (PerWork.push & KEY_A)
        {
            if (gmPartyInventoryAdd(selectedItemId) == -1)
            {
                sySoundPlay(SE_MENU_CANCEL);
                break;
            }
            sySoundPlay(SE_NOTE_HIGH);
        }
        if (PerWork.push & KEY_B)
        {
            sySoundPlay(SE_MENU_CANCEL);
            break;
        }
        if (PerWork.repeat & KEY_UP)
        {
            seekDirection = -1;
            selectedItemId--;
            redraw = 1;
            sySoundPlay(SE_MENU_CURSOR_MOVE);
        }
        if (PerWork.repeat & KEY_DOWN)
        {
            seekDirection = 1;
            selectedItemId++;
            redraw = 1;
            sySoundPlay(SE_MENU_CURSOR_MOVE);
        }
        if (PerWork.repeat & KEY_RIGHT)
        {
            seekDirection   = 1;
            selectedItemId += DEBUG_BROWSER_ROW_STEP;
            redraw          = 1;
            sySoundPlay(SE_MENU_CURSOR_MOVE);
        }
        if (PerWork.repeat & KEY_LEFT)
        {
            seekDirection   = -1;
            selectedItemId -= DEBUG_BROWSER_ROW_STEP;
            redraw          = 1;
            sySoundPlay(SE_MENU_CURSOR_MOVE);
        }
        if (PerWork.repeat & KEY_R)
        {
            seekDirection   = 1;
            selectedItemId += DEBUG_BROWSER_PAGE_STEP;
            redraw          = 1;
            sySoundPlay(SE_MENU_CURSOR_MOVE);
        }
        if (PerWork.repeat & KEY_L)
        {
            seekDirection   = -1;
            selectedItemId -= DEBUG_BROWSER_PAGE_STEP;
            redraw          = 1;
            sySoundPlay(SE_MENU_CURSOR_MOVE);
        }
        // Skip items with no icon in the chosen direction.
        if (seekDirection == -1)
        {
            selectedItemId = (selectedItemId + ITEM_BROWSER_ITEM_COUNT) % entryCount;
            while (gmItemGetDirect(selectedItemId & INVENTORY_ITEM_ID_MASK)->icon == 0)
                selectedItemId = (selectedItemId + ITEM_BROWSER_ITEM_COUNT - 1) % entryCount;
        }
        if (seekDirection == 1)
        {
            selectedItemId = (selectedItemId + ITEM_BROWSER_ITEM_COUNT) % entryCount;
            while (gmItemGetDirect(selectedItemId & INVENTORY_ITEM_ID_MASK)->icon == 0)
                selectedItemId = (selectedItemId + ITEM_BROWSER_ITEM_COUNT + 1) % entryCount;
        }
        seekDirection = 0;
        sySynch(1);
    }
    mnWindowSpriteRedrawSavedRect(window);
    sySynch(1);
    mnWindowClose(window, 1);
    mnWindowClose(details, 1);
}

// The level editor's label before a member's level.
char           IncreaseItemAndLevelLvLabel[4]     = "Lv";

// The item selector's prompt, before the item number.
char           IncreaseItemAndLevelItemPrompt[12] = "Item No:";

// The item selector's help line, while the bags have room.
char           IncreaseItemAndLevelItemHelp[24]   = "A:Get Item  B:Return";

// The item selector's line when the party's bags are full.
char           IncreaseItemAndLevelItemFull[20]   = "ITEM FULL........";

SCENE_ENTRANCE SceneEntrances[]                   = {
    {0, CONDITION_ALWAYS, 80, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 80, 0, 272, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_NAMES_FOURTH, CONDITION_ALWAYS, 192, 0, 240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_NAMES_THIRD, CONDITION_ALWAYS, 192, 0, 240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_NAMES_SECOND, CONDITION_ALWAYS, 176, 0, 240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_NAMES_FIRST, CONDITION_ALWAYS, 176, 0, 240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// The room has no exits of its own.
Uint32 SceneExits[] = {
    SCENE_EXITS_END,
};

// The debug room's actors, in the order of INCREASE_ITEM_AND_LEVEL_ACTOR_ID.
ACTOR_PLACE IncreaseItemAndLevelActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    48,
                    0,
                    272,
                    FACING_SOUTHEAST,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    48,
                    0,
                    72,
                    FACING_SOUTHEAST,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_SUN_DISC_STATUE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    48,
                    0,
                    136,
                    FACING_SOUTHEAST,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_CAT,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    104,
                    0,
                    272,
                    FACING_SOUTHEAST,
                    TALK_FACING_PARTY,
                    0),
    {SCENE_TABLE_END},
};

// The first room of names, from Master Poi.
ACTOR_PLACE IncreaseItemAndLevelFirstNamePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_POI,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    160,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_RIKI,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    192,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_TAVI,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    224,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    256,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    288,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_ALHAFRAS_MAYOR,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    320,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_FORTUNE_TELLER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    352,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    160,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_MOAPA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    192,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_MAHA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    224,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_BRIGGS,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    256,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_CHAUCHA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    288,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_EOLEO,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    320,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_OBABA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    352,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    {SCENE_TABLE_END},
};

// The second room of names.
ACTOR_PLACE IncreaseItemAndLevelSecondNamePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUSA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    256,
                    0,
                    160,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_LADY_UZUME,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    256,
                    0,
                    192,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_KUSHINADA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    256,
                    0,
                    224,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_HYDROS,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    256,
                    0,
                    256,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_CONSERVATO,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    256,
                    0,
                    288,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_KARST,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    256,
                    0,
                    320,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_AGATIO,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    256,
                    0,
                    352,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_JENNAS_FATHER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    160,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_JENNAS_MOTHER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    192,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_KYLE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    224,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_LUNPA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    256,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_PUELLE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    288,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_MASKED_SHAMAN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    320,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_PROX_ELDER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    352,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    {SCENE_TABLE_END},
};

// The third room of names, from Alex.
ACTOR_PLACE IncreaseItemAndLevelThirdNamePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ISAAC,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    160,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_GARET,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    192,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_IVAN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    224,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_MIA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    256,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_JENNA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    288,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_SHEBA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    320,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_PIERS,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    160,
                    0,
                    352,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_ALEX,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    160,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_KRADEN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    192,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_HAMMA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    224,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_HEALER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    256,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    288,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TEAL_SHIRT,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    336,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_HEADSCARF,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    368,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_FEIZHI,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    208,
                    0,
                    400,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    {SCENE_TABLE_END},
};

// The fourth room of names.
ACTOR_PLACE IncreaseItemAndLevelFourthNamePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    128,
                    0,
                    160,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    128,
                    0,
                    192,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    128,
                    0,
                    224,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_CREAM,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    128,
                    0,
                    256,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_IN_RED,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    128,
                    0,
                    288,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_ORANGE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    128,
                    0,
                    320,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    176,
                    0,
                    160,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_BEARD,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    176,
                    0,
                    192,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_LONG_BLONDE_HAIR,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    176,
                    0,
                    224,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_HAIR,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    176,
                    0,
                    256,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    176,
                    0,
                    288,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    176,
                    0,
                    320,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    224,
                    0,
                    160,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    224,
                    0,
                    192,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    224,
                    0,
                    224,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_ARTIFACT_MERCHANT,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    224,
                    0,
                    256,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    224,
                    0,
                    288,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    224,
                    0,
                    320,
                    FACING_SOUTHEAST,
                    TALK_FACING_KEEP,
                    0),
    {SCENE_TABLE_END},
};

// Talking to each tester runs its tool, as The Broken Seal's Japanese edition has it: Force cast at
// the portrait browser runs a motion test, and a second row on the same person, run before the
// cast, sets the effect's child mode, so Force's shot leaves the portrait browser where it stands.
CUE IncreaseItemAndLevelCues[] = {
    {CUE_TALK, ACTOR_ITEM_BROWSER, CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelSelectItem)},
    {CUE_TALK,
     ACTOR_ICON_BROWSER, CONDITION_ALWAYS,
     SCRIPT_REF(dbIncreaseItemAndLevelOpenIconBrowser)},
    {CUE_TALK,
     ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     SCRIPT_REF(dbIncreaseItemAndLevelOpenPortraitBrowser)},
    {CUE_TALK,
     ACTOR_LEVEL_EDITOR, CONDITION_ALWAYS,
     SCRIPT_REF(dbIncreaseItemAndLevelRunLevelEditor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE),
     ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     SCRIPT_REF(dbIncreaseItemAndLevelRunMotionTest)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     SCRIPT_REF(dbIncreaseItemAndLevelKeepPortraitBrowserInPlace)},
    {CUE_TABLE_END},
};

// The rooms of names' cues: each party member says their name, and each of the first 25 people
// placed theirs. The debug room's two Force rows stay at the end.
CUE IncreaseItemAndLevelNameCues[] = {
    {CUE_TALK, ISAAC, CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowCharacterName)},
    {CUE_TALK, GARET, CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowCharacterName)},
    {CUE_TALK, IVAN, CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowCharacterName)},
    {CUE_TALK, MIA, CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowCharacterName)},
    {CUE_TALK, JENNA, CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowCharacterName)},
    {CUE_TALK, SHEBA, CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowCharacterName)},
    {CUE_TALK, PIERS, CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowCharacterName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 0,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 1,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 2,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 3,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 4,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 5,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 6,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 7,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 8,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 9,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 10,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 11,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 12,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 13,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 14,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 15,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 16,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 17,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 18,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 19,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 20,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 21,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 22,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 23,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {CUE_TALK,
     ACTOR_FIRST_PLACED + 24,
     CONDITION_ALWAYS, SCRIPT_REF(dbIncreaseItemAndLevelShowName)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE),
     ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     SCRIPT_REF(dbIncreaseItemAndLevelRunMotionTest)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     SCRIPT_REF(dbIncreaseItemAndLevelKeepPortraitBrowserInPlace)},
    {CUE_TABLE_END},
};

// Unused: requests an exit through a window transition over 24 ticks; no cue row names it.
void dbIncreaseItemAndLevelRequestExit(Sint32 countdown)
{
    FIELD_STATE *event     = RamHeapSlots->fieldState;

    event->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    event->transitionFrame = 24;
    syImportScriptSetExitRequest(countdown);
}

// Opens the room through the window transition, and in the debug room draws ACTOR_LEVEL_EDITOR 1.7
// times its size, as The Broken Seal's Japanese edition does.
Sint32 scInit(void)
{
    FIELD_STATE *event     = RamHeapSlots->fieldState;

    event->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    event->transitionFrame = 24;
    if (MainRoutineStateWork.entrance <= ENTRANCE_DEBUG_ROOM_LAST)
    {
        fdActorGetById(ACTOR_LEVEL_EDITOR)->scaleY = LEVEL_EDITOR_SCALE;
        fdActorGetById(ACTOR_LEVEL_EDITOR)->scaleX = LEVEL_EDITOR_SCALE;
    }
    return 0;
}

// Runs the animation test, then moves ACTOR_LEVEL_EDITOR to its test position, as The Broken Seal's
// Japanese edition: Force cast at ACTOR_PORTRAIT_BROWSER walks it and ACTOR_LEVEL_EDITOR shakes its
// head; once ACTOR_PORTRAIT_BROWSER's animation has changed, ACTOR_LEVEL_EDITOR moves to (312,
// 416).
void dbIncreaseItemAndLevelRunMotionTest(void)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorSetAnimationById(ACTOR_PORTRAIT_BROWSER, ANIM_WALK);
    fdActorSetAnimationById(ACTOR_LEVEL_EDITOR, ANIM_SHAKE_HEAD);
    fdActorWaitForAnimationChange(ACTOR_PORTRAIT_BROWSER);
    fdActorSetPos(ACTOR_LEVEL_EDITOR, 312, 416);
    fdScriptEnd();
}

// Keeps Force from moving ACTOR_PORTRAIT_BROWSER: run just before Force is cast at it, this sets
// the effect's childMode, and Force's shot then leaves its target where it is.
void dbIncreaseItemAndLevelKeepPortraitBrowserInPlace(void)
{
    FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffectState;

    state->childMode          = 1;
}

Sint32 scPrepareMap(void)
{
    return 0;
}

// Talking to ACTOR_ICON_BROWSER: pages of item, Psynergy and status icons.
void dbIncreaseItemAndLevelOpenIconBrowser(void)
{
    mnDebugBrowserIcons();
}

// Talking to ACTOR_PORTRAIT_BROWSER: the portraits, one at a time.
void dbIncreaseItemAndLevelOpenPortraitBrowser(void)
{
    mnDebugBrowserPortraits();
}
