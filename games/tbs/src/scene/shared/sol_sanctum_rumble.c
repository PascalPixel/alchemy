/* The initialization that two rooms of Sol Sanctum share,
   SceneId_SolSanctumMaze and SceneId_SolSanctumCorridor. */

#include "flags.h"

/* The scene opens in the window transition over 16 frames; after the eruption
   the ground rumbles and shakes, and the weather's sparkles start
   (fdEffectInitSparkles). */
Sint32 scInit(void)
{
	FIELD_STATE *state;

	FIXED shakeX = toFIXED(1);
	FIXED shakeY = toFIXED(1);
	FIXED shakeDecay = toFIXED(1);

	state = FieldStateWork;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	state->transitionFrm = MAP_TRANSITION_FRAMES;
	if(gmFlagGet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
		fdQueueMusic(SE_RUMBLE);
		fdMapGridSetScreenShake(shakeX, shakeY, shakeDecay);
		fdEffectInitSparkles();
	}
	return 0;
}
