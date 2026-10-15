//--------------------------------------------------------------------------
// clear_character_block.c
// タイル：BG のキャラクターとパレットの半分を消す
//--------------------------------------------------------------------------

#include "unit/graphics/tile/slides.h"
#include "unit/runtime/runtime.h"
#include "dma.h"
#include "io_reg.h"
#include "types.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "resource.h"
#include "ramcode.h"
#include "callback_scheduler.h"
#include "io_write_queue.h"
#include "iowrite.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "resource_ids.h"
#include "edition.h"
#include "iwram_call.h"
#include "layout_guard.h"
#include "btlpty.h"
#include "btlsys.h"
#include "frame.h"

//--------------------------------------------------------------------------
// 256 色のキャラクター 30K バイトを色 1 で埋め、パレット 128 色を 0 にする
// alt : 0 なら前半（VRAM 0x06000000・パレット 0〜127）
//       1 なら後半（VRAM 0x06008000・パレット 128〜255、色 129 で埋める）
//--------------------------------------------------------------------------
void Graphics_ClearCharacterBlockAndPalette(s32 alt)
{
	u32			 v;
	u32			 vram;
	u32			 pal;
	volatile u32 fill;

	if (alt == 0)
	{
		v = 0x01010101;
		vram = 0x06000000;
		pal = 0x05000000;
	}
	else
	{
		v = 0x81818181;
		vram = 0x06008000;
		pal = 0x05000100;
	}
	Dma_Fill32Cell(fill, v, (void *)vram, 0x7800);
	Dma_Fill32Cell(fill, 0, (void *)pal, 0x100);
}

typedef s32 (*PackedDecoder)(u8 *source, u32 destination, u32 fill);

// スタッフロールで順に映す戦闘背景（0 で消す）
const u32 DisplayScroll_SlideResources[] = {
	(u32)&ResourceId_ValeStormBackdrop,
	(u32)&ResourceId_SolSanctumBackdrop,
	(u32)&ResourceId_GrasslandBackdrop,
	(u32)&ResourceId_InteriorBackdrop,
	(u32)&ResourceId_ForestBackdrop,
	(u32)&ResourceId_GomaCaveBackdrop,
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
	0				// 消す
};

void Graphics_LoadCharacterBlockAndPalette(u32 resource, s32 alternate)
{
	u8 *data;
	u32 palette;
	u32 fill;
	u32 vram;

	if (resource == 0)
	{
		Graphics_ClearCharacterBlockAndPalette(alternate);
		return;
	}
	data = Resource_GetTableEntry(resource);
	if (alternate == 0)
	{
		fill = 0;
		vram = 0x06000000;
		palette = 0x05000000;
	}
	else
	{
		fill = 0x80808080;
		vram = 0x06008000;
		palette = 0x05000100;
	}
	RamCode_LoadSlot(49, BattleEffect_Expand, BattleEffect_ExpandCodeSize);
	((PackedDecoder)((union HeapState *)gWorkSlot)->slots[49])(data + 256, vram, fill);
	Runtime_ReleaseHeapBlock(49);

	IO_WRITE_QUEUE_PUSH(palette, (u32)data, 0x84000040);
}

void DisplayScroll_BuildHblankWordTable(u32 *arg0)
{
	s32 count;
	u32 value = 0x01FF01FF;
	u32 step = 0x10000;

	count = 31;
	do
	{
		count--;
		*arg0++ = value;
	} while (count >= 0);
	count = 239;
	do
	{
		count--;
		*arg0++ = step;
		step += 0x20002;
	} while (count >= 0);
	count = 47;
	do
	{
		count--;
		*arg0++ = value;
	} while (count >= 0);
	step = 0;
	count = 191;
	do
	{
		count--;
		*arg0++ = step;
	} while (count >= 0);
}

void DisplayScroll_StepPositionEveryFourFrames(void)
{
	if ((gFrameTick & 3) == 0)
	{
		u32 decrement = 0xffff;

		gBgScroll[2].x += decrement;
		gBgScroll[3].x += decrement;
	}
}

s32 DisplayScroll_RunSlideshow(void)
{
	u32 slide;
	s32 alpha;

	gOamCopyEnabled = 0;
	gCpuLoadDisplayEnabled = 0;
	gDebugTextEnabled = 0;
	gOptionMirror = 0;
	Scheduler_ResetTaskTable();
	Scheduler_AddOrUpdateCallback(DisplayScroll_StepPositionEveryFourFrames, 0x480);
	Io_Write16(0x40, &REG_DISPCNT);
	DisplayScroll_BuildHblankWordTable((u32 *)0x06007800);
	DisplayScroll_BuildHblankWordTable((u32 *)0x0600f800);
	Graphics_ClearCharacterBlockAndPalette(0);
	Graphics_ClearCharacterBlockAndPalette(1);
	Io_Write16(0x1f8a, &REG_BG2CNT);
	Io_Write16(0x0f83, &REG_BG3CNT);
	Io_Write16(0x1c40, &REG_DISPCNT);
	Io_Write16(0x2844, &REG_BLDCNT);
	DisplayScroll_InitObjectTable();
	WaitFrames(300);
	for (slide = 0; slide <= 32; slide++)
	{
		Graphics_LoadCharacterBlockAndPalette(DisplayScroll_SlideResources[slide], (slide & 1) ^ 1);
		for (alpha = 1; alpha <= 16; alpha++)
		{
			if (slide & 1)
				Io_Write16((alpha << 8) | (16 - alpha), &REG_BLDALPHA);
			else
				Io_Write16(((16 - alpha) << 8) | alpha, &REG_BLDALPHA);
			WaitFrames(4);
		}
		WaitFrames(267);
	}
	Io_Write16(0, &REG_BLDCNT);
	Io_Write16(0x1040, &REG_DISPCNT);
	Ui_LoadWindowGraphics();
	Bg0_ClearTilemap();
	gOamCopyEnabled = 1;
	return 0;
}

struct DisplayScrollObject
{
	u32 attr01;
	u32 attr2;
};

extern u16 Flash_Handler3; // クレジットはフラッシュ用のワードを流用する

// OAM へ送る 128 個の表（これもフラッシュ用のワードに置く）
#define SCROLL_OBJECTS (*(struct DisplayScrollObject **)&gFlashNumRemainingBytes)

// スタッフロールの空行
static const u8 Blank[] = "";

// スタッフロール：1 行ずつ、0 で終わり
const void *DisplayScroll_LineTable[] = {
	Blank,
#if EDITION_INTERNATIONAL
	"GOLDEN SUN STAFF CREDITS",
#else
	"OUGON NO TAIYO STAFF",
#endif
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Game Planning",
	Blank,
	"Hiroyuki Takahashi",
	"Shugo Takahashi",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Scenario",
	Blank,
	"Hiroyuki Takahashi",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Director",
	Blank,
	"Shugo Takahashi",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Assistant Director",
	Blank,
	"Yusuke Sugimoto",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Game Design",
	Blank,
	"Yusuke Sugimoto",
	"Shuji Shimizu",
	"Ayumu Shindo",
	"Kentaro Sakou",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Programming",
	Blank,
	"System",
	Blank,
	"Yasuhiro Taguchi",
	Blank,
	Blank,
	Blank,
	Blank,
	"Battle",
	Blank,
	"Haruki Kodera",
	"Toru Takamatsu",
	Blank,
	Blank,
	Blank,
	Blank,
	"Field",
	Blank,
	"Kaoru Shimada",
	"Hiroyuki Ichikawa",
	"Akiko Sato",
	Blank,
	Blank,
	Blank,
	Blank,
	"User Interface",
	Blank,
	"Yutaka Yamamoto",
	"Yasushi Kazawa",
	Blank,
	Blank,
	Blank,
	Blank,
	"Support",
	Blank,
	"Kenji Numaya",
	"P. E. Jareth Hein",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Graphic Arts",
	Blank,
	"Character Design",
	Blank,
	"Shin Yamanouchi",
	Blank,
	Blank,
	Blank,
	Blank,
	"Monster Design",
	Blank,
	"Hiroshi Kajiyama",
	Blank,
	Blank,
	Blank,
	Blank,
	"Battle Characters",
	Blank,
	"Junko Nakamura",
	"Kaori Matsunaga",
	Blank,
	Blank,
	Blank,
	Blank,
	"Battle Effects",
#if EDITION_INTERNATIONAL
	"  & Faces",
#else
	"& Faces",
#endif
	Blank,
	"Junichi Ochiai",
	Blank,
	Blank,
	Blank,
	Blank,
	"Battle Backgrounds",
	Blank,
	"Hiroto Nakashima",
	Blank,
	Blank,
	Blank,
	Blank,
	"Maps",
	Blank,
	"Masayuki Hashimoto",
	"Mitsumasa Muraishi",
	"Mitsuru Fukumoto",
	"Masashi Mikami",
	"Akihito Kimura",
	"Shinichiro Sugimoto",
	Blank,
	Blank,
	Blank,
	Blank,
	"Map Characters",
	Blank,
	"Mieko Koguchi",
	"Kanako Horiguchi",
	Blank,
	Blank,
	Blank,
	Blank,
	"Graphics Direction",
	Blank,
	"Fumihide Aoki",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Staff Coordination",
	Blank,
	"Akiko Sato",
	"Kenji Numaya",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Music",
	Blank,
	"Motoi Sakuraba",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Sound Effects",
	Blank,
	"Hiroyuki Hamada",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
#if EDITION_INTERNATIONAL
	"Sound Director",
#else
	"Sound Direction",
#endif
	Blank,
	"Masaaki Uno",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Illustration",
	Blank,
	"Shin Yamanouchi",
	"Masanori Sato",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
#if EDITION_INTERNATIONAL
	"Graphic Supports",
#else
	"Graphic Support",
#endif
	Blank,
	"Yoichi Kotabe",
	"Masanori Sato",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
#if EDITION_INTERNATIONAL
	"Artwork",
#else
	"Artwork Design",
#endif
	Blank,
	"Yoshitaka Nishikawa",
	"Mamoru Sakurai",
	"Nanako Kinoshita",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
#if EDITION_INTERNATIONAL
	"Coordination",
	Blank,
	"Masaaki Uno",
	"Toshiharu Izuno",
	"Atsushi Okada",
	"Kenichi Nishida",
	Blank,
	Blank,
	Blank,
	Blank,
#endif
#if !EDITION_INTERNATIONAL
	"Public Relations",
	Blank,
	"Mitsuaki Hagishima",
	"Asako Haruhana",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Coordination",
	Blank,
	"Masaaki Uno",
	"Toshiharu Izuno",
	"Kenichi Nishida",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Special Thanks to",
	"Super Mario Club Staff",
	Blank,
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
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Produced by",
	Blank,
	"Shinji Hatano",
	"Hiroyuki Takahashi",
	"Shugo Takahashi",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Executive Producer",
	Blank,
	"Hiroshi Yamauchi",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Copyrights of Game, Scenario",
	"and Program reserved by",
	Blank,
	"NINTENDO Co.,Ltd.",
	"and",
	"CAMELOT Co.,Ltd.",
	Blank,
	"Copyrights of All Characters",
	"and Music reserved by",
	Blank,
	"NINTENDO Co.,Ltd.",
	Blank,
	Blank,
	Blank,
	0
#elif defined(TBS_EDITION_EN)
	Blank,
	"North American",
	" Localization",
	Blank,
	Blank,
	Blank,
	Blank,
	"Translation",
	" Coordination",
	Blank,
	"Bill Trinen",
	Blank,
	Blank,
	Blank,
	Blank,
	"Writing",
	" & Editing",
	Blank,
	"Richard Amtower",
	"Nate Bihldorf",
	Blank,
	Blank,
	Blank,
	Blank,
	"Additional",
	" Localization",
	Blank,
	"Tim O'Leary",
	"Scot Ritchey",
	Blank,
	Blank,
	Blank,
	Blank,
	"Localization",
	" Management",
	Blank,
	"Leslie Swan",
	"Jeff Miller",
	Blank,
	Blank,
	Blank,
	Blank,
	"Testing &",
	" Debugging",
	Blank,
	"Michael Kelbaugh",
	"Todd Buechele",
	"NOA Product Testing",
	" Department",
	Blank,
	Blank,
	Blank,
	Blank,
	"Special Thanks to",
	Blank,
	"Motoyasu Tojima",
	"Mike Fukuda",
	Blank,
	" and",
	Blank,
	Blank,
	"Minoru Arakawa",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Producer",
	Blank,
	"Shinji Hatano",
	"Hiroyuki Takahashi",
	"Shugo Takahashi",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Executive Producer",
	Blank,
	"Hiroshi Yamauchi",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Copyrights of Game, Scenario,",
	"and Program reserved by",
	Blank,
	"NINTENDO and CAMELOT.",
	Blank,
	"Copyrights of all characters",
	"and music reserved by",
	Blank,
	"NINTENDO.",
	Blank,
	Blank,
	Blank,
	0
#elif defined(TBS_EDITION_DE)
	"North American",
	" Localization",
	Blank,
	Blank,
	Blank,
	"Translation",
	" Coordination",
	Blank,
	"Bill Trinen",
	Blank,
	Blank,
	Blank,
	"Writing",
	" & Editing",
	Blank,
	"Richard Amtower",
	"Nate Bihldorf",
	Blank,
	Blank,
	Blank,
	"Additional   ",
	" Localization",
	Blank,
	"Tim O'Leary",
	"Scot Ritchey",
	Blank,
	Blank,
	Blank,
	"Localization",
	" Management ",
	Blank,
	"Leslie Swan",
	"Jeff Miller",
	Blank,
	Blank,
	Blank,
	"Localization & ",
	" Testing Europe",
	Blank,
	"Andy Fey",
	"Kai Zeh",
	"Tanja Baar",
	Blank,
	Blank,
	Blank,
	"German Translation",
	Blank,
	"Micky Auer",
	"Martina Deimel",
	Blank,
	Blank,
	Blank,
	"German Testing",
	Blank,
	"Maurice Tisdale",
	"Andreas Dietz",
	"Rudolf Schnitzer",
	"Wolfgang Weber",
	"Frank Heuchle",
	"Zoran Tasevski",
	Blank,
	Blank,
	Blank,
	"Special Thanks to",
	Blank,
	"Motoyasu Tojima",
	"Takako Morita",
	Blank,
	Blank,
	Blank,
	Blank,
	"Producer",
	Blank,
	"Shinji Hatano",
	"Hiroyuki Takahashi",
	"Shugo Takahashi",
	Blank,
	Blank,
	Blank,
	"Executive Producer",
	Blank,
	"Hiroshi Yamauchi",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Copyrights of Game, Scenario,",
	"and Program reserved by",
	Blank,
	"NINTENDO and CAMELOT.",
	Blank,
	"Copyrights of all characters",
	"and music reserved by",
	Blank,
	"NINTENDO.",
	Blank,
	Blank,
	Blank,
	0
#elif defined(TBS_EDITION_ES)
	"North American",
	" Localization",
	Blank,
	Blank,
	Blank,
	"Translation",
	" Coordination",
	Blank,
	"Bill Trinen",
	Blank,
	Blank,
	Blank,
	"Writing",
	" & Editing",
	Blank,
	"Richard Amtower",
	"Nate Bihldorf",
	Blank,
	Blank,
	Blank,
	"Additional   ",
	" Localization",
	Blank,
	"Tim O'Leary",
	"Scot Ritchey",
	Blank,
	Blank,
	Blank,
	"Localization",
	" Management ",
	Blank,
	"Leslie Swan",
	"Jeff Miller",
	Blank,
	Blank,
	Blank,
	"Localization & ",
	" Testing Europe",
	Blank,
	"Andy Fey",
	"Kai Zeh",
	"Tanja Baar",
	Blank,
	Blank,
	Blank,
	"Spanish Translation",
	Blank,
	"Carlos Montilla",
	"Miguel Lopez",
	Blank,
	Blank,
	Blank,
	"Spanish Testing",
	Blank,
	"Maurice Tisdale",
	"Andreas Dietz",
	"Luis Martinez",
	"Jose Ramon Diaz",
	"Ana Paris",
	"Monica Gomez",
	Blank,
	Blank,
	Blank,
	"Special Thanks to",
	Blank,
	"Motoyasu Tojima",
	"Takako Morita",
	Blank,
	Blank,
	Blank,
	Blank,
	"Producer",
	Blank,
	"Shinji Hatano",
	"Hiroyuki Takahashi",
	"Shugo Takahashi",
	Blank,
	Blank,
	Blank,
	"Executive Producer",
	Blank,
	"Hiroshi Yamauchi",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Copyrights of Game, Scenario,",
	"and Program reserved by",
	Blank,
	"NINTENDO and CAMELOT.",
	Blank,
	"Copyrights of all characters",
	"and music reserved by",
	Blank,
	"NINTENDO.",
	Blank,
	Blank,
	Blank,
	0
#elif defined(TBS_EDITION_FR)
	"North American",
	" Localization",
	Blank,
	Blank,
	Blank,
	"Translation",
	" Coordination",
	Blank,
	"Bill Trinen",
	Blank,
	Blank,
	Blank,
	"Writing",
	" & Editing",
	Blank,
	"Richard Amtower",
	"Nate Bihldorf",
	Blank,
	Blank,
	Blank,
	"Additional   ",
	" Localization",
	Blank,
	"Tim O'Leary",
	"Scot Ritchey",
	Blank,
	Blank,
	Blank,
	"Localization",
	" Management ",
	Blank,
	"Leslie Swan",
	"Jeff Miller",
	Blank,
	Blank,
	Blank,
	"Localization & ",
	" Testing Europe",
	Blank,
	"Andy Fey",
	"Kai Zeh",
	"Tanja Baar",
	Blank,
	Blank,
	Blank,
	"French Translation",
	Blank,
	"Daniel Charbit",
	"Steven Arhan",
	Blank,
	Blank,
	Blank,
	"French Testing",
	Blank,
	"Maurice Tisdale",
	"Peter Ehlert",
	"Emeline Condy",
	"Pascal Di Pasquale",
	"Kamel Chelirem",
	"Sebastien Aubre",
	"Michael Troger",
	Blank,
	Blank,
	Blank,
	"Special Thanks to",
	Blank,
	"Motoyasu Tojima",
	"Takako Morita",
	Blank,
	Blank,
	Blank,
	Blank,
	"Producer",
	Blank,
	"Shinji Hatano",
	"Hiroyuki Takahashi",
	"Shugo Takahashi",
	Blank,
	Blank,
	Blank,
	"Executive Producer",
	Blank,
	"Hiroshi Yamauchi",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Copyrights of Game, Scenario,",
	"and Program reserved by",
	Blank,
	"NINTENDO and CAMELOT.",
	Blank,
	"Copyrights of all characters",
	"and music reserved by",
	Blank,
	Blank,
	Blank,
	Blank,
	0
#else
	"North American",
	" Localization",
	Blank,
	Blank,
	Blank,
	"Translation",
	" Coordination",
	Blank,
	"Bill Trinen",
	Blank,
	Blank,
	Blank,
	"Writing",
	" & Editing",
	Blank,
	"Richard Amtower",
	"Nate Bihldorf",
	Blank,
	Blank,
	Blank,
	"Additional   ",
	" Localization",
	Blank,
	"Tim O'Leary",
	"Scot Ritchey",
	Blank,
	Blank,
	Blank,
	"Localization",
	" Management ",
	Blank,
	"Leslie Swan",
	"Jeff Miller",
	Blank,
	Blank,
	Blank,
	"Localization & ",
	" Testing Europe",
	Blank,
	"Andy Fey",
	"Kai Zeh",
	"Tanja Baar",
	Blank,
	Blank,
	Blank,
	"Italian Translation",
	Blank,
	"Andrea Salvi",
	"Calimero Tiiijun",
	Blank,
	Blank,
	Blank,
	"Italian Testing",
	Blank,
	"Maurice Tisdale",
	"Andreas Dietz",
	"Sonia Trepiccioni",
	"Michele Liuzzi",
	"Luca Riccioni",
	"Andrea De Benedetto",
	Blank,
	Blank,
	Blank,
	"Special Thanks to",
	Blank,
	"Motoyasu Tojima",
	"Takako Morita",
	Blank,
	Blank,
	Blank,
	Blank,
	"Producer",
	Blank,
	"Shinji Hatano",
	"Hiroyuki Takahashi",
	"Shugo Takahashi",
	Blank,
	Blank,
	Blank,
	"Executive Producer",
	Blank,
	"Hiroshi Yamauchi",
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	Blank,
	"Copyrights of Game, Scenario,",
	"and Program reserved by",
	Blank,
	"NINTENDO and CAMELOT.",
	Blank,
	"Copyrights of all characters",
	"and music reserved by",
	Blank,
	"NINTENDO.",
	Blank,
	Blank,
	Blank,
	0
#endif
};

#include "../../../../common/src/graphics/display/glyph_widths.h"

void DisplayScroll_UpdateObjects(void)
{
	u32	 frame = Flash_Handler3;
	u32	 fine = frame & 7;
	s32	 tile = (((s16)frame / 8) & 0x1f) * 3 * 8;
	u32 *entry = (u32 *)(SCROLL_OBJECTS + 24);
	s32	 row;

	for (row = 0; row <= 15; row++)
	{
		s32 col;

		for (col = 0; col < 6; col++)
		{
			*entry++ = (row * 8 + 16 - fine) | ((col * 32 + 24) << 16) | 0x40004000;
			*entry++ = tile;
			tile += 4;
			if (tile == 0x300)
				tile = 0;
		}
	}
	Dma_Set(SCROLL_OBJECTS, OAM, 0x84000100, REG_DMA3);
	if ((*(s16 *)&Flash_Handler0) == 0 && (gFrameTick & 3) == 0)
		Flash_Handler3++;
}

void DisplayScroll_RenderEnteringLine(void)
{
	u32 current;
	s32 line;

	if ((*(s16 *)&Flash_Handler0) == 0)
	{
		current = Flash_Handler3;
		if ((s16)Flash_Handler3 / 8 != *(s16 *)&gFlash / 8)
		{
			line = (s16)Flash_Handler3 / 8;
			*(s16 *)&gFlash = current;
			(*(s16 *)&Flash_Handler0) =
				DisplayScroll_DrawLine(DisplayScroll_LineTable[line], ((line + 16) & 31) * 24, 1);
		}
	}
}

void DisplayScroll_InitObjectTable(void)
{
	volatile u32 fill;
	u32			*entry;
	u32			 i;
	u32			 j;

	SCROLL_OBJECTS = Runtime_BumpAllocateAlternatePool(128 * sizeof(struct DisplayScrollObject));
	Dma_Fill32Cell(fill, 0, (void *)0x06010000, 0x6000);
	Dma_Fill32Cell(fill, 0x11111111, (void *)0x06016000, 0x100);

	entry = (u32 *)SCROLL_OBJECTS;
	for (i = 0; i < 8; i++)
	{
		*entry++ = (i << 21) | 0x80004000;
		*entry++ = 0x300;
	}
	for (i = 0; i < 8; i++)
	{
		*entry++ = (i << 21) | 0x80004088;
		*entry++ = 0x300;
	}
	for (i = 0; i < 8; i++)
	{
		*entry++ = (i << 21) | 0x40004098;
		*entry++ = 0x300;
	}
	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 6; j++)
		{
			u32 tile = (i * 3) * 8 + j * 4;
			*entry++ = (i * 8 + 16) | ((j * 32 + 24) << 16) | 0x40004000;
			*entry++ = tile;
		}
	}
	for (i = 0; i < 8; i++)
	{
		*entry++ = 0x00c000c0;
		*entry++ = 0x300;
	}
	Flash_Handler3 = 0;
	*(s16 *)&gFlash = 0;
	(*(s16 *)&Flash_Handler0) = 0;
	Scheduler_AddOrUpdateCallback(DisplayScroll_UpdateObjects, 0x480);
	Scheduler_AddOrUpdateCallback(DisplayScroll_RenderEnteringLine, 0xc80);
	for (i = 0; i < 32; i++)
		DisplayScroll_DrawLine(DisplayScroll_LineTable[0], i * 24, 1);
}

s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align)
{
	u8		 *buf = Runtime_BumpAllocateAlternatePool(0x900);
	s32		  x = 0;
	s32		  width = 192;
	const u8 *font = DisplayScroll_Font;
	s32		  tiles;
	s32		  pos;
	const u8 *p;
	u8		 *dst;
	u32		  c;
	s32		  glyph;
	s32		  w;
	s32		  row;
	s32		  bit;
	u32		  bits;
	u32		  mask;

	/* FAKEMATCH: unused local; its 32 bytes are the native frame space no instruction touches. Without it differ 4 on tbs-ja, tbs-en and tbs-de: sub sp, #12 for #44 and the matching add. */
	u32 unused[8];

	if (text == NULL)
		return -1;
	if (!GameFlag_TestFar(0x200))
	{
		Iwram_FillWords(buf, 0x900, 0);
		GameFlag_SetBitFar(0x200);
	}
	else
	{
		Iwram_CopyWords(buf, buf + 0x800, 0x100);
		Iwram_FillWords(buf + 0x100, 0x800, 0);
	}

	p = text;
	pos = 0;
	while ((c = *p++) != 0)
	{
		if (c > 31)
			pos += DisplayScroll_GlyphWidths[c - 32];
	}
	if (align == 2)
		x = width - pos;
	else if (align == 1)
		x = (width - pos) / 2;

	// p is set to the text again, though nothing reads it before the glyph rows; the first character is fetched through it
	p = text;
	pos = 0;
	while ((c = *text++) != 0)
	{
		if (c > 31)
		{
			glyph = c - 32;
			p = font + glyph * 8;
			dst = buf + x + pos;
			for (row = 0; row < 8; row++)
			{
				bits = *p++;
				mask = 0x80;
				for (bit = 7; bit >= 0; bit--)
				{
					if (bits & mask)
					{
						dst[0x101] = 1;
						dst[0] = 15;
					}
					dst++;
					mask >>= 1;
				}
				dst += 248;
			}
			w = 1;
			if (c > 31)
				w = DisplayScroll_GlyphWidths[glyph];
			pos += w;
		}
	}

	tiles = width / 8;
	p = buf;
	dst = buf;
	for (row = 0; row < 8; row++)
	{
		for (bit = 0; bit < tiles * 4; bit++)
		{
			c = *p++;
			c |= *p++ << 4;
			*dst++ = c;
		}
		dst += 256 - tiles * 4;
		p += 256 - tiles * 8;
	}

	for (row = 0; row < tiles; row++)
	{
		s32	 tile = slot + row;
		u32 *words = (u32 *)buf + row;

		*(u32 *)(0x06010000 + tile * 32) = words[0x000];
		*(u32 *)(0x06010004 + tile * 32) = words[0x040];
		*(u32 *)(0x06010008 + tile * 32) = words[0x080];
		*(u32 *)(0x0601000c + tile * 32) = words[0x0c0];
		*(u32 *)(0x06010010 + tile * 32) = words[0x100];
		*(u32 *)(0x06010014 + tile * 32) = words[0x140];
		*(u32 *)(0x06010018 + tile * 32) = words[0x180];
		*(u32 *)(0x0601001c + tile * 32) = words[0x1c0];
	}
	MemFree(buf);
	return 0;
}
