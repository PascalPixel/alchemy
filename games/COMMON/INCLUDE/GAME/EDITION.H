#ifndef GUARD_EDITION_H
#define GUARD_EDITION_H

/* A build selects one edition. Japanese is the source language when none
   is selected; international behavior belongs only to the named editions. */
#if (defined(TBS_EDITION_JA) + defined(TBS_EDITION_EN) + \
     defined(TBS_EDITION_DE) + defined(TBS_EDITION_ES) + \
     defined(TBS_EDITION_FR) + defined(TBS_EDITION_IT) + \
     defined(TLA_EDITION_JA) + defined(TLA_EDITION_EN) + \
     defined(TLA_EDITION_DE) + defined(TLA_EDITION_ES) + \
     defined(TLA_EDITION_FR) + defined(TLA_EDITION_IT)) > 1
#error multiple game editions selected
#endif

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || \
    defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define EDITION_INTERNATIONAL 1
#else
#define EDITION_INTERNATIONAL 0
#endif

#endif
