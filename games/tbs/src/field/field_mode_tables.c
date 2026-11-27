/* Effect tables: actor scripts, curves and lookup tables of the field mode
   module. */

#include "field/actor_script.h"
#include "field/script_commands.h"
#include "field/line_scroll_wave_sine.h"
#include "field/scene_tables.h"
#include "field/speaker.h"
#include "field/sprite.h"

/* Their readers declare them in battle.h, field.h, graphics.h, line_scroll.h,
   field_effect_scripts.h and field_mode_internal.h, and
   field_mode_palette_fade.c declares the colour curves. */

/* The message of each searchable metatile behavior from 242 to 247, as an
   offset from MsgTileChecked and MsgTileResult. */
const Uint8 FieldExamineCueMessageOffsets[6] = {
	2,	/* a jar */
	3,	/* a barrel */
	13, /* a wooden box */
	8,	/* a grave */
	30, /* books */
	31, /* a well */
};

/* {field Psynergy kind, range in pixels}, ended by -1; a kind not listed
   reaches 16 pixels. */
/* clang-format off */
const Sint16 FieldEffectTargetRangeByMode[25] = {
	FIELD_PSYNERGY_PLY, 16,
	FIELD_PSYNERGY_FROST, 16,
	FIELD_PSYNERGY_LIFT, 16,
	FIELD_PSYNERGY_CARRY, 16,
	FIELD_PSYNERGY_GROWTH, 16,
	FIELD_PSYNERGY_MIND_READ, 32,
	FIELD_PSYNERGY_MOVE, 32,
	FIELD_PSYNERGY_WHIRLWIND, 32,
	FIELD_PSYNERGY_DOUSE, 32,
	FIELD_PSYNERGY_HALT, 32,
	FIELD_PSYNERGY_FORCE, 48,
	FIELD_PSYNERGY_CATCH, 48,
	-1,
};

/* The palette cycle of Mind Read's caster (fdActorSetLayersColorMode, */
/* (TickCount >> 1) & 7): palettes 1 to 4 and back to 0, two ticks each. */
const Uint8 MindReadCasterPaletteCycle[8] = {1, 2, 3, 4, 3, 2, 1, 0};

/* Grows from half size to full size, a step every 2 ticks. */
const Sint32 ItemDisplayGrowInScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(1) / 2),
	SC_SET(SW_SCALE_Y, toFIXED(1) / 2),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(3) / 4),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 4),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(7) / 8),
	SC_SET(SW_SCALE_Y, toFIXED(7) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(15) / 16),
	SC_SET(SW_SCALE_Y, toFIXED(15) / 16),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_END,
};

/* Pulses twice, to 1.5 times full size, then rests, over and over: the found */
/* item's picture held over the leader (field_mode_item_display.c). */
const Sint32 ItemDisplayPulseScript[] = {
	SC_WAIT(10),
	SC_SET(SW_SCALE_X, toFIXED(11) / 8),
	SC_SET(SW_SCALE_Y, toFIXED(11) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(3) / 2),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(9) / 8),
	SC_SET(SW_SCALE_Y, toFIXED(9) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(11) / 8),
	SC_SET(SW_SCALE_Y, toFIXED(11) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(3) / 2),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(9) / 8),
	SC_SET(SW_SCALE_Y, toFIXED(9) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_WAIT(2),
	SC_WAIT(50),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

/* Sinks a quarter pixel a tick for 60 ticks, then disappears. */
const Sint32 FieldEffectParticleScript[] = {
	SC_ADD(SW_Y, -toFIXED(1) / 4),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_RELEASE,
};

/* A Reveal marker: stays while fdIsRevealActive returns nonzero (SC_CALL */
/* runs it again each tick), and releases itself once Reveal ends. */
const Sint32 RevealMarkerScript[] = {
	SC_CALL(fdIsRevealActive),
	SC_RELEASE,
};

/* The width of the bars of WINDOW_TRANSITION_BARS, by progress. */
const Uint8 ScreenTransitionNoiseWidths[34] = {
	0, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 7, 8, 9, 11, 13,
	16, 18, 22, 26, 30, 36, 43, 51, 60, 72, 85, 101, 120, 143, 170, 202, 240,
};

/* The wobble of the circle edge of WINDOW_TRANSITION_RIPPLE. */
const Sint8 ScreenTransitionEdgeOffsets[32] = {
	-4, -3, -3, -3, -2, -2, -1, 0, 0, 0, 1, 2, 2, 3, 3, 3,
	4, 3, 3, 3, 2, 2, 1, 0, 0, 0, -1, -2, -2, -3, -3, -3,
};

/* 0 to 63 shuffled: the order in which the dissolve sets the pixels. */
const Uint8 ScreenTransitionDitherTable[64] = {
	0, 12, 35, 3, 29, 39, 6, 7, 50, 59, 4, 11, 18, 40, 46, 31,
	16, 42, 47, 51, 20, 23, 43, 32, 8, 52, 17, 41, 54, 15, 63, 24,
	58, 5, 2, 26, 36, 61, 9, 34, 56, 27, 10, 48, 53, 30, 28, 22,
	62, 45, 19, 60, 57, 49, 25, 55, 38, 14, 44, 13, 33, 37, 1, 21,
};

/* Colour channel curves of the palette tints (field_mode_palette_fade.c): */
/* each maps a 5-bit channel (0 to 31) to a palette fade channel, shifted up */
/* 10 bits, so 31 is 31 << 10 (31744). */
const Uint16 ColorBufferChannelCurve[32] = {
	0, 524, 1071, 1643, 2240, 2863, 3513, 4191,
	4898, 5635, 6403, 7202, 8035, 8901, 9803, 10740,
	11715, 12729, 13783, 14877, 16014, 17195, 18422, 19694,
	21015, 22386, 23808, 25283, 26812, 28397, 30041, 31744,
};

const Uint16 ColorBufferChannelCurveLight[32] = {
	0, 613, 1247, 1902, 2580, 3281, 4005, 4753,
	5526, 6324, 7148, 7999, 8876, 9782, 10716, 11680,
	12674, 13699, 14755, 15843, 16965, 18121, 19312, 20538,
	21801, 23101, 24440, 25818, 27237, 28697, 30199, 31744,
};

const Uint16 ColorBufferChannelCurveContrast[32] = {
	0, 360, 759, 1201, 1689, 2226, 2817, 3466,
	4178, 4957, 5808, 6738, 7752, 8856, 10058, 11365,
	12784, 14325, 15996, 17806, 19767, 21888, 24183, 26662,
	29340, 31744, 31744, 31744, 31744, 31744, 31744, 31744,
};

/* Speaker definitions: {sprite, voice, the lines it says while Halted}, each */
/* voice one of the eight that start at SE_VOICE_FIRST, four blips apart. */
const SPEAKER_DEFINITION SpeakerDefinitions[131] = {
	{SPRITE_ISAAC, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_GARET, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_IVAN, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_MIA, SPEAKER_VOICE(SE_VOICE_YOUNG_WOMAN_A), 0},
	{FELIX, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_JENNA, SPEAKER_VOICE(SE_VOICE_YOUNG_WOMAN_A), 0},
	{SPRITE_SHEBA, SPEAKER_VOICE(SE_VOICE_YOUNG_WOMAN_A), 0},
	{SPRITE_ISAAC_CHILD, SPEAKER_VOICE(SE_VOICE_BOY_A), 0},
	{SPRITE_GARET_CHILD, SPEAKER_VOICE(SE_VOICE_BOY_A), 0},
	{SPRITE_KRADEN, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), 0},
	{SPRITE_DORA, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_SATUROS, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_MENARDI, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_FELIX_MASKED, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_FELIX, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_FELIX_CHILD, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_BOY_WITH_ORANGE_HAIR, SPEAKER_VOICE(SE_VOICE_BOY_A), 0},
	{SPRITE_WOMAN_IN_GREEN_HEADSCARF, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_FEIZHI, SPEAKER_VOICE(SE_VOICE_YOUNG_WOMAN_A), 0},
	{SPRITE_FEH, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_HSU, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, SPEAKER_VOICE(SE_VOICE_YOUNG_WOMAN_A), 0},
	{SPRITE_ALEX, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_DONPA, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_LAYANA, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_KAJA, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_JENNA_CHILD, SPEAKER_VOICE(SE_VOICE_GIRL_A), 0},
	{SPRITE_HAMMET, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), 0},
	{SPRITE_DODONPA, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_NYUNPA, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), 0},
	{SPRITE_BABI, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_KYLE, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_MAN_IN_TEAL_SHIRT, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_TRET, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), 0},
	{SPRITE_TRET_RAISED, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_LAUREL, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_IODEM, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_MCCOY, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_MAN_IN_GREEN_SHIRT, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_WOMAN_WITH_PINK_HAIR, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_HAMA, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_WARRIOR_IN_GOLDEN_HELMET, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_MAN_WITH_LONG_DARK_HAIR, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_PERSON_IN_PURPLE, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_INVISIBLE_BABI, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), 0},
	{SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_YOUNG_MAN_WITH_RED_HAIR, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_BUNZA, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), 0},
	{SPRITE_WOMAN_WITH_PURPLE_HAIR, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_JUPITER_DJINNI, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_VENUS_DJINNI, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_MARS_DJINNI, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_MERCURY_DJINNI, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_BURLY_MAN_WITH_GREY_HAIR, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_GIRL_WITH_PIGTAILS, SPEAKER_VOICE(SE_VOICE_GIRL_A), 0},
	{SPRITE_OLD_MAN_WITH_CANE, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), 0},
	{SPRITE_DOG, SPEAKER_VOICE(SE_VOICE_FIRST), 0},
	{SPRITE_WOMAN_WITH_FAIR_HAIR, SPEAKER_VOICE(SE_VOICE_YOUNG_WOMAN_A), 0},
	{SPRITE_BOY_WITH_FAIR_HAIR, SPEAKER_VOICE(SE_VOICE_BOY_A), 0},
	{SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_STOUT_MAN, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_OLD_WOMAN, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_BALD_MUSCULAR_MAN, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_WOMAN_IN_GREEN_DRESS, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_STRONGMAN, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_OLD_MAN_WITH_WHITE_HAIR, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), 0},
	{SPRITE_WOMAN_WITH_PONYTAIL, SPEAKER_VOICE(SE_VOICE_YOUNG_WOMAN_A), 0},
	{SPRITE_MAN_WITH_DARK_HAIR, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_WOMAN_IN_BLUE_DRESS, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_MAN_IN_TALL_HAT, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), 0},
	{SPRITE_BEARDED_MAN_IN_LONG_ROBE, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_COOK, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_BALD_MAN_IN_RED, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_YOUNG_MAN_IN_GREEN, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_WOMAN_IN_PINK_APRON, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_MAN_WITH_LUTE, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_YOUTH_WITH_HEADBAND, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_YOUTH_WITH_TIED_HAIR, SPEAKER_VOICE(SE_VOICE_YOUNG_WOMAN_A), 0},
	{SPRITE_CHEF, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_MAN_WITH_ORANGE_HAIR, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_MAN_IN_ORANGE, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_MAN_WITH_PICKAXE, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_CAT, SPEAKER_VOICE(SE_VOICE_FIRST), 0},
	{SPRITE_MAN_IN_RED_BANDANA, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_MAN_IN_BLUE_BANDANA, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_MAN_IN_YELLOW_BANDANA, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_MAN_IN_GREEN_BANDANA, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_YOUNG_WOMAN_IN_GREEN, SPEAKER_VOICE(SE_VOICE_YOUNG_WOMAN_A), 0},
	{SPRITE_MONK_IN_WHITE_AND_ORANGE, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_SOLDIER_IN_SPIKED_HELMET, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_BALD_BOY_IN_ORANGE, SPEAKER_VOICE(SE_VOICE_BOY_A), 0},
	{SPRITE_SOLDIER, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_SOLDIER_IN_TURBAN, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_BIRD, SPEAKER_VOICE(SE_VOICE_FIRST), 0},
	{SPRITE_YOUNG_MAN_IN_BLUE_JACKET, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_PERSON_WITH_SWORD, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_MONK_IN_ORANGE_ROBE, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_WOMAN_WITH_BLACK_HAIR, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_MAN_WITH_TOPKNOT, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_WOMAN_IN_GREEN_APRON, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_SOLDIER_WITH_TORCH, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_OLD_MAN_IN_BLUE, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), 0},
	{SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
	{SPRITE_OLD_WOMAN_WITH_CANE, SPEAKER_VOICE(SE_VOICE_WOMAN_A), 0},
	{SPRITE_MAN_IN_BLACK_SUIT, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), 0},
	{SPRITE_BOY_IN_GREEN, SPEAKER_VOICE(SE_VOICE_BOY_A), 0},
	{SPRITE_GIRL_IN_PINK, SPEAKER_VOICE(SE_VOICE_GIRL_A), 0},
	{SPRITE_MUSCULAR_MAN_WITH_TOPKNOT, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_MAN_CARRYING_A_PACK, SPEAKER_VOICE(SE_VOICE_MAN_A), 0},
	{SPRITE_TOADONPA, SPEAKER_VOICE(SE_VOICE_MAN_A), SPEAKER_HALTED_LINES_NONE},
	{SPRITE_JELLYFISH_MONSTER, SPEAKER_VOICE(SE_VOICE_WOMAN_A), SPEAKER_HALTED_LINES_NONE},
	{SPRITE_WATER_BEAST, SPEAKER_VOICE(SE_VOICE_MAN_A), SPEAKER_HALTED_LINES_NONE},
	{SPRITE_APE, SPEAKER_VOICE(SE_VOICE_MAN_A), SPEAKER_HALTED_LINES_NONE},
	{SPRITE_LIZARD_MAN, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), SPEAKER_HALTED_LINES_NONE},
	{SPRITE_ANT_LION, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), SPEAKER_HALTED_LINES_NONE},
	{SPRITE_KRAKEN, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), SPEAKER_HALTED_LINES_NONE},
	{SPRITE_MANTICORE, SPEAKER_VOICE(SE_VOICE_MAN_A), SPEAKER_HALTED_LINES_NONE},
	{SPRITE_PINK_WINGED_MONSTER, SPEAKER_VOICE(SE_VOICE_FIRST), SPEAKER_HALTED_LINES_NONE},
	{SPRITE_BLUE_WINGED_MONSTER, SPEAKER_VOICE(SE_VOICE_YOUNG_WOMAN_A), SPEAKER_HALTED_LINES_NONE},
	{SPRITE_LIZARD_MONSTER, SPEAKER_VOICE(SE_VOICE_MAN_A), SPEAKER_HALTED_LINES_NONE},
	{SPRITE_HYDROS_STATUE, SPEAKER_VOICE(SE_VOICE_OLD_MAN_A), SPEAKER_HALTED_LINES_NONE},
	{SPRITE_CHEST, SPEAKER_VOICE_NONE, SPEAKER_HALTED_LINES_NONE},
	{SPRITE_ROCK, SPEAKER_VOICE_NONE, SPEAKER_HALTED_LINES_NONE},
	{SPRITE_TREE, SPEAKER_VOICE_NONE, SPEAKER_HALTED_LINES_NONE},
	{SPRITE_SMALL_TREE, SPEAKER_VOICE_NONE, SPEAKER_HALTED_LINES_NONE},
	{SPRITE_SIGNPOST, SPEAKER_VOICE_NONE, SPEAKER_HALTED_LINES_NONE},
	{SPRITE_MINE_CART, SPEAKER_VOICE_NONE, SPEAKER_HALTED_LINES_NONE},
	{SPRITE_TURNING_ROCK, SPEAKER_VOICE_NONE, SPEAKER_HALTED_LINES_NONE},
	{SPRITE_BLANK, SPEAKER_VOICE_NONE, SPEAKER_HALTED_LINES_NONE},
	{SPRITE_ISAAC, SPEAKER_VOICE(SE_VOICE_YOUNG_MAN_A), 0},
};

/* Squashes as many times as the variant (1 to 3): variant v starts at word */
/* (3 - v) * 32 and runs to the end. field.h declares it const Uint8[], and */
/* script_commands.c adds (3 - v) << 7 bytes and reads it through an Sint32 *. */
const Sint32 ActorVariantScripts[97] = {
	/* variant 3 */
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 4),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(4),
	/* variant 2 */
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 4),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(4),
	/* variant 1 */
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 4),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(4),
	SC_END,
};

/* A blinking actor's colour mode by tick ((TickCount >> 1) & 3, */
/* fdActorUpdateBlink): white for two ticks out of eight. */
const Uint8 ActorBlinkSpriteColors[4] = {
	SPRITE_COLOR_WHITE, SPRITE_COLOR_NORMAL, SPRITE_COLOR_NORMAL, SPRITE_COLOR_NORMAL,
};

const Sint16 LineScrollWaveSine[256] = {
	LINE_SCROLL_WAVE_SINE};

/* {dx, dy, tile, shape, size} for each of the 16 frames of a raindrop's */
/* splash (script_commands_weather.c): dx and dy in screen pixels from its */
/* spot, tile from its first tile. */
const Sint16 WeatherRainSplashFrames[80] = {
	-8, -12, 20, 0, 1,
	-8, -12, 20, 0, 1,
	-8, -12, 20, 0, 1,
	-8, -12, 16, 0, 1,
	-8, -12, 16, 0, 1,
	-8, -12, 12, 0, 1,
	-8, -12, 8, 0, 1,
	0, -32, 0, 2, 2,
	8, -48, 0, 2, 2,
	16, -64, 0, 2, 2,
	24, -80, 0, 2, 2,
	32, -96, 0, 2, 2,
	40, -112, 0, 2, 2,
	48, -128, 0, 2, 2,
	56, -144, 0, 2, 2,
	64, -160, 0, 2, 2,
};

/* {dy, tile} of a sparkle (script_commands_weather.c): dy in screen pixels */
/* from its spot, a row every two ticks. */
const SPARKLE_PICTURE FieldEffectSparklePictures[32] = {
	{-2, 12},
	{-4, 12},
	{-6, 12},
	{-8, 8},
	{-10, 8},
	{-12, 8},
	{-14, 8},
	{-16, 8},
	{-19, 4},
	{-21, 4},
	{-24, 4},
	{-27, 4},
	{-30, 4},
	{-33, 4},
	{-36, 4},
	{-40, 4},
	{-44, 4},
	{-48, 0},
	{-52, 0},
	{-56, 0},
	{-60, 0},
	{-64, 0},
	{-68, 0},
	{-72, 0},
	{-76, 0},
	{-80, 0},
	{-85, 0},
	{-90, 0},
	{-95, 0},
	{-100, 0},
	{-110, 0},
	{-120, 0},
};

/* Two scales that alternate ((FrameCount >> 2) & 1): full size and 0.97 of */
/* it (fdEffectPulseActorScale, script_commands_jupiter.c); nothing reads the */
/* third word. */
const Uint32 FieldEffectPulseScales[3] = {toFIXED(1), toFIXED(97) / 100, 16};

/* [0] only releases the actor, +1 waits for it to arrive and releases it; +3 */
/* is FieldEffectSourceHoldScript, which follows. */
const Sint32 FieldEffectSharedParticleScript[] = {
	SC_RELEASE,
	SC_SYNC,
	SC_RELEASE,
};

/* Faces the watched actor every tick, forever: the caster holding its */
/* target while Move, Carry or Lift has it (each gives the caster its target */
/* first). */
const Sint32 FieldEffectSourceHoldScript[] = {
	SC_FACE_WATCHED,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* A shard or particle: waits to arrive, slows to 1/16 pixel a tick, and */
/* releases itself 10 ticks later. */
const Sint32 FieldEffectFragmentScript[] = {
	SC_SYNC,
	SC_SPEED(0.0625),
	SC_ANIMATION(0),
	SC_WAIT(10),
	SC_RELEASE,
};

/* An angle for each value of the right, left, up and down bits of the */
/* +Control Pad; DPAD_ANGLE_NONE for none (fdInputGetDpadAngle, */
/* script_commands_move.c). */
const Uint16 InputDpadAngles[16] = {
	DPAD_ANGLE_NONE, FACING_EAST,     FACING_WEST,     DPAD_ANGLE_NONE,
	FACING_NORTH,    DPAD_ANGLE_NONE, DPAD_ANGLE_NONE, FACING_NORTH,
	FACING_SOUTH,    DPAD_ANGLE_NONE, DPAD_ANGLE_NONE, FACING_SOUTH,
	DPAD_ANGLE_NONE, FACING_EAST,     FACING_WEST,     DPAD_ANGLE_NONE,
};

/* Move's hand animation by the quarter turns from its side of the target to */
/* the push (script_commands_move.c): 3 toward its side, 2 away from it, 1 */
/* either way across. */
const Uint8 FieldEffectQuadrantModes[4] = {3, 1, 2, 1};

/* Waits 60 ticks and releases the actor. Word +4 is */
/* ForceFistScript. */
const Sint32 GrowthLeafScript[] = {
	SC_WAIT(60),
	SC_RELEASE,
};

/* The same four bytes as FieldEffectQuadrantModes; nothing reads them. */
const Uint8 UnusedBattleEffectBytes[4] = {3, 1, 2, 1};

/* Waits to arrive, sets off fdEffectSpawnHeavyImpact */
/* (script_commands_lift.c) and releases the actor. */
const Sint32 ForceFistScript[] = {
	SC_SYNC,
	SC_CALL(fdEffectSpawnHeavyImpact),
	SC_RELEASE,
};

/* The three scripts of FieldEffectDurationScripts. */
const Sint32 FieldEffectShortEffectScript[] = {
	SC_WAIT(24),
	SC_RELEASE,
};

const Sint32 FieldEffectMediumEffectScript[] = {
	SC_WAIT(46),
	SC_RELEASE,
};

const Sint32 FieldEffectLongEffectScript[] = {
	SC_WAIT(128),
	SC_RELEASE,
};

/* The colour modes a Halted actor flickers through at random */
/* (fdPsynergyUpdateHaltedActor, script_commands_halt.c): its own colours */
/* twice as often as each of the five channel swaps and grey. */
const Sint8 HaltFlickerColorModes[8] = {
	SPRITE_COLOR_NORMAL, SPRITE_COLOR_NORMAL, SPRITE_COLOR_GRB, SPRITE_COLOR_RBG,
	SPRITE_COLOR_BGR, SPRITE_COLOR_GBR, SPRITE_COLOR_BRG, SPRITE_COLOR_GREY,
};

/* The World Map markers' blend weight in sixteenths, against the picture's */
/* 16 less it ((TickCount >> 1) & 31, fdMapScreenUpdateMarkers): solid */
/* for 32 ticks, then down to half and back over the next 32. */
const Uint8 WorldMapMarkerBlendCycle[32] = {
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 15, 15, 14, 12, 10, 9, 9, 8, 9, 9, 10, 12, 14, 14, 15,
};

/* An angle for each +Control Pad direction (eight of them); DPAD_ANGLE_NONE */
/* for none. */
const Uint16 WorldMapCursorDirectionAngles[16] = {
	DPAD_ANGLE_NONE, FACING_EAST, FACING_WEST, DPAD_ANGLE_NONE,
	FACING_NORTH, FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
	FACING_SOUTH, FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH,
	DPAD_ANGLE_NONE, FACING_EAST, FACING_WEST, DPAD_ANGLE_NONE,
};
/* clang-format on */
