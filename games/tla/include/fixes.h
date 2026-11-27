#ifndef ALCHEMY_FIXES_H
#define ALCHEMY_FIXES_H

// The fixes: each repairs a bug or undefined behaviour of the original, is off in every edition,
// and is turned on by BUGFIX or UBFIX, which a build may define. They do not depend on the edition,
// so a header may include this alone.

// UBFIX gives undefined behaviour its defined form.
#ifdef UBFIX
#    define FIX_ACTOR_LOOKUP_PAST_TABLE            1
#    define FIX_BOREAS_POINTS_OVERRUN              1
#    define FIX_COATLICUE_GLOW_OVERRUN             1
#    define FIX_CRYSTAL_SHARD_TABLE_OVERRUN        1
#    define FIX_DICE_DIVIDE_BY_ZERO                1
#    define FIX_DICE_MATCH_LIST_OVERFLOW           1
#    define FIX_FUNCTION_SIGNATURES                1
#    define FIX_ITALIAN_ITEM_GENDER_READ_PAST_END  1
#    define FIX_NULL_DEREFERENCE                   1
#    define FIX_POINTER_OUTSIDE_OBJECT             1
#    define FIX_RAIN_STREAK_FIRST_TICK             1
#    define FIX_RESIDENT_SPAWN_LEADER_ACTOR        1
#    define FIX_RETURNED_STACK_BUFFER              1
#    define FIX_SEA_SAVED_COLORS_READ_BEFORE_ARRAY 1
#    define FIX_SIGNED_OVERFLOW                    1
#    define FIX_TYPE_PUNNING                       1
#    define FIX_UNDEFINED_SHIFT                    1
#    define FIX_UNINITIALIZED_READ                 1
#    define FIX_UNSEQUENCED_SIDE_EFFECT            1
#    define FIX_ZERO_SIZE_EMBER                    1
#endif

// BUGFIX repairs the original's bugs.
#ifdef BUGFIX
#    define FIX_ABILITY_ID_BOUND                     1
#    define FIX_ACTOR_LOOP_ONE_PAST_END              1
#    define FIX_ANEMOS_BLOCK_HOP_OVERRUN             1
#    define FIX_BRIGGS_FAN_SPEAKER                   1
#    define FIX_CREDITS_LINE_LEAK                    1
#    define FIX_DAEDALUS_MISSILE_SPRITES             1
#    define FIX_DAILA_BOAT_SAILOR_SPEAKER            1
#    define FIX_DAILA_PUDDLE_FLAGS_CROSSED           1
#    define FIX_DAILA_SIXTH_PUDDLE_STAYS_IN_SIGHT    1
#    define FIX_EMPTY_WINDOW_SPAN_FILL               1
#    define FIX_ENEMY_ID_BOUND                       1
#    define FIX_FROST_CHIP_DRIFT                     1
#    define FIX_GABOMBA_STEEL_EAST_LADDER_CELL       1
#    define FIX_GERMAN_ITEM_GENDER_OFF_BY_ONE        1
#    define FIX_HAURES_DIVE_VERTICAL_SPEED           1
#    define FIX_INTRO_LIGHTHOUSE_ROWS_COPIED_ONE_LOW 1
#    define FIX_ISLET_CAVE_BLUE_NEVER_DIMS           1
#    define FIX_KEPT_MESSAGE_FLAG_CLEAR              1
#    define FIX_LURE_CAP_STRAY_SECOND_LINE           1
#    define FIX_MAP_MARKER_TILE_ALLOCATION           1
#    define FIX_MONSTER_BATTLE_DEFEATED_FLAGS        1
#    define FIX_NARIBWE_WHERE_FROM_FLAG              1
#    define FIX_OCEAN_FIST_DROPS_VANISH_AT_TOP       1
#    define FIX_PACING_SON_EAST_END_WAIT             1
#    define FIX_PROCNE_SPARK_DEPTH                   1
#    define FIX_PUSH_FACING_AND_LEADER_STEP          1
#    define FIX_QUIT_HI_LO_KEEPS_GAME                1
#    define FIX_RECT_BOTTOM_CLIP                     1
#    define FIX_REQUEST_SUM_CANCELS                  1
#    define FIX_RESTORE_COUNTS_STUN                  1
#    define FIX_SHAMAN_VILLAGE_LETTERING_OVERRUN     1
#    define FIX_SLEEP_SHEEP_BOUNCE                   1
#    define FIX_SPARKLE_GROUND_HEIGHT                1
#    define FIX_STRING_BUFFER_LEAK                   1
#    define FIX_TASK_SORT_LAST_PASS                  1
#    define FIX_TEXT_TILE_SCAN_ROW_STRIDE            1
#    define FIX_VINE_SLOT_COUNTER                    1
#    define SET_DJINNI_WINDOW_FITS_SCREEN            1
#endif

// The fixes, each off unless BUGFIX or UBFIX turns it on above.

// gmAbilityGetDirect sends ability id ABILITY_COUNT, one past the end of the ability table, to row
// 0 like every other id past the table. The original lets that id through and reads the first bytes
// of the enemy table, which follows it, as an ability.
#ifndef FIX_ABILITY_ID_BOUND
#    define FIX_ABILITY_ID_BOUND 0
#endif

// In The Lost Age, an actor id from 80 (the end of the actor table) up to 191 no longer reads the
// field state after the table as an actor pointer. That state starts with the step-trigger and
// push-cell requests. fdActorLookup returns NULL for those ids, and fdActorSetOthersPaused, Sand's
// pause of every other actor, stops before id 80.
#ifndef FIX_ACTOR_LOOKUP_PAST_TABLE
#    define FIX_ACTOR_LOOKUP_PAST_TABLE 0
#endif

// Loops over the field's actors stop at the end of the actor table; the original runs one past it
// and reads the step cue and push metatile requests there as an actor.
#ifndef FIX_ACTOR_LOOP_ONE_PAST_END
#    define FIX_ACTOR_LOOP_ONE_PAST_END 0
#endif

// In Anemos Inner Sanctum's entrance, the loop that makes the six symbol blocks hop runs six times
// instead of seven. It no longer reads a seventh actor id from past the end of the six-entry block
// order and makes that actor hop, with an extra jump sound and wait.
#ifndef FIX_ANEMOS_BLOCK_HOP_OVERRUN
#    define FIX_ANEMOS_BLOCK_HOP_OVERRUN 0
#endif

// Boreas's unused copy of its point table copies the table's 16 rows instead of 32, so it no longer
// reads the 32 bytes after BoreasPoints. Nothing reads the copy, so nothing on screen changes.
#ifndef FIX_BOREAS_POINTS_OVERRUN
#    define FIX_BOREAS_POINTS_OVERRUN 0
#endif

// The boy in the blue shirt in Madra who asks about Briggs gets his message windows over his own
// head. Without the fix they open beside the young man off screen to the north-west, at the
// screen's left edge. It covers talking to him and reading him with Mind Read, before and after
// Briggs is caught.
#ifndef FIX_BRIGGS_FAN_SPEAKER
#    define FIX_BRIGGS_FAN_SPEAKER 0
#endif

// Coatlicue's cog-glow loop reads only the one entry of its one-entry glow-tick table instead of
// three. It no longer reads the two coil values after the table as glow ticks 20992 and 4096. The
// summon never reaches those ticks, so nothing visible changes in the ROM layout.
#ifndef FIX_COATLICUE_GLOW_OVERRUN
#    define FIX_COATLICUE_GLOW_OVERRUN 0
#endif

// mnCreditsDrawLine frees the line buffer it has just allocated when it reaches the end-of-text
// marker. The original returns with the buffer still taken.
#ifndef FIX_CREDITS_LINE_LEAK
#    define FIX_CREDITS_LINE_LEAK 0
#endif

// Effects that draw the spinning crystal shards skip the fifth kind of shard, whose pictures 12 to
// 14 lie past the ends of the twelve-entry crystal shard tables (PrismShardPictures,
// PrismShardWidths, PrismShardHeights). Before, they read past those tables and found zero heights,
// which draw nothing.
#ifndef FIX_CRYSTAL_SHARD_TABLE_OVERRUN
#    define FIX_CRYSTAL_SHARD_TABLE_OVERRUN 0
#endif

// Daedalus's back eight missiles (8 to 15) are placed with their own sprites instead of the sprites
// one place earlier. Missile 8 no longer borrows the front row's sprite 7, which is drawn in front
// of the machine, and the last sprite behind the machine is now placed.
#ifndef FIX_DAEDALUS_MISSILE_SPRITES
#    define FIX_DAEDALUS_MISSILE_SPRITES 0
#endif

// In flooded Daila, the boat sailor's question ('You want a boat?') and his answer open at the
// sailor himself, not at the woman who felt Indra move.
#ifndef FIX_DAILA_BOAT_SAILOR_SPEAKER
#    define FIX_DAILA_BOAT_SAILOR_SPEAKER 0
#endif

// In Daila, the Parch rows for the second, third and fourth puddles each test their own puddle's
// parched flag. Before, they took each other's flags, so drying one puddle switched off Parch on
// another and left its own row live.
#ifndef FIX_DAILA_PUDDLE_FLAGS_CROSSED
#    define FIX_DAILA_PUDDLE_FLAGS_CROSSED 0
#endif

// In Daila, Parch on the sixth puddle (north of the item stall) hides the puddle's own actor, 29.
// Before, it hid actor 31, which does not exist, so the dried puddle stayed in sight unlike the
// other five.
#ifndef FIX_DAILA_SIXTH_PUDDLE_STAYS_IN_SIGHT
#    define FIX_DAILA_SIXTH_PUDDLE_STAYS_IN_SIGHT 0
#endif

// In Lucky Dice (both games) and Super Lucky Dice (The Lost Age), two touching dice whose
// whole-pixel separation comes out as 0 get no push apart that tick, instead of dividing by that
// zero. That happens when they are less than a pixel apart on every axis, measured toward the other
// die.
#ifndef FIX_DICE_DIVIDE_BY_ZERO
#    define FIX_DICE_DIVIDE_BY_ZERO 0
#endif

// The unused list of matched numbers in dice scoring (Lucky Dice in both games, Super Lucky Dice in
// TLA) gets a third slot. Three alike make three pairs, and the original writes the third pair past
// its two-slot array.
#ifndef FIX_DICE_MATCH_LIST_OVERFLOW
#    define FIX_DICE_MATCH_LIST_OVERFLOW 0
#endif

// With no window state, mnScanlineWindowFillSpans fills the screen transition's whole window-edge
// page: two halfwords per scan line, DISPLAY_HEIGHT * 4 bytes. The original fills only half of it,
// so the lower 80 lines keep stale window 1 edges.
#ifndef FIX_EMPTY_WINDOW_SPAN_FILL
#    define FIX_EMPTY_WINDOW_SPAN_FILL 0
#endif

// gmEnemyGetDefinition sends every row past the end of the enemy table to row 0, not only rows from
// ENEMY_ID_MAX on. Ids from the table's length (165 rows in TBS, 380 in TLA) up to ENEMY_ID_MAX no
// longer read the data after the table.
#ifndef FIX_ENEMY_ID_BOUND
#    define FIX_ENEMY_ID_BOUND 0
#endif

// Frost's ice chips get a sideways velocity (velocityX) of 0 when they burst, so they fall
// straight. The original never sets it, so each chip drifts sideways by whatever word its PARTICLES
// slot last held: the field's map grid cells, or an earlier effect's particles.
#ifndef FIX_FROST_CHIP_DRIFT
#    define FIX_FROST_CHIP_DRIFT 0
#endif

// Every function is declared before it is called, and its definition, prototypes, calls and stored
// pointers agree on one type. Prototypes are added. A function that falls off its end returns a
// value. A function whose result nobody reads is made void, or given a return value where it stays
// s32. Missing arguments are passed. Calls and stores through casts to other function types are
// dropped.
#ifndef FIX_FUNCTION_SIGNATURES
#    define FIX_FUNCTION_SIGNATURES 0
#endif

// In Gabomba Statue, the east ladder down in the room where Steel is found tests the cell west of
// its hole, (41, 13), for an eastward push. Before, it tested the cell east of the hole, (43, 13),
// where the map has no hole cell, so the leader dropped in from the west without pushing east. With
// the fix, this ladder works like the room's other two.
#ifndef FIX_GABOMBA_STEEL_EAST_LADDER_CELL
#    define FIX_GABOMBA_STEEL_EAST_LADDER_CELL 0
#endif

// The German item gender table gains a row for item 0, so each item takes its own gender for its
// possessive ('dein' or 'deine') instead of the next item's.
#ifndef FIX_GERMAN_ITEM_GENDER_OFF_BY_ONE
#    define FIX_GERMAN_ITEM_GENDER_OFF_BY_ONE 0
#endif

// As Haures dives away up and to the left, his vertical move adds his vertical speed instead of his
// horizontal speed. The climb becomes about twice as steep instead of a 45-degree diagonal; before,
// the vertical speed was computed and never used.
#ifndef FIX_HAURES_DIVE_VERTICAL_SPEED
#    define FIX_HAURES_DIVE_VERTICAL_SPEED 0
#endif

// When the title intro scrolls back up the lighthouse picture, the row copy starts from picture row
// 16, the row it writes to, instead of row 15. Each restored row lands in its own place, and the
// last copy is row 0 rather than the row above the picture. The fault is hidden under the fade to
// white.
#ifndef FIX_INTRO_LIGHTHOUSE_ROWS_COPIED_ONE_LOW
#    define FIX_INTRO_LIGHTHOUSE_ROWS_COPIED_ONE_LOW 0
#endif

// The Islet Cave's darkening dims the blue channel by the given scale, as it already does red and
// green. Before, it multiplied blue by the full level, so blue never dimmed. The deeper corridors
// and the Sentinel's room now get darker without also turning bluer.
#ifndef FIX_ISLET_CAVE_BLUE_NEVER_DIMS
#    define FIX_ISLET_CAVE_BLUE_NEVER_DIMS 0
#endif

// The Italian item gender table runs to ITEM_MAGMA_BALL, so an item after the Cleric's Ring (id
// 268, its last row) no longer reads past the table's end. The added rows are all masculine, not
// real genders.
#ifndef FIX_ITALIAN_ITEM_GENDER_READ_PAST_END
#    define FIX_ITALIAN_ITEM_GENDER_READ_PAST_END 0
#endif

// While a message is kept in the info window, a row move in the Psynergy list clears the
// kept-message flag instead of FLAG_SCENE_TEMP_FF. Psynergy descriptions then come back on the next
// row move. In the original they stay away until another screen clears the flag.
#ifndef FIX_KEPT_MESSAGE_FLAG_CLEAR
#    define FIX_KEPT_MESSAGE_FLAG_CLEAR 0
#endif

// In German, the Lure Cap's effect line in the item details no longer gets the unrelated message
// stored after the real continuation lines ('Items') as a second line. That line is left blank, and
// the effect name stays 'Erhöht gegn.'.
#ifndef FIX_LURE_CAP_STRAY_SECOND_LINE
#    define FIX_LURE_CAP_STRAY_SECOND_LINE 0
#endif

// The map screen reserves the full 128 bytes that its four marker tiles fill, write and upload,
// instead of 32 (the size worked out as if the heap counted words), so the tiles no longer run 96
// bytes past their block. The same repair in both games.
#ifndef FIX_MAP_MARKER_TILE_ALLOCATION
#    define FIX_MAP_MARKER_TILE_ALLOCATION 0
#endif

// The monster battle pool marks Punch Ant, Giant Bat and Wild Wolf defeated through
// ENEMY_DEFEATED_FLAG. The original sets their bare enemy ids as flags, which sets the Spritz and
// Ether Djinn flags and flag 0xe6 instead. So formations 241 to 248, made only of these three
// enemies, stayed out of the pool until the player had beaten them.
#ifndef FIX_MONSTER_BATTLE_DEFEATED_FLAGS
#    define FIX_MONSTER_BATTLE_DEFEATED_FLAGS 0
#endif

// A Yes to the Naribwe woman's 'Where have you come from?' sets her own flag,
// FLAG_ASKED_WHERE_FROM, not the expedition woman's. So she stops asking, the expedition woman
// keeps asking her own question, and the woman's line that it is always hot in Naribwe, never heard
// in the ROM, is shown at her.
#ifndef FIX_NARIBWE_WHERE_FROM_FLAG
#    define FIX_NARIBWE_WHERE_FROM_FLAG 0
#endif

// Code that reads or writes through a NULL actor, sprite, window, sprite-list head or placement
// checks for NULL first, returns early, or drops the store or call. The cases are empty actor
// slots, actors a scene never places, a sprite that failed to create, a camera with no leader, a
// window that was never opened, and the actor placement search when the last table is absent.
#ifndef FIX_NULL_DEREFERENCE
#    define FIX_NULL_DEREFERENCE 0
#endif

// In Ocean Fist, a rising drop is removed only once it is wholly above the canvas's top line, not
// as soon as it crosses it while its lower half is still drawn. The test now converts the drop's
// size to 16.16 before comparing it with the drop's 16.16 y.
#ifndef FIX_OCEAN_FIST_DROPS_VANISH_AT_TOP
#    define FIX_OCEAN_FIST_DROPS_VANISH_AT_TOP 0
#endif

// The sailor's son pacing West Indra Islet's pier waits 80 ticks at the east end, as he already
// does at the west end, instead of turning straight back. The ROM's script has SC_SYNC and a bare
// 80 where SC_WAIT(80) was meant, and the walker skips the 80.
#ifndef FIX_PACING_SON_EAST_END_WAIT
#    define FIX_PACING_SON_EAST_END_WAIT 0
#endif

// Indexes and pointer walks stay inside the object they start in, and the same bytes are read and
// written. Separate members read as an array get a real array in a union. One-element arrays,
// tables and heap slots are no longer indexed past; the neighbouring object is named or reached
// through its own slot. Pointers are no longer formed before an array's start. Reads and writes
// that must still cross a struct's members go through memcpy on the enclosing object.
#ifndef FIX_POINTER_OUTSIDE_OBJECT
#    define FIX_POINTER_OUTSIDE_OBJECT 0
#endif

// Procne's strike sparks keep their up-and-down speed and move in depth by their own random depth
// speed, kept in velocityZ. The original writes the depth speed over velocityY, so the up-and-down
// speed is lost and each spark's height and depth move together.
#ifndef FIX_PROCNE_SPARK_DEPTH
#    define FIX_PROCNE_SPARK_DEPTH 0
#endif

// A push in The Lost Age reads the leader's 16-bit facing as a sixteenth (FACING_INDEX), not
// as 16.16, whose whole part is always 0 (east). It also takes the leader's step after the push
// from both halves of its PushLeaderSteps entry (half of PUSH_STEP_X and of PUSH_STEP_Z), not from
// the z half alone. The pushed log or pillar then rolls with the animation for its real direction
// (3 from southwest to north-northeast, otherwise 2), and the leader steps half a cell after it,
// the way he pushed. In the ROM every push uses animation 2 and the leader takes no step. Where the
// object ends up is unchanged in both builds.
#ifndef FIX_PUSH_FACING_AND_LEADER_STEP
#    define FIX_PUSH_FACING_AND_LEADER_STEP 0
#endif

// Leaving a Super Lucky Dice hi-lo game with B resets the hi-lo game and its target total. The next
// throw is then scored as a fresh throw, not judged higher or lower against the abandoned game with
// its banner left up. This can happen only where hi-lo games can be quit: in the ja edition, games
// 2 to 5 (that is, without HI_LO_GAMES_CANNOT_BE_QUIT).
#ifndef FIX_QUIT_HI_LO_KEEPS_GAME
#    define FIX_QUIT_HI_LO_KEEPS_GAME 0
#endif

// A rain streak respawned in the field rain (Prox, Mars Lighthouse) now starts its timer at 11,
// not 12. Its first tick reads the offset table's last pair instead of the pair one past the end,
// which would draw it 2 pixels left of its spot and 12 below. Each fall after a respawn becomes one
// tick shorter. The 8 drops that the rain's start gives 12, and streaks while the weather is held,
// still read past the end.
#ifndef FIX_RAIN_STREAK_FIRST_TICK
#    define FIX_RAIN_STREAK_FIRST_TICK 0
#endif

// The window tilemap rectangle routines clip a rectangle's bottom once it passes the screen's 20
// tile rows. These are the highlight, palette, interior-palette, fill and text-tile routines. The
// original tested the bottom against the 30-tile width, so a rectangle ending in rows 20 to 28 was
// kept whole and written past the end of the windows' 32 by 20 tilemap.
#ifndef FIX_RECT_BOTTOM_CLIP
#    define FIX_RECT_BOTTOM_CLIP 0
#endif

// fdCountRequests ORs the field's pending requests instead of adding them. Requests of opposite
// sign, such as a party member falling to poison (-1) and step trigger 1, no longer sum to 0 and
// read as "nothing pending". In the original both wait until another request arrives, and by then a
// later cell may have replaced the step trigger.
#ifndef FIX_REQUEST_SUM_CANCELS
#    define FIX_REQUEST_SUM_CANCELS 0
#endif

// The field-effect spawner in the main program (the copy outside the overlays) takes the sprite
// priority from the current leader's actor, as the overlays' copy does. It used to take it from
// Isaac's actor 0, which is not on stage while Felix leads, so the priority was read through NULL.
// Its one caller is the radial particle ring.
#ifndef FIX_RESIDENT_SPAWN_LEADER_ACTOR
#    define FIX_RESIDENT_SPAWN_LEADER_ACTOR 0
#endif

// When the enemy AI picks an ally to Restore, it counts the ally's stun, which Restore cures,
// instead of confusion and charm, which it does not. An ally who is only stunned is no longer
// passed over, and one who is only confused or charmed is no longer picked. Tonic's count includes
// Restore's and changes the same way.
#ifndef FIX_RESTORE_COUNTS_STUN
#    define FIX_RESTORE_COUNTS_STUN 0
#endif

// btTextCopyFourCharsUnderscored builds its four-character copy in a static buffer, so the pointer
// it returns into that copy stays valid after the function returns. Before, it pointed into a stack
// array that no longer exists.
#ifndef FIX_RETURNED_STACK_BUFFER
#    define FIX_RETURNED_STACK_BUFFER 0
#endif

// When the Sea of Time's saved sea colours (BG palette 3's 14 colours, which the fade returns to)
// are rotated, the last pass no longer copies the halfword before the array. The store after the
// loop overwrote that copy anyway, so the colours are the same.
#ifndef FIX_SEA_SAVED_COLORS_READ_BEFORE_ARRAY
#    define FIX_SEA_SAVED_COLORS_READ_BEFORE_ARRAY 0
#endif

// The buffer that Shaman Village decodes its lettering sprite sheet into is sized for all eight
// pictures instead of seven. Decoding no longer writes past it, by 1024 bytes in most editions or
// 512 in French, whose GO picture is half height.
#ifndef FIX_SHAMAN_VILLAGE_LETTERING_OVERRUN
#    define FIX_SHAMAN_VILLAGE_LETTERING_OVERRUN 0
#endif

// Arithmetic that can overflow a signed int is done unsigned. One case is negating a negative
// number in the message decoder's signed-number argument: the magnitude becomes u32, 0U -
// (u32)value. The other is the file list glow's sine phase, FrameCount times 768, which overflows
// after about 13 hours: it is done in u32 and masked to an angle.
#ifndef FIX_SIGNED_OVERFLOW
#    define FIX_SIGNED_OVERFLOW 0
#endif

// Sleep's sheep bounce off the ground, tumbling faster, whenever they are below it and falling
// (speed above 0). The original bounces them only at a speed of exactly 0, which never happens just
// after gravity is added, so they fall through the ground.
#ifndef FIX_SLEEP_SHEEP_BOUNCE
#    define FIX_SLEEP_SHEEP_BOUNCE 0
#endif

// A burst of rising sparkles takes its ground height from under the burst (spawnX, spawnZ). The
// original reads it at the screen position of the last sparkle placed, taken as a map position,
// which lands near the map's top-left corner. This applies to the burst's first sparkle and to the
// rest of it.
#ifndef FIX_SPARKLE_GROUND_HEIGHT
#    define FIX_SPARKLE_GROUND_HEIGHT 0
#endif

// mnTextDrawStringAtOffset frees its string buffer whether or not the text starts inside the
// tilemap. The original frees it only on the on-screen path, so text placed below the screen leaves
// the buffer taken.
#ifndef FIX_STRING_BUFFER_LEAK
#    define FIX_STRING_BUFFER_LEAK 0
#endif

// syTaskSort's bubble sort runs its last pass (down to pass > 0, not > 1), so the tasks in slots 0
// and 1 also end in priority order, highest first.
#ifndef FIX_TASK_SORT_LAST_PASS
#    define FIX_TASK_SORT_LAST_PASS 0
#endif

// The scan that marks which text tiles the screen shows (mnWindowMarkVisibleTextTiles) skips the
// two hidden columns at the end of each 32-entry tilemap row, so it covers exactly the visible 30
// by 20 area. The original never reaches the 36 visible tiles at the bottom (row 19 and the last
// six of row 18), so their text tiles are freed while still on screen.
#ifndef FIX_TEXT_TILE_SCAN_ROW_STRIDE
#    define FIX_TEXT_TILE_SCAN_ROW_STRIDE 0
#endif

// Objects are read and written through their own declared types, not through casts to other integer
// widths, to words standing for pointers, or to types that drop volatile. Halfword pairs are built
// with shifts or byte stores, and a 16.16 value's whole part is taken with a shift. Sprite and OAM
// words are written through their fields or copied in with memcpy. Pointers and function pointers
// are read and stored as pointers, and TickCount is read as declared.
#ifndef FIX_TYPE_PUNNING
#    define FIX_TYPE_PUNNING 0
#endif

// Shifts that C leaves undefined are rewritten to give the result the ARM gives. Left shifts of
// negative or overflowing signed values become multiplications, unsigned shifts or the equivalent
// defined facing comparison (FACING_IS_NORTH). Shifts by 32 bits or more are skipped where the
// count can reach the word's width: the status bar's empty last tile, and an empty enemy name's
// letter bit.
#ifndef FIX_UNDEFINED_SHIFT
#    define FIX_UNDEFINED_SHIFT 0
#endif

// Locals, parameters and stack words that are read before anything sets them are given a value or
// no longer read. The cases are camera zoom, the star base point, a frozen actor's arrival flag,
// the SC_MOVE_LOCAL offset, the encounter zone entry, the summon max-HP bonus, the speaker message
// position, the map cursor and marker-0 name, the overwrite result, the line spacing count, the
// VRAM cache entry freed, the never-created details window and the knock-back state pointer.
#ifndef FIX_UNINITIALIZED_READ
#    define FIX_UNINITIALIZED_READ 0
#endif

// An expression that changes a value and also reads or changes it again with no sequence point
// between is split into ordered statements that keep the shipped result. The cases: a picture's
// width, height or size assigned in one draw-call argument and read in another; n = ++n % count; a
// kind byte read through *code++ in the call target while code is also passed; a page arrow's row
// written twice in one expression.
#ifndef FIX_UNSEQUENCED_SIDE_EFFECT
#    define FIX_UNSEQUENCED_SIDE_EFFECT 0
#endif

// The hanging-vine sway saves its slot counter before the loop that hangs a vine's five links,
// which reuses that counter, and restores it afterwards. Every active vine now sways. Before, past
// the first active vine, only an unbroken run of active slots kept swaying. The fix also stops the
// sweep from reading one slot past the end of the array.
#ifndef FIX_VINE_SLOT_COUNTER
#    define FIX_VINE_SLOT_COUNTER 0
#endif

// In the ember burst of Meteor and Outer Space, the farthest embers (depth 736 and beyond) are
// drawn at size 1 instead of size 0. At size 0 the picture offset is read from
// BattleEffectMotePictures[-1], before the table's start.
#ifndef FIX_ZERO_SIZE_EMBER
#    define FIX_ZERO_SIZE_EMBER 0
#endif

// When a battle target is chosen, the Set Djinni window is kept on screen by its real width: 9
// tiles outside Japanese, rather than the Japanese 8. Against the right edge of the screen, its
// last column therefore no longer runs past the screen. BUGFIX also turns it on in every edition of
// both games.
#ifndef SET_DJINNI_WINDOW_FITS_SCREEN
#    define SET_DJINNI_WINDOW_FITS_SCREEN 0
#endif

#endif
