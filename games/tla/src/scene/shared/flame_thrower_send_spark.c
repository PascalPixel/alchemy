/* A flame thrower's spark (FLAME_SPARK): a red dust cloud thrown from
   `pos`, a little east or west, up by chance and south at `speed` and
   more, spinning, blended over the room (scFlameThrowerUpdateSpark). Unlike the
   flames it never keeps its picture out of sight. */
void scFlameThrowerSendSpark(Sint32 *pos, Sint32 speed, Sint32 offScreen)
{
	ACTOR *actor = scFieldActorCreate(
		SPRITE_DUST_CLOUD, pos[0], pos[1], pos[2] - toFIXED(2));
	Sint32 spin;

	if(actor != NULL) {
		actor->bounce =
			syCos((Uint16)(syRandom() * 3 >> 4) + TRIG_TURN * 3 / 16);
		actor->gravity = (Uint16)syRandom();
		actor->sceneWord = (Uint16)(syRandom() << 1) + speed;
		spin = (Uint16)syRandom() + 0xffff8000;
		actor->timer = spin;
		scActorSetupScriptedFor(actor, 2);
		actor->update = (OBJECT_UPDATE)scFlameThrowerUpdateSpark;
		fdActorSetBlendMode(actor, 1);
	}
}
