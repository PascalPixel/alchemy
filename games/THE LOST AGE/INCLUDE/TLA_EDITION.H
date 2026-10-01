#ifndef GUARD_TLA_EDITION_H
#define GUARD_TLA_EDITION_H

/* Historical targets define one edition; reject conflicting manual choices. */
#if (defined(TLA_EDITION_JA) + defined(TLA_EDITION_EN) + \
     defined(TLA_EDITION_DE) + defined(TLA_EDITION_ES) + \
     defined(TLA_EDITION_FR) + defined(TLA_EDITION_IT)) > 1
#error multiple TLA editions selected
#endif

/* The window work, as ☀️'s TBS_EDITION.H lays out its own: three channel
   slots, and twelve directly drawn windows where ☀️'s English build keeps
   eight at 0x500 with its channels at 0x620. */
#define RENDER_CHANNEL_OFS 0x6B8
#define RENDER_DIRECT_OFS  0x508
#define WINDOW_COUNT       12

/* The top menu's selection window width, sized to the English words as in
   ☀️; the other editions link their own copies until theirs are proven. */
#define SELECT_MENU_WIDTH 7

/* Where the debug selection's text sits, as in ☀️'s English editions. */
#define MENU_TEXT_X 74
#define MENU_TEXT_Y 60

/* Prefix scans stop after eight characters in these localizations and five
   in the Japanese/English editions. Localizations append a separating space. */
#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define NAME_PREFIX_LAST_INDEX 7
#else
#define NAME_PREFIX_LAST_INDEX 4
#endif

/* Columns follow the localized words; window flags and channel markers
   follow the Japanese window/name layout. */
#if defined(TLA_EDITION_JA)
#define SELECT_ENTRY_11_COLUMN 6
#define CONFIRM_ENTRY_19_COLUMN 8
#define WINDOW_BUSY_FLAGS_OFS 0x118A
#define SUMMON_CHANNEL_MARKER 'A'
#elif defined(TLA_EDITION_DE)
#define SELECT_ENTRY_11_COLUMN 8
#define CONFIRM_ENTRY_19_COLUMN 10
#define WINDOW_BUSY_FLAGS_OFS 0x138A
#define SUMMON_CHANNEL_MARKER '1'
#else
#define SELECT_ENTRY_11_COLUMN 7
#define CONFIRM_ENTRY_19_COLUMN 10
#define WINDOW_BUSY_FLAGS_OFS 0x138A
#define SUMMON_CHANNEL_MARKER '1'
#endif


#endif
