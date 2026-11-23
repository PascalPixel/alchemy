/// Leaves a small spark where an orbiting one is, released after 6 frames
/// (gOrbitSpark_Script): the orbit's trail.
void OrbitSpark_Spawn(struct Orbiter *source) {
  struct Actor *spark =
      FieldActor_Create(SPRITE_PARTICLE, source->x, source->y, source->z);

  spark->scaleY = FIXED_ONE * 2 / 5;
  spark->scaleX = FIXED_ONE * 2 / 5;
  spark->motionFlags = 0;
  FieldActor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
  Scene_SetActorSpritePriority(spark, 1);
  Actor_SelectAnimation(spark, 1);
  Actor_SetSpriteFlags(spark, 0);
  Actor_SetBlendMode(spark, 1);
  Actor_StartScript(spark, gOrbitSpark_Script);
}
