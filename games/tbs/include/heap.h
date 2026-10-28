#ifndef ALCHEMY_HEAP_H
#define ALCHEMY_HEAP_H

/// @file
/// The heap's slots and HeapState.
///
/// The heap is two stacks of blocks, one in IWRAM and one in EWRAM; the
/// runtime README's "The heap" gives the allocators and
/// the lifetime rule. Every scene change resets both pools and clears every
/// slot (InitHeap, from the parent sequence in overworld/parent_sequence.c), so
/// no block outlives the scene, battle or minigame that took it.

#include "types.h"
#include "static_assert.h"

/// The heap's 64 words in IWRAM (0x03001e50), seen as union HeapState. The
/// work globals named in cartridge/iwram.s (gBattleWork, gBattleCamera,
/// gFieldWork and others) are single slots of this table.
extern void *gHeapSlots[64];

/// Slot n's block, or null before it is allocated.
// Reached through the slot's own address, adjacent slots by offsets from it;
// gHeapSlots[n] addresses from the table's start instead. Each site keeps the
// form it matches with (MATCHING.md).
#define HEAP_SLOT_BLOCK(n) (*(gHeapSlots + (n)))

/// The heap's 64 slots, the first two holding the next free EWRAM and
/// IWRAM addresses.
union HeapState {
  struct {
    void *nextEwram;  ///< the EWRAM pool's next free byte, up to 0x02040000
    void *nextIwram;  ///< the IWRAM pool's next free byte, up to the heap's end
    void *blocks[62]; ///< heap slots 2 to 63; not named by TBS code
  } heap;
  /// Each slot's block once allocated, else null; slots 0 and 1 are the pool
  /// pointers. Allocating a slot that already has a block returns that block,
  /// whatever size is asked for.
  void *slots[64];
};

STATIC_ASSERT(sizeof(union HeapState) == 0x100);
STATIC_ASSERT(offsetof(union HeapState, heap.nextIwram) == 4);
STATIC_ASSERT(offsetof(union HeapState, heap.blocks) == 8);

/// The heap slots shared across folders, by index into gHeapSlots. Releasing a
/// slot (FreeHeapSlot) also releases every block taken after it in
/// the same pool.
enum HeapSlot {
  HEAP_SLOT_TRANSFORM_STACK = 2, ///< The 3D transform's one saved level.
  /// The sprite anims (struct SpriteAnim) of the actor
  /// system's sprites (gSpriteAnims).
  HEAP_SLOT_SPRITE_ANIMS = 3,
  /// The actor system's sprite pool (struct Sprite).
  HEAP_SLOT_SPRITES = 4,
  /// The 64 actors (struct Actor).
  HEAP_SLOT_ACTORS = 5,
  /// The actor system's work (struct ActorSystemWork, gActorSystemWork),
  /// taken by ActorSystem_Initialize. The item, Psynergy, Status, Djinn and
  /// debug menus and the world map effect set its suspended field while they
  /// are open, so sprites that are not held are not drawn and free their VRAM.
  HEAP_SLOT_ACTOR_SYSTEM = 6,
  /// The world map's affine work (struct WorldMapAffineWork).
  HEAP_SLOT_WORLD_MAP_AFFINE = 7,
  /// The field map's scrolling work (struct MapGridScrollWork), or on the world
  /// map its perspective view (struct WorldMapView); gMapGridWork.
  HEAP_SLOT_MAP_GRID = 8,
  /// A battle's struct BattleWork (gBattleWork), from Battle_Run to
  /// the next scene change.
  HEAP_SLOT_BATTLE = 9,
  /// The battle background's scanline work (struct BattleAffineHdma).
  HEAP_SLOT_BATTLE_SCANLINES = 10,
  /// The 3D camera (struct BattleCamera, gBattleCamera) of a battle, the world
  /// map or Lucky Dice.
  HEAP_SLOT_CAMERA = 12,
  /// A scratch buffer, usually 1 KB, that a picture is decoded into and
  /// released as soon as its tiles are in VRAM.
  HEAP_SLOT_TILE_SCRATCH = 14,
  /// The window renderer's work (struct WindowWork, gWindowWork).
  HEAP_SLOT_WINDOW = 15,
  /// The party status window's layout (struct PartyStatusWindow).
  HEAP_SLOT_PARTY_STATUS = 16,
  /// The buffers an icon or portrait is decoded through (GlyphTransfer).
  HEAP_SLOT_GLYPH_TRANSFER = 17,
  /// The selection screen's work (struct SelectionMenuWork).
  HEAP_SLOT_SELECTION_MENU = 18,
  /// The work of Menu_AllocateSelectionWorkspace, which nothing calls.
  HEAP_SLOT_SELECTION_WORKSPACE = 19,
  /// The settings menu's work (struct OptionMenuWork, gOptionMenuWork).
  HEAP_SLOT_SETTINGS_MENU = 20,
  /// Mind Read's wave work (struct MindReadWork, gMindReadWork).
  HEAP_SLOT_MIND_READ = 22,
  /// A field scene's struct FieldWork, which scenes read as struct FieldWork
  /// (gFieldWork); SceneSequence_Run takes it and frees it on the way out.
  HEAP_SLOT_FIELD = 27,
  /// The palette cycles (struct PaletteCycleWork).
  HEAP_SLOT_PALETTE_CYCLES = 28,
  /// The running ground particles, motes or sparkles (struct
  /// WeatherWork).
  HEAP_SLOT_WEATHER = 29,
  /// The field's colour blend work (gFieldBlendWork): the struct
  /// FieldBlendWork the storm's lightning and Weather_StartLightningFlash
  /// allocate.
  HEAP_SLOT_FIELD_BLEND = 30,
  /// The screen transition (struct ScreenTransitionState,
  /// gScreenTransitionWork): the window edges and dissolve of a scene change.
  HEAP_SLOT_SCREEN_TRANSITION = 31,
  /// The palette fade (gPaletteFadeWork) that moves the BG and sprite palettes
  /// toward target colours: the struct PaletteFade
  /// PaletteFade_Initialize allocates on scene load, or the title
  /// intro's struct TitlePaletteFade.
  HEAP_SLOT_PALETTE_FADE = 32,
  /// The floating lights of Sol Sanctum's Elemental Star room (struct
  /// SolSanctumRingList, sol_sanctum_elemental_star_ring_setup.h).
  HEAP_SLOT_SOL_SANCTUM_LIGHTS = 33,
  /// The scanline effect's work (struct ScanlineEffectWork).
  HEAP_SLOT_SCANLINE_EFFECT = 34,
  /// Where Mercury Lighthouse keeps a pointer to the state of the lights that
  /// carry the leader over its pools (struct MercuryLighthouseLights).
  HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS = 35,
  /// The fade overlay's work (struct FadeOverlayWork).
  HEAP_SLOT_FADE_OVERLAY = 36,
  /// The battle message window's work (struct BattleMessageWork).
  HEAP_SLOT_BATTLE_MESSAGE = 37,
  /// The running battle animation's own work (gBattleAnimWork). Lucky Dice
  /// keeps its own work in this slot and the next two.
  HEAP_SLOT_BATTLE_ANIM = 39,
  /// The running battle animation's pixel canvas.
  HEAP_SLOT_BATTLE_CANVAS = 40,
  /// The running battle animation's tile sheet, or its set of mote cells.
  HEAP_SLOT_BATTLE_SHEET = 41,
  /// The battle's entry transition timer, in frames (gBattleEntryTimer).
  HEAP_SLOT_BATTLE_ENTRY_TIMER = 42,
  /// The title intro's state (struct IntroWork).
  HEAP_SLOT_TITLE_INTRO = 43,
  /// The battle background's view and entry transition (32 bytes), from
  /// Battle_Run.
  HEAP_SLOT_BATTLE_BACKGROUND = 44,
  /// Lucky Dice's sprites (0x618 bytes), for as long as the game runs.
  HEAP_SLOT_LUCKY_DICE_SPRITES = 45,
  /// A routine run from RAM: the canvas drawing routine
  /// BattleAnim_BuildBlitter writes, called as a DrawRectangle and released
  /// before its canvas, or the world map's copy of Transform_UpdateVertices.
  HEAP_SLOT_RAM_CODE = 46,
  /// A second routine as HEAP_SLOT_RAM_CODE, released before it: always a
  /// BattleAnim_BuildBlitter routine, the second of a pair whose first is in
  /// HEAP_SLOT_RAM_CODE.
  HEAP_SLOT_RAM_CODE_2 = 47,
  /// Copied field row drawer or window tile decoder, released after each call.
  HEAP_SLOT_RAM_CODE_TEMP = 49,
  /// The save workspace (struct SaveWork), from
  /// Save_InitializeWork to Save_ReleaseWork around each
  /// save, load, copy or delete and each scan of the files, as the main
  /// menu's (Save_ScanRecordFlags).
  HEAP_SLOT_SAVE = 51,
  /// Lives for one frame: WaitFrames builds the OAM list in it, and the field
  /// sprite decoder keeps its code there between its own uses.
  HEAP_SLOT_FRAME_SCRATCH = 52,
  /// A RAM copy of Tile_CopyStrided, which uploads field sprite pictures to
  /// VRAM; loaded with the animation pools.
  HEAP_SLOT_PICTURE_UPLOADER = 53,
  /// Each linked console's party (LINK_OWNER_STATES_SIZE bytes), from the
  /// Battle Arena and Battle_Run.
  HEAP_SLOT_ENEMY_UNITS = 54,
  /// The menu work (MENU_WORK_SIZE bytes) shared by the field menu's screens
  /// and the shop; each takes it on opening and releases it on closing. The
  /// save screen takes it for its sprite work (struct ShopWork).
  HEAP_SLOT_MENU = 55,
  /// The Psynergy effect's work (struct FieldEffectWork, gFieldEffectWork). A
  /// field Psynergy holds it from FieldPsynergy_LoadResources to
  /// FieldPsynergy_End, a Djinni's joining from
  /// FieldEffect_InitializeSlots to
  /// FieldEffect_ClearActiveSlotsAndScheduleUpdates.
  HEAP_SLOT_FIELD_EFFECT = 56,
  /// The battle command menus' work (struct BattleMenuWork).
  HEAP_SLOT_BATTLE_MENU = 57,
  /// A command icon menu's work (struct IconMenuWork).
  HEAP_SLOT_ICON_MENU = 58,
  /// The Colosso finals' race work (struct ColossoRaceWork), which
  /// gColossoWork reads by its own name.
  HEAP_SLOT_COLOSSO_RACE = 59
};

#endif
