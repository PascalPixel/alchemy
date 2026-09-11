#include "types.h"
#include "scene.h"

void State_Run83WhenFlag201Clear(void) { if (Story_Run(0x201) == 0) Story_unk2_4(83); }
