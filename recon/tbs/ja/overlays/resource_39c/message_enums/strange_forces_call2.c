/* NONMATCHING: Japanese compile-known message number, 2026-10-01.
 * The approved TBS flags produce a 24-byte extent instead of the game's
 * 28 bytes. The number 6080 is built from a shifted byte, while the game
 * loads the canonical message from its pool. Call2 produces the same mismatch.
 */
#include "TYPES.H"
#include "text/MSG_IDS.H"

TEXT_MESSAGE_ENUM(MsgMakyuriHeyaStrangeForcesBlockWay);
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "../../../../../../games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/PROBE.H"
#include "CALL.H"

extern const struct SceneEntrance gMakyuriHeyaEntrances4[];
extern const struct SceneEntrance gMakyuriHeyaEntrances3[];
extern const struct SceneEntrance gMakyuriHeyaEntrances2[];
extern const struct SceneEntrance gMakyuriHeyaEntrancesOther[];

extern u8 MakyuriHeya_SceneTable[];

extern const struct ScenePlacement gMakyuriHeyaPlacements1[];
extern const struct ScenePlacement gMakyuriHeyaPlacements2[];
extern const struct ScenePlacement gMakyuriHeyaPlacements3[];
extern const struct ScenePlacement gMakyuriHeyaPlacements4[];
extern const struct ScenePlacement gMakyuriHeyaPlacementsOther[];

extern const struct SceneEvent gMakyuriHeyaEvents1[];
extern const struct SceneEvent gMakyuriHeyaEvents2[];
extern const struct SceneEvent gMakyuriHeyaEvents3[];
extern const struct SceneEvent gMakyuriHeyaEventsOther[];

/* Where the party appears in each of the lighthouse rooms; the first takes
   the table the other scenes take. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MakyuriHeya4) {
        return gMakyuriHeyaEntrances4;
    }
    if (selector == (s32)&SceneId_MakyuriHeya3) {
        return gMakyuriHeyaEntrances3;
    }
    if (selector == (s32)&SceneId_MakyuriHeya2) {
        return gMakyuriHeyaEntrances2;
    }
    return gMakyuriHeyaEntrancesOther;
}

void SceneDialogue_RunLine1637(void)
{
    Engine_EventBegin();
    Call2(Engine_MessageShowCentered, MsgMakyuriHeyaStrangeForcesBlockWay, 1);
    Engine_EventEnd();
}
