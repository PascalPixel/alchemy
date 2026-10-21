#ifndef ALCHEMY_HEAP_STATE_H
#define ALCHEMY_HEAP_STATE_H

/// @file
/// The heap's work slots and HeapState.
///
/// The heap is two stacks of blocks, one in IWRAM and one in EWRAM; the
/// runtime README's "The heap: two stacks of blocks" gives the allocators and
/// the lifetime rule. Every scene change resets both pools and clears every
/// slot (Runtime_InitializeHeap, from the scene loop in object/scene.c), so
/// no block outlives the scene, battle or minigame that took it.

#include "types.h"
#include "static_assert.h"

/// The heap's 64 words in IWRAM (0x03001e50), seen as union HeapState. The
/// work globals named in system/iwram.s (gBattleWork, gCameraWork,
/// gEventWork and others) are single slots of this table.
extern void *gWorkSlot[64];

/// Slot n's block, or null before it is allocated.
// Reached through the slot's own address, adjacent slots by offsets from it;
// gWorkSlot[n] addresses from the table's start instead. Each site keeps the
// form it matches with (MATCHING.md).
#define WORK_SLOT(n) (*(gWorkSlot + (n)))

/// The heap's 64 work slots, the first two holding the next free EWRAM and
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

/// The work slots shared across folders, by index into gWorkSlot. Releasing a
/// slot (Runtime_ReleaseHeapBlock) also releases every block taken after it in
/// the same pool.
enum HeapSlot {
  HEAP_SLOT_TRANSFORM_STACK = 2, ///< The 3D transform's one saved level.
  /// The animation layer records (struct AnimationEntry) of the object
  /// system's sprites (gAnimationObjects).
  HEAP_SLOT_ANIMATION_LAYERS = 3,
  /// The object system's sprite pool (struct AnimationObject).
  HEAP_SLOT_ANIMATION_SPRITES = 4,
  /// The 64 field objects of the object pool (struct ObjectRuntime).
  HEAP_SLOT_OBJECT_POOL = 5,
  /// The object system's work (struct ObjectSystemWork, gMenuCtrlWork), taken
  /// by ObjectSystem_Initialize. The item, Psynergy, Status, Djinn and debug
  /// menus and the world map effect set its suspended field while they are
  /// open, so sprites that are not held are not drawn and free their VRAM.
  HEAP_SLOT_MENU_CONTROL = 6,
  /// The world map's frame work (struct MapFrameWork).
  HEAP_SLOT_MAP_FRAME = 7,
  /// The field map's scrolling work (struct MapScrollWork), or on the world
  /// map its perspective view (struct PerspectiveWork); gMapWork.
  HEAP_SLOT_MAP = 8,
  /// A battle's struct BattleWork (gBattleWork), from Battle_RunEncounter to
  /// the next scene change.
  HEAP_SLOT_BATTLE = 9,
  /// The battle background's scanline work (struct BattleAffineHdma).
  HEAP_SLOT_BATTLE_SCANLINES = 10,
  /// The 3D camera (struct BattleCamera, gCameraWork) of a battle, the world
  /// map or Lucky Dice.
  HEAP_SLOT_CAMERA = 12,
  /// A scratch buffer, usually 1 KB, that a picture is decoded into and
  /// released as soon as its tiles are in VRAM.
  HEAP_SLOT_TILE_SCRATCH = 14,
  /// The window renderer's work (struct UiRenderWork, gWindowWork).
  HEAP_SLOT_WINDOW = 15,
  /// The buffers an icon or portrait is decoded through (GlyphTransfer).
  HEAP_SLOT_GLYPH_TRANSFER = 17,
  /// The selection screen's work (struct SelectionScreen).
  HEAP_SLOT_SELECTION = 18,
  /// The work of Menu_AllocateSelectionWorkspace, which nothing calls.
  HEAP_SLOT_SELECTION_WORKSPACE = 19,
  /// The option screen's work (struct WorkspaceWork, gSelectionWork).
  HEAP_SLOT_OPTIONS = 20,
  /// Mind Read's wave work (struct WaveFxWork).
  HEAP_SLOT_MIND_READ = 22,
  /// A field scene's struct FieldWork, which scenes read as struct EventWork
  /// (gEventWork); Field_RunScene takes it and frees it on the way out.
  HEAP_SLOT_EVENT = 27,
  /// The palette cycling queue (struct PaletteSnapshotWork).
  HEAP_SLOT_PALETTE_CYCLES = 28,
  /// The running ground particles, motes or sparkles (struct
  /// GroundParticleWork).
  HEAP_SLOT_GROUND_PARTICLES = 29,
  /// The field's colour blend work (gFieldBlendWork): the struct
  /// FieldBlendWork the storm's lightning and BattleFx_StartBufferBlend
  /// allocate.
  HEAP_SLOT_FIELD_BLEND = 30,
  /// The display transition (struct DisplayTransitionState,
  /// gDisplayTransition): the window edges and dissolve of a scene change.
  HEAP_SLOT_TRANSITION = 31,
  /// The colour buffer (gPaletteBufferWork) that moves the BG and OBJ palettes
  /// toward target colours: the struct BattleEffectBuffers
  /// BattleEffect_InitializeBuffers allocates on scene load, or the title
  /// intro's struct PaletteFade.
  HEAP_SLOT_PALETTE_BUFFER = 32,
  /// The wavy background's work (struct DisplayScrollWork).
  HEAP_SLOT_WAVY_BACKGROUND = 34,
  /// The fade overlay's work (struct FadeOverlayWork).
  HEAP_SLOT_FADE_OVERLAY = 36,
  /// The battle message display (struct BattleDisplayWork).
  HEAP_SLOT_BATTLE_DISPLAY = 37,
  /// The running battle effect's own work (gBattleFxWork). Lucky Dice keeps
  /// its own work in this slot and the next two.
  HEAP_SLOT_BATTLE_EFFECT = 39,
  /// The running battle effect's pixel canvas.
  HEAP_SLOT_BATTLE_CANVAS = 40,
  /// The running battle effect's tile sheet, or its set of mote cells.
  HEAP_SLOT_BATTLE_SHEET = 41,
  /// The battle's entry transition timer, in frames (gDisp).
  HEAP_SLOT_BATTLE_ENTRY_TIMER = 42,
  /// The title intro's state (struct IntroWork).
  HEAP_SLOT_TITLE_INTRO = 43,
  /// The battle background's view and entry transition (32 bytes), from
  /// Battle_RunEncounter.
  HEAP_SLOT_BATTLE_BACKGROUND = 44,
  /// A canvas drawing routine written by BattleFx_BuildBlitter and called as a
  /// DrawRectangle; an effect releases it before its canvas.
  HEAP_SLOT_BLITTER = 46,
  /// A second routine as HEAP_SLOT_BLITTER, released before it.
  HEAP_SLOT_BLITTER_ALTERNATE = 47,
  /// Copied field row drawer or window tile decoder, released after each call.
  HEAP_SLOT_TEMP_RENDERER = 49,
  /// The save workspace (struct SaveWorkspace), from
  /// SaveState_InitializeWorkspace to SaveState_ReleaseWorkspace around each
  /// save, load, copy or delete and each scan of the files, as the title
  /// menu's (SaveState_ScanRecordFlags).
  HEAP_SLOT_SAVE = 51,
  /// Lives for one frame: WaitFrames builds the OAM list in it, and the field
  /// sprite decoder keeps its code there between its own uses.
  HEAP_SLOT_FRAME_SCRATCH = 52,
  /// A RAM copy of Tile_CopyStrided, which uploads field sprite frames to
  /// VRAM; loaded with the animation pools.
  HEAP_SLOT_FRAME_UPLOADER = 53,
  /// Each linked console's party (LINK_OWNER_STATES_SIZE bytes), from the
  /// link lobby and Battle_RunEncounter.
  HEAP_SLOT_LINK_OWNER_STATES = 54,
  /// The menu work (MENU_WORK_SIZE bytes) shared by the main menu's screens
  /// and the shop; each takes it on opening and releases it on closing. The
  /// save screen takes it for its sprite work (struct ShopRuntime).
  HEAP_SLOT_MENU = 55,
  /// The Psynergy effect's work (struct BattleFxScene, gEffectWork). A field
  /// Psynergy holds it from BattleFx_LoadActionEffectResources to
  /// BattleEffect_CleanupSceneObjects, a Djinni's joining from
  /// BattleFx_InitializeSlots to BattleFx_ClearActiveSlotsAndScheduleUpdates.
  HEAP_SLOT_FX = 56,
  /// The battle command menus' work (struct BattleMenuWork).
  HEAP_SLOT_BATTLE_COMMANDS = 57,
  /// A command icon menu's work (struct ResourceMenuWork).
  HEAP_SLOT_ICON_MENU = 58,
  /// The Colosso finals' race work (struct ColossoRaceWork), which
  /// gColossoWork reads by its own name.
  HEAP_SLOT_COLOSSO_RACE = 59
};

#endif
