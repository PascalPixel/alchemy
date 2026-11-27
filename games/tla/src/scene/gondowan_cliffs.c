// The Gondowan Cliffs, the maze of ledges, vines and slides west of Madra on the way to Kibombo,
// and the river channel below them (SceneIdGondowanCliffs and SceneIdGondowanCliffsPass, which
// share this overlay). Coming from Madra, Felix finds a dog lying in the grass by the east
// entrance; Scoop on the yellow sand beside it digs up a geyser that carries him up onto the cliffs
// to the west. Whirlwind clears the weeds from a cave mouth nearby, where Kibombo warriors are
// hiding. Further on, Move pushes a tree stump off its ledge into the stream as a stepping stone,
// and close by Kindle, a Mars Djinni, waits to be fought; the tightrope across the river in the far
// west leads on towards Kibombo. Two puddles can be frozen with Frost once Felix has it, the
// eastern one on a later visit for the Healing Fungus. If the party comes back here after meeting
// Piers in Kibombo and before the ceremony at the Gabomba Statue is over, Kraden, Jenna and Sheba
// turn Felix back. Late in the game, Grind breaks the boulder in the river channel so the ship can
// sail on south to the western sea.

#include "field/field_mode.h"
#include "field/script_inline.h"
#include "scene/scene.h"
#include "game/game.h"
#include "field/actor_script.h"
#include "field/field_sprite.h"
#include "field/call.h"
#include "macros.h"

extern SCENE_ENTRANCE GondowanCliffsEntrances[];
extern SCENE_ENTRANCE GondowanCliffsPassEntrances[];

#include "shared/tile_sprite_reset.h"

// The cliffs' actors by their rows (Jenna's and Sheba's rows take their own numbers).
enum
{
    // The posts of the tightrope across the river, strung once Briggs is caught.
    ACTOR_TIGHTROPE_POST_A = ACTOR_FIRST_PLACED,
    ACTOR_TIGHTROPE_POST_B,
    // The posts of the rope Lash ties from the lower one up to the cliff.
    ACTOR_ROPE_POST_A,
    ACTOR_ROPE_POST_B,
    // The two puddles Frost freezes into pillars of ice.
    ACTOR_FIRST_PUDDLE,
    ACTOR_SECOND_PUDDLE,
    ACTOR_WEEDS,  // over the mouth of the cave Whirlwind opens
                  // The stump that stands in the stream once
                  // FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER is set.
    ACTOR_STUMP,
    ACTOR_DOG,
    ACTOR_GEYSER, // the unseen point the geyser rises from
                  // The two Kibombo warriors stranded by the river until Briggs is caught.
    ACTOR_FIRST_WARRIOR,
    ACTOR_SECOND_WARRIOR,
    // The boulder in the river channel, seen from the cliffs above until Grind breaks it.
    ACTOR_SEA_ROCK,
    ACTOR_KINDLE,
    ACTOR_KRADEN
};

// The river channel's actors: the unseen one that resets the tile sprites, the tightrope's two
// posts and the boulder Grind breaks.
enum
{
    ACTOR_PASS_TILE_SPRITE_RESET = ACTOR_FIRST_PLACED,
    ACTOR_PASS_TIGHTROPE_POST_A,
    ACTOR_PASS_TIGHTROPE_POST_B,
    ACTOR_PASS_SEA_ROCK
};

// The white dog's sprite curled up asleep in the grass.
enum
{
    DOG_ANIMATION_LYING = 4
};

// The metatile behaviour of the patch of yellow sand beside the dog, metatile (119, 45), where
// Scoop digs up the geyser.
enum
{
    METATILE_BEHAVIOR_GEYSER_SAND = 10
};

// The river channel's entrance a script sends the party in by, aboard the ship.
enum
{
    ENTRANCE_PASS_BY_SHIP = 99
};

// Unused: an entrance on the tree stump's ledge, metatile (79, 34), between the stump and Kindle.
// No exit, script or debug room leads to it.
enum
{
    ENTRANCE_STUMP_LEDGE = 99
};

// Talking to the red Djinni on the ledge near the tree stump starts the battle with Kindle, a Mars
// Djinni, who joins Felix once beaten.
void scGondowanCliffsFightKindle(void)
{
    fdRequestDjinnBattle(ACTOR_KINDLE, DJINNI_KINDLE);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
    if (MainRoutineStateWork.next == SCENE_GONDOWAN_CLIFFS_PASS)
        return GondowanCliffsPassEntrances;
    return GondowanCliffsEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
    return 0;
}

const Uint32 *scGetExits(void)
{
    return SceneExits;
}

extern ACTOR_PLACE GondowanCliffsActorPlacements[];
extern ACTOR_PLACE GondowanCliffsPassActorPlacements[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (MainRoutineStateWork.next == SCENE_GONDOWAN_CLIFFS_PASS)
        return GondowanCliffsPassActorPlacements;
    return GondowanCliffsActorPlacements;
}

// clang-format off
#include "shared/field_effect_duration_scripts.c"
// clang-format on

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

// The Gondowan Cliffs' scripts and set-up, and the geysers Scoop digs up out of the yellow sand:
// each rises and falls on its timer, carrying the leader up its column of water (scGeyserUpdate,
// btGeyserDraw).

#define EFFECT_SET_MODE   fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

// The rope Lash ties, for this visit.
#define FLAG_GONDOWAN_CLIFFS_ROPE_LASHED          FLAG_SCENE_TEMP_00
// Set as Frost freezes each puddle into a pillar of ice, for this visit.
#define FLAG_GONDOWAN_CLIFFS_FIRST_PUDDLE_FROZEN  FLAG_SCENE_TEMP_01
#define FLAG_GONDOWAN_CLIFFS_SECOND_PUDDLE_FROZEN FLAG_SCENE_TEMP_02

struct ACTOR;
struct EFFECT_OPTIONS;

// MAP_STATE.groundEffects read as its bit-fields: the footprints, and the kind of wading (1 in
// rooms with a second level).
typedef struct MAP_GROUND_EFFECTS
{
    Uint8 unusedBeforeGroundEffects[23]; // MAP_STATE's fields before it
    Uint8 footprints     : 2;
    Uint8 wading         : 2;
    Uint8 unusedHighBits : 4;            // no code names them
} MAP_GROUND_EFFECTS;

// The geyser Scoop digs up: it rises and falls on its timer, carrying the platform at its top, as
// Yampi Desert's geyser does.
typedef struct GEYSER
{
    Sint16 id;             // its actor; 0 for a free entry
    Sint16 timer;          // ticks into its period
    Sint16 height;         // pixels it has risen
    Sint16 period;         // ticks in one rise and fall
    Sint16 riseAt;         // the timer it starts to rise at
    Sint16 fallAt;         // the timer it starts to fall at
    Sint16 max;            // pixels it rises to
    Sint16 min;            // pixels it falls to
    Sint16 unreadSetToOne; // set to 1 by scGeyserAdd and never read
    Sint16 step;           // pixels a tick: 4 rising, -8 falling, 0 at rest
    Sint16 unreadCleared;  // cleared by scGeyserAdd and never read
    Sint16 phase;          // the platform's bob, 1/65536 turns
    ACTOR *rider;          // the platform at its top, which the leader rides
} GEYSER;

// The geysers' state, kept in the inventory snapshot's spare room.
typedef struct GEYSER_WORK
{
    Sint16 entry; // the VRAM cache entry holding the column's tiles
    // the terrain id and collision slot of the first entry
    Sint16 firstTerrain;
    GEYSER entries[GEYSER_ENTRIES];
    // The OAM words btGeyserDraw queues: GEYSER_SPRITES sprites of three words.
    void  *block;
} GEYSER_WORK;

void scGondowanCliffsFreezeSecondPuddle(void);
void scGondowanCliffsBlowAwayWeeds(void);
void scFieldEffectSpawn(Sint32                x,
                        Sint32                y,
                        Sint32                z,
                        Sint32                velocityX,
                        Sint32                velocityY,
                        Sint32                velocityZ,
                        Uint32                flags,
                        const EFFECT_OPTIONS *options);
void scGeyserStart(Sint32 unused, Sint32 firstTerrain);

// The leaping stump's update: its sprite turns a 32nd of a turn a tick.
void scGondowanCliffsSpinStump(ACTOR *actor)
{
    FIELD_SPR *sprite  = actor->sprite;

    sprite->rotation  -= TRIG_TURN / 32;
}

// Waits until the actor has come down to height (16.16), a second at most.
void scGondowanCliffsWaitToLand(ACTOR *actor, Sint32 height)
{
    Sint32 count = TICKS_PER_SECOND;

    while (count != 0)
    {
        syWait(1);
        count--;
        if (actor->y.fixed <= height)
            break;
    }
}

// Pushing the tree stump west with Move to the edge of its ledge, metatile (76, 34), sends it
// rolling off: it leaps into the stream and lands upright in the water with a thump and a splash,
// at metatile (74, 33), a stepping stone Felix can hop onto to cross.
//
// The Move row answers every push; only a push that leaves the stump in column 76 starts the leap.
// The landing metatile takes the hop flag of the metatile two rows north of it.
void scGondowanCliffsLeapStumpIntoWater(Sint32 mode, Sint32 id)
{
    ACTOR         *actor = fdActorGetById(id);
    // LEFTOVER(frame): an unused local the ROM's frame reserves and never
    // * touches.
    Sint32         unused[3];
    EFFECT_OPTIONS options;
    FIELD_SPR     *sprite;
    Uint32         i;

    if (FIELD_METATILE(actor->x.fixed) != 76)
        return;
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdMapGridCopyMetatileAttributes(74, 31, 1, 1, 74, 33);
    if (FIELD_METATILE(fdActorGetById(ACTOR_STUMP)->x.fixed) == 76)
    {
        actor->motionFlags    = 0;
        actor->collisionFlags = 0;
        for (i = 0; i <= 17; i++)
        {
            syWait(1);
            sprite            = actor->sprite;
            sprite->rotation -= TRIG_TURN / 256;
            actor->x.fixed   -= syCos(((FIELD_SPR *)actor->sprite)->rotation) / 2;
            actor->targetX    = ACTOR_NO_TARGET;
        }
        actor->update = scGondowanCliffsSpinStump;
        fdActorSetSpeed(id, 3);
        fdActorSetDestination(id, 1193, 530);
        actor->gravity     = toFIXED(4) / 5;
        actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        actor->layer       = 0;
        scFieldActorWaitForMoveById(id);
        scGondowanCliffsWaitToLand(actor, 0);
        fdScriptPlaySound(SE_THUMP);
        fdMapGridSetShake(5, 5, 1);
        fdScriptPlaySound(SE_RUMBLE);
        fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        ((FIELD_SPR *)actor->sprite)->rotation = TRIG_TURN / 16;
        actor->update                          = NULL;
        options.type                           = SPRITE_SPLASH;
        options.startScaleX                    = toFIXED(1) / 2;
        options.targetScaleX                   = toFIXED(3) / 2;
        options.targetScaleY                   = toFIXED(6) / 5;
        options.startScaleY                    = toFIXED(4) / 5;
        scFieldEffectSpawn(actor->x.fixed,
                           actor->y.fixed,
                           actor->z.fixed,
                           0,
                           0,
                           0,
                           EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
                           &options);
        fdScriptPlaySound(SE_CAST);
        fdActorSetAnimation(id, STUMP_ANIMATION_SPLASH);
        syFieldMapGridWaitForShakeToSettle();
        fdActorGetById(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        fdScriptSetFlag(FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER);
    }
    fdScriptEnd();
}

// Frost on the puddle in the sandy gap between two ledges, near the tree stump, metatile (81, 28),
// raises a pillar of ice there that Felix can hop onto to cross the gap; it stays until he leaves
// the cliffs.
//
// The pillar's sprite stops moving, and the puddle's metatile takes the hop flag of the metatile
// two to its west.
void scGondowanCliffsFreezeFirstPuddle(void)
{
    ACTOR *actor         = fdActorGet(ACTOR_FIRST_PUDDLE);

    actor->priorityFlags = 0;
    actor->motionFlags   = 0;
    fdMapGridCopyMetatileAttributes(79, 28, 1, 1, 81, 28);
    fdScriptSetFlag(FLAG_GONDOWAN_CLIFFS_FIRST_PUDDLE_FROZEN);
}

// Frost on the puddle in the grass at the east end of the cliffs, metatile (117, 35), raises a
// pillar of ice there that Felix can hop onto to jump the gap on the way to the Healing Fungus; it
// stays until he leaves the cliffs.
//
// The pillar's sprite stops moving and its height is set to 32 pixels, and the puddle's metatile
// takes the hop flag of the metatile two to its west.
void scGondowanCliffsFreezeSecondPuddle(void)
{
    ACTOR *actor         = fdActorGet(ACTOR_SECOND_PUDDLE);

    actor->priorityFlags = 0;
    actor->motionFlags   = 0;
    actor->groundHeight  = toFIXED(32);
    actor->y.fixed       = toFIXED(32);
    fdMapGridCopyMetatileAttributes(115, 35, 1, 1, 117, 35);
    fdScriptSetFlag(FLAG_GONDOWAN_CLIFFS_SECOND_PUDDLE_FROZEN);
}

// Whirlwind on the weeds hanging over a cave mouth west of the dog blows it away for good, opening
// the way into the cave where the Kibombo warriors hide.
//
// The leaves are Whirlwind's own; the script only sets the flag that keeps the weeds gone and opens
// the cave's exit.
void scGondowanCliffsBlowAwayWeeds(void)
{
    syGameFlagSet(FLAG_GONDOWAN_CLIFFS_WEEDS_BLOWN);
}

// Scoop on the patch of yellow sand beside the dog at the east end, metatile (119, 45), digs up a
// geyser that from then on rises and falls there, a lift Felix can ride up to the cliffs above.
//
// The Scoop row runs the script every tick of the dig; at `SCOOP_DUG_TICK` it redraws the dug
// ground from a spare metatile of the map and moves the geyser's unseen point onto it.
void scGondowanCliffsScoopUpGeyser(Sint32 stage, Sint32 frame)
{
    ACTOR *actor;

    if (frame == SCOOP_DUG_TICK)
    {
        actor = fdActorGet(ACTOR_GEYSER);
        fdMapGridCopyMetatilesLayered(124, 1, 1, 1, 119, 43);
        fdActorSetPos(ACTOR_GEYSER, 1912, 728);
        actor->groundHeight = actor->y.fixed =
            fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
        fdScriptSetFlag(FLAG_GONDOWAN_CLIFFS_GEYSER_DUG);
    }
}

#include "text/msg_ids.h"

// Talking to the white dog lying in the grass at the east end makes it bark "Rrruff! Rrruff!" at
// Felix. When Felix is facing the yellow sand beside it, metatile (119, 45), the button opens the
// field menu instead, as it does where there is nothing to talk to, so that he can choose Scoop
// there.
void scGondowanCliffsTalkToDog(void)
{
    ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);

    struct
    {
        Sint32 x, y, z;
    } pos;

    Sint32 *out = (Sint32 *)&pos;

    out[0]      = actor->x.fixed;
    out[1]      = actor->y.fixed;
    out[2]      = actor->z.fixed;
    syFieldVectorAddPolarOffset(16, actor->facing, out);
    if (FIELD_METATILE(pos.x) == 119 && FIELD_METATILE(pos.z) == 45)
    {
        FieldStateWork->fieldMenu = 1;
    }
    else
    {
        fdScriptStart();
        syFieldScriptEndLastingPsynergy(0);
        fdActorFaceEachOther(ACTOR_DOG, MainRoutineStateWork.leaderActorId, 0);
        syFieldScriptSetMessageId((Sint32)MsgRrruffRrruff);
        // "Rrruff! Rrruff!"
        fdScriptShowMessage(ACTOR_DOG, 0);
        fdActorFaceDirection(ACTOR_DOG, FACING_WEST, 0);
        fdScriptEnd();
    }
}

extern CUE GondowanCliffsPassCues[];
extern CUE GondowanCliffsCues[];

// Grind on the tall brown boulder standing in the river channel grinds it away for good, so the
// ship can sail on through.
void       scGondowanCliffsGrindTheRock(Sint32 unused, Sint32 id)
{
    fdActorSetPos(id, 0, 0);
    fdScriptSetFlag(FLAG_GONDOWAN_CLIFFS_ROCK_GROUND);
}

const CUE *scGetCues(void)
{
    if (MainRoutineStateWork.next == SCENE_GONDOWAN_CLIFFS_PASS)
        return GondowanCliffsPassCues;
    return GondowanCliffsCues;
}

// The water's colours as the map loads them, which the shimmer turns through.
Uint16 GondowanCliffsSavedWaterColors[WATER_COLORS] BSS_DATA;

// Makes the water shimmer: its four colours turn one step every 16 ticks.
void   scGondowanCliffsCycleWaterColors(void)
{
    Uint16 *color = (Uint16 *)&BG_PLTT_COLOR(0, FIRST_WATER_COLOR);
    Uint32  phase = TickCount >> 4;
    Sint32  i;

    for (i = 0; i < WATER_COLORS; i++)
    {
        Uint32 index  = 3;

        index        &= phase;
        *color++      = GondowanCliffsSavedWaterColors[index];
        phase++;
    }
}

// Sets up the river channel: arriving aboard the ship, the party sails on, the tightrope across the
// channel is strung and the water starts to shimmer.
void scGondowanCliffsSetUpPass(void)
{
    if (MainRoutineStateWork.entrance == ENTRANCE_PASS_BY_SHIP)
    {
        MainRoutineStateWork.movementMode  = LEADER_MOVEMENT_SHIP;
        MainRoutineStateWork.ownerOverride = 1;
        fdScriptSetDestination(SCENE_GONDOWAN_CLIFFS_PASS,
                               GONDOWAN_CLIFFS_PASS_FROM_WORLD_MAP_NORTH);
    }
    fdTightropeInit();
    fdTightropePlace(0, ACTOR_PASS_TIGHTROPE_POST_A, ACTOR_PASS_TIGHTROPE_POST_B);
    syWordsCopy((Sint32)GondowanCliffsSavedWaterColors,
                (Sint32)&BG_PLTT_COLOR(0, FIRST_WATER_COLOR),
                sizeof(GondowanCliffsSavedWaterColors));
    sySetTickEvent(scGondowanCliffsCycleWaterColors, EVENT_PASS_TICK);
}

// Coming back to the cliffs after meeting Piers in Kibombo, before the ceremony at the Gabomba
// Statue is over, the party stops as soon as it enters: Kraden asks "Felix! Where do you intend to
// go?", Jenna and Sheba side with him, everyone nods, and Felix walks back out to the world map to
// go and help Piers.
void scGondowanCliffsTurnBackToHelpPiers(void)
{
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 552, 488);
    fdActorPlaceOnActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
    scFieldActorWatchAndSetScript(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 568, 480);
    scFieldActorWatchAndSetScript(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
    fdActorPlaceOnActor(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 584, 472);
    scFieldActorWatchAndSetScript(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
    fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 608, 472);
    syFieldScriptSetMessageId((Sint32)MsgWhereDoYou);
    scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
    // "Felix! Where do you intend to go?"
    fdScriptShowMessage(ACTOR_KRADEN, 0);
    fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 10);
    scFieldActorJump(ACTOR_JENNA, 6, 15);
    scFieldActorJump(ACTOR_JENNA, 6, 23);
    // "Kraden's right. I think we should try to help Piers, too."
    fdScriptShowMessage(ACTOR_JENNA, 0);
    scFieldActorStop(ACTOR_SHEBA);
    fdActorFaceActor(ACTOR_SHEBA, ACTOR_JENNA, 0);
    // "I completely agree with Kraden and Jenna."
    fdScriptShowMessage(ACTOR_SHEBA, 0);
    scFieldActorWatchAndSetScript(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
    fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
                             ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    syFieldScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    // "It looks like the majority wins! Let's go back to Kibombo and help..."
    fdScriptShowMessage(ACTOR_KRADEN, 0);
    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    syFieldScriptWait(30);
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 584, 472);
    fdActorSetPos(ACTOR_KRADEN, 0, 0);
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 568, 480);
    fdActorSetPos(ACTOR_SHEBA, 0, 0);
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 552, 488);
    fdActorSetPos(ACTOR_JENNA, 0, 0);
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 528, 488);
    fdScriptFinishScreenTransition();
    fdScriptPlaySound(SE_MAP_EXIT);
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 512, 488);
    fdScriptWaitTransitionTicks();
    syFieldScriptSetExitRequest(GONDOWAN_CLIFFS_TO_WORLD_MAP_A);
}

GEYSER *scGeyserAdd(Sint32 slot, Sint32 id, Sint32 timer, Sint32 low, Sint32 high);

// Sets up the scene being entered. The river channel has its own set-up; the cliffs turn the party
// back while Piers still needs their help, and otherwise tie the rope and string the tightrope, put
// back the weeds, the frozen puddles, the geyser and the stump as their flags say, and start the
// water shimmering.
Sint32  scInit(void)
{
    ACTOR *actor;
    Sint32 height;

    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
    if (MainRoutineStateWork.next == SCENE_GONDOWAN_CLIFFS_PASS)
    {
        scGondowanCliffsSetUpPass();
        return 0;
    }
    if (syGameFlagGet(FLAG_KIBOMBO_PIERS_MET) && !syGameFlagGet(FLAG_GABOMBA_CEREMONY_ENDED))
    {
        scGondowanCliffsTurnBackToHelpPiers();
        return 0;
    }
    fdActorGetById(fdPartyGetLeaderActor())->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    if (MainRoutineStateWork.next == SCENE_GONDOWAN_CLIFFS)
    {
        fdRopeInit();
        fdRopePlace(0, FLAG_GONDOWAN_CLIFFS_ROPE_LASHED, ACTOR_ROPE_POST_A, ACTOR_ROPE_POST_B);
        if (syGameFlagGet(FLAG_BRIGGS_CAUGHT))
        {
            fdTightropeInit();
            fdTightropePlace(0, ACTOR_TIGHTROPE_POST_A, ACTOR_TIGHTROPE_POST_B);
        }
        if (syGameFlagGet(FLAG_GONDOWAN_CLIFFS_WEEDS_BLOWN))
            fdActorSetPos(ACTOR_WEEDS, 0, 0);
        else
            fdActorGetById(ACTOR_WEEDS)->scaleY = toFIXED(5) / 4;
        if (syGameFlagGet(FLAG_GONDOWAN_CLIFFS_FIRST_PUDDLE_FROZEN))
            fdMapGridCopyMetatileAttributes(79, 28, 1, 1, 81, 28);
        if (syGameFlagGet(FLAG_GONDOWAN_CLIFFS_SECOND_PUDDLE_FROZEN))
            fdMapGridCopyMetatileAttributes(115, 35, 1, 1, 117, 35);
        scGeyserStart(0, GEYSER_FIRST_TERRAIN);
        scGeyserAdd(0, ACTOR_GEYSER, 0, 0, GEYSER_MAX_HEIGHT);
        if (syGameFlagGet(FLAG_GONDOWAN_CLIFFS_GEYSER_DUG))
        {
            actor = fdActorGetById(ACTOR_GEYSER);
            fdMapGridCopyMetatilesLayered(124, 1, 1, 1, 119, 43);
            fdActorSetPos(ACTOR_GEYSER, 1912, 728);
            height              = fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
            actor->y.fixed      = height;
            actor->groundHeight = height;
        }
        fdActorSetAnimation(ACTOR_DOG, DOG_ANIMATION_LYING);
        if (syGameFlagGet(FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER))
        {
            fdMapGridCopyMetatileAttributes(74, 31, 1, 1, 74, 33);
            fdActorSetPos(ACTOR_STUMP, 1193, 530);
            fdActorGetById(ACTOR_STUMP)->priorityFlags  |= ACTOR_PRIORITY_UNDERFOOT;
            fdActorGetById(ACTOR_STUMP)->collisionFlags  = 0;
            fdActorSetAnimation(ACTOR_STUMP, STUMP_ANIMATION_SINK);
        }
        else
        {
            fdActorGetById(ACTOR_STUMP)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
        }
        syWordsCopyEntry(((Uint8 *)GondowanCliffsSavedWaterColors),
                         (void *)&BG_PLTT_COLOR(0, FIRST_WATER_COLOR),
                         sizeof(GondowanCliffsSavedWaterColors));
        sySetTickEvent(scGondowanCliffsCycleWaterColors, EVENT_PASS_TICK);
        ((MAP_GROUND_EFFECTS *)RamHeapSlots->mapGrid)->wading = 1;
    }
    return 0;
}

extern Sint16 GondowanCliffsCameraBoxes[];

Sint32        scPrepareMap(void)
{
    if (MainRoutineStateWork.next == SCENE_GONDOWAN_CLIFFS)
        ((MAP_STATE *)RamHeapSlots->mapGrid)->cameraBoxes = GondowanCliffsCameraBoxes;
    return 0;
}

#include "shared/geyser_update.h"

#include "shared/geyser_draw.h"

#include "shared/geyser_start.h"

// Makes actor id the geyser of entry slot, rising from low to high pixels every six seconds, with
// an unseen platform at its top for the leader.
GEYSER *scGeyserAdd(Sint32 slot, Sint32 id, Sint32 timer, Sint32 low, Sint32 high)
{
    GEYSER_WORK *state = GEYSER_STATE;
    GEYSER      *entry = &state->entries[slot];
    ACTOR       *actor = fdActorGet(id);
    ACTOR       *rider;

    fdActorGet(id);
    fdActorSetSpriteFlags(actor, 0);
    if (!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
    {
        if (high < low)
        {
            Sint32 swap = low;

            low         = high;
            high        = swap;
        }
        entry->id              = id;
        entry->height          = 0;
        entry->timer           = timer;
        entry->period          = GEYSER_PERIOD_TICKS;
        entry->max             = high;
        entry->riseAt          = GEYSER_RISE_TICK;
        entry->fallAt          = GEYSER_FALL_TICK;
        entry->min             = low;
        entry->unreadSetToOne  = 1;
        actor->motionFlags     = 0;
        actor->collisionFlags  = ACTOR_NOT_TARGETED;
        entry->step            = 0;
        entry->phase           = 0;
        entry->unreadCleared   = 0;
        actor->priorityFlags  |= ACTOR_PRIORITY_UNDERFOOT;
    }
    rider                  = (scFieldActorCreate(
        ACTOR_CREATE_KIND(ACTOR_CREATE_NO_SPRITE), actor->x.fixed, actor->y.fixed, actor->z.fixed));
    rider->collisionFlags  = 0;
    rider->motionFlags     = 0;
    rider->priorityFlags  |= ACTOR_PRIORITY_PLATFORM;
    entry->rider           = rider;
    return entry;
}

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY

// The Gondowan Cliffs' tables, which the scene's getters return.

extern Sint32    GondowanCliffsResetTileSpriteScript[];

LATE_DATA Sint32 GondowanCliffsResetTileSpriteScript[] = {
    SC_CALL(scTileSpriteReset),
    SC_END,
};

// clang-format off
LATE_DATA SCENE_ENTRANCE GondowanCliffsEntrances[] = {
    {0, CONDITION_ALWAYS, 680, 0, 280, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {GONDOWAN_CLIFFS_FROM_WORLD_MAP_A, CONDITION_ALWAYS, 536, 0, 488, FACING_EAST, 0, 512, 0, 2048, 864, 0},
    {GONDOWAN_CLIFFS_FROM_WORLD_MAP_B, CONDITION_ALWAYS, 2024, 0, 792, FACING_WEST, 0, 512, 0, 2048, 864, 0},
    {GONDOWAN_CLIFFS_FROM_GONDOWAN_CLIFFS_HIDEOUT, CONDITION_ALWAYS, 1576, 0, 680, FACING_SOUTH, 0, 512, 0, 2048, 864, 0},
    {ENTRANCE_STUMP_LEDGE, CONDITION_ALWAYS, 1272, 0, 552, FACING_SOUTH, 0, 512, 0, 2048, 864, 0},
    {SCENE_TABLE_END},
};

LATE_DATA SCENE_ENTRANCE GondowanCliffsPassEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0, 280, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA Uint32 SceneExits[] = {
    SCENE_EXITS(GondowanCliffs),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_TO_WORLD_MAP_A, WorldMap, WORLD_MAP_FROM_GONDOWAN_CLIFFS_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_TO_WORLD_MAP_B, WorldMap, WORLD_MAP_FROM_GONDOWAN_CLIFFS_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_TO_GONDOWAN_CLIFFS_HIDEOUT, GondowanCliffsHideout, GONDOWAN_CLIFFS_HIDEOUT_FROM_GONDOWAN_CLIFFS, EXIT_FLAG_NONE),
    SCENE_EXITS(GondowanCliffsPass),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_PASS_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_CLIFFS_PASS_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_PASS_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_CLIFFS_PASS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

LATE_DATA ACTOR_PLACE GondowanCliffsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1560, 0, 760, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1624, 0, 760, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1304, 0, 456, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1880, 0, 568, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1576, 0, 672, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1232, 0, 544, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1928, 0, 728, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 888, 0, 200, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 888, 0, 232, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, FLAG_GONDOWAN_CLIFFS_ROCK_GROUND, MOVEMENT_TYPE_STAND, 816, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_KINDLE), MOVEMENT_TYPE_STAND, 1336, 0, 648, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, IF_FLAG_SET(FLAG_KIBOMBO_PIERS_MET), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, IF_FLAG_SET(FLAG_KIBOMBO_PIERS_MET), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, IF_FLAG_SET(FLAG_KIBOMBO_PIERS_MET), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE GondowanCliffsPassActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, (Sint32)GondowanCliffsResetTileSpriteScript, 330, 0, 79, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, FLAG_GONDOWAN_CLIFFS_ROCK_GROUND, MOVEMENT_TYPE_STAND, 304, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

LATE_DATA CUE GondowanCliffsPassCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, 1},
    {CUE_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_PASS_SEA_ROCK, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCliffsGrindTheRock)},
    {CUE_TABLE_END},
};

LATE_DATA CUE GondowanCliffsCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, 1},
    {CUE_EXIT, 2, CONDITION_ALWAYS, 2},
    {CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, IF_FLAG_SET(FLAG_GONDOWAN_CLIFFS_WEEDS_BLOWN), 3},
    {CUE_TALK, ACTOR_DOG, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCliffsTalkToDog)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOG, FLAG_GONDOWAN_CLIFFS_GEYSER_DUG, MSGNO(MsgDigHereYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOG, CONDITION_ALWAYS, MSGNO(MsgIKnewIt)},
    {CUE_TALK, ACTOR_FIRST_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgITriedTo)},
    {CUE_TALK, ACTOR_SECOND_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgIWonderWho)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgIfTheOther)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgILongTo)},
    {CUE_TALK, ACTOR_KINDLE, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCliffsFightKindle)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST_A, FLAG_GONDOWAN_CLIFFS_ROPE_LASHED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE, FLAG_GONDOWAN_CLIFFS_FIRST_PUDDLE_FROZEN, SCRIPT_REF(scGondowanCliffsFreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE, FLAG_GONDOWAN_CLIFFS_SECOND_PUDDLE_FROZEN, SCRIPT_REF(scGondowanCliffsFreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEEDS, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCliffsBlowAwayWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER, SCRIPT_REF(scGondowanCliffsLeapStumpIntoWater)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_GEYSER_SAND, FLAG_GONDOWAN_CLIFFS_GEYSER_DUG, SCRIPT_REF(scGondowanCliffsScoopUpGeyser)},
    {CUE_TABLE_END},
};

// The parts of the cliffs the camera keeps out of, left, top, right and bottom in pixels.
LATE_DATA Sint16 GondowanCliffsCameraBoxes[] ALIGNED(4) = {
    512, 512, 1024, 864,
    1536, 0, 4096, 352,
    -1, -1, -1, -1,
};
// clang-format on
