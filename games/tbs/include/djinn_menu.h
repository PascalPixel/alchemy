#ifndef ALCHEMY_DJINN_MENU_H
#define ALCHEMY_DJINN_MENU_H

#include "types.h"
#include "global_cells.h"
#include "render_input.h"

#define DJINN_ENTRY_INDEX 31
#define DJINN_ENTRY_ELEMENT 0xe0
#define DJINN_ENTRY_OWNER 0xf00
#define DJINN_ENTRY_IS_SET 0x8000

#define DJINN_ELEMENTS 4
#define DJINN_PER_ELEMENT 20
#define DJINN_LIST_OWNERS 8
#define DJINN_LIST_ROWS 10

struct DjinnListTable
{
    u16 ids[DJINN_LIST_OWNERS][DJINN_LIST_ROWS];
    s8  counts[DJINN_LIST_OWNERS];
};

struct DjinnMenuBackdrop
{
    struct DjinnListTable list;
    u8                    tiles[0x2000];
    u16                   palette[64];
    s32                   tutorialTimer;
    long                  tutorialStep;
};

struct DjinnMenuWork
{
    u8                        unk000[8];
    s32                       owner;
    u8                        unk00c[4];
    s32                       statusWindow;
    struct RenderOutput      *cursor;
    u8                        unk018[4];
    s8                        selectedColumn[2];
    u8                        unk01e[6];
    s32                       window;
    s32                       unknownWindow028;
    u8                        unk02c[4];
    s32                       listWindow;
    s32                       secondWindow;
    u8                        unk038[0xd4];
    s32                       helpWindow;
    u8                        unk110[0x24];
    s16                       ownerX[8];
    s16                       ownerY[8];
    u8                        unk154[0x20];
    u16                       selection[2];
    u16                       pairEntries[2];
    struct RenderOutput      *secondCursor;
    u8                        unk180[0x04];
    struct DjinnMenuBackdrop *backdrop;
    u8                        unk188[0x80];
    u16                       owners[DJINN_LIST_OWNERS];
    u8                        pairCount;
    u8                        ownerCount;
    u8                        shownOwner[2];
    u8                        unk21c[4];
    u16                       pending;
    u8                        unk222[0x12];
    u16                       slotX[4];
    u16                       slotY[4];
    u8                        unk244[0x10];
    u8                        pairIndex[2];
    u8                        pairElement[2];
    u8                        pairOwner[2];
};

LAYOUT_OFFSET_GUARD(DjinnMenuWork_StatusWindow, struct DjinnMenuWork, statusWindow, 0x10);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_SelectedColumn, struct DjinnMenuWork, selectedColumn, 0x1c);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_OwnerX, struct DjinnMenuWork, ownerX, 0x134);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_OwnerY, struct DjinnMenuWork, ownerY, 0x144);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_Selection, struct DjinnMenuWork, selection, 0x174);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_Pending, struct DjinnMenuWork, pending, 0x220);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_PairIndex, struct DjinnMenuWork, pairIndex, 0x254);
LAYOUT_OFFSET_GUARD(
    DjinnMenuBackdrop_TutorialTimer, struct DjinnMenuBackdrop, tutorialTimer, 0x2128);
LAYOUT_OFFSET_GUARD(DjinnMenuBackdrop_TutorialStep, struct DjinnMenuBackdrop, tutorialStep, 0x212c);

#endif
