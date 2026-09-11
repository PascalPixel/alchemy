#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/actor_sequence/run_83_when_flag_clear.h"

void State_Run83WhenFlag201Clear(void) { if (Story_Run(0x201) == 0) Story_Run2(83); }
