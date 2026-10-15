#include "unit/window/articles.h"
#include "types.h"
#include "edition.h"
#include "text_font.h"

//--------------------------------------------------------------------------
// 文字と絵の表: 冠詞, 濁点, カーソルと揺れ
// Window tables: each edition's articles, the kana marks and sprite motion.
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 冠詞と所有の語 (言語ごと)
//--------------------------------------------------------------------------

// window.c は冠詞の表を struct ArticleTable (s8 *text[種類の数]) として
// まるごと写して読む。どの表も ROM の .rodata にある。語はそれぞれの表の
// 前に並ぶ。

#if defined(TBS_EDITION_DE)

const s8 UiText_ArticleEmpty[] = "";

static const s8 Article_Ein[] = "ein ";
static const s8 Article_Eine[] = "eine ";
static const s8 Article_Die[] = "die ";
static const s8 Article_Das[] = "das ";

// 種類ごとの冠詞
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

// 数える語 (UiText_NumberWords が {単数, 複数} に並べる)
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

// 道具の性 (UiText_ItemGenders) ごとの所有の語
const s8 *const UiText_ItemPossessives[4] =
{
	Possessive_Dein,
	Possessive_Deine,
	Possessive_Dein,
	UiText_ArticleEmpty,
};

#elif defined(TBS_EDITION_ES)

const s8 UiText_ArticleEmpty[] = "";

static const s8 Article_El[] = "el ";
static const s8 Article_La[] = "la ";
static const s8 Article_Los[] = "los ";
static const s8 Article_Las[] = "las ";

// 種類ごとの定冠詞
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

// 種類ごとの不定冠詞
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

// {単数, 複数} の所有の語
const s8 *const UiText_ItemPossessives[2] =
{
	Possessive_Tu,
	Possessive_Tus,
};

#elif defined(TBS_EDITION_FR)

const s8 UiText_ArticleEmpty[] = "";

static const s8 Article_Le[] = "le ";
static const s8 Article_La[] = "la ";
static const s8 Article_L[] = "l' ";
static const s8 Article_Les[] = "les ";
static const s8 Article_Un[] = "un ";
static const s8 Article_Une[] = "une ";
static const s8 Article_Des[] = "des ";

// 種類ごとの冠詞
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

#elif defined(TBS_EDITION_IT)

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

// 種類ごとの冠詞
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

// 道具の性 (UiText_ItemGenders) ごとの所有の語
const s8 *const UiText_ItemPossessives[4] =
{
	Possessive_IlTuo,
	Possessive_LaTua,
	Possessive_IlTuo,
	UiText_ArticleEmpty,
};

#elif EDITION_INTERNATIONAL

const s8 UiText_ArticleEmpty[] = "";

static const s8 Article_A[] = "a ";
static const s8 Article_An[] = "an ";
static const s8 Article_Some[] = "some ";
static const s8 Article_The[] = "the ";

// 種類ごとの冠詞
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
// 濁点とスプライトの動き
//--------------------------------------------------------------------------

#if !EDITION_INTERNATIONAL
const u16 UiText_MarkGlyphCodes[] = {
	TEXT_MARK_NONE,
	TEXT_MARK_VOICED,
	TEXT_MARK_SEMIVOICED,
	TEXT_MARK_NONE
};
#endif

// カーソルの上下
const u8 UiSprite_CursorBobOffsets[80] =
{
	0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5,
	5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7,
	7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7,
	7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5,
	5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1,
};

// 半径 4 の円
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

// 弾み
const u8 UiSprite_BounceOffsets[16] =
{
	0, 1, 1, 2, 3, 4, 6, 8, 6, 4, 3, 2, 1, 1, 0, 0,
};
