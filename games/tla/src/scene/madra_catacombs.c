// The Madra Catacombs, a deserted village in a cave under the town. Madra's two ladders, the one
// inside the walls and the one in the Champa's hole, come down into a small ladder room, and the
// tunnel the Champa dug runs from there to the village. On the first visit Felix comes in along the
// tunnel's upper path, Moves a pillar off a ledge into a pit to cross it, and takes the Tremor Bit
// from the village's broken building. Once Briggs is caught that way is blocked off, and the
// village is reached from the south, through a golden door whose push button only Reveal shows.
// Inside, Frost freezes a puddle, Move shifts two pillars to climb by, Lash ties a rope across, and
// Tremor rocks a bookshelf on the building's upper floor until the chest on top falls through to
// the floor below; it holds the Ruin Key, which opens the red double door down the stairs, lighting
// the braziers before Moloch's tablet.

#include "item_ids.h"
#include "field/script_inline.h"
#include "field/field.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "game/game.h"
#include "text/msg_ids.h"

// The ropes Lash ties, the tunnel's and the village's, which stay tied until the party leaves.
#define FLAG_TUNNEL_ROPE_TIED                   FLAG_SCENE_TEMP_00
#define FLAG_VILLAGE_ROPE_TIED                  FLAG_SCENE_TEMP_01
// Frost on the village's puddle, which stays a pillar of ice until the party leaves.
#define FLAG_VILLAGE_PUDDLE_FROZEN              FLAG_SCENE_TEMP_02
// Set while the bookshelf leans as Tremor shakes it, so that the chest on it is nudged once each
// time it comes upright.
#define FLAG_BOOKSHELF_LEANING                  FLAG_SCENE_TEMP_04

// Set once the pillar Move pushed onto metatile (40, 13) has dropped into the pit in the village's
// northern ledge and filled it.
#define FLAG_MADRA_CATACOMBS_HOLE_FILLED        0x8c8
// Set once the Ruin Key has opened the red double door before Moloch's tablet; its braziers burn
// from then on.
#define FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN   0x8c9
// Set once Move has left the village's plain stone pillar on metatile (44, 26).
#define FLAG_MADRA_CATACOMBS_PILLAR_PLACED      0x8ca
// Set once the push button Reveal shows has opened the village's golden door.
#define FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN  0x8cb
// Set once Tremor has shaken the chest off the bookshelf into the room below.
#define FLAG_MADRA_CATACOMBS_CHEST_FELL         0x8cc
// Set once Move has left the vine-covered pillar on metatile (35, 17).
#define FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED 0x9ef
// Set while Reveal shows the push button by the closed golden door.
#define FLAG_MADRA_CATACOMBS_BUTTON_REVEALED    FLAG_SCENE_TEMP_03

// The deserted village's actors that its scripts move.
typedef enum MADRA_CATACOMBS_VILLAGE_ACTOR_ID
{
    // The vine-covered pillar Move carries onto the upper level to climb by.
    ACTOR_VINE_PILLAR = ACTOR_FIRST_PLACED,
    // The posts at the two ends of the rope Lash makes taut.
    ACTOR_VILLAGE_ROPE_LOW_POST,
    ACTOR_VILLAGE_ROPE_HIGH_POST,
    // The pillar on the northern ledge that Move pushes into the pit.
    ACTOR_HOLE_PILLAR = ACTOR_FIRST_PLACED + 3,
    // The plain stone pillar Move pushes onto metatile (44, 26).
    ACTOR_PUSHED_PILLAR,
    // The puddle Frost freezes into a pillar.
    ACTOR_VILLAGE_PUDDLE,
    // The chest by the rope, the map's first treasure spot, drawn over the ledge.
    ACTOR_VILLAGE_CHEST = FIELD_TREASURE_ACTOR
} MADRA_CATACOMBS_VILLAGE_ACTOR_ID;

// The tunnel the Champa dug (MadraCatacombsChampaTunnel), by the rows of its table.
typedef enum MADRA_CATACOMBS_TUNNEL_ACTOR_ID
{
    // The posts at the two ends of the tunnel's rope.
    ACTOR_TUNNEL_ROPE_EAST_POST = ACTOR_FIRST_PLACED,
    ACTOR_TUNNEL_ROPE_WEST_POST,
    // A man in the tunnel until Briggs is caught: "I can't believe the Champa just stumbled onto
    // this cave accidentally..."
    ACTOR_MAN_ON_THE_CHAMPAS_CAVE
} MADRA_CATACOMBS_TUNNEL_ACTOR_ID;

// The far end of the Champa tunnel's upper path, metatile (51, 13): the number of both the tunnel's
// entrance there and its exit back the way the party came.
enum
{
    CHAMPA_TUNNEL_UPPER_PATH_END = 8
};

// The metatiles whose cues run the catacombs' scripts.
typedef enum MADRA_CATACOMBS_METATILE_BEHAVIOR
{
    // The red double door before Moloch's tablet, metatiles (9, 22) and (10, 22).
    METATILE_BEHAVIOR_MADRA_CATACOMBS_TABLET_DOOR      = 10,
    // Metatile (33, 17), west of the vine-covered pillar's place.
    METATILE_BEHAVIOR_MADRA_CATACOMBS_VINE_PILLAR_PUSH = 11,
    // The village's wall east of the golden door, metatile (36, 39), where Reveal shows the push
    // button; the village's only metatile of this behavior.
    METATILE_BEHAVIOR_MADRA_CATACOMBS_BUTTON_WALL      = 10
} MADRA_CATACOMBS_METATILE_BEHAVIOR;

// The braziers that flare up down each side of the hall before Moloch's tablet.
#define FLARES_PER_SIDE 5

// The tablet hall's actors: the two halves of the red double door, the brazier flames that flare up
// down each side as it opens, numbered from the tablet down to the door, and an unseen marker that
// stands on each flame once they burn.
typedef enum MADRA_CATACOMBS_TABLET_ROOM_ACTOR_ID
{
    ACTOR_LEFT_DOOR = ACTOR_FIRST_PLACED,
    ACTOR_RIGHT_DOOR,
    ACTOR_FIRST_WEST_FLARE,
    ACTOR_FIRST_EAST_FLARE   = ACTOR_FIRST_WEST_FLARE + FLARES_PER_SIDE,
    ACTOR_FIRST_FLAME_MARKER = ACTOR_FIRST_EAST_FLARE + FLARES_PER_SIDE
} MADRA_CATACOMBS_TABLET_ROOM_ACTOR_ID;

// The bookshelf on the broken building's upper floor, and the chest on top of it that Tremor shakes
// off; the room below places the fallen chest.
typedef enum MADRA_CATACOMBS_BOOKSHELF_ACTOR_ID
{
    ACTOR_BOOKSHELF   = ACTOR_FIRST_PLACED,
    ACTOR_SHELF_CHEST = FIELD_TREASURE_ACTOR,
    ACTOR_FALLEN_CHEST
} MADRA_CATACOMBS_BOOKSHELF_ACTOR_ID;

struct ACTOR;

extern SCENE_ENTRANCE SceneEntrances[];
extern SCENE_ENTRANCE MadraCatacombsStoneDoorEntrances[];

extern SCENE_ENTRANCE MadraCatacombsBookshelfEntrances[];

extern SCENE_REGION   MadraCatacombsLadderRoomRegions[];

extern ACTOR_PLACE    MadraCatacombsPlacements[];

extern ACTOR_PLACE    MadraCatacombsEntrancePlacements[];

extern ACTOR_PLACE    MadraCatacombsChampaTunnelPlacements[];

extern ACTOR_PLACE    MadraCatacombsBookshelfPlacements[];
extern ACTOR_PLACE    MadraCatacombsTabletPlacements[];
extern CUE            MadraCatacombsEntranceCues[];
extern CUE            MadraCatacombsLaddersCues[];
extern CUE            MadraCatacombsChampaTunnelCues[];
extern CUE            MadraCatacombsStoneDoorCues[];
extern CUE            MadraCatacombsBookshelfCues[];
extern CUE            MadraCatacombsTabletCues[];

void                  scMadraCatacombsSetUpVillage(void);

void                  scMadraCatacombsSetUpChampaTunnel(void);

void                  scMadraCatacombsSetUpBookshelfRoom(void);

void                  scMadraCatacombsSetUpTabletRoom(void);

extern Sint16         MadraCatacombsPushedPillar[];
extern Sint16         MadraCatacombsPuddles[];

extern Sint32         MadraCatacombsFlameFlaresUp[];

extern Sint32         MadraCatacombsGreatFlameFlaresUp[];
extern Sint16         MadraCatacombsVinePillar[];
extern Sint32         LeaderWalkThroughDoorScript[];

void                  scMadraCatacombsRockChestWithBookshelf(void);

// Arriving in the village from the Champa tunnel's west end once that way is blocked off: Felix
// vanishes and the screen fades back to the tunnel.
void                  scMadraCatacombsLeaveBlockedVillage(void);

#include "shared/field_rope_place.c"

// Raises each listed actor's metatiles: the shape grid and the metatile flags under it, then lifts
// the leader if the new terrain is above the leader.
#include "shared/pillar_raise.h"

#include "shared/pillar_lower_held_metatiles.h"

#include "shared/held_actor_raise_metatile.h"

extern CUE            MadraCatacombsCues[];

void                  scMadraCatacombsSetUpStoneDoorRoom(void);

const SCENE_ENTRANCE *scGetEntrances(void)
{
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
        return MadraCatacombsStoneDoorEntrances;
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
        return MadraCatacombsBookshelfEntrances;
    return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_MADRA_CATACOMBS_LADDERS)
        return MadraCatacombsLadderRoomRegions;
    return 0;
}

const Uint32 *scGetExits(void)
{
    return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
        return MadraCatacombsEntrancePlacements;
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
        return MadraCatacombsChampaTunnelPlacements;
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
        return MadraCatacombsBookshelfPlacements;
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_TABLET)
        return MadraCatacombsTabletPlacements;
    return MadraCatacombsPlacements;
}

const CUE *scGetCues(void)
{
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
        return MadraCatacombsEntranceCues;
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_LADDERS)
        return MadraCatacombsLaddersCues;
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
        return MadraCatacombsChampaTunnelCues;
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
        return MadraCatacombsStoneDoorCues;
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
        return MadraCatacombsBookshelfCues;
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_TABLET)
        return MadraCatacombsTabletCues;
    return MadraCatacombsCues;
}

// Examining the red double door at the foot of the hall before Moloch's tablet (metatiles (9, 22)
// and (10, 22)) before the Ruin Key has opened it: "The door won't budge. ...Must be locked."
void scMadraCatacombsTryLockedDoor(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    // "The door won't budge. ...Must be locked."
    mnFieldMessageShow((Sint32)&MsgFieldDoorWontBudge, 1);
    state->fieldMenu = 1;
    fdScriptEnd();
}

#include "shared/actor_drop_and_wait.h"

// Felix Moves the pillar on the village's northern ledge one metatile east, off the edge (onto
// metatile (40, 13)): it drops into the pit there and fills it, its top level with the ledge, so
// the party can walk across. It stays there for good. A push that leaves it anywhere else does
// nothing.
void scMadraCatacombsDropPillarIntoHole(void)
{
    ACTOR *actor = fdActorGetById(ACTOR_HOLE_PILLAR);
    Sint32 z;

    if (FIELD_METATILE(actor->x.fixed) == 40)
    {
        z = FIELD_METATILE(actor->z.fixed);
        if (z == 13)
        {
            fdScriptStart();
            fdScriptEndLastingPsynergy(0);
            fdActorDropAndWait(actor);
            fdMapGridCopyMetatileAttributes(41, 13, 2, 1, 39, z);
            fdScriptSetFlag(FLAG_MADRA_CATACOMBS_HOLE_FILLED);
            fdScriptEnd();
        }
    }
}

// Nothing visible happens as Move takes hold of the plain stone pillar east of the village's
// middle; the game frees the ground it stood on, so that it can slide.
void scMadraCatacombsStartMovingPushedPillar(void)
{
    fdActorLiftTargetBlock(MadraCatacombsPushedPillar);
}

// Felix Moves the plain stone pillar east of the village's middle; once it is one metatile east, on
// metatile (44, 26), it stays there on later visits, a stepping stone to jump across from the upper
// level.
void scMadraCatacombsFinishMovingPushedPillar(void)
{
    ACTOR *actor = fdActorGetById(ACTOR_PUSHED_PILLAR);

    fdActorPlaceMovedBlock();
    if (FIELD_METATILE(actor->x.fixed) == 44 && FIELD_METATILE(actor->z.fixed) == 26)
        fdScriptSetFlag(FLAG_MADRA_CATACOMBS_PILLAR_PLACED);
}

// Felix casts Frost on the puddle just east of the broken building's south doorway: it freezes into
// a pillar of ice Felix can jump onto, until the party leaves the catacombs.
void scMadraCatacombsFreezePuddle(void)
{
    fdFlagBlockApplyFlags(MadraCatacombsPuddles);
}

void scMadraCatacombsLightFlames(void);

// Felix uses the Ruin Key at the red double door down the stairs (metatiles (9,
// 22) and (10, 22)): the two halves rumble apart, the camera climbs the hall
// beyond to Moloch's tablet, and the braziers down both sides flare up in pairs from the door
// upwards, the two great pairs by the tablet last. The key is used up.
void scMadraCatacombsUnlockWithRuinKey(void)
{
    ACTOR *left  = fdActorGetById(ACTOR_LEFT_DOOR);
    ACTOR *right = fdActorGetById(ACTOR_RIGHT_DOOR);
    Uint32 i;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdScriptPlaySound(SE_STONE_RUMBLE);
    for (i = 0; i < 120; i++)
    {
        left->x.fixed  += -(toFIXED(1) / 10);
        right->x.fixed += toFIXED(1) / 10;
        sySynch(1);
    }
    fdCameraSetSpeed(3.2);
    fdCameraMoveTo(160, CAMERA_KEEP, 240, 1);
    fdCameraWaitForArrival();
    fdCameraSetSpeed(0.4);
    fdCameraMoveTo(160, CAMERA_KEEP, 186, 1);
    fdScriptPlaySound(SE_UNLOCK);
    fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 4, 136, 258);
    fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 4, 184, 258);
    fdActorSetScript(ACTOR_FIRST_WEST_FLARE + 4, MadraCatacombsFlameFlaresUp);
    fdActorSetScript(ACTOR_FIRST_EAST_FLARE + 4, MadraCatacombsFlameFlaresUp);
    fdScriptWait(40);
    fdScriptPlaySound(SE_UNLOCK);
    fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 3, 120, 226);
    fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 3, 200, 226);
    fdActorSetScript(ACTOR_FIRST_WEST_FLARE + 3, MadraCatacombsFlameFlaresUp);
    fdActorSetScript(ACTOR_FIRST_EAST_FLARE + 3, MadraCatacombsFlameFlaresUp);
    fdScriptWait(40);
    fdScriptPlaySound(SE_UNLOCK);
    fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 2, 120, 194);
    fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 2, 200, 194);
    fdActorSetScript(ACTOR_FIRST_WEST_FLARE + 2, MadraCatacombsFlameFlaresUp);
    fdActorSetScript(ACTOR_FIRST_EAST_FLARE + 2, MadraCatacombsFlameFlaresUp);
    fdScriptWait(60);
    fdScriptPlaySound(SE_SPLASH);
    fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 1, 136, 162);
    fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 1, 184, 162);
    fdActorSetPos(ACTOR_FIRST_WEST_FLARE, 136, 138);
    fdActorSetPos(ACTOR_FIRST_EAST_FLARE, 184, 138);
    fdActorSetScript(ACTOR_FIRST_WEST_FLARE + 1, MadraCatacombsGreatFlameFlaresUp);
    fdActorSetScript(ACTOR_FIRST_EAST_FLARE + 1, MadraCatacombsGreatFlameFlaresUp);
    fdActorSetScript(ACTOR_FIRST_WEST_FLARE, MadraCatacombsGreatFlameFlaresUp);
    fdActorSetScript(ACTOR_FIRST_EAST_FLARE, MadraCatacombsGreatFlameFlaresUp);
    fdScriptWait(80);
    scMadraCatacombsLightFlames();
    gmPartyInventoryRemove(ITEM_RUIN_KEY);
    fdScriptSetFlag(FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN);
    fdScriptEnd();
}

// The vine-covered pillar becomes ground to stand on where Move left it; on metatile (35, 17) it
// stays there on later visits.
void scMadraCatacombsSettleVinePillar(ACTOR *actor);
// Coming up into the far end of the Champa tunnel's upper path (metatile (51, 13)) once that way is
// blocked off: Felix vanishes and the screen fades back the way he came.
void scMadraCatacombsLeaveBlockedTunnel(void);

// Casting Reveal in the village shows a push button in the wall just east of the golden door on the
// south side (metatile (36, 38)), pressed in once the door has opened.
void scMadraCatacombsRevealButton(void)
{
    if (fdScriptGetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN) != 0)
        fdMapGridCopyMetatilesTo(72, 63, 36, 38, 1, 2);
    else
        fdMapGridCopyMetatilesTo(71, 63, 36, 38, 1, 2);
    fdScriptSetFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
}

// As Reveal wears off, the push button by the golden door disappears.
void scMadraCatacombsHideButton(void)
{
    fdMapGridCopyMetatilesTo(70, 63, 36, 38, 1, 2);
    fdScriptClearFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
}

// Examining the wall just east of the golden door on the village's south side (metatile (36, 39))
// reads "Look upon me with eyes of truth... Then the door will open..." While Reveal shows the push
// button there, Felix presses it instead: it rattles in, and the golden door slides open with a
// thump and stays open.
void scMadraCatacombsHearEyesOfTruth(void)
{
    if (fdScriptGetFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED) != 0
        && fdScriptGetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN) == 0)
    {
        fdScriptStart();
        fdScriptEndLastingPsynergy(1);
        fdScriptPlaySound(SE_RATTLE);
        fdMapGridCopyMetatilesTo(72, 63, 36, 38, 1, 2);
        fdScriptWait(40);
        fdScriptPlaySound(SE_THUMP);
        fdMapGridCopyMetatilesTo(71, 66, 97, 39, 2, 2);
        sySynch(2);
        fdMapGridCopyMetatilesTo(73, 66, 97, 39, 2, 2);
        sySynch(2);
        fdMapGridCopyMetatilesTo(75, 66, 97, 39, 2, 2);
        fdMapGridCopyMetatileAttributes(33, 37, 2, 2, 33, 39);
        sySynch(1);
        fdScriptSetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN);
        fdScriptEnd();
    }
    else
    {
        fdScriptStart();
        fdScriptEndLastingPsynergy(0);
        // "Look upon me with eyes of truth... Then the door will open..."
        mnFieldMessageShow((Sint32)&MsgFieldEyesOfTruth, 1);
        fdScriptEnd();
    }
}

// Felix casts Tremor on the bookshelf in the bedroom behind the west door of the small hall on the
// broken building's upper floor: it rocks, and the red chest on top creeps west each time it rights
// itself. Once the chest is far enough over the edge it falls with a puff of dust, through the
// floor to the room below, where it waits with the Ruin Key.
void scMadraCatacombsShakeBookshelf(void)
{
    ACTOR *actor;
    FIXED  x;

    if (fdScriptGetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) == 0)
    {
        actor = fdActorGetById(ACTOR_SHELF_CHEST);
        x     = actor->x.fixed;
        if (x < (toFIXED(186)))
        {
            fdScriptStart();
            fdScriptEndLastingPsynergy(0);
            fdScriptPlaySound(SE_DUST_PUFF);
            actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
            sySynch(10);
            fdActorSetPos(ACTOR_SHELF_CHEST, 0, 0);
            fdScriptSetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL);
            fdScriptEnd();
        }
    }
}

void scMadraCatacombsSettleVinePillar(ACTOR *actor)
{
    scHeldActorRaiseMetatile();
    if (FIELD_METATILE(actor->x.fixed) == 35 && FIELD_METATILE(actor->z.fixed) == 17)
        fdScriptSetFlag(FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED);
}

// Walking east into the vine-covered pillar from metatile (33, 17) while Move holds it pushes it a
// metatile on.
void scMadraCatacombsPushVinePillar(void)
{
    ACTOR *actor;

    if (fdLeaderCanPushObjectAhead() != 0)
    {
        actor = fdActorGetById(fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId));
        scPillarLowerHeldMetatiles(MadraCatacombsVinePillar);
        fdLeaderPushObjectAhead();
        scMadraCatacombsSettleVinePillar(actor);
    }
}

// Nothing visible happens as Move takes hold of the vine-covered pillar north of the village's
// middle; the game lowers the ground it made, so that it can slide.
void scMadraCatacombsStartMovingVinePillar(void)
{
    scPillarLowerHeldMetatiles(MadraCatacombsVinePillar);
}

void scMadraCatacombsOpenDoorAndEnter(
    Sint32 srcX, Sint32 srcZ, Sint32 x, Sint32 z, Sint32 width, Sint32 height);

// Felix Moves the vine-covered pillar north of the village's middle; where it comes to rest becomes
// ground to stand on, so he can climb its vines to the upper level, and one metatile east, on
// metatile (35, 17), it stays on later visits.
void scMadraCatacombsFinishMovingVinePillar(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;

    if (state->movedBlock != 0)
        scMadraCatacombsSettleVinePillar(state->movedBlock);
}

// The braziers down the hall before Moloch's tablet burn: each flame stands in its place, a marker
// stands on each, and the doorway can be walked through.
void scMadraCatacombsLightFlames(void)
{
    Uint32 i;
    Sint32 target = ACTOR_FIRST_WEST_FLARE;
    Sint32 actor  = ACTOR_FIRST_FLAME_MARKER;

    fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 4, 136, 258);
    fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 4, 184, 258);
    fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 3, 120, 226);
    fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 3, 200, 226);
    fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 2, 120, 194);
    fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 2, 200, 194);
    fdActorSetPos(ACTOR_FIRST_WEST_FLARE + 1, 136, 162);
    fdActorSetPos(ACTOR_FIRST_EAST_FLARE + 1, 184, 162);
    fdActorSetPos(ACTOR_FIRST_WEST_FLARE, 136, 138);
    fdActorSetPos(ACTOR_FIRST_EAST_FLARE, 184, 138);
    for (i = 0; i < FLARES_PER_SIDE * 2; i++)
    {
        fdActorSetAnimation(actor, 0);
        fdActorPlaceOnActor(actor, target);
        target++;
        actor++;
    }
    fdMapGridCopyMetatileAttributes(0, 0, 2, 2, 9, 22);
    sySynch(1);
}

// The red chest on the bookshelf tilts as the bookshelf rocks, and each time the bookshelf comes
// upright again the chest hops two pixels west with a scrape.
void scMadraCatacombsRockChestWithBookshelf(void)
{
    ACTOR *bookshelf = fdActorGetById(ACTOR_BOOKSHELF);
    ACTOR *chest     = fdActorGetById(ACTOR_SHELF_CHEST);
    SPR   *sprite    = chest->sprite;

    sprite->rotation = ((SPR *)bookshelf->sprite)->rotation;
    if (fdScriptGetFlag(FLAG_BOOKSHELF_LEANING) != 0 && sprite->rotation == 0)
    {
        fdScriptPlaySound(SE_BOOKSHELF_SLIDE);
        chest->x.fixed   += toFIXED(-2);
        chest->velocityY  = toFIXED(2);
        fdScriptClearFlag(FLAG_BOOKSHELF_LEANING);
    }
    if (sprite->rotation != 0)
        fdScriptSetFlag(FLAG_BOOKSHELF_LEANING);
}

// Felix walks north into one of the dark doorways of the village's broken building, or the opening
// on its east side: he steps to its middle, walks in growing smaller, and the screen closes to the
// room beyond. The doorway at metatile (17, 20) leads to the small hall of the upper floor, the one
// at (18, 22) is the main entrance into the ground floor's hall, the one at (21, 17) opens into the
// east room with the stairs down to the red double door, and the opening at (54, 18) leads into the
// Champa tunnel.
void scMadraCatacombsEnterDoor(void)
{
    FIELD_STATE        *state = RamHeapSlots->fieldState;
    MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
    ACTOR              *actor;
    Sint32              metatile;

    actor = fdActorGetById(party->leaderActorId);
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    fdActorSetAnimation(party->leaderActorId, ANIM_WALK);
    fdActorSetSpeed(party->leaderActorId, 0.5);
    metatile = FIXEDtoINT(actor->x.fixed);
    fdActorSetMoveTarget(actor,
                         toFIXED(8 - metatile % MAPGRID_METATILE_PIXELS) + actor->x.fixed,
                         actor->y.fixed,
                         actor->z.fixed);
    scFieldActorWaitForMove(actor);
    actor->facing = FACING_NORTH;
    fdActorSetScript(party->leaderActorId, LeaderWalkThroughDoorScript);
    sySynch(12);
    fdScriptPlaySound(SE_MAP_EXIT);
    fdScriptFinishScreenTransition();
    fdScriptWaitTransitionTicks();
    syImportScriptSetExitRequest(state->stepCue);
}

// A door opens, its open picture copied over it from (srcX, srcZ), and Felix walks through: a stone
// door two metatiles wide slides with a thump, a door one metatile wide opens as he steps to its
// middle.
void scMadraCatacombsOpenDoorAndEnter(
    Sint32 srcX, Sint32 srcZ, Sint32 x, Sint32 z, Sint32 width, Sint32 height)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;
    ACTOR       *actor;
    Sint32       metatile;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    if (width == 2)
        fdScriptPlaySound(SE_THUMP);
    else
        fdScriptPlaySound(SE_DOOR_OPEN);
    fdMapGridCopyMetatilesTo(srcX, srcZ, x, z, width, height);
    fdScriptWait(20);
    fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
    if (width == 1)
    {
        actor    = fdActorGetById(MainRoutineStateWork.leaderActorId);
        metatile = FIXEDtoINT(actor->x.fixed);
        fdActorSetMoveTarget(actor,
                             toFIXED(8 - metatile % MAPGRID_METATILE_PIXELS) + actor->x.fixed,
                             actor->y.fixed,
                             actor->z.fixed);
        scFieldActorWaitForMove(actor);
        actor->facing = FACING_NORTH;
    }
    fdActorSetScript(MainRoutineStateWork.leaderActorId, LeaderWalkThroughDoorScript);
    sySynch(12);
    fdScriptPlaySound(SE_MAP_EXIT);
    fdScriptFinishScreenTransition();
    fdScriptWaitTransitionTicks();
    syImportScriptSetExitRequest(state->stepCue);
}

// Felix walks north up the purple carpet of the broken building's main hall into the golden stone
// door at its end (metatiles (14, 33) and (15, 33)): it slides open and he goes through to the room
// beyond, where the chest from the bookshelf falls.
void scMadraCatacombsOpenStoneDoor(void)
{
    scMadraCatacombsOpenDoorAndEnter(78, 29, 78, 31, 2, 2);
}

// Felix walks into the west door of the small hall on the broken building's upper floor (metatile
// (45, 5)): it opens and he goes through to the bedroom with the bookshelf.
void scMadraCatacombsOpenBookshelfRoomWestDoor(void)
{
    scMadraCatacombsOpenDoorAndEnter(109, 1, 109, 3, 1, 2);
}

// Felix walks into the east door of the small hall on the broken building's upper floor (metatile
// (54, 5)): it opens and he goes through to the bedroom with the broken floor.
void scMadraCatacombsOpenBookshelfRoomEastDoor(void)
{
    scMadraCatacombsOpenDoorAndEnter(118, 1, 118, 3, 1, 2);
}

// Felix walks into the door in the north-west corner of the small room over the drop (metatile (28,
// 33)): it opens and he goes through to the narrow passage with a red chest.
void scMadraCatacombsOpenTabletRoomDoor(void)
{
    scMadraCatacombsOpenDoorAndEnter(92, 29, 92, 31, 1, 2);
}

// Arriving by a way that is blocked off once Briggs is caught, Felix vanishes and the screen fades
// out to `exit`.
#define DEFINE_LEAVE_BLOCKED_WAY(name, exit)                                                       \
    void name(void)                                                                                \
    {                                                                                              \
        fdScriptStart();                                                                           \
        fdScriptEndLastingPsynergy(0);                                                             \
        fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);                                  \
        fdActorSetSpriteFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), 0);              \
        fdActorSetSpriteColor(fdActorGetById(MainRoutineStateWork.leaderActorId),                  \
                              SPRITE_COLOR_HIDDEN);                                                \
        sySynch(1);                                                                                \
        fdScriptStartScreenTransition();                                                           \
        fdScriptWaitTransitionTicks();                                                             \
        fdScriptWait(40);                                                                          \
        syImportScriptSetExitRequest(exit);                                                        \
    }

DEFINE_LEAVE_BLOCKED_WAY(scMadraCatacombsLeaveBlockedVillage,
                         MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST)

DEFINE_LEAVE_BLOCKED_WAY(scMadraCatacombsLeaveBlockedTunnel, CHAMPA_TUNNEL_UPPER_PATH_END)

Sint32 scInit(void)
{
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
        scMadraCatacombsSetUpVillage();
    else if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
        scMadraCatacombsSetUpChampaTunnel();
    else if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
        scMadraCatacombsSetUpStoneDoorRoom();
    else if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
        scMadraCatacombsSetUpBookshelfRoom();
    else if (MainRoutineStateWork.next == SCENE_MADRA_CATACOMBS_TABLET)
        scMadraCatacombsSetUpTabletRoom();
    return 0;
}

Sint32 scPrepareMap(void)
{
    return 0;
}

// Sets up the deserted village: its rope, the pillars Move has placed, the frozen puddle, the
// opened golden door and, once Briggs is caught, the Champa tunnel's way in blocked off.
void scMadraCatacombsSetUpVillage(void)
{
    fdScriptClearFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
    fdActorSetSpritePriority(ACTOR_VILLAGE_CHEST, 3);
    fdRopeInit();
    scFieldRopePlace(
        1, FLAG_VILLAGE_ROPE_TIED, ACTOR_VILLAGE_ROPE_LOW_POST, ACTOR_VILLAGE_ROPE_HIGH_POST);
    fdActorDisableById(ACTOR_HOLE_PILLAR);
    if (fdScriptGetFlag(FLAG_MADRA_CATACOMBS_HOLE_FILLED) != 0)
    {
        fdMapGridCopyMetatileAttributes(41, 13, 2, 1, 39, 13);
        scFieldActorSetPosAndResetMotion(
            fdActorGetById(ACTOR_HOLE_PILLAR), toFIXED(648), 0, toFIXED(216));
    }
    if (fdScriptGetFlag(FLAG_MADRA_CATACOMBS_PILLAR_PLACED) != 0)
        fdActorSetPos(ACTOR_PUSHED_PILLAR, 712, 424);
    fdActorPlaceBlocks(MadraCatacombsPushedPillar);
    if (fdScriptGetFlag(FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED) != 0)
        fdActorSetPos(ACTOR_VINE_PILLAR, 568, 280);
    fdActorGetById(ACTOR_VINE_PILLAR)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    scPillarRaise(MadraCatacombsVinePillar);
    fdFlagBlockPlaceAll(MadraCatacombsPuddles);
    if (fdScriptGetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN) != 0)
    {
        fdMapGridCopyMetatilesTo(75, 66, 97, 39, 2, 2);
        fdMapGridCopyMetatileAttributes(33, 37, 2, 2, 33, 39);
        sySynch(1);
    }
    if (fdScriptGetFlag(FLAG_BRIGGS_CAUGHT) != 0)
    {
        fdMapGridCopyMetatilesTo(82, 74, 116, 16, 5, 3);
        fdMapGridCopyMetatileAttributes(52, 11, 5, 1, 52, 18);
        if (MainRoutineStateWork.entrance
            == MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST)
            scMadraCatacombsLeaveBlockedVillage();
        else
            sySynch(10);
    }
}

// Sets up the Champa tunnel: its rope and, once Briggs is caught, the way on to the village blocked
// off and the man who marvels at the cave gone.
void scMadraCatacombsSetUpChampaTunnel(void)
{
    fdRopeInit();
    scFieldRopePlace(
        0, FLAG_TUNNEL_ROPE_TIED, ACTOR_TUNNEL_ROPE_EAST_POST, ACTOR_TUNNEL_ROPE_WEST_POST);
    if (fdScriptGetFlag(FLAG_BRIGGS_CAUGHT) != 0)
    {
        fdActorSetPos(ACTOR_MAN_ON_THE_CHAMPAS_CAVE, 0, 0);
        fdMapGridCopyMetatilesTo(68, 70, 113, 11, 5, 3);
        fdMapGridCopyMetatilesTo(50, 74, 50, 75, 2, 1);
        fdMapGridCopyMetatileAttributes(49, 9, 5, 1, 49, 13);
        if (MainRoutineStateWork.entrance == CHAMPA_TUNNEL_UPPER_PATH_END)
            scMadraCatacombsLeaveBlockedTunnel();
        else
            sySynch(1);
    }
}

// Sets up the broken building's ground floor, below the bookshelf: the chest with the Ruin Key is
// there only once Tremor has shaken it down.
void scMadraCatacombsSetUpStoneDoorRoom(void)
{
    if (fdScriptGetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) == 0)
        fdActorSetPos(ACTOR_FALLEN_CHEST, 0, 0);
}

// Sets up the broken building's upper floor: the red chest stands on top of the bookshelf until
// Tremor has shaken it down.
void scMadraCatacombsSetUpBookshelfRoom(void)
{
    ACTOR *actor;

    if (fdScriptGetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) != 0)
    {
        fdActorSetPos(ACTOR_SHELF_CHEST, 0, 0);
    }
    else
    {
        actor                                   = fdActorGetById(ACTOR_SHELF_CHEST);
        actor->collisionFlags                   = ACTOR_NOT_TARGETED;
        actor->motionFlags                      = ACTOR_FALLS;
        actor->y.fixed                          = toFIXED(32);
        actor->groundHeight                     = toFIXED(32);
        actor->update                           = scMadraCatacombsRockChestWithBookshelf;
        fdActorGetById(ACTOR_BOOKSHELF)->scaleY = toFIXED(4) / 5;
    }
}

// Sets up the hall before Moloch's tablet: the red double door's two halves stand closed; once the
// Ruin Key has opened it they stand 12 pixels apart (already so when the party comes back from a
// battle) and the braziers burn.
void scMadraCatacombsSetUpTabletRoom(void)
{
    ACTOR *left        = fdActorGetById(ACTOR_LEFT_DOOR);
    ACTOR *right       = fdActorGetById(ACTOR_RIGHT_DOOR);

    left->scaleX       = toFIXED(5) / 4;
    right->scaleX      = -toFIXED(5) / 4;
    left->motionFlags  = 0;
    right->motionFlags = 0;
    left->y.fixed      = 0;
    right->y.fixed     = 0;
    if (fdScriptGetFlag(FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN) != 0)
    {
        if (fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
        {
            left->x.fixed  += toFIXED(-12);
            right->x.fixed += toFIXED(12);
        }
        scMadraCatacombsLightFlames();
    }
}

// clang-format off
#include "shared/leader_walk_through_door_script.c"

// A brazier's flame's actor script as it flares up.
Sint32 MadraCatacombsFlameFlaresUp[] = {
    SC_SET(SW_SCALE_X, toFIXED(1) / 16),
    SC_SET(SW_SCALE_Y, toFIXED(1) / 16),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 4),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_SET(SW_SCALE_X, toFIXED(1)),
    SC_SET(SW_SCALE_Y, toFIXED(1)),
    SC_END,
};

// The actor script of the great braziers by Moloch's tablet as they flare up.
Sint32 MadraCatacombsGreatFlameFlaresUp[] = {
    SC_SET(SW_SCALE_X, toFIXED(1) / 16),
    SC_SET(SW_SCALE_Y, toFIXED(1) / 16),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 2),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 4),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -toFIXED(1) / 8),
    SC_ADD(SW_SCALE_Y, toFIXED(3) / 8),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, -toFIXED(1) / 8),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 4),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(3)),
    SC_SET(SW_SCALE_X, toFIXED(1)),
    SC_SET(SW_SCALE_Y, toFIXED(1)),
    SC_END,
};

// The plain stone pillar Move pushes, ended by -1.
Sint16 MadraCatacombsPushedPillar[] = {
    ACTOR_PUSHED_PILLAR, -1,
};

// The puddle Frost freezes into a pillar, with its flag.
Sint16 MadraCatacombsPuddles[] = {
    ACTOR_VILLAGE_PUDDLE, FLAG_VILLAGE_PUDDLE_FROZEN, -1,
};

// The vine-covered pillar Move pushes, ended by -1.
Sint16 MadraCatacombsVinePillar[] = {
    ACTOR_VINE_PILLAR, -1, 0,
};

#include "shared/scene_entrances_default.c"

// The entrances of the stone door's room.
SCENE_ENTRANCE MadraCatacombsStoneDoorEntrances[] = {
    {3, CONDITION_ALWAYS, 512, 0, 224, FACING_NORTH, 0, 392, 40, 632, 280, 0},
    {SCENE_TABLE_END},
};

// The entrances of the bookshelf room.
SCENE_ENTRANCE MadraCatacombsBookshelfEntrances[] = {
    {5, CONDITION_ALWAYS, 728, 0, 88, FACING_SOUTH, 0, 680, 32, 920, 192, 0},
    {6, CONDITION_ALWAYS, 872, 0, 88, FACING_SOUTH, 0, 680, 32, 920, 192, 0},
    {7, CONDITION_ALWAYS, 872, 0, 120, FACING_EAST, 0, 680, 32, 920, 192, 0},
    {8, CONDITION_ALWAYS, 792, 0, 152, FACING_NORTH, 0, 680, 32, 920, 192, 0},
    {SCENE_TABLE_END},
};

// Climbing either ladder up out of the ladder room takes the party up into Madra.
SCENE_REGION MadraCatacombsLadderRoomRegions[] = {
    {112, 48, 144, 128, 64, 160, CONDITION_ALWAYS, MADRA_CATACOMBS_LADDERS_TO_MADRA_WEST},
    {336, 48, 144, 352, 64, 160, CONDITION_ALWAYS, MADRA_CATACOMBS_LADDERS_TO_MADRA_EAST},
    {SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
    SCENE_EXITS(MadraCatacombsEntrance),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_BOOKSHELF, MadraCatacombsBookshelf, MADRA_CATACOMBS_BOOKSHELF_FROM_MADRA_CATACOMBS_ENTRANCE),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_STONE_DOOR_NORTH, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_ENTRANCE_NORTH),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST, MadraCatacombsChampaTunnel, MADRA_CATACOMBS_CHAMPA_TUNNEL_FROM_MADRA_CATACOMBS_ENTRANCE_WEST),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_ENTRANCE_SOUTH),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_EAST, MadraCatacombsChampaTunnel, MADRA_CATACOMBS_CHAMPA_TUNNEL_FROM_MADRA_CATACOMBS_ENTRANCE_EAST),
    SCENE_EXITS(MadraCatacombsLadders),
    SCENE_EXIT(MADRA_CATACOMBS_LADDERS_TO_MADRA_WEST, Madra, MADRA_FROM_MADRA_CATACOMBS_LADDERS_WEST),
    SCENE_EXIT(MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_NORTH, MadraCatacombsChampaTunnel, MADRA_CATACOMBS_CHAMPA_TUNNEL_FROM_MADRA_CATACOMBS_LADDERS_NORTH),
    SCENE_EXIT(MADRA_CATACOMBS_LADDERS_TO_MADRA_EAST, Madra, MADRA_FROM_MADRA_CATACOMBS_LADDERS_EAST),
    SCENE_EXIT(MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_SOUTH, MadraCatacombsChampaTunnel, MADRA_CATACOMBS_CHAMPA_TUNNEL_FROM_MADRA_CATACOMBS_LADDERS_SOUTH),
    SCENE_EXITS(MadraCatacombsChampaTunnel),
    SCENE_EXIT(MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_WEST, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST),
    SCENE_EXIT(2, MadraCatacombsChampaTunnel, 3),
    SCENE_EXIT(3, MadraCatacombsChampaTunnel, 2),
    SCENE_EXIT(MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_SOUTH, MadraCatacombsLadders, MADRA_CATACOMBS_LADDERS_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_SOUTH),
    SCENE_EXIT(5, MadraCatacombsChampaTunnel, 8),
    SCENE_EXIT(MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_EAST, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_EAST),
    SCENE_EXIT(MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_NORTH, MadraCatacombsLadders, MADRA_CATACOMBS_LADDERS_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_NORTH),
    SCENE_EXIT(8, MadraCatacombsChampaTunnel, 5),
    SCENE_EXITS(MadraCatacombsStoneDoor),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_NORTHWEST, MadraCatacombsBookshelf, MADRA_CATACOMBS_BOOKSHELF_FROM_MADRA_CATACOMBS_STONE_DOOR_NORTHWEST),
    SCENE_EXIT(2, MadraCatacombsStoneDoor, 11),
    SCENE_EXIT(3, MadraCatacombsStoneDoor, 9),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_EAST, MadraCatacombsBookshelf, MADRA_CATACOMBS_BOOKSHELF_FROM_MADRA_CATACOMBS_STONE_DOOR_EAST),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_EAST, MadraCatacombsTablet, MADRA_CATACOMBS_TABLET_FROM_MADRA_CATACOMBS_STONE_DOOR_EAST),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_WEST, MadraCatacombsTablet, MADRA_CATACOMBS_TABLET_FROM_MADRA_CATACOMBS_STONE_DOOR_WEST),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_NORTH, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_STONE_DOOR_NORTH),
    SCENE_EXIT(8, MadraCatacombsStoneDoor, 12),
    SCENE_EXIT(9, MadraCatacombsStoneDoor, 3),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_SOUTH, MadraCatacombsBookshelf, MADRA_CATACOMBS_BOOKSHELF_FROM_MADRA_CATACOMBS_STONE_DOOR_SOUTH),
    SCENE_EXIT(11, MadraCatacombsStoneDoor, 2),
    SCENE_EXIT(12, MadraCatacombsStoneDoor, 8),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_SOUTH, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_STONE_DOOR_SOUTH),
    SCENE_EXITS(MadraCatacombsBookshelf),
    SCENE_EXIT(MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_NORTHWEST, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_BOOKSHELF_NORTHWEST),
    SCENE_EXIT(MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_EAST, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_BOOKSHELF_EAST),
    SCENE_EXIT(3, MadraCatacombsBookshelf, 5),
    SCENE_EXIT(4, MadraCatacombsBookshelf, 6),
    SCENE_EXIT(5, MadraCatacombsBookshelf, 3),
    SCENE_EXIT(6, MadraCatacombsBookshelf, 4),
    SCENE_EXIT(MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_BOOKSHELF_SOUTH),
    SCENE_EXIT(MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_ENTRANCE, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_BOOKSHELF),
    SCENE_EXITS(MadraCatacombsTablet),
    SCENE_EXIT(MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_WEST, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_TABLET_WEST),
    SCENE_EXIT(2, MadraCatacombsTablet, 3),
    SCENE_EXIT(3, MadraCatacombsTablet, 2),
    SCENE_EXIT(MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_EAST, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_TABLET_EAST),
    SCENE_EXITS_END,
};

// The actors placed in the catacombs' rooms with no table of their own.
ACTOR_PLACE MadraCatacombsPlacements[] = {
    {SCENE_TABLE_END},
};

// The actors placed in the catacombs' entrance.
ACTOR_PLACE MadraCatacombsEntrancePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_VINE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The actors placed in the Champa tunnel.
ACTOR_PLACE MadraCatacombsChampaTunnelPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 794, 0, 236, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

// The actors placed in the bookshelf room.
ACTOR_PLACE MadraCatacombsBookshelfPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BOOKSHELF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 202, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The actors placed in the tablet room.
ACTOR_PLACE MadraCatacombsTabletPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 150, 0, 366, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 170, 0, 366, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The cues of the catacombs' rooms with no table of their own.
CUE MadraCatacombsCues[] = {
    {CUE_TABLE_END},
};

// The cues of the catacombs' entrance.
CUE MadraCatacombsEntranceCues[] = {
    {STEP_CUE_WALKING_INTO(FACING_NORTH), MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_BOOKSHELF, CONDITION_ALWAYS, SCRIPT_REF(scMadraCatacombsEnterDoor)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH), MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_STONE_DOOR_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scMadraCatacombsEnterDoor)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH), MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST, CONDITION_ALWAYS, SCRIPT_REF(scMadraCatacombsEnterDoor)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH), MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scMadraCatacombsEnterDoor)},
    {CUE_EXIT, MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_EAST, CONDITION_ALWAYS, MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_EAST},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_VILLAGE_ROPE_LOW_POST, FLAG_VILLAGE_ROPE_TIED, 0},
    // A pushed cue answers whatever metatile is pushed against

    // (fdFindCue): its 10 is never compared.
    {CUE_PUSHED, 10, FLAG_MADRA_CATACOMBS_HOLE_FILLED, SCRIPT_REF(scMadraCatacombsDropPillarIntoHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HOLE_PILLAR, FLAG_MADRA_CATACOMBS_HOLE_FILLED, SCRIPT_REF(scMadraCatacombsDropPillarIntoHole)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PUSHED_PILLAR, FLAG_MADRA_CATACOMBS_PILLAR_PLACED, SCRIPT_REF(scMadraCatacombsStartMovingPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_PILLAR, FLAG_MADRA_CATACOMBS_PILLAR_PLACED, SCRIPT_REF(scMadraCatacombsFinishMovingPushedPillar)},
    {STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_MADRA_CATACOMBS_VINE_PILLAR_PUSH, FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED, SCRIPT_REF(scMadraCatacombsPushVinePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VINE_PILLAR, FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED, SCRIPT_REF(scMadraCatacombsStartMovingVinePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VINE_PILLAR, FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED, SCRIPT_REF(scMadraCatacombsFinishMovingVinePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VILLAGE_PUDDLE, FLAG_VILLAGE_PUDDLE_FROZEN, SCRIPT_REF(scMadraCatacombsFreezePuddle)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scMadraCatacombsRevealButton)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scMadraCatacombsHideButton)},
    {EXAMINE_CUE_FACING(FACING_NORTH), METATILE_BEHAVIOR_MADRA_CATACOMBS_BUTTON_WALL, CONDITION_ALWAYS, SCRIPT_REF(scMadraCatacombsHearEyesOfTruth)},
    {CUE_TABLE_END},
};

// The cues of the ladder room.
CUE MadraCatacombsLaddersCues[] = {
    {CUE_EXIT, MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_NORTH, CONDITION_ALWAYS, MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_NORTH},
    {CUE_EXIT, MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_SOUTH, CONDITION_ALWAYS, MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_SOUTH},
    {CUE_TABLE_END},
};
// clang-format on

// The cues of the Champa tunnel.
CUE MadraCatacombsChampaTunnelCues[] = {
    {CUE_EXIT,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_WEST, CONDITION_ALWAYS,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_WEST},
    {CUE_EXIT, 2, CONDITION_ALWAYS, 2},
    {CUE_EXIT, 3, CONDITION_ALWAYS, 3},
    {CUE_EXIT,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_SOUTH, CONDITION_ALWAYS,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_SOUTH},
    {CUE_EXIT, 5, CONDITION_ALWAYS, 5},
    {CUE_EXIT,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_EAST, CONDITION_ALWAYS,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_EAST},
    {CUE_EXIT,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_NORTH, CONDITION_ALWAYS,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_NORTH},
    {CUE_EXIT, CHAMPA_TUNNEL_UPPER_PATH_END, CONDITION_ALWAYS, CHAMPA_TUNNEL_UPPER_PATH_END},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_TUNNEL_ROPE_EAST_POST, FLAG_TUNNEL_ROPE_TIED, 0},
    {CUE_TALK, ACTOR_MAN_ON_THE_CHAMPAS_CAVE, CONDITION_ALWAYS, MSGNO(MsgFieldCaveFoundByChampa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_MAN_ON_THE_CHAMPAS_CAVE, CONDITION_ALWAYS,
     MSGNO(MsgFieldWhoDugCave)},
    {CUE_TABLE_END},
};

// The cues of the stone door's room.
CUE MadraCatacombsStoneDoorCues[] = {
    {CUE_EXIT,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_NORTHWEST, CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_NORTHWEST},
    {CUE_EXIT, 2, CONDITION_ALWAYS, 2},
    {CUE_EXIT, 3, CONDITION_ALWAYS, 3},
    {CUE_EXIT,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_EAST, CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_EAST},
    {CUE_EXIT,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_EAST, CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_EAST},
    {CUE_EXIT,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_WEST, CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_WEST},
    {CUE_EXIT,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_NORTH, CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_NORTH},
    {CUE_EXIT, 8, CONDITION_ALWAYS, 8},
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     9, CONDITION_ALWAYS,
     SCRIPT_REF(scMadraCatacombsOpenStoneDoor)},
    {CUE_EXIT,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_SOUTH, CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_SOUTH},
    {CUE_EXIT, 11, CONDITION_ALWAYS, 11},
    {CUE_EXIT, 12, CONDITION_ALWAYS, 12},
    {CUE_EXIT,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_SOUTH, CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_SOUTH},
    {CUE_TABLE_END},
};

// The cues of the bookshelf room.
CUE MadraCatacombsBookshelfCues[] = {
    {CUE_EXIT,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_NORTHWEST, CONDITION_ALWAYS,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_NORTHWEST},
    {CUE_EXIT,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_EAST, CONDITION_ALWAYS,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_EAST},
    {CUE_EXIT, 3, CONDITION_ALWAYS, 3},
    {CUE_EXIT, 4, CONDITION_ALWAYS, 4},
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     5, CONDITION_ALWAYS,
     SCRIPT_REF(scMadraCatacombsOpenBookshelfRoomWestDoor)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     6, CONDITION_ALWAYS,
     SCRIPT_REF(scMadraCatacombsOpenBookshelfRoomEastDoor)},
    {CUE_EXIT,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH, CONDITION_ALWAYS,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH},
    {CUE_EXIT,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_ENTRANCE, CONDITION_ALWAYS,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_ENTRANCE},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR),
     ACTOR_BOOKSHELF, CONDITION_ALWAYS,
     SCRIPT_REF(scMadraCatacombsShakeBookshelf)},
    {CUE_TABLE_END},
};

// The cues of the tablet room.
CUE MadraCatacombsTabletCues[] = {
    {CUE_EXIT,
     MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_WEST, CONDITION_ALWAYS,
     MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_WEST},
    {CUE_EXIT, 2, CONDITION_ALWAYS, 2},
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     3, CONDITION_ALWAYS,
     SCRIPT_REF(scMadraCatacombsOpenTabletRoomDoor)},
    {CUE_EXIT,
     MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_EAST, CONDITION_ALWAYS,
     MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_EAST},
    {USE_ITEM(ITEM_RUIN_KEY),
     METATILE_BEHAVIOR_MADRA_CATACOMBS_TABLET_DOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scMadraCatacombsUnlockWithRuinKey)},
    {CUE_EXAMINE,
     METATILE_BEHAVIOR_MADRA_CATACOMBS_TABLET_DOOR, FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN,
     SCRIPT_REF(scMadraCatacombsTryLockedDoor)},
    {CUE_TABLE_END},
};
