#include "types.h"
#include "edition.h"
#include "text_font.h"

//--------------------------------------------------------------------------
// Window tables: each edition's articles, the kana marks and sprite motion.
// window.c copies an article table whole as struct ArticleTable
// (s8 *text[kinds]); each table's words come just before it.
//--------------------------------------------------------------------------

#if !EDITION_INTERNATIONAL || defined(TLA_EDITION_EN)
const u16 UiText_MarkGlyphCodes[] = {
    TEXT_MARK_NONE,
    TEXT_MARK_DAKUTEN,
    TEXT_MARK_HANDAKUTEN,
    TEXT_MARK_NONE
};
#endif

const u32 Data_0805c0e8[2] = {0x1999999a, 0x1999999a};

#if defined(TLA_EDITION_DE)

const s8 UiText_ArticleEmpty[] = "";

static const s8 Article_Ein[] = "ein ";
static const s8 Article_Eine[] = "eine ";
static const s8 Article_Die[] = "die ";
static const s8 Article_Das[] = "das ";

// The article of each kind
const s8 *const UiText_ArticleTable[8] =
{
    UiText_ArticleEmpty,
    Article_Ein,
    Article_Eine,
    Article_Die,
    Article_Das,
    UiText_ArticleEmpty,
    UiText_ArticleEmpty,
    UiText_ArticleEmpty,
};

// The counted nouns (UiText_NumberWords pairs them {singular, plural})
const u8 UiText_NumberWord0[] = "K\xe4pfe";
const u8 UiText_NumberWord1[] = "Kampf";
const u8 UiText_NumberWord2[] = "Tickets";
const u8 UiText_NumberWord3[] = "Ticket";
const u8 UiText_NumberWord4[] = "Runden";
const u8 UiText_NumberWord5[] = "Runde";
const u8 UiText_NumberWord6[] = "M\xfcnzen";
const u8 UiText_NumberWord7[] = "M\xfcnze";
const u8 UiText_NumberWord8[] = "Punkte";
const u8 UiText_NumberWord9[] = "Punkt";

static const s8 Possessive_Dein[] = "dein ";
static const s8 Possessive_Deine[] = "deine ";

// The possessive of each item gender (UiText_ItemGenders)
const s8 *const UiText_ItemPossessives[4] =
{
    Possessive_Dein,
    Possessive_Deine,
    Possessive_Dein,
    UiText_ArticleEmpty,
};

#elif defined(TLA_EDITION_ES)

const s8 UiText_ArticleEmpty[] = "";

static const s8 Article_El[] = "el ";
static const s8 Article_La[] = "la ";
static const s8 Article_Los[] = "los ";
static const s8 Article_Las[] = "las ";

// The definite article of each kind
const s8 *const UiText_ArticleTable[5] =
{
    UiText_ArticleEmpty,
    Article_El,
    Article_La,
    Article_Los,
    Article_Las,
};

static const s8 Article_Un[] = "un ";
static const s8 Article_Una[] = "una ";
static const s8 Article_Unos[] = "unos ";
static const s8 Article_Unas[] = "unas ";

// The indefinite article of each kind
const s8 *const UiText_IndefiniteArticles[5] =
{
    UiText_ArticleEmpty,
    Article_Un,
    Article_Una,
    Article_Unos,
    Article_Unas,
};

static const s8 Possessive_Tu[] = "tu ";
static const s8 Possessive_Tus[] = "tus ";

// The possessive, {singular, plural}
const s8 *const UiText_ItemPossessives[2] =
{
    Possessive_Tu,
    Possessive_Tus,
};

#elif defined(TLA_EDITION_FR)

static const s8 Article_DeElided[] = "d'";
static const s8 Article_De[] = "de ";

// "de", elided and whole
const s8 *const UiText_DeArticles[2] =
{
    Article_DeElided,
    Article_De,
};

const s8 UiText_ArticleEmpty[] = "";

static const s8 Article_Le[] = "le ";
static const s8 Article_La[] = "la ";
static const s8 Article_L[] = "l' ";
static const s8 Article_Les[] = "les ";
static const s8 Article_Un[] = "un ";
static const s8 Article_Une[] = "une ";
static const s8 Article_Des[] = "des ";

// The article of each kind
const s8 *const UiText_ArticleTable[8] =
{
    UiText_ArticleEmpty,
    Article_Le,
    Article_La,
    Article_L,
    Article_Les,
    Article_Un,
    Article_Une,
    Article_Des,
};

const s8 UiText_ItemPronoun[] = "vous ";

#elif defined(TLA_EDITION_IT)

const s8 UiText_ArticleEmpty[] = "";

static const s8 Article_Il[] = "il ";
static const s8 Article_L[] = "l' ";
static const s8 Article_La[] = "la ";
static const s8 Article_Lo[] = "lo ";
static const s8 Article_Le[] = "le ";
static const s8 Article_Gli[] = "gli ";
static const s8 Article_Un[] = "un ";
static const s8 Article_UnElided[] = "un' ";
static const s8 Article_Uno[] = "uno ";
static const s8 Article_Una[] = "una ";

// The article of each kind
const s8 *const UiText_ArticleTable[11] =
{
    UiText_ArticleEmpty,
    Article_Il,
    Article_L,
    Article_La,
    Article_Lo,
    Article_Le,
    Article_Gli,
    Article_Un,
    Article_UnElided,
    Article_Uno,
    Article_Una,
};

static const s8 Possessive_IlTuo[] = "Il tuo ";
static const s8 Possessive_LaTua[] = "la tua ";

// The possessive of each item gender (UiText_ItemGenders)
const s8 *const UiText_ItemPossessives[4] =
{
    Possessive_IlTuo,
    Possessive_LaTua,
    Possessive_IlTuo,
    UiText_ArticleEmpty,
};

#elif defined(TLA_EDITION_EN)

const s8 UiText_ArticleEmpty[] = "";

static const s8 Article_A[] = "a ";
static const s8 Article_An[] = "an ";
static const s8 Article_Some[] = "some ";
static const s8 Article_The[] = "the ";

// The article of each kind
const s8 *const UiText_ArticleTable[8] =
{
    UiText_ArticleEmpty,
    Article_A,
    Article_An,
    Article_Some,
    UiText_ArticleEmpty,
    Article_The,
    UiText_ArticleEmpty,
    UiText_ArticleEmpty,
};

#endif

//--------------------------------------------------------------------------
// Sprite motion
//--------------------------------------------------------------------------

// The cursor's rise and fall
const u8 UiSprite_CursorBobOffsets[80] =
{
    0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5,
    5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7,
    7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7,
    7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5,
    5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1,
};

// A circle of radius 4
const s8 UiSprite_WobbleOffsets[40] =
{
    0, 4, 1, 4, 2, 3, 3, 2,
    4, 1, 4, 0, 4, 1, 3, 2,
    2, 3, 1, 4, 0, 4, -1, 4,
    -2, 3, -3, 2, -4, 1, -4, 0,
    -4, 1, -3, 2, -2, 3, -1, 4,
};

const u8 UiSprite_WobbleTail[16] =
{
    4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// A bounce
const u8 UiSprite_BounceOffsets[16] =
{
    0, 1, 1, 2, 3, 4, 6, 8, 6, 4, 3, 2, 1, 1, 0, 0,
};
