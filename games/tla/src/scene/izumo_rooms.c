// Inside Izumo's houses on stilts, one room each: two houses in the east, the inn, the shop,
// Kushinada's house, the Sanctum, and the hall of Susa's house in the north, which has a cast and a
// story of its own.
//
// Before the Serpent is slain the village mourns: Kushinada sits on her seat of stones and, taking
// Felix for Susa, begs him not to come for her, and in Susa's house Lady Uzume says he has gone to
// fight the Serpent. Once it is slain the party's first visit to Susa's house becomes the heroes'
// welcome (scIzumoRoomsRecognizeTheHeroes): Lady Uzume and Kushinada learn who slew it, Susa comes
// home, and Kraden explains Psynergy. Afterwards Lady Uzume takes the Dancing Idol and gives the
// party Coal, the Mars Djinni (scIzumoRoomsGiveUzumeTheDoll).

#include "item_ids.h"
#include "field/script_inline.h"
#include "field/field_mode.h"
#include "game/game.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "shop/shop.h"
#include "text/msg_ids.h"

// Story flags: Lady Uzume has told the party where Susa went; her prayer for him has been read with
// Mind Read; Kushinada has begged "Susa" not to come for her; and Lady Uzume has caught the party
// reading her mind ("Trying again, Felix?").
#define FLAG_UZUME_SPOKE_OF_SUSA              0x922
#define FLAG_UZUMES_PRAYER_READ               0x923
#define FLAG_KUSHINADA_ASKED_SUSA_NOT_TO_COME 0x925
#define FLAG_UZUME_MIND_READ                  0x92b
// Set as Susa's house welcomes the heroes (and again as Lady Uzume takes the Dancing Idol), and
// cleared on entering any other room: while it is set, the two women in red and the old men by the
// drums stay off stage.
#define FLAG_HEROES_JUST_RECOGNIZED           FLAG_AREA_TEMP_50

// The metatiles across the hall of Susa's house, on its east and west sides, where the heroes'
// welcome starts.
enum
{
    METATILE_EAST_SIDE_OF_THE_HALL = 22,
    METATILE_WEST_SIDE_OF_THE_HALL
};

extern ACTOR_PLACE IzumoRoomsActorPlacements[];
extern ACTOR_PLACE IzumoRoomsSusasHouseActorPlacements[];
extern ACTOR_PLACE IzumoRoomsActorPlacementsOnceTheSerpentIsSlain[];
extern CUE         IzumoRoomsCues[];
extern CUE         IzumoRoomsSusasHouseCues[];
extern CUE         IzumoRoomsCuesOnceTheSerpentIsSlain[];

// The side of the hall of Susa's house Felix came up for the heroes' welcome.
enum
{
    HEROES_FROM_THE_EAST,
    HEROES_FROM_THE_WEST
};

void          scIzumoRoomsRecognizeTheHeroes(Sint32 side);

void          scIzumoRoomsShowMove(void);
extern Sint32 IzumoRoomsFirstDancerScript[];
extern Sint32 IzumoRoomsSecondDancerScript[];
extern Sint32 IzumoRoomsHeavingScript[];

// Whether the other of the two women dancing in Susa's house is clear of the spot by the doorway
// this one steps to; each one's dance waits on it.
#define DEFINE_IZUMO_ROOMS_IS_CLEAR(name, n1, n2)                                                  \
    Sint32 name(void)                                                                              \
    {                                                                                              \
        ACTOR *actor = fdActorGetById(n1);                                                         \
                                                                                                   \
        if ((Uint32)(actor->x.fixed - n2) <= toFIXED(8))                                           \
        {                                                                                          \
            Sint32 z = actor->z.fixed;                                                             \
                                                                                                   \
            if (z >= toFIXED(620))                                                                 \
            {                                                                                      \
                if (z <= toFIXED(628))                                                             \
                    return 0;                                                                      \
            }                                                                                      \
        }                                                                                          \
        return 1;                                                                                  \
    }

// Whether the second dancer is clear of the spot the first steps to; the first dancer's script
// waits on it.
DEFINE_IZUMO_ROOMS_IS_CLEAR(IzumoRoomsIsFirstDancerClear, 10, toFIXED(148))

// Whether the first dancer is clear of the spot the second steps to; the second dancer's script
// waits on it.
DEFINE_IZUMO_ROOMS_IS_CLEAR(IzumoRoomsIsSecondDancerClear, 9, toFIXED(132))

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

const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (syGameFlagGet(FLAG_SERPENT_SLAIN))
        return IzumoRoomsActorPlacementsOnceTheSerpentIsSlain;
    if (MainRoutineStateWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH)
        return IzumoRoomsSusasHouseActorPlacements;
    return IzumoRoomsActorPlacements;
}

const CUE *scGetCues(void)
{
    if (syGameFlagGet(FLAG_SERPENT_SLAIN))
        return IzumoRoomsCuesOnceTheSerpentIsSlain;
    if (MainRoutineStateWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH)
        return IzumoRoomsSusasHouseCues;
    return IzumoRoomsCues;
}

// Before the Serpent is slain: Kushinada, whose turn it is to be offered to it, and the Sanctum's
// priest ("Why would our guardian spirit demand a sacrifice?"), who stands behind the same counter
// once it is slain.
enum
{
    ACTOR_KUSHINADA         = 17,
    ACTOR_PRIEST            = 22,
    ACTOR_PRIEST_ONCE_SLAIN = 12
};

// Susa's house before the Serpent is slain: Lady Uzume, Susa's sister ("If the serpent defeats
// Susa, I'll...be alone."), with two women dancing ("There's no point dancing if Okuni's not
// here...") and two old men by the drums.
enum
{
    ACTOR_SUSAS_HOUSE_UZUME = 8,
    ACTOR_SUSAS_HOUSE_FIRST_DANCER,
    ACTOR_SUSAS_HOUSE_SECOND_DANCER,
    ACTOR_SUSAS_HOUSE_WEST_VILLAGER,
    ACTOR_SUSAS_HOUSE_EAST_VILLAGER
};

// Once the Serpent is slain, Susa's house holds Lady Uzume ("This is my home, and I am having a
// private conversation."), the same four, Kushinada, Susa, the villager who saw the heroes come
// down the mountain ("Yes... After the serpent's roar, these warriors appeared."), the party for
// the welcome, and Coal.
enum
{
    ACTOR_UZUME = 15,
    ACTOR_FIRST_DANCER,
    ACTOR_SECOND_DANCER,
    ACTOR_WEST_VILLAGER,
    ACTOR_EAST_VILLAGER,
    ACTOR_SAVED_KUSHINADA,
    ACTOR_SUSA,
    ACTOR_WITNESS,
    ACTOR_KRADEN,
    ACTOR_COAL // the Mars Djinni Uzume gives the party
};

// Talking to the priest in the Sanctum across his counter opens the Sanctum's healing. From any
// other side he asks why the guardian spirit would demand a sacrifice, or once the Serpent is slain
// says the island is flourishing.
void scIzumoRoomsTalkToPriest(Sint32 actor)
{
    Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

    if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH)
    {
        mnSanctumRun(actor);
    }
    else
    {
        fdScriptStart();
        fdScriptEndLastingPsynergy(0);
        if (syGameFlagGet(FLAG_SERPENT_SLAIN))
        {
            fdScriptSetMessageId((Sint32)MsgNowThatPeace);
            // "Now that peace has been restored, our island is flourishing once..."
            fdScriptShowMessageAndWait(ACTOR_PRIEST_ONCE_SLAIN, 0);
        }
        else
        {
            fdScriptSetMessageId((Sint32)MsgWhyWouldOur);
            // "Why would our guardian spirit demand a sacrifice? It's..."
            fdScriptShowMessageAndWait(ACTOR_PRIEST, 0);
        }
        fdScriptEnd();
    }
}

// The fifth animation of these villagers' sprites, as Kushinada's room and Susa's house show them
// before the Serpent is slain: Kushinada kneeling on her seat, the women in red dancing with their
// arms out, and the old men beating the drums.
enum
{
    ANIM_KNEEL     = 5,
    ANIM_DANCE     = 5,
    ANIM_BEAT_DRUM = 5
};

// Izumo's inn, as mnInnCheckIn numbers the inns.
enum
{
    IZUMO_INN = 9
};

// Talking to the innkeeper across his counter offers a night at Izumo's inn. From any other side he
// says he never believed the legend of the Serpent, or once it is slain that Briggs from Champa
// hasn't been by selling fish.
void scIzumoRoomsTalkToInnkeeper(Sint32 actor)
{
    Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

    if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH)
    {
        mnInnCheckIn(IZUMO_INN, actor);
    }
    else
    {
        fdScriptStart();
        fdScriptEndLastingPsynergy(0);
        if (syGameFlagGet(FLAG_SERPENT_SLAIN))
            fdScriptSetMessageId((Sint32)MsgBriggsFromChampa);
        else
            fdScriptSetMessageId((Sint32)MsgOfCourseI);
        // "Of course, I knew the legend of the great serpent... Everyone does!..."
        fdScriptShowMessageAndWait(actor, 0);
        fdScriptEnd();
    }
}

// Talking to the shopkeeper across her counter opens Izumo's shop. From any other side she speaks
// of the special powers of Izumo's leaders, or once the Serpent is slain of the wonders the party
// must see on its travels.
void scIzumoRoomsTalkToMerchant(Sint32 actor)
{
    Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

    if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH)
    {
        mnShopRun(SHOP_IZUMO, actor);
    }
    else
    {
        fdScriptStart();
        fdScriptEndLastingPsynergy(0);
        if (syGameFlagGet(FLAG_SERPENT_SLAIN))
            fdScriptSetMessageId((Sint32)MsgYouMustSee);
        else
            fdScriptSetMessageId((Sint32)MsgIzumoSLeaders);
        // "Izumo's leaders have always possessed certain special powers...."
        fdScriptShowMessageAndWait(actor, 0);
        fdScriptEnd();
    }
}

// Talking to Lady Uzume in Susa's house before the Serpent is slain: the first time she says Susa
// has gone to fight the Serpent to save Kushinada, all for naught since a hero will surely appear,
// and each time she fears she will be alone if it defeats him.
void scIzumoRoomsAskAfterSusa(void)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    if (!fdScriptGetFlag(FLAG_UZUME_SPOKE_OF_SUSA))
    {
        fdScriptSetMessageId((Sint32)MsgSusaSNot);
        // "Susa's not here. I believe he went to Mt. Mikage."
        fdScriptShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
        fdScriptWait(30);
        fdActorShowEmoteById(ACTOR_SUSAS_HOUSE_UZUME, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
        fdScriptWait(60);
        // "He believes he can save Kushinada, and so he has gone to fight the..."
        fdScriptShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
        fdScriptWait(30);
        fdActorShowEmoteById(ACTOR_SUSAS_HOUSE_UZUME, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
        fdScriptWait(60);
        // "He truly believes he can defeat the Great Serpent of Mikage."
        fdScriptShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
        fdScriptWait(30);
        fdActorStartRepeatedMotion(ACTOR_SUSAS_HOUSE_UZUME, 1);
        fdScriptWait(60);
        // "It's all for naught, though, as a hero will certainly appear and..."
        fdScriptShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
        fdScriptWait(30);
        syGameFlagSet(FLAG_UZUME_SPOKE_OF_SUSA);
    }
    fdActorSetAnimationById(ACTOR_SUSAS_HOUSE_UZUME, ANIM_SHAKE_HEAD);
    fdScriptWait(60);
    fdScriptSetMessageId((Sint32)MsgIfTheSerpent);
    // "If the serpent defeats Susa, I'll...be alone."
    fdScriptShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
    fdScriptEnd();
}

// Mind Read on Lady Uzume before the Serpent is slain shows her prayer that the guardian's anger is
// not aroused and that Susa will be protected.
void scIzumoRoomsReadPrayerForSusa(void)
{
    syGameFlagSet(FLAG_UZUMES_PRAYER_READ);
    fdScriptSetMessageId((Sint32)MsgIPrayThat);
    // "I pray that our guardian's anger is not aroused and that Susa will..."
    fdScriptShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
}

// Talking to Lady Uzume in Susa's house once the Serpent is slain: she invites the party to enjoy
// the festival, and if one of the party carries the Dancing Idol the field menu opens so that it
// can be given to her. Once she has it, she says she can explore Mt. Mikage on her own with it.
void scIzumoRoomsTalkToUzume(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
    if (syGameFlagGet(FLAG_UZUME_HAS_THE_DOLL))
    {
        fdScriptSetMessageId((Sint32)MsgWithThisDancing);
        // "With this dancing doll, I can explore Mt. Mikage on my own. Take..."
        fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    }
    else
    {
        fdScriptSetMessageId((Sint32)MsgIfYouHave);
        // "If you have time, won't you enjoy a little of our celebratory festival?"
        fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
        if (gmPartyInventoryFindMember(ITEM_DANCING_IDOL) != -1)
            state->fieldMenu = 1;
    }
    fdActorFaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
    fdScriptEnd();
}

// The west one of the two women in red in Susa's house, once the Serpent is slain, asks whether the
// party went into the mountain too: either way she tells of the legend of a dancing doll of
// mysterious power hidden up there, which Lady Uzume would love to have.
void scIzumoRoomsAskAboutMtMikage(void)
{
    Sint32 msg;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    msg = (Sint32)MsgTheyVeBeen;
    fdScriptSetMessageId(msg);
    // "They've been saying that you went into Mt. Mikage, too... Is that true?"
    fdMessageShowAtActor(ACTOR_FIRST_DANCER, 0);
    if (fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0)
    {
        fdScriptWait(10);
        fdScriptSetMessageId(msg + 1);
    }
    else
    {
        fdScriptWait(20);
        fdScriptSetMessageId(msg + 2);
    }
    fdScriptShowMessageAndWait(ACTOR_FIRST_DANCER, 0);
    fdScriptEnd();
}

// Mind Read on Lady Uzume once the Serpent is slain: she notices it ("And what were you planning to
// do, once you'd read my mind, hm?"), and scolds Felix if her prayer was read before; tried again,
// she calls him a pest who can see her mind but not her heart.
void scIzumoRoomsReadUzumesMind(void)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
    if (fdScriptGetFlag(FLAG_UZUME_MIND_READ))
    {
        fdScriptSetMessageId((Sint32)MsgTryingAgainWhat);
        // "Trying again, Felix? What a pest. You can see my mind, but you are..."
        fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    }
    else
    {
        fdScriptSetMessageId((Sint32)MsgAndWhatWere);
        // "And what were you planning to do, once you'd read my mind, hm?"
        fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
        if (syGameFlagGet(FLAG_UZUMES_PRAYER_READ))
        {
            fdActorShowEmoteById(
                MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
            fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
            fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
            fdActorShowEmoteById(
                MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
            fdActorSetAnimationById(ACTOR_UZUME, ANIM_SHAKE_HEAD);
            fdScriptWait(30);
            fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
        }
        syGameFlagSet(FLAG_UZUME_MIND_READ);
    }
    fdActorFaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
    fdScriptEnd();
}

// Talking to Kushinada on her seat of stones in her house in the north-west, before the Serpent is
// slain: she takes Felix for Susa without looking up. The first time she begs him not to come for
// her, lest she lose the courage to be offered for Izumo; after that only "Susa, please... Don't
// try to save me." Each time Felix steps back from her.
Sint32 scIzumoRoomsKushinadaAsksSusaNotToCome(void)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    if (!fdScriptGetFlag(FLAG_KUSHINADA_ASKED_SUSA_NOT_TO_COME))
    {
        fdScriptSetMessageId((Sint32)MsgSusaDonT);
        // "Susa, don't come for me..."
        fdScriptShowMessageAndWait(ACTOR_KUSHINADA, 0);
        fdScriptWait(60);
        // "If I see you, I'll surely lose the courage to sacrifice myself for..."
        fdScriptShowMessageAndWait(ACTOR_KUSHINADA, 0);
        fdScriptWait(60);
        fdScriptWait(15);
        // "If I run, Lady Uzume and all of Izumo will suffer greatly......"
        fdScriptShowMessageAndWait(ACTOR_KUSHINADA, 0);
        fdScriptWait(60);
        fdScriptWait(15);
        // "Lady Uzume has foretold the coming of a hero, and I believe her..."
        fdScriptShowMessageAndWait(ACTOR_KUSHINADA, 0);
        fdScriptWait(60);
        fdActorStartRepeatedMotion(ACTOR_KUSHINADA, 1);
        fdScriptWait(30);
        // "And so I shall remain here, until the coming of the next full..."
        fdScriptShowMessageAndWait(ACTOR_KUSHINADA, 0);
        fdScriptWait(60);
        fdScriptSetFlag(FLAG_KUSHINADA_ASKED_SUSA_NOT_TO_COME);
    }
    else
    {
        fdScriptSetMessageId((Sint32)MsgSusaPleaseDon);
        // "Susa, please... Don't try to save me."
        fdScriptShowMessageAndWait(ACTOR_KUSHINADA, 0);
    }
    fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorWalkByById(MainRoutineStateWork.leaderActorId, 0, 16);
    fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
    fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlags |= ACTOR_FACES_MOTION;
    fdScriptEnd();
    return 0;
}

// The first time the party comes into Susa's house after the Serpent is slain, Felix walking up the
// east side of the hall (metatile (12, 35)) comes upon Lady Uzume and Kushinada hearing that
// warriors came out of the mountain, and the welcome of the heroes begins.
void scIzumoRoomsHeroesComeFromTheEast(void)
{
    scIzumoRoomsRecognizeTheHeroes(HEROES_FROM_THE_EAST);
}

// The first time the party comes into Susa's house after the Serpent is slain, Felix walking up the
// west side of the hall (metatile (5, 35)) comes upon Lady Uzume and Kushinada hearing that
// warriors came out of the mountain, and the welcome of the heroes begins.
void scIzumoRoomsHeroesComeFromTheWest(void)
{
    scIzumoRoomsRecognizeTheHeroes(HEROES_FROM_THE_WEST);
}

// Susa's answers to Sheba's "Don't forget, Susa dealt the finishing blow, didn't he?", from
// MsgYouReNotImplying on.
typedef enum FINISHING_BLOW_MESSAGE
{
    // "Finishing blow? All I did was stab it with the Cloud Brand."
    SUSA_ALL_I_DID_WAS_STAB_IT   = 10,
    // "That's right. Piercing it with the Cloud Brand was hardly a finishing blow."
    SUSA_HARDLY_A_FINISHING_BLOW = 11
} FINISHING_BLOW_MESSAGE;

// The party's answers to Kraden's "Even Felix agrees, don't you, Felix?", from MsgIDisagree on.
typedef enum SUSAS_REWARD_MESSAGE
{
    // Yes: "I do, too.", "We weren't fighting in hopes of getting a reward, were we?", "We'll be
    // satisfied just learning your Psynergy."
    PARTY_WANTS_NO_REWARD    = 4,
    // No: "Are you asking for a reward? That's just plain greedy!", "We couldn't possibly take any
    // sort of reward.", "Learning new Psynergy will be reward enough."
    PARTY_CALLS_FELIX_GREEDY = 7
} SUSAS_REWARD_MESSAGE;

// The heroes' welcome in Susa's house. A villager tells Lady Uzume and Kushinada that warriors came
// out of the mountain after the Serpent's roar; Lady Uzume orders the strangers out of her home
// until the villager knows them for the heroes, and she bids them welcome. The party introduce
// themselves, and Lady Uzume shows them her Move. Susa walks in from the entrance, Kraden explains
// Psynergy and Alchemy, and Felix answers three questions: whether the party slew the Serpent only
// for its quest, whether Susa dealt the finishing blow, and whether Susa deserves the reward. The
// party say they must leave for Lemuria and vow to return.
//
// The party, Lady Uzume, the villager and Susa walk on the side of the hall Felix came up.
void scIzumoRoomsRecognizeTheHeroes(Sint32 side)
{
    ACTOR *leader;
    Sint32 answer, msg;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdCameraMoveTo(143, CAMERA_KEEP, 560, 1);
    if (side == HEROES_FROM_THE_EAST)
    {
        fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 184, 568);
    }
    else
    {
        fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 104, 568);
    }
    syFieldScriptWait(15);
    syFieldScriptSetMessageId((Sint32)MsgWarriorsAppearedFrom);
    // "Warriors appeared from within Mt. Mikage?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    fdActorSetAnimation(ACTOR_WITNESS, ANIM_NOD);
    // "Yes... After the serpent's roar, these warriors appeared."
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WITNESS, 0);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    // "So, it wasn't Susa who defeated the serpent, but those heroes..."
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    syFieldScriptWait(30);
    fdActorSetAnimation(ACTOR_WITNESS, ANIM_NOD);
    syFieldScriptWait(15);
    // "It might have been..."
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WITNESS, 0);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
    scFieldActorStartRepeatedMotion(ACTOR_WITNESS, 3);
    fdActorSetAttachedEffect(ACTOR_WITNESS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    syFieldScriptWait(30);
    // "Lady Kushinada, please forgive me..."
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WITNESS, 0);
    fdActorFaceDirection(ACTOR_SAVED_KUSHINADA, FACING_EAST, 30);
    fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_SHAKE_HEAD);
    // "I don't care who defeated the serpent... I'm more worried about what..."
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
    syFieldScriptWait(30);
    fdActorFaceDirection(ACTOR_UZUME, FACING_WEST, 30);
    fdActorSetAnimation(ACTOR_UZUME, ANIM_NOD);
    syFieldScriptWait(30);
    // "So, Susa never came down from Mt. Mikage?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    fdActorSetAnimation(ACTOR_WITNESS, ANIM_NOD);
    syFieldScriptWait(15);
    // "He returned to the village after the heroes appeared, and he knew..."
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WITNESS, 0);
    scFieldActorStartRepeatedMotion(ACTOR_SAVED_KUSHINADA, 2);
    syFieldScriptWait(15);
    // "Oh, Susa..."
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
    fdActorFaceActor(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
    syFieldScriptWait(30);
    fdActorSetDestinationOffset(ACTOR_UZUME, 0, 16);
    scFieldActorWaitForMoveById(ACTOR_UZUME);
    syFieldScriptWait(15);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    if (side == HEROES_FROM_THE_EAST)
    {
        fdActorWalkToAndWait(ACTOR_UZUME, 168, 536);
    }
    else
    {
        fdActorWalkToAndWait(ACTOR_UZUME, 120, 536);
    }
    fdActorFaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
    fdActorFaceActor(ACTOR_SAVED_KUSHINADA, MainRoutineStateWork.leaderActorId, 0);
    // "Who are you?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
    syFieldScriptWait(30);
    // "This is my home, and I am having a private conversation. Leave at once!"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    scFieldActorJump(ACTOR_WITNESS, 4, 0);
    fdActorSetAttachedEffect(ACTOR_WITNESS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    syFieldScriptWait(30);
    fdActorFaceDirection(ACTOR_SAVED_KUSHINADA, FACING_SOUTH, 30);
    fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "What is it?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
    fdActorFaceActor(ACTOR_WITNESS, ACTOR_SAVED_KUSHINADA, 0);
    fdActorSetAttachedEffect(ACTOR_WITNESS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    // "There can be no doubt... These are the heroes who appeared on Mt...."
    fdScriptShowMessage(ACTOR_WITNESS, 0);
    syFieldScriptWait(30);
    syFieldScriptWait(30);
    // "Are you the ones who defeated the serpent?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    scFieldActorStartRepeatedMotion(ACTOR_SAVED_KUSHINADA, 2);
    syFieldScriptWait(30);
    fdActorFaceActor(ACTOR_SAVED_KUSHINADA, MainRoutineStateWork.leaderActorId, 0);
    // "So it was you..."
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "My apologies... You are welcome here."
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    fdActorWalkTo(ACTOR_UZUME, 144, 520);
    scFieldActorWaitForMoveById(ACTOR_UZUME);
    fdActorFaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_SAVED_KUSHINADA, FACING_SOUTH, 0);
    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    syFieldScriptWait(30);
    if (side == HEROES_FROM_THE_EAST)
    {
        fdActorWalkTo(ACTOR_WITNESS, 104, 568);
    }
    else
    {
        fdActorWalkTo(ACTOR_WITNESS, 184, 568);
    }
    scFieldActorWaitForMoveById(ACTOR_WITNESS);
    if (side == HEROES_FROM_THE_EAST)
    {
        fdActorFaceDirection(ACTOR_WITNESS, FACING_EAST, 0);
    }
    else
    {
        fdActorFaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
    }
    fdActorWalkTo(MainRoutineStateWork.leaderActorId, 144, 560);
    scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
    fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
    scFieldActorLaunchFromLeader(ACTOR_JENNA, 8, 24, FACING_NORTH);
    scFieldActorLaunchFromLeader(ACTOR_SHEBA, 24, 24, FACING_NORTH);
    scFieldActorLaunchFromLeader(ACTOR_PIERS, -8, 24, FACING_NORTH);
    scFieldActorLaunchFromLeader(ACTOR_KRADEN, -24, 24, FACING_NORTH);
    syFieldScriptWait(60);
    // "Who are you?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
    fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
    syFieldScriptWait(30);
    // "I'm Kraden, and this is Felix."
    fdScriptShowMessage(ACTOR_KRADEN, 0);
    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    syFieldScriptWait(30);
    fdActorSetAnimation(ACTOR_UZUME, ANIM_NOD);
    fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
    syFieldScriptWait(30);
    fdActorFaceActor(ACTOR_KRADEN, ACTOR_PIERS, 0);
    syFieldScriptWait(30);
    // "Behind him is Piers."
    fdScriptShowMessage(ACTOR_KRADEN, 0);
    fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
    syFieldScriptWait(30);
    // "Pleased to meet you."
    fdScriptShowMessage(ACTOR_PIERS, 0);
    fdActorSetAnimation(ACTOR_UZUME, ANIM_NOD);
    fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
    syFieldScriptWait(30);
    fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
    syFieldScriptWait(30);
    // "I'm Jenna, and this is Sheba."
    fdScriptShowMessage(ACTOR_JENNA, 0);
    fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
    syFieldScriptWait(30);
    fdActorSetAnimation(ACTOR_UZUME, ANIM_NOD);
    fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
    syFieldScriptWait(30);
    fdActorFaceActor(ACTOR_JENNA, ACTOR_KRADEN, 0);
    fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
    syFieldScriptWait(30);
    // "We're on a quest. Our travels across the Great Eastern Sea brought..."
    fdScriptShowMessage(ACTOR_KRADEN, 0);
    fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
    syFieldScriptWait(30);
    // "You were seen coming down from Mt. Mikage. What brought you to such..."
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
    fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
    syFieldScriptWait(30);
    // "To complete our quest, we need many types of Psynergy."
    fdScriptShowMessage(ACTOR_SHEBA, 0);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "Psynergy?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    fdActorFaceEachOther(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
    syFieldScriptWait(60);
    fdActorFaceActor(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
    fdActorFaceActor(ACTOR_SAVED_KUSHINADA, MainRoutineStateWork.leaderActorId, 0);
    syFieldScriptWait(30);
    fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_SHAKE_HEAD);
    syFieldScriptWait(30);
    // "What is Psynergy?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
    fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_KRADEN, 0);
    fdActorFaceActor(ACTOR_JENNA, ACTOR_KRADEN, 0);
    fdActorFaceActor(ACTOR_SHEBA, ACTOR_KRADEN, 0);
    fdActorFaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
    syFieldScriptWait(30);
    fdActorFaceActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
    syFieldScriptWait(30);
    fdActorFaceActor(ACTOR_KRADEN, ACTOR_UZUME, 0);
    syFieldScriptWait(30);
    // "Psynergy is many things... The power to move things with one's mind..."
    fdScriptShowMessage(ACTOR_KRADEN, 0);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    syFieldScriptWait(30);
    // "All with one's mind?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_UZUME, 0);
    fdActorFaceActor(ACTOR_JENNA, ACTOR_UZUME, 0);
    fdActorFaceActor(ACTOR_SHEBA, ACTOR_UZUME, 0);
    fdActorFaceActor(ACTOR_PIERS, ACTOR_UZUME, 0);
    syFieldScriptWait(30);
    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
    fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
    syFieldScriptWait(30);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "Tell me, is it something like...this?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    syFieldScriptWait(60);
    fdActorFaceDirection(ACTOR_UZUME, FACING_EAST, 0);
    syFieldScriptWait(30);
    scIzumoRoomsShowMove();
    syFieldScriptWait(30);
    fdActorFaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
    syFieldScriptWait(30);
    // "Is that the power you're talking about?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    fdActorShowEmote(MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    syFieldScriptWait(30);
    scFieldActorStartRepeatedMotion(ACTOR_JENNA, 1);
    syFieldScriptWait(15);
    // "That power? Lady Uzume?"
    fdScriptShowMessage(ACTOR_JENNA, 0);
    fdActorFaceActor(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
    syFieldScriptWait(30);
    fdActorFaceActor(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
    syFieldScriptWait(30);
    // "It's not mine alone. Kushinada and Susa possess it as well."
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    syFieldScriptWait(30);
    // "This is the power we possess. I assume you have something like it..."
    fdScriptOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
    answer = fdScriptAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId, 0);
    if (answer == 1)
    {
        syFieldScriptSetMessageId((Sint32)MsgIThoughtSo);
        fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
        // "I thought so. Only those who can wield the power can see it. And you..."
        fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
        fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
        fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
        syFieldScriptWait(30);
        fdActorFaceActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId, 0);
        syFieldScriptWait(15);
        fdActorSetAnimation(ACTOR_PIERS, ANIM_SHAKE_HEAD);
        syFieldScriptWait(30);
        // "Felix, there's no need to hide our abilities from them, is there?"
        fdScriptShowMessage(ACTOR_PIERS, 0);
    }
    syFieldScriptSetMessageId((Sint32)MsgSoYouCall);
    fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
    syFieldScriptWait(30);
    // "So, you call what we do "Psynergy," and you can do it, too?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
    fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
    syFieldScriptWait(30);
    fdActorSetAnimation(ACTOR_UZUME, ANIM_NOD);
    fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
    syFieldScriptWait(60);
    fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    fdActorFaceActor(ACTOR_SAVED_KUSHINADA, MainRoutineStateWork.leaderActorId, 0);
    syFieldScriptWait(30);
    // "If you already have Psynergy, why do you need more of this power?"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
    syFieldScriptWait(90);
    // "Their powers are different from our own. There seem to be many forms..."
    fdScriptShowMessage(ACTOR_SUSA, 0);
    fdActorSetPos(ACTOR_SUSA, 144, 680);
    fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
    fdActorFaceActor(ACTOR_JENNA, ACTOR_SUSA, 0);
    fdActorFaceActor(ACTOR_SHEBA, ACTOR_SUSA, 0);
    fdActorFaceActor(ACTOR_PIERS, ACTOR_SUSA, 0);
    fdActorFaceActor(ACTOR_KRADEN, ACTOR_SUSA, 0);
    fdActorFaceActor(ACTOR_WITNESS, ACTOR_SUSA, 0);
    syFieldScriptWait(120);
    // "Susa!"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
    syFieldScriptWait(30);
    syScriptCameraFollowActor(ACTOR_SUSA, 1);
    syFieldScriptWait(120);
    fdActorSetSpeed(ACTOR_SUSA, 0.6);
    fdActorSetSpeed(ACTOR_SUSA, 0.6);
    fdActorGetById(ACTOR_SUSA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    fdActorWalkTo(ACTOR_SUSA, 144, 648);
    if (side == HEROES_FROM_THE_EAST)
    {
        fdActorWalkToAndWait(ACTOR_SUSA, 88, 648);
        fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
        fdActorWalkToAndWait(ACTOR_SUSA, 56, 616);
        fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
        fdActorWalkToAndWait(ACTOR_SUSA, 56, 584);
        fdActorGetById(ACTOR_WITNESS)->steeringFlags &= ~ACTOR_FACES_MOTION;
        fdActorWalkToAndWait(ACTOR_WITNESS, 88, 568);
        fdScriptWait(1);
        fdActorGetById(ACTOR_WITNESS)->steeringFlags |= ACTOR_FACES_MOTION;
        fdActorFaceDirectionById(ACTOR_WITNESS, FACING_EAST, 0);
        fdActorSetSpritePriority(ACTOR_SUSA, 1);
        fdActorWalkToAndWait(ACTOR_SUSA, 104, 568);
        fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
        fdActorSetSpritePriority(ACTOR_KRADEN, 1);
        fdActorSetSpritePriority(ACTOR_SUSA, 2);
        fdActorWalkToAndWaitById(ACTOR_SUSA, 120, 568);
        fdActorSetSpritePriority(ACTOR_SUSA, 3);
        fdActorSetSpritePriority(ACTOR_KRADEN, 3);
        fdActorWalkToAndWait(ACTOR_SUSA, 120, 536);
        fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
        fdActorSetAnimationById(ACTOR_WITNESS, ANIM_NOD);
        fdActorWalkToAndWait(ACTOR_WITNESS, 56, 584);
        fdActorWalkToAndWait(ACTOR_WITNESS, 88, 648);
        fdActorSetPos(ACTOR_WITNESS, 0, 0);
    }
    else
    {
        fdActorWalkToAndWait(ACTOR_SUSA, 200, 648);
        fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
        fdActorWalkToAndWait(ACTOR_SUSA, 232, 616);
        fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
        fdActorWalkToAndWait(ACTOR_SUSA, 232, 584);
        fdActorGetById(ACTOR_WITNESS)->steeringFlags &= ~ACTOR_FACES_MOTION;
        fdActorWalkToAndWait(ACTOR_WITNESS, 200, 568);
        fdScriptWait(1);
        fdActorGetById(ACTOR_WITNESS)->steeringFlags |= ACTOR_FACES_MOTION;
        fdActorFaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
        fdActorSetSpritePriority(ACTOR_SUSA, 1);
        fdActorWalkToAndWait(ACTOR_SUSA, 184, 568);
        fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
        fdActorSetSpritePriority(ACTOR_SUSA, 2);
        fdActorSetSpritePriority(ACTOR_SHEBA, 1);
        fdActorWalkToAndWaitById(ACTOR_SUSA, 168, 568);
        fdActorSetSpritePriority(ACTOR_SUSA, 3);
        fdActorSetSpritePriority(ACTOR_SHEBA, 3);
        fdActorWalkToAndWait(ACTOR_SUSA, 168, 536);
        fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
        fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
        fdActorSetAnimationById(ACTOR_WITNESS, ANIM_NOD);
        fdActorWalkToAndWait(ACTOR_WITNESS, 232, 584);
        fdActorWalkToAndWait(ACTOR_WITNESS, 200, 648);
        fdActorSetPos(ACTOR_WITNESS, 0, 0);
    }
    fdActorGetById(ACTOR_SUSA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
    fdActorFaceDirectionById(ACTOR_KRADEN, FACING_NORTH, 0);
    fdScriptWait(15);
    // "Susa..."
    fdScriptShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
    fdScriptWait(15);
    fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "So, you lived after all, Susa..."
    fdScriptShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
    fdActorFaceActorById(ACTOR_SUSA, ACTOR_SAVED_KUSHINADA, 0);
    fdScriptWait(30);
    // "Yes, I've survived, and I've made it home to you both."
    fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
    fdActorFaceActorById(ACTOR_SUSA, MainRoutineStateWork.leaderActorId, 0);
    fdScriptWait(60);
    fdActorFaceActorById(ACTOR_SUSA, ACTOR_SAVED_KUSHINADA, 0);
    fdScriptWait(30);
    // "Thanks to them, actually..."
    fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
    fdActorTurnToFaceEachOther(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
    fdScriptWait(30);
    fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
    fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, MainRoutineStateWork.leaderActorId, 0);
    // "Felix, our gratitude to you and your friends grows deeper each..."
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdActorSetAnimationById(ACTOR_UZUME, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
    fdScriptWait(30);
    fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "Susa, you spoke of different types of power. Tell me, what did you mean?"
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdActorFaceActorById(ACTOR_SUSA, MainRoutineStateWork.leaderActorId, 0);
    // "I can only describe what I saw, my sister. Ask Felix here about it."
    fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
    fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
    fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, MainRoutineStateWork.leaderActorId, 0);
    fdScriptWait(30);
    fdActorShowEmoteById(MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    fdScriptWait(30);
    // "Ah, yes. Psynergy... I have dedicated most of my life to its study..."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorSetAnimationById(ACTOR_UZUME, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
    fdScriptWait(30);
    fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    // "The power of Psynergy comes from the four basic forces of earth..."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    // "You don't know Alchemy? The four elements? Why, they're the very..."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "All existing matter comes from Alchemy and this Psynergy?"
    fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
    fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
    fdScriptWait(30);
    // "Yes, that's right. Everything you see around you is some combination..."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorShowEmoteById(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
    // "Does this mean that there are as many types of Psynergy as there are..."
    fdScriptShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
    fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "That's right, Kushinada. You're a smart lass."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    fdScriptWait(30);
    // "Mt. Mikage was a place of great earth powers. I'd imagine your..."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "That's right, but how did you know this?"
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdActorFaceActorById(ACTOR_PIERS, ACTOR_KRADEN, 0);
    fdScriptWait(30);
    // "Kraden... Mt. Mikage is Gaia Rock, isn't it?"
    fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
    fdActorFaceActorById(ACTOR_KRADEN, ACTOR_PIERS, 0);
    fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    fdScriptWait(30);
    fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    fdActorFaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
    fdActorFaceActorById(ACTOR_PIERS, ACTOR_UZUME, 0);
    fdScriptWait(30);
    // "...Gaia Rock? Mt. Mikage is the sacred mount of our people!"
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "Yes, but there are many mountains like your Mt. Mikage all across..."
    fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
    fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    // "You have seen other mountains like our Mt. Mikage?"
    fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
    fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
    fdScriptWait(30);
    // "Yes! In fact, it seems that each of these rocks contains a different..."
    fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
    fdActorSetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
    fdScriptWait(30);
    // "And now, you are questing to gather the power of these elements, yes?"
    fdScriptShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
    fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    fdScriptWait(90);
    fdActorSetAnimationById(ACTOR_UZUME, ANIM_NOD);
    // "And you only defeated the Great Serpent of Mikage to accomplish this..."
    fdScriptOpenMessage(ACTOR_UZUME, 0);
    answer = fdScriptAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId, 0);
    if (answer == 0)
    {
        fdScriptSetMessageId((Sint32)MsgRegardlessOfYour);
        fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
        // "Regardless of your motives, you have saved us all."
        fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    }
    else
    {
        fdScriptSetMessageId((Sint32)MsgYouTookPity);
        // "You took pity on Kushinada? You are very kind to help Susa, Felix."
        fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    }
    msg = (Sint32)MsgYouReNotImplying;
    fdScriptSetMessageId(msg);
    fdActorStartRepeatedMotion(ACTOR_KRADEN, 1);
    fdScriptWait(30);
    // "You're...not implying that we defeated the serpent, are you?"
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorFaceActorById(ACTOR_UZUME, ACTOR_KRADEN, 0);
    fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    fdScriptWait(30);
    // "It's true that we did fight the creature..."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
    fdScriptWait(30);
    // "But without Susa's power, the battle might not have ended so happily."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorTurnToFaceEachOther(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
    fdScriptWait(30);
    // "What... What are you°"
    fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
    fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
    fdScriptWait(30);
    // "Susa had given the beast a lot of Dragonsbane in order to weaken it."
    fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
    fdActorFaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
    fdActorFaceDirectionById(ACTOR_SAVED_KUSHINADA, FACING_SOUTH, 0);
    fdScriptWait(30);
    fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    fdScriptWait(30);
    // "We could never have beaten the beast if it had not been in..."
    fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
    fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "We were victorious only because Susa risked everything to weaken the..."
    fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
    fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    fdScriptWait(15);
    fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
    // "Susa, perhaps, after all, you are..."
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "No, I didn't..."
    fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
    fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
    fdActorFaceActorById(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId, 0);
    // "Don't forget, Susa dealt the finishing blow, didn't he?"
    fdScriptOpenMessage(ACTOR_SHEBA, 0);
    fdActorFaceActorById(ACTOR_JENNA, MainRoutineStateWork.leaderActorId, 0);
    fdActorFaceActorById(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId, 0);
    fdActorFaceActorById(ACTOR_PIERS, MainRoutineStateWork.leaderActorId, 0);
    fdActorFaceActorById(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
    fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
    fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, MainRoutineStateWork.leaderActorId, 0);
    fdActorFaceActorById(ACTOR_SUSA, MainRoutineStateWork.leaderActorId, 0);
    fdScriptWait(30);
    answer = fdScriptAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId, 0);
    if (answer == 0)
    {
        fdScriptSetMessageId(msg + SUSA_ALL_I_DID_WAS_STAB_IT);
        fdActorSetAnimationById(ACTOR_SUSA, ANIM_SHAKE_HEAD);
        fdScriptWait(30);
        // "Finishing blow? All I did was stab it with the Cloud Brand."
        fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
    }
    else
    {
        fdScriptSetMessageId(msg + SUSA_HARDLY_A_FINISHING_BLOW);
        fdActorSetAnimationById(ACTOR_SUSA, ANIM_NOD);
        fdScriptWait(30);
        // "That's right. Piercing it with the Cloud Brand was hardly a..."
        fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
    }
    msg = (Sint32)MsgIDisagree;
    fdScriptSetMessageId(msg);
    fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
    fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "I disagree..."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    fdScriptWait(30);
    // "You weakened the serpent. You dealt the final blow. I'd say you were..."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
    fdScriptWait(30);
    fdActorFaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
    fdScriptWait(30);
    // "So, Lady Uzume, wouldn't you say that Susa deserves the reward for..."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
    fdActorFaceActorById(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
    fdScriptWait(30);
    // "Even Felix agrees, don't you, Felix?"
    fdScriptOpenMessage(ACTOR_KRADEN, 0);
    answer = fdScriptAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId, 0);
    if (answer == 0)
    {
        fdScriptSetMessageId(msg + PARTY_WANTS_NO_REWARD);
        fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
        fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
        fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
        fdScriptWait(30);
        // "I do, too."
        fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
        // "We weren't fighting in hopes of getting a reward, were we?"
        fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
        // "We'll be satisfied just learning your Psynergy."
        fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
    }
    else
    {
        fdScriptSetMessageId(msg + PARTY_CALLS_FELIX_GREEDY);
        fdActorSetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
        fdActorSetAnimationById(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
        fdActorSetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
        fdScriptWait(30);
        // "Are you asking for a reward? That's just plain greedy!"
        fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
        // "We couldn't possibly take any sort of reward."
        fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
        // "Learning new Psynergy will be reward enough."
        fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
    }
    fdScriptSetMessageId((Sint32)MsgAsYouCanSee);
    fdActorFaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
    fdScriptWait(30);
    // "As you can see."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorTurnToFaceEachOther(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
    fdScriptWait(30);
    fdActorSetAnimationById(ACTOR_UZUME, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
    fdScriptWait(30);
    fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
    fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, MainRoutineStateWork.leaderActorId, 0);
    fdActorStartRepeatedMotion(ACTOR_SUSA, 1);
    fdScriptWait(15);
    fdActorShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
    fdActorFaceActorById(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
    fdScriptWait(30);
    // "Now, it's about time for us to leave Izumo Village."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_KRADEN, 0);
    fdScriptWait(15);
    fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    fdScriptWait(30);
    fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    fdScriptWait(30);
    // "Yes, indeed! We're on our way to Lemuria."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorFaceActorById(ACTOR_JENNA, ACTOR_KRADEN, 0);
    fdActorFaceActorById(ACTOR_SHEBA, ACTOR_KRADEN, 0);
    fdActorFaceActorById(ACTOR_PIERS, ACTOR_KRADEN, 0);
    fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
    fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdScriptWait(30);
    // "You're leaving so soon?"
    fdScriptShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
    fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_UZUME, 0);
    fdActorFaceActorById(ACTOR_JENNA, ACTOR_UZUME, 0);
    fdActorFaceActorById(ACTOR_SHEBA, ACTOR_UZUME, 0);
    fdActorFaceActorById(ACTOR_PIERS, ACTOR_UZUME, 0);
    fdActorFaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
    fdScriptWait(30);
    // "We are in a great hurry."
    fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
    fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "Then you must return to Izumo someday."
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
    fdScriptWait(30);
    // "I'm sure we'll be back once our mission is completed."
    fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
    fdActorSetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
    fdScriptWait(30);
    // "Excellent! I shall accept that as your oath!"
    fdScriptShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
    fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdScriptWait(30);
    // "...Oath?"
    fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
    fdActorFaceActorById(ACTOR_SHEBA, ACTOR_JENNA, 0);
    fdScriptWait(30);
    // "Oh, by promising to return, we'll be ensuring that our quest will..."
    fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
    fdActorStartRepeatedMotion(ACTOR_PIERS, 1);
    fdScriptWait(30);
    // "I understand... We must vow to return, everyone."
    fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
    fdActorTurnToFaceEachOther(MainRoutineStateWork.leaderActorId, ACTOR_PIERS, 0);
    fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
    fdActorTurnToFaceEachOther(ACTOR_JENNA, ACTOR_SHEBA, 0);
    fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    fdScriptWait(15);
    fdActorTurnToFaceEachOther(MainRoutineStateWork.leaderActorId, ACTOR_JENNA, 0);
    fdActorTurnToFaceEachOther(ACTOR_PIERS, ACTOR_SHEBA, 0);
    fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    fdScriptWait(30);
    fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_UZUME, 0);
    fdActorFaceActorById(ACTOR_JENNA, ACTOR_UZUME, 0);
    fdActorFaceActorById(ACTOR_SHEBA, ACTOR_UZUME, 0);
    fdActorFaceActorById(ACTOR_PIERS, ACTOR_UZUME, 0);
    fdActorFaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
    fdActorSetAnimationById(ACTOR_UZUME, ANIM_NOD);
    fdScriptWait(30);
    // "We'll be waiting."
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    fdScriptWait(15);
    fdActorSetAnimationById(ACTOR_UZUME, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
    fdActorSetAnimationById(ACTOR_SUSA, ANIM_NOD);
    fdScriptWait(60);
    fdActorSetAnimationById(ACTOR_JENNA, ANIM_WALK);
    leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    if (leader != 0)
        fdActorSetDestinationById(
            ACTOR_JENNA, leader->x.part[0].integer, leader->z.part[0].integer);
    fdActorWaitForMoveById(ACTOR_JENNA);
    fdActorSetPos(ACTOR_JENNA, 0, 0);
    fdActorSetAnimationById(ACTOR_SHEBA, ANIM_WALK);
    leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    if (leader != 0)
        fdActorSetDestinationById(
            ACTOR_SHEBA, leader->x.part[0].integer, leader->z.part[0].integer);
    fdActorWaitForMoveById(ACTOR_SHEBA);
    fdActorSetPos(ACTOR_SHEBA, 0, 0);
    fdActorSetAnimationById(ACTOR_PIERS, ANIM_WALK);
    leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    if (leader != 0)
        fdActorSetDestinationById(
            ACTOR_PIERS, leader->x.part[0].integer, leader->z.part[0].integer);
    fdActorWaitForMoveById(ACTOR_PIERS);
    fdActorSetPos(ACTOR_PIERS, 0, 0);
    fdActorSetAnimationById(ACTOR_KRADEN, ANIM_WALK);
    leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    if (leader != 0)
        fdActorSetDestinationById(
            ACTOR_KRADEN, leader->x.part[0].integer, leader->z.part[0].integer);
    fdActorWaitForMoveById(ACTOR_KRADEN);
    fdActorSetPos(ACTOR_KRADEN, 0, 0);
    syGameFlagSet(FLAG_IZUMO_RECOGNIZED_THE_HEROES);
    syGameFlagSet(FLAG_HEROES_JUST_RECOGNIZED);
    fdScriptEnd();
}

// Lady Uzume's Move in the welcome has nothing to move.
Sint32 scIzumoRoomsFindNoMoveTarget(void)
{
    return -1;
}

// Lady Uzume casts Move in the welcome ("Tell me, is it something like...this?") with nothing to
// move.
void scIzumoRoomsShowMove(void)
{
    fdPsynergyLoadResources(ABILITY_MOVE, 1);
    fdPsynergySetupActorPair(ACTOR_UZUME, -1);
    ((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffectState)->finishCallback =
        (void (*)(void))scIzumoRoomsFindNoMoveTarget;
    ((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffectState)->aimAhead = 1;
    fdPsynergyStartCast();
    fdScriptWait(10);
    fdPsynergyRunHandler(0);
    fdBattleEffectStopMainActor();
    fdPsynergyEnd();
}

// Using the Dancing Idol on Lady Uzume in Susa's house: Felix steps up to her, she asks whether it
// was found at the peak of the mountain, and in thanks gives the party her pet, Coal, the Mars
// Djinni.
void scIzumoRoomsGiveUzumeTheDoll(void)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdScriptSetMessageId((Sint32)&MsgAhCouldIt);
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 144, 536);
    fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
    fdScriptWait(15);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    // "Ah... Could it be? Is this the dancing doll of Mt. Mikage?"
    fdScriptOpenMessage(ACTOR_UZUME, 0);
    if (!fdScriptAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId, 0))
    {
        fdScriptSetMessageId((Sint32)&MsgAhCouldIt + 1);
        // "You found this at the peak of Mt. Mikage?"
        fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    }
    else
    {
        fdScriptSetMessageId((Sint32)&MsgAhCouldIt + 2);
        // "Don't lie to me. The style, the craftsmanship... You must have found..."
        fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    }
    fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 60);
    fdScriptSetMessageId((Sint32)&MsgAgainYouHave);
    // "Again, you have done more for me than I can properly reward you for..."
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
    fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
    // "I think I may have something that might please you."
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdBattleEffectStartArcEffect();
    fdBattleEffectSetObjectUpdateMode(fdActorGetById(ACTOR_UZUME), CASTER_LOOK_CYCLING_COLORS);
    sySoundPlay(SE_ASCEND);
    fdScriptWait(60);
    fdActorSetPos(ACTOR_COAL, 144, 520);
    fdActorJump(ACTOR_COAL, 6, 0);
    fdActorSetDestination(ACTOR_COAL, 160, 520);
    fdActorWaitForMoveById(ACTOR_COAL);
    fdActorFaceDirectionById(ACTOR_COAL, FACING_SOUTH, 0);
    fdScriptWait(60);
    fdBattleEffectSetObjectUpdateMode(fdActorGetById(ACTOR_UZUME), CASTER_LOOK_NORMAL);
    fdBattleEffectWaitForArcGroups();
    fdBattleEffectEndArcEffect();
    fdScriptWait(30);
    fdScriptWait(30);
    fdActorFaceActorById(ACTOR_UZUME, ACTOR_COAL, 0);
    fdScriptWait(30);
    // "This is my pet. It's very dear to me."
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
    fdScriptWait(60);
    // "I am unsure how it works, but this fellow seems to magnify our power..."
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdScriptWait(30);
    fdActorSetAnimationById(ACTOR_UZUME, ANIM_SHAKE_HEAD);
    fdScriptWait(60);
    // "Though it saddens me to part with it, I would have you take him, as..."
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdActorTurnToFaceEachOther(ACTOR_UZUME, ACTOR_COAL, 0);
    // "Felix and his friends have agreed to care for you from now on."
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdScriptWait(60);
    fdActorJump(ACTOR_COAL, 4, 20);
    fdScriptWait(60);
    fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
    fdActorFaceActorById(ACTOR_COAL, MainRoutineStateWork.leaderActorId, 0);
    fdScriptWait(60);
    // "Please, accept this gift."
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdActorJump(ACTOR_COAL, 2, 20);
    fdDjinnJoinParty(ACTOR_COAL, DJINNI_COAL / DJINN_PER_ELEMENT, DJINNI_INDEX(DJINNI_COAL));
    fdScriptEnd();
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_UZUME, 0);
    fdScriptWait(60);
    fdActorStartRepeatedMotion(ACTOR_UZUME, 2);
    fdScriptWait(30);
    // "With this dancing doll, I can explore Mt. Mikage on my own. Take..."
    fdScriptShowMessageAndWait(ACTOR_UZUME, 0);
    fdScriptWait(60);
    syGameFlagSet(FLAG_UZUME_HAS_THE_DOLL);
    gmPartyInventoryRemove(ITEM_DANCING_IDOL);
    fdScriptEnd();
}

// Sets up the room the party enters: the window in, Susa's house for the heroes' welcome or after
// it, Kushinada sobbing on her seat before the Serpent is slain, and the dancers and old men in
// Susa's house before it. Returns 0.
Sint32 scInit(void)
{
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
    if (MainRoutineStateWork.entrance != IZUMO_ROOMS_FROM_IZUMO_NORTH)
        fdScriptClearFlag(FLAG_HEROES_JUST_RECOGNIZED);
    if (syGameFlagGet(FLAG_SERPENT_SLAIN) && !syGameFlagGet(FLAG_IZUMO_RECOGNIZED_THE_HEROES)
        && MainRoutineStateWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH)
    {
        fdScriptPlaySound(MUS_IZUMO);
        fdActorSetPos(ACTOR_WITNESS, 144, 568);
        fdActorSetPos(ACTOR_SUSA, 0, 0);
        fdActorSetPos(ACTOR_FIRST_DANCER, 0, 0);
        fdActorSetPos(ACTOR_SECOND_DANCER, 0, 0);
        fdActorSetPos(ACTOR_WEST_VILLAGER, 0, 0);
        fdActorSetPos(ACTOR_EAST_VILLAGER, 0, 0);
    }
    if (syGameFlagGet(FLAG_SERPENT_SLAIN) && syGameFlagGet(FLAG_IZUMO_RECOGNIZED_THE_HEROES)
        && MainRoutineStateWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH
        && syGameFlagGet(FLAG_HEROES_JUST_RECOGNIZED))
    {
        fdScriptPlaySound(MUS_IZUMO);
        fdActorSetPos(ACTOR_FIRST_DANCER, 0, 0);
        fdActorSetPos(ACTOR_SECOND_DANCER, 0, 0);
        fdActorSetPos(ACTOR_WEST_VILLAGER, 0, 0);
        fdActorSetPos(ACTOR_EAST_VILLAGER, 0, 0);
    }
    if (MainRoutineStateWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTHWEST
        && !syGameFlagGet(FLAG_SERPENT_SLAIN))
    {
        fdActorSetAnimation(ACTOR_KUSHINADA, ANIM_KNEEL);
        fdActorSetAttachedEffect(ACTOR_KUSHINADA,
                                 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    }
    if (MainRoutineStateWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH
        && !syGameFlagGet(FLAG_SERPENT_SLAIN))
    {
        ACTOR *actor;

        fdActorSetAnimation(ACTOR_SUSAS_HOUSE_FIRST_DANCER, ANIM_DANCE);
        fdActorSetAnimation(ACTOR_SUSAS_HOUSE_SECOND_DANCER, ANIM_DANCE);
        fdActorSetAnimation(ACTOR_SUSAS_HOUSE_WEST_VILLAGER, ANIM_BEAT_DRUM);
        fdActorSetAnimation(ACTOR_SUSAS_HOUSE_EAST_VILLAGER, ANIM_BEAT_DRUM);
        actor         = fdActorGetById(ACTOR_SUSAS_HOUSE_WEST_VILLAGER);
        actor->scaleX = -fdActorGetById(ACTOR_SUSAS_HOUSE_WEST_VILLAGER)->scaleX;
    }
    return 0;
}

// Nothing to prepare before the map is built. Returns 0.
Sint32 scPrepareMap(void)
{
    return 0;
}

extern Sint32 IzumoRoomsKushinadaHeavingScript[];

// The two women dancing in Susa's house before the Serpent is slain step to the doorway and back
// with a hop, each waiting until the other is clear of the spot.
Sint32        IzumoRoomsFirstDancerScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SET(SW_GRAVITY, toFIXED(0.4)),
    SC_SPEED(0.6),
    SC_SET(SW_STEERING, 2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_LABEL(1),
    SC_CALL(IzumoRoomsIsFirstDancerClear),
    SC_MOVE(112, 0, 624),
    SC_SYNC,
    SC_MOVE(112, 0, 600),
    SC_SYNC,
    SC_MOVE(136, 0, 600),
    SC_SYNC,
    SC_MOVE(136, 0, 624),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_MOVE(112, 0, 624),
    SC_SYNC,
    SC_MOVE(136, 0, 600),
    SC_SYNC,
    SC_MOVE(112, 0, 600),
    SC_SYNC,
    SC_MOVE(136, 0, 624),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    -1,
};

Sint32 IzumoRoomsSecondDancerScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SET(SW_GRAVITY, toFIXED(0.4)),
    SC_SPEED(0.6),
    SC_SET(SW_STEERING, 2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_LABEL(1),
    SC_CALL(IzumoRoomsIsSecondDancerClear),
    SC_MOVE(176, 0, 624),
    SC_SYNC,
    SC_MOVE(176, 0, 600),
    SC_SYNC,
    SC_MOVE(152, 0, 600),
    SC_SYNC,
    SC_MOVE(152, 0, 624),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_MOVE(176, 0, 624),
    SC_SYNC,
    SC_MOVE(152, 0, 600),
    SC_SYNC,
    SC_MOVE(176, 0, 600),
    SC_SYNC,
    SC_MOVE(152, 0, 624),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    -1,
};

// clang-format off
// A girl in the Sanctum heaves with sobs ("*Sniff* How could we sacrifice our beloved Kushinada?"): her sprite is squashed and stretched three times, then rests.
Sint32 IzumoRoomsHeavingScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(0),
    SC_LABEL(1),
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
    SC_WAIT(40),
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
    SC_WAIT(5),
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
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    // Unused: a walk north and back again, which nothing starts.
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
    SC_ACCELERATION(0.1),
    SC_SPEED(0.3),
    SC_ANIMATION(2),
    SC_LABEL(1),
    SC_MOVE(264, 0, 120),
    SC_SYNC,
    SC_WAIT(60),
    SC_MOVE(264, 0, 96),
    SC_SYNC,
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    -1,
};

// Kushinada on her seat of stones heaves with sobs as the girl in the

// Sanctum does, with longer rests.
Sint32 IzumoRoomsKushinadaHeavingScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
    SC_LABEL(1),
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
    SC_WAIT(60),
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
    SC_WAIT(10),
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
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    // Unused: a hop, which nothing starts.
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SET(SW_GRAVITY, toFIXED(0.1)),
    SC_SPEED(0.4),
    SC_ANIMATION(0),
    SC_LABEL(1),
    SC_SET(SW_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_END,
};
// clang-format on

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
    SCENE_EXITS(IzumoRooms),
    SCENE_EXIT_UNLESS(IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A,
                      Izumo,
                      IZUMO_FROM_IZUMO_ROOMS_SOUTHEAST_A,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B,
                      Izumo,
                      IZUMO_FROM_IZUMO_ROOMS_SOUTHEAST_B,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        IZUMO_ROOMS_TO_IZUMO_SOUTHWEST, Izumo, IZUMO_FROM_IZUMO_ROOMS_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        IZUMO_ROOMS_TO_IZUMO_SOUTH, Izumo, IZUMO_FROM_IZUMO_ROOMS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        IZUMO_ROOMS_TO_IZUMO_NORTHWEST, Izumo, IZUMO_FROM_IZUMO_ROOMS_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        IZUMO_ROOMS_TO_IZUMO_NORTHEAST, Izumo, IZUMO_FROM_IZUMO_ROOMS_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        IZUMO_ROOMS_TO_IZUMO_NORTH, Izumo, IZUMO_FROM_IZUMO_ROOMS_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE IzumoRoomsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 120, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 88, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 96, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 80, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 84, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 136, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 120, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KUSHINADA, CONDITION_ALWAYS, (Sint32)IzumoRoomsKushinadaHeavingScript, 352, 0, 328, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_CREAM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 362, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 108, 0, 320, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 88, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, (Sint32)IzumoRoomsHeavingScript, 504, 0, 360, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 392, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
ACTOR_PLACE IzumoRoomsSusasHouseActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LADY_UZUME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 520, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (Sint32)IzumoRoomsFirstDancerScript, 136, 0, 624, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (Sint32)IzumoRoomsSecondDancerScript, 152, 0, 624, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 90, 0, 622, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 198, 0, 622, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
ACTOR_PLACE IzumoRoomsActorPlacementsOnceTheSerpentIsSlain[] = {
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_CREAM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 362, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 108, 0, 320, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 88, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, (Sint32)IzumoRoomsHeavingScript, 600, 0, 360, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 376, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_UZUME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 624, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 624, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 616, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 616, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KUSHINADA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 520, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 552, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_COAL), MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};
// clang-format on

CUE IzumoRoomsCues[] = {
    {CUE_EXIT,
     IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A, CONDITION_ALWAYS,
     IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A},
    {CUE_EXIT,
     IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B, CONDITION_ALWAYS,
     IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTHWEST, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_SOUTHWEST},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTH, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_SOUTH},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTHWEST, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_NORTHWEST},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTHEAST, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_NORTHEAST},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTH, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_NORTH},
    {CUE_STEP, 21, CONDITION_ALWAYS, SCRIPT_REF(scIzumoRoomsKushinadaAsksSusaNotToCome)},
    {CUE_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgOkuniKushinadaAnd)},
    {CUE_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgIfLadyUzume)},
    {CUE_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgIMSoRelieved)},
    {CUE_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgWhatIWant)},
    {CUE_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAWhileBack)},
    {CUE_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgThatDragonsbaneSeems)},
    {CUE_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgICanTFind)},
    {CUE_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIAmOld)},
    {CUE_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTheSerpentCan)},
    {CUE_TALK, 18, CONDITION_ALWAYS, SCRIPT_REF(scIzumoRoomsTalkToMerchant)},
    {CUE_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgOneDayKushinada)},
    {CUE_TALK, 20, CONDITION_ALWAYS, SCRIPT_REF(scIzumoRoomsTalkToInnkeeper)},
    {CUE_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgIfIWere)},
    {CUE_TALK, 22, CONDITION_ALWAYS, SCRIPT_REF(scIzumoRoomsTalkToPriest)},
    {CUE_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgSniffHowCould)},
    {CUE_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgWhenIGet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSusaSBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLadyUzumeS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgIOnlySuggested)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTheSerpentIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgIWonderIfAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTheSerpentWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgAccordingToLegend)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgOurGuardianSerpent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgThereSNo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgThePowerThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgItWasThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgThingsLikeThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgWhatDoesThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgIfOnlyI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgIDonTWantThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgNobodyWantsTo)},
    {CUE_TABLE_END},
};

CUE IzumoRoomsSusasHouseCues[] = {
    {CUE_EXIT,
     IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A, CONDITION_ALWAYS,
     IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A},
    {CUE_EXIT,
     IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B, CONDITION_ALWAYS,
     IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTHWEST, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_SOUTHWEST},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTH, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_SOUTH},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTHWEST, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_NORTHWEST},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTHEAST, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_NORTHEAST},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTH, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_NORTH},
    {CUE_TALK, ACTOR_SUSAS_HOUSE_UZUME, CONDITION_ALWAYS, SCRIPT_REF(scIzumoRoomsAskAfterSusa)},
    {CUE_TALK, ACTOR_SUSAS_HOUSE_FIRST_DANCER, CONDITION_ALWAYS, MSGNO(MsgThereSNoPoint)},
    {CUE_TALK, ACTOR_SUSAS_HOUSE_SECOND_DANCER, CONDITION_ALWAYS, MSGNO(MsgTheSerpentAttacked)},
    {CUE_TALK, ACTOR_SUSAS_HOUSE_WEST_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgTheSerpentAte)},
    {CUE_TALK, ACTOR_SUSAS_HOUSE_EAST_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgTheSerpentLives)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_SUSAS_HOUSE_UZUME, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRoomsReadPrayerForSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_SUSAS_HOUSE_FIRST_DANCER, CONDITION_ALWAYS,
     MSGNO(MsgOkuniSSuch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_SUSAS_HOUSE_SECOND_DANCER, CONDITION_ALWAYS,
     MSGNO(MsgTheSerpentWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_SUSAS_HOUSE_WEST_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgBeforeTheSerpent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_SUSAS_HOUSE_EAST_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgLegendHasIt)},
    {CUE_TABLE_END},
};

CUE IzumoRoomsCuesOnceTheSerpentIsSlain[] = {
    {CUE_EXIT,
     IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A, CONDITION_ALWAYS,
     IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A},
    {CUE_EXIT,
     IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B, CONDITION_ALWAYS,
     IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTHWEST, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_SOUTHWEST},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTH, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_SOUTH},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTHWEST, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_NORTHWEST},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTHEAST, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_NORTHEAST},
    {CUE_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTH, CONDITION_ALWAYS, IZUMO_ROOMS_TO_IZUMO_NORTH},
    {CUE_STEP,
     METATILE_EAST_SIDE_OF_THE_HALL, FLAG_IZUMO_RECOGNIZED_THE_HEROES,
     SCRIPT_REF(scIzumoRoomsHeroesComeFromTheEast)},
    {CUE_STEP,
     METATILE_WEST_SIDE_OF_THE_HALL, FLAG_IZUMO_RECOGNIZED_THE_HEROES,
     SCRIPT_REF(scIzumoRoomsHeroesComeFromTheWest)},
    {CUE_TALK, 8, CONDITION_ALWAYS, SCRIPT_REF(scIzumoRoomsTalkToMerchant)},
    {CUE_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMyGoodsAre)},
    {CUE_TALK, 10, CONDITION_ALWAYS, SCRIPT_REF(scIzumoRoomsTalkToInnkeeper)},
    {CUE_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAsLadyUzume)},
    {CUE_TALK, 12, CONDITION_ALWAYS, SCRIPT_REF(scIzumoRoomsTalkToPriest)},
    {CUE_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgNowThatTheSerpent)},
    {CUE_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgIfYouReA)},
    {CUE_TALK, ACTOR_UZUME, CONDITION_ALWAYS, SCRIPT_REF(scIzumoRoomsTalkToUzume)},
    {CUE_TALK, ACTOR_FIRST_DANCER, CONDITION_ALWAYS, SCRIPT_REF(scIzumoRoomsAskAboutMtMikage)},
    {CUE_TALK, ACTOR_SECOND_DANCER, CONDITION_ALWAYS, MSGNO(MsgSupposedlyTheDoll)},
    {CUE_TALK, ACTOR_WEST_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgLadyUzumeHas)},
    {CUE_TALK, ACTOR_EAST_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgLadyUzumeSNew)},
    {CUE_TALK, ACTOR_SAVED_KUSHINADA, CONDITION_ALWAYS, MSGNO(MsgSomedaySusaAnd)},
    {CUE_TALK, ACTOR_SUSA, CONDITION_ALWAYS, MSGNO(MsgIMSureYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgIDLoveTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgThisMayBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgIfBriggsS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgIUnderstandNow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgIThankThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgMaybeNowKushinada)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgThatDragonWanted)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_UZUME | CUE_SUBJECT_OWN_EFFECT,
     CONDITION_ALWAYS, SCRIPT_REF(scIzumoRoomsReadUzumesMind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_FIRST_DANCER, CONDITION_ALWAYS,
     MSGNO(MsgWhyDoesLadyUzume)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_SECOND_DANCER, CONDITION_ALWAYS,
     MSGNO(MsgIWonderWhatSort)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_WEST_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgIHeardThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_EAST_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgIDonTKnowWhatIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_SAVED_KUSHINADA, CONDITION_ALWAYS,
     MSGNO(MsgIfSDescendants)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSA, CONDITION_ALWAYS, MSGNO(MsgIMNo)},
    {USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL),
     ACTOR_UZUME, FLAG_UZUME_HAS_THE_DOLL,
     SCRIPT_REF(scIzumoRoomsGiveUzumeTheDoll)},
    {CUE_TABLE_END},
};
