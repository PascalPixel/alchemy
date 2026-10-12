#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_0200045c(void);
void Func_0200065c(void);

extern u8 Data_02005e98[];
extern u8 Data_02005ed4[];
extern u8 Data_02005f10[];

struct SceneEvent gSceneEvents[] = {
	{1, 1, CONDITION_ALWAYS, 11},
	{1, 2, CONDITION_ALWAYS, 12},
	{2, 100, 0x200, EVENT_SCRIPT(Func_0200045c)},
	{2, 101, 0xa21, EVENT_SCRIPT(Func_0200065c)},
	{0, 28, CONDITION_ALWAYS, MSGNO(MsgOnceYouVe)},
	{0, 27, CONDITION_ALWAYS, MSGNO(MsgOnceJupiterIs)},
	{0x8d15, 28, CONDITION_ALWAYS, MSGNO(MsgOnceJupiterIsLit)},
	{0x8d15, 27, CONDITION_ALWAYS, MSGNO(MsgDoEvenThink)},
	{SCENE_EVENTS_END},
};

u32 Data_020069dc[] = {
	17, 18, 19, 20, 21, 22,
};

s32 Data_020069f4[] = {
	-1,
};

u32 Data_020069f8[] = {
	1,
};

u32 Data_020069fc[] = {
	39, 5, 0, 12, 38,
};

s32 Data_0200ea10[] = {
	(s32)Data_02005e98,
	(s32)Data_02005ed4,
	(s32)Data_02005f10,
};
