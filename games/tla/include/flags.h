#ifndef ALCHEMY_FLAGS_H
#define ALCHEMY_FLAGS_H

/// @file
/// Game flags, named where their meaning is shown, as The Broken Seal's
/// flags.h names its own.

/// The next field scene starts where the party was, as after a battle: the
/// scene's flags are not reset, the actors come back from their snapshot,
/// and the music saved in gMainRoutineWork.returnMusic resumes if there is
/// one. The scenes skip their arrival events while it is set.
#define FLAG_KEEP_PARTY_POSITION 0x109

/// Set once the old werewolf of Garoh's inn has asked the party whether they
/// learned Reveal; he then only sighs about his wasted years.
/// ★ Named from the game: "You... Is it true that you learned Reveal?"
/// (MsgGarohInteriors47)
#define FLAG_GAROH_ASKED_ABOUT_REVEAL 0x91a
/// Set as the party wakes at Garoh's inn after the night with Maha
/// (GarohInteriors_WakeAtInn): from then on it is day in Garoh and its people
/// no longer hide that they are werewolves.
/// ★ Named from the game: "We don't have to hide our secret from you any
/// longer!" (MsgGaroh15)
#define FLAG_GAROH_SECRET_SHARED 0x91b

/// Set while it is night in Garoh: its scenes fade the palette to night and
/// play a howl every 1,024 frames, and its inn shows its night layers. The
/// party walks into the night as it climbs to the village, and the morning
/// after the night with Maha (FLAG_GAROH_SECRET_SHARED) clears it.
/// ★ Named from the game: "on nights of the full moon, we only sell claws
/// and fangs" (MsgGarohInteriors17)
#define FLAG_GAROH_NIGHT 0x310

/// Set as Briggs sails from Alhafra in the boat he bought with the Ankohl
/// jewels, leaving the party on the shore; from then on Champa is fed and
/// talks of his return.
/// ★ Named from the game: "I paid for this boat, fair and square. I'm no
/// thief." (MsgBriggs2)
#define FLAG_BRIGGS_SAILED_FROM_ALHAFRA 0x97f

/// The first of four flags, one for each patch of overgrowth in the Apojii
/// Islands' cave that Whirlwind has blown away, by its placed actor id.
#define FLAG_APOJII_CAVE_FOLIAGE_FIRST 0x8e9
#define FLAG_APOJII_CAVE_FOLIAGE_BLOWN(actor)                                  \
  (FLAG_APOJII_CAVE_FOLIAGE_FIRST - ACTOR_FIRST_PLACED + (actor))

/// Set as Briggs is caught on the boat he bought at Alhafra; the town then
/// thanks the party and talks of pirates.
/// ★ Named from the game: "Capturing the dreaded Briggs was a great deed."
/// (MsgAlhafraInteriors86)
#define FLAG_BRIGGS_CAUGHT 0x8ab
/// Set once Burst has broken the last block on the Alhafran boat's mast, so
/// the mast is ready for repairs; the town then wonders whether Briggs was a
/// pirate at all.
/// ★ Named from the game: "Briggs paid for that boat fair and square."
/// (MsgAlhafraInteriors88)
#define FLAG_ALHAFRA_MAST_CLEARED 0x8a7
/// Set as Alex greets the party at Alhafra's inn.
/// ★ Named from the game: "So, you've finally come." (MsgAlhafraInteriors31)
#define FLAG_ALHAFRA_MET_ALEX 0x8b2
/// Set once Alex has left Alhafra's inn.
/// ★ Named from the game: "That cute boy next door left already."
/// (MsgAlhafraInteriors84)
#define FLAG_ALEX_LEFT_ALHAFRA 0x971
/// Set as the party from Madra, its mayor among them, reaches Alhafra across
/// the Yampi Desert.
/// ★ Named from the game: "Finally! We made it!" (MsgAlhafra10)
#define FLAG_ALHAFRA_MADRANS_ARRIVED 0x8b3
/// Set as Alhafra's sailboat leaves its harbour, with
/// FLAG_BRIGGS_SAILED_FROM_ALHAFRA; the town then mourns its only ship.
/// ★ Named from the game: "Alhafra lost its ship." (MsgAlhafra132)
#define FLAG_ALHAFRA_SHIP_GONE 0x90f
/// Set as Alhafra's inn tells that someone named Isaac picked Crossbone Isle
/// clean, which it tells only while FLAG_ISAAC_CLEARED_CROSSBONE_ISLE is set.
/// ★ Named from the game: "Legends said that Crossbone Isle could be found in
/// the middle of the Karagol Sea." (MsgAlhafraInteriors120)
#define FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE 0x903
/// XXX: what sets it; the code that reads a Broken Seal save or password would
/// settle it. Alhafra's inn tells of Isaac picking Crossbone Isle clean only
/// while it is set.
/// ★ Named from the game: "Someone named Isaac picked it clean."
/// (MsgAlhafraInteriors120)
#define FLAG_ISAAC_CLEARED_CROSSBONE_ISLE 0x823

/// Set as the Great Serpent of Mikage is slain in Gaia Rock; Izumo then
/// celebrates and takes its tables for after.
/// ★ Named from the game: "After the serpent's roar, these warriors appeared."
/// (MsgWarriorsAppearedFrom)
#define FLAG_SERPENT_SLAIN 0x927
/// Set once Izumo has recognized the party as the warriors who slew the
/// serpent (IzumoInteriors_RecognizeTheHeroes).
/// ★ Named from the game: "So, it wasn't Susa who defeated the serpent, but
/// those heroes..." (MsgWarriorsAppearedFrom)
#define FLAG_IZUMO_RECOGNIZED_THE_HEROES 0x928
/// Set once the party has given Lady Uzume the dancing doll of Mt. Mikage
/// (IzumoInteriors_GiveUzumeTheDoll).
/// ★ Named from the game: "With this dancing doll, I can explore Mt. Mikage
/// on my own." (MsgWithThisDancing)
#define FLAG_UZUME_HAS_THE_DOLL 0x92c

/// Set as the party throws the boy Tavi the rope at the Shrine of the Sea God;
/// Daila's houses then take their later tables.
/// ★ Named from the game: "These guys threw me the rope..."
/// (MsgFieldPraiseRopeWork)
#define FLAG_TAVI_RESCUED 0x873

/// Set once the pillar inside Jupiter Lighthouse's main gates stands on the
/// button beside them: the gates stay open.
#define FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED 0xa01

/// Set once the pillar on the floor below Jupiter Lighthouse's aerie stands on
/// the button by its door: the door stays open, a shortcut.
#define FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED 0xa19

/// Set as Hover cast on the raised platform in Jupiter Lighthouse wakes the
/// lighthouse: its hover platforms and blocks rise in every room.
#define FLAG_JUPITER_LIGHTHOUSE_ACTIVATED 0xa00

/// Set as the left tower's archer statue shoots its arrow at the left angel on
/// Jupiter Lighthouse's aerie, which starts to lift its half of the lid.
#define FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED 0xa0a
/// The same for the right tower's archer and the right angel.
#define FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED 0xa0b

/// Set as the left tower's archer statue shoots its arrow at the left angel
/// on Jupiter Lighthouse's aerie, which starts to lift its half of the lid.
/// ★ Named from what the player sees: the arrow and the rising angel
/// (the aerie's JupiterLighthouse events)
#define FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED 0xa0a
/// The same for the right tower's archer and the right angel.
#define FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED 0xa0b

/// Set once Felix has woken on Idejima's shore.
/// ★ Named from the game: "Do you want to check yourself for injuries?"
/// (MsgIdejimaCheckForInjuries)
#define FLAG_IDEJIMA_FELIX_WOKE 0x844
/// Set as Kraden, Jenna and Sheba, each in turn, join Felix on Idejima.
/// ★ Named from the game: "Kraden has joined your party." (MsgIdejimaGroan)
#define FLAG_IDEJIMA_KRADEN_JOINED 0x863
/// ★ Named from the game: "{actor_name:6} has joined your party."
/// (MsgIdejimaAwakeningExclamation)
#define FLAG_IDEJIMA_JENNA_JOINED 0x864
/// ★ Named from the game: "{actor_name:7} has joined your party."
/// (MsgIdejimaSecondGroan)
#define FLAG_IDEJIMA_SHEBA_JOINED 0x865
/// Set once the party has wondered where Alex has gone.
/// ★ Named from the game: "By the way... Where has Alex gone?"
/// (MsgIdejimaWhereHasAlexGone)
#define FLAG_IDEJIMA_ASKED_WHERE_ALEX_WENT 0x867

/// Set once Daila's greeter has asked whether the party has been there before.
/// ★ Named from the game: "You don't look familiar. Have you been to Daila
/// before?" (MsgDailaBeenBefore)
#define FLAG_DAILA_GREETER_ASKED 0x843
/// Set as Alex leaves Daila for Madra.
/// ★ Named from the game: "Perhaps you will see me in Madra..."
/// (MsgDailaFelixCall)
#define FLAG_DAILA_MET_ALEX 0x84b
/// Set once Scoop has dug up the Psy Crystal under Daila's puddle.
/// ★ Named from the game: "Psy Crystal" (MsgItemName)
#define FLAG_DAILA_PSY_CRYSTAL_DUG 0x300
/// Set once the healer of Daila's Sanctum has sent the party to Master Poi;
/// after that the Sanctum serves them across its counter.
/// ★ Named from the game: "Then I suggest you visit the Kandorean Temple to
/// see Master Poi." (MsgDailaInteriorsTravelersFreeTime)
#define FLAG_DAILA_HEALER_SENT_TO_POI 0x847
/// Set once the party has taken the Sea God's Tear from Daila's sanctuary.
/// ★ Named from the game: "Sea God's Tear" (MsgItemName)
#define FLAG_SEA_GODS_TEAR_TAKEN 0xa20

#endif
