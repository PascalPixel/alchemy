/// @file
/// Vault, on both visits: Isaac walking in through a house's open door.

/// Walks Isaac slowly into the doorway at (x, z) as the screen changes to the
/// room behind it.
static void Vault_WalkThroughExit(s32 x, s32 z, s32 exitNumber) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, x, z);
  gFieldWork->transitionFrames = MAP_TRANSITION_FRAMES;
  Import_EventSetExitRequest(exitNumber);
}
