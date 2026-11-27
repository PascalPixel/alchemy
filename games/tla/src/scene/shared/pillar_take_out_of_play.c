// Keeps a pillar's sprite in VRAM, its picture composed, and takes the pillar out of play.
void scPillarTakeOutOfPlay(Sint32 id)
{
    fdActorGet(id)->retainSpriteCache = ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
    fdActorDisableById(id);
}
