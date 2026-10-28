#include "ankohl_ruins.h"
#include "edition.h"

struct AnkohlRuinsEffectParams {
  u8 unk00[0x18];
  u16 kind;
  u8 unk1a[0xe];
};

extern const u8 gData_02003790[];
extern const u8 gData_020037b4[];
extern const u8 gData_02003a78[];
extern const u8 gData_02003b98[];
extern const u8 gData_02003c94[];
extern const u8 gData_02003e38[];
extern const u8 gData_02003f4c[];
extern const u8 gData_02004018[];
extern u32 gFrameCount;

void Func_0200015c(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct AnkohlRuinsEffectParams *params);
void Func_02000b30(s32 id, s32 cue);
void Func_02002df4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002e6c(s32 id, s32 a, s32 b);
void Func_02002ebc(s32 a);
void Func_02002ee4(void);
void Func_02002eec(s32 a);

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002df4(a, b, c, d, e, f);
}

void Func_02002260(void) {
  struct Actor *object = Actor_GetById(9);

  if (object->x >> 20 == 26 && object->z >> 20 == 35) {
    GameFlag_SetBit(0x9db);
    Func_02002eec(204);
    Func_02000b30(9, 0);
    Func_02002e6c(9, 0, 0);
    Func_02002ee4();
    Func_02002ebc(20);
  }
#if (LANGUAGE == LANGUAGE_JAPANESE) /* The Japanese edition copies cells here. */
  Map_CopyCells(40, 98, 3, 5, 25, 98);
#endif
}

void Func_020022a8(void) {
#if !defined(                                                                  \
    TLA_EDITION_JA) /* The Japanese edition does not touch flag 0x205. */
  GameFlag_SetBit(0x205);
#endif
  Map_CopyCells(35, 34, 3, 5, 25, 34);
  Map_CopyCells(35, 98, 3, 3, 25, 98);
}

void Func_020022e0(void) {
#if (LANGUAGE != LANGUAGE_JAPANESE)
  GameFlag_ClearBit(0x205);
#endif
  Map_CopyCells(40, 34, 3, 5, 25, 34);
  Map_CopyCells(40, 98, 3, 3, 25, 98);
}

void Func_02002318(void) { GameFlag_SetBit(0x205); }

void Func_02002328(void) { GameFlag_ClearBit(0x205); }

const u8 *Func_02002338(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov686Room1)
    return gData_020037b4;
  if (gAdventure.scene == (s32)&SceneId_Ov686Room2)
    return gData_02003a78;
  if (gAdventure.scene == (s32)&SceneId_Ov686Room3)
    return gData_02003b98;
  if (gAdventure.scene == (s32)&SceneId_Ov686Room4)
    return gData_02003c94;
  if (gAdventure.scene == (s32)&SceneId_Ov686Room5)
    return gData_02003e38;
  if (gAdventure.scene == (s32)&SceneId_Ov686Room6)
    return gData_02003f4c;
  if (gAdventure.scene == (s32)&SceneId_Ov686Room8)
    return gData_02004018;
  return gData_02003790;
}

void Func_020023d0(struct Actor *object) {
  struct AnkohlRuinsEffectParams params;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    {
      /* FAKEMATCH: the native builds the mask before the random draw. */
      s32 mask = 15;

      dx = mask & Random16Far();
    }
    dy = Random16Far() & 7;
    params.kind = 673;
    Func_0200015c(object->x + ((dx - 8) << 16), object->y + ((dy - 8) << 16),
                  object->z, 0, 0x8000, phase, 0x100000, &params);
  }
}
