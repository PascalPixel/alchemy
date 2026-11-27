/* Leaves a small spark where an orbiting one is, released after 6 frames */
/* (OrbitSpark_Script): the orbit's trail. */
void syOrbitSparkSpawn(ORBITER *source) {
	ACTOR *spark =
		scFieldActorCreate(SPRITE_PARTICLE, source->x, source->y, source->z);

	spark->scaleY = FIXED_ONE * 2 / 5;
	spark->scaleX = FIXED_ONE * 2 / 5;
	spark->motionFlg = 0;
	scFieldActorSetSprCol(spark, SPRITE_COLOR_RBG);
	scSetActorSprPriority(spark, 1);
	fdActorSelectAnimation(spark, 1);
	fdActorSetSprFlags(spark, 0);
	fdActorSetBlendMode(spark, 1);
	fdActorStartScript(spark, OrbitSpark_Script);
}
