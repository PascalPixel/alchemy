#ifndef ALCHEMY_HEAP_H
#define ALCHEMY_HEAP_H

/// @file
/// The heap's slots and HeapState.
///
/// The heap is two stacks of blocks, one in IWRAM and one in EWRAM; the
/// System README's "The heap" gives the allocators and
/// the lifetime rule. Every scene change resets both pools and clears every
/// slot (Heap_Initialize, from the main routine in
/// system/main_routine.c), so no block outlives the scene, battle or
/// minigame that took it.

#include "static_assert.h"
#include "types.h"

/// The heap's 64 words in IWRAM, seen as union HeapState.
extern void *gHeapSlots[64];

/// Slot n's block, or null before it is allocated.
#define HEAP_SLOT_BLOCK(n) (*(gHeapSlots + (n)))

/// Heap slot `slot` read through `slots`, a pointer to slot `from` of
/// gHeapSlots: code that holds one slot's address reaches its neighbours by
/// their distance from it.
#define HEAP_SLOT_FROM(slots, from, slot) ((slots)[(slot) - (from)])

/// The heap's 64 slots, the first two holding the next free EWRAM and
/// IWRAM addresses.
union HeapState {
  struct {
    void *nextEwram;  ///< the EWRAM pool's next free byte, up to EWRAM_END
    void *nextIwram;  ///< the IWRAM pool's next free byte, up to the heap's end
    void *blocks[62]; ///< heap slots 2 to 63
  } heap;
  /// Each slot's block once allocated, else null; slots 0 and 1 are the pool
  /// pointers.
  void *slots[64];
};

STATIC_ASSERT(sizeof(union HeapState) == 256);
STATIC_ASSERT(offsetof(union HeapState, heap.nextIwram) == 4);
STATIC_ASSERT(offsetof(union HeapState, heap.blocks) == 8);

/// The heap slots shared across folders, by index into gHeapSlots.
enum HeapSlot {
  HEAP_SLOT_MATRIX_STACK = 2, ///< The 3D transform's one saved level.
  /// The sprite animations (struct SpriteAnimation) of the actor
  /// system's sprites (gSpriteAnimations).
  HEAP_SLOT_SPRITE_ANIMATIONS = 3,
  /// The actor system's sprite pool (struct Sprite).
  HEAP_SLOT_SPRITES = 4,
  /// The 64 actors (struct Actor).
  HEAP_SLOT_ACTORS = 5,
  /// The actor system's state (struct ActorSystemState, gActorSystemState),
  /// taken by ActorSystem_Initialize.
  HEAP_SLOT_ACTOR_SYSTEM = 6,
  /// The world map's affine state (struct WorldMapAffineState).
  HEAP_SLOT_WORLD_MAP_AFFINE = 7,
  /// The field map's scrolling state (struct MapGridScrollState), or on the
  /// world map its perspective view (struct WorldMapView); gMapGridState.
  HEAP_SLOT_MAP_GRID = 8,
  /// A battle's struct BattleState (gBattleState), from Battle_Run to
  /// the next scene change.
  HEAP_SLOT_BATTLE = 9,
  /// The battle background's scanline state (struct BattleAffineHdma).
  HEAP_SLOT_BATTLE_SCANLINES = 10,
  /// Unused: 640 bytes Battle_Run allocates with the battle's other blocks,
  /// which nothing reads or writes.
  HEAP_SLOT_BATTLE_UNUSED = 11,
  /// The 3D camera (struct BattleCamera, gBattleCamera) of a battle, the world
  /// map or Lucky Dice.
  HEAP_SLOT_CAMERA = 12,
  /// A scratch buffer, usually 1 KB, that a picture is decoded into and
  /// released as soon as its tiles are in VRAM.
  HEAP_SLOT_TILE_SCRATCH = 14,
  /// The window renderer's state (struct WindowState, gWindowState).
  HEAP_SLOT_WINDOW = 15,
  /// The party status window's layout (struct PartyStatusWindow).
  HEAP_SLOT_PARTY_STATUS = 16,
  /// The buffers an icon or portrait is decoded through (GlyphTransfer).
  HEAP_SLOT_GLYPH_TRANSFER = 17,
  /// The selection screen's state (struct SelectionMenuState).
  HEAP_SLOT_SELECTION_MENU = 18,
  /// The selection state Menu_AllocateSelectionState allocates, which nothing
  /// calls.
  HEAP_SLOT_SELECTION_STATE = 19,
  /// The settings menu's state (struct OptionMenuState, gOptionMenuState).
  HEAP_SLOT_SETTINGS_MENU = 20,
  /// Mind Read's wave state (struct MindReadState, gMindReadState).
  HEAP_SLOT_MIND_READ = 22,
  /// A field scene's struct FieldState, which scenes read as struct FieldState
  /// (gFieldState); FieldSequence_Run takes it and frees it on the way out.
  HEAP_SLOT_FIELD = 27,
  /// The palette cycles (struct PaletteCycleState).
  HEAP_SLOT_PALETTE_CYCLES = 28,
  /// The running ground particles, motes or sparkles (struct
  /// WeatherState).
  HEAP_SLOT_WEATHER = 29,
  /// The field's colour blend state (gFieldBlendState): the struct
  /// FieldBlendState the storm's lightning and Weather_StartLightningFlash
  /// allocate.
  HEAP_SLOT_FIELD_BLEND = 30,
  /// The screen transition (struct ScreenTransitionState,
  /// gScreenTransitionState): the window edges and dissolve of a scene change.
  HEAP_SLOT_SCREEN_TRANSITION = 31,
  /// The palette fade (gPaletteFadeState) that moves the BG and sprite palettes
  /// toward target colours: the struct PaletteFade
  /// PaletteFade_Initialize allocates on scene load, or the title
  /// intro's struct TitlePaletteFade.
  HEAP_SLOT_PALETTE_FADE = 32,
  /// The floating lights of Sol Sanctum's Elemental Star room (struct
  /// SolSanctumRingList, sol_sanctum_elemental_star_ring_setup.c).
  HEAP_SLOT_SOL_SANCTUM_LIGHTS = 33,
  /// The line scroll's state (struct LineScrollState).
  HEAP_SLOT_LINE_SCROLL = 34,
  /// Where Mercury Lighthouse keeps a pointer to the state of the lights that
  /// carry the leader over its pools (struct MercuryLighthouseLights).
  HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS = 35,
  /// The fade overlay's state (struct FadeOverlayState).
  HEAP_SLOT_FADE_OVERLAY = 36,
  /// The battle message window's state (struct BattleMessageState).
  HEAP_SLOT_BATTLE_MESSAGE = 37,
  /// The running battle effect's own state (gBattleEffectState). Lucky Dice
  /// keeps its own state in this slot and the next two.
  HEAP_SLOT_BATTLE_EFFECT = 39,
  /// The running battle effect's pixel canvas.
  HEAP_SLOT_BATTLE_CANVAS = 40,
  /// The running battle effect's tile sheet, or its set of mote cells.
  HEAP_SLOT_BATTLE_SHEET = 41,
  /// The battle's entry transition timer, in frames (gBattleEntryTimer).
  HEAP_SLOT_BATTLE_ENTRY_TIMER = 42,
  /// The title intro's state (struct IntroState).
  HEAP_SLOT_TITLE_INTRO = 43,
  /// The battle background's view and entry transition (32 bytes), from
  /// Battle_Run.
  HEAP_SLOT_BATTLE_BACKGROUND = 44,
  /// Lucky Dice's sprites (1,560 bytes), for as long as the game runs.
  HEAP_SLOT_LUCKY_GAME = 45,
  /// A routine run from RAM: the canvas drawing routine
  /// BattleEffect_BuildBlitter writes, called as a DrawRectangle and released
  /// before its canvas, or the world map's copy of Matrix_UpdateVertices.
  HEAP_SLOT_RAM_CODE = 46,
  /// A second routine as HEAP_SLOT_RAM_CODE, released before it: always a
  /// BattleEffect_BuildBlitter routine, the second of a pair whose first is in
  /// HEAP_SLOT_RAM_CODE.
  HEAP_SLOT_RAM_CODE_PAIRED = 47,
  /// Copied field row drawer or window tile decoder, released after each call.
  HEAP_SLOT_RAM_CODE_TEMP = 49,
  /// The message decoder routine (Text_DecodeSymbol), copied here to run for
  /// the length of one message's decoding (Message_Decode), or of one
  /// Message_DecodeRaw call when the slot is empty.
  HEAP_SLOT_MESSAGE_DECODER = 50,
  /// The save state (struct SaveState), from
  /// Save_InitializeState to Save_ReleaseState around each
  /// save, load, copy or delete and each scan of the files, as the main
  /// menu's (Save_ScanFileSummaries).
  HEAP_SLOT_SAVE = 51,
  /// Lives for one frame: wait() builds the OAM list in it, and the field
  /// sprite decoder keeps its code there between its own uses.
  HEAP_SLOT_FRAME_SCRATCH = 52,
  /// A RAM copy of Tile_CopyStrided, which uploads field sprite pictures to
  /// VRAM; loaded with the animation pools.
  HEAP_SLOT_PICTURE_UPLOADER = 53,
  /// Each linked console's party (ENEMY_UNITS_SIZE bytes), from the
  /// Battle Arena and Battle_Run.
  HEAP_SLOT_ENEMY_UNITS = 54,
  /// The menu state (MENU_STATE_SIZE bytes) shared by the field menu's screens
  /// and the shop; each takes it on opening and releases it on closing.
  HEAP_SLOT_MENU = 55,
  /// The Psynergy effect's state (struct FieldEffectState, gFieldEffectState).
  HEAP_SLOT_FIELD_EFFECT = 56,
  /// The battle command menus' state (struct BattleMenuState).
  HEAP_SLOT_BATTLE_MENU = 57,
  /// A command icon menu's state (struct IconMenuState).
  HEAP_SLOT_ICON_MENU = 58,
  /// The Colosso finals' race state (struct ColossoRaceState), which
  /// gColossoState reads by its own name.
  HEAP_SLOT_COLOSSO_RACE = 59
};

#endif
