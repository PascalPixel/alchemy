/* A flame thrower's spark (FLAME_SPARK): a red dust cloud thrown from */
/* `position`, a little east or west, up by chance and south at `speed` and */
/* more, spinning, blended over the room (scFlameThrowerUpdateSpark). Unlike */
/* the flames it never keeps its picture out of sight. */
void scFlameThrowerSendSpark(s32 *position, s32 speed, s32 offScreen) {
	ACTOR *actor = scFieldActorCreate(SPRITE_DUST_CLOUD, position[0],
									  position[1], position[2] - toFIXED(2));
	s32 spin;

	if(actor != NULL) {
		actor->bounce = syCos((u16)(syRandom() * 3 >> 4) + TRIG_TURN * 3 / 16);
		actor->gravity = (u16)syRandom();
		actor->sceneWord = (u16)(syRandom() << 1) + speed;
		spin = (u16)syRandom() + 0xffff8000;
		actor->tmr = spin;
		fdActorSetupScriptedFor(actor, 2);
		actor->update = (OBJECT_UPDATE)scFlameThrowerUpdateSpark;
		fdActorSetBlendMode(actor, 1);
	}
}
