/* Leaves a small spark where an orbiting one is, released after 6 ticks
   (OrbitSparkScript): the orbit's trail. */
void scOrbitSparkSpawn(ORBITER *src)
{
	ACTOR *spark = scFieldActorCreate(SPRITE_PARTICLE, src->x, src->y, src->z);

	spark->scaleY = toFIXED(2) / 5;
	spark->scaleX = toFIXED(2) / 5;
	spark->motionFlg = 0;
	scFieldActorSetSprCol(spark, SPRITE_COLOR_RBG);
	fdSetActorSprPriority(spark, 1);
	fdActorSelectAnimation(spark, 1);
	fdActorSetSprFlags(spark, 0);
	fdActorSetBlendMode(spark, 1);
	fdActorStartScript(spark, OrbitSparkScript);
}
