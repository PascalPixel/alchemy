// Readies a flame thrower's spark (FLAME_SPARK): it stops, shadowless and tinted
// SPRITE_COLOR_RED_TINT, and breaks up (the dust cloud's animation 2) without spinning or growing
// until ScriptedPartScript releases it, half a second on.
void scActorSetupScriptedFor(ACTOR *actor, Sint32 unused)
{
    actor->motionFlags    = 0;
    actor->collisionFlags = 0;
    fdActorSetSpriteFlags(actor, 0);
    fdActorSelectAnimation(actor, 2);
    fdActorStartScript(actor, ScriptedPartScript);
    scFieldActorSetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
    actor->timer        = 0;
    actor->speedLimit   = 0;
    actor->acceleration = 0;
}
