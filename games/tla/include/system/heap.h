#ifndef ALCHEMY_HEAP_H
#define ALCHEMY_HEAP_H

/// @file
/// The heap's slots.
///
/// The heap is two stacks of blocks, one in IWRAM and one in EWRAM, as in The
/// Broken Seal; the System README's "The heap" gives the allocators and the
/// lifetime rule. Every scene change resets both pools and clears every slot
/// (Heap_Initialize), so no block outlives the scene, battle or minigame that
/// took it. The Lost Age's slot routines (HeapSlot_Alloc, HeapSlot_AllocEwram,
/// HeapSlot_Release) take a slot by its byte offset into gHeapSlots, so each
/// HEAP_SLOT_ name below is that offset.

#include "static_assert.h"
#include "stddef.h"
#include "types.h"

struct BattleBackgroundWork;
struct BattleWork;
struct MapWork;
struct ActorSystemWork;
struct ScreenTransitionState;
struct RopeWork;
struct TightropeWork;

/// The heap's 64 words in IWRAM (gHeapSlots, 0x03000000): the two pools' next
/// free addresses, then each slot's block once allocated, else null.
struct HeapSlots {
  void *nextEwram;                     ///< the EWRAM pool's next free byte
  void *nextIwram;                     ///< the IWRAM pool's next free byte
  u8 *transformStack;                  ///< the 3D transform's one saved level
  void *spriteAnimations;              ///< the actor system's sprite animations
  void *sprites;                       ///< the actor system's sprite pool
  void *actors;                        ///< the 64 actors (struct Actor)
  struct ActorSystemWork *actorSystem; ///< the actor system's work
  void *worldMapAffine;                ///< the world map's affine work
  /// The field map's scrolling work, or on the world map its perspective view
  /// (struct WorldMapView).
  void *mapGrid;
  struct BattleWork *battleWork; ///< a battle's work, from Battle_Run on
  void *battleScanlines;         ///< the battle background's scanline work
  /// Unused: 640 bytes Battle_Run allocates with the battle's other blocks,
  /// which nothing reads or writes.
  void *battleUnused;
  /// The 3D camera (struct BattleCamera) of a battle, the world map or the
  /// dice games.
  void *cameraWork;
  void *unusedAfterCamera; ///< no code allocates or reads this slot
  /// A scratch buffer, usually 1 KB, that a picture is decoded into and
  /// released as soon as its tiles are in VRAM.
  void *tileScratch;
  u8 *windowWork;      ///< the window renderer's work (struct WindowWork)
  void *partyStatus;   ///< the party status window's layout
  void *glyphTransfer; ///< the buffers an icon or portrait is decoded through
  void *selectionMenu; ///< the selection screen's work
  void *unusedAfterSelectionMenu; ///< no code allocates or reads this slot
  /// Lives for one frame: wait() builds the OAM list in it, and the field
  /// sprite decoder keeps its code there between its own uses.
  void *frameScratch;
  /// A RAM copy of Render_CopyStrided, which uploads field sprite pictures to
  /// VRAM.
  void *pictureUploader;
  void *mindRead; ///< Mind Read's wave work
  /// The running battle effect's own work. The dice games keep their own work
  /// in this slot and the next two.
  void *battleEffect;
  void *battleCanvas; ///< the running battle effect's pixel canvas
  void *battleSheet;  ///< the running battle effect's tile sheet
  /// A routine run from RAM: the canvas drawing routine
  /// BattleEffect_BuildBlitter writes, or the world map's copy of
  /// Matrix_UpdateVertices.
  void *ramCode;
  void *fieldWork;     ///< a field scene's struct FieldWork
  void *paletteCycles; ///< the palette cycles
  void *weather;       ///< the running ground particles, motes or sparkles
  void *fieldBlend;    ///< the field's colour blend work
  /// The screen transition: the window edges and dissolve of a scene change.
  struct ScreenTransitionState *screenTransition;
  /// The palette fade that moves the BG and sprite palettes toward target
  /// colours (struct PaletteFade).
  void *paletteFade;
  void *unusedAfterPaletteFade;    ///< no code allocates or reads this slot
  void *lineScroll;                ///< the line scroll's work
  void *unusedAfterLineScroll;     ///< no code allocates or reads this slot
  void *fadeOverlay;               ///< the fade overlay's work
  void *battleMessage;             ///< the battle message window's work
  void *unusedAfterBattleMessage;  ///< no code allocates or reads this slot
  struct RopeWork *rope;           ///< a rope's work
  struct TightropeWork *tightrope; ///< a tightrope's work
  void *hangingVine;      ///< a hanging vine's work (struct HangingVineWork)
  void *battleEntryTimer; ///< the battle's entry transition timer, in frames
  void *titleIntro;       ///< the title intro's state
  /// The battle background's view and entry transition (32 bytes), from
  /// Battle_Run.
  struct BattleBackgroundWork *battleBackground;
  /// The dice games' sprites, or a field Psynergy's canvas map.
  u16 *canvasMap;
  void *unusedAfterCanvasMap; ///< no code allocates this slot
  /// A second routine as ramCode, released before it: always a
  /// BattleEffect_BuildBlitter routine, the second of a pair.
  void *ramCodePaired;
  void *unusedAfterRamCodePaired;   ///< no code allocates or reads this slot
  void *unusedBeforeMessageDecoder; ///< no code allocates or reads this slot
  /// The message decoder routine (Text_DecodeSymbol), copied here to run for
  /// the length of one message's decoding.
  void *messageDecoder;
  void *saveWork;     ///< the save workspace, around each save or load
  void *settingsMenu; ///< the settings menu's work
  void *unusedAfterSettingsMenu; ///< no code allocates or reads this slot
  void *enemyUnits; ///< each linked console's party, from Battle_Run
  /// The menu work shared by the field menu's screens and the shop; each takes
  /// it on opening and releases it on closing.
  void *menuWork;
  void *fieldEffectWork;         ///< the Psynergy effect's work
  void *battleMenuWork;          ///< the battle menus' struct BattleMenuWork
  void *iconMenu;                ///< a command icon menu's work
  void *trialRoadLink;           ///< the Trial Road's link work
  void *canvasSprite;            ///< a field Psynergy's canvas sprite
  void *unusedAfterCanvasSprite; ///< no code allocates or reads this slot
  void *arcEffect;               ///< a field Psynergy's arc work
  /// The Djinn the Naribwe fortune teller may hint at, 320 bytes, while she
  /// picks one (naribwe_rooms.c).
  s32 *djinniHints;
};

STATIC_ASSERT(sizeof(struct HeapSlots) == 256);
STATIC_ASSERT(offsetof(struct HeapSlots, fieldWork) == 27 * 4);
STATIC_ASSERT(offsetof(struct HeapSlots, menuWork) == 55 * 4);

/// The heap slots shared across folders, by byte offset into gHeapSlots.
enum HeapSlot {
  HEAP_SLOT_MATRIX_STACK = offsetof(struct HeapSlots, transformStack),
  HEAP_SLOT_SPRITE_ANIMATIONS = offsetof(struct HeapSlots, spriteAnimations),
  HEAP_SLOT_SPRITES = offsetof(struct HeapSlots, sprites),
  HEAP_SLOT_ACTORS = offsetof(struct HeapSlots, actors),
  HEAP_SLOT_ACTOR_SYSTEM = offsetof(struct HeapSlots, actorSystem),
  HEAP_SLOT_WORLD_MAP_AFFINE = offsetof(struct HeapSlots, worldMapAffine),
  HEAP_SLOT_MAP_GRID = offsetof(struct HeapSlots, mapGrid),
  HEAP_SLOT_BATTLE = offsetof(struct HeapSlots, battleWork),
  HEAP_SLOT_BATTLE_SCANLINES = offsetof(struct HeapSlots, battleScanlines),
  HEAP_SLOT_BATTLE_UNUSED = offsetof(struct HeapSlots, battleUnused),
  HEAP_SLOT_CAMERA = offsetof(struct HeapSlots, cameraWork),
  HEAP_SLOT_TILE_SCRATCH = offsetof(struct HeapSlots, tileScratch),
  HEAP_SLOT_WINDOW = offsetof(struct HeapSlots, windowWork),
  HEAP_SLOT_PARTY_STATUS = offsetof(struct HeapSlots, partyStatus),
  HEAP_SLOT_GLYPH_TRANSFER = offsetof(struct HeapSlots, glyphTransfer),
  HEAP_SLOT_SELECTION_MENU = offsetof(struct HeapSlots, selectionMenu),
  HEAP_SLOT_FRAME_SCRATCH = offsetof(struct HeapSlots, frameScratch),
  HEAP_SLOT_PICTURE_UPLOADER = offsetof(struct HeapSlots, pictureUploader),
  HEAP_SLOT_MIND_READ = offsetof(struct HeapSlots, mindRead),
  HEAP_SLOT_BATTLE_EFFECT = offsetof(struct HeapSlots, battleEffect),
  HEAP_SLOT_BATTLE_CANVAS = offsetof(struct HeapSlots, battleCanvas),
  HEAP_SLOT_BATTLE_SHEET = offsetof(struct HeapSlots, battleSheet),
  HEAP_SLOT_RAM_CODE = offsetof(struct HeapSlots, ramCode),
  HEAP_SLOT_FIELD = offsetof(struct HeapSlots, fieldWork),
  HEAP_SLOT_PALETTE_CYCLES = offsetof(struct HeapSlots, paletteCycles),
  HEAP_SLOT_WEATHER = offsetof(struct HeapSlots, weather),
  HEAP_SLOT_FIELD_BLEND = offsetof(struct HeapSlots, fieldBlend),
  HEAP_SLOT_SCREEN_TRANSITION = offsetof(struct HeapSlots, screenTransition),
  HEAP_SLOT_PALETTE_FADE = offsetof(struct HeapSlots, paletteFade),
  HEAP_SLOT_LINE_SCROLL = offsetof(struct HeapSlots, lineScroll),
  HEAP_SLOT_FADE_OVERLAY = offsetof(struct HeapSlots, fadeOverlay),
  HEAP_SLOT_BATTLE_MESSAGE = offsetof(struct HeapSlots, battleMessage),
  HEAP_SLOT_ROPE = offsetof(struct HeapSlots, rope),
  HEAP_SLOT_TIGHTROPE = offsetof(struct HeapSlots, tightrope),
  HEAP_SLOT_HANGING_VINE = offsetof(struct HeapSlots, hangingVine),
  HEAP_SLOT_BATTLE_ENTRY_TIMER = offsetof(struct HeapSlots, battleEntryTimer),
  HEAP_SLOT_TITLE_INTRO = offsetof(struct HeapSlots, titleIntro),
  HEAP_SLOT_BATTLE_BACKGROUND = offsetof(struct HeapSlots, battleBackground),
  HEAP_SLOT_CANVAS_MAP = offsetof(struct HeapSlots, canvasMap),
  HEAP_SLOT_RAM_CODE_PAIRED = offsetof(struct HeapSlots, ramCodePaired),
  HEAP_SLOT_MESSAGE_DECODER = offsetof(struct HeapSlots, messageDecoder),
  HEAP_SLOT_SAVE = offsetof(struct HeapSlots, saveWork),
  HEAP_SLOT_SETTINGS_MENU = offsetof(struct HeapSlots, settingsMenu),
  HEAP_SLOT_ENEMY_UNITS = offsetof(struct HeapSlots, enemyUnits),
  HEAP_SLOT_MENU = offsetof(struct HeapSlots, menuWork),
  HEAP_SLOT_FIELD_EFFECT = offsetof(struct HeapSlots, fieldEffectWork),
  HEAP_SLOT_BATTLE_MENU = offsetof(struct HeapSlots, battleMenuWork),
  HEAP_SLOT_ICON_MENU = offsetof(struct HeapSlots, iconMenu),
  HEAP_SLOT_TRIAL_ROAD_LINK = offsetof(struct HeapSlots, trialRoadLink),
  HEAP_SLOT_CANVAS_SPRITE = offsetof(struct HeapSlots, canvasSprite),
  HEAP_SLOT_ARC_EFFECT = offsetof(struct HeapSlots, arcEffect),
  HEAP_SLOT_DJINNI_HINTS = offsetof(struct HeapSlots, djinniHints)
};

#define Ram_HeapSlots                                                          \
  FIXED_ADDRESS((struct HeapSlots *), 0x03000000, gHeapSlots)

/// Heap slot `kind` of the heap's words `work`, by its byte offset (enum
/// HeapSlot): the block, or null while none is allocated.
#define HEAP_SLOT(work, kind) (*(void **)((u8 *)(work) + (kind)))

#endif
