/// @file
/// The Status screen's tables: its cursor stops and its labels.

#include "types.h"
#include "edition.h"
#include "window/screens.h"

/// The Status screen's stops among its stats, each {help entry, column, row,
/// highlight column, highlight row, highlight width}: nine for a member with
/// Djinn, then seven for one without.
// clang-format off
const struct BattleSummaryStatCursor gOwnerStatus_StatCursors[] = {
    {.entry = 0, .column = 2, .row = 0, .highlightColumn = 0, .highlightRow = 0, .highlightWidth = 12},
    {.entry = 2, .column = 8, .row = 1, .highlightColumn = 0, .highlightRow = 1, .highlightWidth = 13},
    {.entry = 3, .column = 12, .row = 3, .highlightColumn = 5, .highlightRow = 3, .highlightWidth = 11},
    {.entry = 4, .column = 12, .row = 4, .highlightColumn = 5, .highlightRow = 4, .highlightWidth = 11},
    {.entry = 5, .column = 3, .row = 6, .highlightColumn = 0, .highlightRow = 6, .highlightWidth = 10},
    {.entry = 6, .column = 13, .row = 9, .highlightColumn = 0, .highlightRow = 9, .highlightWidth = 20},
    {.entry = 7, .column = 13, .row = 10, .highlightColumn = 0, .highlightRow = 10, .highlightWidth = 20},
    {.entry = 8, .column = 13, .row = 11, .highlightColumn = 0, .highlightRow = 11, .highlightWidth = 20},
    {.entry = 9, .column = 13, .row = 12, .highlightColumn = 0, .highlightRow = 12, .highlightWidth = 20},
    {.entry = 0, .column = 2, .row = 0, .highlightColumn = 0, .highlightRow = 0, .highlightWidth = 12},
    {.entry = 2, .column = 8, .row = 1, .highlightColumn = 0, .highlightRow = 1, .highlightWidth = 13},
    {.entry = 3, .column = 12, .row = 3, .highlightColumn = 5, .highlightRow = 3, .highlightWidth = 11},
    {.entry = 4, .column = 12, .row = 4, .highlightColumn = 5, .highlightRow = 4, .highlightWidth = 11},
    {.entry = 7, .column = 13, .row = 10, .highlightColumn = 0, .highlightRow = 10, .highlightWidth = 20},
    {.entry = 8, .column = 13, .row = 11, .highlightColumn = 0, .highlightRow = 11, .highlightWidth = 20},
    {.entry = 9, .column = 13, .row = 12, .highlightColumn = 0, .highlightRow = 12, .highlightWidth = 20},
};

/// The stops on Attack, Defense, Agility and Luck.
const struct BattleSummaryCursor gOwnerStatus_EquipmentCursors[] = {
    {.entry = 10, .column = 19, .row = 1},
    {.entry = 11, .column = 19, .row = 2},
    {.entry = 12, .column = 19, .row = 3},
    {.entry = 13, .column = 19, .row = 4},
};

/// The stops on the status icons.
const struct BattleSummaryCursor gOwnerStatus_ConditionCursors[] = {
    {.entry = 14, .column = 13, .row = 0},
    {.entry = 15, .column = 15, .row = 0},
    {.entry = 16, .column = 17, .row = 0},
    {.entry = 17, .column = 19, .row = 0},
    {.entry = 18, .column = 21, .row = 0},
    {.entry = 19, .column = 23, .row = 0},
    {.entry = 20, .column = 25, .row = 0},
    {.entry = 21, .column = 27, .row = 0},
};
// clang-format on

// The Status screen's labels: the German, Spanish, French and Italian
// editions carry every language's level and HP abbreviations.
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
const u8 gOwnerStatus_NivString[] = "Niv";
const u8 gOwnerStatus_NvString[] = "Nv.";
#endif
const u8 gOwnerStatus_LevelString[] = "Lv";
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
const u8 gOwnerStatus_KpString[] = "KP";
const u8 gOwnerStatus_PvString[] = "PV";
const u8 gOwnerStatus_HpString[] = "PS";
#else
const u8 gOwnerStatus_HpString[] = "HP";
#endif
const u8 gOwnerStatus_SlashString[] = "/";
#if LANGUAGE != LANGUAGE_JAPANESE
const u8 gOwnerStatus_PpString[] = "PP";
#else
const u8 gOwnerStatus_PpString[] = "EP";
#endif
