#include "spawnfn.h"

// 常駐版: エンジンを遠隔呼び出しで使う
EFFECT_SPAWN(Effect_SpawnResident, Object_CreateFar, ObjectDispatch_ApplyArgumentToChildrenFar, ObjectDispatch_InitializeFar)
