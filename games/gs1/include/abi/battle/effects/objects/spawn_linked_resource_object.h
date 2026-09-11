#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_OBJECTS_SPAWN_LINKED_RESOURCE_OBJECT
#define ALCHEMY_ABI_BATTLE_EFFECTS_OBJECTS_SPAWN_LINKED_RESOURCE_OBJECT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08009098(
    struct BattleEffectLinkedObject *object,
    const void *configuration);
void Func_08009080(struct BattleEffectLinkedObject *object, s32 mode);
void Func_080f9010(s32 cue);
void Func_0809163c(s32 state);
void Func_0809376c(void);

#define Battle_Apply Func_08009098
#define Battle_Apply2 Func_08009080
#define Battle_Do Func_080f9010
#define Battle_Do2 Func_0809163c
#define Battle_Run Func_080090c8
#define Battle_Run2 Func_0809376c
#define gRom Data_0809fc2c

#endif
