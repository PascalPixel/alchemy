#ifndef ALCHEMY_KALAY_H
#define ALCHEMY_KALAY_H

/* What Kalay (kalay.c), whose scripts set it, and Lord Hammet's palace
   (hammet_palace.c), where Layana answers it, share. */

#include "flags.h"

/* The party has heard in Lunpa that Hammet is held there, or has freed him, so
   Layana waits elsewhere in her room and says otherwise. An area flag, cleared
   when the party enters another area. */
#define FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA FLAG_AREA_TEMP_21

#endif
