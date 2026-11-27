// The Status screen's tables: its cursor stops and its labels.

#include "edition.h"
#include "window/screens.h"

// clang-format off
// The Status screen's stops among its stats, each {help entry, column, row, highlight column, highlight row, highlight width}: nine for a member with Djinn, then seven for one without.
const BATTLE_SUMMARY_STAT_CURSOR OwnerStatusStatCursors[] = {
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

// The stops on Attack, Defense, Agility and Luck.
const BATTLE_SUMMARY_CURSOR OwnerStatusEquipmentCursors[] = {
	{.entry = 10, .column = 19, .row = 1},
	{.entry = 11, .column = 19, .row = 2},
	{.entry = 12, .column = 19, .row = 3},
	{.entry = 13, .column = 19, .row = 4},
};

// The stops on the status icons.
const BATTLE_SUMMARY_CURSOR OwnerStatusConditionCursors[] = {
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

// The Status screen's labels: the German, Spanish, French and Italian editions carry every
// language's level and HP abbreviations.
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
const Uint8 OwnerStatusNivString[] = "Niv"; // level, in French
const Uint8 OwnerStatusNvString[]  = "Nv."; // level, in Spanish
#endif
// level, in the other languages
const Uint8 OwnerStatusLevelString[] = "Lv";
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
const Uint8 OwnerStatusKpString[] = "KP"; // HP, in German
const Uint8 OwnerStatusPvString[] = "PV"; // HP, in French and Spanish
const Uint8 OwnerStatusHpString[] = "PS"; // HP, in Italian
#else
const Uint8 OwnerStatusHpString[] = "HP"; // HP
#endif
// between a value and its maximum
const Uint8 OwnerStatusSlashString[] = "/";
#if LANGUAGE != LANGUAGE_JAPANESE
const Uint8 OwnerStatusPpString[] = "PP"; // PP
#else
// PP, as the Japanese edition writes it.
const Uint8 OwnerStatusPpString[] = "EP";
#endif
