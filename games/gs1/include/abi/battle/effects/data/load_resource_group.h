#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_DATA_LOAD_RESOURCE_GROUP
#define ALCHEMY_ABI_BATTLE_EFFECTS_DATA_LOAD_RESOURCE_GROUP

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08002fb0(s32 first, s32 second);

#define SceneData_Apply Func_08002fb0
#define gCell Data_02000240
#define gOv Data_02008000
#define gRom Data_0809f1a8

#endif
