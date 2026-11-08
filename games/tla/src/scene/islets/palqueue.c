/// @file
/// Overlay 674 keeps its own palette queue between its allocation and step
/// functions; its PaletteQueue_Add is the main image's.
#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "palette_snapshots.h"

#define gPaletteWork ((struct PaletteSnapshotWork *)Ram_HeapSlots->unk70[0])

#include "../../field/palqueue_add.c"
