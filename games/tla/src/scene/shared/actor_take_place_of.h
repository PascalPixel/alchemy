/// Puts `actor` where `target` stands, at rest, and `target` away at the
/// origin.
void Actor_TakePlaceOf(struct Actor *actor, struct Actor *target) {
  FieldActor_SetPositionAndResetMotion(actor, target->x.fixed, target->y.fixed,
                                       target->z.fixed);
  FieldActor_SetPositionAndResetMotion(target, 0, 0, 0);
}
