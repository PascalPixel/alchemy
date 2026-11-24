/// @file
/// The plain push of Altmiller Cave's fallen columns and Crossbone Isle's two
/// logs on the grass.

/// Walking into a pushed thing moves it a cell on, if the way beyond is clear.
static void PushedActor_PushIfClear(void) {
  struct PushedActorProbe probe;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    PushedActor_Roll(probe);
  }
  Event_End();
}
