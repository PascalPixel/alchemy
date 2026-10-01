#ifndef GUARD_TLA_EDITION_H
#define GUARD_TLA_EDITION_H

/* Historical targets define one edition; reject conflicting manual choices. */
#if (defined(TLA_EDITION_JA) + defined(TLA_EDITION_EN) + \
     defined(TLA_EDITION_DE) + defined(TLA_EDITION_ES) + \
     defined(TLA_EDITION_FR) + defined(TLA_EDITION_IT)) > 1
#error multiple TLA editions selected
#endif

/* The top menu's selection window width, sized to the English words as in
   ☀️; the other editions link their own copies until theirs are proven. */
#define SELECT_MENU_WIDTH 7

#endif
