/// @file
/// The System object: what every other part of the game stands on. Power-up
/// and the interrupt handlers, the frame wait and VBlank, input, the register
/// write queue, fades, the sprite lists and VRAM cache, tasks,
/// maths, debug text, the heap, the 3D transform, saving and the link cable.
///
/// system_internal.h holds what every part shares (its includes); the code
/// is in the files under system/, included here in link order, one family to a
/// part.
/// README.md in this folder gives the words and prefixes the parts use. Where
/// to start:
///
///   Power-up        system_start_up.c: System_Initialize
///   Frames          system_frame.c: wait(), System_VBlankHandler,
///                   Irq_SetHandler; system_task.c:
///                   Task_Add
///   Screen          system_screen_fade.c: ScreenFade_Start; system_oam.c:
///   Oam_BuildBuffer;
///                   system_vram_cache.c: VramCache_Load
///   Memory          system_heap.c: HeapSlot_Alloc, Heap_Alloc
///   3D              system_transform.c: Transform_ApplyRotationTranslation,
///                   Transform_ProjectPoint
///   Saving          system_save.c: Save_WritePart,
///                   Save_ReadPart
///   Link cable      system_serial.c: Serial_Initialize,
///                   Serial_ExchangePayloads

#include "system_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "system_start_up.c"
#include "system_frame.c"
#include "system_io_write_queue.c"
#include "system_screen_fade.c"
#include "system_oam.c"
#include "system_vram_cache.c"
#include "system_task.c"
#include "system_math.c"
#include "system_debug_text.c"
#include "system_heap.c"
#include "system_transform.c"
#include "system_decompress.c"
#include "system_save.c"
#include "system_serial.c"
