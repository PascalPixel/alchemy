// The Kibombo Mountains, the pass the Kibombo warriors guard on the way to their village, in three
// rooms: the chest room with the Disk Axe, the ledges above it where Waft hides, and the small
// cavern behind a bush.
//
// As the party nears the pass, warriors report to one another that the way is clear
// (scKibomboMountainsGuardsReport), and from then on any guard who sees Felix shouts for the others
// and the party is marched back down the road (scKibomboMountainsGuardSpotsParty). In the chest
// room the party climbs the vines past the second guard, hops across the two crates below him once
// they are pushed into place, drops a pillar into the gap in the west as a shortcut, and pushes the
// crate along the first guard's ledge to hide behind it; the rope over the third guard is tied with
// Lash. On the ledges above, a crate pushed off a ledge knocks out the guard below, a pillar
// dropped into the dog's run shuts the dog in, and Tremor shakes a bone off a crate to keep the dog
// busy; Whirlwind blows the bush from the cavern's way in. Later, with Frost and Growth, an ice
// pillar and a vine lead up to Waft (scKibomboMountainsFightWaft), and the warriors let the party
// pass.

#include "item_ids.h"
#include "field/script_inline.h"
#include "field/field_mode.h"
#include "scene/scene.h"
#include "game/game.h"
#include "field/field_script.h"
#include "field/call.h"
#include "edition.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

// Once the message of the Gabomba is carried the Kibombo let the party pass: the guards and the dog
// are gone.
#define FLAG_KIBOMBO_LETS_PARTY_PASS            FLAG_GABOMBA_MESSAGE_CARRIED
#define FLAG_KIBOMBO_GUARDS_REPORTED            0x880
#define FLAG_KIBOMBO_BUSH_BLOWN_AWAY            0x882
// Set once the party has lashed the rope between the posts.
#define FLAG_KIBOMBO_ROPE_TIED                  0x9e8
#define FLAG_KIBOMBO_FIRST_PILLAR_DROPPED       0x8e7
#define FLAG_KIBOMBO_SECOND_PILLAR_DROPPED      0x8e8
// For this visit: the lower guard knocked out by the crate, the dog shut in south or north of the
// dropped crate or eating the bone, and the party turned back by the guards.
#define FLAG_GUARD_KNOCKED_OUT                  FLAG_SCENE_TEMP_00
#define FLAG_DOG_SHUT_SOUTH                     FLAG_SCENE_TEMP_01
#define FLAG_DOG_EATING_BONE                    FLAG_SCENE_TEMP_02
#define FLAG_DOG_SHUT_NORTH                     FLAG_SCENE_TEMP_03
#define FLAG_TURNED_BACK                        FLAG_AREA_TEMP_00
// For this visit: the first and second puddles of the Disk Axe room frozen, the Waft room's puddle
// frozen and its sprout grown, and the dog's pillar and the guard's crate dropped.
#define FLAG_DISK_AXE_ROOM_FIRST_PUDDLE_FROZEN  FLAG_SCENE_TEMP_10
#define FLAG_DISK_AXE_ROOM_SECOND_PUDDLE_FROZEN FLAG_SCENE_TEMP_11
#define FLAG_WAFT_ROOM_PUDDLE_FROZEN            FLAG_SCENE_TEMP_21
#define FLAG_SPROUT_GROWN                       FLAG_SCENE_TEMP_22
#define FLAG_DOG_PILLAR_DROPPED                 FLAG_SCENE_TEMP_23
#define FLAG_GUARD_CRATE_DROPPED                FLAG_SCENE_TEMP_24

// The window the rooms open with: a shape in Japanese, an interlaced wipe in every edition but the
// Japanese.
#if !REVISED_ENTRANCE_TRANSITIONS
#    define ROOM_TRANSITION SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE)
#else
#    define ROOM_TRANSITION SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE)
#endif

// The room where Waft hides above the guards, by the rows of KibomboMountainsWaftPlacements.
typedef enum KIBOMBO_MOUNTAINS_WAFT_ACTOR_ID
{
    // The warrior on watch above the dog, who also shouts when it barks.
    ACTOR_UPPER_GUARD = ACTOR_FIRST_PLACED,
    // The warrior on watch below, whom the crate can knock out.
    ACTOR_LOWER_GUARD,
    ACTOR_GUARD_DOG,
    // The puddle Frost turns into a pillar.
    ACTOR_PUDDLE,
    // The sprout Growth turns into a vine up to Waft's ledge.
    ACTOR_SPROUT,
    // The bush Whirlwind blows away from the small cavern's way in.
    ACTOR_CAVERN_BUSH,
    // The pillar pushed off the ledge to shut the dog in.
    ACTOR_DOG_PILLAR,
    // The crate pushed off the ledge onto the lower guard.
    ACTOR_GUARD_CRATE,
    // The crate the bone lies on, which Tremor shakes.
    ACTOR_BONE_CRATE,
    ACTOR_BONE,
    ACTOR_WAFT,
    // Unused: an unseen marker under the trees south of the dog's pillar. It draws nothing, and no
    // cue or script names it; the room only takes away its shadow and collision as it opens.
    ACTOR_UNSEEN_MARKER
} KIBOMBO_MOUNTAINS_WAFT_ACTOR_ID;

// The room of the Disk Axe chest, by the rows of KibomboMountainsDiskAxeChestPlacements.
typedef enum KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_ACTOR_ID
{
    ACTOR_WEST_ROPE_POST = ACTOR_FIRST_PLACED,
    ACTOR_EAST_ROPE_POST,
    ACTOR_FIRST_GUARD,
    ACTOR_SECOND_GUARD,
    ACTOR_THIRD_GUARD,
    // The pillar dropped to the chest's ledge.
    ACTOR_SECOND_DROPPED_PILLAR,
    // The two crates the party hides behind from the first guard.
    ACTOR_WEST_CRATE,
    ACTOR_EAST_CRATE,
    // The two crates further south.
    ACTOR_LOWER_WEST_CRATE,
    ACTOR_LOWER_EAST_CRATE,
    ACTOR_FIRST_PUDDLE,
    ACTOR_SECOND_PUDDLE,
    // The pillar dropped to the lower path.
    ACTOR_FIRST_DROPPED_PILLAR,
    ACTOR_WEST_TORCH,
    ACTOR_EAST_TORCH,
    // The warriors who turn the party back; ACTOR_WARRIOR_E also comes to hear the second guard's
    // report.
    ACTOR_WARRIOR_A,
    ACTOR_WARRIOR_B,
    ACTOR_WARRIOR_C,
    ACTOR_WARRIOR_D,
    ACTOR_WARRIOR_E,
    ACTOR_WARRIOR_F
} KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_ACTOR_ID;

// The chest room's entrances the guards send the party back to: the first time it is caught on a
// visit the warriors march it down the road, after that it is only put back.
typedef enum KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_ENTRANCE
{
    ENTRANCE_CAUGHT = 8,
    ENTRANCE_CAUGHT_AGAIN
} KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_ENTRANCE;

// The metatile where the dog's pillar lands off its ledge, beside the dog's run.
#define DOG_PILLAR_METATILE_X  47
#define DOG_PILLAR_METATILE_Z  28
// The metatile where the guard's crate lands on the lower guard's path.
#define GUARD_CRATE_METATILE_X 53
#define GUARD_CRATE_METATILE_Z 36

// The rooms' own metatile behaviours, named by what stands near them.
typedef enum KIBOMBO_MOUNTAINS_METATILE_BEHAVIOR
{
    // Waft's ledges: south west of the upper guard, metatiles (35, 21) to (35, 23), and south east
    // of him, (41, 23) to (41, 26); by the north end of the dog's run, (49, 22) to (49, 24), (45,
    // 24) and (45, 25); south of the lower guard's path, (51, 36) to (51, 38) and (47, 38) to (47,
    // 40), and south east of it, (54, 38) to (55, 41); and by the south end of the dog's run, (50,
    // 30) to (50, 32).
    METATILE_BEHAVIOR_WEST_OF_UPPER_GUARD = 20,
    METATILE_BEHAVIOR_EAST_OF_UPPER_GUARD,
    METATILE_BEHAVIOR_NORTH_END_OF_DOG_RUN,
    METATILE_BEHAVIOR_SOUTH_OF_LOWER_GUARD,
    METATILE_BEHAVIOR_SOUTH_EAST_OF_LOWER_GUARD,
    METATILE_BEHAVIOR_SOUTH_END_OF_DOG_RUN,
    // Waft's ledges: the edge Felix jumps west from, metatile (43, 30).
    METATILE_BEHAVIOR_JUMP_WEST           = 33,
    // The chest room: the first guard's ledge west of where he paces, metatiles (8, 18) to (10,
    // 19), and east of it, (16, 18) to (19, 18); east of the second guard, (18, 28) to (18, 30),
    // and south of him, (14, 30) to (17, 32); between the torches below him, (15, 33) to (16, 34);
    // and the rope's line above the third guard, (26, 12) and (27, 12), and the metatiles west of
    // him, (25, 15), (24, 16) and (25, 16).
    METATILE_BEHAVIOR_WEST_OF_FIRST_GUARD = 30,
    METATILE_BEHAVIOR_EAST_OF_FIRST_GUARD,
    METATILE_BEHAVIOR_EAST_OF_SECOND_GUARD,
    METATILE_BEHAVIOR_SOUTH_OF_SECOND_GUARD,
    METATILE_BEHAVIOR_BETWEEN_TORCHES,
    METATILE_BEHAVIOR_ROPE_ABOVE_THIRD_GUARD,
    METATILE_BEHAVIOR_WEST_OF_THIRD_GUARD
} KIBOMBO_MOUNTAINS_METATILE_BEHAVIOR;

// A cue's subject that answers only while the leader faces `facing`.
#define FACING_SUBJECT(facing, target) ((Sint16)((facing) | CUE_SUBJECT_NEEDS_FACING | (target)))

extern Sint32 KibomboMountainsTorchScript[];
extern Sint32 KibomboMountainsCrateScript[];
extern Sint32 KibomboMountainsWatchScript[];
extern Sint32 KibomboMountainsPaceScript[];
extern Sint32 KibomboMountainsDogRunScript[];
extern Sint32 KibomboMountainsSecondGuardPaceScript[];

#include "shared/field_rope_place.c"

// Felix talks to Waft, the Jupiter Djinni on the high ledge reached by the ice pillar and the vine:
// Waft fights.
void scKibomboMountainsFightWaft(void)
{
    fdRequestDjinnBattle(ACTOR_WAFT, DJINNI_WAFT);
}

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

#include "shared/actor_bob.h"

extern ACTOR_PLACE KibomboMountainsPlacements[];
extern ACTOR_PLACE KibomboMountainsWaftPlacements[];
extern ACTOR_PLACE KibomboMountainsDiskAxeChestPlacements[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_KIBOMBO_MOUNTAINS_WAFT)
        return KibomboMountainsWaftPlacements;
    if (scene == SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST)
        return KibomboMountainsDiskAxeChestPlacements;
    return KibomboMountainsPlacements;
}

// Piers casts Frost on the puddle on the third level of Waft's ledges: it freezes into an ice
// pillar, and the metatile beside it, (22, 32), becomes one Felix can hop across on its top.
void scKibomboMountainsFreezePuddle(void)
{
    ACTOR *actor         = fdActorGet(ACTOR_PUDDLE);

    actor->priorityFlags = 0;
    actor->motionFlags   = 0;
    fdMapGridCopyMetatileAttributes(24, 32, 1, 1, 22, 32);
    fdScriptSetFlag(FLAG_WAFT_ROOM_PUDDLE_FROZEN);
}

// Growth on the sprout near Waft's ledge: it grows into a vine, and metatiles (28, 33) and (28, 34)
// become climbable, up to Waft.
void scKibomboMountainsGrowVine(void)
{
    fdMapGridCopyMetatileAttributes(30, 35, 1, 1, 28, 34);
    fdMapGridCopyMetatileAttributes(30, 34, 1, 1, 28, 33);
    fdScriptSetFlag(FLAG_SPROUT_GROWN);
}

#include "shared/actor_wait_to_land.h"

// Nothing visible happens as Felix starts pushing the crate on the ledge above the lower guard's
// path, metatile (55, 34). The crate's sprite is set to be drawn in front of the guard, so that it
// covers him if it lands on him.
void scKibomboMountainsStartPushingGuardCrate(Sint32 unused, Sint32 id)
{
    if (id == ACTOR_GUARD_CRATE)
        scFieldActorSetSpritePriority(ACTOR_GUARD_CRATE, 1);
}

extern Sint32 KibomboMountainsCrateJumpScript[];

// Felix pushes the pillar or the crate off its ledge on Waft's ledges. The pillar lands in the
// middle of the dog's run, metatile (47, 28), shutting the dog in north or south of it (a dog
// caught under it cries out and is pushed aside), and Felix can hop across its top. The crate lands
// on the lower guard's path, metatile (53, 36): if the guard is under it, he walks into it and is
// trapped, the crate rocking as he struggles; if not, the guard jumps, looks at the crate puzzled,
// shakes his head and walks on, and the crate can be pushed again.
void          scKibomboMountainsDropOffLedge(Sint32 unused, Sint32 id)
{
    ACTOR saved;

    if (id == ACTOR_DOG_PILLAR)
    {
        ACTOR *pillar = fdActorGetById(ACTOR_DOG_PILLAR);
        ACTOR *dog;
        Sint32 x;
        Sint32 z;
        Sint32 metatile;

        if (!fdScriptGetFlag(FLAG_DOG_PILLAR_DROPPED)
            && (x = FIELD_METATILE(pillar->x.fixed)) == DOG_PILLAR_METATILE_X
            && (z = FIELD_METATILE(pillar->z.fixed)) == DOG_PILLAR_METATILE_Z)
        {
            scActorWaitToLand(ACTOR_DOG_PILLAR);
            fdMapGridCopyMetatileAttributes(49, 28, 1, 1, x, z);
            fdScriptSetFlag(FLAG_DOG_PILLAR_DROPPED);
            fdActorSetSpritePriority(ACTOR_DOG_PILLAR, 2);
            dog      = fdActorGetById(ACTOR_GUARD_DOG);
            metatile = FIELD_METATILE(dog->z.fixed);
            if (metatile == DOG_PILLAR_METATILE_Z - 1)
            {
                sySoundPlay(SE_VOICE_ANIMAL_B);
                dog->z.fixed = toFIXED(440);
            }
            else if (metatile == DOG_PILLAR_METATILE_Z || metatile == DOG_PILLAR_METATILE_Z + 1)
            {
                sySoundPlay(SE_VOICE_ANIMAL_C);
                dog->z.fixed = toFIXED(472);
            }
            if (pillar->z.fixed < dog->z.fixed)
                fdScriptSetFlag(FLAG_DOG_SHUT_SOUTH);
            else
                fdScriptSetFlag(FLAG_DOG_SHUT_NORTH);
        }
        else
        {
            fdActorSetSpritePriority(id, 1);
        }
    }
    if (id == ACTOR_GUARD_CRATE)
    {
        ACTOR *crate = fdActorGetById(ACTOR_GUARD_CRATE);
        ACTOR *guard;

        if (!fdScriptGetFlag(FLAG_GUARD_CRATE_DROPPED)
            && FIELD_METATILE(crate->x.fixed) == GUARD_CRATE_METATILE_X
            && FIELD_METATILE(crate->z.fixed) == GUARD_CRATE_METATILE_Z)
        {
            FIXED dx;

            scActorWaitToLand(ACTOR_GUARD_CRATE);
            fdActorSetSpritePriority(ACTOR_GUARD_CRATE, 2);
            guard = fdActorGetById(ACTOR_LOWER_GUARD);
            dx    = crate->x.fixed - guard->x.fixed;
            if (dx >= 0 ? dx < toFIXED(8) : guard->x.fixed - crate->x.fixed < toFIXED(8))
            {
                guard->motionFlags = 0;
                guard->speedLimit  = toFIXED(1);
                scFieldActorSetMoveTargetFX(guard, crate->x.fixed, crate->y.fixed, crate->z.fixed);
                scFieldActorWaitForMove(guard);
                fdActorStartScript(crate, KibomboMountainsCrateJumpScript);
                fdActorStartIdle(guard);
                guard->x.fixed = 0;
                guard->z.fixed = 0;
                fdScriptSetFlag(FLAG_GUARD_KNOCKED_OUT);
            }
            else
            {
                syWordsCopy(&saved, guard, sizeof(saved));
                fdActorResetTargetsAndVelocity(ACTOR_LOWER_GUARD);
                scFieldActorJump(ACTOR_LOWER_GUARD, 4, 0);
                syFieldScriptWait(30);
                fdActorShowEmote(ACTOR_LOWER_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
                fdActorFaceActor(ACTOR_LOWER_GUARD, ACTOR_GUARD_CRATE, 0);
                syFieldScriptWait(60);
                fdActorSetAnimationAndWait(ACTOR_LOWER_GUARD, ANIM_SHAKE_HEAD);
                syFieldScriptWait(20);
                syWordsCopy(guard, &saved, sizeof(saved));
                fdMapGridCopyMetatileAttributes(53, 35, 1, 1, 53, 36);
                fdScriptSetFlag(FLAG_GUARD_CRATE_DROPPED);
            }
        }
    }
}

// Felix casts Tremor on the crate with the bone on it, by the north end of the dog's run, facing it
// from the south or the west: the crate shakes, the bone drops to the ground, and the dog, unless
// shut in south of the pillar, runs to it and stays there eating, no longer watching the run.
void scKibomboMountainsTremorDropsBone(void)
{
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdActorSetDestination(ACTOR_BONE, 764, 384);
    fdActorMoveToAndWait(ACTOR_BONE_CRATE, 746, 380);
    fdActorGet(ACTOR_BONE)->velocityY = toFIXED(4);
    scFieldActorWaitForMoveById(ACTOR_BONE);
    fdActorGet(ACTOR_BONE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    if (!fdScriptGetFlag(FLAG_KIBOMBO_LETS_PARTY_PASS) && !fdScriptGetFlag(FLAG_DOG_SHUT_SOUTH))
    {
        scFieldActorStop(ACTOR_GUARD_DOG);
        fdActorMoveToAndWait(ACTOR_GUARD_DOG, 764, 388);
        fdActorSetDestination(ACTOR_BONE, 764, 376);
        fdActorWalkToAndWait(ACTOR_GUARD_DOG, 764, 380);
        fdActorSetAnimation(ACTOR_GUARD_DOG, 4);
        fdActorSetAttachedEffect(ACTOR_GUARD_DOG,
                                 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
        fdScriptSetFlag(FLAG_DOG_EATING_BONE);
    }
    fdScriptEnd();
}

// Felix casts Whirlwind on the bush on Waft's ledges: the bush blows away, and the opening behind
// it leads into the small cavern.
void scKibomboMountainsBlowAwayBush(void)
{
    syGameFlagSet(FLAG_KIBOMBO_BUSH_BLOWN_AWAY);
}

// Felix walks west off the edge at metatile (43, 30) on Waft's ledges and jumps the gap ahead.
void scKibomboMountainsJumpForward(void)
{
    fdTryJumpForward();
}

// Piers casts Frost on the puddle high in the north west of the chest room, metatile (4, 16): it
// freezes into an ice pillar Felix can hop across.
void scKibomboMountainsFreezeFirstPuddle(void)
{
    ACTOR *actor         = fdActorGet(ACTOR_FIRST_PUDDLE);

    actor->priorityFlags = 0;
    actor->motionFlags   = 0;
    fdMapGridCopyMetatileAttributes(6, 16, 1, 1, 4, 16);
    fdScriptSetFlag(FLAG_DISK_AXE_ROOM_FIRST_PUDDLE_FROZEN);
}

// Piers casts Frost on the puddle on the west path of the chest room, metatile (10, 30): it freezes
// into an ice pillar Felix can hop across.
void scKibomboMountainsFreezeSecondPuddle(void)
{
    ACTOR *actor         = fdActorGet(ACTOR_SECOND_PUDDLE);

    actor->priorityFlags = 0;
    actor->motionFlags   = 0;
    fdMapGridCopyMetatileAttributes(8, 30, 1, 1, 10, 30);
    fdScriptSetFlag(FLAG_DISK_AXE_ROOM_SECOND_PUDDLE_FROZEN);
}

// Nothing visible happens as Felix starts pushing one of the chest room's two pairs of crates: the
// pair on the first guard's ledge, (8, 18) and (19, 15), or the pair below the second guard, (20,
// 34) and (27, 33). The ground the pair stood on is laid back from the copy kept beside the map, so
// that the metatiles they leave are ordinary ground again.
void scKibomboMountainsStartPushingChestRoomCrate(Sint32 unused, Sint32 id)
{
    if (id == ACTOR_LOWER_WEST_CRATE || id == ACTOR_LOWER_EAST_CRATE)
        fdMapGridCopyMetatileAttributes(120, 33, 8, 2, 20, 33);
    if (id == ACTOR_WEST_CRATE || id == ACTOR_EAST_CRATE)
        fdMapGridCopyMetatileAttributes(112, 15, 16, 4, 8, 15);
}

// Copies the metatile at (srcX, srcY) of the map grid to the metatile actor `id` stands on.
void scKibomboMountainsSetCrateMetatile(Sint32 id, Sint32 srcX, Sint32 srcY)
{
    ACTOR *actor = fdActorGet(id);
    Sint32 x     = FIELD_METATILE(actor->x.fixed);
    Sint32 z     = FIELD_METATILE(actor->z.fixed);

    fdMapGridCopyMetatileAttributes(srcX, srcY, 1, 1, x, z);
}

// Felix finishes pushing a crate or pillar in the chest room. The pillar in the west, pushed off
// its ledge into the gap at metatile (4, 28), lands for good as a shortcut Felix can hop across;
// the pillar by the chest, dropped to metatile (28, 30), lands where he can climb it. Where the two
// crates below the second guard stop, Felix can hop across their tops.
void scKibomboMountainsFinishPushingChestRoomCrate(Sint32 unused, Sint32 id)
{
    if (id == ACTOR_FIRST_DROPPED_PILLAR)
    {
        ACTOR *actor = fdActorGet(ACTOR_FIRST_DROPPED_PILLAR);

        if (!fdScriptGetFlag(FLAG_KIBOMBO_FIRST_PILLAR_DROPPED))
        {
            Sint32 x = FIELD_METATILE(actor->x.fixed);

            if (x == 4)
            {
                scActorWaitToLand(ACTOR_FIRST_DROPPED_PILLAR);
                fdMapGridCopyMetatileAttributes(3, 28, 1, 1, x, 28);
                fdScriptSetFlag(FLAG_KIBOMBO_FIRST_PILLAR_DROPPED);
            }
        }
    }
    if (id == ACTOR_SECOND_DROPPED_PILLAR)
    {
        ACTOR *actor = fdActorGet(ACTOR_SECOND_DROPPED_PILLAR);

        if (!fdScriptGetFlag(FLAG_KIBOMBO_SECOND_PILLAR_DROPPED))
        {
            Sint32 x = FIELD_METATILE(actor->x.fixed);

            if (x == 28)
            {
                Sint32 z = FIELD_METATILE(actor->z.fixed);

                if (z == 30)
                {
                    scActorWaitToLand(ACTOR_SECOND_DROPPED_PILLAR);
                    fdMapGridCopyMetatileAttributes(32, 30, 1, 2, x, z);
                    fdScriptSetFlag(FLAG_KIBOMBO_SECOND_PILLAR_DROPPED);
                }
            }
        }
    }
    if (id == ACTOR_WEST_CRATE || id == ACTOR_EAST_CRATE)
    {
        scKibomboMountainsSetCrateMetatile(ACTOR_WEST_CRATE, 8, 17);
        scKibomboMountainsSetCrateMetatile(ACTOR_EAST_CRATE, 8, 17);
    }
    if (id == ACTOR_LOWER_WEST_CRATE || id == ACTOR_LOWER_EAST_CRATE)
    {
        scKibomboMountainsSetCrateMetatile(ACTOR_LOWER_WEST_CRATE, 22, 33);
        scKibomboMountainsSetCrateMetatile(ACTOR_LOWER_EAST_CRATE, 22, 33);
    }
}

extern CUE KibomboMountainsCues[];
extern CUE KibomboMountainsWaftCues[];
extern CUE KibomboMountainsDiskAxeChestCues[];
extern CUE KibomboMountainsCavernCues[];

const CUE *scGetCues(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_KIBOMBO_MOUNTAINS_WAFT)
        return KibomboMountainsWaftCues;
    if (scene == SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST)
        return KibomboMountainsDiskAxeChestCues;
    if (scene == SCENE_KIBOMBO_MOUNTAINS_CAVERN)
        return KibomboMountainsCavernCues;
    return KibomboMountainsCues;
}

void   scKibomboMountainsGuardsTurnPartyBack(void);

// Sets up the room the party enters: in Waft's room the puddle, vine, pillar and crate as this
// visit left them, the bone on its crate or in the dog's mouth, and the bush unless blown away; in
// the chest room the rope, the puddles, the dropped pillars and the crates' metatiles, and the
// warriors marching the party back if it was caught. Returns 0.
Sint32 scInit(void)
{
    ACTOR *actor;
    Sint16 scene;
    Sint32 flag;
    Sint32 height;

    FieldStateWork->transition                              = ROOM_TRANSITION;
    fdActorGetById(fdPartyGetLeaderActor())->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    scene                                                   = MainRoutineStateWork.next;
    if (scene == SCENE_KIBOMBO_MOUNTAINS_WAFT)
    {
        fdActorSetSpritePriority(ACTOR_WAFT, 1);
        fdActorSetSpritePriority(ACTOR_UNSEEN_MARKER, 1);
        fdActorSetSpriteFlags(fdActorGetById(ACTOR_UNSEEN_MARKER), 0);
        fdActorGetById(ACTOR_UNSEEN_MARKER)->collisionFlags  = 0;
        fdActorGetById(ACTOR_UNSEEN_MARKER)->priorityFlags  |= ACTOR_PRIORITY_UNDERFOOT;
        fdActorGetById(ACTOR_SPROUT)->collisionFlags         = 0;
        if (syGameFlagGet(FLAG_SPROUT_GROWN))
        {
            fdMapCopyMetatileAttributeRect(30, 35, 1, 1, 28, 34);
            fdMapCopyMetatileAttributeRect(30, 34, 1, 1, 28, 33);
        }
        if (syGameFlagGet(FLAG_WAFT_ROOM_PUDDLE_FROZEN))
            fdMapGridCopyMetatileAttributes(24, 32, 1, 1, 22, 32);
        if (syGameFlagGet(FLAG_DOG_PILLAR_DROPPED))
            fdMapGridCopyMetatileAttributes(49, 28, 1, 1, 47, 28);
        else
            fdActorSetSpritePriority(ACTOR_DOG_PILLAR, 1);
        if (syGameFlagGet(FLAG_GUARD_CRATE_DROPPED))
            fdMapGridCopyMetatileAttributes(53, 35, 1, 1, 53, 36);
        flag = syGameFlagGet(FLAG_DOG_EATING_BONE);
        if (flag == 0)
        {
            ACTOR *lift;

            fdActorGetById(ACTOR_BONE_CRATE)->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
            lift                                             = fdActorGetById(ACTOR_BONE);
            lift->priorityFlags                             |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
            lift->y.fixed                                    = toFIXED(224);
            lift->groundHeight                               = toFIXED(224);
            lift->collisionFlags                             = flag;
        }
        else
        {
            fdActorSetAttachedEffect(ACTOR_GUARD_DOG,
                                     ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
        }
        fdActorShowItemIconById(ACTOR_BONE, ITEM_BONE);
        if (syGameFlagGet(FLAG_KIBOMBO_BUSH_BLOWN_AWAY))
            fdActorSetPos(ACTOR_CAVERN_BUSH, 0, 0);
        else
            fdActorGetById(ACTOR_CAVERN_BUSH)->scaleY = toFIXED(5) / 4;
    }
    else if (scene == SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST)
    {
        fdActorSetSpritePriority(ACTOR_WEST_ROPE_POST, 1);
        fdActorSetSpritePriority(ACTOR_EAST_ROPE_POST, 1);
        fdRopeInit();
        scFieldRopePlace(0, FLAG_KIBOMBO_ROPE_TIED, ACTOR_WEST_ROPE_POST, ACTOR_EAST_ROPE_POST);
        if (syGameFlagGet(FLAG_DISK_AXE_ROOM_FIRST_PUDDLE_FROZEN))
            fdMapGridCopyMetatileAttributes(6, 16, 1, 1, 4, 16);
        if (syGameFlagGet(FLAG_DISK_AXE_ROOM_SECOND_PUDDLE_FROZEN))
            fdMapGridCopyMetatileAttributes(8, 30, 1, 1, 10, 30);
        if (syGameFlagGet(FLAG_KIBOMBO_FIRST_PILLAR_DROPPED))
        {
            actor = fdActorGetById(ACTOR_FIRST_DROPPED_PILLAR);
            fdActorSetPos(ACTOR_FIRST_DROPPED_PILLAR, 72, 456);
            actor->motionFlags = 0;
            height = fdMapGetTerrainHeight(MAPGRID_LAYER_GROUND, actor->x.fixed, actor->z.fixed);
            actor->groundHeight = height;
            actor->y.fixed      = height;
            fdMapGridCopyMetatileAttributes(3, 28, 1, 1, 4, 28);
        }
        if (syGameFlagGet(FLAG_KIBOMBO_SECOND_PILLAR_DROPPED))
        {
            actor = fdActorGetById(ACTOR_SECOND_DROPPED_PILLAR);
            fdActorSetPos(ACTOR_SECOND_DROPPED_PILLAR, 456, 488);
            actor->motionFlags = 0;
            height = fdMapGetTerrainHeight(MAPGRID_LAYER_GROUND, actor->x.fixed, actor->z.fixed);
            actor->groundHeight = height;
            actor->y.fixed      = height;
            fdMapGridCopyMetatileAttributes(32, 30, 1, 2, 28, 30);
        }
        fdMapGridCopyMetatileAttributes(8, 15, 16, 4, 112, 15);
        fdMapGridCopyMetatileAttributes(20, 33, 8, 2, 120, 33);
        scKibomboMountainsSetCrateMetatile(ACTOR_WEST_CRATE, 8, 17);
        scKibomboMountainsSetCrateMetatile(ACTOR_EAST_CRATE, 8, 17);
        scKibomboMountainsSetCrateMetatile(ACTOR_LOWER_WEST_CRATE, 22, 33);
        scKibomboMountainsSetCrateMetatile(ACTOR_LOWER_EAST_CRATE, 22, 33);
        if (!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)
            && MainRoutineStateWork.entrance == ENTRANCE_CAUGHT)
            scKibomboMountainsGuardsTurnPartyBack();
    }
    return 0;
}

Sint32 scPrepareMap(void)
{
    return 0;
}

// The first time Felix climbs the stairs between the two torches below the stone gateway the second
// guard keeps, metatiles (15, 33) to (16, 34): a warrior comes asking "Is the way clear?", the
// second guard reports that the strange one has not been seen, and the warrior hurries off to find
// whoever attacked their guards, leaving him on watch.
void scKibomboMountainsGuardsReport(void)
{
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    syFieldScriptSetMessageId(MsgKibomboMountainsIsWayClear);
    fdActorSetPos(ACTOR_WARRIOR_E, 312, 304);
    // "Is the way clear?"
    fdScriptShowMessage(ACTOR_WARRIOR_E, 0);
    fdCameraMoveTo(256, CAMERA_KEEP, 392, 1);
    fdCameraWaitForArrival();
    fdActorResetTargetsAndVelocity(ACTOR_SECOND_GUARD);
    fdActorFaceActor(ACTOR_SECOND_GUARD, ACTOR_WARRIOR_E, 0);
    fdActorWalkToAndWait(ACTOR_WARRIOR_E, 264, 312);
    fdActorWalkToAndWait(ACTOR_WARRIOR_E, 256, 400);
    fdActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
    // "This area is clear! The strange one has not been seen!"
    fdScriptShowMessage(ACTOR_SECOND_GUARD, 0);
    fdActorRunRepeatedMotion(ACTOR_SECOND_GUARD, 1);
    // "And there?"
    fdScriptShowMessage(ACTOR_SECOND_GUARD, 0);
    fdActorSetAnimationAndWait(ACTOR_WARRIOR_E, ANIM_SHAKE_HEAD);
    // "We have not seen the stranger near the village."
    fdScriptShowMessage(ACTOR_WARRIOR_E, 0);
    // "Do you believe he was truly headed toward Kibombo?"
    fdScriptShowMessage(ACTOR_WARRIOR_E, 0);
    fdActorShowEmote(ACTOR_SECOND_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "The guards here were only recently knocked unconscious."
    fdScriptShowMessage(ACTOR_SECOND_GUARD, 0);
    fdActorShowEmote(ACTOR_WARRIOR_E, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    // "Correct. We were under orders to maintain a strict watch over the..."
    fdScriptShowMessage(ACTOR_WARRIOR_E, 0);
    fdActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
    // "No one must be permitted into Kibombo until Akafubu has completed..."
    fdScriptShowMessage(ACTOR_SECOND_GUARD, 0);
    fdActorShowEmote(ACTOR_WARRIOR_E, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "Then we had better hurry to find the one who attacked our guards."
    fdScriptShowMessage(ACTOR_WARRIOR_E, 0);
    fdActorWalkToAndWait(ACTOR_WARRIOR_E, 264, 312);
    fdActorWalkTo(ACTOR_WARRIOR_E, 360, 312);
    fdActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
    // "Keep a good watch. Do not permit anyone to pass."
    fdScriptShowMessage(ACTOR_SECOND_GUARD, 0);
    scFieldActorWatchAndSetScript(ACTOR_SECOND_GUARD, MainRoutineStateWork.leaderActorId);
    syWait(30);
    // "If anyone interrupts Akafubu's ceremony, he will hold you responsible..."
    fdScriptShowMessage(ACTOR_SECOND_GUARD, 0);
    fdActorRunRepeatedMotion(ACTOR_SECOND_GUARD, 2);
    syFieldScriptWait(60);
    fdActorSetPos(ACTOR_WARRIOR_E, 0, 0);
    fdActorSetScript(ACTOR_SECOND_GUARD, KibomboMountainsSecondGuardPaceScript);
    fdScriptEnd();
    fdScriptSetFlag(FLAG_KIBOMBO_GUARDS_REPORTED);
}

// Sets an actor walking slowly, at half a pixel a tick.
void scKibomboMountainsWalkSlowly(Sint32 id)
{
    ACTOR *actor = fdActorGet(id);

    fdActorSelectAnimation(actor, ANIM_STAND);
    actor->acceleration = toFIXED(1) / 4;
    actor->speedLimit   = toFIXED(1) / 2;
}

// Caught for the first time on this visit, the party is back at the top of the stairs between two
// torches, below the second guard's stone gateway, surrounded by six warriors: "Here! The intruder
// is here! Kibombo does not permit visitors!" Two of them walk Felix down the stairs, and all of
// them leave.
void scKibomboMountainsGuardsTurnPartyBack(void)
{
    Sint32 leader = MainRoutineStateWork.leaderActorId;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdActorSetPos(ACTOR_WARRIOR_A, 248, 472);
    fdActorSetPos(ACTOR_WARRIOR_B, 264, 472);
    fdActorSetPos(ACTOR_WARRIOR_C, 235, 483);
    fdActorSetPos(ACTOR_WARRIOR_D, 232, 496);
    fdActorSetPos(ACTOR_WARRIOR_E, 277, 483);
    fdActorSetPos(ACTOR_WARRIOR_F, 280, 496);
    scKibomboMountainsWalkSlowly(leader);
    scKibomboMountainsWalkSlowly(ACTOR_WARRIOR_A);
    scKibomboMountainsWalkSlowly(ACTOR_WARRIOR_B);
    scKibomboMountainsWalkSlowly(ACTOR_WARRIOR_C);
    scKibomboMountainsWalkSlowly(ACTOR_WARRIOR_D);
    scKibomboMountainsWalkSlowly(ACTOR_WARRIOR_E);
    scKibomboMountainsWalkSlowly(ACTOR_WARRIOR_F);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    syFieldScriptSetMessageId(MsgKibomboMountainsHereIntruderIsHere);
    fdActorShowEmote(ACTOR_WARRIOR_E, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
    // "Here! The intruder is here!"
    fdScriptShowMessage(ACTOR_WARRIOR_E, 0);
    fdActorFaceActor(ACTOR_WARRIOR_E, ACTOR_WARRIOR_C, 0);
    syFieldScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_WARRIOR_C, ANIM_NOD);
    fdActorFaceActor(ACTOR_WARRIOR_E, leader, 0);
    syFieldScriptWait(10);
    fdActorShowEmote(ACTOR_WARRIOR_E, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    // "Kibombo does not permit visitors!"
    fdScriptShowMessage(ACTOR_WARRIOR_E, 0);
    fdActorSetAnimationAndWait(ACTOR_WARRIOR_E, ANIM_NOD);
    // "You just follow this road right back to where you came from!"
    fdScriptShowMessage(ACTOR_WARRIOR_E, 0);
    fdActorFaceActor(ACTOR_WARRIOR_C, ACTOR_WARRIOR_A, 0);
    fdActorShowEmote(ACTOR_WARRIOR_C, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    // "Guard! Get him out of here!"
    fdScriptShowMessage(ACTOR_WARRIOR_C, 0);
    fdActorWalkTo(ACTOR_WARRIOR_A, 248, 536);
    syFieldScriptWait(1);
    fdActorWalkTo(ACTOR_WARRIOR_B, 264, 536);
    scFieldActorWatchAndSetScript(ACTOR_WARRIOR_C, leader);
    scFieldActorWatchAndSetScript(ACTOR_WARRIOR_D, leader);
    scFieldActorWatchAndSetScript(ACTOR_WARRIOR_E, leader);
    scFieldActorWatchAndSetScript(ACTOR_WARRIOR_F, leader);
    syFieldScriptWait(2);
    fdActorGet(leader)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorWalkToAndWait(leader, 256, 552);
    syFieldScriptWait(1);
    fdActorGet(leader)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorSetAnimation(ACTOR_WARRIOR_A, ANIM_STAND);
    fdActorSetAnimation(ACTOR_WARRIOR_B, ANIM_STAND);
    syFieldScriptWait(20);
    fdActorSetAnimation(ACTOR_WARRIOR_A, ANIM_SHAKE_HEAD);
    syFieldScriptWait(3);
    fdActorSetAnimationAndWait(ACTOR_WARRIOR_B, ANIM_SHAKE_HEAD);
    syFieldScriptWait(20);
    fdActorWalkTo(ACTOR_WARRIOR_A, 248, 488);
    syFieldScriptWait(2);
    fdActorWalkToAndWait(ACTOR_WARRIOR_B, 264, 488);
    fdActorWalkTo(ACTOR_WARRIOR_A, 248, 440);
    fdActorWalkTo(ACTOR_WARRIOR_B, 264, 440);
    fdActorWalkTo(ACTOR_WARRIOR_C, 232, 456);
    fdActorWalkTo(ACTOR_WARRIOR_D, 232, 472);
    fdActorWalkTo(ACTOR_WARRIOR_E, 280, 456);
    fdActorWalkToAndWait(ACTOR_WARRIOR_F, 280, 472);
    fdActorSetPos(ACTOR_WARRIOR_A, 0, 0);
    fdActorSetPos(ACTOR_WARRIOR_B, 0, 0);
    fdActorSetPos(ACTOR_WARRIOR_C, 0, 0);
    fdActorSetPos(ACTOR_WARRIOR_D, 0, 0);
    fdActorSetPos(ACTOR_WARRIOR_E, 0, 0);
    fdActorSetPos(ACTOR_WARRIOR_F, 0, 0);
    fdScriptEnd();
    fdScriptSetFlag(FLAG_TURNED_BACK);
}

// Sends the party back to the stairs below the second guard's gateway: the first time on a visit to
// be met by the warriors, after that only put back.
void scKibomboMountainsSendPartyBack(void)
{
    Sint32 moved = fdScriptGetFlag(FLAG_TURNED_BACK);
    Sint32 scene = SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST;

    if (moved)
        fdScriptSetDestination(scene, ENTRANCE_CAUGHT_AGAIN);
    else
        fdScriptSetDestination(scene, ENTRANCE_CAUGHT);
}

// A guard sees Felix: he starts, turns to face him, "Guards! An intruder! Someone is trying to
// pass! To arms, Kibombo warriors! To arms!", and the party is sent back to the stairs below the
// second guard's gateway.
void scKibomboMountainsGuardSpotsParty(Sint32 id)
{
    Sint32 leader = MainRoutineStateWork.leaderActorId;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdActorResetTargetsAndVelocity(id);
    fdActorShowEmote(id, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    fdActorFaceActor(id, leader, 20);
    syFieldScriptSetMessageId(MsgKibomboMountainsGuardsIntruder);
    // "Guards! An intruder!"
    fdScriptShowMessage(id, 0);
    fdActorSetAttachedEffect(leader, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    fdActorSetAnimation(id, ANIM_SHAKE_HEAD);
    syFieldScriptWait(30);
    // "Someone is trying to pass! To arms, Kibombo warriors! To arms!"
    fdScriptShowMessage(id, 0);
    fdActorSetAttachedEffect(leader, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    scKibomboMountainsSendPartyBack();
    fdScriptEnd();
}

// The guard dog sees Felix: it starts, "Grrrrr! Rrrruff! Rrufff!", the upper guard shouts "An
// intruder! Repel the intruder immediately!", and the party is sent back to the stairs below the
// second guard's gateway.
void scKibomboMountainsDogSpotsParty(Sint32 id)
{
    Sint32 leader = MainRoutineStateWork.leaderActorId;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    scFieldActorStop(id);
    fdActorShowEmote(id, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    fdActorFaceActor(id, leader, 20);
    syFieldScriptSetMessageId(MsgKibomboMountainsGrrrrrRrrruffRrufff);
    // "Grrrrr! Rrrruff! Rrufff!"
    fdScriptShowMessage(id, 0);
    fdActorSetAttachedEffect(leader, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    fdActorSetAnimation(id, 4);
    syFieldScriptWait(30);
    // "An intruder! Repel the intruder immediately!"
    fdScriptShowMessage(ACTOR_UPPER_GUARD, 0);
    fdActorSetAttachedEffect(leader, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    scKibomboMountainsSendPartyBack();
    fdScriptEnd();
}

// Felix steps onto the first guard's ledge west of where he paces, metatiles (8, 18) to (10, 19):
// the guard sees him and the party is sent back, unless the west crate stands just east of Felix in
// the same row, hiding him.
void scKibomboMountainsStepWestOfFirstGuard(void)
{
    ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
    ACTOR *crate  = fdActorGet(ACTOR_WEST_CRATE);

    if ((crate->z.fixed - leader->z.fixed >= 0 ? crate->z.fixed - leader->z.fixed
                                               : leader->z.fixed - crate->z.fixed)
            < toFIXED(16)
        && crate->x.fixed > leader->x.fixed)
        return;
    scKibomboMountainsGuardSpotsParty(ACTOR_FIRST_GUARD);
}

// Felix steps onto the first guard's ledge east of where he paces, metatiles (16, 18) to (19, 18):
// the guard sees him and the party is sent back, unless the east crate stands just west of Felix in
// the same row, hiding him.
void scKibomboMountainsStepEastOfFirstGuard(void)
{
    ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
    ACTOR *crate  = fdActorGet(ACTOR_EAST_CRATE);

    if ((crate->z.fixed - leader->z.fixed >= 0 ? crate->z.fixed - leader->z.fixed
                                               : leader->z.fixed - crate->z.fixed)
            < toFIXED(16)
        && crate->x.fixed < leader->x.fixed)
        return;
    scKibomboMountainsGuardSpotsParty(ACTOR_FIRST_GUARD);
}

// Felix steps east of the second guard, metatiles (18, 28) to (18, 30): the guard sees him and the
// party is sent back.
void scKibomboMountainsStepEastOfSecondGuard(void)
{
    scKibomboMountainsGuardSpotsParty(ACTOR_SECOND_GUARD);
}

// Felix steps south of the second guard, metatiles (14, 30) to (17, 32), in front of where he
// looks: the guard sees him and the party is sent back.
void scKibomboMountainsStepSouthOfSecondGuard(void)
{
    scKibomboMountainsGuardSpotsParty(ACTOR_SECOND_GUARD);
}

// Felix steps onto the line of the rope above the third guard, metatiles (26,
// 12) and (27, 12): walking the rope he is not seen, but anywhere else there
// the guard sees him and the party is sent back.
void scKibomboMountainsStepOnRopeAboveThirdGuard(void)
{
    if (MainRoutineStateWork.movementMode != LEADER_MOVEMENT_ROPE)
        scKibomboMountainsGuardSpotsParty(ACTOR_THIRD_GUARD);
}

// Felix steps west of the third guard, metatiles (25, 15), (24, 16) and (25, 16): the guard sees
// him and the party is sent back.
void scKibomboMountainsStepWestOfThirdGuard(void)
{
    scKibomboMountainsGuardSpotsParty(ACTOR_THIRD_GUARD);
}

// Felix steps south west of the upper guard on Waft's ledges, metatiles (35,
// 21) to (35, 23): the guard sees him and the party is sent back.
void scKibomboMountainsStepWestOfUpperGuard(void)
{
    scKibomboMountainsGuardSpotsParty(ACTOR_UPPER_GUARD);
}

// Felix steps south east of the upper guard, metatiles (41, 23) to (41, 26): the guard sees him and
// the party is sent back.
void scKibomboMountainsStepEastOfUpperGuard(void)
{
    scKibomboMountainsGuardSpotsParty(ACTOR_UPPER_GUARD);
}

// Felix steps by the north end of the dog's run, metatiles (49, 22) to (49, 24), (45, 24) and (45,
// 25): the dog sees him and barks, and the party is sent back, unless the dog is eating the bone or
// shut in south of the pillar.
void scKibomboMountainsStepByNorthEndOfDogRun(void)
{
    if (!fdScriptGetFlag(FLAG_DOG_EATING_BONE) && !fdScriptGetFlag(FLAG_DOG_SHUT_SOUTH))
        scKibomboMountainsDogSpotsParty(ACTOR_GUARD_DOG);
}

// Felix steps south of the lower guard's path, metatiles (51, 36) to (51, 38) and (47, 38) to (47,
// 40): the guard sees him and the party is sent back, unless the crate has trapped him.
void scKibomboMountainsStepSouthOfLowerGuard(void)
{
    if (!fdScriptGetFlag(FLAG_GUARD_KNOCKED_OUT))
        scKibomboMountainsGuardSpotsParty(ACTOR_LOWER_GUARD);
}

// Felix steps south east of the lower guard's path, metatiles (54, 38) to (55, 41): the guard sees
// him and the party is sent back, unless the crate has trapped him.
void scKibomboMountainsStepSouthEastOfLowerGuard(void)
{
    if (!fdScriptGetFlag(FLAG_GUARD_KNOCKED_OUT))
        scKibomboMountainsGuardSpotsParty(ACTOR_LOWER_GUARD);
}

// Felix steps by the south end of the dog's run, metatiles (50, 30) to (50, 32): the dog sees him
// and barks, and the party is sent back, unless the dog is eating the bone or shut in north of the
// pillar.
void scKibomboMountainsStepBySouthEndOfDogRun(void)
{
    if (!fdScriptGetFlag(FLAG_DOG_SHUT_NORTH) && !fdScriptGetFlag(FLAG_DOG_EATING_BONE))
        scKibomboMountainsDogSpotsParty(ACTOR_GUARD_DOG);
}

extern Sint32 KibomboMountainsFirstGuardPaceScript[];

// clang-format off
// A torch burning.
Sint32 KibomboMountainsTorchScript[] = {
    SC_ANIMATION(2),
    SC_END,
};

// Unused: a flame raised 24 pixels and held there, as Naribwe's NaribweFlameRises raises the flames over its braziers.
UNUSED Sint32 KibomboMountainsFlameRisesScript[] = {
    SC_ANIMATION(0),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(24)),
    SC_END,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
    SCENE_EXITS(KibomboMountainsWaft),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KIBOMBO_MOUNTAINS_WAFT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_A, KibomboMountainsDiskAxeChest, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_FROM_KIBOMBO_MOUNTAINS_WAFT_EAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_B, KibomboMountainsDiskAxeChest, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_FROM_KIBOMBO_MOUNTAINS_WAFT_EAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_WEST, KibomboMountainsDiskAxeChest, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_FROM_KIBOMBO_MOUNTAINS_WAFT_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_NORTH, KibomboMountainsCavern, KIBOMBO_MOUNTAINS_CAVERN_FROM_KIBOMBO_MOUNTAINS_WAFT_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_A, KibomboMountainsCavern, KIBOMBO_MOUNTAINS_CAVERN_FROM_KIBOMBO_MOUNTAINS_WAFT_SOUTH_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_B, KibomboMountainsCavern, KIBOMBO_MOUNTAINS_CAVERN_FROM_KIBOMBO_MOUNTAINS_WAFT_SOUTH_B, EXIT_FLAG_NONE),
    SCENE_EXITS(KibomboMountainsDiskAxeChest),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_EAST_A, KibomboMountainsWaft, KIBOMBO_MOUNTAINS_WAFT_FROM_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_EAST_B, KibomboMountainsWaft, KIBOMBO_MOUNTAINS_WAFT_FROM_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_WEST, KibomboMountainsWaft, KIBOMBO_MOUNTAINS_WAFT_FROM_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_WEST, EXIT_FLAG_NONE),
    SCENE_EXITS(KibomboMountainsCavern),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_NORTH, KibomboMountainsWaft, KIBOMBO_MOUNTAINS_WAFT_FROM_KIBOMBO_MOUNTAINS_CAVERN_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_SOUTH_A, KibomboMountainsWaft, KIBOMBO_MOUNTAINS_WAFT_FROM_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_SOUTH_B, KibomboMountainsWaft, KIBOMBO_MOUNTAINS_WAFT_FROM_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_B, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// A crate or pillar held still until it is pushed.
Sint32 KibomboMountainsCrateScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

// The mountain path's actors.
ACTOR_PLACE KibomboMountainsPlacements[] = {
    {SCENE_TABLE_END},
};

// A guard keeping watch where he stands, looking about to the south.
Sint32 KibomboMountainsWatchScript[] = {
    SC_SPEED(0.5),
    SC_ACCELERATION(0.1),
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

// The lower guard pacing his path, looking about to the south at each end.
Sint32 KibomboMountainsPaceScript[] = {
    SC_SPEED(0.5),
    SC_ACCELERATION(0.1),
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(856, 0, 584),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(3),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(792, 0, 584),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(3),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

// The crate over the knocked-out guard, jumping and rocking as he struggles inside.
Sint32 KibomboMountainsCrateJumpScript[] = {
    SC_CALL(scActorBob),
    SC_END,
};

// The guard dog running up and down its path, hopping as it goes.
Sint32 KibomboMountainsDogRunScript[] = {
    SC_SPEED(1.25),
    SC_ACCELERATION(0.25),
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(3),
    SC_SET(SW_VELOCITY_Y, toFIXED(3)),
    SC_MOVE(760, 0, 488),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(764, 0, 472),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(760, 0, 456),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(764, 0, 440),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(760, 0, 424),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(764, 0, 408),
    SC_SYNC,
    SC_MOVE(760, 0, 392),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(3)),
    SC_MOVE(764, 0, 408),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(760, 0, 424),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(764, 0, 440),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(760, 0, 456),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(764, 0, 472),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(760, 0, 488),
    SC_SYNC,
    SC_MOVE(764, 0, 504),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

// Waft's ledges' actors, by KIBOMBO_MOUNTAINS_WAFT_ACTOR_ID.
ACTOR_PLACE KibomboMountainsWaftPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_KIBOMBO_LETS_PARTY_PASS, KibomboMountainsWatchScript, 616, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_KIBOMBO_LETS_PARTY_PASS, KibomboMountainsPaceScript, 824, 0, 584, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, FLAG_KIBOMBO_LETS_PARTY_PASS, KibomboMountainsDogRunScript, 760, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 520, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, KibomboMountainsCrateScript, 712, 0, 472, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, KibomboMountainsCrateScript, 888, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 732, 0, 376, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 732, 0, 376, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_WAFT), MOVEMENT_TYPE_STAND, 440, 0, 472, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 584, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The first guard pacing his ledge, looking about to the south at each end.
Sint32 KibomboMountainsFirstGuardPaceScript[] = {
    SC_SPEED(0.5),
    SC_ACCELERATION(0.1),
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(184, 0, 296),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(3),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(248, 0, 296),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(3),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

// The second guard pacing his ledge, looking about to the south at each end.
Sint32 KibomboMountainsSecondGuardPaceScript[] = {
    SC_SPEED(0.5),
    SC_ACCELERATION(0.1),
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(248, 0, 448),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(3),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(264, 0, 448),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(3),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

// The chest room's actors, by KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_ACTOR_ID.
ACTOR_PLACE KibomboMountainsDiskAxeChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_KIBOMBO_LETS_PARTY_PASS, KibomboMountainsFirstGuardPaceScript, 200, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_KIBOMBO_LETS_PARTY_PASS, KibomboMountainsSecondGuardPaceScript, 248, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_KIBOMBO_LETS_PARTY_PASS, KibomboMountainsWatchScript, 424, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VINE_PILLAR, CONDITION_ALWAYS, KibomboMountainsCrateScript, 456, 0, 472, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, KibomboMountainsCrateScript, 136, 0, 296, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, KibomboMountainsCrateScript, 312, 0, 248, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, KibomboMountainsCrateScript, 328, 0, 552, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, KibomboMountainsCrateScript, 440, 0, 536, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 264, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 488, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, KibomboMountainsCrateScript, 88, 0, 456, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, KibomboMountainsTorchScript, 232, 0, 536, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, KibomboMountainsTorchScript, 280, 0, 536, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, KibomboMountainsWatchScript, 0, 0, 0, FACING_SOUTH - 256, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, KibomboMountainsPaceScript, 0, 0, 0, FACING_SOUTH + 256, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, KibomboMountainsWatchScript, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, KibomboMountainsPaceScript, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, KibomboMountainsWatchScript, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, KibomboMountainsPaceScript, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The mountain path's cues.
CUE KibomboMountainsCues[] = {
    {CUE_TABLE_END},
};

// The cues of Waft's ledges.
CUE KibomboMountainsWaftCues[] = {
    {CUE_EXIT, KIBOMBO_MOUNTAINS_WAFT_TO_WORLD_MAP, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_WAFT_TO_WORLD_MAP},
    {CUE_EXIT, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_A, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_A},
    {CUE_EXIT, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_B, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_B},
    {CUE_EXIT, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_WEST, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_WEST},
    {CUE_EXIT | STEP_CUE_FLAG_WALKING | STEP_CUE_FLAG_QUICK | CUE_FLAG_FACING | FACING_NORTH, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_NORTH, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_NORTH},
    {CUE_EXIT | STEP_CUE_FLAG_WALKING | STEP_CUE_FLAG_QUICK | CUE_FLAG_FACING | FACING_NORTH, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_A, IF_FLAG_SET(FLAG_KIBOMBO_BUSH_BLOWN_AWAY), KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_A},
    {CUE_EXIT | STEP_CUE_FLAG_WALKING | STEP_CUE_FLAG_QUICK | CUE_FLAG_FACING | FACING_NORTH, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_B, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_B},
    {CUE_STEP, METATILE_BEHAVIOR_WEST_OF_UPPER_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, SCRIPT_REF(scKibomboMountainsStepWestOfUpperGuard)},
    {CUE_STEP, METATILE_BEHAVIOR_EAST_OF_UPPER_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, SCRIPT_REF(scKibomboMountainsStepEastOfUpperGuard)},
    {CUE_STEP, METATILE_BEHAVIOR_NORTH_END_OF_DOG_RUN, FLAG_KIBOMBO_LETS_PARTY_PASS, SCRIPT_REF(scKibomboMountainsStepByNorthEndOfDogRun)},
    {CUE_STEP, METATILE_BEHAVIOR_SOUTH_OF_LOWER_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, SCRIPT_REF(scKibomboMountainsStepSouthOfLowerGuard)},
    {CUE_STEP, METATILE_BEHAVIOR_SOUTH_EAST_OF_LOWER_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, SCRIPT_REF(scKibomboMountainsStepSouthEastOfLowerGuard)},
    {CUE_STEP, METATILE_BEHAVIOR_SOUTH_END_OF_DOG_RUN, FLAG_KIBOMBO_LETS_PARTY_PASS, SCRIPT_REF(scKibomboMountainsStepBySouthEndOfDogRun)},
    {STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_JUMP_WEST, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsJumpForward)},
    {CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsStartPushingGuardCrate)},
    {CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsDropOffLedge)},
    {CUE_TALK, ACTOR_GUARD_DOG, IF_FLAG_SET(FLAG_DOG_EATING_BONE), MSGNO(MsgKibomboMountainsSniffSniffLickLickLick)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_DOG, IF_FLAG_SET(FLAG_DOG_EATING_BONE), MSGNO(MsgKibomboMountainsBoneThatsMyFavruffLick)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_DOG, CONDITION_ALWAYS, MSGNO(MsgKibomboMountainsThisRock)},
    {CUE_TALK, ACTOR_GUARD_CRATE, IF_FLAG_SET(FLAG_GUARD_KNOCKED_OUT), MSGNO(MsgKibomboMountainsHeyWhosThereGetMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_CRATE, IF_FLAG_SET(FLAG_GUARD_KNOCKED_OUT), MSGNO(MsgKibomboMountainsItsDarkItsCrampedSomebody)},
    {CUE_TALK, ACTOR_WAFT, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsFightWaft)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_WAFT_ROOM_PUDDLE_FROZEN, SCRIPT_REF(scKibomboMountainsFreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_SPROUT_GROWN, SCRIPT_REF(scKibomboMountainsGrowVine)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_CAVERN_BUSH, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsBlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOG_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsDropOffLedge)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_GUARD_CRATE, FLAG_GUARD_KNOCKED_OUT, SCRIPT_REF(scKibomboMountainsStartPushingGuardCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GUARD_CRATE, FLAG_GUARD_KNOCKED_OUT, SCRIPT_REF(scKibomboMountainsDropOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), FACING_SUBJECT(FACING_NORTH, ACTOR_BONE_CRATE), FLAG_DOG_EATING_BONE, SCRIPT_REF(scKibomboMountainsTremorDropsBone)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), FACING_SUBJECT(FACING_EAST, ACTOR_BONE_CRATE), FLAG_DOG_EATING_BONE, SCRIPT_REF(scKibomboMountainsTremorDropsBone)},
    {CUE_TABLE_END},
};

// The chest room's cues.
CUE KibomboMountainsDiskAxeChestCues[] = {
    {CUE_EXIT, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_WORLD_MAP, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_WORLD_MAP},
    {CUE_EXIT, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_EAST_A, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_EAST_A},
    {CUE_EXIT, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_EAST_B, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_EAST_B},
    {CUE_EXIT, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_WEST, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_WEST},
    {CUE_STEP, METATILE_BEHAVIOR_WEST_OF_FIRST_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, SCRIPT_REF(scKibomboMountainsStepWestOfFirstGuard)},
    {CUE_STEP, METATILE_BEHAVIOR_EAST_OF_FIRST_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, SCRIPT_REF(scKibomboMountainsStepEastOfFirstGuard)},
    {CUE_STEP, METATILE_BEHAVIOR_EAST_OF_SECOND_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, SCRIPT_REF(scKibomboMountainsStepEastOfSecondGuard)},
    {CUE_STEP, METATILE_BEHAVIOR_SOUTH_OF_SECOND_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, SCRIPT_REF(scKibomboMountainsStepSouthOfSecondGuard)},
    {CUE_STEP, METATILE_BEHAVIOR_BETWEEN_TORCHES, FLAG_KIBOMBO_GUARDS_REPORTED, SCRIPT_REF(scKibomboMountainsGuardsReport)},
    {CUE_STEP, METATILE_BEHAVIOR_ROPE_ABOVE_THIRD_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, SCRIPT_REF(scKibomboMountainsStepOnRopeAboveThirdGuard)},
    {CUE_STEP, METATILE_BEHAVIOR_WEST_OF_THIRD_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, SCRIPT_REF(scKibomboMountainsStepWestOfThirdGuard)},
    {CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsStartPushingChestRoomCrate)},
    {CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsFinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_WEST_ROPE_POST, FLAG_KIBOMBO_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE, FLAG_DISK_AXE_ROOM_FIRST_PUDDLE_FROZEN, SCRIPT_REF(scKibomboMountainsFreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE, FLAG_DISK_AXE_ROOM_SECOND_PUDDLE_FROZEN, SCRIPT_REF(scKibomboMountainsFreezeSecondPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WEST_TORCH, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_EAST_TORCH, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_DROPPED_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsFinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_DROPPED_PILLAR, FLAG_KIBOMBO_FIRST_PILLAR_DROPPED, SCRIPT_REF(scKibomboMountainsFinishPushingChestRoomCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WEST_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsStartPushingChestRoomCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_EAST_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsStartPushingChestRoomCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LOWER_WEST_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsStartPushingChestRoomCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LOWER_EAST_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsStartPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsFinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsFinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOWER_WEST_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsFinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOWER_EAST_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scKibomboMountainsFinishPushingChestRoomCrate)},
    {CUE_TABLE_END},
};

// The small cavern's cues.
CUE KibomboMountainsCavernCues[] = {
    {CUE_EXIT, KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_NORTH, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_NORTH},
    {CUE_EXIT, KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_SOUTH_A, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_SOUTH_A},
    {CUE_EXIT, KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_SOUTH_B, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_SOUTH_B},
    {CUE_TABLE_END},
};
// clang-format on
