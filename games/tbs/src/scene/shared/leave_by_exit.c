/// @file
/// Timber Rooms A (Suhalla) and Timber Rooms B and C (Lalivero).

/// The party leaves by the touched exit, with the exit sound: the exit
/// numbered as the step trigger the party walked into.
static void Scene_LeaveByExit(void) {
  struct FieldWork *work = gFieldWork;
  s16 *trigger = (s16 *)&work->stepTrigger;

  Import_EventSetExitRequest(*trigger);
  Sound_Play(SE_MAP_EXIT);
}
