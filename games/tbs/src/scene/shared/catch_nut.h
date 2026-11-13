/// @file
/// The Nuts floating in Vale, Kolima Forest, Mogall Forest, Kalay, Crossbone
/// Isle, Lunpa and McCoy's Palace.

/// Catch on a floating Nut: the hand takes it off the map, its flag is set so
/// it stays gone, and the leader holds it up and the party gets it.
static inline void Scene_CatchNut(s32 actorId, s32 flag) {
  Event_Start();
  Actor_SetPosition(actorId, 0, 0);
  GameFlag_Set(flag);
  // Called by the routines' own names, not through event_inline.h's
  // Item_ShowFound and Party_GiveItem, because those inlines compile
  // differently inside this one.
  ItemDisplay_ShowItem(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
  Event_End();
}
