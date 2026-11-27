/* @file */
/* The palette of a Teleport circle, which every scene with a circle keeps a */
/* copy of: the seven background palette entries the circle is drawn in, its */
/* colours and its shimmer's (teleport_circle_colors.c), and the entries each */
/* step of the shimmer paints those colours into, so that they run round the */
/* circle (scPaletteSetColsFade). */
/* */
/* An includer defines TELEPORT_CIRCLE_FIRST_ENTRY, the first of its circle's */
/* seven entries, first. */

#define CIRCLE_ENTRY(n) (TELEPORT_CIRCLE_FIRST_ENTRY + (n))

/* The entries the circle is drawn in. */
u16 TeleportCircle_PaletteSlots[] = {
	CIRCLE_ENTRY(0), CIRCLE_ENTRY(1), CIRCLE_ENTRY(2), CIRCLE_ENTRY(3),
	CIRCLE_ENTRY(4), CIRCLE_ENTRY(5), CIRCLE_ENTRY(6), PALETTE_LIST_END,
};

#include "teleport_circle_colors.c"

/* The shimmer's six steps: for each, the entries its colours go to. */
u16 TeleportCircle_ShimmerSlots[] ALIGNED(4) = {
	CIRCLE_ENTRY(5), CIRCLE_ENTRY(0), CIRCLE_ENTRY(1), CIRCLE_ENTRY(2),
	CIRCLE_ENTRY(3), CIRCLE_ENTRY(4), CIRCLE_ENTRY(4), CIRCLE_ENTRY(5),
	CIRCLE_ENTRY(0), CIRCLE_ENTRY(1), CIRCLE_ENTRY(2), CIRCLE_ENTRY(3),
	CIRCLE_ENTRY(3), CIRCLE_ENTRY(4), CIRCLE_ENTRY(5), CIRCLE_ENTRY(0),
	CIRCLE_ENTRY(1), CIRCLE_ENTRY(2), CIRCLE_ENTRY(2), CIRCLE_ENTRY(3),
	CIRCLE_ENTRY(4), CIRCLE_ENTRY(5), CIRCLE_ENTRY(0), CIRCLE_ENTRY(1),
	CIRCLE_ENTRY(1), CIRCLE_ENTRY(2), CIRCLE_ENTRY(3), CIRCLE_ENTRY(4),
	CIRCLE_ENTRY(5), CIRCLE_ENTRY(0), CIRCLE_ENTRY(0), CIRCLE_ENTRY(1),
	CIRCLE_ENTRY(2), CIRCLE_ENTRY(3), CIRCLE_ENTRY(4), CIRCLE_ENTRY(5),
};

#undef CIRCLE_ENTRY
#undef TELEPORT_CIRCLE_FIRST_ENTRY
