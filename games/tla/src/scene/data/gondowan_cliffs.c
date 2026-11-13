/// @file
/// The Gondowan Cliffs' tables, which the scene's getters return.

#include "types.h"
#include "flags.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "objcmd.h"

// Set as Frost freezes each puddle into a pillar of ice, for this visit.
#define FLAG_FIRST_PUDDLE_FROZEN 0x201
#define FLAG_SECOND_PUDDLE_FROZEN 0x202

void GondowanCliffs_FreezeSecondPuddle(void);
void GondowanCliffs_BlowAwayFoliage(void);
extern s32 gGondowanCliffs_ResetTileSpriteScript[];

void TileSprite_Reset(void);
void GondowanCliffs_FightKindle(void);
void GondowanCliffs_PushPillarIntoWater(void);
void GondowanCliffs_FreezeFirstPuddle(void);
void GondowanCliffs_ScoopUpGeyser(void);
void GondowanCliffs_TalkToDog(void);
void GondowanCliffs_GrindTheRock(void);

extern u8 gFieldEffect_ShortScript[];
extern u8 gFieldEffect_MediumScript[];
extern u8 gFieldEffect_LongScript[];

s32 gGondowanCliffs_ResetTileSpriteScript[] = {
    SC_CALL(TileSprite_Reset),
    SC_END,
};

// clang-format off
s16 gGondowanCliffs_Entrances[] = {
    0, -1, 680, 0, 280, 0x4000, 0, -1,
    -1, -1, -1, 0, 1, -1, 536, 0,
    488, 0, 0, 512, 0, 0x800, 864, 0,
    2, -1, 0x7e8, 0, 792, 0x8000, 0, 512,
    0, 0x800, 864, 0, 3, -1, 0x628, 0,
    680, 0x4000, 0, 512, 0, 0x800, 864, 0,
    99, -1, 0x4f8, 0, 552, 0x4000, 0, 512,
    0, 0x800, 864, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

struct ActorPlacement gGondowanCliffs_PassEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0x40000118, PIXELS(-1), -0x1, 0xffff, 0, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(102),
    SCENE_EXIT_IF(1, 2, 21, -1),
    SCENE_EXIT_IF(2, 2, 22, -1),
    SCENE_EXIT_IF(3, 104, 1, -1),
    SCENE_EXITS(103),
    SCENE_EXIT_IF(1, 2, 85, -1),
    SCENE_EXIT_IF(2, 2, 86, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gGondowanCliffs_ActorPlacements[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(1560), 0, PIXELS(760), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(1624), 0, PIXELS(760), FACING_NORTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(1304), 0, PIXELS(456), FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(1880), 0, PIXELS(568), FACING_SOUTH, 0, 0},
    {307, CONDITION_ALWAYS, 1, PIXELS(1576), 0, PIXELS(672), FACING_NORTH, 2, 0},
    {273, CONDITION_ALWAYS, 1, PIXELS(1232), 0, PIXELS(544), FACING_SOUTH, 2, 0},
    {189, CONDITION_ALWAYS, 1, PIXELS(1928), 0, PIXELS(728), FACING_WEST, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {83, 0x8ab, 1, PIXELS(888), 0, PIXELS(200), FACING_WEST, 1, 0},
    {83, 0x8ab, 1, PIXELS(888), 0, PIXELS(232), FACING_WEST, 1, 0},
    {424, FLAG_GONDOWAN_CLIFFS_ROCK_GROUND, 1, PIXELS(816), 0, PIXELS(232), FACING_WEST, 2, 0},
    {245, 0x61, 1, PIXELS(1336), 0, PIXELS(648), FACING_SOUTH, 0, 0},
    {5, IF_FLAG_SET(0x8f3), 1, 0, 0, 0, FACING_EAST, 0, 0},
    {6, IF_FLAG_SET(0x8f3), 1, 0, 0, 0, FACING_EAST, 0, 0},
    {56, IF_FLAG_SET(0x8f3), 1, 0, 0, 0, FACING_EAST, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gGondowanCliffs_PassActorPlacements[] = {
    {8, CONDITION_ALWAYS, (s32)gGondowanCliffs_ResetTileSpriteScript, PIXELS(330), 0, PIXELS(79), FACING_EAST, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {424, FLAG_GONDOWAN_CLIFFS_ROCK_GROUND, 1, PIXELS(304), 0, PIXELS(232), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct Trigger gGondowanCliffs_PassTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 11, CONDITION_ALWAYS, EVENT(GondowanCliffs_GrindTheRock)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGondowanCliffs_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, IF_FLAG_SET(FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN), 3},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(GondowanCliffs_TalkToDog)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_GONDOWAN_CLIFFS_GEYSER_DUG, MSGNO(MsgDigHereYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgIKnewIt)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgITriedTo)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgIWonderWho)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgIfTheOther)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgILongTo)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(GondowanCliffs_FightKindle)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 10, 0x200, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, FLAG_FIRST_PUDDLE_FROZEN, EVENT(GondowanCliffs_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, FLAG_SECOND_PUDDLE_FROZEN, EVENT(GondowanCliffs_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14, CONDITION_ALWAYS, EVENT(GondowanCliffs_BlowAwayFoliage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, FLAG_GONDOWAN_CLIFFS_PILLAR_IN_WATER, EVENT(GondowanCliffs_PushPillarIntoWater)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, FLAG_GONDOWAN_CLIFFS_GEYSER_DUG, EVENT(GondowanCliffs_ScoopUpGeyser)},
    {TRIGGER_TABLE_END},
};

s32 gGondowanCliffs_MapRects[] = {
    0x2000200, 0x3600400, 0x600, 0x1601000, -1, -1,
};
// clang-format on
