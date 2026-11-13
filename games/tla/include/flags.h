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

#endif
