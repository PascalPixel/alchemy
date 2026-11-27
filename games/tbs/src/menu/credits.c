// The credits at the end of the game: the staff names, drawn line by line into a ring of sprite
// tiles, scroll up over battle backgrounds that crossfade behind them.

#include "menu/credits.h"
#include "flags.h"
#include "resource_ids.h"
#include "battle/battle_state.h"
#include "field/map_grid_scroll.h"
#include "sdk/flash.h"
#include "sdk/flash_data.h"
#include "graphics/graphics.h"

// Credits text uses six 32-pixel sprites across each 192-pixel line.
#define CREDITS_LINE_PIXELS       192
#define CREDITS_VISIBLE_ROWS      16
#define CREDITS_COLUMNS           6
#define CREDITS_RING_ROWS         32
#define CREDITS_LINE_TILES        24 // A line's 4bpp tiles, 192 / 8.
#define CREDITS_RING_TILES        (CREDITS_RING_ROWS * CREDITS_LINE_TILES)
#define CREDITS_RING_BYTES        (CREDITS_RING_TILES * TILE_SIZE_4BPP)
// A line's raster: nine rows of 256 one-byte pixels, the ninth for the shadow below the eighth.
#define CREDITS_BUFFER_BYTES      2304
#define CREDITS_RASTER_STRIDE     256
// OAM entries 0 to 23: the opaque strips above and below the text.
#define CREDITS_BORDER_SPRITES    24
// The whole OAM: the borders, 96 text sprites and 8 parked off screen.
#define CREDITS_OAM_ENTRIES       128
// The border sprites' one 32x16 picture, solid in colour 1, after the ring.
#define CREDITS_BORDER_TILE_BYTES (8 * TILE_SIZE_4BPP)
// The first tile of the ring line a scroll line is drawn into, CREDITS_VISIBLE_ROWS below the top.
#define CREDITS_RING_LINE_TILE(line)                                                               \
    ((((line) + CREDITS_VISIBLE_ROWS) & (CREDITS_RING_ROWS - 1)) * CREDITS_LINE_TILES)
// The text sprites' rows start below the top strip, and their 192 pixels stand in the middle of the
// screen.
#define CREDITS_TEXT_TOP                16
#define CREDITS_TEXT_LEFT               ((DISPLAY_WIDTH - CREDITS_LINE_PIXELS) / 2)
// The x and the y at which the eight spare sprites wait, below the screen.
#define CREDITS_PARKED                  192
// The colour a line's letters are drawn in, and the colour of the shadow a pixel below and to the
// right of each, the strips' black.
#define CREDITS_TEXT_COLOR              15
#define CREDITS_SHADOW_COLOR            1

// A battle background resource begins with its BATTLE_BACKGROUND_COLORS colours, the palette half
// it takes; its packed tiles follow.
#define BATTLE_BACKGROUND_PALETTE_BYTES (BATTLE_BACKGROUND_COLORS * 2)
// A battle background's tiles of 256 colours, 256 by 120 pixels.
#define BATTLE_BACKGROUND_TILES         480
#define BATTLE_BACKGROUND_TILE_BYTES    (BATTLE_BACKGROUND_TILES * TILE_SIZE_8BPP)

// The two pictures' screen blocks, each holding the same tilemap: BG3's, over character block 0,
// and BG2's, over character block 2.
#define CREDITS_BG3_SCREEN_BLOCK        15
#define CREDITS_BG2_SCREEN_BLOCK        31
// A tilemap row's words.
#define TILEMAP_ROW_WORDS               (BG_SCREEN_WIDTH / 2)
// The tile the margins show: the character block's last, which lies in the screen block's own
// zeroed tail and so is clear.
#define CREDITS_BLANK_TILE              511

// Where mnCreditsDrawLine starts a line in its 192-pixel row.
typedef enum CREDITS_ALIGNMENT
{
    CREDITS_ALIGN_LEFT,
    CREDITS_ALIGN_CENTER,
    CREDITS_ALIGN_RIGHT,
} CREDITS_ALIGNMENT;

// Fills a battle background's 256-colour tiles with colour 1 of its half, black once its palette is
// cleared and opaque, for the next crossfade, and clears the half's palette colours. alternate is 0
// for the first half (character block 0, the low palette half), 1 for the second (character block
// 2, the high palette half).
void mnCreditsClearBuffer(Sint32 alternate)
{
    Uint32          fillColor;
    void           *tileDest;
    void           *paletteDest;
    volatile Uint32 dmaFill;

    if (alternate == 0)
    {
        fillColor   = PIXELS_8BPP(1);
        tileDest    = BG_CHAR_BLOCK(0);
        paletteDest = (void *)BG_PLTT;
    }
    else
    {
        fillColor   = PIXELS_8BPP(BATTLE_BACKGROUND_COLORS + 1);
        tileDest    = BG_CHAR_BLOCK(2);
        paletteDest = (void *)&BG_PLTT_COLOR(BATTLE_BACKGROUND_COLORS / PALETTE_COLORS, 0);
    }
    DMA_FILL32_CELL(dmaFill, fillColor, tileDest, BATTLE_BACKGROUND_TILE_BYTES);
    DMA_FILL32_CELL(dmaFill, 0, paletteDest, BATTLE_BACKGROUND_PALETTE_BYTES);
}

typedef Sint32 (*PACKED_DECODER)(const Uint8 *source, void *destination, Uint32 fill);

// Battle backgrounds shown in the credits; 0 clears the display.
// clang-format off
const Uint32 CreditsBackgrounds[] = {
    (Uint32)RESOURCE_VALE_STORM_BACKDROP,
    (Uint32)RESOURCE_SOL_SANCTUM_BACKDROP,
    (Uint32)RESOURCE_GRASSLAND_BACKDROP,
    (Uint32)RESOURCE_ROOMS_BACKDROP,
    (Uint32)RESOURCE_FOREST_BACKDROP,
    (Uint32)RESOURCE_GOMA_CAVE_BACKDROP,
    (Uint32)RESOURCE_KOLIMA_FOREST_BACKDROP,
    (Uint32)RESOURCE_TRET_TREE_ROOMS_BACKDROP,
    (Uint32)RESOURCE_TRET_TREE_BRANCH_BACKDROP,
    (Uint32)RESOURCE_SNOWFIELD_BACKDROP,
    (Uint32)RESOURCE_MERCURY_LIGHTHOUSE_BACKDROP,
    (Uint32)RESOURCE_MERCURY_LIGHTHOUSE_AERIE_BACKDROP,
    (Uint32)RESOURCE_FUCHIN_FALLS_CAVE_BACKDROP,
    (Uint32)RESOURCE_MOGALL_FOREST_BACKDROP,
    (Uint32)RESOURCE_ALTIN_PEAK_BACKDROP,
    (Uint32)RESOURCE_LAMAKAN_DESERT_BACKDROP,
    (Uint32)RESOURCE_TOLBI_SHIP_DECK_BACKDROP,
    (Uint32)RESOURCE_TOLBI_SHIP_ROOMS_BACKDROP,
    (Uint32)RESOURCE_BEACH_BACKDROP,
    (Uint32)RESOURCE_COLOSSO_BACKDROP,
    (Uint32)RESOURCE_ALTMILLER_CAVE_BACKDROP,
    (Uint32)RESOURCE_SUHALLA_DESERT_BACKDROP,
    (Uint32)RESOURCE_CROSSBONE_ISLE_CAVE_BACKDROP,
    (Uint32)RESOURCE_PIRATE_SHIP_BACKDROP,
    (Uint32)RESOURCE_SUHALLA_GATE_BACKDROP,
    (Uint32)RESOURCE_TUNNEL_RUINS_BACKDROP,
    (Uint32)RESOURCE_BABI_LIGHTHOUSE_BACKDROP,
    (Uint32)RESOURCE_VENUS_LIGHTHOUSE_BACKDROP,
    (Uint32)RESOURCE_VENUS_LIGHTHOUSE_AERIE_BACKDROP,
    (Uint32)RESOURCE_VENUS_LIGHTHOUSE_AERIE_STORM_BACKDROP,
    (Uint32)RESOURCE_DUSK_CLOUDS_BACKDROP,
    (Uint32)RESOURCE_VIOLET_SKY_BACKDROP,
    0               // Clear
};
// clang-format on

// Loads a credits battle background into one of the two tile/palette halves.
void           mnCreditsLoadBackground(Uint32 resourceId, Sint32 alternate)
{
    const Uint8 *resourceData;
    Uint32       paletteDest;
    Uint32       paletteOffset;
    void        *tileDest;

    if (resourceId == 0)
    {
        mnCreditsClearBuffer(alternate);
        return;
    }
    resourceData = syResourceGetTableEntry(resourceId);
    if (alternate == 0)
    {
        paletteOffset = 0;
        tileDest      = BG_CHAR_BLOCK(0);
        paletteDest   = (Uint32)BG_PLTT;
    }
    else
    {
        paletteOffset = PIXELS_8BPP(BATTLE_BACKGROUND_COLORS);
        tileDest      = BG_CHAR_BLOCK(2);
        paletteDest   = (Uint32)&BG_PLTT_COLOR(BATTLE_BACKGROUND_COLORS / PALETTE_COLORS, 0);
    }
    // Offset every pixel into the selected palette half so both pictures can remain visible
    // throughout a crossfade.
    RAM_CODE_LOAD_SLOT(
        HEAP_SLOT_RAM_CODE_TEMP, mnCreditsDecodeBackground, CreditsDecodeBackgroundCodeSize);
    ((PACKED_DECODER)HeapSlots[HEAP_SLOT_RAM_CODE_TEMP])(
        resourceData + BATTLE_BACKGROUND_PALETTE_BYTES, tileDest, paletteOffset);
    syHeapSlotRelease(HEAP_SLOT_RAM_CODE_TEMP);

    // Queue palette RAM writes for VBlank rather than transferring colours during the active
    // display.
    IO_WRITE_QUEUE_PUSH(
        paletteDest, (Uint32)resourceData, DMA_COPY_WORDS(BATTLE_BACKGROUND_PALETTE_BYTES / 4));
}

// Builds a battle background's tilemap: two blank rows, its tiles in order over fifteen rows, three
// blank rows, then a zeroed tail.
void mnCreditsBuildBackgroundTilemap(Uint32 *dest)
{
    Sint32 i;
    Uint32 blankTiles = MAP_ENTRY_PAIR(CREDITS_BLANK_TILE, CREDITS_BLANK_TILE);
    Uint32 tilePair   = MAP_ENTRY_PAIR(0, 1);

    // Pad above and below the picture so vertical wrapping stays blank.
    for (i = 0; i < 2 * TILEMAP_ROW_WORDS; i++)
        *dest++ = blankTiles;
    for (i = 0; i < BATTLE_BACKGROUND_TILES / 2; i++)
    {
        *dest++   = tilePair;
        tilePair += MAP_ENTRY_PAIR(2, 2);
    }
    for (i = 0; i < 3 * TILEMAP_ROW_WORDS; i++)
        *dest++ = blankTiles;
    tilePair = 0;
    for (i = 0; i < 12 * TILEMAP_ROW_WORDS; i++)
        *dest++ = tilePair;
}

// A VBlank event of the credits: drifts both pictures together, keeping their horizontal offsets
// aligned throughout the crossfade.
void mnCreditsScrollBackgrounds(void)
{
    if ((FrameCount & 3) == 0)
    {
        Uint32 decrement   = (Uint16)-1;

        BgScrollWork[2].x += decrement;
        BgScrollWork[3].x += decrement;
    }
}

// Crossfades the credits' battle backgrounds through alternating tile banks while the staff names
// scroll over it, then restores the display for the following screen.
Sint32 mnCreditsRun(void)
{
    Uint32 slide;
    Sint32 alpha;

    OamCopyEnabled        = 0;
    CpuLoadDisplayEnabled = 0;
    DebugTextEnabled      = 0;
    IdleSleepEnabled      = 0;
    syInitEvent();
    sySetEvent(mnCreditsScrollBackgrounds, EVENT_PASS_VBLANK);
    // Each layer owns its map, tiles and palette half.
    IO_WRITE16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
    mnCreditsBuildBackgroundTilemap((Uint32 *)BG_SCREEN_BLOCK(CREDITS_BG3_SCREEN_BLOCK));
    mnCreditsBuildBackgroundTilemap((Uint32 *)BG_SCREEN_BLOCK(CREDITS_BG2_SCREEN_BLOCK));
    mnCreditsClearBuffer(0);
    mnCreditsClearBuffer(1);
    IO_WRITE16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_256_COLORS
                   | BGCNT_SCREEN_BASE(CREDITS_BG2_SCREEN_BLOCK),
               &REG_BG2CNT);
    IO_WRITE16(BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(CREDITS_BG3_SCREEN_BLOCK),
               &REG_BG3CNT);
    IO_WRITE16(DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON, &REG_DISPCNT);
    IO_WRITE16(BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_BD,
               &REG_BLDCNT);
    // Give the names a five-second lead before the first picture.
    mnCreditsInitTextSprites();
    sySynch(300);
    // Alternate the hidden layer after each 64-tick crossfade.
    for (slide = 0; slide < sizeof(CreditsBackgrounds) / sizeof(CreditsBackgrounds[0]); slide++)
    {
        mnCreditsLoadBackground(CreditsBackgrounds[slide], (slide & 1) ^ 1);
        // Fade the new half in: an even slide went to BG2, the first target (BLDALPHA bits 4..0),
        // an odd one to BG3, the second (bits 12..8).
        for (alpha = 1; alpha <= BLEND_WEIGHT_MAX; alpha++)
        {
            if (slide & 1)
                IO_WRITE16((alpha << 8) | (BLEND_WEIGHT_MAX - alpha), &REG_BLDALPHA);
            else
                IO_WRITE16(((BLEND_WEIGHT_MAX - alpha) << 8) | alpha, &REG_BLDALPHA);
            sySynch(4);
        }
        sySynch(267);
    }
    IO_WRITE16(0, &REG_BLDCNT);
    IO_WRITE16(DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON, &REG_DISPCNT);
    syWindowLoadGraphics();
    syPrintClear();
    OamCopyEnabled = 1;
    return 0;
}

// One eight-byte OAM entry in the credits' private sprite buffer.
typedef struct CREDITS_OAM_ENTRY
{
    // Attribute 0 in the low half, attribute 1 in the high half.
    Uint32 attr01;
    // Attribute 2 in the low half; the unused high half stays zero.
    Uint32 attr2;
} CREDITS_OAM_ENTRY;

STATIC_ASSERT(sizeof(CREDITS_OAM_ENTRY) == 8);

// Credits scroll position in pixels, in the low half of WaitForFlashWriteWork.
extern Uint16 CreditsScrollPosition;

// ewram.s reserves four bytes for each cell. The credits replace the event table and borrow them
// while flash operations are inactive: a sprite-buffer pointer, the last rendered pixel position,
// and a zero/-1 end-of-text latch. Borrowing them is harmless because the SDK's flash library sets
// them again before it reads them: IdentifyFlash, which syBupInitState runs before each use of the
// save, writes FlashWork, ProgramFlashSectorWork and WaitForFlashWriteWork, and each program
// function sets FlashNumRemainingBytes. UB: the casts alias the flash declarations and store a
// pointer through a declared Uint16. UBFIX reads each cell by its credits name instead, which
// ewram.s gives it beside its flash name.
#if FIX_TYPE_PUNNING
// The sprite buffer, in FlashNumRemainingBytes.
extern CREDITS_OAM_ENTRY *CreditsScrollObjects;
// The last line drawn, as a pixel position, in the low half of FlashWork.
extern Sint16             CreditsLastLinePosition;
// 0 until the text ends, then -1, in the low half of ProgramFlashSectorWork.
extern Sint16             CreditsEndOfText;

// The three cells, as the credits read them.
#    define SCROLL_OBJECTS             CreditsScrollObjects
#    define CREDITS_LAST_LINE_POSITION CreditsLastLinePosition
#    define CREDITS_END_OF_TEXT        CreditsEndOfText
#else
#    define SCROLL_OBJECTS             (*(CREDITS_OAM_ENTRY **)&FlashNumRemainingBytes)
#    define CREDITS_LAST_LINE_POSITION (*(Sint16 *)&FlashWork)
#    define CREDITS_END_OF_TEXT        (*(Sint16 *)&ProgramFlashSectorWork)
#endif

static const Uint8 BlankCreditLine[] = "";

// The staff roll's headings whose wording the Japanese edition and the others spell differently.
#if !CREDITS_REVISED_FOR_LOCALIZATION
#    define CREDITS_STAFF_TITLE            "OUGON NO TAIYO STAFF"
#    define CREDITS_FACES_SUBTITLE         "& Faces"
#    define CREDITS_SOUND_DIRECTION_LABEL  "Sound Direction"
#    define CREDITS_GRAPHICS_SUPPORT_LABEL "Graphic Support"
#    define CREDITS_ARTWORK_LABEL          "Artwork Design"
#else
#    define CREDITS_STAFF_TITLE            "GOLDEN SUN STAFF CREDITS"
#    define CREDITS_FACES_SUBTITLE         "  & Faces"
#    define CREDITS_SOUND_DIRECTION_LABEL  "Sound Director"
#    define CREDITS_GRAPHICS_SUPPORT_LABEL "Graphic Supports"
#    define CREDITS_ARTWORK_LABEL          "Artwork"
#endif

// Credits text, one row per line, terminated by 0. A line is 8 pixels of scroll, 32 ticks; the text
// is drawn with CreditsFont, ASCII 32 to 127.
// clang-format off
const void *CreditsLines[] = {
    BlankCreditLine,
    CREDITS_STAFF_TITLE,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Game Planning",
    BlankCreditLine,
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Scenario",
    BlankCreditLine,
    "Hiroyuki Takahashi",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Director",
    BlankCreditLine,
    "Shugo Takahashi",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Assistant Director",
    BlankCreditLine,
    "Yusuke Sugimoto",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Game Design",
    BlankCreditLine,
    "Yusuke Sugimoto",
    "Shuji Shimizu",
    "Ayumu Shindo",
    "Kentaro Sakou",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Programming",
    BlankCreditLine,
    "System",
    BlankCreditLine,
    "Yasuhiro Taguchi",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Battle",
    BlankCreditLine,
    "Haruki Kodera",
    "Toru Takamatsu",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Field",
    BlankCreditLine,
    "Kaoru Shimada",
    "Hiroyuki Ichikawa",
    "Akiko Sato",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "User Interface",
    BlankCreditLine,
    "Yutaka Yamamoto",
    "Yasushi Kazawa",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Support",
    BlankCreditLine,
    "Kenji Numaya",
    "P. E. Jareth Hein",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Graphic Arts",
    BlankCreditLine,
    "Character Design",
    BlankCreditLine,
    "Shin Yamanouchi",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Monster Design",
    BlankCreditLine,
    "Hiroshi Kajiyama",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Battle Characters",
    BlankCreditLine,
    "Junko Nakamura",
    "Kaori Matsunaga",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Battle Effects",
    CREDITS_FACES_SUBTITLE,
    BlankCreditLine,
    "Junichi Ochiai",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Battle Backgrounds",
    BlankCreditLine,
    "Hiroto Nakashima",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Maps",
    BlankCreditLine,
    "Masayuki Hashimoto",
    "Mitsumasa Muraishi",
    "Mitsuru Fukumoto",
    "Masashi Mikami",
    "Akihito Kimura",
    "Shinichiro Sugimoto",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Map Characters",
    BlankCreditLine,
    "Mieko Koguchi",
    "Kanako Horiguchi",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Graphics Direction",
    BlankCreditLine,
    "Fumihide Aoki",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Staff Coordination",
    BlankCreditLine,
    "Akiko Sato",
    "Kenji Numaya",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Music",
    BlankCreditLine,
    "Motoi Sakuraba",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Sound Effects",
    BlankCreditLine,
    "Hiroyuki Hamada",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    CREDITS_SOUND_DIRECTION_LABEL,
    BlankCreditLine,
    "Masaaki Uno",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Illustration",
    BlankCreditLine,
    "Shin Yamanouchi",
    "Masanori Sato",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    CREDITS_GRAPHICS_SUPPORT_LABEL,
    BlankCreditLine,
    "Yoichi Kotabe",
    "Masanori Sato",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    CREDITS_ARTWORK_LABEL,
    BlankCreditLine,
    "Yoshitaka Nishikawa",
    "Mamoru Sakurai",
    "Nanako Kinoshita",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
#if CREDITS_REVISED_FOR_LOCALIZATION
    "Coordination",
    BlankCreditLine,
    "Masaaki Uno",
    "Toshiharu Izuno",
    "Atsushi Okada",
    "Kenichi Nishida",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
#endif
#if !CREDITS_REVISED_FOR_LOCALIZATION
    "Public Relations",
    BlankCreditLine,
    "Mitsuaki Hagishima",
    "Asako Haruhana",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Coordination",
    BlankCreditLine,
    "Masaaki Uno",
    "Toshiharu Izuno",
    "Kenichi Nishida",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Special Thanks to",
    "Super Mario Club Staff",
    BlankCreditLine,
    "Tomohiko Komada",
    "Nobuaki Maeda",
    "Naoki Inoue",
    "Keisuke Moriyasu",
    "Kei Nadabe",
    "Syunsuke Nose",
    "Keiichiro Nishimoto",
    "Toshimasa Minegishi",
    "Hiroyuki Murata",
    "Hisashi Takizaki",
    "Satoru Uno",
    "Akira Nakatsumi",
    "Takeshi Morishima",
    "Yoshikazu Tanaka",
    "Haruka Kato",
    "Kazuyo Ikeda",
    "Yayoi Yagi",
    "Katsunori Masutani",
    "Takafumi Harano",
    "Shinya Tashiro",
    "Tomohiro Nishigaki",
    "Natsuko Shigeta",
    "Takahisa Ohfuji",
    "Masaru Ikeshita",
    "Masaharu Shima",
    "Kouichi Osaka",
    "Naoki Tsuta",
    "Yoshihiro Muramatsu",
    "Tsutomu Mukai",
    "Naotaka Matsumoto",
    "Naoki Takami",
    "Junya Hanai",
    "Kouzou Uchida",
    "Kouji Okamoto",
    "Tomoko Yuasa",
    "Youichiro Ushijima",
    "Koutaro Haneda",
    "Yukio Homma",
    "Kotoe Arihara",
    "Yuji Yamashita",
    "Syogo Tachibana",
    "Mitsuhiro Seno",
    "Takehiko Asaka",
    "Masato Mizuta",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Produced by",
    BlankCreditLine,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Executive Producer",
    BlankCreditLine,
    "Hiroshi Yamauchi",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Copyrights of Game, Scenario",
    "and Program reserved by",
    BlankCreditLine,
    "NINTENDO Co.,Ltd.",
    "and",
    "CAMELOT Co.,Ltd.",
    BlankCreditLine,
    "Copyrights of All Characters",
    "and Music reserved by",
    BlankCreditLine,
    "NINTENDO Co.,Ltd.",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    0
#elif LANGUAGE == LANGUAGE_ENGLISH
    BlankCreditLine,
    "North American",
    " Localization",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Translation",
    " Coordination",
    BlankCreditLine,
    "Bill Trinen",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Writing",
    " & Editing",
    BlankCreditLine,
    "Richard Amtower",
    "Nate Bihldorf",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Additional",
    " Localization",
    BlankCreditLine,
    "Tim O'Leary",
    "Scot Ritchey",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Localization",
    " Management",
    BlankCreditLine,
    "Leslie Swan",
    "Jeff Miller",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Testing &",
    " Debugging",
    BlankCreditLine,
    "Michael Kelbaugh",
    "Todd Buechele",
    "NOA Product Testing",
    " Department",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Special Thanks to",
    BlankCreditLine,
    "Motoyasu Tojima",
    "Mike Fukuda",
    BlankCreditLine,
    " and",
    BlankCreditLine,
    BlankCreditLine,
    "Minoru Arakawa",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Producer",
    BlankCreditLine,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Executive Producer",
    BlankCreditLine,
    "Hiroshi Yamauchi",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Copyrights of Game, Scenario,",
    "and Program reserved by",
    BlankCreditLine,
    "NINTENDO and CAMELOT.",
    BlankCreditLine,
    "Copyrights of all characters",
    "and music reserved by",
    BlankCreditLine,
    "NINTENDO.",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    0
#else
    // European releases share this order, with local staff and publisher rows.
    "North American",
    " Localization",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Translation",
    " Coordination",
    BlankCreditLine,
    "Bill Trinen",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Writing",
    " & Editing",
    BlankCreditLine,
    "Richard Amtower",
    "Nate Bihldorf",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Additional   ",
    " Localization",
    BlankCreditLine,
    "Tim O'Leary",
    "Scot Ritchey",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Localization",
    " Management ",
    BlankCreditLine,
    "Leslie Swan",
    "Jeff Miller",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Localization & ",
    " Testing Europe",
    BlankCreditLine,
    "Andy Fey",
    "Kai Zeh",
    "Tanja Baar",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
#if LANGUAGE == LANGUAGE_GERMAN
    "German Translation",
    BlankCreditLine,
    "Micky Auer",
    "Martina Deimel",
#elif LANGUAGE == LANGUAGE_SPANISH
    "Spanish Translation",
    BlankCreditLine,
    "Carlos Montilla",
    "Miguel Lopez",
#elif LANGUAGE == LANGUAGE_FRENCH
    "French Translation",
    BlankCreditLine,
    "Daniel Charbit",
    "Steven Arhan",
#elif LANGUAGE == LANGUAGE_ITALIAN
    "Italian Translation",
    BlankCreditLine,
    "Andrea Salvi",
    "Calimero Tiiijun",
#endif
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
#if LANGUAGE == LANGUAGE_GERMAN
    "German Testing",
    BlankCreditLine,
    "Maurice Tisdale",
    "Andreas Dietz",
    "Rudolf Schnitzer",
    "Wolfgang Weber",
    "Frank Heuchle",
    "Zoran Tasevski",
#elif LANGUAGE == LANGUAGE_SPANISH
    "Spanish Testing",
    BlankCreditLine,
    "Maurice Tisdale",
    "Andreas Dietz",
    "Luis Martinez",
    "Jose Ramon Diaz",
    "Ana Paris",
    "Monica Gomez",
#elif LANGUAGE == LANGUAGE_FRENCH
    "French Testing",
    BlankCreditLine,
    "Maurice Tisdale",
    "Peter Ehlert",
    "Emeline Condy",
    "Pascal Di Pasquale",
    "Kamel Chelirem",
    "Sebastien Aubre",
    "Michael Troger",
#elif LANGUAGE == LANGUAGE_ITALIAN
    "Italian Testing",
    BlankCreditLine,
    "Maurice Tisdale",
    "Andreas Dietz",
    "Sonia Trepiccioni",
    "Michele Liuzzi",
    "Luca Riccioni",
    "Andrea De Benedetto",
#endif
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Special Thanks to",
    BlankCreditLine,
    "Motoyasu Tojima",
    "Takako Morita",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Producer",
    BlankCreditLine,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Executive Producer",
    BlankCreditLine,
    "Hiroshi Yamauchi",
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    "Copyrights of Game, Scenario,",
    "and Program reserved by",
    BlankCreditLine,
    "NINTENDO and CAMELOT.",
    BlankCreditLine,
    "Copyrights of all characters",
    "and music reserved by",
    BlankCreditLine,
#if LANGUAGE != LANGUAGE_FRENCH
    "NINTENDO.",
#endif
    BlankCreditLine,
    BlankCreditLine,
    BlankCreditLine,
    0
#endif
};
// clang-format on

// The staff credits font's advance widths in pixels, character codes 32 to 127, beside the font
// (font.png).
// clang-format off
const Uint8 CreditsGlyphWidths[96] = {
    7, 4, 7, 7, 8, 8, 8, 4, 5, 5, 5, 6, 4, 6, 4, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 4, 5, 6, 5, 7,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 5, 6, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 5, 6, 5, 7, 7,
    6, 7, 7, 7, 7, 7, 5, 7, 7, 4, 4, 6, 4, 8, 7, 7,
    7, 7, 7, 7, 5, 7, 6, 8, 6, 6, 6, 5, 4, 5, 7, 1,
};
// clang-format on

// A credits line has been drawn, so its scratch buffer holds the shadow row to carry.
#define FLAG_CREDITS_LINE_DRAWN FLAG_SCENE_TEMP_00

// A VBlank event added by mnCreditsInitTextSprites: maps the visible sixteen rows onto the tile
// ring and uploads their sprites.
void mnCreditsUpdateTextSprites(void)
{
    Uint32  frame            = CreditsScrollPosition;
    Uint32  scrollWithinTile = frame & 7;
    Sint32  tileIndex        = (((Sint16)frame / 8) & (CREDITS_RING_ROWS - 1)) * CREDITS_LINE_TILES;
    Uint32 *oamWords         = (Uint32 *)(SCROLL_OBJECTS + CREDITS_BORDER_SPRITES);
    Sint32  row;

    // Sixteen rows of six 32x8 sprites from CREDITS_TEXT_TOP and CREDITS_TEXT_LEFT, raised by the
    // scroll within a line, showing the ring from the line at the top.
    for (row = 0; row < CREDITS_VISIBLE_ROWS; row++)
    {
        Sint32 column;

        for (column = 0; column < CREDITS_COLUMNS; column++)
        {
            *oamWords++ = (row * 8 + CREDITS_TEXT_TOP - scrollWithinTile)
                        | OBJ_X(column * 32 + CREDITS_TEXT_LEFT) | OBJ_SIZE_32X8;
            *oamWords++  = tileIndex;
            tileIndex   += 4;
            if (tileIndex == CREDITS_RING_TILES)
                tileIndex = 0;
        }
    }
    syDMAXCopy(SCROLL_OBJECTS, OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
    if (CREDITS_END_OF_TEXT == 0 && (FrameCount & 3) == 0)
        CreditsScrollPosition++;
}

// A tick-pass event added by mnCreditsInitTextSprites: renders each entering line sixteen rows
// ahead of the visible top, into the hidden half of the tile ring. A null table entry latches the
// end-of-text state.
void mnCreditsDrawEnteringLine(void)
{
    Uint32 scrollPos;
    Sint32 lineIndex;

    // A line is 8 pixels, so a new one is drawn each time the scroll crosses into the next line,
    // every 32 ticks.
    if (CREDITS_END_OF_TEXT == 0)
    {
        scrollPos = CreditsScrollPosition;
        if ((Sint16)CreditsScrollPosition / 8 != CREDITS_LAST_LINE_POSITION / 8)
        {
            lineIndex                  = (Sint16)CreditsScrollPosition / 8;
            CREDITS_LAST_LINE_POSITION = scrollPos;
            CREDITS_END_OF_TEXT        = mnCreditsDrawLine(
                CreditsLines[lineIndex], CREDITS_RING_LINE_TILE(lineIndex), CREDITS_ALIGN_CENTER);
        }
    }
}

// Allocates the credits sprites, builds the blank border and text rows, clears the tile ring and
// adds the scrolling and line-rendering events.
void mnCreditsInitTextSprites(void)
{
    volatile Uint32 dmaFill;
    Uint32         *oamWords;
    Uint32          row;
    Uint32          column;

    SCROLL_OBJECTS = syHeapAllocateEWRAM(CREDITS_OAM_ENTRIES * sizeof(CREDITS_OAM_ENTRY));
    // Solid border sprites immediately after the ring mask the partial text rows at the top and
    // bottom of the window.
    DMA_FILL32_CELL(dmaFill, 0, (void *)OBJ_VRAM0, CREDITS_RING_BYTES);
    DMA_FILL32_CELL(dmaFill,
                    PIXELS_4BPP(1),
                    (void *)(OBJ_VRAM0 + CREDITS_RING_BYTES),
                    CREDITS_BORDER_TILE_BYTES);

    // OAM attributes 0 and 1 share the first word; attribute 2 is the second.
    oamWords = (Uint32 *)SCROLL_OBJECTS;
    // Here row counts the sprites across, 32 pixels apart.
    for (row = 0; row < 8; row++)
    {
        *oamWords++ = OBJ_X(row * 32) | OBJ_SIZE_32X16;
        *oamWords++ = CREDITS_RING_TILES;
    }
    // The bottom strip: eight 32x16 sprites at y 136 and eight 32x8 at y 152, over y 136 to 159.
    for (row = 0; row < 8; row++)
    {
        *oamWords++ = OBJ_X(row * 32) | 136 | OBJ_SIZE_32X16;
        *oamWords++ = CREDITS_RING_TILES;
    }
    for (row = 0; row < 8; row++)
    {
        *oamWords++ = OBJ_X(row * 32) | 152 | OBJ_SIZE_32X8;
        *oamWords++ = CREDITS_RING_TILES;
    }
    // The text sprites, as mnCreditsUpdateTextSprites rebuilds them each VBlank.
    for (row = 0; row < CREDITS_VISIBLE_ROWS; row++)
    {
        for (column = 0; column < CREDITS_COLUMNS; column++)
        {
            Uint32 tileIndex = (row * 3) * 8 + column * 4;
            *oamWords++      = (row * 8 + CREDITS_TEXT_TOP) | OBJ_X(column * 32 + CREDITS_TEXT_LEFT)
                        | OBJ_SIZE_32X8;
            *oamWords++ = tileIndex;
        }
    }
    // The last eight entries are parked off screen.
    for (row = 0; row < 8; row++)
    {
        *oamWords++ = OBJ_X(CREDITS_PARKED) | CREDITS_PARKED;
        *oamWords++ = CREDITS_RING_TILES;
    }
    CreditsScrollPosition      = 0;
    CREDITS_LAST_LINE_POSITION = 0;
    CREDITS_END_OF_TEXT        = 0;
    sySetEvent(mnCreditsUpdateTextSprites, EVENT_PASS_VBLANK);
    sySetEvent(mnCreditsDrawEnteringLine, EVENT_PASS_TICK);
    for (row = 0; row < CREDITS_RING_ROWS; row++)
        mnCreditsDrawLine(CreditsLines[0], row * CREDITS_LINE_TILES, CREDITS_ALIGN_CENTER);
}

// Row `row` (4 bytes, 8 pixels) of sprite tile `tile` in OBJ VRAM.
#define OBJ_TILE_ROW(tile, row) (*(Uint32 *)(OBJ_VRAM0 + (row) * 4 + (tile) * TILE_SIZE_4BPP))

// Centers, right-aligns or left-aligns one credits line in a 192-pixel row.
Sint32 mnCreditsDrawLine(const Uint8 *text, Sint32 firstTile, Sint32 alignment)
{
    Uint8        *linePixels = syHeapAllocateEWRAM(CREDITS_BUFFER_BYTES);
    Sint32        startX     = 0;
    Sint32        lineWidth  = CREDITS_LINE_PIXELS;
    const Uint8  *font       = CreditsFont;
    Sint32        tileCount;
    Sint32        textAdvance;
    const Uint8  *srcBytes;
    Uint8        *dest;
    Uint32        srcValue;
    Sint32        glyphIndex;
    Sint32        glyphWidth;
    Sint32        row;
    Sint32        columnIndex;
    Uint32        glyphBits;
    Uint32        glyphMask;

    // LEFTOVER(frame): an unused local the ROM's frame reserves and never
    // * touches.
    UNUSED Uint32 unused[8];

    if (text == NULL)
    {
        // BUG: the end marker is checked after allocation, leaking one line buffer. syHeapRelease
        // rewinds this bump pool to the allocation's start.
#if FIX_CREDITS_LINE_LEAK
        if (linePixels != NULL)
            syHeapRelease(linePixels);
#endif
        return -1;
    }
    // The ninth raster row holds the shadow below this line.
    if (!gmFlagGetStub(FLAG_CREDITS_LINE_DRAWN))
    {
        syWordsFill(linePixels, CREDITS_BUFFER_BYTES, 0);
        gmFlagSetStub(FLAG_CREDITS_LINE_DRAWN);
    }
    else
    {
        syWordsCopy(linePixels, linePixels + 8 * CREDITS_RASTER_STRIDE, CREDITS_RASTER_STRIDE);
        syWordsFill(linePixels + CREDITS_RASTER_STRIDE, 8 * CREDITS_RASTER_STRIDE, 0);
    }

    // Measure the line in the credits font, then choose where it starts. srcBytes walks the text
    // here, and each glyph's rows below.
    srcBytes    = text;
    textAdvance = 0;
    while ((srcValue = *srcBytes++) != 0)
    {
        if (srcValue >= ' ')
            textAdvance += CreditsGlyphWidths[srcValue - ' '];
    }
    if (alignment == CREDITS_ALIGN_RIGHT)
        startX = lineWidth - textAdvance;
    else if (alignment == CREDITS_ALIGN_CENTER)
        startX = (lineWidth - textAdvance) / 2;

    // Rasterize each glyph and its one-pixel shadow at the aligned start. FAKEMATCH(dead): srcBytes
    // = text, overwritten before it is read; this
    // * pass walks text itself; as no store.
    srcBytes    = text;
    textAdvance = 0;
    while ((srcValue = *text++) != 0)
    {
        if (srcValue >= ' ')
        {
            glyphIndex = srcValue - ' ';
            srcBytes   = font + glyphIndex * 8;
            dest       = linePixels + startX + textAdvance;
            for (row = 0; row < 8; row++)
            {
                glyphBits = *srcBytes++;
                glyphMask = 0x80;
                for (columnIndex = 0; columnIndex < 8; columnIndex++)
                {
                    if (glyphBits & glyphMask)
                    {
                        dest[CREDITS_RASTER_STRIDE + 1] = CREDITS_SHADOW_COLOR;
                        dest[0]                         = CREDITS_TEXT_COLOR;
                    }
                    dest++;
                    glyphMask >>= 1;
                }
                dest += CREDITS_RASTER_STRIDE - 8;
            }
            glyphWidth = 1;
            if (srcValue >= ' ')
                glyphWidth = CreditsGlyphWidths[glyphIndex];
            textAdvance += glyphWidth;
        }
    }

    // Pack pairs of pixel indices into bytes, keeping the 256-pixel stride.
    tileCount = lineWidth / 8;
    srcBytes  = linePixels;
    dest      = linePixels;
    for (row = 0; row < 8; row++)
    {
        for (columnIndex = 0; columnIndex < tileCount * 4; columnIndex++)
        {
            srcValue  = *srcBytes++;
            srcValue |= *srcBytes++ << 4;
            *dest++   = srcValue;
        }
        dest     += CREDITS_RASTER_STRIDE - tileCount * 4;
        srcBytes += CREDITS_RASTER_STRIDE - tileCount * 8;
    }

    // Transpose raster rows into the tile ring used by the scrolling sprites.
    for (row = 0; row < tileCount; row++)
    {
        Sint32  tileIndex          = firstTile + row;
        Uint32 *words              = (Uint32 *)linePixels + row;

        // A tile's eight rows of 4 bytes, each a 256-pixel raster row apart.
        OBJ_TILE_ROW(tileIndex, 0) = words[0];
        OBJ_TILE_ROW(tileIndex, 1) = words[1 * (CREDITS_RASTER_STRIDE / 4)];
        OBJ_TILE_ROW(tileIndex, 2) = words[2 * (CREDITS_RASTER_STRIDE / 4)];
        OBJ_TILE_ROW(tileIndex, 3) = words[3 * (CREDITS_RASTER_STRIDE / 4)];
        OBJ_TILE_ROW(tileIndex, 4) = words[4 * (CREDITS_RASTER_STRIDE / 4)];
        OBJ_TILE_ROW(tileIndex, 5) = words[5 * (CREDITS_RASTER_STRIDE / 4)];
        OBJ_TILE_ROW(tileIndex, 6) = words[6 * (CREDITS_RASTER_STRIDE / 4)];
        OBJ_TILE_ROW(tileIndex, 7) = words[7 * (CREDITS_RASTER_STRIDE / 4)];
    }
    syHeapRelease(linePixels);
    return 0;
}
