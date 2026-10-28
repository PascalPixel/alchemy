#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "field_event.h"

struct BattleFxScene {
  u8 unk00[16];
  void *volatile mainObject;
};

void FieldEffect_RunMove(void) {
  /* FAKEMATCH: an unused 12-byte local explains the native 12-byte stack frame. */
  s32 work[3];
  struct BattleFxScene *scene;
  void *object;

  scene = Ram_HeapSlots->effectWork;
  object = scene->mainObject;

  BattleEffect_InitializeSharedScene();
  object = BattleEffect_StartItemBreak(object);
  BattleEffect_SnapScaleToFull(object);
  if (object != 0) {
    Actor_SetMode(object, 4);
    WaitFrames(30);
  }
  BattleEffect_PrepareBufferInterpolation();
  UpdateRisingParticleBurst(object);
}
