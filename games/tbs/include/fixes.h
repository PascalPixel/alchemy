#ifndef ALCHEMY_FIXES_H
#define ALCHEMY_FIXES_H

// The fixes: each repairs a bug or undefined behaviour of the original, is off in every edition,
// and is turned on by BUGFIX or UBFIX (config.h). They do not depend on the edition, so a header
// may include this alone.

#include "config.h"

// UBFIX gives undefined behaviour its defined form.
#ifdef UBFIX
#	define FIX_BOREAS_POINTS_OVERRUN         1
#	define FIX_DICE_DIVIDE_BY_ZERO           1
#	define FIX_DICE_MATCH_LIST_OVERFLOW      1
#	define FIX_FUNCTION_SIGNATURES           1
#	define FIX_NULL_DEREFERENCE              1
#	define FIX_PILLAR_SORT_SCRATCH_OVERRUN   1
#	define FIX_POINTER_OUTSIDE_OBJECT        1
#	define FIX_RETURNED_STACK_BUFFER         1
#	define FIX_SHIP_DECK_CONST_TABLE_WRITTEN 1
#	define FIX_SIGNED_OVERFLOW               1
#	define FIX_SPEAKER_SPRITE_PAST_TABLE     1
#	define FIX_TYPE_PUNNING                  1
#	define FIX_UNDEFINED_SHIFT               1
#	define FIX_UNINITIALIZED_READ            1
#	define FIX_UNSEQUENCED_SIDE_EFFECT       1
#endif

// BUGFIX repairs the original's bugs.
#ifdef BUGFIX
#	define FIX_ABILITY_ID_BOUND                   1
#	define FIX_ASURA_SWORD_LOW_DRAW_HEIGHT        1
#	define FIX_CREDITS_LINE_LEAK                  1
#	define FIX_DEBUG_ENTRANCE_JUMP_WRAP           1
#	define FIX_DONPA_ATTENDANT_GRATEFUL_SPEAKER   1
#	define FIX_DROP_PICKER_CANNOT_REMOVE_MESSAGE  1
#	define FIX_EMPTY_WINDOW_SPAN_FILL             1
#	define FIX_ENEMY_ID_BOUND                     1
#	define FIX_FACING_PASSED_AS_ACTOR_ID          1
#	define FIX_FALL_WAIT_NULL_ACTOR               1
#	define FIX_FIRST_TREASURE_SPOT_IN_ACTOR_LOOPS 1
#	define FIX_FROST_CHIP_DRIFT                   1
#	define FIX_FUSION_WAITS_FOR_BOTH              1
#	define FIX_GATE_GUARDS_ACTOR_IDS              1
#	define FIX_HAMAS_ROOM_FEIZHI_BOUNCE           1
#	define FIX_HAND_CURSOR_FREE_BLOCK_TEST        1
#	define FIX_HUT_OWNER_HIDDEN_BY_DAY            1
#	define FIX_KEPT_MESSAGE_FLAG_CLEAR            1
#	define FIX_LUNPA_TRAVELING_HEALER_SANCTUM     1
#	define FIX_MAP_MARKER_TILE_ALLOCATION         1
#	define FIX_MISSING_IVY_SCALE                  1
#	define FIX_PAIR_JOINED_DIVIDER                1
#	define FIX_PROCNE_SPARK_DEPTH                 1
#	define FIX_RECT_BOTTOM_CLIP                   1
#	define FIX_REQUEST_SUM_CANCELS                1
#	define FIX_RESTORE_COUNTS_STUN                1
#	define FIX_SANDSTORM_LEADER_SHADOW            1
#	define FIX_SAND_PUFF_ROW                      1
#	define FIX_SHIP_CAMERA_WAIT_MASK              1
#	define FIX_SLEEP_SHEEP_BOUNCE                 1
#	define FIX_TASK_SORT_LAST_PASS                1
#	define FIX_TEXT_TILE_SCAN_ROW_STRIDE          1
#	define FIX_TITLE_INTRO_STATE_ALLOCATION       1
#	define FIX_UNOPENED_WINDOW_REDRAW             1
#	define FIX_WANDER_AHEAD_PROBE                 1
#	define SET_DJINNI_WINDOW_FITS_SCREEN          1
#endif

// The fixes, each off unless BUGFIX or UBFIX turns it on above.

// gmAbilityGetDirect sends ability id ABILITY_COUNT, one past the end of the ability table, to row
// 0 like every other id past the table. The original lets that id through and reads the first bytes
// of the enemy table, which follows it, as an ability.
#ifndef FIX_ABILITY_ID_BOUND
#	define FIX_ABILITY_ID_BOUND 0
#endif

// Asura's swordsman with his sword low is drawn 96 rows tall, the low picture's own height, instead
// of the swung picture's 104. The draw no longer reads 8 rows into the swung picture, so the raised
// sword's tip stops showing under his feet.
#ifndef FIX_ASURA_SWORD_LOW_DRAW_HEIGHT
#	define FIX_ASURA_SWORD_LOW_DRAW_HEIGHT 0
#endif

// Boreas's unused copy of its point table copies the table's 16 rows instead of 32, so it no longer
// reads the 32 bytes after BoreasPoints. Nothing reads the copy, so nothing on screen changes.
#ifndef FIX_BOREAS_POINTS_OVERRUN
#	define FIX_BOREAS_POINTS_OVERRUN 0
#endif

// mnCreditsDrawLine frees the line buffer it has just allocated when it reaches the end-of-text
// marker. The original returns with the buffer still taken.
#ifndef FIX_CREDITS_LINE_LEAK
#	define FIX_CREDITS_LINE_LEAK 0
#endif

// In the debug scene selector, an entrance moved past either end by L or R (a jump of 10) wraps by
// the 100 entrances instead of by 99, so 95 + 10 gives 5, not 6. Left and Right, which step by one,
// already wrap correctly.
#ifndef FIX_DEBUG_ENTRANCE_JUMP_WRAP
#	define FIX_DEBUG_ENTRANCE_JUMP_WRAP 0
#endif

// In Lucky Dice (both games) and Super Lucky Dice (The Lost Age), two touching dice whose
// whole-pixel separation comes out as 0 get no push apart that tick, instead of dividing by that
// zero. That happens when they are less than a pixel apart on every axis, measured toward the other
// die.
#ifndef FIX_DICE_DIVIDE_BY_ZERO
#	define FIX_DICE_DIVIDE_BY_ZERO 0
#endif

// The unused list of matched numbers in dice scoring (Lucky Dice in both games, Super Lucky Dice in
// TLA) gets a third slot. Three alike make three pairs, and the original writes the third pair past
// its two-slot array.
#ifndef FIX_DICE_MATCH_LIST_OVERFLOW
#	define FIX_DICE_MATCH_LIST_OVERFLOW 0
#endif

// In Lunpa Fortress, once Dodonpa is locked up, the attendant's line "Donpa is very grateful for
// what you have done" opens beside her, as her other lines do. The ROM names Dodonpa's wife, who
// stands in the family room far across the fortress, as the speaker, so the window is pushed to the
// screen's lower left.
#ifndef FIX_DONPA_ATTENDANT_GRATEFUL_SPEAKER
#	define FIX_DONPA_ATTENDANT_GRATEFUL_SPEAKER 0
#endif

// The Broken Seal's drop picker, where the player chooses an item to drop because every pack is
// full, tests the discard status instead of the selection result, which is still zero at that
// point. An equipped cursed item now shows its "You can't remove it!" message along with the cancel
// sound; before, it got only the sound.
#ifndef FIX_DROP_PICKER_CANNOT_REMOVE_MESSAGE
#	define FIX_DROP_PICKER_CANNOT_REMOVE_MESSAGE 0
#endif

// With no window state, mnScanlineWindowFillSpans fills the screen transition's whole window-edge
// page: two halfwords per scan line, DISPLAY_HEIGHT * 4 bytes. The original fills only half of it,
// so the lower 80 lines keep stale window 1 edges.
#ifndef FIX_EMPTY_WINDOW_SPAN_FILL
#	define FIX_EMPTY_WINDOW_SPAN_FILL 0
#endif

// gmEnemyGetDefinition sends every row past the end of the enemy table to row 0, not only rows from
// ENEMY_ID_MAX on. Ids from the table's length (165 rows in TBS, 380 in TLA) up to ENEMY_ID_MAX no
// longer read the data after the table.
#ifndef FIX_ENEMY_ID_BOUND
#	define FIX_ENEMY_ID_BOUND 0
#endif

// Where a scene passes a heading (FACING_SOUTH, FACING_NORTH) to fdActorFaceActor in the slot for
// the actor to face, it calls fdActorFaceDirection instead, so the actor really turns: the leader
// south toward Altin's water beast, the opponent and the leader south after a Colosso match with
// Azart or Satrage, and Iodem south and then north in the Suhalla Desert. In the ROM nobody turns,
// because no actor has an id that large.
#ifndef FIX_FACING_PASSED_AS_ACTOR_ID
#	define FIX_FACING_PASSED_AS_ACTOR_ID 0
#endif

// After the leader drops through the hole in Venus Lighthouse's drifting floor, the wait is given
// the leader instead of a null pointer. It now ends when he lands; before, it always ran a full
// second.
#ifndef FIX_FALL_WAIT_NULL_ACTOR
#	define FIX_FALL_WAIT_NULL_ACTOR 0
#endif

// In The Broken Seal, loops over the actor table stop before ACTOR_ID_COUNT instead of at it.
// Before, fdActorLookup(ACTOR_ID_COUNT) returned the first treasure spot's actor, the slot just
// past the table. So the map's first chest or Psynergy Stone was paused with the other actors,
// could become the talk target or the Psynergy target, and was looked at by the Colosso friend
// search.
#ifndef FIX_FIRST_TREASURE_SPOT_IN_ACTOR_LOOPS
#	define FIX_FIRST_TREASURE_SPOT_IN_ACTOR_LOOPS 0
#endif

// Frost's ice chips get a sideways velocity (velocityX) of 0 when they burst, so they fall
// straight. The original never sets it, so each chip drifts sideways by whatever word its PARTICLES
// slot last held: the field's map grid cells, or an earlier effect's particles.
#ifndef FIX_FROST_CHIP_DRIFT
#	define FIX_FROST_CHIP_DRIFT 0
#endif

// Every function is declared before it is called, and its definition, prototypes, calls and stored
// pointers agree on one type. Prototypes are added. A function that falls off its end returns a
// value. A function whose result nobody reads is made void, or given a return value where it stays
// s32. Missing arguments are passed. Calls and stores through casts to other function types are
// dropped.
#ifndef FIX_FUNCTION_SIGNATURES
#	define FIX_FUNCTION_SIGNATURES 0
#endif

// In Venus Lighthouse's aerie, Saturos and Menardi's fusion starts only once both have arrived. The
// original overwrites Saturos's arrival test with Menardi's, so the fusion starts as soon as
// Menardi alone arrives.
#ifndef FIX_FUSION_WAITS_FOR_BOTH
#	define FIX_FUSION_WAITS_FOR_BOTH 0
#endif

// When Isaac is thrown out of Lunpa Fortress, the turns to south-southeast and south-southwest go
// to the gate's own guards (ACTOR_LEFT_GUARD, ACTOR_RIGHT_GUARD) instead of the town guards' ids,
// which no actor at the gate holds. Nothing visible changes: GateActorPlacements already places the
// two guards facing those headings.
#ifndef FIX_GATE_GUARDS_ACTOR_IDS
#	define FIX_GATE_GUARDS_ACTOR_IDS 0
#endif

// In Hama's room at Lama Temple, Feizhi bounces before she says gems fell from the sky, as she does
// at her other lines in that scene. The ROM passes ACTOR_FEIZHI, her id inside Fuchin Temple, which
// no actor in Hama's room holds, so she doesn't move.
#ifndef FIX_HAMAS_ROOM_FEIZHI_BOUNCE
#	define FIX_HAMAS_ROOM_FEIZHI_BOUNCE 0
#endif

// When the menu makes its hand cursor, it takes VRAM_CACHE_ENTRY_COUNT (96), the value
// syVramCacheFindFreeEntry returns when no VRAM cache block is free, as "none free" instead of
// block 0. In the ROM no hand is made when block 0 is the free one, and when none is free the
// sprite is made on block 96, which syVramCacheLoad refuses.
#ifndef FIX_HAND_CURSOR_FREE_BLOCK_TEST
#	define FIX_HAND_CURSOR_FREE_BLOCK_TEST 0
#endif

// Vale's plaza moves the storm's Jenna and strongman away only on the stormy night. The original
// tests only the sent-to-aid-the-elders flag, which stays set, and by day the storm's Jenna has the
// hut owner's actor number. So from the end of the prologue until Kalay the hut owner is moved out
// of sight, and neither of his talks plays: the food stored for winter before the party leaves, and
// the fallen stone after.
#ifndef FIX_HUT_OWNER_HIDDEN_BY_DAY
#	define FIX_HUT_OWNER_HIDDEN_BY_DAY 0
#endif

// While a message is kept in the info window, a row move in the Psynergy list clears the
// kept-message flag instead of FLAG_SCENE_TEMP_FF. Psynergy descriptions then come back on the next
// row move. In the original they stay away until another screen clears the flag.
#ifndef FIX_KEPT_MESSAGE_FLAG_CLEAR
#	define FIX_KEPT_MESSAGE_FLAG_CLEAR 0
#endif

// In Lunpa, in the house beside Suhalla Gate, the traveling Healer's Sanctum is run with his own
// actor instead of the temple Healer's. Nothing visible changes: the Sanctum takes only its
// keeper's portrait from the actor, and both Healers use the same sprite.
#ifndef FIX_LUNPA_TRAVELING_HEALER_SANCTUM
#	define FIX_LUNPA_TRAVELING_HEALER_SANCTUM 0
#endif

// The map screen reserves the full 128 bytes that its four marker tiles fill, write and upload,
// instead of 32 (the size worked out as if the heap counted words), so the tiles no longer run 96
// bytes past their block. The same repair in both games.
#ifndef FIX_MAP_MARKER_TILE_ALLOCATION
#	define FIX_MAP_MARKER_TILE_ALLOCATION 0
#endif

// Crossbone Isle's ivy growth no longer scales a fourth ivy that is never placed; the original
// writes through the empty slot to address 0x18, in the BIOS, where the write is lost.
#ifndef FIX_MISSING_IVY_SCALE
#	define FIX_MISSING_IVY_SCALE 0
#endif

// Code that reads or writes through a NULL actor, sprite, window, sprite-list head or placement
// checks for NULL first, returns early, or drops the store or call. The cases are empty actor
// slots, actors a scene never places, a sprite that failed to create, a camera with no leader, a
// window that was never opened, and the actor placement search when the last table is absent.
#ifndef FIX_NULL_DEREFERENCE
#	define FIX_NULL_DEREFERENCE 0
#endif

// The window that announces two members joining draws its divider down column 8, just before its
// text at x 68. The ROM asks for a line from (8,0) to (4,4), which is neither across nor down, so
// mnWindowDrawDividerLine draws nothing.
#ifndef FIX_PAIR_JOINED_DIVIDER
#	define FIX_PAIR_JOINED_DIVIDER 0
#endif

// Crossbone Isle's stepping-stone pillar sort keeps a pillar's saved placement in a whole 16-byte
// PILLAR_PLACEMENT. The original uses a 12-byte s32[3], which the 16-byte DMA copy overruns on the
// stack.
#ifndef FIX_PILLAR_SORT_SCRATCH_OVERRUN
#	define FIX_PILLAR_SORT_SCRATCH_OVERRUN 0
#endif

// Indexes and pointer walks stay inside the object they start in, and the same bytes are read and
// written. Separate members read as an array get a real array in a union. One-element arrays,
// tables and heap slots are no longer indexed past; the neighbouring object is named or reached
// through its own slot. Pointers are no longer formed before an array's start. Reads and writes
// that must still cross a struct's members go through memcpy on the enclosing object.
#ifndef FIX_POINTER_OUTSIDE_OBJECT
#	define FIX_POINTER_OUTSIDE_OBJECT 0
#endif

// Procne's strike sparks keep their up-and-down speed and move in depth by their own random depth
// speed, kept in velocityZ. The original writes the depth speed over velocityY, so the up-and-down
// speed is lost and each spark's height and depth move together.
#ifndef FIX_PROCNE_SPARK_DEPTH
#	define FIX_PROCNE_SPARK_DEPTH 0
#endif

// The window tilemap rectangle routines clip a rectangle's bottom once it passes the screen's 20
// tile rows. These are the highlight, palette, interior-palette, fill and text-tile routines. The
// original tested the bottom against the 30-tile width, so a rectangle ending in rows 20 to 28 was
// kept whole and written past the end of the windows' 32 by 20 tilemap.
#ifndef FIX_RECT_BOTTOM_CLIP
#	define FIX_RECT_BOTTOM_CLIP 0
#endif

// fdCountRequests ORs the field's pending requests instead of adding them. Requests of opposite
// sign, such as a party member falling to poison (-1) and step trigger 1, no longer sum to 0 and
// read as "nothing pending". In the original both wait until another request arrives, and by then a
// later cell may have replaced the step trigger.
#ifndef FIX_REQUEST_SUM_CANCELS
#	define FIX_REQUEST_SUM_CANCELS 0
#endif

// When the enemy AI picks an ally to Restore, it counts the ally's stun, which Restore cures,
// instead of confusion and charm, which it does not. An ally who is only stunned is no longer
// passed over, and one who is only confused or charmed is no longer picked. Tonic's count includes
// Restore's and changes the same way.
#ifndef FIX_RESTORE_COUNTS_STUN
#	define FIX_RESTORE_COUNTS_STUN 0
#endif

// btTextCopyFourCharsUnderscored builds its four-character copy in a static buffer, so the pointer
// it returns into that copy stays valid after the function returns. Before, it pointed into a stack
// array that no longer exists.
#ifndef FIX_RETURNED_STACK_BUFFER
#	define FIX_RETURNED_STACK_BUFFER 0
#endif

// As a sandstorm carries the party away in the Suhalla Desert, the leader's sprite flags are
// cleared through his actor, which hides his shadow. The ROM passes his actor id (Isaac's, 0) where
// the helper takes an actor, so it gets a null pointer, does nothing, and the shadow stays drawn.
#ifndef FIX_SANDSTORM_LEADER_SHADOW
#	define FIX_SANDSTORM_LEADER_SHADOW 0
#endif

// At the head of a pouring sand slide in Venus Lighthouse, the second kind of sand puff takes its
// row from z instead of x. It now starts just north of the other puffs instead of far to the south
// of the passage. Only the row changes; the across position is the same.
#ifndef FIX_SAND_PUFF_ROW
#	define FIX_SAND_PUFF_ROW 0
#endif

// The ship scenes' camera helper waits for the ticks in the flags' wait field
// (SHIP_CAMERA_WAIT_TICKS, 0xffff) instead of masking them with 0x1111, which kept only bits 0, 4,
// 8 and 12. The ROM's mask turned the callers' 20-tick waits into 16 and their 40-tick wait into
// none. SHIP_CAMERA_JUMP sits in bit 0, so it still adds one tick.
#ifndef FIX_SHIP_CAMERA_WAIT_MASK
#	define FIX_SHIP_CAMERA_WAIT_MASK 0
#endif

// TolbiShipDeckActorPlacementsWaiting, the Tolbi ship deck's actor placements while it waits at
// Kalay, is declared and defined without const. scGetActorPlacements writes talkFacing into up to
// three of its Kalay rows through STORE_CONST, and without the flag a MODERN build writes into a
// const object.
#ifndef FIX_SHIP_DECK_CONST_TABLE_WRITTEN
#	define FIX_SHIP_DECK_CONST_TABLE_WRITTEN 0
#endif

// Arithmetic that can overflow a signed int is done unsigned. One case is negating a negative
// number in the message decoder's signed-number argument: the magnitude becomes u32, 0U -
// (u32)value. The other is the file list glow's sine phase, FrameCount times 768, which overflows
// after about 13 hours: it is done in u32 and masked to an angle.
#ifndef FIX_SIGNED_OVERFLOW
#	define FIX_SIGNED_OVERFLOW 0
#endif

// Sleep's sheep bounce off the ground, tumbling faster, whenever they are below it and falling
// (speed above 0). The original bounces them only at a speed of exactly 0, which never happens just
// after gravity is added, so they fall through the ground.
#ifndef FIX_SLEEP_SHEEP_BOUNCE
#	define FIX_SLEEP_SHEEP_BOUNCE 0
#endif

// In The Broken Seal, fdActorGetSpriteById returns -1 (no sprite) for a speaker id whose low 12
// bits fall past the 66-slot actor table. That includes the -1 'no speaker' key, which masks to
// 4095. Before, it read a word far beyond the field state as an actor pointer.
#ifndef FIX_SPEAKER_SPRITE_PAST_TABLE
#	define FIX_SPEAKER_SPRITE_PAST_TABLE 0
#endif

// syTaskSort's bubble sort runs its last pass (down to pass > 0, not > 1), so the tasks in slots 0
// and 1 also end in priority order, highest first.
#ifndef FIX_TASK_SORT_LAST_PASS
#	define FIX_TASK_SORT_LAST_PASS 0
#endif

// The scan that marks which text tiles the screen shows (mnWindowMarkVisibleTextTiles) skips the
// two hidden columns at the end of each 32-entry tilemap row, so it covers exactly the visible 30
// by 20 area. The original never reaches the 36 visible tiles at the bottom (row 19 and the last
// six of row 18), so their text tiles are freed while still on screen.
#ifndef FIX_TEXT_TILE_SCAN_ROW_STRIDE
#	define FIX_TEXT_TILE_SCAN_ROW_STRIDE 0
#endif

// The title intro reserves its whole state (IntroState, 984 bytes) in its heap slot instead of only
// the part for the prompt's sprites (224 bytes). Its OAM list, written every VBlank, ran past that
// block into the memory after it.
#ifndef FIX_TITLE_INTRO_STATE_ALLOCATION
#	define FIX_TITLE_INTRO_STATE_ALLOCATION 0
#endif

// Objects are read and written through their own declared types, not through casts to other integer
// widths, to words standing for pointers, or to types that drop volatile. Halfword pairs are built
// with shifts or byte stores, and a 16.16 value's whole part is taken with a shift. Sprite and OAM
// words are written through their fields or copied in with memcpy. Pointers and function pointers
// are read and stored as pointers, and TickCount is read as declared.
#ifndef FIX_TYPE_PUNNING
#	define FIX_TYPE_PUNNING 0
#endif

// Shifts that C leaves undefined are rewritten to give the result the ARM gives. Left shifts of
// negative or overflowing signed values become multiplications, unsigned shifts or the equivalent
// defined facing comparison (FACING_IS_NORTH). Shifts by 32 bits or more are skipped where the
// count can reach the word's width: the status bar's empty last tile, and an empty enemy name's
// letter bit.
#ifndef FIX_UNDEFINED_SHIFT
#	define FIX_UNDEFINED_SHIFT 0
#endif

// Locals, parameters and stack words that are read before anything sets them are given a value or
// no longer read. The cases are camera zoom, the star base point, a frozen actor's arrival flag,
// the SC_MOVE_LOCAL offset, the encounter zone entry, the summon max-HP bonus, the speaker message
// position, the map cursor and marker-0 name, the overwrite result, the line spacing count, the
// VRAM cache entry freed, the never-created details window and the knock-back state pointer.
#ifndef FIX_UNINITIALIZED_READ
#	define FIX_UNINITIALIZED_READ 0
#endif

// The Djinn menu's preview no longer redraws unopenedWindow, a window it never makes, which the
// original redraws through a null pointer.
#ifndef FIX_UNOPENED_WINDOW_REDRAW
#	define FIX_UNOPENED_WINDOW_REDRAW 0
#endif

// An expression that changes a value and also reads or changes it again with no sequence point
// between is split into ordered statements that keep the shipped result. The cases: a picture's
// width, height or size assigned in one draw-call argument and read in another; n = ++n % count; a
// kind byte read through *code++ in the call target while code is also passed; a page arrow's row
// written twice in one expression.
#ifndef FIX_UNSEQUENCED_SIDE_EFFECT
#	define FIX_UNSEQUENCED_SIDE_EFFECT 0
#endif

// SC_WANDER also tests the ground probe it places 8 pixels straight past a wandering actor's chosen
// point, as SC_WANDER_HOME does, so the actor no longer picks a point just short of a wall. The ROM
// places that probe but tests only the two at 45 degrees.
#ifndef FIX_WANDER_AHEAD_PROBE
#	define FIX_WANDER_AHEAD_PROBE 0
#endif

#endif
