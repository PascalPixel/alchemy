/// The script call at the end of an attached actor's run (Actor_Attach): it
/// launches the actor it carries (Actor_LaunchAhead), then is put away at the
/// origin, at rest.
s32 Actor_Park(struct Actor *actor) {
  Actor_LaunchAhead(actor);
  SET_ACTOR_POSITION(actor, 0, 0, 0);
  SET_VELOCITY(actor, 0, 0, 0);
  return 0;
}
