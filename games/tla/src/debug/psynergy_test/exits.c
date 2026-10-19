#include "types.h"
#include "scene_ids.h"

void Func_02000eb4(s32 scene, s32 entrance);
void Func_02000ecc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                     s32 g) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000ecc(a, b, c, d, e, f, g);
}

void Func_02000624(void) {
  Map_CopyCells(34, 26, 5, 4, 2, 10, 0);
  Func_02000eb4((s32)&SceneId_Ov6b8Room1, 6);
}

void Func_02000650(void) { Func_02000eb4((s32)&SceneId_Ov6b8Room1, 8); }
