#ifndef ALCHEMY_ABI_BATTLE_PLACEMENT_PLACE_ACTORS_BY_FORMATION_KIND
#define ALCHEMY_ABI_BATTLE_PLACEMENT_PLACE_ACTORS_BY_FORMATION_KIND

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0800b168(void *object, s32 *position, s32 *scale, u32 mode);

#define Battle_PlaceActorsByFormationKind Func_08012e28
#define Battle_SetMode Func_0800b168
#define gIw Data_03001e60
#define gRom Data_08013584

#endif
