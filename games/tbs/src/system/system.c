/// @file
/// The System object: what every other part of the game stands on. Power-up
/// and the interrupt handlers, the frame wait and VBlank, input, the register
/// write queue, fades, the sprite lists and VRAM cache, tasks,
/// maths, debug text, the heap, the 3D transform, saving and the link cable.
///
/// system_internal.h holds what every part shares (its includes); the code
/// is in the files under system/, included here in the order it has in the
/// ROM. Camelot's own files are not recoverable: the ROM shows one object, and
/// these parts are ours, cut where one family ends and the next begins.
/// README.md in this folder gives the words and prefixes the parts use. Where
/// to start:
///
///   Power-up        start_up.c: System_Initialize
///   Frames          frame.c: wait(), System_VBlankHandler,
///                   Irq_SetHandler; task.c:
///                   Task_Add
///   Screen          screen_fade.c: ScreenFade_Start; oam.c: Oam_BuildBuffer;
///                   vram_cache.c: VramCache_Load
///   Memory          heap.c: HeapSlot_Alloc, Heap_Alloc
///   3D              transform.c: Transform_ApplyRotationTranslation,
///                   Transform_ProjectPoint
///   Saving          save.c: Save_WritePart,
///                   Save_ReadPart
///   Link cable      serial.c: Serial_Initialize,
///                   Serial_ExchangePayloads

#include "system_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "system/start_up.c"
#include "system/frame.c"
#include "system/io_write_queue.c"
#include "system/screen_fade.c"
#include "system/oam.c"
#include "system/vram_cache.c"
#include "system/task.c"
#include "system/math.c"
#include "system/debug_text.c"
#include "system/heap.c"
#include "system/transform.c"
#include "system/decompress.c"
#include "system/save.c"
#include "system/serial.c"
