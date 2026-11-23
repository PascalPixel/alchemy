/// The Lemurian ship's script call as it is placed on the sea: drops its
/// sprite's shadow and puts it on the middle layer.
s32 Ship_DropShadow(struct Actor *actor) {
  ((struct FieldSprite *)actor->sprite)->flags = 0;
  actor->layer = MAPGRID_LAYER_MIDDLE;
  return 1;
}
