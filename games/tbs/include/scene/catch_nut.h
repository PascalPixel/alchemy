#ifndef ALCHEMY_CATCH_NUT_H
#define ALCHEMY_CATCH_NUT_H

// The Nuts floating in Vale, Kolima Forest, Mogall Forest, Kalay, Crossbone Isle, Lunpa and McCoy's
// Palace.

// Catch on a floating Nut: the hand takes it off the map, its flag is set so it stays gone, and the
// leader holds it up and the party gets it.
static inline void scCatchNut(Sint32 actorId, Sint32 flag)
{
    fdScriptStart();
    fdActorSetPos(actorId, 0, 0);
    gmFlagSet(flag);
    // Called by the functions' own names, not through script_inline.h's gmItemShowFound and
    // gmPartyGiveItem, because those inlines compile differently inside this one.
    fdItemDisplayShowItem(ITEM_NUT, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
    fdPartyInventoryGiveItem(ITEM_NUT, 0);
    fdScriptEnd();
}

#endif
