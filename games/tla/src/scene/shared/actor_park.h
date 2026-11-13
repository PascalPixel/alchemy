/// Runs Actor_LaunchAhead, then parks an actor out of the way.
s32 Actor_Park(struct Actor *object) {
  Actor_LaunchAhead(object);
  SET_POSITION(object, 0, 0, 0);
  SET_VELOCITY(object, 0, 0, 0);
  return 0;
}
