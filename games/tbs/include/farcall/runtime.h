/// @file
/// Tasks, frames, memory, resources and core runtime services.

/// Runs the game's frame loop for `frames` frames: the tasks, the OAM
/// list, the idle sleep and the debug pause, then VBlank and the keys.
void WaitFramesFar(s32 frames);
/// Adds a task running `func` at priority `order`, or gives the task
/// already running it the new priority. Returns the task's index, or -1 when
/// all 20 slots are taken.
s32 Task_AddFar(TaskFunc func, s32 order);
/// Removes the task running `func` and returns its old index, or -1.
s32 Task_RemoveFar(TaskFunc func);
/// Advances the random number generator and returns 16 bits of the new seed,
/// 0 to 0xffff.
u32 Random16Far(void);
/// Returns the angle of (x, y) in 1/65536 turns, 0 along +y and 0x4000 along
/// +x; (0, 0) gives 0.
s32 ArcTan2Far(s32 x, s32 y);
/// Returns the square root of a 16.16 value in 16.16, whose low eight bits are
/// always 0.
s32 FixedSqrtFar(s32 value);
/// Moves the x, y, z point at `position` by `radius` towards `angle` on the
/// ground plane, changing only x and z.
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, void *position);
/// Installs `handler` for interrupt `irq` and enables it, or disables it for a
/// null handler; `vcount` is the line a VCount interrupt fires on.
void Irq_SetHandlerFar(u32 irq, s32 vcount, InterruptHandler handler);
/// Returns heap slot `kind`'s block, first allocating `size` bytes for it from
/// IWRAM when they fit and EWRAM otherwise. Returns NULL when neither has room.
void *AllocHeapSlotFar(s32 kind, s32 size);
/// Returns heap slot `kind`'s block like AllocHeapSlotFar, but
/// tries EWRAM first, for blocks that need room more than speed.
void *AllocHeapSlotEwramFar(s32 kind, s32 size);
/// Allocates an uncleared short-lived buffer of `size` bytes, from IWRAM when
/// it fits (Alloc); Free frees it and everything allocated after it.
void *AllocFar(s32 size);
/// Takes a short-lived buffer as Alloc does, but from EWRAM first, for
/// buffers too large to want IWRAM.
void *AllocEwramFar(s32 size);
/// Unpacks a byte-oriented LZ resource with a RAM copy of its ARM decoder, and
/// returns the unpacked size in bytes.
s32 Decompress_ByteLzFar(const void *source, void *destination);
/// Unpacks a tagged LZ resource (a first byte of 0 or 1) the same way and
/// returns the unpacked size in bytes.
s32 Decompress_TaggedLzFar(const void *source, void *destination);
/// Frees VRAM cache entry `no`, its sprite VRAM included, once its picture is
/// no longer shown.
s32 VramCache_FreeEntryFar(u32 no);
/// Gives back entry `no`'s sprite VRAM but keeps the entry taken, so a later
/// VramCache_LoadFar can refill it.
s32 VramCache_ReleaseVramFar(u32 no);
/// Gives cache entry `no` `size` bytes of sprite VRAM and copies `src` there.
/// Returns the first tile number, or 0 when there was no room.
s32 VramCache_LoadFar(u32 no, u32 size, const void *src);
/// Returns the first free VRAM cache entry, or VRAM_CACHE_ENTRY_COUNT when all
/// are taken.
s32 VramCache_FindFreeEntryFar(void);
/// Fills the next of the 32 OAM affine matrices with `source`'s scale and
/// rotation and returns its number. When none is left it returns 0 unfilled.
s32 OamMatrix_AllocFromTransformFar(struct AffineTransform *source);
/// Links a sprite entry into depth slot `slot` (0 to 255) for this frame's OAM
/// list; the entry's first word becomes its link.
void Oam_AddSpriteAtDepthFar(void *slot_entry, s32 slot);
/// Loads the window font, border tiles and palette, and the basic sprite
/// palettes, as modes that replace them must put them back.
void Window_LoadGraphicsFar(void);
/// Returns the ROM address of resource `index` from the resource directory.
void *Resource_GetTableEntryFar(s32 index);
/// Does nothing. The Title, Lucky Dice and Lucky Wheels pass it a
/// far-call resource id.
void Stub_NoOpHookFar(s32 unused);
/// Fades BG1 to BG3, the sprites and the backdrop out to black over `vblanks`
/// VBlanks; it only starts the screen fade.
void ScreenFade_ToBlackFar(s32 vblanks);
/// Waits a frame at a time until the running screen fade finishes.
void ScreenFade_WaitFar(void);
/// Puts the serial port in multiplayer mode, installs the link interrupts and
/// starts the per-frame exchange with a link partner.
void Serial_InitializeFar(void);
/// Looks for the link partner and, once connected, does one exchange of
/// `send` and `receive`. Returns the link status.
s32 Serial_PollAndTransferFar(void *send, void *receive);
/// Stops the per-frame link exchange and removes the link interrupt handlers.
void Serial_RemoveIrqHandlersFar(void);
/// Copies reverb code `code`'s mixer code (enum SoundReverbCode, anything
/// else SOUND_REVERB_MONO) into IWRAM. Always returns 0.
s32 Sound_LoadReverbCodeFar(s32 code);
/// Starts sending `transferValue` bytes from `source` to the partner in 20-byte
/// blocks; the size must be a multiple of 20. Returns -1 when a send is
/// running.
s32 Serial_StartSendFar(void *source, s32 transferValue);
/// Starts storing the partner's blocks in `destination`, which needs room for
/// the whole send. Returns -1 when a receive is running.
s32 Serial_StartReceiveFar(void *destination);
/// Returns bit 0 while a send runs and bit 1 while a receive runs.
s32 Serial_GetActiveTransfersFar(void);
/// Stores `mask` as the flags of the task running `func`; bit 0 keeps it
/// running while the overlay tasks are suspended. Returns its index, or -1.
s32 Task_SetFlagsFar(TaskFunc func, u32 mask);
