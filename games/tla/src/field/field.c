/// @file
/// The field object, the field bank's C as The Broken Seal's field.c begins:
/// resource metadata and LZ decompression, then composing sprite pictures and
/// placing projected sprites. field_internal.h holds what the parts share; the
/// parts under field/ are included here in the order their code has in the
/// ROM. Camelot's own files are not recoverable: the ROM shows one object, and
/// these parts are ours, cut where The Broken Seal's are.

#include "field_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "field/decompress.c"
#include "field/sprite_draw.c"
