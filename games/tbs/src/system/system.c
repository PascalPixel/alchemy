/* The system library's object: what every other part of the game stands on.
   Power-up and the interrupt handlers, the tick wait and VBlank, input, the
   register write queue, fades, the sprite lists and VRAM cache, events, maths,
   debug text, the heap, the 3D transform, saving and the link cable.

   system_internal.h holds what every part shares (its includes); the code is in
   the files under system/, included here in link order, one group of functions
   to a part. README.md in this folder gives the words and prefixes the parts
   use. Where to start:

   - power-up: syInit (system_start_up.c);
   - ticks: sySynch(), syVBlankHandler and syIntFunction
	 (system_frame.c), sySetEvent (system_event.c);
   - the screen: syScreenFadeStart (system_screen_fade.c), syOAMBuildBuffer
	 (system_oam.c), syVRAMCacheLoad (system_vram_cache.c);
   - memory: syHeapSlotAlloc and syHeapAlloc (system_heap.c);
   - 3D: syMatrixRotateTranslate and syConvert3Dto2D (system_matrix.c);
   - saving: syBupWritePart and syBupReadPart (system_bup.c);
   - the link cable: sySerialInit and sySerialExchangePayloads
	 (system_serial.c). */

#include "system_internal.h"

/* The parts, in ROM order (src/README.md, "Objects and parts"). */
#include "system_start_up.c"
#include "system_frame.c"
#include "system_io_write_queue.c"
#include "system_screen_fade.c"
#include "system_oam.c"
#include "system_vram_cache.c"
#include "system_event.c"
#include "system_math.c"
#include "system_debug_text.c"
#include "system_heap.c"
#include "system_matrix.c"
#include "system_decompress.c"
#include "system_bup.c"
#include "system_serial.c"
