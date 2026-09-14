#ifndef GUARD_TLA_EDITION_H
#define GUARD_TLA_EDITION_H

/* Historical targets define one edition; reject conflicting manual choices. */
#if (defined(TLA_EDITION_JA) + defined(TLA_EDITION_EN) + \
     defined(TLA_EDITION_DE) + defined(TLA_EDITION_ES) + \
     defined(TLA_EDITION_FR) + defined(TLA_EDITION_IT)) > 1
#error multiple TLA editions selected
#endif

#endif
