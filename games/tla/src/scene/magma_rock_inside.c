// Magma Rock's inside, the rooms in the volcano that the great blast on its top opened. Three halls
// are each watched by a great stone face: Burst on the small statue beside it makes the statue spit
// a fireball into the face, whose eyes light red as it pours lava over the hall's floor, and a
// floor switch beside a grated gate lets the lava drain away again. While the lava is up, platforms
// float round it and Felix leaps from its edge onto them, or onto the pillars he has pushed into
// it. A Mars Djinni left over from the first journey may wait in the entrance hall's south, Fury
// waits in its own room, and the Lucky Medal, Mist Potion, Salamander Tail and Golem Core lie in
// chests. Deep inside, stepping onto an islet in the lava sets off a rock that bursts out of it,
// throwing burning rocks and flames round the room; a crest on the stone wall there opens into a
// stairway to a sea of lava crossed on stepping stones, where Jenna learns Blaze at the tablet.
// Blaze then carries a flame to a torch to sink the pillar on the way out, and back in the ember
// stone's room carries the fallen flames to the two torches that let the party reach the stone and
// take the Magma Ball from it.

#include "graphics/palette_buffers.h"
#include "scene/falling_rocks.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "item_ids.h"
#include "field/script_inline.h"
#include "field/field_mode.h"
#include "scene/magma_rock.h"
#include "scene/scene.h"
#include "game/game.h"
#include "macros.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

extern MAP_METATILE      MapGridMetatileBuffer[];

void                     fdSetActorSpritePriority(ACTOR *actor, Sint32 priority);

extern Sint32            MagmaRockInsideFlameScript[];

extern Sint32            MagmaRockInsideLastStepMetatile;

void                     scMagmaRockInsideBurstCrackedPillar(Sint32 stage, Sint32 sound);
void                     scMagmaRockInsideTrackStepMetatile(void);
void                     fdActorSetMetatileShape(Sint32 heightOffset, Sint32 layer, Sint32 owner);

// Creates an actor of the given sprite through the overlay's import.
static __inline__ ACTOR *Create(Sint32 kind, Sint32 x, Sint32 y, Sint32 z)
{
    return syImportActorCreate(kind, x, y, z);
}

#include "shared/trial_road.h"
#include "shared/tables.h"
#include "shared/tundaria_tower.h"

// The lava rises in three parts of the volcano, each when a statue's fireball strikes the great
// stone face there, and drains when a floor switch by that part's gate is pressed; the flags hold
// the state until the party comes back in from outside.
#define FLAG_ENTRANCE_FLOODED              FLAG_AREA_TEMP_00
#define FLAG_FURY_FLOODED                  FLAG_AREA_TEMP_01
#define FLAG_LOWER_ROOMS_FLOODED           FLAG_AREA_TEMP_02
#define FLAG_WEST_TORCH_LIT                FLAG_AREA_TEMP_03
#define FLAG_EAST_TORCH_LIT                FLAG_AREA_TEMP_04
#define FLAG_TABLET_TORCH_LIT              FLAG_AREA_TEMP_05
// Set as Burst breaks each of the cracked pillars' room's two pillars, until the party leaves Magma
// Rock.
#define FLAG_INSIDE_CRACKED_PILLAR_A_BURST FLAG_AREA_TEMP_50
#define FLAG_INSIDE_CRACKED_PILLAR_B_BURST FLAG_AREA_TEMP_51
// Each pushed pillar's flag keeps it where it was pushed.
#define FLAG_ENTRANCE_PILLAR_PUSHED        0x9f0
#define FLAG_FURY_PILLAR_PUSHED            0x9f1
#define FLAG_PILLAR_A_PUSHED               0x9f2
#define FLAG_PILLAR_B_PUSHED               0x9f3
#define FLAG_PILLAR_C_PUSHED               0x9f4
#define FLAG_NORTH_PILLAR_PUSHED           0x9f5
#define FLAG_EAST_PILLAR_PUSHED            0x9f6
#define FLAG_LEDGE_PILLAR_PUSHED           0x9f7
#define FLAG_ROCKS_RELEASED                0x9f8
#define FLAG_MAGMA_BALL_TAKEN              0x9f9
#define FLAG_BLAZE_LEARNED                 0x9fa
// The Blaze tablet's room: the map's upper layer numbers some of the stones on the way to the
// tablet; stopping on one raises the cue STEP_METATILE_CUE_BASE plus its number, and its flag then
// keeps the stone behind it gone.
#define FLAG_ANY_MARKER_LIT                FLAG_SCENE_TEMP_00
#define FLAG_MARKER_LIT(marker)            (FLAG_SCENE_TEMP_00 + (marker))

// The cue a numbered stone of the Blaze tablet's room raises, less its number.
enum
{
    STEP_METATILE_CUE_BASE = 200
};

// The numbers the Blaze tablet room's upper layer gives the metatiles of column 16 on the way to
// the tablet: the end of the entrance's walkway (row 26), then a stone every second row, up to the
// foot of the tablet's platform (row 14). The five stones between are the ones that fade away.
enum
{
    MARKER_WALKWAY_END   = 1,
    MARKER_PLATFORM_FOOT = 7
};

// The metatile behaviours the rooms' maps mark their spots with, which the cue tables answer. A
// room may use a number in its own sense.
typedef enum MAGMA_ROCK_INSIDE_METATILE_BEHAVIOR
{
    // A floor switch beside a lava gate; the Golem Core's room has a second.
    METATILE_FLOOR_SWITCH        = 30,
    METATILE_SECOND_FLOOR_SWITCH = 31,
    // The ember stone's room: the islets in the lava that set the stone off, and the stone where it
    // lands.
    METATILE_ERUPTION_ISLET      = 30,
    METATILE_EMBER_STONE         = 31,
    // The Blaze tablet, read from any side.
    METATILE_BLAZE_TABLET        = 30,
    // The metatile before a fire-spitting statue that Burst is cast at.
    METATILE_STATUE              = 50,
    METATILE_LOWER_WEST_STATUE   = 51,
    METATILE_LOWER_EAST_STATUE   = 52,
    // The lava's edge, where Felix leaps onto what stands in the lava; the cracked pillars' room's
    // two pillars stand on edge metatiles of their own.
    METATILE_LAVA_EDGE           = 70,
    METATILE_CRACKED_PILLAR_A    = 71,
    METATILE_CRACKED_PILLAR_B    = 72,
    METATILE_LAVA_EDGE_NORTH     = 73, // leapt from walking north
    METATILE_LAVA_EDGE_WEST      = 75, // leapt from walking west
                                       // the far side of the Lucky Medal room's lava
    METATILE_LAVA_EDGE_EAST      = 78,
    // The Blaze tablet room's stepping stones, two pictures of them in turn.
    METATILE_STEPPING_STONE_A    = 80,
    METATILE_STEPPING_STONE_B    = 81
} MAGMA_ROCK_INSIDE_METATILE_BEHAVIOR;

// The cracked pillars' room's entrances up to this one open onto its upper hall, whose lava rises
// with Fury's room's; the rest onto its lower rooms.
enum
{
    CRACKED_PILLARS_UPPER_HALL_LAST_ENTRANCE = 7
};

// The Golem Core room's entrances up to this one open onto its north part, by the north floor
// switch; the rest onto its south part.
enum
{
    GOLEM_CORE_NORTH_LAST_ENTRANCE = 8
};

// The room past the Lucky Medal's chest: one lava platform.
typedef enum MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_ACTOR_ID
{
    ACTOR_LUCKY_MEDAL_PLATFORM = ACTOR_FIRST_PLACED
} MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_ACTOR_ID;

// The entrance hall: the small statue whose fireball sets the great face flooding it, the row of
// columns standing across its lava, the pillar pushed into that row, an unseen marker on its floor
// switch and the Mars Djinni "Fire19".
typedef enum MAGMA_ROCK_INSIDE_ENTRANCE_ACTOR_ID
{
    ACTOR_STATUE = ACTOR_FIRST_PLACED,
    ACTOR_COLUMN_A,
    ACTOR_COLUMN_B,
    ACTOR_COLUMN_C,
    ACTOR_COLUMN_D,
    ACTOR_COLUMN_E,
    ACTOR_COLUMN_F,
    ACTOR_ENTRANCE_PILLAR,
    ACTOR_ENTRANCE_SWITCH_MARKER,
    ACTOR_FIRE19 // the Mars Djinni left over from the first journey
} MAGMA_ROCK_INSIDE_ENTRANCE_ACTOR_ID;

// Fury's room: the statue, an unseen marker on its floor switch, two lava platforms, the pillar
// pushed into the lava, the tightrope's posts and Fury.
typedef enum MAGMA_ROCK_INSIDE_FURY_ACTOR_ID
{
    ACTOR_FURY_SWITCH_MARKER = ACTOR_FIRST_PLACED + 1,
    ACTOR_WEST_PLATFORM,
    ACTOR_FURY_PILLAR,
    ACTOR_EAST_PLATFORM,
    ACTOR_TIGHTROPE_WEST_POST,
    ACTOR_TIGHTROPE_EAST_POST,
    ACTOR_FURY
} MAGMA_ROCK_INSIDE_FURY_ACTOR_ID;

// The room of the cracked pillars: three statues, the two cracked pillars Burst breaks, ten lava
// platforms, three pillars to push and a column.
typedef enum MAGMA_ROCK_INSIDE_CRACKED_PILLARS_ACTOR_ID
{
    ACTOR_CRACKED_PILLAR_A  = ACTOR_FIRST_PLACED + 1,
    ACTOR_CRACKED_PILLAR_B  = ACTOR_FIRST_PLACED + 3,
    ACTOR_LOWER_WEST_STATUE = ACTOR_FIRST_PLACED + 5,
    ACTOR_LOWER_EAST_STATUE,
    ACTOR_LAVA_PLATFORM_A,
    ACTOR_LAVA_PLATFORM_B,
    ACTOR_LAVA_PLATFORM_C,
    ACTOR_LAVA_PLATFORM_D,
    ACTOR_LAVA_PLATFORM_E,
    ACTOR_LAVA_PLATFORM_F,
    ACTOR_LAVA_PLATFORM_G,
    ACTOR_LAVA_PLATFORM_H,
    ACTOR_PILLAR_A,
    ACTOR_PILLAR_B,
    ACTOR_PILLAR_C,
    ACTOR_LOWER_COLUMN
} MAGMA_ROCK_INSIDE_CRACKED_PILLARS_ACTOR_ID;

// The Golem Core's room: unseen markers on its two floor switches and the two chains hanging down
// its wall either side of its doorway, which Whirlwind swings.
typedef enum MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_ACTOR_ID
{
    ACTOR_NORTH_SWITCH_MARKER = ACTOR_FIRST_PLACED,
    ACTOR_SOUTH_SWITCH_MARKER,
    ACTOR_HANGING_CHAIN_A,
    ACTOR_HANGING_CHAIN_B
} MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_ACTOR_ID;

// The Salamander Tail's room: three lava platforms and two pillars to push.
typedef enum MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_ACTOR_ID
{
    ACTOR_NORTH_PLATFORM = ACTOR_FIRST_PLACED,
    ACTOR_NORTH_PILLAR,
    ACTOR_SOUTHWEST_PLATFORM,
    ACTOR_SOUTHEAST_PLATFORM,
    ACTOR_EAST_PILLAR
} MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_ACTOR_ID;

// The room of the ember stone: the sunken pillars, the pillar the two torches lower, the stone that
// holds the Magma Ball, the rocks and flames that fall with it, the torches' flames and a pillar to
// push.
typedef enum MAGMA_ROCK_INSIDE_EMBER_STONE_ACTOR_ID
{
    ACTOR_SUNKEN_PILLAR_A = ACTOR_FIRST_PLACED,
    ACTOR_SUNKEN_PILLAR_B,
    ACTOR_SUNKEN_PILLAR_C,
    ACTOR_BLOCKING_PILLAR,
    ACTOR_EMBER_STONE,
    ACTOR_FALLEN_ROCK_A,
    ACTOR_FALLEN_ROCK_B,
    ACTOR_FALLEN_ROCK_C,
    ACTOR_FALLEN_ROCK_D,
    ACTOR_FALLEN_FLAME_A,
    ACTOR_FALLEN_FLAME_B,
    ACTOR_FALLEN_FLAME_C,
    ACTOR_FALLEN_FLAME_D,
    ACTOR_WEST_TORCH_FLAME,
    ACTOR_EAST_TORCH_FLAME,
    ACTOR_LEDGE_PILLAR
} MAGMA_ROCK_INSIDE_EMBER_STONE_ACTOR_ID;

// The flag that keeps a torch of the ember stone's room lit, by its flame's actor:
// FLAG_WEST_TORCH_LIT or FLAG_EAST_TORCH_LIT.
#define FLAG_TORCH_LIT(flame) ((flame) - ACTOR_WEST_TORCH_FLAME + FLAG_WEST_TORCH_LIT)

// A torch Blaze lights: its flame starts a sixteenth of its size and grows by a sixteenth
// FLAME_GROW_STEPS times, 5 ticks apart; then the pillar the torches guard sinks PILLAR_SINK_DEPTH
// pixels into the floor, a pixel every 5 ticks, as deep as the ember stone room's sunken pillars
// stand.
enum
{
    FLAME_GROW_STEPS = 16
};

// The quarter turn a facing lies in, as Felix faces a switch or the lava: an eighth of a turn
// added, then this kept.
#define FACING_QUARTER_MASK (TRIG_TURN - DEGtoANG(90))

// What scMagmaRockInsideRaiseLava and scMagmaRockInsideDrainLava keep while the lava is already up;
// only its being nonzero is read.
enum
{
    LAVA_ALREADY_UP = 212
};

// A lava platform's path script is four corners, each a move and a wait for arrival, five words:
// the second platform of each pair of the cracked pillars' room starts at this word, the third
// corner, across from the first.
enum
{
    PLATFORM_PATH_THIRD_CORNER = 10
};

// The Blaze tablet's room: the tablet, and on the ledge in the lava's south-west the pillar in the
// way out, the flame burning in the west bowl beside it, which Blaze carries, and the flame of the
// east bowl it lights.
typedef enum MAGMA_ROCK_INSIDE_BLAZE_TABLET_ACTOR_ID
{
    ACTOR_BLAZE_TABLET = ACTOR_FIRST_PLACED,
    ACTOR_TORCH_PILLAR,
    ACTOR_SOURCE_FLAME,
    ACTOR_TORCH_FLAME
} MAGMA_ROCK_INSIDE_BLAZE_TABLET_ACTOR_ID;

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE   fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

#include "shared/position_distance.h"

#include "../field/actor_find_in_metatile.c"

struct ACTOR;
struct FIELD_SPR;
struct PARTICLE;
struct PARTICLE_SPR;

void          scMagmaRockInsidePausePlatform();

extern Sint16 MagmaRockInsideEntrancePillars[];

extern Sint16 MagmaRockInsideFuryPillars[];

extern Sint16 MagmaRockInsideCrackedPillarsPillars[];

extern Sint16 MagmaRockInsideEmberStonePillars[];

void          scBlocksMarkMetatiles(Uint16 *ids);

void          scMagmaRockInsideWaitForLanding(ACTOR *actor);

Sint32        fdMapMetatileGetAttributes(Uint32 layer, Sint32 x, Sint32 y);

Sint32        scStatueFindUnderBurst(Uint16 *table);

Sint32        scPosFlatDistance(FIXED *from, FIXED *to);

void          scMagmaRockInsideTrailEmbers(ACTOR *actor);
void          scMagmaRockInsideRaiseLava(void);

void          scMagmaRockInsideDropRocks(Sint32 x, Sint32 y, Sint32 z, Sint32 count);

void          scMagmaRockInsideStartFallingRocks(void);

void          scMagmaRockInsideSetEntranceFlooded(void);
void          scMagmaRockInsideSetFuryFlooded(void);
void          scMagmaRockInsideSetLowerRoomsFlooded(void);

extern Sint32 MagmaRockInsideCrackedPillarsFirstPairPath[];
extern Sint32 MagmaRockInsideCrackedPillarsSecondPairPath[];
extern Sint32 MagmaRockInsideCrackedPillarsThirdPairPath[];
extern Sint32 MagmaRockInsideCrackedPillarsFourthPairPath[];

void          scMagmaRockInsideRaiseActorMetatiles(Uint16 *list);

void          scDustScatter(Sint32 spread);
void          fdLeaderSetMetatileShapeAt(Sint32 heightOffset, Sint32 layer);
void          scMagmaRockInsideSpawnDustRing(void);
extern Uint32 DustScatterScript[];

extern Uint32 DustBurstScript[];

// Decompresses a tagged LZ resource through the overlay's import.
void          scMagmaRockInsideDecompressTaggedLZ(void *entry, void *dest);
// Sets up an effect sprite of the given size, attributes and tile, through the overlay's import.
void          scMagmaRockInsideInitRockSprite(
             EFFECT_SPR *sprite, Sint32 width, Sint32 height, Uint32 attributes, Sint32 tile);

void          scParticleUpdateFalling(void);

extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];

void          scMagmaRockInsideBurstCrackedPillar(Sint32 stage, Sint32 sound);

void          syImportFieldPsynergySetupActorPair(Sint32 owner, Uint16 value);
void          fdActorPlaySoundInView(ACTOR *actor, Sint32 sound);
Sint32 MagmaRockInsideStepMetatileWidth  BSS_DATA;
Sint32 MagmaRockInsideStepMetatileHeight BSS_DATA;
Sint32 MagmaRockInsideStepMetatileX      BSS_DATA;
Sint32 MagmaRockInsideStepMetatileZ      BSS_DATA;

extern Sint32                            PushLeaderSteps[];

#include "shared/block_push_onto.h"

#include "../field/map_metatile_fill_byte.c"

// Talking to Fury, the Mars Djinni on the ledge just west of the north doorway into its room's east
// hall, starts a battle with it.
void scMagmaRockInsideFightFury(void)
{
    fdRequestDjinnBattle(ACTOR_FURY, DJINNI_FURY);
}

// Talking to the Mars Djinni in the entrance hall's south (it stands at metatile (18, 31)) makes it
// join the party. It is always the Djinni the text calls "Fire19" (DJINNI_FIRE19), wearing the Mars
// Djinni's sprite, and it stands there only while that Djinni has not joined (its FLAG_DJINNI); it
// is kept for one of The Broken Seal's Mars Djinn the party never found there, and the walkthrough
// says such a Djinni may wait here.
void scMagmaRockInsideMeetDjinni(void)
{
    fdDjinnJoinParty(ACTOR_FIRE19, DJINNI_FIRE19 / DJINN_PER_ELEMENT, DJINNI_INDEX(DJINNI_FIRE19));
}

#include "shared/position_flat_distance.h"

#include "shared/map_metatile_set_shape.h"

// Waits up to 60 ticks for an actor's vertical speed to come to zero.
void scMagmaRockInsideWaitForLanding(ACTOR *actor)
{
    Sint32 ticks;
    Sint32 busy;

    ticks = 60;
    while (ticks != 0)
    {
        syWait(1);
        busy = actor->velocityY;
        ticks--;
        if (busy == 0)
        {
            break;
        }
    }
}

#include "../field/map_metatile_get.c"

#include "shared/map_metatile_set.c"

#include "shared/blocks_mark_metatiles.c"

// Clears the object's mark and the metatiles ahead of and behind it that the mark blocked.
#include "shared/block_clear_metatiles.h"

// A lava platform's update: holds it still while a menu, Psynergy, an item or a transition is in
// progress.
void scMagmaRockInsidePausePlatform(ACTOR *actor)
{
    FIELD_STATE *fieldState = RamHeapSlots->fieldState;
    Uint8       *state      = &actor->paused;

    if (fieldState->psynergy != 0 || fieldState->fieldMenu != 0 || fieldState->pauseMenu != 0
        || fieldState->item != 0 || fieldState->transitionActive == 0)
        *state = 1;
    else
        *state = 0;
}

// Sets a white lava platform afloat: risen and running round its path while flag (its part's lava)
// is set, sunk under the floor otherwise; its update holds it still during menus.
void scMagmaRockInsideStartLavaPlatform(Sint32 id, Sint32 *path, Sint16 flag)
{
    ACTOR *platform;
    Sint32 risen;

    platform = fdActorGetById(id);
    fdActorSelectAnimation(platform, 4);
    platform->steeringFlags  = 0;
    platform->speedLimit     = toFIXED(1) / 2;
    platform->acceleration   = toFIXED(1) / 2;
    platform->motionFlags    = ACTOR_FALLS;
    platform->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
    fdActorSetSpriteFlags(platform, 0);
    if (fdScriptGetFlag((Uint16)flag))
    {
        platform->groundHeight = toFIXED(16);
        platform->y.fixed      = toFIXED(16);
        if (!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
            fdActorStartScript(platform, path);
    }
    else
    {
        platform->groundHeight = -toFIXED(14);
        platform->y.fixed      = -toFIXED(14);
    }
    platform->update = (OBJECT_UPDATE)scMagmaRockInsidePausePlatform;
}

Sint32 scMagmaRockInsideSetMetatileShape(Sint32 terrain, Sint32 x, Sint32 z, Sint32 height);

#include "shared/actors_raise_metatiles.h"

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
    return 0;
}

const Uint32 *scGetExits(void)
{
    return SceneExits;
}

extern ACTOR_PLACE MagmaRockInsidePlacements[];
extern ACTOR_PLACE MagmaRockInsideEntrancePlacements[];
extern ACTOR_PLACE MagmaRockInsideFuryPlacements[];
extern ACTOR_PLACE MagmaRockInsideCrackedPillarsPlacements[];
extern ACTOR_PLACE MagmaRockInsideGolemCoreChestPlacements[];
extern ACTOR_PLACE MagmaRockInsideSalamanderTailChestPlacements[];
extern ACTOR_PLACE MagmaRockInsideEmberStonePlacements[];
extern ACTOR_PLACE MagmaRockInsideBlazeTabletPlacements[];

extern Sint16      MagmaRockInsideSalamanderTailChestPillars[];

// The placed actors of the room the party is in.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene != SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST)
    {
        if (scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
            return MagmaRockInsideEntrancePlacements;
        if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
            return MagmaRockInsideFuryPlacements;
        if (scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
            return MagmaRockInsideCrackedPillarsPlacements;
        if (scene == SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST)
            return MagmaRockInsideGolemCoreChestPlacements;
        if (scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST)
            return MagmaRockInsideSalamanderTailChestPlacements;
        if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
            return MagmaRockInsideEmberStonePlacements;
        if (scene == SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
            return MagmaRockInsideBlazeTabletPlacements;
    }
    return MagmaRockInsidePlacements;
}

// Nothing visible happens as Felix starts to push one of the yellow-topped stone pillars, whether
// by walking into it or with Move; the game frees the metatiles it walled off, so that it can
// slide.
void scMagmaRockInsideStartPushingPillar(void)
{
    FIELD_STATE *state;
    Sint16       scene;

    state = RamHeapSlots->fieldState;
    scene = MainRoutineStateWork.next;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
        fdActorLiftTargetBlock(MagmaRockInsideEntrancePillars);
    else if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
        fdActorLiftTargetBlock(MagmaRockInsideFuryPillars);
    else if (scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
        fdActorLiftTargetBlock(MagmaRockInsideCrackedPillarsPillars);
    else if (scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST)
        fdActorLiftTargetBlock(MagmaRockInsideSalamanderTailChestPillars);
    else if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
        fdActorLiftTargetBlock(MagmaRockInsideEmberStonePillars);
    scMagmaRockInsideClearBlockMetatiles(state->movedBlock);
}

// A yellow-topped stone pillar Felix has pushed comes to rest a metatile further on. Pushed off the
// ledge in the Salamander Tail's room or the ember stone's, it drops with a thud, into the lava
// when it is up, sinking until only its top shows. Once a pillar reaches its place its flag keeps
// it there when the party comes back: the entrance hall's pushed north into the row of columns (row
// 16 of the map), Fury's room's at row 41, the cracked pillars' room's three at column 21, the
// Salamander Tail room's at columns 47 and 57 and the ember stone room's at column 43.
void scMagmaRockInsideFinishPushingPillar(void)
{
    ACTOR *actor = FieldStateWork->movedBlock;
    Sint32 scene;
    Sint32 x;
    Sint32 z;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorPlaceMovedBlock();
    scene = MainRoutineStateWork.next;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST)
    {
        scBlocksMarkMetatiles(MagmaRockInsideSalamanderTailChestPillars);
        if (fdMapGetTerrainHeight(2, actor->x.fixed, actor->z.fixed) != actor->y.fixed)
        {
            actor->layer       = 2;
            actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
            syWait(1);
            if (fdScriptGetFlag(FLAG_LOWER_ROOMS_FLOODED))
            {
                ACTOR *copy;

                actor->gravity        = toFIXED(1) / 16;
                actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
                REG_BLDALPHA          = BLEND_ALPHA(0, 16);
                REG_BLDCNT            = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
                copy                  = fdActorCreateSilhouette(actor->x.fixed,
                                               actor->groundHeight,
                                               actor->z.fixed + toFIXED(16),
                                               SPRITE_YELLOW_BLOCK);
                scMagmaRockInsideWaitForLanding(actor);
                fdScriptPlaySound(SE_HEAVY_STOP);
                fdActorRelease(copy);
                fdActorSelectAnimation(actor, 4);
                syWait(1);
                REG_BLDCNT = 0;
            }
            else
            {
                scMagmaRockInsideWaitForLanding(actor);
                fdScriptPlaySound(SE_THUMP);
            }
            actor->motionFlags = 0;
            actor->layer       = 0;
        }
    }
    else if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
    {
        scBlocksMarkMetatiles(MagmaRockInsideEmberStonePillars);
        if (fdMapGetTerrainHeight(2, actor->x.fixed, actor->z.fixed) != actor->y.fixed)
        {
            ACTOR *copy;

            actor->layer       = 2;
            actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
            syWait(1);
            actor->gravity        = toFIXED(1) / 16;
            actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
            REG_BLDALPHA          = BLEND_ALPHA(0, 16);
            REG_BLDCNT            = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
            copy                  = fdActorCreateSilhouette(actor->x.fixed,
                                           actor->groundHeight,
                                           actor->z.fixed + toFIXED(16),
                                           SPRITE_YELLOW_BLOCK);
            scMagmaRockInsideWaitForLanding(actor);
            fdScriptPlaySound(SE_HEAVY_STOP);
            fdActorRelease(copy);
            fdActorSelectAnimation(actor, 4);
            syWait(1);
            REG_BLDCNT = 0;
            scMagmaRockInsideWaitForLanding(actor);
            actor->motionFlags = 0;
            actor->layer       = 0;
        }
    }
    x     = FIELD_METATILE(actor->x.fixed);
    z     = FIELD_METATILE(actor->z.fixed);
    scene = MainRoutineStateWork.next;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
    {
        if (z == 16)
            fdScriptSetFlag(FLAG_ENTRANCE_PILLAR_PUSHED);
    }
    else if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
    {
        if (z == 41)
            fdScriptSetFlag(FLAG_FURY_PILLAR_PUSHED);
    }
    else if (scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
    {
        if (x == 21)
        {
            if (z == 43)
                fdScriptSetFlag(FLAG_PILLAR_A_PUSHED);
            if (z == 45)
                fdScriptSetFlag(FLAG_PILLAR_B_PUSHED);
            if (z == 47)
                fdScriptSetFlag(FLAG_PILLAR_C_PUSHED);
        }
    }
    else if (scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST)
    {
        if (x == 47)
        {
            fdScriptSetFlag(FLAG_NORTH_PILLAR_PUSHED);
            fdMapGridCopyMetatileAttributes(46, 74, 1, 1, 46, 73);
        }
        if (x == 57)
            fdScriptSetFlag(FLAG_EAST_PILLAR_PUSHED);
    }
    else if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
    {
        if (x == 43)
            fdScriptSetFlag(FLAG_LEDGE_PILLAR_PUSHED);
    }
    fdScriptEnd();
}

// The overlay's import stub to fdBattleEffectStopMainActor.
Sint32 syImportBattleEffectStopMainActor(void);

// Plays Burst from Felix at an actor, as when he casts it, for a statue's fireball that strikes
// one: in the cracked pillars' room, the pillar it hits breaks as if Burst had been cast at it.
void   scMagmaRockInsidePlayBurst(Uint16 value)
{
    MAIN_ROUTINE_STATE *party;

    fdPsynergyLoadResources(ABILITY_BURST, 1);
    party = &MainRoutineStateWork;
    syImportFieldPsynergySetupActorPair(party->leaderActorId, value);
    fdPsynergyRunHandler(1);
    syImportBattleEffectStopMainActor();
    fdPsynergyEnd();
}

extern Uint32 MagmaRockEmberScript[];

void          scFieldEffectSpawn(Sint32                x,
                                 Sint32                y,
                                 Sint32                z,
                                 Sint32                velocityX,
                                 Sint32                velocityY,
                                 Sint32                velocityZ,
                                 Uint32                flags,
                                 const EFFECT_OPTIONS *options);

// A statue's fireball's update as it flies: it trails an ember every fourth tick, a few pixels to
// either side, and whooshes every sixteenth.
void          scMagmaRockInsideTrailEmbers(ACTOR *actor)
{
    EFFECT_OPTIONS options;
    Uint32         frame = TickCount;
    Sint32         randomZ;
    Sint32         vz;

    if ((frame & 3) != 0)
        return;
    options.type     = SPRITE_FLAME_PUFF;
    options.priority = 2;
    options.script   = MagmaRockEmberScript;
    if ((frame & 15) == 0)
        fdActorPlaySoundInView(actor, SE_WHOOSH);
    randomZ = syRandom() & 15;
    vz      = ((syRandom() & 31) - 16) << 12;
    scFieldEffectSpawn(actor->x.fixed,
                       actor->y.fixed,
                       actor->z.fixed + ((randomZ - 8) << 15),
                       0,
                       0,
                       vz,
                       EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | 1,
                       &options);
}

#include "shared/statue_flash_under_burst.h"

#include "shared/statue_find_under_burst.h"

// Nothing visible happens as Felix starts casting Burst at a fire-spitting statue; the game
// finishes what the field engine still had pending, so that the cast and the statue's fire play
// undisturbed.
void scMagmaRockInsideResolveBeforeBurst(void)
{
    fdScriptEndLastingPsynergy(0);
}

extern Sint16 MagmaRockInsideStatues[];
extern Sint16 MagmaRockInsideCrackedPillarsStatues[];

void          syStatueFlashUnderBurst(ACTOR *actor);

// As Felix casts Burst at the small statue beside a great stone face, the statue blinks red, slowly
// through the small explosions and faster at the last blast. In the entrance hall the statue's
// metatile is (26, 12); in Fury's room (20, 15); the cracked pillars' room has three, at (12, 12),
// (20, 33) and (30, 33), and its own table.
void          scMagmaRockInsideTargetStatue(Sint32 stage)
{
    Uint16             *table;
    FIELD_EFFECT_STATE *watch = RamHeapSlots->fieldEffectState;
    Sint32              scene;

    table = MagmaRockInsideStatues;
    syFieldScriptEndLastingPsynergy(0);
    scene = MainRoutineStateWork.next;
    // DEAD: the entrance hall's and Fury's room's arms are empty; the ROM keeps both tests.
    if (scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
    {
    }
    else if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
    {
    }
    else if (scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
    {
        table = MagmaRockInsideCrackedPillarsStatues;
    }
    if (stage == BURST_STAGE_SMALL_BLAST)
    {
        ACTOR *actor;

        scStatueFindUnderBurst(table);
        actor            = watch->target;
        actor->flashMask = 4;
        actor->update    = (OBJECT_UPDATE)syStatueFlashUnderBurst;
    }
    if (stage == BURST_STAGE_GREAT_BLAST)
    {
        ACTOR *actor;

        scStatueFindUnderBurst(table);
        actor            = watch->target;
        actor->flashMask = 2;
        actor->update    = (OBJECT_UPDATE)syStatueFlashUnderBurst;
    }
}

// An ember's update as a statue's fireball bursts: moves it by its velocity, kept in bounce,
// gravity and sceneWord, and slows it by an eighth.
void scMagmaRockInsideDriftAndSlow(ACTOR *actor)
{
    Sint32 dx, dy, dz;

    actor->x.fixed += dx  = actor->bounce;
    actor->y.fixed += dy  = actor->gravity;
    actor->z.fixed += dz  = actor->sceneWord;
    dx                   -= dx >> 3;
    dy                   -= dy >> 3;
    dz                   -= dz >> 3;
    actor->bounce         = dx;
    actor->gravity        = dy;
    actor->sceneWord      = dz;
}

// The ticks a statue's fireball bursts where it lands, throwing out an ember every other tick.
enum
{
    FIRE_BURST_TICKS = 16
};

// A statue's fireball flies the way the statue faces, the camera following, until it meets the edge
// of the fire's reach, a great stone face or something standing in its path. It bursts against the
// edge or a stone pillar; against a face it sets the face pouring lava
// (scMagmaRockInsideRaiseLava); against anything else it plays Burst there.
void scMagmaRockInsideFireFromStatue(ACTOR *source)
{
    ACTOR *flame = Create(SPRITE_FLAME_PUFF, source->x.fixed, source->y.fixed, source->z.fixed);
    ACTOR *other;
    FIXED  position[3];
    EFFECT_OPTIONS options;
    Sint32         step = syCos(source->facing) << 1;
    Sint32         metatile;
    Sint32         id;
    Sint32         bit;

    flame->motionFlags = 0;
    fdActorSelectAnimation(flame, 1);
    fdActorSetSpriteFlags(flame, 0);
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    position[0] = flame->x.fixed;
    position[1] = flame->y.fixed;
    position[2] = flame->z.fixed;
    for (;;)
    {
        position[0] += step;
        metatile     = fdMapMetatileGetAttributes(MAPGRID_LAYER_OVERHEAD,
                                              FIELD_METATILE(position[0]),
                                              FIELD_METATILE(position[2] - position[1]))
                >> 8;
        if (metatile == METATILE_BEHAVIOR_MAGMA_ROCK_FIRE_EDGE
            || metatile == METATILE_BEHAVIOR_MAGMA_ROCK_LAVA_FALL)
            goto end;
        for (id = ACTOR_FIRST_PLACED; id != FIELD_TREASURE_ACTOR; id++)
        {
            other = FieldStateWork->actors[id];
            if (other->script == NULL)
                continue;
            if (FIELD_METATILE(source->x.fixed) == FIELD_METATILE(other->x.fixed)
                && FIELD_METATILE(source->y.fixed) == FIELD_METATILE(other->y.fixed)
                && FIELD_METATILE(source->z.fixed) == FIELD_METATILE(other->z.fixed))
                continue;
            if (scPosFlatDistance(&other->x.fixed, position) > 8)
                continue;
            if (FIELD_METATILE(other->y.fixed) != FIELD_METATILE(position[1]))
                continue;
            if (((SPR *)other->sprite)->entries[0]->animationId == SPRITE_STONE_PILLAR)
                goto end;
            goto hit;
        }
    }
end:
    flame->acceleration = toFIXED(2);
    flame->speedLimit   = toFIXED(2);
    flame->update       = (OBJECT_UPDATE)scMagmaRockInsideTrailEmbers;
    scFieldActorSetMoveTargetFX(flame, position[0], position[1], position[2]);
    fdCameraSetSpeed(2);
    fdCameraMoveToFX(position[0], position[1], position[2], 1);
    scFieldActorWaitForMove(flame);
    flame->update = NULL;
    fdActorRelease(flame);
    for (id = 0; id != FIRE_BURST_TICKS; id++)
    {
        bit  = TickCount;
        bit &= 1;
        if (bit == 0)
        {
            options.type     = SPRITE_FLAME_PUFF;
            options.priority = 1;
            options.update   = (void (*)(FIELD_OBJECT *))scMagmaRockInsideDriftAndSlow;
            options.script   = MagmaRockEmberScript;
            scFieldEffectSpawn(position[0],
                               position[1],
                               position[2],
                               ((syRandom() & 31) - 16) << 13,
                               ((syRandom() & 31) - 16) << 13,
                               bit,
                               EFFECT_USE_UPDATE | EFFECT_USE_SCRIPT | EFFECT_USE_TYPE
                                   | EFFECT_USE_PRIORITY,
                               &options);
        }
        syWait(1);
    }
    syWait(24);
    if (metatile == METATILE_BEHAVIOR_MAGMA_ROCK_LAVA_FALL)
    {
        scMagmaRockInsideRaiseLava();
        fdScriptEnd();
        return;
    }
    syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
    fdCameraWaitForArrival();
    syWait(10);
    fdScriptEnd();
    return;
hit:
    flame->acceleration = toFIXED(2);
    flame->speedLimit   = toFIXED(2);
    flame->update       = (OBJECT_UPDATE)scMagmaRockInsideTrailEmbers;
    scFieldActorSetMoveTargetFX(flame, position[0], position[1], position[2]);
    fdCameraSetSpeed(2);
    fdCameraMoveToFX(position[0], position[1], position[2], 1);
    scFieldActorWaitForMove(flame);
    flame->update = NULL;
    fdActorRelease(flame);
    fdScriptSetFlag(FLAG_FIRE_BURST_PLAYING);
    scMagmaRockInsidePlayBurst(id);
    fdScriptClearFlag(FLAG_FIRE_BURST_PLAYING);
    syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
    fdCameraWaitForArrival();
    syWait(10);
    fdScriptEnd();
}

void scRockEndFalling(void);

// Once Felix's Burst on the small statue beside a great stone face is over, the statue flashes
// white and spits a fireball the way it faces. In the three halls with a face the fireball strikes
// it, and the face's eyes light red as it pours lava over the floor
// (scMagmaRockInsideFireFromStatue).
void scMagmaRockInsideFireStatue(void)
{
    FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffectState;
    ACTOR              *actor;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    syFieldScriptWait(10);
    actor         = state->target;
    actor->update = NULL;
    scFieldActorSetSpriteColor(actor, SPRITE_COLOR_WHITE);
    syFieldScriptWait(3);
    scFieldActorSetSpriteColor(state->target, SPRITE_COLOR_NORMAL);
    syFieldScriptWait(1);
    scMagmaRockInsideFireFromStatue(state->target);
    fdScriptEnd();
}

// A cracked pillar's animation as Burst breaks it: its two cracked pictures, then its stump, 41
// ticks each. It starts at tick CRACKED_PILLAR_CRUMBLE_TICK of the cast, among the small
// explosions, and the pillar has crumbled to its stump by the last blast, which takes it away.
enum
{
    CRACKED_PILLAR_ANIMATION_CRUMBLE = 3,
    CRACKED_PILLAR_CRUMBLE_TICK      = 178
};

// Burst on one of the tall cracked pillars: rocks fall round it while Burst lasts, one at each
// small explosion and eight at the last blast, which also takes the pillar away, leaving a cloud of
// dust; meanwhile the pillar crumbles.
void scMagmaRockInsideBurstCrackedPillar(Sint32 stage, Sint32 sound)
{
    FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffectState;

    if (sound == 0)
        scMagmaRockInsideStartFallingRocks();
    if (stage == BURST_STAGE_SMALL_BLAST)
    {
        ACTOR *actor = state->target;

        scMagmaRockInsideDropRocks(actor->x.fixed, actor->y.fixed + toFIXED(15), actor->z.fixed, 1);
    }
    if (stage == BURST_STAGE_GREAT_BLAST)
    {
        ACTOR *actor = state->target;
        ACTOR *target;

        scMagmaRockInsideDropRocks(actor->x.fixed, actor->y.fixed + toFIXED(15), actor->z.fixed, 8);
        target = state->target;
        SET_ACTOR_POSITION(target, 0, 0, 0);
    }
    if (sound == PSYNERGY_SCRIPT_LAST_TICK)
        scRockEndFalling();
    if (sound == CRACKED_PILLAR_CRUMBLE_TICK)
        fdActorSelectAnimation(state->target, CRACKED_PILLAR_ANIMATION_CRUMBLE);
}

// Felix walks to the lava's edge where something stands in the lava two metatiles ahead, a white
// platform, a column or a pillar pushed in, and springs across onto it, keeping pace with a
// platform as it moves. Such edges are on both sides of the Lucky Medal room's lava, at metatiles
// (44, 39) and (50, 39), and along the lava in Fury's room, the cracked pillars' room and, once its
// lava is up, the Salamander Tail's room. Where nothing stands there, nothing happens.
void scMagmaRockInsideLeapOverLava(void)
{
    ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
    ACTOR *target;
    FIXED  pos[3];
    FIXED  stepX;
    FIXED  stepZ;
    Sint32 i;
    Sint32 angle = (leader->facing + DEGtoANG(45)) & FACING_QUARTER_MASK;

    pos[0]       = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    pos[1]       = leader->y.fixed - toFIXED(16);
    pos[2]       = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    syFieldVectorAddPolarOffset(32, angle, pos);
    target = fdActorFindInMetatile(pos, leader);
    if (target == NULL)
        return;
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
    syWait(6);
    fdScriptPlaySound(SE_JUMP);
    fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
    leader->speedLimit    = toFIXED(3);
    leader->acceleration  = toFIXED(2);
    leader->velocityY     = toFIXED(24) / 5;
    leader->motionFlags  &= ~(ACTOR_FOLLOWS_TERRAIN | ACTOR_MOTION_SPARE);
    fdActorSetSpriteFlags(leader, 0);
    stepX = (pos[0] - leader->x.fixed) / 12;
    stepZ = (pos[2] - leader->z.fixed) / 12;
    for (i = 0; i != 12; i++)
    {
        leader->x.fixed += target->x.fixed - pos[0] + stepX;
        leader->z.fixed += target->z.fixed - pos[2] + stepZ;
        pos[0]           = target->x.fixed;
        pos[2]           = target->z.fixed;
        syWait(1);
    }
    leader->x.fixed = target->x.fixed;
    leader->y.fixed = leader->groundHeight = target->y.fixed + toFIXED(16);
    leader->z.fixed                        = target->z.fixed;
    fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
    fdActorSetSpriteFlags(leader, 1);
    leader->motionFlags |= ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    fdScriptEnd();
}

extern Sint32 MagmaRockFlameEmberScript[];

// The bursts of flame, 8 ticks apart, as a statue's fireball strikes a great stone face.
enum
{
    LAVA_FALL_FLAME_BURSTS = 16
};

// A statue's fireball strikes the great stone face: the screen shakes as flames burst over the
// face, it roars and its eyes light red, lava pours from its mouth and spreads over the hall's
// floor until the hall is flooded. When the lava is already up, the face only flares and pours
// again.
void scMagmaRockInsideRaiseLava(void)
{
    Sint32         srcX  = 0;
    Sint32         srcZ  = 0;
    FIXED          baseX = 0;
    MAP_LAYER     *layer = &((MAP_STATE *)RamHeapSlots->mapGrid)->layers[MAPGRID_LAYER_MIDDLE];
    EFFECT_OPTIONS options;
    Sint32         pos[3];
    // LAVA_ALREADY_UP when the room is already flooded; then the ticks the lava takes to rise.
    Uint32         frame = 0;
    Uint32         i;
    Uint32         row;

    options.type        = SPRITE_FLAME_PUFF;
    options.priority    = 1;
    options.script      = MagmaRockFlameEmberScript;
    options.startScaleX = toFIXED(9) / 5;
    options.startScaleY = toFIXED(9) / 5;
    if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
    {
        baseX  = toFIXED(328);
        pos[1] = toFIXED(32);
        pos[2] = toFIXED(200);
        srcX   = 20;
        srcZ   = 69;
        if (fdScriptGetFlag(FLAG_ENTRANCE_FLOODED))
            frame = LAVA_ALREADY_UP;
    }
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY)
    {
        baseX  = toFIXED(232);
        pos[1] = toFIXED(32);
        pos[2] = toFIXED(248);
        srcX   = 14;
        srcZ   = 72;
        if (fdScriptGetFlag(FLAG_FURY_FLOODED))
            frame = LAVA_ALREADY_UP;
    }
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
    {
        baseX  = toFIXED(392);
        pos[1] = toFIXED(32);
        pos[2] = toFIXED(536);
        srcX   = 24;
        srcZ   = 90;
        if (fdScriptGetFlag(FLAG_LOWER_ROOMS_FLOODED))
            frame = LAVA_ALREADY_UP;
    }
    for (i = 0; i != LAVA_FALL_FLAME_BURSTS; i++)
    {
        pos[0] = baseX + ((syRandom() & 31) << 16);
        fdScriptPlaySound(SE_CRASH);
        scFieldEffectSpawn(pos[0],
                           pos[1],
                           pos[2],
                           0,
                           (syRandom() & 31) << 12,
                           0,
                           EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_START_SCALE
                               | EFFECT_USE_PRIORITY | 1,
                           &options);
        fdMapGridSetShake(5, 5, 1);
        fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        fdScriptWait(8);
    }
    fdScriptPlaySound(SE_BOSS_ROAR);
    options.priority      = 1;
    options.startScaleX   = toFIXED(2);
    options.startScaleY   = toFIXED(2);
    options.targetScaleX  = toFIXED(1) / 2;
    options.targetScaleY  = toFIXED(1) / 2;
    baseX                -= toFIXED(8);
    for (i = 0; i != 8; i++)
    {
        pos[0] = baseX + i * toFIXED(6);
        scFieldEffectSpawn(pos[0],
                           pos[1],
                           pos[2],
                           0,
                           0,
                           (syRandom() & 31) << 10,
                           EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET | EFFECT_USE_PRIORITY
                               | 1,
                           &options);
    }
    fdMapGridCopyMetatilesTo(68, 70, srcX, srcZ, 3, 6);
    fdMapGridSetShake(5, 7, 1);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    fdScriptWait(30);
    fdMapGridCopyMetatilesTo(71, 70, srcX, srcZ, 3, 6);
    options.type     = SPRITE_FLAME_PUFF;
    options.priority = 1;
    options.palette  = SPRITE_COLOR_RED_TINT;
    options.script   = MagmaRockEmberScript;
    if (frame != 0)
    {
        fdMapGridCopyMetatilesTo(68, 70, srcX, srcZ, 3, 6);
        fdScriptWait(30);
        pos[2] -= toFIXED(40);
    }
    else
    {
        fdMapGridSetShake(2, 1, 1);
        fdScriptPlaySound(SE_TREMOR);
        for (row = 0; row != 12; row++)
        {
            for (i = 0; i != 6; i++)
            {
                pos[0] = baseX + ((syRandom() % 48) << 16);
                scFieldEffectSpawn(pos[0],
                                   pos[1],
                                   pos[2] + row * toFIXED(4) + toFIXED(8),
                                   0,
                                   toFIXED(1),
                                   0,
                                   EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY
                                       | EFFECT_USE_PALETTE,
                                   &options);
            }
            sySynch(3);
            fdMapGridCopyMetatilesTo(74, 70, srcX, srcZ, 3, (row >> 2) + 6);
        }
        fdMapGridCopyMetatilesTo(74, 70, srcX, srcZ, 3, 8);
        frame = 0;
        do
        {
            if ((frame & 3) == 0)
            {
                for (i = 0; i != 4; i++)
                {
                    pos[0] = baseX + ((syRandom() % 48) << 16);
                    scFieldEffectSpawn(pos[0],
                                       pos[1],
                                       pos[2] + toFIXED(48),
                                       0,
                                       toFIXED(1),
                                       0,
                                       EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY
                                           | EFFECT_USE_PALETTE,
                                       &options);
                }
            }
            if (frame == 0)
            {
                layer->offsetY -= toFIXED(32);
                fdMapGridCenterOnFollowedPoint();
                fdScriptWait(1);
            }
            else if (frame <= 63)
            {
                REG_BLDALPHA = ((16 - (frame >> 2)) << 8) | (frame >> 2);
                REG_BLDCNT   = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
            }
            else if (frame == 64)
            {
                REG_BLDCNT = 0;
            }
            else if (frame <= 127)
            {
                layer->offsetY += toFIXED(1) / 2;
                pos[2]         -= toFIXED(1) / 2;
                fdMapGridCopyMetatilesTo(71, 76, srcX, srcZ - ((frame - 47) >> 5) + 8, 3, 1);
            }
            else if (frame == 128)
            {
                fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
            }
            frame++;
            sySynch(1);
        } while (frame != 160);
        syFieldMapGridWaitForShakeToSettle();
    }
    fdScriptPlaySound(SE_STOP_ACTION_SE);
    fdScriptPlaySound(SE_WHOOSH);
    options.startScaleY = toFIXED(9) / 5;
    options.startScaleX = toFIXED(9) / 5;
    for (i = 0; i != 6; i++)
    {
        pos[0] = baseX + ((syRandom() % 48) << 16);
        scFieldEffectSpawn(pos[0],
                           pos[1],
                           pos[2] + toFIXED(48),
                           0,
                           0,
                           0,
                           EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_START_SCALE
                               | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                           &options);
    }
    fdMapGridSetShake(5, 7, 1);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    fdMapGridCopyMetatilesTo(68, 64, srcX, srcZ, 3, 6);
    syFieldMapGridWaitForShakeToSettle();
    fdScriptWait(30);
    layer->offsetY = 0;
    if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
        scMagmaRockInsideSetEntranceFlooded();
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY)
        scMagmaRockInsideSetFuryFlooded();
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
        scMagmaRockInsideSetLowerRoomsFlooded();
}

// Felix steps onto the small floor switch beside a grated gate and presses it down: the ground
// shakes and the gate lifts. If the lava is up it pours out through the gate, embers rising as it
// fades from the floor; then the gate drops shut and he steps back off the switch. The switches are
// at metatile (19, 27) in the entrance hall, (32, 21) in Fury's room, and (47, 13) and (49, 42) in
// the Golem Core's room, which drain the lower rooms.
void scMagmaRockInsideDrainLava(void)
{
    ACTOR         *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
    Uint32         facing = (leader->facing + DEGtoANG(45)) & FACING_QUARTER_MASK;
    Sint32         pos[3];
    EFFECT_OPTIONS options;
    FIXED          baseX = 0;
    Sint32         destX = 0;
    Sint32         destZ = 0;
    Sint32         open  = 0;
    Sint32         srcX  = 0;
    Uint8         *flags;
    // The gate's row on the map, then each row of embers over the drained fall.
    Uint32         row = 0;
    Uint32         j;
    Sint32         rowAbove;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    pos[0] = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    pos[2] = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    fdActorWalkToAndWait(
        MainRoutineStateWork.leaderActorId, FIXEDtoINT(pos[0]), FIXEDtoINT(pos[2]));
    syFieldScriptWait(20);
    fdScriptPlaySound(SE_BUTTON_PRESS);
    flags   = &leader->priorityFlags;
    *flags &= ~ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    syFieldScriptWait(20);
    fdMapGridSetShake(1, 2, 1);
    fdScriptPlaySound(SE_TREMOR);
    syFieldScriptWait(30);
    if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
    {
        srcX   = 20;
        row    = 89;
        destX  = 84;
        destZ  = 26;
        baseX  = toFIXED(320);
        pos[1] = 0;
        pos[2] = toFIXED(424);
        if (fdScriptGetFlag(FLAG_ENTRANCE_FLOODED))
            open = LAVA_ALREADY_UP;
        fdScriptClearFlag(FLAG_ENTRANCE_FLOODED);
        facing = FACING_EAST;
        scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 15, 13, MAP_METATILE_CLIMBABLE);
        scFieldActorSetSpritePriority(ACTOR_ENTRANCE_PILLAR, 3);
        for (j = 0; j != ACTOR_ENTRANCE_PILLAR - ACTOR_COLUMN_A + 1; j++)
            fdActorSetAnimation(j + ACTOR_COLUMN_A, 1);
    }
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY)
    {
        srcX   = 28;
        row    = 83;
        destX  = 92;
        destZ  = 21;
        baseX  = toFIXED(448);
        pos[1] = 0;
        pos[2] = toFIXED(344);
        if (fdScriptGetFlag(FLAG_FURY_FLOODED))
            open = LAVA_ALREADY_UP;
        fdScriptClearFlag(FLAG_FURY_FLOODED);
        scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 24, 15, MAP_METATILE_CLIMBABLE);
        scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 9, 16, MAP_METATILE_CLIMBABLE);
        fdActorSetAnimation(ACTOR_FURY_PILLAR, 4);
    }
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST)
    {
        if (MainRoutineStateWork.entrance <= GOLEM_CORE_NORTH_LAST_ENTRANCE)
        {
            srcX   = 49;
            row    = 75;
            destX  = 113;
            destZ  = 13;
            baseX  = toFIXED(792);
            pos[1] = 0;
            pos[2] = toFIXED(216);
            if (fdScriptGetFlag(FLAG_LOWER_ROOMS_FLOODED))
                open = LAVA_ALREADY_UP;
            fdScriptClearFlag(FLAG_LOWER_ROOMS_FLOODED);
        }
        else
        {
            srcX   = 51;
            row    = 104;
            destX  = 115;
            destZ  = 42;
            baseX  = toFIXED(824);
            pos[1] = 0;
            pos[2] = toFIXED(680);
            if (fdScriptGetFlag(FLAG_LOWER_ROOMS_FLOODED))
                open = LAVA_ALREADY_UP;
            fdScriptClearFlag(FLAG_LOWER_ROOMS_FLOODED);
        }
    }
    fdScriptPlaySound(SE_BOOM);
    fdMapGridSetShake(5, 7, 1);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    rowAbove = row - 1;
    fdMapGridCopyMetatilesTo(srcX, row, srcX, rowAbove, 3, 4);
    if (open == 0)
    {
        syFieldMapGridWaitForShakeToSettle();
        syFieldScriptWait(20);
    }
    else
    {
        fdMapGridSetShake(2, 1, 1);
        options.type     = SPRITE_FLAME_PUFF;
        options.priority = 2;
        options.palette  = SPRITE_COLOR_RED_TINT;
        options.script   = MagmaRockEmberScript;
        for (row = 0; row != 10; row++)
        {
            for (j = 0; j != 6; j++)
            {
                Uint32 random = syRandom();

                pos[0]        = baseX + ((random % 48) << 16);
                scFieldEffectSpawn(pos[0],
                                   pos[1],
                                   pos[2] + row * toFIXED(4) + toFIXED(23),
                                   0,
                                   toFIXED(1),
                                   0,
                                   EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY
                                       | EFFECT_USE_PALETTE,
                                   &options);
            }
            syWait(3);
            fdMapGridCopyMetatilesTo(75, 64, destX, destZ, 3, (row >> 1) + 1);
        }
        for (j = 0; j != BLEND_WEIGHT_MAX * 4; j++)
        {
            REG_BLDALPHA = ((j >> 2) << 8) | (BLEND_WEIGHT_MAX - (j >> 2));
            REG_BLDCNT   = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
            syWait(3);
        }
        fdMapGridCopyMetatilesTo(78, 64, destX, destZ, 3, 5);
    }
    if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
        fdActorGet(ACTOR_ENTRANCE_PILLAR)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    fdScriptPlaySound(SE_STOP_ACTION_SE);
    fdScriptPlaySound(SE_BOOM);
    fdMapGridSetShake(5, 7, 1);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    fdMapGridCopyMetatilesTo(72, 63, srcX, rowAbove, 3, 4);
    syFieldMapGridWaitForShakeToSettle();
    fdScriptPlaySound(SE_BUTTON_PRESS);
    syActorFlagsSet(flags, ACTOR_PRIORITY_WALKS_ON_PLATFORMS);
    syFieldScriptWait(20);
    fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlags &= ~ACTOR_FACES_MOTION;
    pos[0] = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    pos[1] = leader->y.fixed;
    pos[2] = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    syFieldVectorAddPolarOffset(16, (Sint32)facing ^ DEGtoANG(180), pos);
    leader->facing = facing;
    fdActorWalkToAndWait(
        MainRoutineStateWork.leaderActorId, FIXEDtoINT(pos[0]), FIXEDtoINT(pos[2]));
    fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlags |= ACTOR_FACES_MOTION;
    fdScriptEnd();
}

// Defines what Burst does to a cracked pillar: it crumbles (scMagmaRockInsideBurstCrackedPillar)
// and its flag is set.
#define DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(name, pillar, flag)                                  \
    void name(Sint32 stage, Sint32 sound)                                                          \
    {                                                                                              \
        FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffectState;                                \
                                                                                                   \
        state->target             = fdActorGet(pillar);                                            \
        scMagmaRockInsideBurstCrackedPillar(stage, sound);                                         \
        fdScriptSetFlag(flag);                                                                     \
    }

// Burst cast at the tall cracked pillar east of the cracked pillars' room's north statue (metatile
// (17, 12)) breaks it: rocks fall round it as it crumbles and the last blast leaves only dust. The
// statue's own fireball breaks it the same way. `FLAG_INSIDE_CRACKED_PILLAR_A_BURST` keeps it gone.
DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(MagmaRockInsideBurstCrackedPillarA,
                                      ACTOR_CRACKED_PILLAR_A,
                                      FLAG_INSIDE_CRACKED_PILLAR_A_BURST)

// Burst cast at the cracked pillars' room's other tall cracked pillar (metatiles (19, 14) and (24,
// 16)) breaks it: rocks fall round it as it crumbles and the last blast leaves only dust.
// `FLAG_INSIDE_CRACKED_PILLAR_B_BURST` keeps it gone.
DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(MagmaRockInsideBurstCrackedPillarB,
                                      ACTOR_CRACKED_PILLAR_B,
                                      FLAG_INSIDE_CRACKED_PILLAR_B_BURST)

// The update of a rock or flame the ember stone throws: it trails smoking embers every other tick
// for 64 ticks.
void scMagmaRockInsideSmokeEmber(ACTOR *actor)
{
    if (++actor->timer > 64)
        actor->update = NULL;
    if ((actor->timer & 1) == 0)
    {
        EFFECT_OPTIONS config;

        config.type     = SPRITE_FLAME_PUFF;
        config.priority = 2;
        config.palette  = SPRITE_COLOR_RED_TINT;
        config.script   = MagmaRockEmberScript;
        scFieldEffectSpawn(actor->x.fixed,
                           actor->y.fixed,
                           actor->z.fixed,
                           0,
                           toFIXED(1) / 16,
                           0,
                           EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY
                               | EFFECT_USE_PALETTE,
                           &config);
    }
}

extern Sint32          MagmaRockFlameEmberScript[];

// Throws a rock or flame up out of the lava where the ember stone lies, to land at (x, z) in
// pixels, smoking as it flies (scMagmaRockInsideSmokeEmber).
static __inline__ void Arrive(Sint32 id, Sint32 x, Sint32 z)
{
    fdActorSetPos(id, 776, 248);
    fdActorSetDestination(id, x, z);
    fdActorGetById(id)->velocityY = toFIXED(12);
    fdActorGetById(id)->gravity   = toFIXED(1) / 2;
    fdActorGetById(id)->timer     = 0;
    fdActorGetById(id)->update    = scMagmaRockInsideSmokeEmber;
}

// The first time Felix hops onto the islet in the lava at metatile (45, 19), in the ember stone's
// room east of the torches' ledge, the room shakes and the camera turns to the lava: embers spout
// and pound, four rocks and four flames are thrown out to land on the islets round the spot, and
// last the ember stone itself, a glowing red rock, lands on an islet to the west.
// `FLAG_ROCKS_RELEASED` keeps them there.
void scMagmaRockInsideReleaseRocks(void)
{
    FIXED          position[3];
    EFFECT_OPTIONS options;
    Sint32         beat;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdScriptPlaySound(SE_TREMOR);
    fdMapGridSetShake(3, 3, 1);
    fdScriptWait(30);
    fdCameraMoveTo(776, CAMERA_KEEP, 272, 1);
    fdCameraWaitForArrival();
    options.type        = SPRITE_FLAME_PUFF;
    options.priority    = 2;
    options.palette     = SPRITE_COLOR_RED_TINT;
    options.script      = MagmaRockFlameEmberScript;
    options.startScaleY = toFIXED(3) / 2;
    options.startScaleX = toFIXED(3) / 2;
    position[1]         = 0;
    // A beat every 3 ticks: embers burst from the stone, a pound every eighth beat; the rocks and
    // flames are thrown eight beats apart from beat 32, and the stone itself at beat 112.
    for (beat = 0; beat != 128; beat++)
    {
        position[0] = toFIXED(syRandom() & 31) + (toFIXED(760));
        position[2] = toFIXED(syRandom() & 31) + (toFIXED(256));
        scFieldEffectSpawn(position[0],
                           position[1],
                           position[2],
                           0,
                           toFIXED(1),
                           0,
                           EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_START_SCALE
                               | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                           &options);
        if ((beat & 7) == 0)
            fdScriptPlaySound(SE_POUND);
        if (beat == 32)
        {
            Arrive(ACTOR_FALLEN_ROCK_A, 744, 216);
        }
        else if (beat == 40)
        {
            Arrive(ACTOR_FALLEN_FLAME_D, 776, 328);
        }
        else if (beat == 48)
        {
            Arrive(ACTOR_FALLEN_ROCK_B, 856, 232);
        }
        else if (beat == 56)
        {
            Arrive(ACTOR_FALLEN_FLAME_C, 696, 312);
        }
        else if (beat == 64)
        {
            Arrive(ACTOR_FALLEN_ROCK_C, 728, 280);
        }
        else if (beat == 72)
        {
            Arrive(ACTOR_FALLEN_FLAME_B, 824, 280);
        }
        else if (beat == 80)
        {
            Arrive(ACTOR_FALLEN_ROCK_D, 824, 312);
        }
        else if (beat == 88)
        {
            Arrive(ACTOR_FALLEN_FLAME_A, 776, 232);
        }
        else if (beat == 112)
        {
            fdActorSetPos(ACTOR_EMBER_STONE, 776, 248);
            fdActorSetDestination(ACTOR_EMBER_STONE, 696, 232);
            fdCameraMoveTo(696, CAMERA_KEEP, 232, 1);
            fdActorGetById(ACTOR_EMBER_STONE)->velocityY = toFIXED(13);
            fdActorGetById(ACTOR_EMBER_STONE)->gravity   = toFIXED(1) / 2;
            fdActorGetById(ACTOR_EMBER_STONE)->timer     = 0;
            fdActorGetById(ACTOR_EMBER_STONE)->update    = scMagmaRockInsideSmokeEmber;
            fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        }
        sySynch(3);
    }
    scFieldActorWaitForMoveById(ACTOR_EMBER_STONE);
    syFieldMapGridWaitForShakeToSettle();
    fdScriptPlaySound(SE_STOP_ACTION_SE);
    fdScriptWait(40);
    fdScriptSetFlag(FLAG_ROCKS_RELEASED);
    syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
    fdCameraWaitForArrival();
    sySynch(10);
    fdScriptEnd();
}

// Felix casts Blaze at one of the flames the ember stone threw into the lava: Blaze sprays far, and
// unless the east torch on the ledge west of the lava is already lit, a small hidden flame waits on
// it, which grows if the spray reaches it (scMagmaRockInsideGrowFlame).
void scMagmaRockInsideTakeFlame(void)
{
    FIELD_STATE *state    = RamHeapSlots->fieldState;

    state->blazeLongSpray = 1;
    if (!fdScriptGetFlag(FLAG_EAST_TORCH_LIT))
    {
        scFieldActorSetSpriteColor(fdActorGet(ACTOR_EAST_TORCH_FLAME), SPRITE_COLOR_HIDDEN);
        fdActorSetPos(ACTOR_EAST_TORCH_FLAME, 632, 312);
        fdActorGet(ACTOR_EAST_TORCH_FLAME)->scaleX = fdActorGet(ACTOR_EAST_TORCH_FLAME)->scaleY =
            toFIXED(1) / 16;
    }
}

// Defines what Blaze at a torch does: unless litFlag is set, flame waits hidden on it at (x, z), a
// sixteenth of its size.
#define DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(name, litFlag, flame, x, z)                           \
    void name(void)                                                                                \
    {                                                                                              \
        if (!fdScriptGetFlag(litFlag))                                                             \
        {                                                                                          \
            scFieldActorSetSpriteColor(fdActorGet(flame), SPRITE_COLOR_HIDDEN);                    \
            fdActorSetPos(flame, x, z);                                                            \
            fdActorGet(flame)->scaleX = fdActorGet(flame)->scaleY = toFIXED(1) / 16;               \
        }                                                                                          \
    }

// Felix casts Blaze at the east torch's flame on the ledge west of the ember stone's lava: unless
// the west torch beside it is lit, a small hidden flame waits on it, which grows if the spray
// reaches it (scMagmaRockInsideGrowFlame).
DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(
    MagmaRockInsideLightTorch, FLAG_WEST_TORCH_LIT, ACTOR_WEST_TORCH_FLAME, 600, 312)

struct ACTOR;

// Blaze's spray reaches a torch on the ledge west of the ember stone's lava, and its flame grows
// until it burns full. Once both torches burn, the ground rumbles, the camera turns to the stone
// pillar between them, and it sinks into the floor, opening the doorway behind it (metatile (38,
// 18)).
void scMagmaRockInsideGrowFlame(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;
    ACTOR       *object;
    ACTOR       *actor;
    ACTOR       *other;
    Sint32       i;

    object = fdActorGetById(state->psynergyHitObject);
    scFieldActorSetSpriteColor(object, SPRITE_COLOR_NORMAL);
    fdScriptPlaySound(SE_WHOOSH);
    i = 0;
    do
    {
        object->scaleY = object->scaleX += toFIXED(1) / FLAME_GROW_STEPS;
        sySynch(5);
        i++;
    } while (i != FLAME_GROW_STEPS);
    fdScriptSetFlag(FLAG_TORCH_LIT(state->psynergyHitObject));
    if (fdScriptGetFlag(FLAG_WEST_TORCH_LIT) && fdScriptGetFlag(FLAG_EAST_TORCH_LIT))
    {
        other = fdActorGetById(ACTOR_BLOCKING_PILLAR);
        fdScriptStart();
        syFieldScriptEndLastingPsynergy(0);
        fdMapGridSetShake(2, 1, 1);
        fdScriptPlaySound(SE_RUMBLE);
        syFieldScriptWait(30);
        fdCameraMoveTo(616, CAMERA_KEEP, 296, 1);
        fdCameraWaitForArrival();
        syFieldScriptWait(30);
        fdScriptPlaySound(SE_RISE_WITH_SPARKS_LOW);
        REG_BLDALPHA = BLEND_ALPHA(0, 16);
        REG_BLDCNT   = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
        {
            ACTOR *prepared    = fdActorCreateSilhouette(other->x.fixed,
                                                      other->groundHeight,
                                                      other->z.fixed + toFIXED(48),
                                                      SPRITE_YELLOW_BLOCK);

            other->motionFlags = 0;
            actor              = prepared;
        }
        i = 0;
        do
        {
            other->y.fixed -= toFIXED(1);
            syFieldScriptWait(5);
            i++;
        } while (i != PILLAR_SINK_DEPTH);
        fdScriptPlaySound(SE_FLAME_GROW);
        fdActorSelectAnimation(other, 4);
        fdActorRelease(actor);
        syFieldScriptWait(1);
        REG_BLDCNT = 0;
        scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 38, 18, 0);
        fdScriptEnd();
    }
}

// Nothing visible happens when Blaze, cast at a flame in the ember stone's room, reaches no torch;
// the game takes away the hidden flames it had set waiting on the unlit torches.
void scMagmaRockInsidePutOutFlames(void)
{
    if (FieldStateWork->psynergyHitObject == -1)
    {
        if (!fdScriptGetFlag(FLAG_EAST_TORCH_LIT))
            fdActorSetPos(ACTOR_EAST_TORCH_FLAME, 0, 0);
        if (!fdScriptGetFlag(FLAG_WEST_TORCH_LIT))
            fdActorSetPos(ACTOR_WEST_TORCH_FLAME, 0, 0);
    }
}

// Examining the ember stone, the red rock the eruption threw onto an islet in the lava (metatile
// (43, 14)): "There's something burning like an ember within the stone." Answering yes, Felix takes
// the Magma Ball from it, if someone in the party can carry it.
void scMagmaRockInsideExamineEmberStone(void)
{
    const Uint8 *msg;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    msg = MsgBlazeStoneEmber;
    mnMessageShowCentered(msg, 1);
    syFieldScriptSetMessageId(msg + 1);
    fdScriptOpenMessage(ACTOR_EMBER_STONE, 0);
    if (syFieldScriptAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId, 0) == 0)
    {
        gmPartyGiveItem(ITEM_MAGMA_BALL, 0);
        if (syFieldPartyInventoryFindMember(ITEM_MAGMA_BALL) == -1)
        {
            fdScriptEnd();
            return;
        }
        fdItemDisplayShowItem(ITEM_MAGMA_BALL, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
        fdActorSetAnimation(ACTOR_EMBER_STONE, 2);
        fdScriptSetFlag(FLAG_MAGMA_BALL_TAKEN);
    }
    fdScriptEnd();
}

// Walking north into the stone wall under its golden crest, at the end of the sunken pillars' path
// in the ember stone room's west (metatile (15, 21)), opens a stairway in it step by step, and
// Felix walks in and on to the Blaze tablet's room.
void scMagmaRockInsideOpenStairs(void)
{
    Sint32 width  = 1;
    Sint32 height = 2;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdScriptPlaySound(SE_THUMP);
    fdMapGridCopyMetatilesTo(72, 67, 15, 19, width, height);
    syFieldScriptWait(8);
    fdMapGridCopyMetatilesTo(73, 67, 15, 19, width, height);
    syFieldScriptWait(8);
    fdMapGridCopyMetatilesTo(74, 67, 15, 19, width, height);
    syFieldScriptWait(8);
    fdMapGridCopyMetatilesTo(75, 67, 15, 19, width, height);
    syFieldScriptWait(15);
    fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlags = 0;
    fdActorGet(MainRoutineStateWork.leaderActorId)->speedLimit =
        fdActorGet(MainRoutineStateWork.leaderActorId)->acceleration = toFIXED(1) / 2;
    fdScriptPlaySound(SE_MAP_EXIT);
    fdActorWalkTo(MainRoutineStateWork.leaderActorId, 248, 328);
    syFieldScriptWait(5);
    syFieldScriptSetExitRequest(MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_BLAZE_TABLET);
    fdScriptEnd();
}

// A tick event: once Felix stops on a new metatile, puts back the picture of the stepping stone he
// left and, in the Blaze tablet's room, raises the cue of that stone if the map's upper layer
// numbers it (STEP_METATILE_CUE_BASE plus its number, scMagmaRockInsideStepOnMarker).
void scMagmaRockInsideTrackStepMetatile(void)
{
    ACTOR       *actor = fdActorGet(syFieldPartyGetLeaderActor());
    Sint32       x     = FIELD_METATILE(actor->x.fixed);
    Sint32       z     = FIELD_METATILE(actor->z.fixed);
    Sint32       key   = METATILE_KEY(x, z);
    Sint32       last;
    Sint32       value;
    Sint32       cue;
    FIELD_STATE *state;

    if (actor->velocityX != 0 || actor->velocityZ != 0)
        return;
    last = MagmaRockInsideLastStepMetatile;
    if (last == -1 || key == last)
        return;
    x                               = METATILE_KEY_X(last);
    z                               = METATILE_KEY_Z(last);
    MagmaRockInsideLastStepMetatile = -1;
    fdMapGridCopyMetatilesLayered(73,
                                  72,
                                  MagmaRockInsideStepMetatileWidth,
                                  MagmaRockInsideStepMetatileHeight,
                                  MagmaRockInsideStepMetatileX,
                                  MagmaRockInsideStepMetatileZ);
    if (MainRoutineStateWork.next != SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
        return;
    state = RamHeapSlots->fieldState;
    value = fdMapMetatileGetAttributes(MAPGRID_LAYER_OVERHEAD, x, z) >> 8;
    if (value != 0)
    {
        cue              = value + STEP_METATILE_CUE_BASE;
        state->raisedCue = cue;
    }
}

// Starts watching the stepping stones Felix walks on in the Blaze tablet's room
// (scMagmaRockInsideTrackStepMetatile).
void scMagmaRockInsideStartStepMetatiles(void)
{
    MagmaRockInsideLastStepMetatile = -1;
    syFieldSetTickEvent(scMagmaRockInsideTrackStepMetatile, EVENT_PASS_TICK);
}

// Nothing seems to happen as Felix steps onto one of the stepping stones that cross the Blaze
// tablet room's lava, but he is seen standing on top of it: the game saves the stone's picture
// under him (to metatile (73, 72)) and draws the picture kept at (70, 72) in its place, until he
// leaves it (scMagmaRockInsideTrackStepMetatile). The stones of this picture alternate with the
// other kind along each row (scMagmaRockInsideStampOtherStepMetatile), as Air's Rock's do
// (scAirsRockInsideStampWideMetatileUnderLeader).
void scMagmaRockInsideStampStepMetatile(void)
{
    ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
    Sint32 x     = FIELD_METATILE(actor->x.fixed);
    Sint32 y     = FIELD_METATILE(actor->y.fixed);
    Sint32 z     = FIELD_METATILE(actor->z.fixed);
    Sint32 key   = METATILE_KEY(x, z);

    if (key != MagmaRockInsideLastStepMetatile)
    {
        MagmaRockInsideLastStepMetatile   = key;
        MagmaRockInsideStepMetatileWidth  = 1;
        MagmaRockInsideStepMetatileHeight = 2;
        MagmaRockInsideStepMetatileX      = x + 64;
        MagmaRockInsideStepMetatileZ      = z - y;
        fdMapGridCopyMetatilesLayered(
            MagmaRockInsideStepMetatileX, MagmaRockInsideStepMetatileZ, 1, 2, 73, 72);
        fdMapGridCopyMetatilesLayered(70,
                                      72,
                                      MagmaRockInsideStepMetatileWidth,
                                      MagmaRockInsideStepMetatileHeight,
                                      MagmaRockInsideStepMetatileX,
                                      MagmaRockInsideStepMetatileZ);
    }
}

// Nothing seems to happen as Felix steps onto a stepping stone of the other kind in the Blaze
// tablet room's lava, but he is seen standing on top of it: as scMagmaRockInsideStampStepMetatile,
// drawing the picture kept at (71, 72).
void scMagmaRockInsideStampOtherStepMetatile(void)
{
    ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
    Sint32 x     = FIELD_METATILE(actor->x.fixed);
    Sint32 y     = FIELD_METATILE(actor->y.fixed);
    Sint32 z     = FIELD_METATILE(actor->z.fixed);
    Sint32 key   = METATILE_KEY(x, z);

    if (key != MagmaRockInsideLastStepMetatile)
    {
        MagmaRockInsideLastStepMetatile   = key;
        MagmaRockInsideStepMetatileWidth  = 1;
        MagmaRockInsideStepMetatileHeight = 2;
        MagmaRockInsideStepMetatileX      = x + 64;
        MagmaRockInsideStepMetatileZ      = z - y;
        fdMapGridCopyMetatilesLayered(
            MagmaRockInsideStepMetatileX, MagmaRockInsideStepMetatileZ, 1, 2, 73, 72);
        fdMapGridCopyMetatilesLayered(71,
                                      72,
                                      MagmaRockInsideStepMetatileWidth,
                                      MagmaRockInsideStepMetatileHeight,
                                      MagmaRockInsideStepMetatileX,
                                      MagmaRockInsideStepMetatileZ);
    }
}

// The stepping stone at (x, z), in pixels, in the line of stepping stones that leads to the Blaze
// tablet, glows white with a drip and fades away into the lava (an object window over it, blended
// out), and its metatile is walled off.
void scMagmaRockInsideFadeStoneAway(Sint32 x, Sint32 z)
{
    ACTOR *actor;
    Sint32 i;
    Sint32 metatileZ;

    actor        = scFieldActorCreate(SPRITE_STACKED_CRATES, toFIXED(x), 0, toFIXED(z));
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
    REG_BLDCNT   = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    fdSetActorSpritePriority(actor, 0);
    fdActorSetSpriteFlags(actor, 0);
    ((FIELD_SPR *)actor->sprite)->objectMode = OBJ_MODE_WINDOW;
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3
               | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_ALL;
    REG_DISPCNT |= DISPCNT_OBJWIN_ON;
    fdScriptPlaySound(SE_DRIP);
    for (i = 0; i != BLEND_WEIGHT_MAX / 2; i++)
    {
        REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, i * 2);
        syWait(1);
    }
    for (i = 0; i != BLEND_WEIGHT_MAX; i++)
    {
        REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - i, BLEND_WEIGHT_MAX);
        syWait(1);
    }
    x         >>= 4;
    metatileZ   = (z >> 4) - 1;
    fdMapGridCopyMetatilesTo(65, 1, x + 64, metatileZ, 1, 2);
    scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, x, metatileZ, METATILE_BEHAVIOR_WALL << 8);
    fdActorRelease(actor);
    fdScriptEnd();
    REG_BLDCNT   = 0;
    REG_DISPCNT &= ~DISPCNT_OBJWIN_ON;
}

// Felix walks north from the Blaze tablet room's entrance along the line of stepping stones to the
// tablet's platform (column 16 of the map): on every second stone, the stone he has just stepped
// off glows white with a drip and fades away into the lava, so there is no way back. Five go in
// all, the first the stone just north of the entrance's walkway, the last the stone just below the
// tablet's platform. Retreat is barred in this room too, so the party leaves by the way Blaze opens
// (scMagmaRockInsideGrowTabletFlame). The flags keep the stones gone.
void scMagmaRockInsideStepOnMarker(void)
{
    FIELD_STATE *state = FieldStateWork;
    ACTOR       *leader;
    Sint16      *cue;
    Sint32       metatile, row = 54;

    leader   = fdActorGet(MainRoutineStateWork.leaderActorId);
    metatile = fdMapMetatileGetAttributes(
        MAPGRID_LAYER_OVERHEAD, FIELD_METATILE(leader->x.fixed), FIELD_METATILE(leader->z.fixed));
    cue        = &state->raisedCue;
    // The marker's row: 54 for the first, four rows up for each after it.
    row       -= (*cue - (STEP_METATILE_CUE_BASE + 1)) << 2;
    metatile >>= 8;
    if (metatile != MARKER_WALKWAY_END && metatile != MARKER_PLATFORM_FOOT)
    {
        scMagmaRockInsideStampStepMetatile();
        scMagmaRockInsideFadeStoneAway(264, row * 8 + 10);
        if (!fdScriptGetFlag(FLAG_ANY_MARKER_LIT))
            fdScriptSetFlag(FLAG_ANY_MARKER_LIT);
        fdScriptSetFlag(FLAG_MARKER_LIT(*cue - STEP_METATILE_CUE_BASE));
    }
    else if (fdScriptGetFlag(FLAG_ANY_MARKER_LIT))
    {
        scMagmaRockInsideFadeStoneAway(264, row * 8 + 10);
    }
}

// Felix casts Blaze at the flame in the west bowl on the ledge in the Blaze tablet room's
// south-west: unless the east bowl beside it is lit, a small hidden flame waits on it, which grows
// if the spray reaches it (scMagmaRockInsideGrowTabletFlame).
DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(
    MagmaRockInsideLightTabletTorch, FLAG_TABLET_TORCH_LIT, ACTOR_TORCH_FLAME, 136, 360)

// Blaze's spray reaches the east bowl on the Blaze tablet room's south-west ledge, and its flame
// grows until it burns full; the ground rumbles, the camera turns to the stone pillar between the
// bowls, and it sinks into the floor until its top is a stepping stone across the lava (metatile
// (7, 21)), the way out of the room.
void scMagmaRockInsideGrowTabletFlame(void)
{
    ACTOR *object;
    ACTOR *actor;
    ACTOR *other;
    Sint32 i;

    object = fdActorGet(FieldStateWork->psynergyHitObject);
    scFieldActorSetSpriteColor(object, SPRITE_COLOR_NORMAL);
    fdScriptPlaySound(SE_WHOOSH);
    i = 0;
    do
    {
        object->scaleY = object->scaleX += toFIXED(1) / FLAME_GROW_STEPS;
        syWait(5);
        i++;
    } while (i != FLAME_GROW_STEPS);
    fdScriptSetFlag(FLAG_TABLET_TORCH_LIT);
    other = fdActorGet(ACTOR_TORCH_PILLAR);
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdMapGridSetShake(2, 1, 1);
    fdScriptPlaySound(SE_RUMBLE);
    syFieldScriptWait(30);
    fdCameraMoveTo(120, CAMERA_KEEP, 344, 1);
    fdCameraWaitForArrival();
    syFieldScriptWait(30);
    fdScriptPlaySound(SE_RISE_WITH_SPARKS_LOW);
    REG_BLDALPHA = BLEND_ALPHA(0, 16);
    REG_BLDCNT   = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    {
        ACTOR *prepared = fdActorCreateSilhouette(
            other->x.fixed, other->groundHeight, other->z.fixed + toFIXED(48), SPRITE_YELLOW_BLOCK);

        other->motionFlags = 0;
        actor              = prepared;
    }
    i = 0;
    do
    {
        other->y.fixed -= toFIXED(1);
        syFieldScriptWait(5);
        i++;
    } while (i != PILLAR_SINK_DEPTH);
    fdScriptPlaySound(SE_FLAME_GROW);
    fdActorSelectAnimation(other, 4);
    fdActorRelease(actor);
    syFieldScriptWait(1);
    REG_BLDCNT = 0;
    scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 7, 21, MAP_METATILE_JUMP);
    fdScriptEnd();
}

// Nothing visible happens when Blaze, cast at the flame on the Blaze tablet room's south-west
// ledge, does not reach the east bowl; the game takes away the hidden flame it had set waiting
// there.
void scMagmaRockInsidePutOutTabletFlame(void)
{
    if (FieldStateWork->psynergyHitObject == -1)
    {
        if (!fdScriptGetFlag(FLAG_TABLET_TORCH_LIT))
            fdActorSetPos(ACTOR_TORCH_FLAME, 0, 0);
    }
}

// Leaves a small red spark behind a spark circling in to Jenna as she learns Blaze
// (scMagmaRockInsideOrbitParent).
void scMagmaRockInsideSpawnSpark(ACTOR *src)
{
    ACTOR *actor       = Create(SPRITE_PARTICLE, src->x.fixed, src->y.fixed, src->z.fixed);

    actor->scaleY      = toFIXED(2) / 5;
    actor->scaleX      = toFIXED(2) / 5;
    actor->motionFlags = 0;
    scFieldActorSetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
    fdSetActorSpritePriority(actor, 1);
    fdActorSelectAnimation(actor, 1);
    fdActorSetSpriteFlags(actor, 0);
    fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
    fdActorStartScript(actor, MagmaRockInsideFlameScript);
}

// A spark circling Jenna as she learns Blaze, as its script reads the actor: its place, the radius
// it circles at, the steps it has left, its angle and speed, and the actor it circles.
typedef struct SPARK_ORBITER
{
    Uint8                 beforeX[8];
    Sint32                x;
    Sint32                y;
    Sint32                z;
    Uint8                 beforeRadius[56];
    Sint32                radius;
    Uint8                 beforeTimer[18];
    Uint8                 stepsLeft;
    Uint8                 afterStepsLeft;
    Uint16                angle;
    Uint16                speed;
    struct SPARK_ORBITER *parent;
} SPARK_ORBITER;

// A spark's script call: circles in to its parent, leaving sparks, until its steps run out; 0 ends
// its script.
Bool scMagmaRockInsideOrbitParent(SPARK_ORBITER *object)
{
    Uint16        *angle  = &object->angle;
    Uint16        *speed  = &object->speed;
    SPARK_ORBITER *parent = object->parent;
    Sint32        *pos;
    Sint16         heading;

    object->angle += (Sint16)*speed >> 1;
    object->x      = parent->x;
    object->y      = parent->y + toFIXED(8);
    object->z      = parent->z;
    heading        = *angle;
    syFieldVectorAddPolarOffsetFX(object->radius, heading, pos = &object->x);
    if (--object->stepsLeft == 0)
        return FALSE;
    if (object->radius == toFIXED(2))
        return TRUE;
    scMagmaRockInsideSpawnSpark(object);
    object->radius -= toFIXED(4);
    object->angle  += (Sint16)*speed >> 1;
    *pos            = parent->x;
    object->y       = parent->y + toFIXED(8);
    object->z       = parent->z;
    syFieldVectorAddPolarOffsetFX(object->radius, (Sint16)*angle, pos);
    scMagmaRockInsideSpawnSpark(object);
    object->radius -= toFIXED(4);
    return TRUE;
}

// Moves around in a circle.
#include "shared/orbit_update.h"

#include "shared/actor_aim_at_watched.h"

// A rising spark's script call: drifts by its centre and radius, kept as velocities, for its
// phase's count of steps; 0 ends its script.
Sint32 scMagmaRockInsideDriftForSteps(ORBIT *object)
{
    Uint8 *steps;
    Sint32 left;
    Uint8  stepsLeft;

    object->x += object->centreX;
    object->y += object->orbitRadius;
    object->z += object->centreZ;
    steps      = &object->phase;
    left       = *steps + (Uint8)-1;
    *steps     = left;
    stepsLeft  = left;
    return stepsLeft != 0;
}

// An actor's update: drops a white sparkle near it every eighth tick.
void scMagmaRockInsideTrailSparks(ACTOR *actor)
{
    EFFECT_OPTIONS options;
    FIXED          pos[3];
    Uint32         frame = TickCount & 7;

    if (frame != 0)
        return;
    options.palette  = SPRITE_COLOR_WHITE;
    options.type     = SPRITE_SPARKLE;
    options.priority = 1;
    pos[0]           = actor->x.fixed + (((syRandom() & 31) - 16) << 16);
    pos[1]           = actor->y.fixed + toFIXED(syRandom() & 31) + toFIXED(8);
    pos[2]           = actor->z.fixed;
    scFieldEffectSpawn(pos[0],
                       pos[1],
                       pos[2] + toFIXED(4),
                       0,
                       toFIXED(1) / 2,
                       frame,
                       EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                       &options);
}

extern Uint32 LessonTabletRiseScript[];
extern Sint32 MagmaRockInsideRisingSparkScript[];
extern Sint32 MagmaRockInsideCirclingSparkScript[];
extern Uint32 LessonAdeptRiseScript[];
extern Sint32 MagmaRockInsideOrbitScript[];
extern Sint32 LessonTabletSettleScript[];
extern Sint32 LessonAdeptSettleScript[];

// The Blaze tablet's lesson: the music fades, the ground shakes and the screen takes on a warm tint
// as the tablet lifts, blinking and shooting sparks up; red sparks circle in to the one who learns
// and rise round her as she lifts too, the screen flashes white, and both settle back as the music
// returns.
void          scMagmaRockInsideLearnBlaze(Sint32 tabletId, Sint32 learnerId)
{
    ACTOR       *tablet  = fdActorGetById(tabletId);
    ACTOR       *learner = fdActorGetById(learnerId);
    ACTOR       *actor;
    ACTOR       *spark;
    ACTOR       *riser;
    Sint32       i;
    Uint16       noMotion;
    Sint32       probe;
    const Uint8 *script;
    // LEFTOVER(frame): an unused local the ROM's frame reserves and never
    // * touches.
    Sint32       unused[3];

    script = LessonAdeptRiseScript;
    fdScriptPlaySound(SOUND_FADE_OUT_BGM);
    fdScriptWait(30);
    fdCameraSetSpeed(0.5);
    fdCameraMoveToFX(toFIXED(tablet->x.part[0].integer),
                     toFIXED(tablet->y.part[0].integer),
                     toFIXED(tablet->z.part[0].integer),
                     1);
    fdScriptPlaySound(SE_RUMBLE);
    fdMapGridSetShake(2, 2, 1);
    fdPaletteFadeSetSource(PALETTE_TINT_NONE, 0);
    fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 1);
    fdPaletteFadeStartInterpolation(60);
    tablet->motionFlags  = 0;
    tablet->acceleration = toFIXED(1) / 5;
    tablet->speedLimit   = toFIXED(1) / 5;
    fdActorSetScript(tabletId, LessonTabletRiseScript);
    fdScriptPlaySound(SE_TABLET_RISE);
    fdScriptWait(45);
    fdActorSetSpriteColorById(tabletId, SPRITE_COLOR_BLINK);
    REG_BLDALPHA = BLEND_ALPHA(8, 16);
    REG_BLDCNT   = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    i            = 0;
    do
    {
        actor = (ACTOR *)syImportActorCreate(
            SPRITE_PARTICLE, tablet->x.fixed, tablet->y.fixed, tablet->z.fixed);
        fdScriptPlaySound(SE_GLITTER);
        actor->x.fixed      += ((syRandom() & 31) - 16) << 16;
        actor->y.fixed      += ((syRandom() & 15) - 8) << 16;
        actor->speedLimit    = toFIXED(8);
        actor->acceleration  = toFIXED(1) / 5;
        actor->motionFlags   = 0;
        scFieldActorSetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
        fdSetActorSpritePriority(actor, 1);
        fdActorSelectAnimation(actor, 7);
        fdActorSetSpriteFlags(actor, 0);
        fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
        fdActorStartScript(actor, MagmaRockInsideRisingSparkScript);
        REG_BLDALPHA = BLEND_ALPHA(4, 16);
        sySynch(2);
        REG_BLDALPHA = BLEND_ALPHA(10, 16);
        sySynch(2);
        REG_BLDALPHA = BLEND_ALPHA(16, 16);
        sySynch(2);
        REG_BLDALPHA = BLEND_ALPHA(10, 16);
        sySynch(2);
        i++;
    } while (i != TABLET_RISING_SPARKS);
    REG_BLDALPHA = BLEND_ALPHA(8, 16);
    sySynch(30);
    for (i = 0; i != LEARN_SPARKS; i++)
    {
        spark = scFieldActorCreate(
            SPRITE_PARTICLE, learner->x.fixed, learner->y.fixed, learner->z.fixed);
        fdScriptPlaySound(SE_SPARKS);
        spark->timer        = (syRandom() & 0x7fff) + DEGtoANG(180);
        spark->userValue    = (((i & 1) << 1) - 1) << ((syRandom() & 3) + 9);
        noMotion            = 0;
        spark->motionFlags  = noMotion;
        spark->sceneWord    = toFIXED(122);
        spark->watchedActor = (FIELD_OBJECT *)learner;
        spark->stepsLeft    = (LEARN_SPARKS - i) * LEARN_SPARK_INTERVAL + LEARN_SPIRAL_TICKS;
        scFieldActorSetSpriteColor(spark, SPRITE_COLOR_RED_TINT);
        fdSetActorSpritePriority(spark, 1);
        fdActorSelectAnimation(spark, 7);
        fdActorSetSpriteFlags(spark, 0);
        fdActorSetBlendMode(spark, OBJ_MODE_SEMI_TRANSPARENT);
        syFieldVectorAddPolarOffsetFX(spark->sceneWord, spark->timer, &spark->x.fixed);
        fdActorStartScript(spark, MagmaRockInsideCirclingSparkScript);
        if (i == 3)
        {
            fdActorSetSpriteColorById(learnerId, SPRITE_COLOR_BLINK);
            learner->motionFlags  = noMotion;
            learner->acceleration = toFIXED(1) / 5;
            learner->speedLimit   = toFIXED(1) / 5;
            fdActorSetScript(learnerId, script);
        }
        sySynch(LEARN_SPARK_INTERVAL);
    }
    fdScriptPlaySound(SE_SHIMMER);
    sySynch(16);
    i = 2;
    do
    {
        riser                    = scFieldActorCreate(SPRITE_PARTICLE,
                                   learner->x.fixed,
                                   learner->y.fixed + i * toFIXED(1) - toFIXED(8),
                                   learner->z.fixed);
        riser->timer             = syRandom();
        riser->userValue         = TRIG_TURN / 32;
        ((ORBIT *)riser)->phase  = i;
        ((ORBIT *)riser)->growth = 1;
        riser->bounce            = riser->x.fixed;
        riser->sceneWord         = 0;
        riser->gravity           = riser->z.fixed;
        riser->watchedActor      = (FIELD_OBJECT *)learner;
        riser->motionFlags       = (Uint16)0;
        riser->priorityFlags     = ACTOR_PRIORITY_AUTOMATIC;
        scFieldActorSetSpriteColor(riser, SPRITE_COLOR_RED_TINT);
        fdActorSelectAnimation(riser, 7);
        fdActorSetSpriteFlags(riser, 0);
        fdActorSetBlendMode(riser, OBJ_MODE_SEMI_TRANSPARENT);
        fdActorStartScript(riser, MagmaRockInsideOrbitScript);
        i += 2;
    } while (i != ORBIT_HALF_TURN);
    fdScriptPlaySound(SE_SHIMMER);
    sySynch(50);
    fdPaletteFadeSetTarget(RGB_WHITE, 0);
    fdPaletteFadeStartInterpolation(8);
    sySynch(16);
    fdMapGridSetShake(0, 0, 0);
    fdActorSetSpriteColorById(tabletId, SPRITE_COLOR_NORMAL);
    fdActorSetSpriteColorById(learnerId, SPRITE_COLOR_NORMAL);
    fdScriptPlaySound(SE_FLASH);
    fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
    fdPaletteFadeStartInterpolation(80);
    fdActorSetScript(tabletId, LessonTabletSettleScript);
    fdActorSetScript(learnerId, LessonAdeptSettleScript);
    learner->update = (OBJECT_UPDATE)scMagmaRockInsideTrailSparks;
    sySynch(120);
    learner->update = NULL;
    fdScriptPlaySound(SOUND_FADE_OUT_ACTION_SE);
    gmPlayMusic();
    sySynch(1);
}

// Felix reads the Blaze tablet from the south: "Wielder of Flame's strength, Lay your hands upon
// this stone." The first time, Jenna steps out beside him ("Flame's strength? Hey, that must mean
// it's finally my turn!"), he nods and steps back, she lays her hands on the tablet and learns
// Blaze, and after they nod to each other she walks back into the party. Once Blaze is learned,
// only the inscription is shown.
void scMagmaRockInsideReadBlazeTablet(void)
{
    ACTOR *actor;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    mnMessageShowCentered(MsgBlazeTabletInscription, 1);
    if (fdScriptGetFlag(FLAG_BLAZE_LEARNED) != 0)
    {
        fdScriptEnd();
    }
    else
    {
        syFieldScriptSetMessageId((Sint32)MsgBlazeTabletInscription);
        FieldStateWork->msg++;
        fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.7);
        fdActorSetSpeed(ACTOR_JENNA, 0.7);
        fdActorPlaceOnActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
        syFieldScriptWait(1);
        fdActorWalkTo(MainRoutineStateWork.leaderActorId, 256, 136);
        fdActorWalkTo(ACTOR_JENNA, 272, 136);
        scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
        scFieldActorWaitForMoveById(ACTOR_JENNA);
        fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
        fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
        syFieldScriptWait(10);
        // "Flame's strength? Hey, that must mean it's finally my turn!"
        fdScriptShowMessage(ACTOR_JENNA, 0);
        fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
        syFieldScriptWait(30);
        fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 256, 152);
        fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
        syFieldScriptWait(20);
        fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
        syFieldScriptWait(20);
        fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
        syFieldScriptWait(30);
        fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
        syFieldScriptWait(20);
        fdActorWalkToAndWait(ACTOR_JENNA, 264, 137);
        fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
        syFieldScriptWait(20);
#if !BLAZE_TABLET_TOUCH_LINE_FIXED
        mnMessageShowCentered(MsgBlazeTabletInscription + 3, 1);
#else
        mnMessageShowCentered(MsgBlazeTabletInscription + 2, 1);
#endif
        fdMapGridSetShake(5, 7, 1);
        fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        syFieldMapGridWaitForShakeToSettle();
        scMagmaRockInsideLearnBlaze(ACTOR_BLAZE_TABLET, ACTOR_JENNA);
        fdPartyLearnAbility(JENNA, ABILITY_BLAZE);
        fdScriptSetFlag(FLAG_BLAZE_LEARNED);
        fdActorFaceEachOther(MainRoutineStateWork.leaderActorId, ACTOR_JENNA, 0);
        syFieldScriptWait(10);
        fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
        fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
        syFieldScriptWait(20);
        fdActorSetSpeed(ACTOR_JENNA, 1.2);
        fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
        actor = fdActorGet(MainRoutineStateWork.leaderActorId);
        if (actor != NULL)
            fdActorSetDestination(ACTOR_JENNA, actor->x.part[0].integer, actor->z.part[0].integer);
        scFieldActorWaitForMoveById(ACTOR_JENNA);
        fdActorSetPos(ACTOR_JENNA, 0, 0);
        fdActorSetMovementType(fdActorGet(ACTOR_BLAZE_TABLET), MOVEMENT_TYPE_TAKE_LEADER_LAYER);
        fdScriptEnd();
    }
}

// Felix reads the Blaze tablet from behind, on its north side: the first time he walks round its
// west side to the front and it is read there (scMagmaRockInsideReadBlazeTablet); once Blaze is
// learned only its inscription is shown.
void scMagmaRockInsideReadBlazeTabletFromNorth(void)
{
    if (fdScriptGetFlag(FLAG_BLAZE_LEARNED))
    {
        fdScriptStart();
        syFieldScriptEndLastingPsynergy(0);
        mnMessageShowCentered(MsgBlazeTabletInscription, 1);
        fdScriptEnd();
    }
    else
    {
        fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 248, 107);
        fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 248, 136);
        fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 264, 136);
        fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
        syFieldScriptWait(10);
        scMagmaRockInsideReadBlazeTablet();
    }
}

// Defines a function for reading the Blaze tablet from one side: once Blaze is learned only its
// inscription shows; before, Felix walks from sideX round to its front, faces it, and reads it.
#define DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM(name, sideX)                                     \
    void name(void)                                                                                \
    {                                                                                              \
        if (fdScriptGetFlag(FLAG_BLAZE_LEARNED))                                                   \
        {                                                                                          \
            fdScriptStart();                                                                       \
            syFieldScriptEndLastingPsynergy(0);                                                    \
            mnMessageShowCentered(MsgBlazeTabletInscription, 1);                                   \
            fdScriptEnd();                                                                         \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, sideX, 136);                  \
            fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 264, 136);                    \
            fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);             \
            syFieldScriptWait(10);                                                                 \
            scMagmaRockInsideReadBlazeTablet();                                                    \
        }                                                                                          \
    }

// Felix reads the Blaze tablet from its east side: the first time he steps round to the front and
// it is read there (scMagmaRockInsideReadBlazeTablet); once Blaze is learned only its inscription
// is shown.
DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM(MagmaRockInsideReadBlazeTabletFromEast, 280)

// Felix reads the Blaze tablet from its west side: the first time he steps round to the front and
// it is read there (scMagmaRockInsideReadBlazeTablet); once Blaze is learned only its inscription
// is shown.
DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM(MagmaRockInsideReadBlazeTabletFromWest, 248)

extern CUE MagmaRockInsideCues[];
extern CUE MagmaRockInsideEntranceCues[];
extern CUE MagmaRockInsideFuryCues[];
extern CUE MagmaRockInsideCrackedPillarsCues[];
extern CUE MagmaRockInsideGolemCoreChestCues[];
extern CUE MagmaRockInsideSalamanderTailChestCues[];
extern CUE MagmaRockInsideEmberStoneCues[];
extern CUE MagmaRockInsideBlazeTabletCues[];

const CUE *scGetCues(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene != SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST)
    {
        if (scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
            return MagmaRockInsideEntranceCues;
        if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
            return MagmaRockInsideFuryCues;
        if (scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
            return MagmaRockInsideCrackedPillarsCues;
        if (scene == SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST)
            return MagmaRockInsideGolemCoreChestCues;
        if (scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST)
            return MagmaRockInsideSalamanderTailChestCues;
        if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
            return MagmaRockInsideEmberStoneCues;
        if (scene == SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
            return MagmaRockInsideBlazeTabletCues;
    }
    return MagmaRockInsideCues;
}

// The entrance hall as flooded: its flag set, the metatile at (15, 13) no longer climbable, and its
// columns and pillar shown standing in the lava.
void scMagmaRockInsideSetEntranceFlooded(void)
{
    Sint32 i;

    fdScriptSetFlag(FLAG_ENTRANCE_FLOODED);
    scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 15, 13, 0);
    for (i = 0; i != ACTOR_COLUMN_F - ACTOR_COLUMN_A + 1; i++)
        fdActorSetAnimation(i + ACTOR_COLUMN_A, 2);
    fdActorSetAnimation(ACTOR_ENTRANCE_PILLAR, 4);
}

// Fury's room as flooded: its flag set, the metatiles at (24, 15) and (9, 16) no longer climbable,
// and its pillar shown standing in the lava; from the cracked pillars' room, only the metatile at
// (26, 14).
void scMagmaRockInsideSetFuryFlooded(void)
{
    fdScriptSetFlag(FLAG_FURY_FLOODED);
    if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY)
    {
        scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 24, 15, 0);
        scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 9, 16, 0);
        fdActorSetAnimation(ACTOR_FURY_PILLAR, 4);
    }
    else
    {
        scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 26, 14, 0);
    }
}

// The lower rooms as flooded: their flag set and, in the cracked pillars' room, its eight white
// platforms afloat in pairs, each pair's second starting across the square from the first, and its
// three pillars and column shown standing in the lava.
void scMagmaRockInsideSetLowerRoomsFlooded(void)
{
    fdScriptSetFlag(FLAG_LOWER_ROOMS_FLOODED);
    if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
    {
        Sint32 *script = MagmaRockInsideCrackedPillarsFirstPairPath;

        scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_A, script, FLAG_LOWER_ROOMS_FLOODED);
        scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_B,
                                           MagmaRockInsideCrackedPillarsFirstPairPath,
                                           FLAG_LOWER_ROOMS_FLOODED);
        scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_C,
                                           MagmaRockInsideCrackedPillarsSecondPairPath,
                                           FLAG_LOWER_ROOMS_FLOODED);
        scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_D,
                                           MagmaRockInsideCrackedPillarsSecondPairPath,
                                           FLAG_LOWER_ROOMS_FLOODED);
        scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_E,
                                           MagmaRockInsideCrackedPillarsThirdPairPath,
                                           FLAG_LOWER_ROOMS_FLOODED);
        scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_F,
                                           MagmaRockInsideCrackedPillarsThirdPairPath,
                                           FLAG_LOWER_ROOMS_FLOODED);
        scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_G,
                                           MagmaRockInsideCrackedPillarsFourthPairPath,
                                           FLAG_LOWER_ROOMS_FLOODED);
        scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_H,
                                           MagmaRockInsideCrackedPillarsFourthPairPath,
                                           FLAG_LOWER_ROOMS_FLOODED);
        if (fdActorGet(ACTOR_LAVA_PLATFORM_B)->scriptCursor == 0)
        {
            fdActorGet(ACTOR_LAVA_PLATFORM_B)->scriptCursor = PLATFORM_PATH_THIRD_CORNER;
            fdActorGet(ACTOR_LAVA_PLATFORM_D)->scriptCursor = PLATFORM_PATH_THIRD_CORNER;
            fdActorGet(ACTOR_LAVA_PLATFORM_F)->scriptCursor = PLATFORM_PATH_THIRD_CORNER;
            fdActorGet(ACTOR_LAVA_PLATFORM_H)->scriptCursor = PLATFORM_PATH_THIRD_CORNER;
        }
        fdActorSetAnimation(ACTOR_PILLAR_A, 4);
        fdActorSetAnimation(ACTOR_PILLAR_B, 4);
        fdActorSetAnimation(ACTOR_PILLAR_C, 4);
        fdActorSetAnimation(ACTOR_LOWER_COLUMN, 2);
    }
}

// The entrance hall pillar's update: its drawing order comes from the metatiles under it, except
// while Felix stands at its height and not in front of it, when it is drawn at the lowest priority.
void scMagmaRockInsideLayerWithLeader(ACTOR *arg)
{
    MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
    ACTOR              *actor;
    ACTOR              *owner;
    Sint16              ownerY;
    Sint16              y;
    Uint8              *flags;

    actor  = arg;
    owner  = fdActorGet(party->leaderActorId);
    ownerY = FIXEDtoINT(owner->y.fixed);
    y      = FIXEDtoINT(actor->y.fixed);
    flags  = &actor->priorityFlags;

    if (ownerY != y || (Sint16)(FIXEDtoINT(owner->z.fixed)) < (Sint16)(FIXEDtoINT(actor->z.fixed)))
    {
        *flags = 1;
    }
    else
    {
        *flags = 0;
        fdSetActorSpritePriority(actor, 3);
    }
}

#undef DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR

// The crumbling pillars and floors of Magma Rock's outside follow, unused here.

extern Sint32 MagmaRockInsideLuckyMedalPlatformPath[];
extern Sint32 MagmaRockInsideFuryWestPlatformPath[];
extern Sint32 MagmaRockInsideFuryEastPlatformPath[];
extern Sint32 MagmaRockInsideCrackedPillarsWestPlatformPath[];
extern Sint32 MagmaRockInsideCrackedPillarsEastPlatformPath[];
extern Sint32 MagmaRockInsideSalamanderTailNorthPlatformPath[];
extern Sint32 MagmaRockInsideSalamanderTailSouthwestPlatformPath[];
extern Sint32 MagmaRockInsideSalamanderTailSoutheastPlatformPath[];
extern Sint16 MagmaRockInsideStatues[];

Sint32        scMapMetatileSetAttributes(Uint32 layer, Sint32 x, Sint32 y, Sint32 value);

// Blends BG2 out of sight: weight 0 over every layer beneath it.
#define CLOSED_BLEND()                                                                             \
    {                                                                                              \
        REG_BLDALPHA = BLEND_ALPHA(0, 16);                                                         \
        REG_BLDCNT   = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;                    \
    }

// Stands an unseen marker 20 pixels up on a floor switch, falling, as a platform Felix stands on.
static __inline__ void Raise(Sint32 id)
{
    ACTOR *actor       = fdActorGetById(id);

    actor->motionFlags = ACTOR_FALLS;
    actor->y.fixed = actor->groundHeight  = toFIXED(20);
    actor->priorityFlags                 |= ACTOR_PRIORITY_PLATFORM;
}

// Settles an actor at the given height, its gravity off.
static __inline__ void Lower(Sint32 id, Sint32 height)
{
    fdActorGetById(id)->motionFlags = 0;
    fdActorGetById(id)->y.fixed = fdActorGetById(id)->groundHeight = height;
}

Sint32 scInit(void)
{
    Sint32 i;

    FieldStateWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST)
    {
        fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
            ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
        scMagmaRockInsideStartLavaPlatform(ACTOR_LUCKY_MEDAL_PLATFORM,
                                           MagmaRockInsideLuckyMedalPlatformPath,
                                           FLAG_ENTRANCE_FLOODED);
        if (!fdScriptGetFlag(FLAG_ENTRANCE_FLOODED))
        {
            CLOSED_BLEND();
            fdMapGridCopyMetatilesTo(71, 64, 47, 98, 1, 1);
        }
    }
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
    {
        Sint32 j;

        fdEffectStartBlankOverlay(0);
        fdActorGetById(ACTOR_ENTRANCE_PILLAR)->update = scMagmaRockInsideLayerWithLeader;
        for (j = 0; j != ACTOR_COLUMN_F - ACTOR_COLUMN_A + 1; j++)
        {
            {
                ACTOR *actor       = fdActorGetById(j + ACTOR_COLUMN_A);

                actor->motionFlags = 0;
                actor->y.fixed = actor->groundHeight = 0;
            }
        }
        if (fdScriptGetFlag(FLAG_ENTRANCE_PILLAR_PUSHED))
            fdActorGetById(ACTOR_ENTRANCE_PILLAR)->z.fixed = toFIXED(264);
        fdActorPlaceBlocks(MagmaRockInsideEntrancePillars);
        scMagmaRockInsideRaiseActorMetatiles(MagmaRockInsideStatues);
        fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
            ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
        Raise(ACTOR_ENTRANCE_SWITCH_MARKER);
        if (fdScriptGetFlag(FLAG_ENTRANCE_FLOODED))
            scMagmaRockInsideSetEntranceFlooded();
        else
            CLOSED_BLEND();
    }
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY)
    {
        fdEffectStartBlankOverlay(0);
        if (fdScriptGetFlag(FLAG_FURY_PILLAR_PUSHED))
            fdActorGetById(ACTOR_FURY_PILLAR)->z.fixed = toFIXED(664);
        fdActorPlaceBlocks(MagmaRockInsideFuryPillars);
        fdTightropeInit();
        fdTightropePlace(0, ACTOR_TIGHTROPE_WEST_POST, ACTOR_TIGHTROPE_EAST_POST);
        scMagmaRockInsideRaiseActorMetatiles(MagmaRockInsideStatues);
        fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
            ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
        Raise(ACTOR_FURY_SWITCH_MARKER);
        scMagmaRockInsideStartLavaPlatform(
            ACTOR_WEST_PLATFORM, MagmaRockInsideFuryWestPlatformPath, FLAG_FURY_FLOODED);
        scMagmaRockInsideStartLavaPlatform(
            ACTOR_EAST_PLATFORM, MagmaRockInsideFuryEastPlatformPath, FLAG_FURY_FLOODED);
        if (fdScriptGetFlag(FLAG_FURY_FLOODED))
            scMagmaRockInsideSetFuryFlooded();
        else
            CLOSED_BLEND();
    }
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
    {
        fdEffectStartBlankOverlay(0);
        fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
            ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
        scMagmaRockInsideRaiseActorMetatiles(MagmaRockInsideCrackedPillarsStatues);
        if (MainRoutineStateWork.entrance <= CRACKED_PILLARS_UPPER_HALL_LAST_ENTRANCE)
        {
            scMagmaRockInsideStartLavaPlatform(ACTOR_WEST_PLATFORM,
                                               MagmaRockInsideCrackedPillarsWestPlatformPath,
                                               FLAG_FURY_FLOODED);
            scMagmaRockInsideStartLavaPlatform(ACTOR_EAST_PLATFORM,
                                               MagmaRockInsideCrackedPillarsEastPlatformPath,
                                               FLAG_FURY_FLOODED);
            if (fdScriptGetFlag(FLAG_FURY_FLOODED))
            {
                scMagmaRockInsideSetFuryFlooded();
            }
            else
            {
                CLOSED_BLEND();
                fdMapGridCopyMetatilesTo(71, 64, 35, 82, 1, 1);
            }
        }
        else
        {
            fdActorGetById(ACTOR_PILLAR_A)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
            fdActorGetById(ACTOR_PILLAR_B)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
            fdActorGetById(ACTOR_PILLAR_C)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
            fdActorSetSpritePriority(ACTOR_PILLAR_A, 3);
            fdActorSetSpritePriority(ACTOR_PILLAR_B, 3);
            fdActorSetSpritePriority(ACTOR_PILLAR_C, 3);
            if (fdScriptGetFlag(FLAG_PILLAR_A_PUSHED))
                fdActorGetById(ACTOR_PILLAR_A)->x.fixed = toFIXED(344);
            if (fdScriptGetFlag(FLAG_PILLAR_B_PUSHED))
                fdActorGetById(ACTOR_PILLAR_B)->x.fixed = toFIXED(344);
            if (fdScriptGetFlag(FLAG_PILLAR_C_PUSHED))
                fdActorGetById(ACTOR_PILLAR_C)->x.fixed = toFIXED(344);
            fdActorPlaceBlocks(MagmaRockInsideCrackedPillarsPillars);
            Lower(ACTOR_LOWER_COLUMN, 0);
            // The platforms ride in and out of view: each keeps its sprite's tiles in VRAM and its
            // picture composed while off screen, so it shows at once as it comes back.
            fdActorGetById(ACTOR_LAVA_PLATFORM_A)->retainSpriteCache =
                ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
            fdActorGetById(ACTOR_LAVA_PLATFORM_B)->retainSpriteCache =
                ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
            fdActorGetById(ACTOR_LAVA_PLATFORM_C)->retainSpriteCache =
                ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
            fdActorGetById(ACTOR_LAVA_PLATFORM_D)->retainSpriteCache =
                ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
            fdActorGetById(ACTOR_LAVA_PLATFORM_E)->retainSpriteCache =
                ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
            fdActorGetById(ACTOR_LAVA_PLATFORM_F)->retainSpriteCache =
                ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
            fdActorGetById(ACTOR_LAVA_PLATFORM_G)->retainSpriteCache =
                ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
            fdActorGetById(ACTOR_LAVA_PLATFORM_H)->retainSpriteCache =
                ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
            scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_A,
                                               MagmaRockInsideCrackedPillarsFirstPairPath,
                                               FLAG_LOWER_ROOMS_FLOODED);
            scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_B,
                                               MagmaRockInsideCrackedPillarsFirstPairPath,
                                               FLAG_LOWER_ROOMS_FLOODED);
            scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_C,
                                               MagmaRockInsideCrackedPillarsSecondPairPath,
                                               FLAG_LOWER_ROOMS_FLOODED);
            scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_D,
                                               MagmaRockInsideCrackedPillarsSecondPairPath,
                                               FLAG_LOWER_ROOMS_FLOODED);
            scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_E,
                                               MagmaRockInsideCrackedPillarsThirdPairPath,
                                               FLAG_LOWER_ROOMS_FLOODED);
            scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_F,
                                               MagmaRockInsideCrackedPillarsThirdPairPath,
                                               FLAG_LOWER_ROOMS_FLOODED);
            scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_G,
                                               MagmaRockInsideCrackedPillarsFourthPairPath,
                                               FLAG_LOWER_ROOMS_FLOODED);
            scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_H,
                                               MagmaRockInsideCrackedPillarsFourthPairPath,
                                               FLAG_LOWER_ROOMS_FLOODED);
            if (fdScriptGetFlag(FLAG_LOWER_ROOMS_FLOODED))
                scMagmaRockInsideSetLowerRoomsFlooded();
            else
                CLOSED_BLEND();
        }
    }
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST)
    {
        fdEffectStartBlankOverlay(0);
        fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
            ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
        fdHangingVinePlace(ACTOR_HANGING_CHAIN_A, 1);
        fdHangingVinePlace(ACTOR_HANGING_CHAIN_B, 1);
        Raise(ACTOR_NORTH_SWITCH_MARKER);
        Raise(ACTOR_SOUTH_SWITCH_MARKER);
        if (fdScriptGetFlag(FLAG_LOWER_ROOMS_FLOODED))
        {
            scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 22, 26, 0);
            scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 25, 26, 0);
            fdActorSetSpritePriority(FIELD_TREASURE_ACTOR, 3);
        }
        else
        {
            CLOSED_BLEND();
        }
    }
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST)
    {
        fdEffectStartBlankOverlay(0);
        fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
            ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
        if (fdScriptGetFlag(FLAG_NORTH_PILLAR_PUSHED))
        {
            fdMapGridCopyMetatileAttributes(46, 74, 1, 1, 46, 73);
            fdActorGetById(ACTOR_NORTH_PILLAR)->x.fixed = toFIXED(760);
        }
        if (fdScriptGetFlag(FLAG_EAST_PILLAR_PUSHED))
            fdActorSetPos(ACTOR_EAST_PILLAR, 920, 616);
        fdActorPlaceBlocks(MagmaRockInsideSalamanderTailChestPillars);
        scMagmaRockInsideStartLavaPlatform(ACTOR_NORTH_PLATFORM,
                                           MagmaRockInsideSalamanderTailNorthPlatformPath,
                                           FLAG_LOWER_ROOMS_FLOODED);
        scMagmaRockInsideStartLavaPlatform(ACTOR_SOUTHWEST_PLATFORM,
                                           MagmaRockInsideSalamanderTailSouthwestPlatformPath,
                                           FLAG_LOWER_ROOMS_FLOODED);
        scMagmaRockInsideStartLavaPlatform(ACTOR_SOUTHEAST_PLATFORM,
                                           MagmaRockInsideSalamanderTailSoutheastPlatformPath,
                                           FLAG_LOWER_ROOMS_FLOODED);
        if (fdScriptGetFlag(FLAG_LOWER_ROOMS_FLOODED))
        {
            fdActorSetAnimation(ACTOR_NORTH_PILLAR, 4);
            if (fdScriptGetFlag(FLAG_EAST_PILLAR_PUSHED))
                fdActorSetAnimation(ACTOR_EAST_PILLAR, 4);
            if (fdScriptGetFlag(FLAG_NORTH_PILLAR_PUSHED))
                scMapMetatileSetAttributes(
                    MAPGRID_LAYER_OVERHEAD, 46, 8, METATILE_BEHAVIOR_WALL << 8);
            scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 10, 45, 0);
            scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 53, 40, 0);
        }
        else
        {
            CLOSED_BLEND();
            fdMapGridCopyMetatilesTo(71, 64, 50, 102, 1, 1);
        }
    }
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
    {
        for (i = 0; i != 3; i++)
        {
            ACTOR *actor = fdActorGetById(i + ACTOR_SUNKEN_PILLAR_A);

            fdActorSelectAnimation(actor, 4);
            actor->motionFlags = 0;
            actor->y.fixed = actor->groundHeight = -toFIXED(PILLAR_SINK_DEPTH);
        }
        if (fdScriptGetFlag(FLAG_LEDGE_PILLAR_PUSHED))
        {
            fdActorSetAnimation(ACTOR_LEDGE_PILLAR, 4);
            fdActorSetPos(ACTOR_LEDGE_PILLAR, 696, 648);
        }
        fdActorPlaceBlocks(MagmaRockInsideEmberStonePillars);
        fdEffectStartBlankOverlay(0);
        if (!fdScriptGetFlag(FLAG_WEST_TORCH_LIT))
            fdActorSetPos(ACTOR_WEST_TORCH_FLAME, 0, 0);
        if (!fdScriptGetFlag(FLAG_EAST_TORCH_LIT))
            fdActorSetPos(ACTOR_EAST_TORCH_FLAME, 0, 0);
        if (fdScriptGetFlag(FLAG_WEST_TORCH_LIT) && fdScriptGetFlag(FLAG_EAST_TORCH_LIT))
        {
            Lower(ACTOR_BLOCKING_PILLAR, -toFIXED(PILLAR_SINK_DEPTH));
            fdActorSetAnimation(ACTOR_BLOCKING_PILLAR, 4);
        }
        else
        {
            scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 38, 18, METATILE_BEHAVIOR_WALL << 8);
        }
        if (fdScriptGetFlag(FLAG_MAGMA_BALL_TAKEN))
            fdActorSetAnimation(ACTOR_EMBER_STONE, 0);
        for (i = 0; i != 4; i++)
            fdActorSetAnimation(i + ACTOR_FALLEN_ROCK_A, 2);
        for (i = 0; i != ACTOR_LEDGE_PILLAR - ACTOR_EMBER_STONE; i++)
            fdActorSetSpriteFlags(fdActorGetById(i + ACTOR_EMBER_STONE), 0);
        if (!fdScriptGetFlag(FLAG_ROCKS_RELEASED))
        {
            for (i = 0; i != 9; i++)
                fdActorSetPos(i + ACTOR_EMBER_STONE, 0, 0);
        }
    }
    else if (MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
    {
        scMagmaRockInsideStartStepMetatiles();
        fdScriptSetFlag(FLAG_RETREAT_BLOCKED);
        for (i = 0; i != 6; i++)
        {
            if (fdScriptGetFlag(FLAG_MARKER_LIT(i + 2)))
            {
                Sint32 y = 24 - i * 2;

                fdMapGridCopyMetatilesTo(65, 1, 80, y, 1, 2);
                scMapMetatileSetAttributes(
                    MAPGRID_LAYER_GROUND, 16, y, METATILE_BEHAVIOR_WALL << 8);
            }
        }
        fdActorSetSpriteFlags(fdActorGetById(ACTOR_SOURCE_FLAME), 0);
        fdActorSetSpriteFlags(fdActorGetById(ACTOR_TORCH_FLAME), 0);
        if (fdScriptGetFlag(FLAG_TABLET_TORCH_LIT))
        {
            Lower(ACTOR_TORCH_PILLAR, -toFIXED(PILLAR_SINK_DEPTH));
            fdActorSetAnimation(ACTOR_TORCH_PILLAR, 4);
        }
        else
        {
            scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 7, 21, METATILE_BEHAVIOR_WALL << 8);
            fdActorSetPos(ACTOR_TORCH_FLAME, 0, 0);
        }
        Lower(ACTOR_BLAZE_TABLET, toFIXED(16));
        fdActorSetMovementType(fdActorGetById(ACTOR_BLAZE_TABLET), MOVEMENT_TYPE_TAKE_LEADER_LAYER);
    }
    return 0;
}

Sint32 scPrepareMap(void)
{
    Sint32 scene    = MainRoutineStateWork.next;
    Sint32 entrance = MainRoutineStateWork.entrance;

    if (!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
    {
        if ((scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE
             && entrance == MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_STATUE)
            || (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE
                && entrance == MAGMA_ROCK_INSIDE_EMBER_STONE_FROM_MAGMA_ROCK_SLIDE))
        {
            fdScriptClearFlag(FLAG_ENTRANCE_FLOODED);
            fdScriptClearFlag(FLAG_FURY_FLOODED);
            fdScriptClearFlag(FLAG_LOWER_ROOMS_FLOODED);
        }
    }
    return 0;
}

// On entering, sets each listed pillar from its two flags, then lifts the leader onto the ground
// under it.
#include "shared/pillar_restore_raised.h"

#include "shared/leader_check_crumbling_step.h"

// Unused: a crumbling pillar named in the (actor, flag) table gives way with a shake, rubble and a
// thump, as on Magma Rock's outside.
UNUSED void scMagmaRockInsideCrumblePillar(const void *data, Sint32 id)
{
    const TRAP_ENTRY *entry = data;
    ACTOR            *actor = fdActorGet(id);
    Sint32            flag;

    while (entry->actor != ACTOR_LIST_END)
    {
        if (entry->actor != id)
        {
            entry++;
            continue;
        }
        flag = entry->flags;
        fdScriptStart();
        if (!fdScriptGetFlag(flag + 1))
        {
            fdScriptPlaySound(SE_PUSH);
            fdMapGridSetShake(1, 4, 1);
            scDustScatter(0);
            syWait(8);
            fdActorSelectAnimation(actor, 2);
            fdActorSetMetatileShape(-4, actor->layer, id);
            scDustScatter(1);
            syWait(16);
            fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
            syWait(8);
            scFieldActorSetPosAndResetMotion(actor, 0, 0, 0);
            syWait(2);
            fdScriptPlaySound(SE_THUMP);
            scMagmaRockInsideSpawnDustRing();
            syWait(20);
            fdScriptSetFlag(flag);
            fdScriptSetFlag(flag + 1);
        }
        fdScriptEnd();
        return;
    }
}

#include "shared/pillar_take_out_of_play.c"

#include "shared/pillar_restore.h"

#include "shared/actor_take_place_of.h"

// Unused: stepping onto a crumbling floor, as on Magma Rock's outside: the first time it shakes and
// cracks, the second time it gives way with the leader.
UNUSED void scMagmaRockInsideStepOnTrap(const Uint16 *table)
{
    ACTOR *leader;
    ACTOR *partner;
    ACTOR *trap;
    FIXED  pos[3];
    Sint32 id;
    Sint32 flag;
    Sint32 second;

    if (fdTryJumpForward() != 0)
        return;
    leader  = fdActorGetById(MainRoutineStateWork.leaderActorId);
    partner = fdActorGetById(ACTOR_FIRST_PLACED);
    pos[0]  = leader->x.fixed;
    pos[1]  = leader->y.fixed;
    pos[2]  = leader->z.fixed;
    trap    = fdActorFindAtMetatile(pos, 0);
    if (trap == NULL)
        return;
    while (*table != ACTOR_LIST_END)
    {
        id = table[0];
        if (fdActorGetById(id) != trap)
        {
            table += 2;
            continue;
        }
        {
            flag = table[1];
            fdScriptStart();
            if (!fdScriptGetFlag(flag))
            {
                fdActorSelectAnimation(partner, 1);
                scActorTakePlaceOf(partner, trap);
                sySynch(1);
                fdScriptPlaySound(SE_CHEST_CLOSE);
                scFieldActorSetSpriteColor(fdActorGetById(ACTOR_FIRST_PLACED), SPRITE_COLOR_WHITE);
                sySynch(2);
                fdActorSelectAnimation(partner, 0);
                fdActorCopyGraphics(id, ACTOR_FIRST_PLACED + 1);
                scFieldActorSetSpriteColor(fdActorGetById(ACTOR_FIRST_PLACED), SPRITE_COLOR_NORMAL);
                sySynch(2);
                scFieldActorSetSpriteColor(fdActorGetById(ACTOR_FIRST_PLACED), SPRITE_COLOR_WHITE);
                sySynch(4);
                scFieldActorSetSpriteColor(fdActorGetById(ACTOR_FIRST_PLACED), SPRITE_COLOR_NORMAL);
                scDustScatter(0);
                scActorTakePlaceOf(trap, partner);
                sySynch(1);
                fdScriptSetFlag(flag);
            }
            else
            {
                second = flag + 1;
                if (!fdScriptGetFlag(second))
                {
                    fdActorSelectAnimation(partner, 0);
                    scActorTakePlaceOf(partner, trap);
                    sySynch(1);
                    leader->motionFlags = 0;
                    fdScriptPlaySound(SE_PUSH);
                    fdMapGridSetShake(1, 4, 1);
                    scDustScatter(0);
                    sySynch(8);
                    fdActorSelectAnimation(partner, 2);
                    fdLeaderSetMetatileShapeAt(-2, trap->layer);
                    scDustScatter(1);
                    sySynch(16);
                    fdLeaderSetMetatileShapeAt(-2, trap->layer);
                    sySynch(4);
                    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
                    sySynch(8);
                    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
                    sySynch(5);
                    fdActorSetPosAndResetMotion(partner, 0, 0, 0);
                    sySynch(2);
                    fdScriptPlaySound(SE_THUMP);
                    scMagmaRockInsideSpawnDustRing();
                    sySynch(20);
                    fdScriptSetFlag(second);
                }
            }
            fdScriptEnd();
            return;
        }
    }
}

#include "../field/field_effect_advance_motion_priority.c"

#undef CLOSED_BLEND
// The falling rocks and rubble, then the rooms' tables.

#include "shared/dust_scatter.c"

// Throws a ring of 16 dust clouds out from the leader as a pillar lands.
void scMagmaRockInsideSpawnDustRing(void)
{
    ACTOR        *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
    FIELD_EFFECT *effect;
    Uint32        frame;
    Uint32        i;

    if (leader == NULL)
        return;
    frame = 0;
    for (i = 0; i <= DUST_RING_PUFFS; i++)
    {
        effect = (FIELD_EFFECT *)scFieldActorCreate(
            SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed, leader->z.fixed);
        if (effect != NULL)
        {
            frame                  = fdSpriteShareVRAMCacheEntry(effect->sprite, frame);
            effect->motionFlags    = 0;
            effect->collisionFlags = 0;
            fdActorSetSpriteFlags(effect, 0);
            fdActorSelectAnimation(effect, 2);
            fdActorStartScript(effect, DustBurstScript);
            effect->velocity.x        = syCos(i * FACING_STEP);
            effect->velocity.y        = 0;
            effect->velocity.z        = syMathSin(i * FACING_STEP);
            effect->velocity.x       += effect->velocity.x >> 1;
            effect->velocity.x       += syRandomBelow(toFIXED(3) / 4) - toFIXED(3) / 8;
            effect->velocity.z       += syRandomBelow(toFIXED(3) / 8) - toFIXED(3) / 16;
            effect->spin              = syRandomBelow(4096) - TRIG_TURN / 32;
            effect->scaleRateX        = 0;
            effect->scaleRateY        = 0;
            effect->update            = fdEffectAdvanceMotionAtLeaderPriority;
            effect->sprite->priority  = ((FIELD_SPR *)leader->sprite)->priority;
        }
    }
}

#include "shared/actor_set_metatile_shape.h"

#include "shared/leader_set_metatile_shape.h"

#include "shared/particle_update_falling.h"

// Spawns count rocks around (x, y, z), each pushed out at a random angle and thrown up and out
// along it.
void scMagmaRockInsideDropRocks(Sint32 x, Sint32 y, Sint32 z, Sint32 count)
{
    ROCK_STATE *state;
    Sint32      i;

    state = RamHeapSlots->menuState;
    for (i = 0; i < count; i++)
    {
        Sint32 angle = syRandom();
        ROCK  *rock  = &state->rocks[state->next];

        rock->timer  = 0;
        rock->pos[0] = x;
        rock->pos[1] = y;
        rock->pos[2] = z;
        syFieldVectorAddPolarOffsetFX((syRandom() << 3) + toFIXED(8), angle, rock->pos);
        rock->velocity[0] = 0;
        rock->velocity[1] = toFIXED(5);
        rock->velocity[2] = 0;
        syFieldVectorAddPolarOffsetFX((syRandom() << 1) + toFIXED(2), angle, rock->velocity);
        state->next++;
        state->next &= ROCK_COUNT - 1;
    }
}

// Starts the falling rocks: loads their picture, prepares 64 small sprites and adds the event that
// draws them.
void scMagmaRockInsideStartFallingRocks(void)
{
    ROCK_STATE *state = syHeapSlotAllocate(HEAP_SLOT_MENU, sizeof(ROCK_STATE));
    Sint32      entry;
    Sint32      tile;
    Sint32      i;
    ROCK       *rock;
    EFFECT_SPR *sprite;

    scMagmaRockInsideDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_FALLING_ROCK), state);
    entry           = syVRAMCacheFindFreeEntry();
    tile            = mnVRAMLoad(entry, sizeof(state->tiles), state);
    state->tileBase = tile;
    state->resource = entry;
    rock            = state->rocks;
    for (i = 0; i < ROCK_COUNT; i++)
    {
        sprite = &state->sprites[i];
        scMagmaRockInsideInitRockSprite(sprite, 8, 8, OBJ_SIZE_16X16, tile);
        sprite->oam.fullColor = 1;
        sprite->oam.palette   = SPRITE_COLOR_NORMAL;
        sprite->oam.priority  = 0;
        sprite->depth         = OAM_DEPTH_WINDOW;
        rock->timer           = -1;
        rock++;
    }
    state->next = 0;
    syFieldSetTickEvent(scParticleUpdateFalling, EVENT_PASS_TICK);
}

#include "shared/rock_end_falling.h"

extern Uint32 FieldEffectLongScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/push_leader_steps.c"

#include "shared/magma_rock_ember_scripts.c"

// The Lucky Medal chest room's lava platform: round a rectangle, for as long as the lava flows.
Sint32 MagmaRockInsideLuckyMedalPlatformPath[] = {
    SC_MOVE(776, 16, 680), SC_SYNC,
    SC_MOVE(744, 16, 680), SC_SYNC,
    SC_MOVE(744, 16, 632), SC_SYNC,
    SC_MOVE(776, 16, 632), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// Fury's room's west lava platform: round a rectangle.
Sint32 MagmaRockInsideFuryWestPlatformPath[] = {
    SC_MOVE(216, 16, 568), SC_SYNC,
    SC_MOVE(216, 16, 584), SC_SYNC,
    SC_MOVE(136, 16, 584), SC_SYNC,
    SC_MOVE(136, 16, 568), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// Fury's room's east lava platform: east to 760 once, and it stays there.
Sint32 MagmaRockInsideFuryEastPlatformPath[] = {
    SC_MOVE(600, 16, 664), SC_SYNC,
    SC_MOVE(728, 16, 664), SC_SYNC,
    SC_MOVE(728, 16, 632), SC_SYNC,
    SC_MOVE(760, 16, 632), SC_SYNC,
    SC_MOVE(760, 16, 664), SC_SYNC,
    SC_END,
};

// The west lava platform of the cracked pillars' room's upper hall, which floods with Fury's room: round a rectangle.
Sint32 MagmaRockInsideCrackedPillarsWestPlatformPath[] = {
    SC_MOVE(184, 16, 232), SC_SYNC,
    SC_MOVE(280, 16, 232), SC_SYNC,
    SC_MOVE(280, 16, 264), SC_SYNC,
    SC_MOVE(184, 16, 264), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// The east lava platform of the cracked pillars' room's upper hall, which floods with Fury's room: round a rectangle.
Sint32 MagmaRockInsideCrackedPillarsEastPlatformPath[] = {
    SC_MOVE(472, 16, 296), SC_SYNC,
    SC_MOVE(424, 16, 296), SC_SYNC,
    SC_MOVE(424, 16, 264), SC_SYNC,
    SC_MOVE(472, 16, 264), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// The cracked pillars' room's lava platforms A and B: one square, B starting at its third corner (scMagmaRockInsideSetLowerRoomsFlooded).
Sint32 MagmaRockInsideCrackedPillarsFirstPairPath[] = {
    SC_MOVE(216, 16, 600), SC_SYNC,
    SC_MOVE(280, 16, 600), SC_SYNC,
    SC_MOVE(280, 16, 664), SC_SYNC,
    SC_MOVE(216, 16, 664), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// The cracked pillars' room's lava platforms C and D, as A and B.
Sint32 MagmaRockInsideCrackedPillarsSecondPairPath[] = {
    SC_MOVE(312, 16, 664), SC_SYNC,
    SC_MOVE(376, 16, 664), SC_SYNC,
    SC_MOVE(376, 16, 600), SC_SYNC,
    SC_MOVE(312, 16, 600), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// The cracked pillars' room's lava platforms E and F, as A and B.
Sint32 MagmaRockInsideCrackedPillarsThirdPairPath[] = {
    SC_MOVE(440, 16, 600), SC_SYNC,
    SC_MOVE(504, 16, 600), SC_SYNC,
    SC_MOVE(504, 16, 664), SC_SYNC,
    SC_MOVE(440, 16, 664), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// The cracked pillars' room's lava platforms G and H, as A and B.
Sint32 MagmaRockInsideCrackedPillarsFourthPairPath[] = {
    SC_MOVE(536, 16, 664), SC_SYNC,
    SC_MOVE(600, 16, 664), SC_SYNC,
    SC_MOVE(600, 16, 600), SC_SYNC,
    SC_MOVE(536, 16, 600), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// The Salamander Tail's room's north lava platform: round a rectangle.
Sint32 MagmaRockInsideSalamanderTailNorthPlatformPath[] = {
    SC_MOVE(376, 16, 264), SC_SYNC,
    SC_MOVE(376, 16, 232), SC_SYNC,
    SC_MOVE(408, 16, 232), SC_SYNC,
    SC_MOVE(408, 16, 264), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// The Salamander Tail's room's southwest lava platform: round a rectangle.
Sint32 MagmaRockInsideSalamanderTailSouthwestPlatformPath[] = {
    SC_MOVE(216, 16, 680), SC_SYNC,
    SC_MOVE(280, 16, 680), SC_SYNC,
    SC_MOVE(280, 16, 712), SC_SYNC,
    SC_MOVE(216, 16, 712), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// The Salamander Tail's room's southeast lava platform: round a rectangle.
Sint32 MagmaRockInsideSalamanderTailSoutheastPlatformPath[] = {
    SC_MOVE(376, 16, 680), SC_SYNC,
    SC_MOVE(312, 16, 680), SC_SYNC,
    SC_MOVE(312, 16, 712), SC_SYNC,
    SC_MOVE(376, 16, 712), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// The pillars Move pushes in the entrance, ended by -1.
Sint16 MagmaRockInsideEntrancePillars[] = {
    ACTOR_ENTRANCE_PILLAR, -1,
};

// The statue Burst wakes in the entrance hall and in Fury's room, as (actor, the metatile Burst is cast at, unused) rows ended by -1.
ALIGNED(4) Sint16 MagmaRockInsideStatues[] = {
    ACTOR_STATUE, METATILE_STATUE, -1, -1,
};

// The pillars Move pushes in Fury's room, ended by -1.
Sint16 MagmaRockInsideFuryPillars[] = {
    ACTOR_FURY_PILLAR, -1,
};

// The cracked pillars' room's three statues, as (actor, the metatile Burst is cast at, unused) rows ended by -1 (scStatueFindUnderBurst).
Sint16 MagmaRockInsideCrackedPillarsStatues[] = {
    ACTOR_STATUE, METATILE_STATUE, -1,
    ACTOR_LOWER_WEST_STATUE, METATILE_LOWER_WEST_STATUE, -1,
    ACTOR_LOWER_EAST_STATUE, METATILE_LOWER_EAST_STATUE, -1,
    -1,
};

// The pillars Move pushes in the cracked pillars' room, ended by -1.
Sint16 MagmaRockInsideCrackedPillarsPillars[] = {
    ACTOR_PILLAR_A, ACTOR_PILLAR_B, ACTOR_PILLAR_C, -1,
};

// The pillars Move pushes in the Salamander Tail's chest room, ended by -1.
Sint16 MagmaRockInsideSalamanderTailChestPillars[] = {
    ACTOR_NORTH_PILLAR, ACTOR_EAST_PILLAR, -1,
};

// The pillars Move pushes in the ember stone's room, ended by -1.
Sint16 MagmaRockInsideEmberStonePillars[] = {
    ACTOR_LEDGE_PILLAR, -1, 0,
};

// A small spark's actor script (scMagmaRockInsideSpawnSpark).
Sint32 MagmaRockInsideFlameScript[] = {
    SC_WAIT(6),
    SC_RELEASE,
    SC_END,
};

// A spark shooting up from the Blaze tablet as it lifts: its actor script.
Sint32 MagmaRockInsideRisingSparkScript[] = {
    SC_MOVE_BY(0, 160, 0),
    SC_SYNC,
    SC_RELEASE,
    SC_END,
};

// A circling spark's actor script (scMagmaRockInsideOrbitParent).
Sint32 MagmaRockInsideCirclingSparkScript[] = {
    SC_CALL(scMagmaRockInsideOrbitParent),
    SC_RELEASE,
    SC_END,
};

Sint32 scMagmaRockInsideDriftForSteps(ORBIT *object);

// A rising spark's actor script: it circles and drifts for its steps

// (scMagmaRockInsideDriftForSteps).
Sint32 MagmaRockInsideOrbitScript[] = {
    SC_CALL(syOrbitUpdate),
    SC_CALL(scActorAimAtWatched),
    SC_CALL(scMagmaRockInsideDriftForSteps),
    SC_RELEASE,
    SC_END,
};

#include "shared/lesson_scripts.c"

#include "shared/dust_scatter_script.c"

#include "shared/dust_burst_script.c"

const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
    SCENE_EXITS(MagmaRockInsideLuckyMedalChest),
    SCENE_EXIT(1, MagmaRockInsideLuckyMedalChest, 4),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_A, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_B, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_B),
    SCENE_EXIT(4, MagmaRockInsideLuckyMedalChest, 1),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_A, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_B, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_C, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_C),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_FURY, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST),
    SCENE_EXITS(MagmaRockInsideEntrance),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_A, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_A, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_B, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_C, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_C),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_B, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_B),
    SCENE_EXIT(6, MagmaRockInsideEntrance, 7),
    SCENE_EXIT(7, MagmaRockInsideEntrance, 6),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_STATUE, MagmaRockStatue, MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_INSIDE_ENTRANCE),
    SCENE_EXITS(MagmaRockInsideFury),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_FURY),
    SCENE_EXIT(2, MagmaRockInsideFury, 6),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHWEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_NORTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_A, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_B, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_B),
    SCENE_EXIT(6, MagmaRockInsideFury, 2),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_WEST),
    SCENE_EXIT(8, MagmaRockInsideFury, 9),
    SCENE_EXIT(9, MagmaRockInsideFury, 8),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_SOUTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTH, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_SOUTH),
    SCENE_EXITS(MagmaRockInsideCrackedPillars),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHWEST, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_A, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_B, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_WEST, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTHWEST, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTH, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTH),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTH, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTH),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_A, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_B, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_WEST, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_WEST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTHWEST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_A, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_B, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B),
    SCENE_EXITS(MagmaRockInsideGolemCoreChest),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTHWEST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_NORTHWEST),
    SCENE_EXIT(3, MagmaRockInsideGolemCoreChest, 9),
    SCENE_EXIT(4, MagmaRockInsideGolemCoreChest, 10),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTH, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_SOUTH),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_B),
    SCENE_EXIT(9, MagmaRockInsideGolemCoreChest, 3),
    SCENE_EXIT(10, MagmaRockInsideGolemCoreChest, 4),
    SCENE_EXITS(MagmaRockInsideSalamanderTailChest),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_NORTHWEST, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTH, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTH),
    SCENE_EXIT(8, MagmaRockInsideSalamanderTailChest, 14),
    SCENE_EXIT(9, MagmaRockInsideSalamanderTailChest, 16),
    SCENE_EXIT(10, MagmaRockInsideSalamanderTailChest, 17),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_SOUTH, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTH),
    SCENE_EXIT(12, MagmaRockInsideSalamanderTailChest, 18),
    SCENE_EXIT(13, MagmaRockInsideSalamanderTailChest, 19),
    SCENE_EXIT(14, MagmaRockInsideSalamanderTailChest, 8),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_EMBER_STONE, MagmaRockInsideEmberStone, MAGMA_ROCK_INSIDE_EMBER_STONE_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST),
    SCENE_EXIT(16, MagmaRockInsideSalamanderTailChest, 9),
    SCENE_EXIT(17, MagmaRockInsideSalamanderTailChest, 10),
    SCENE_EXIT(18, MagmaRockInsideSalamanderTailChest, 12),
    SCENE_EXIT(19, MagmaRockInsideSalamanderTailChest, 13),
    SCENE_EXITS(MagmaRockInsideEmberStone),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_BLAZE_TABLET, MagmaRockInsideBlazeTablet, MAGMA_ROCK_INSIDE_BLAZE_TABLET_FROM_MAGMA_ROCK_INSIDE_EMBER_STONE),
    SCENE_EXIT(2, MagmaRockInsideEmberStone, 5),
    SCENE_EXIT(3, MagmaRockInsideEmberStone, 6),
    SCENE_EXIT(4, MagmaRockInsideEmberStone, 7),
    SCENE_EXIT(5, MagmaRockInsideEmberStone, 2),
    SCENE_EXIT(6, MagmaRockInsideEmberStone, 3),
    SCENE_EXIT(7, MagmaRockInsideEmberStone, 4),
    SCENE_EXIT(8, MagmaRockInsideEmberStone, 9),
    SCENE_EXIT(9, MagmaRockInsideEmberStone, 8),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_EMBER_STONE),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_SLIDE, MagmaRockSlide, MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_INSIDE_EMBER_STONE),
    SCENE_EXITS(MagmaRockInsideBlazeTablet),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_BLAZE_TABLET_TO_MAGMA_ROCK_INSIDE_EMBER_STONE, MagmaRockInsideEmberStone, MAGMA_ROCK_INSIDE_EMBER_STONE_FROM_MAGMA_ROCK_INSIDE_BLAZE_TABLET),
    SCENE_EXITS_END,
};

// The actors placed in the Lucky Medal's room: its one white platform.
ACTOR_PLACE MagmaRockInsidePlacements[] = {
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

// The actors placed in the entrance.
ACTOR_PLACE MagmaRockInsideEntrancePlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 6, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 7, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 9, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FIRE19), MOVEMENT_TYPE_STAND, 296, 0, 504, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

// The actors placed in Fury's room.
ACTOR_PLACE MagmaRockInsideFuryPlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FURY), MOVEMENT_TYPE_STAND, 456, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

// The actors placed in the cracked pillars' room.
ACTOR_PLACE MagmaRockInsideCrackedPillarsPlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, FLAG_INSIDE_CRACKED_PILLAR_A_BURST, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, FLAG_INSIDE_CRACKED_PILLAR_B_BURST, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 6, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 7, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 9, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 10, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 11, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 12, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 13, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 14, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 15, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 16, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 17, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 18, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 19, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

// The actors placed in the Golem Core's chest room.
ACTOR_PLACE MagmaRockInsideGolemCoreChestPlacements[] = {
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The actors placed in the Salamander Tail's chest room.
ACTOR_PLACE MagmaRockInsideSalamanderTailChestPlacements[] = {
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

// The actors placed in the ember stone's room.
ACTOR_PLACE MagmaRockInsideEmberStonePlacements[] = {
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 12, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 13, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 14, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 15, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 16, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

// The actors placed in the Blaze tablet's room.
ACTOR_PLACE MagmaRockInsideBlazeTabletPlacements[] = {
    {SPRITE_SLATE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// The cues of the Lucky Medal's room.
CUE           MagmaRockInsideCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, 1},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_A, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_A},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_B, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_B},
    {CUE_EXIT, 4, CONDITION_ALWAYS, 4},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_A, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_A},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_B, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_B},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_C, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_C},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
     MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_FURY, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_FURY},
    {STEP_CUE_WALKING,
     METATILE_LAVA_EDGE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideLeapOverLava)},
    {STEP_CUE_WALKING,
     METATILE_LAVA_EDGE_EAST, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideLeapOverLava)},
    {CUE_TABLE_END},
};

// The cues of the entrance.
CUE MagmaRockInsideEntranceCues[] = {
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_A, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_A},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_A, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_A},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_B, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_B},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_C, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_C},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_B, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_B},
    {CUE_EXIT, 6, CONDITION_ALWAYS, 6},
    {CUE_EXIT, 7, CONDITION_ALWAYS, 7},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_STATUE, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_STATUE},
    {CUE_STEP, METATILE_FLOOR_SWITCH, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideDrainLava)},
    {TALK_CUE_FACING(FACING_NORTH),
     ACTOR_FIRE19, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideMeetDjinni)},
    {CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_ENTRANCE_PILLAR, FLAG_ENTRANCE_PILLAR_PUSHED,
     SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_ENTRANCE_PILLAR, FLAG_ENTRANCE_PILLAR_PUSHED,
     SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     METATILE_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     METATILE_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideTargetStatue)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     METATILE_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideFireStatue)},
    {CUE_TABLE_END},
};

// The cues of Fury's room.
CUE MagmaRockInsideFuryCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST},
    {CUE_EXIT, 2, CONDITION_ALWAYS, 2},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHWEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHWEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_A, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_A},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_B, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_B},
    {CUE_EXIT, 6, CONDITION_ALWAYS, 6},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST},
    {CUE_EXIT, 8, CONDITION_ALWAYS, 8},
    {CUE_EXIT, 9, CONDITION_ALWAYS, 9},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTH, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTH},
    {CUE_STEP, METATILE_FLOOR_SWITCH, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideDrainLava)},
    {STEP_CUE_WALKING,
     METATILE_LAVA_EDGE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideLeapOverLava)},
    {CUE_TALK, ACTOR_FURY, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideFightFury)},
    {CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FURY_PILLAR, FLAG_FURY_PILLAR_PUSHED,
     SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_FURY_PILLAR, FLAG_FURY_PILLAR_PUSHED,
     SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     METATILE_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     METATILE_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideTargetStatue)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     METATILE_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideFireStatue)},
    {CUE_TABLE_END},
};

// The cues of the cracked pillars' room.
CUE MagmaRockInsideCrackedPillarsCues[] = {
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHWEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHWEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_A, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_A},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_B, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_B},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_WEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_WEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTHWEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTHWEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTH, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTH},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTH, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTH},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_A, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_A},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_B, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_B},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_WEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_WEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_WEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_WEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTHWEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTHWEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_A, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_A},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_B, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_B},
    {STEP_CUE_WALKING,
     METATILE_LAVA_EDGE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideLeapOverLava)},
    {STEP_CUE_WALKING,
     METATILE_CRACKED_PILLAR_A, IF_FLAG_SET(FLAG_FURY_FLOODED),
     SCRIPT_REF(scMagmaRockInsideLeapOverLava)},
    {STEP_CUE_WALKING,
     METATILE_CRACKED_PILLAR_B, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideLeapOverLava)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_LAVA_EDGE_NORTH, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideLeapOverLava)},
    {STEP_CUE_WALKING_INTO(FACING_WEST),
     METATILE_LAVA_EDGE_WEST, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideLeapOverLava)},
    {CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PILLAR_A, FLAG_PILLAR_A_PUSHED,
     SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_PILLAR_A, FLAG_PILLAR_A_PUSHED,
     SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PILLAR_B, FLAG_PILLAR_B_PUSHED,
     SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_PILLAR_B, FLAG_PILLAR_B_PUSHED,
     SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PILLAR_C, FLAG_PILLAR_C_PUSHED,
     SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_PILLAR_C, FLAG_PILLAR_C_PUSHED,
     SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     METATILE_CRACKED_PILLAR_A, FLAG_INSIDE_CRACKED_PILLAR_A_BURST,
     SCRIPT_REF(MagmaRockInsideBurstCrackedPillarA)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     METATILE_CRACKED_PILLAR_B, FLAG_INSIDE_CRACKED_PILLAR_B_BURST,
     SCRIPT_REF(MagmaRockInsideBurstCrackedPillarB)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     METATILE_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     METATILE_STATUE, FLAG_FIRE_BURST_PLAYING,
     SCRIPT_REF(scMagmaRockInsideTargetStatue)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     METATILE_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(MagmaRockInsideBurstCrackedPillarA)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     METATILE_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideFireStatue)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     METATILE_LOWER_WEST_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     METATILE_LOWER_WEST_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideTargetStatue)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     METATILE_LOWER_WEST_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideFireStatue)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     METATILE_LOWER_EAST_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     METATILE_LOWER_EAST_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideTargetStatue)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     METATILE_LOWER_EAST_STATUE, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideFireStatue)},
    {CUE_TABLE_END},
};

// The cues of the Golem Core's chest room.
CUE MagmaRockInsideGolemCoreChestCues[] = {
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTHWEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTHWEST},
    {CUE_EXIT, 3, CONDITION_ALWAYS, 3},
    {CUE_EXIT, 4, CONDITION_ALWAYS, 4},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTH, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTH},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B},
    {CUE_EXIT, 9, CONDITION_ALWAYS, 9},
    {CUE_EXIT, 10, CONDITION_ALWAYS, 10},
    {CUE_STEP, METATILE_FLOOR_SWITCH, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideDrainLava)},
    {CUE_STEP,
     METATILE_SECOND_FLOOR_SWITCH, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideDrainLava)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HANGING_CHAIN_A, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HANGING_CHAIN_B, CONDITION_ALWAYS, 0},
    {CUE_TABLE_END},
};

// The cues of the Salamander Tail's chest room.
CUE MagmaRockInsideSalamanderTailChestCues[] = {
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_NORTHWEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_NORTHWEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTH, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTH},
    {CUE_EXIT, 8, CONDITION_ALWAYS, 8},
    {CUE_EXIT, 9, CONDITION_ALWAYS, 9},
    {CUE_EXIT, 10, CONDITION_ALWAYS, 10},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_SOUTH, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_SOUTH},
    {CUE_EXIT, 12, CONDITION_ALWAYS, 12},
    {CUE_EXIT, 13, CONDITION_ALWAYS, 13},
    {CUE_EXIT, 14, CONDITION_ALWAYS, 14},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_EMBER_STONE, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_EMBER_STONE},
    {CUE_EXIT, 16, CONDITION_ALWAYS, 16},
    {CUE_EXIT, 17, CONDITION_ALWAYS, 17},
    {CUE_EXIT, 18, CONDITION_ALWAYS, 18},
    {CUE_EXIT, 19, CONDITION_ALWAYS, 19},
    {STEP_CUE_WALKING,
     METATILE_LAVA_EDGE, IF_FLAG_SET(FLAG_LOWER_ROOMS_FLOODED),
     SCRIPT_REF(scMagmaRockInsideLeapOverLava)},
    {CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_NORTH_PILLAR, FLAG_NORTH_PILLAR_PUSHED,
     SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_NORTH_PILLAR, FLAG_NORTH_PILLAR_PUSHED,
     SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_EAST_PILLAR, FLAG_EAST_PILLAR_PUSHED,
     SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_EAST_PILLAR, FLAG_EAST_PILLAR_PUSHED,
     SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {CUE_TABLE_END},
};

// The cues of the ember stone's room.
CUE MagmaRockInsideEmberStoneCues[] = {
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_BLAZE_TABLET, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideOpenStairs)},
    {CUE_EXIT, 2, CONDITION_ALWAYS, 2},
    {CUE_EXIT, 3, CONDITION_ALWAYS, 3},
    {CUE_EXIT, 4, CONDITION_ALWAYS, 4},
    {CUE_EXIT, 5, CONDITION_ALWAYS, 5},
    {CUE_EXIT, 6, CONDITION_ALWAYS, 6},
    {CUE_EXIT, 7, CONDITION_ALWAYS, 7},
    {CUE_EXIT, 8, CONDITION_ALWAYS, 8},
    {CUE_EXIT, 9, CONDITION_ALWAYS, 9},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP,
     MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST},
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_SLIDE, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_SLIDE},
    {CUE_STEP,
     METATILE_ERUPTION_ISLET, FLAG_ROCKS_RELEASED,
     SCRIPT_REF(scMagmaRockInsideReleaseRocks)},
    {CUE_EXAMINE,
     METATILE_EMBER_STONE, FLAG_MAGMA_BALL_TAKEN,
     SCRIPT_REF(scMagmaRockInsideExamineEmberStone)},
    {CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_LEDGE_PILLAR, FLAG_LEDGE_PILLAR_PUSHED,
     SCRIPT_REF(scMagmaRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_LEDGE_PILLAR, FLAG_LEDGE_PILLAR_PUSHED,
     SCRIPT_REF(scMagmaRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FALLEN_FLAME_A, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideTakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FALLEN_FLAME_B, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideTakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FALLEN_FLAME_C, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideTakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FALLEN_FLAME_D, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideTakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_WEST_TORCH_FLAME, CONDITION_ALWAYS,
     0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_EAST_TORCH_FLAME, CONDITION_ALWAYS,
     SCRIPT_REF(MagmaRockInsideLightTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_EAST_TORCH_FLAME, FLAG_EAST_TORCH_LIT,
     SCRIPT_REF(scMagmaRockInsideGrowFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_WEST_TORCH_FLAME, FLAG_WEST_TORCH_LIT,
     SCRIPT_REF(scMagmaRockInsideGrowFlame)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE),
     ACTOR_FALLEN_FLAME_A, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsidePutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE),
     ACTOR_FALLEN_FLAME_B, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsidePutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE),
     ACTOR_FALLEN_FLAME_C, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsidePutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE),
     ACTOR_FALLEN_FLAME_D, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsidePutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE),
     ACTOR_EAST_TORCH_FLAME, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsidePutOutFlames)},
    {CUE_TABLE_END},
};

// The metatile the leader last stopped on, x in the high half and z in the low, or -1 for none
// (scMagmaRockInsideTrackStepMetatile).
Sint32 MagmaRockInsideLastStepMetatile  = -1;

// The cues of the Blaze tablet's room.
CUE    MagmaRockInsideBlazeTabletCues[] = {
    {CUE_EXIT,
     MAGMA_ROCK_INSIDE_BLAZE_TABLET_TO_MAGMA_ROCK_INSIDE_EMBER_STONE, CONDITION_ALWAYS,
     MAGMA_ROCK_INSIDE_BLAZE_TABLET_TO_MAGMA_ROCK_INSIDE_EMBER_STONE},
    {CUE_STEP,
     METATILE_STEPPING_STONE_A, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideStampStepMetatile)},
    {CUE_STEP,
     METATILE_STEPPING_STONE_B, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideStampOtherStepMetatile)},
    {EXAMINE_CUE_FACING(FACING_NORTH),
     METATILE_BLAZE_TABLET, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideReadBlazeTablet)},
    {EXAMINE_CUE_FACING(FACING_SOUTH),
     METATILE_BLAZE_TABLET, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsideReadBlazeTabletFromNorth)},
    {EXAMINE_CUE_FACING(FACING_WEST),
     METATILE_BLAZE_TABLET, CONDITION_ALWAYS,
     SCRIPT_REF(MagmaRockInsideReadBlazeTabletFromEast)},
    {EXAMINE_CUE_FACING(FACING_EAST),
     METATILE_BLAZE_TABLET, CONDITION_ALWAYS,
     SCRIPT_REF(MagmaRockInsideReadBlazeTabletFromWest)},
    {CUE_RAISED,
     STEP_METATILE_CUE_BASE + 2,
     CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideStepOnMarker)},
    {CUE_RAISED,
     STEP_METATILE_CUE_BASE + 3,
     CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideStepOnMarker)},
    {CUE_RAISED,
     STEP_METATILE_CUE_BASE + 4,
     CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideStepOnMarker)},
    {CUE_RAISED,
     STEP_METATILE_CUE_BASE + 5,
     CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideStepOnMarker)},
    {CUE_RAISED,
     STEP_METATILE_CUE_BASE + 6,
     CONDITION_ALWAYS, SCRIPT_REF(scMagmaRockInsideStepOnMarker)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SOURCE_FLAME, CONDITION_ALWAYS,
     SCRIPT_REF(MagmaRockInsideLightTabletTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_TORCH_FLAME, CONDITION_ALWAYS,
     0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_TORCH_FLAME, FLAG_TABLET_TORCH_LIT,
     SCRIPT_REF(scMagmaRockInsideGrowTabletFlame)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE),
     ACTOR_SOURCE_FLAME, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsidePutOutTabletFlame)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE),
     ACTOR_TORCH_FLAME, CONDITION_ALWAYS,
     SCRIPT_REF(scMagmaRockInsidePutOutTabletFlame)},
    {CUE_TABLE_END},
};

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
#undef DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR
#undef DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH
#undef DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM
