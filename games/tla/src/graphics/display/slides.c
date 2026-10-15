// The staff credits: the battle backdrops shown behind them and the lines
// that scroll past, as The Broken Seal's slides.c has them.

#include "types.h"
#include "edition.h"

extern char ResourceId_SuhallaGateBackdrop;
extern char ResourceId_PirateShipBackdrop;
extern char ResourceId_CrossboneIsleCaveBackdrop;
extern char ResourceId_ColossoBackdrop;
extern char ResourceId_VenusAerieStormBackdrop;
extern char ResourceId_VenusAerieBackdrop;
extern char ResourceId_VenusLighthouseBackdrop;
extern char ResourceId_SuhallaDesertBackdrop;
extern char ResourceId_AltmillerCaveBackdrop;
extern char ResourceId_TunnelRuinsBackdrop;
extern char ResourceId_BabiLighthouseBackdrop;
extern char ResourceId_LamakanDesertBackdrop;
extern char ResourceId_MogallForestBackdrop;
extern char ResourceId_MercuryAerieBackdrop;
extern char ResourceId_TolbiShipInteriorsBackdrop;
extern char ResourceId_ImilFuchinFallsBackdrop;
extern char ResourceId_KolimaForestBackdrop;
extern char ResourceId_TretInteriorsBackdrop;
extern char ResourceId_InteriorBackdrop;
extern char ResourceId_AltinPeakBackdrop;
extern char ResourceId_TolbiShipDeckBackdrop;
extern char ResourceId_MercuryLighthouseBackdrop;
extern char ResourceId_ValeStormBackdrop;
extern char ResourceId_TretBranchBackdrop;
extern char ResourceId_BeachBackdrop;
extern char ResourceId_SnowfieldBackdrop;
extern char ResourceId_GrasslandBackdrop;
extern char ResourceId_SolSanctumBackdrop;
extern char ResourceId_ForestBackdrop;
extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_DuskCloudsBackdrop;

// The Broken Seal's slideshow backdrops less one, ending in 0. Nothing
// reads this list.
const u32 DisplayScroll_UnusedSlideResources[] = {
    (u32)&ResourceId_ValeStormBackdrop,
    (u32)&ResourceId_SolSanctumBackdrop,
    (u32)&ResourceId_GrasslandBackdrop,
    (u32)&ResourceId_InteriorBackdrop,
    (u32)&ResourceId_ForestBackdrop,
    (u32)&ResourceId_KolimaForestBackdrop,
    (u32)&ResourceId_TretInteriorsBackdrop,
    (u32)&ResourceId_TretBranchBackdrop,
    (u32)&ResourceId_SnowfieldBackdrop,
    (u32)&ResourceId_MercuryLighthouseBackdrop,
    (u32)&ResourceId_MercuryAerieBackdrop,
    (u32)&ResourceId_ImilFuchinFallsBackdrop,
    (u32)&ResourceId_MogallForestBackdrop,
    (u32)&ResourceId_AltinPeakBackdrop,
    (u32)&ResourceId_LamakanDesertBackdrop,
    (u32)&ResourceId_TolbiShipDeckBackdrop,
    (u32)&ResourceId_TolbiShipInteriorsBackdrop,
    (u32)&ResourceId_BeachBackdrop,
    (u32)&ResourceId_ColossoBackdrop,
    (u32)&ResourceId_AltmillerCaveBackdrop,
    (u32)&ResourceId_SuhallaDesertBackdrop,
    (u32)&ResourceId_CrossboneIsleCaveBackdrop,
    (u32)&ResourceId_PirateShipBackdrop,
    (u32)&ResourceId_SuhallaGateBackdrop,
    (u32)&ResourceId_TunnelRuinsBackdrop,
    (u32)&ResourceId_BabiLighthouseBackdrop,
    (u32)&ResourceId_VenusLighthouseBackdrop,
    (u32)&ResourceId_VenusAerieBackdrop,
    (u32)&ResourceId_VenusAerieStormBackdrop,
    (u32)&ResourceId_DuskCloudsBackdrop,
    (u32)&ResourceId_VioletSkyBackdrop,
    0
};

// The backdrops the slideshow fades between, in turn; 0 ends the list,
// and the table has room for 25.
const u32 DisplayScroll_SlideResources[25] = {
    (u32)&ResourceId_ValeStormBackdrop,
    (u32)&ResourceId_SolSanctumBackdrop,
    (u32)&ResourceId_InteriorBackdrop,
    (u32)&ResourceId_KolimaForestBackdrop,
    (u32)&ResourceId_TretBranchBackdrop,
    (u32)&ResourceId_SnowfieldBackdrop,
    (u32)&ResourceId_MercuryLighthouseBackdrop,
    (u32)&ResourceId_ImilFuchinFallsBackdrop,
    (u32)&ResourceId_MogallForestBackdrop,
    (u32)&ResourceId_AltinPeakBackdrop,
    (u32)&ResourceId_LamakanDesertBackdrop,
    (u32)&ResourceId_TolbiShipDeckBackdrop,
    (u32)&ResourceId_TolbiShipInteriorsBackdrop,
    (u32)&ResourceId_BeachBackdrop,
    (u32)&ResourceId_ColossoBackdrop,
    (u32)&ResourceId_SuhallaDesertBackdrop,
    (u32)&ResourceId_TunnelRuinsBackdrop,
    (u32)&ResourceId_BabiLighthouseBackdrop,
    (u32)&ResourceId_VenusLighthouseBackdrop,
    (u32)&ResourceId_VenusAerieBackdrop,
    (u32)&ResourceId_DuskCloudsBackdrop,
    0
};

static const u8 sBlankLine[] = "";

// Ends a section of the credits: the scroll stops on it, and a section's
// start is found by counting these marks.
#define CREDITS_SECTION_END ((const void *)255)

#if defined(TLA_EDITION_DE)
#define CREDITS_SUBTITLE "DIE VERGESSENE EPOCHE"
#elif defined(TLA_EDITION_ES)
#define CREDITS_SUBTITLE "LA EDAD PERDIDA"
#elif defined(TLA_EDITION_FR)
#define CREDITS_SUBTITLE "L'\177GE PERDU" // L'ÂGE PERDU: the French font draws Â at 0x7f
#elif defined(TLA_EDITION_IT)
#define CREDITS_SUBTITLE "L'ERA PERDUTA"
#endif

// The Spanish, French and Italian fonts draw í at '`'.
#if defined(TLA_EDITION_DE)
#define RODRIGUEZ "Rodriguez"
#else
#define RODRIGUEZ "Rodr`guez"
#endif

// The staff credits, one line each, ending in 0.
const void *DisplayScroll_LineTable[] = {
    sBlankLine,
#if !EDITION_INTERNATIONAL
    "OUGON NO TAIYO STAFF",
    sBlankLine,
#elif defined(TLA_EDITION_EN)
    "GOLDEN SUN: THE LOST AGE",
    "STAFF",
#else
    "GOLDEN SUN",
    CREDITS_SUBTITLE,
    "STAFF",
#endif
    sBlankLine,
    "Game Planning",
    sBlankLine,
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlankLine,
    sBlankLine,
    "Scenario",
    sBlankLine,
    "Hiroyuki Takahashi",
    sBlankLine,
    sBlankLine,
    "Director",
    sBlankLine,
    "Shugo Takahashi",
    sBlankLine,
    sBlankLine,
    "Assistant Director",
    sBlankLine,
    "Yusuke Sugimoto",
    sBlankLine,
    sBlankLine,
    "Game Design",
    sBlankLine,
    "Yusuke Sugimoto",
    "Shuji Shimizu",
    "Ayumu Shindo",
    "Kentaro Sakou",
    "Norisumi Osawa",
    "Masato Kawamura",
    CREDITS_SECTION_END,
    "Programming",
    sBlankLine,
    sBlankLine,
    "System ",
    sBlankLine,
    "Yasuhiro Taguchi",
    sBlankLine,
    sBlankLine,
    "Battle",
    sBlankLine,
    "Haruki Kodera",
    "Toru Takamatsu",
    sBlankLine,
    sBlankLine,
    "Field",
    sBlankLine,
    "Kaoru Shimada",
    "Hiroyuki Ichikawa",
    "Akiko Sato",
    "Kenji Numaya",
#if EDITION_INTERNATIONAL
    "P.E. Jareth Hein",
#else
    "P.E.Jareth Hein",
#endif
    sBlankLine,
    sBlankLine,
    "User Interface",
    sBlankLine,
    "Yutaka Yamamoto",
    CREDITS_SECTION_END,
#if EDITION_INTERNATIONAL
    "Graphic Art",
#else
    "Graphic Arts",
#endif
    sBlankLine,
    sBlankLine,
    "Battle Characters",
    sBlankLine,
    "Junko Nakamura",
    "Kaori Matsunaga",
    sBlankLine,
    sBlankLine,
    "Battle Effects",
    "& Faces",
    sBlankLine,
    "Junichi Ochiai",
    sBlankLine,
    sBlankLine,
    "Battle Backgrounds",
    sBlankLine,
    "Hiroto Nakashima",
    sBlankLine,
    sBlankLine,
    "Maps",
    sBlankLine,
    "Masayuki Hashimoto",
    "Mitsumasa Muraishi",
    "Mitsuru Fukumoto",
    "Akihito Kimura",
    "Shinichiro Sugimoto",
    "Natsuko Takeuchi",
    sBlankLine,
    sBlankLine,
    "Map Characters",
    sBlankLine,
    "Mieko Koguchi",
    "Kanako Horiguchi",
    sBlankLine,
    sBlankLine,
    "Graphics Direction",
    sBlankLine,
    "Fumihide Aoki",
    CREDITS_SECTION_END,
    "Character Design",
    sBlankLine,
    "Shin Yamanouchi",
    sBlankLine,
    sBlankLine,
    "Monster Design",
    sBlankLine,
    "Hiroshi Kajiyama",
    sBlankLine,
    sBlankLine,
    "Music",
    sBlankLine,
    "Motoi Sakuraba",
    sBlankLine,
    sBlankLine,
    "Sound Effects",
    sBlankLine,
    "Hiroyuki Hamada",
    sBlankLine,
    sBlankLine,
    "Sound Direction",
    sBlankLine,
    "Masaaki Uno",
    sBlankLine,
    sBlankLine,
    "Staff Coordination",
    sBlankLine,
    "Akiko Sato",
    "Kenji Numaya",
    CREDITS_SECTION_END,
    "Illustration",
    sBlankLine,
    "Shin Yamanouchi",
    "Hiroshi Kajiyama",
    "Masanori Sato",
    sBlankLine,
    sBlankLine,
    "Graphic Support",
    sBlankLine,
    "Yoichi Kotabe",
    "Masanori Sato",
    sBlankLine,
    sBlankLine,
#if !EDITION_INTERNATIONAL
    "Artwork Design",
    sBlankLine,
    "Nanako Kinoshita",
    "Takashi Maeda",
    "Naoko Okamoto",
    "Shinji Yamamoto",
    sBlankLine,
    sBlankLine,
    "Public Relations",
    sBlankLine,
    "Mitsuaki Hagishima",
    "Asako Haruhana",
    sBlankLine,
    sBlankLine,
#endif
    "Coordination",
    sBlankLine,
    "Masaaki Uno",
    "Toshiharu Izuno",
    "Kenichi Nishida",
    sBlankLine,
    sBlankLine,
#if !EDITION_INTERNATIONAL
    "Special Thanks to",
    "Super Mario Club Staff",
    sBlankLine,
    "Yoshihiro Maruyama",
    "Nobuaki Maeda",
    "Tadasumi Ueno",
    "Tomoharu Hizakae",
    "Yoshifumi Ogawa",
    "Naotaka Matsumoto",
    "Kei Nadabe",
    "Masami Kato",
    "Hideki Katsui",
    "Hiroyuki Murata",
    "Junko Komaki",
    "Kayo Komaki",
    "Hisashi Takizaki",
    "Akiko Matsumoto",
    "Norihide Okamura",
    "Satoru Uno",
    "Masaki Mori",
    "Tomoko Yuasa",
    "Takeshi Morishima",
    "Yoshikazu Tanaka",
    "Haruka Kato",
    "Norihiko Morikawa",
    "Katsuhiro Nii",
    "Katsunori Masutani",
    "Yukio Homma",
    "Ayumi Hidaka",
    "Kuniharu Naitou",
    "Shunsuke Kobori",
    "Masaharu Shima",
    "Masaru Ikeshita",
    "Naoki Tsuta",
    "Satoshi Mutou",
    "Takayuki Murayama",
    "Tsuyoshi Miyamae",
    "Eri Hashimoto",
#else
    "North American",
    " Localization",
    sBlankLine,
    "Translation",
    sBlankLine,
    "William Barnes",
    "Reiko Ninomiya",
    "Tim O'Leary",
    "Scot Ritchey",
    "Bill Trinen",
    sBlankLine,
    "Writing",
    " & Editing",
    sBlankLine,
    "Richard Amtower",
    "Shawn Seavers",
    sBlankLine,
    "Localization",
    " Management",
    sBlankLine,
    "Leslie Swan",
    "Jeff Miller",
    sBlankLine,
    sBlankLine,
    "NOA Product Testing",
    "Michael Kelbaugh",
    "Eric Bush",
    "Todd Buechele",
    "Tim Casey",
    "Jeffrey Storbo",
    sBlankLine,
#if !defined(TLA_EDITION_EN)
    sBlankLine,
    "European Localization",
    sBlankLine,
    "Product Localization",
    "Team",
    sBlankLine,
    "Coordination",
    "Andy Fey",
    sBlankLine,
    "Translation",
    sBlankLine,
    // Each edition names its own translator first.
#if defined(TLA_EDITION_DE)
    "German",
    "Micky Auer",
    sBlankLine,
    "Spanish",
    "Carlos Montilla",
    RODRIGUEZ,
    sBlankLine,
    "French",
    "Zadia Messerli",
    sBlankLine,
    "Italian",
    "Christian Massi",
    sBlankLine,
#elif defined(TLA_EDITION_ES)
    "Spanish",
    "Carlos Montilla",
    RODRIGUEZ,
    sBlankLine,
    "German",
    "Micky Auer",
    sBlankLine,
    "French",
    "Zadia Messerli",
    sBlankLine,
    "Italian",
    "Christian Massi",
    sBlankLine,
#elif defined(TLA_EDITION_FR)
    "French",
    "Zadia Messerli",
    sBlankLine,
    "German",
    "Micky Auer",
    sBlankLine,
    "Spanish",
    "Carlos Montilla",
    RODRIGUEZ,
    sBlankLine,
    "Italian",
    "Christian Massi",
    sBlankLine,
#else
    "Italian",
    "Christian Massi",
    sBlankLine,
    "German",
    "Micky Auer",
    sBlankLine,
    "French",
    "Zadia Messerli",
    sBlankLine,
    "Spanish",
    "Carlos Montilla",
    RODRIGUEZ,
    sBlankLine,
#endif
    "Validation",
    "& Testing Team",
#endif
#endif
    CREDITS_SECTION_END,
    "Produced by",
    sBlankLine,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlankLine,
    sBlankLine,
    "Executive Producer",
    sBlankLine,
    "Hiroshi Yamauchi",
    sBlankLine,
    sBlankLine,
#if EDITION_INTERNATIONAL
    "Copyrights of Game, Scenario,",
#else
    "Copyrights of Game, Scenario",
#endif
    " and Program reserved by",
    sBlankLine,
    "NINTENDO",
    "and",
    "CAMELOT",
    sBlankLine,
    "Copyrights of All Characters",
    "and Music reserved by",
    sBlankLine,
    "NINTENDO",
    sBlankLine,
    CREDITS_SECTION_END,
    0
};

#include "../../../../common/src/graphics/display/glyph_widths.h"
