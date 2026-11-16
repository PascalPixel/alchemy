/// Holds the actor still while the field is handling requests.
void Actor_HoldDuringEvent(struct Actor *object) {
  object->paused = Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS);
}
