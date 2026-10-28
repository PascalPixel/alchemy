#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "adventure.h"
#include "scene_ids.h"

struct SpriteAttributes {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
};

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02003720(void);
void Func_02003880(void);
void Func_020039d4(void);
void Func_02003a8c(void);

s32 Func_02003698(void) {
  s32 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov694Room1)
    Func_02003720();
  else if (scene == (s32)&SceneId_Ov694Room4)
    Func_02003880();
  else if (scene == (s32)&SceneId_Ov694Room2)
    Func_020039d4();
  else if (scene == (s32)&SceneId_Ov694Room3)
    Func_02003a8c();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_020036f0(void) {
  if (Object_GetById(65) != 0) {
    struct ObjectRuntime *owner = Object_GetById(gAdventure.currentOwner);

    ObjectMotion_SetActionVariant(
        65, ((struct SpriteAttributes *)owner->animation)->priority);
  }
}
