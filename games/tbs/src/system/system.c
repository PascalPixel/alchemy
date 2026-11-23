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
/// - power-up: System_Initialize (system_start_up.c);
/// - frames: wait(), System_VBlankHandler and Irq_SetHandler
///   (system_frame.c), Task_Add (system_task.c);
/// - the screen: ScreenFade_Start (system_screen_fade.c), Oam_BuildBuffer
///   (system_oam.c), VramCache_Load (system_vram_cache.c);
/// - memory: HeapSlot_Alloc and Heap_Alloc (system_heap.c);
/// - 3D: Matrix_RotateTranslate and Matrix_ProjectPoint (system_matrix.c);
/// - saving: Save_WritePart and Save_ReadPart (system_save.c);
/// - the link cable: Serial_Initialize and Serial_ExchangePayloads
///   (system_serial.c).

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
#include "system_matrix.c"
#include "system_decompress.c"
#include "system_save.c"
#include "system_serial.c"
