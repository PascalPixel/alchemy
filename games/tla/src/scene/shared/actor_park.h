/// Runs Actor_LaunchAhead, then parks an actor out of the way.
s32 Actor_Park(struct Actor *actor) {
  Actor_LaunchAhead(actor);
  SET_ACTOR_POSITION(actor, 0, 0, 0);
  SET_VELOCITY(actor, 0, 0, 0);
  return 0;
}
