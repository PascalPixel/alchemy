/// @file
/// The runtime object: what every other part of the game stands on. Power-up
/// and the interrupt handlers, the frame wait and VBlank, input, the register
/// write queue, fades, the sprite lists and VRAM cache, tasks,
/// maths, debug text, the heap, the 3D transform, saving and the link cable.
///
/// runtime_internal.h holds what every part shares (its includes); the code
/// is in the files under runtime/, included here in the order it has in the
/// ROM. Camelot's own files are not recoverable: the ROM shows one object, and
/// these parts are ours, cut where one family ends and the next begins.
/// README.md in this folder gives the words and prefixes the parts use. Where
/// to start:
///
///   Power-up        system.c: System_Initialize
///   Frames          frame.c: WaitTicks, System_VBlankHandler,
///                   Irq_SetHandler; task.c:
///                   Task_Add
///   Screen          fade.c: ScreenFade_Start; render.c: Oam_BuildBuffer;
///                   vram_cache.c: VramCache_Load
///   Memory          heap.c: AllocHeapSlot, Alloc
///   3D              transform.c: Transform_ApplyRotationTranslation,
///                   Transform_ProjectPoint
///   Saving          save.c: Save_WriteRecord,
///                   Save_ReadRecord
///   Link cable      serial.c: Serial_Initialize,
///                   Serial_ExchangePayloads

#include "runtime_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "runtime/system.c"
#include "runtime/frame.c"
#include "runtime/io_queue.c"
#include "runtime/fade.c"
#include "runtime/render.c"
#include "runtime/vram_cache.c"
#include "runtime/task.c"
#include "runtime/math.c"
#include "runtime/text.c"
#include "runtime/heap.c"
#include "runtime/transform.c"
#include "runtime/decompress.c"
#include "runtime/save.c"
#include "runtime/serial.c"
