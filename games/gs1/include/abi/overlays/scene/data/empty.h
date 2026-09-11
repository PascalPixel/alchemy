#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_DATA_EMPTY
#define ALCHEMY_ABI_OVERLAYS_SCENE_DATA_EMPTY

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02000114(s32);
void Func_02000116(s32, s32, s32);
void Func_0200012c(s32);
void Func_0200012e(s32, s32, s32);
void Func_02000144(s32);
void Func_02000146(s32, s32, s32);
void Func_0200015c(s32);
void Func_0200015e(s32, s32, s32);

#define SceneData_Do Func_02000114
#define SceneData_Do2 Func_0200012c
#define SceneData_Do3 Func_02000144
#define SceneData_Do4 Func_0200015c
#define SceneData_GetTable81a8 Func_02000030
#define SceneData_GetTable81d8 Func_0200003c
#define SceneData_GetTable81ec Func_02000044
#define SceneData_GetTable8264 Func_020000ac
#define SceneData_Place Func_02000116
#define SceneData_Place2 Func_0200012e
#define SceneData_Place3 Func_02000146
#define SceneData_Place4 Func_0200015e
#define SceneData_ReturnZero Func_02000038
#define Scene_RunEmptyScene Func_020000b4
#define State_ApplyValues10And2And0Then58 Func_0200007c
#define State_ApplyValues11And3And0Then6c Func_02000094
#define State_ApplyValues8And0And0Then30 Func_0200004c
#define State_ApplyValues9And1And0Then44 Func_02000064

#endif
