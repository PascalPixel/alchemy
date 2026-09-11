#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STORY_SCENARIO_DISPATCH_DISPATCH_BY_SCENARIO_ID
#define ALCHEMY_ABI_OVERLAYS_SCENE_STORY_SCENARIO_DISPATCH_DISPATCH_BY_SCENARIO_ID

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_02001236(void);
extern void Func_0200133a(void);

#define Story_Run Func_02001236
#define Story_Run2 Func_0200133a
#define gCell Data_02000240

#endif
