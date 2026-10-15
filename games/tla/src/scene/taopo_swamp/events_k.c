#include "local.h"

void Func_020022ac(void) {
  struct SceneState *state = &gSceneState;
  struct ObjectRuntime *object;
  struct SpriteWork *sprite;

  if (state->mode != 2) {
    object = Object_GetById(Func_02002998());
    sprite = object->animation;
    Func_02002898();
    Func_02002968(0);
    Func_02002818_(object, (object->x & 0xfff00000) + 0x80000, object->y,
                   (object->z & 0xfff00000) + 0x80000);
    Func_02002820(object);
    while (sprite->level != 0) {
      sprite->level--;
      sprite->dirty = 1;
      WaitFrames(1);
    }
    Func_020028a0();
    sprite->mode |= 8;
    sprite->mode &= 0xfe;
    state->mode = 2;
    state->timer = 0;
  }
}

void Func_02002334(void) {
  struct SceneState *state = &gSceneState;
  struct SpriteWork *sprite;

  Func_0200237c();
  if (state->mode != 0) {
    sprite = Object_GetById(Func_02002998())->animation;
    if (sprite->child)
      sprite->child[6] = 9;
    sprite->mode = (sprite->mode & 0xf7) | 1;
    sprite->level = 0;
    sprite->dirty = 1;
    state->mode = 0;
    state->timer = 0;
  }
}

void Func_0200237c(void) {
  struct SceneState *state = &gSceneState;
  struct ObjectRuntime *object = Object_GetById(Func_02002998());

  state->x = (object->x & 0xfff00000) + 0x80000;
  state->z = (object->z & 0xfff00000) + 0x80000;
}
