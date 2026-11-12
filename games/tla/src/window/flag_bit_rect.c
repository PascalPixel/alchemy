#include "types.h"
#include "window.h"
#include "ram_buffer.h"
#include "frame.h"
#include "scene_call.h"
#include "text/msg_ids.h"

s32 Func_08040798(s32 selection);
s32 Func_08043690(void);
s32 Func_080400e8(void);

/// Runs the save screen's selection until a choice completes: saving (refused
/// while flag 0x17c or 0x17d is set), sleep mode or the options. Returns -1
/// when the screen is cancelled and 0 otherwise.
s32 Func_0804e0d0(void) {
  s32 selection = 0;

again:
  selection = Func_08040798(selection);
  if (selection == -1)
    return -1;
  if (selection == 0) {
    if (GameFlag_Test(0x17c)) {
      FieldMessage_Show((s32)MsgCannotSaveHere, 1);
      goto again;
    }
    if (GameFlag_Test(0x17d)) {
      FieldMessage_Show((s32)MsgNoPlaceForThat, 1);
      goto again;
    }
    if (Func_08043690() == -1)
      goto again;
  } else if (selection == 1) {
    FieldMessage_Show((s32)MsgSleepMode, 1);
    gSleepRequested = selection;
  } else if (selection == 2) {
    if (Func_080400e8() == -1)
      goto again;
  }
  return 0;
}

void Render_SetTilemapFlagBitRect(const struct Window *object, s32 x, s32 y,
                                  s32 width, s32 height, u32 field) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;

  x += (s16)object->x + 1;
  y += (s16)object->y + 1;
  field &= 1;
  field <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }
  if (width > 0 && height > 0) {
    do {
      u16 *pixel = &work->tilemap[y][x];
      s32 remaining = width;
      while (remaining != 0) {
        u32 value = *pixel;
        value &= 0xFFFFEFFF;
        value |= field;
        remaining--;
        *pixel = value;
        pixel++;
      }
      height--;
      y++;
    } while (height != 0);
    work->dirty = 1;
  }
}

#include "input.h"
#include "sprite.h"
#include "render_input.h"
#include "uiflow.h"

extern u8 gWorkspace_ItemTitle[];
extern u8 gWorkspace_EnergyTitle[];
extern u8 gWorkspace_StatusTitle[];
extern u8 gWorkspace_PageLabel[];
extern u8 gWorkspace_RangeDash[];

void wait(s32 frames);
struct RenderInput *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                  s32 style);
void Text_DrawStringInWindow(u8 *text, struct RenderInput *window, u32 x,
                             u32 y);
void Text_DrawNumberInWindow(s32 value, s32 width, struct RenderInput *window,
                             s32 x, s32 y);
void Icon_BuildItemIconTiles(s32 icon, s32 with_base, s32 *slot, s32 *tile,
                             s32 reuse);
void Icon_BuildAbilityIconTiles(s32 icon, s32 with_base, s32 *slot, s32 *tile,
                                s32 reuse);
s32 Resource_FindFreeEntry(void);
void Icon_BuildMiscIconTiles(s32 pattern, s32 unused, s32 slot);

s32 Func_0804e1d8(void) {
  s32 redraw;
  s32 tile;
  s32 page;
  s32 mode;
  struct RenderInput *window;
  s32 slot;
  s32 base;
  s32 i;
  s32 row;
  s32 x;
  s32 y;

  window = NULL;
  page = 0;
  mode = 0;
  redraw = 1;
  Ram_HeapSlots->objectSystem->suspended = redraw;
  wait(1);

  while (1) {
    if (gInput.repeat & 0x20) {
      page--;
      redraw = 1;
    }
    if (gInput.repeat & 0x10) {
      redraw = 1;
      page++;
    }
    if (gInput.repeat & 0x200) {
      mode--;
      redraw = 1;
    }
    if (gInput.repeat & 0x100) {
      redraw = 1;
      mode++;
    }
    if (gInput.repeat & 1)
      break;
    if (gInput.repeat & 2)
      break;
    if (redraw != 0) {
      redraw = 0;
      page = (page + 12) % 12;
      mode = (mode + 3) % 3;
      Window_Close(window, 2);
      window = Window_Create(10, 0, 18, 12, 2);
      if (mode == 0)
        Text_DrawStringInWindow(gWorkspace_ItemTitle, window, 0, 0);
      else if (mode == 1)
        Text_DrawStringInWindow(gWorkspace_EnergyTitle, window, 0, 0);
      else
        Text_DrawStringInWindow(gWorkspace_StatusTitle, window, 0, 0);
      Text_DrawStringInWindow(gWorkspace_PageLabel, window, 0, 8);
      Text_DrawNumberInWindow(page, 0, window, 40, 8);
      base = page << 5;
      Text_DrawNumberInWindow(base, 3, window, 64, 8);
      Text_DrawStringInWindow(gWorkspace_RangeDash, window, 88, 8);
      Text_DrawNumberInWindow(base + 31, 3, window, 96, 8);
      for (i = 0; i <= 31; i++) {
        slot = -1;
        row = i / 8;
        x = (i - row * 8) * 16;
        y = row * 16 + 16;
        if (mode == 0) {
          Icon_BuildItemIconTiles(base + i, 1, &slot, &tile, mode);
          WindowSprite_Create(slot, 0x40000000, window, x, y);
        } else if (mode == 1) {
          Icon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
          WindowSprite_Create(slot, 0x40000000, window, x, y);
        } else {
          slot = Resource_FindFreeEntry();
          Icon_BuildMiscIconTiles(i, 0, slot);
          WindowSprite_Create(slot, 0x40000000, window, x, y);
        }
      }
    }
    wait(1);
  }
  Window_Close(window, 2);
  Ram_HeapSlots->objectSystem->suspended = 0;
  return 0;
}

#include "system.h"
#include "text_render.h"

extern s16 gPortrait_CharacterMap[];
extern s16 gPortrait_SpriteMap[];

struct RenderInput *Func_0803d3c0(s32 x, s32 y, s32 width, s32 height);
void WindowSprite_PrepareForRedraw(struct RenderInput *window);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse);

/// A debug browser of the portraits: left and right step through
/// gPortrait_CharacterMap and then gPortrait_SpriteMap, L and R by ten, each
/// drawn with its number and its name from MsgTestName; A or B leaves.
s32 Func_0804e3f4(void) {
  struct WindowWork *work;
  struct RenderInput *portrait;
  s32 redraw;
  struct RenderInput *window;
  s32 count;
  s32 total;
  s32 index;
  s32 i;
  s32 glyph;
  s32 tile;
  s32 slot;
  s32 cancel;
  u32 slotOffset;
  s32 palette;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  redraw = 1;
  portrait = Func_0803d3c0(0, 0, 10, 5);
  window = Window_Create(10, 10, 14, 3, 2);
  index = 0;
  palette = 15;
  for (i = 0; gPortrait_CharacterMap[i * 2] != -1; i++) {
  }
  count = i;
  for (i = 0; gPortrait_SpriteMap[i * 2] != -1; i++) {
  }
  total = count + i;
  /* FAKEMATCH(dead): a zero from the count keeps the mask in r9 and work in
   * fp; as cancel = 2, 30 of 180 instructions differ in tla-en. */
  cancel = ((u32)(u16)count >> 16) | 2;
  slotOffset = offsetof(struct WindowWork, outputIds[1]);
  while (1) {
    if (gInput.repeat & 0x20) {
      redraw = 1;
      index--;
    }
    if (gInput.repeat & 0x10) {
      redraw = 1;
      index++;
    }
    if (gInput.repeat & 0x200) {
      redraw = 1;
      index -= 10;
    }
    if (gInput.repeat & 0x100) {
      redraw = 1;
      index += 10;
    }
    if (gInput.repeat & 1)
      break;
    if (gInput.repeat & cancel)
      break;
    if (redraw) {
      redraw = 0;
      index = (index + total) % total;
      WindowSprite_PrepareForRedraw(window);
      if (index < count)
        glyph = gPortrait_CharacterMap[index * 2 + 1];
      else
        glyph = gPortrait_SpriteMap[(index - count) * 2 + 1] + 128;
      slot = *(u16 *)((u8 *)work + slotOffset);
      UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, palette, 1);
      Text_DrawNumberInWindow(index, 2, window, 0, 0);
      Text_DrawCharacterAtOffset(index + (s32)MsgTestName, window, 24, 0);
    }
    wait(1);
  }
  Window_Close((struct Window *)window, 2);
  Window_Close((struct Window *)portrait, 2);
  wait(1);
  return 0;
}

#include "edition.h"
#include "text_font.h"

//--------------------------------------------------------------------------
// Window tables: each edition's articles, the kana marks and sprite motion.
// window.c copies an article table whole as struct ArticleTable
// (s8 *text[kinds]); each table's words come just before it.
//--------------------------------------------------------------------------

#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
// clang-format off
const u16 gFont_MarkGlyphCodes[] = {
    TEXT_MARK_NONE,
    TEXT_MARK_VOICED,
    TEXT_MARK_SEMIVOICED,
    TEXT_MARK_NONE
};
#endif

const u32 gData_0805c0e8[2] = {0x1999999a, 0x1999999a};

#if (LANGUAGE == LANGUAGE_GERMAN)

const s8 gText_ArticleEmpty[] = "";

static const s8 sArticleEin[] = "ein ";
static const s8 sArticleEine[] = "eine ";
static const s8 sArticleDie[] = "die ";
static const s8 sArticleDas[] = "das ";

/// The article of each kind
const s8 *const gText_ArticleTable[8] = {
    gText_ArticleEmpty,
    sArticleEin,
    sArticleEine,
    sArticleDie,
    sArticleDas,
    gText_ArticleEmpty,
    gText_ArticleEmpty,
    gText_ArticleEmpty,
};

// The counted nouns (gText_NumberWords pairs them {singular, plural}).
const u8 gText_NumberWord0[] = "K\xe4pfe";
const u8 gText_NumberWord1[] = "Kampf";
const u8 gText_NumberWord2[] = "Tickets";
const u8 gText_NumberWord3[] = "Ticket";
const u8 gText_NumberWord4[] = "Runden";
const u8 gText_NumberWord5[] = "Runde";
const u8 gText_NumberWord6[] = "M\xfcnzen";
const u8 gText_NumberWord7[] = "M\xfcnze";
const u8 gText_NumberWord8[] = "Punkte";
const u8 gText_NumberWord9[] = "Punkt";

static const s8 sPossessiveDein[] = "dein ";
static const s8 sPossessiveDeine[] = "deine ";

/// The possessive of each item gender (gLocalizedWord_ItemGenders)
const s8 *const gLocalizedWord_ItemPossessives[4] = {
    sPossessiveDein,
    sPossessiveDeine,
    sPossessiveDein,
    gText_ArticleEmpty,
};

#elif (LANGUAGE == LANGUAGE_SPANISH)

const s8 gText_ArticleEmpty[] = "";

static const s8 sArticleEl[] = "el ";
static const s8 sArticleLa[] = "la ";
static const s8 sArticleLos[] = "los ";
static const s8 sArticleLas[] = "las ";

/// The definite article of each kind
const s8 *const gText_ArticleTable[5] = {
    gText_ArticleEmpty,
    sArticleEl,
    sArticleLa,
    sArticleLos,
    sArticleLas,
};

static const s8 sArticleUn[] = "un ";
static const s8 sArticleUna[] = "una ";
static const s8 sArticleUnos[] = "unos ";
static const s8 sArticleUnas[] = "unas ";

/// The indefinite article of each kind
const s8 *const gText_IndefiniteArticles[5] = {
    gText_ArticleEmpty,
    sArticleUn,
    sArticleUna,
    sArticleUnos,
    sArticleUnas,
};

static const s8 sPossessiveTu[] = "tu ";
static const s8 sPossessiveTus[] = "tus ";

/// The possessive, {singular, plural}
const s8 *const gLocalizedWord_ItemPossessives[2] = {
    sPossessiveTu,
    sPossessiveTus,
};

#elif (LANGUAGE == LANGUAGE_FRENCH)

static const s8 sArticleDeElided[] = "d'";
static const s8 sArticleDe[] = "de ";

/// "de", elided and whole
const s8 *const gText_DeArticles[2] = {
    sArticleDeElided,
    sArticleDe,
};

const s8 gText_ArticleEmpty[] = "";

static const s8 sArticleLe[] = "le ";
static const s8 sArticleLa[] = "la ";
static const s8 sArticleL[] = "l' ";
static const s8 sArticleLes[] = "les ";
static const s8 sArticleUn[] = "un ";
static const s8 sArticleUne[] = "une ";
static const s8 sArticleDes[] = "des ";

/// The article of each kind
const s8 *const gText_ArticleTable[8] = {
    gText_ArticleEmpty,
    sArticleLe,
    sArticleLa,
    sArticleL,
    sArticleLes,
    sArticleUn,
    sArticleUne,
    sArticleDes,
};

const s8 gLocalizedWord_ItemPronoun[] = "vous ";

#elif (LANGUAGE == LANGUAGE_ITALIAN)

const s8 gText_ArticleEmpty[] = "";

static const s8 sArticleIl[] = "il ";
static const s8 sArticleL[] = "l' ";
static const s8 sArticleLa[] = "la ";
static const s8 sArticleLo[] = "lo ";
static const s8 sArticleLe[] = "le ";
static const s8 sArticleGli[] = "gli ";
static const s8 sArticleUn[] = "un ";
static const s8 sArticleUnElided[] = "un' ";
static const s8 sArticleUno[] = "uno ";
static const s8 sArticleUna[] = "una ";

/// The article of each kind
const s8 *const gText_ArticleTable[11] = {
    gText_ArticleEmpty,
    sArticleIl,
    sArticleL,
    sArticleLa,
    sArticleLo,
    sArticleLe,
    sArticleGli,
    sArticleUn,
    sArticleUnElided,
    sArticleUno,
    sArticleUna,
};

static const s8 sPossessiveIlTuo[] = "Il tuo ";
static const s8 sPossessiveLaTua[] = "la tua ";

/// The possessive of each item gender (gLocalizedWord_ItemGenders)
const s8 *const gLocalizedWord_ItemPossessives[4] = {
    sPossessiveIlTuo,
    sPossessiveLaTua,
    sPossessiveIlTuo,
    gText_ArticleEmpty,
};

#elif (LANGUAGE == LANGUAGE_ENGLISH)

const s8 gText_ArticleEmpty[] = "";

static const s8 sArticleA[] = "a ";
static const s8 sArticleAn[] = "an ";
static const s8 sArticleSome[] = "some ";
static const s8 sArticleThe[] = "the ";

/// The article of each kind
const s8 *const gText_ArticleTable[8] = {
    gText_ArticleEmpty,
    sArticleA,
    sArticleAn,
    sArticleSome,
    gText_ArticleEmpty,
    sArticleThe,
    gText_ArticleEmpty,
    gText_ArticleEmpty,
};

#endif

//--------------------------------------------------------------------------
// Sprite motion
//--------------------------------------------------------------------------

/// The cursor's rise and fall
const u8 gUiSprite_CursorBobOffsets[80] = {
    0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5,
    5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7,
    7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7,
    7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5,
    5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1,
};

/// A circle of radius 4
const s8 gUiSprite_WobbleOffsets[40] = {
    0, 4, 1, 4, 2, 3, 3, 2,
    4, 1, 4, 0, 4, 1, 3, 2,
    2, 3, 1, 4, 0, 4, -1, 4,
    -2, 3, -3, 2, -4, 1, -4, 0,
    -4, 1, -3, 2, -2, 3, -1, 4,
};

const u8 gUiSprite_WobbleTail[16] = {
    4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/// A bounce
const u8 gUiSprite_BounceOffsets[16] = {
    0, 1, 1, 2, 3, 4, 6, 8, 6, 4, 3, 2, 1, 1, 0, 0,
};
// clang-format on
