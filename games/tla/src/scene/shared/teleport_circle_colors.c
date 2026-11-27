// The colours of a Teleport circle and of its shimmer (shared/teleport_shimmer.c), which every
// scene with a circle keeps a copy of: seven palette entries, each a red, green and blue, ended by
// PALETTE_LIST_END.

// The circle's colours: one pale grey-green on every entry.
Uint16 TeleportCircleColors[] = {
    23, 25, 22, 23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23, 25, 22, PALETTE_LIST_END,
};

// The shimmer's colours, which it moves round the entries: lilac, blues deepening to violet, and
// white.
Uint16 TeleportCircleShimmerColors[] = {
    30, 27, 31, 22, 21, 31, 20, 18, 31, 13, 11,
    27, 11, 8,  24, 8,  5,  21, 31, 31, 31, PALETTE_LIST_END,
};
