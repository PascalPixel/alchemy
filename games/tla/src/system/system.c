/// @file
/// The System object: power-up and the interrupt handlers, the frame wait and
/// VBlank, input, the register write queue, fades, the sprite lists and VRAM
/// cache, tasks, maths, debug text, the heap, the 3D transform, decoding,
/// saving, the link cable and the game flags.
///
/// system_internal.h holds what every part shares (its includes); the code
/// is in the files under system/, included here in the order it has in the
/// ROM. Camelot's own files are not recoverable: the ROM shows one object, and
/// these parts are ours, cut where The Broken Seal's are. Where to start:
///
///   Power-up        start_up.c: System_Initialize
///   Frames          frame.c: wait(), System_VBlankHandler (the VBlank
///   handler),
///                   Irq_SetHandler
///                   task.c: Task_Add
///   Screen          screen_fade.c: ScreenFade_Start; oam.c:
///   Oam_AddSpriteAtDepth;
///                   vram_cache.c: VramCache_Load
///   Memory          heap.c: HeapSlot_Alloc, Heap_Alloc
///   3D              transform.c: Matrix_RotateTranslate,
///                   Render_ProjectPoint
///   Saving          save.c: Save_ReadPart, Save_InvalidateSector
///   Link cable      serial.c: Serial_Initialize

#include "system_internal.h"
#include "graphics/output.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
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
#include "system_matrix.c"
#include "system_decompress.c"
#include "system_save.c"
#include "system_serial.c"
#include "system_flags.c"
