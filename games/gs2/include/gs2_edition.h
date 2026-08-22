#ifndef GUARD_GS2_EDITION_H
#define GUARD_GS2_EDITION_H

/* Historical targets define one edition; reject conflicting manual choices. */
#if (defined(GS2_EDITION_JA) + defined(GS2_EDITION_EN) + \
     defined(GS2_EDITION_DE) + defined(GS2_EDITION_ES) + \
     defined(GS2_EDITION_FR) + defined(GS2_EDITION_IT)) > 1
#error multiple GS2 editions selected
#endif

#endif
