#ifndef ALCHEMY_FLAGS_H
#define ALCHEMY_FLAGS_H

/// @file
/// Game flags, named where their meaning is shown, as The Broken Seal's
/// flags.h names its own.

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

#endif
