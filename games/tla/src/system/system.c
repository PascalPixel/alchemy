/// @file
/// The System object: power-up and the interrupt handlers, the frame wait and
/// VBlank, input, the register write queue, fades, the sprite lists and VRAM
/// cache, tasks, maths, debug text, the heap, the 3D transform, decoding,
/// saving, the link cable and the game flags.
///
/// system_internal.h holds what every part shares (its includes); the code
/// is in the files under system/, included here in the order it has in the
/// ROM, cut where The Broken Seal's parts are. Where to start:
/// - power-up: System_Initialize (system_start_up.c);
/// - frames: wait() and the VBlank handler (system_frame.c), tasks
///   (system_task.c);
/// - the screen: fades (system_screen_fade.c), sprite depth slots
///   (system_oam.c), sprite VRAM (system_vram_cache.c);
/// - memory: the heap (system_heap.c);
/// - 3D: Matrix_RotateTranslate and Matrix_ProjectPoint (system_matrix.c);
/// - saving: Save_WritePart and Save_ReadPart (system_save.c);
/// - the link cable: Serial_Initialize (system_serial.c).

#include "system_internal.h"
#include "graphics/output.h"

// The parts, in ROM order.
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
