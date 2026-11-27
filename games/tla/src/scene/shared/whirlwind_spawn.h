// A whirlwind blows out of the stone face at `src`. Mars Lighthouse keeps a copy nothing calls, its
// whirlwinds starting from a gust.
ACTOR *scWhirlwindSpawn(ACTOR *src)
{
	VECTOR3        pos;
	EFFECT_OPTIONS options;
	ACTOR         *actor;

	fdScriptPlaySound(SE_WHIRLWIND);
	pos.x = src->x.fixed + syMathMulFX(toFIXED(8), syCos(src->facing));
	pos.z = src->z.fixed + syMathMulFX(toFIXED(8), syMathSin(src->facing));
	actor = scFieldActorCreate(SPRITE_WHIRLWIND, pos.x, src->y.fixed, pos.z);
	fdActorSelectAnimation(actor, 2);
	fdActorSetSpriteFlags(actor, 0);
	actor->motionFlags    = 0;
	actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	actor->facing         = src->facing;
	actor->timer          = 0;
	actor->sceneCount     = 0;
	actor->sceneByte      = 0;
	actor->update         = scWhirlwindUpdate;
	actor->speedLimit     = toFIXED(3) / 2;
	options.priority      = 1;
	options.palette       = SPRITE_COLOR_WHITE;
	scFieldEffectSpawn(pos.x,
	                   src->y.fixed,
	                   pos.z - toFIXED(6),
	                   0,
	                   0,
	                   0,
	                   EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY,
	                   &options);
	return actor;
}
