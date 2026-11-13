/// @file
/// The field Psynergy The Lost Age adds, from Cyclone to Hover, each part named
/// for the Psynergy it runs (its kind in FieldPsynergy_Run), and the effects
/// they share. psynergy_internal.h holds the includes every part shares; the
/// parts under psynergy/ follow in ROM order.

#include "psynergy_internal.h"

// The parts, in ROM order.
#include "psynergy/cyclone.c"
#include "psynergy/blaze.c"
#include "psynergy/ripple.c"
#include "psynergy/growing_particle.c"
#include "psynergy/sand.c"
#include "psynergy/burst.c"
#include "psynergy/sinking_object.c"
#include "psynergy/pound.c"
#include "psynergy/growing_sprite.c"
#include "psynergy/scoop.c"
#include "psynergy/unnamed_28.c"
#include "psynergy/tremor.c"
#include "psynergy/grind.c"
#include "psynergy/hover.c"
#include "psynergy/mote_trail.c"
#include "psynergy/collision_tags.c"
