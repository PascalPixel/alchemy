#ifndef TLA_SHAMAN_VILLAGE_H
#define TLA_SHAMAN_VILLAGE_H

#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "frame.h"

struct ShamanVillageObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[3];
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  u8 unk2c[0x18];
  s32 field44;
  s32 field48;
  u8 unk4c[4];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 actionFlags;
  u8 unk5b;
  u8 field5c;
  u8 unk5d[5];
  u8 timer;
  u8 unk63;
  s16 field64;
  s16 field66;
  s32 field68;
  void *update;
};

struct ShamanVillageEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[2];
  s16 field15e;
  u8 unk160[6];
  s16 field166;
  u8 unk168[0x44];
  s32 screenEffect;
  u8 unk1b0[4];
  s32 field1b4;
  u8 unk1b8[0x0c];
  u16 field1c4;
};

u32 Random16Far(void);

void Func_020048c4(void);
void Func_020048cc(void);
void Func_02004a74(s32 shop, s32 keeper);
void Func_02004a7c(s32 keeper);
void Func_02004a84(s32 shop, s32 keeper);
void Import_ActorSetMode(struct ShamanVillageObject *object, s32 mode);
void Func_0200481c(struct ShamanVillageObject *object);
void Actor_SetSingleChildFlags(struct ShamanVillageObject *object,
                                        u32 value);
void Animation_SetStateFlags(struct ShamanVillageObject *object, s32 flags);
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020049f4(void);

void Func_02004a44(void);
void Func_02004a4c(s32 a, s32 b, s32 c, s32 d);
void Event_StartScreenTransition(void);

void Func_02004944(s32 actor, s32 target);

void Func_02004a54(void);
void Func_02004a5c(void);
void Func_02004a64(void);
void Func_02004a6c(struct ShamanVillageObject *object, s32 value);
void Func_02004a2c(void);
void Func_020048b4(void);

#define SHAMAN_VILLAGE_EMOTE(actor, emote, frames)                             \
  {                                                                            \
    /* FAKEMATCH(pin): the emote pinned to r1, as the native */                \
    /* loads the emote and the frames before the actor at every */             \
    /* emote of this overlay. Measured against it: an inline */                \
    /* call (the actor comes one row early wherever the emote */               \
    /* takes two instructions), an inline with its parameters in */            \
    /* another order or modified, a local (shared between calls), */           \
    /* a once-only block, and barriers that hold r0 (each empty */             \
    /* asm adds four bytes to the pool estimate and moves the */               \
    /* literal pool of a long scene). As an unpinned local, 111 */             \
    /* of 711 instructions differ in tla-en (Func_02002068). */                \
    register s32 emote_ __asm__("r1") = (emote);                               \
    s32 frames_ = (frames);                                                    \
    Func_080d47b4((actor), emote_, frames_);                                   \
  }

// ---- this overlay

void Func_0200057c(void);
void Func_020011f8(void);
void Func_02002f20(s32 a, s32 b);
void Func_02002fb0(s32 a);
void Func_02003b6c(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f);
s32 Func_02000f94(void);
void Func_02001380(void);
void Func_0200289c(void);
void Func_0200290c(void);
void Func_02003040(void);

extern const u8 gData_02004c08[];
extern const u8 gData_020050f8[];
extern u8 gData_02005284[];
extern u8 gData_020052b4[];
extern u8 gData_02005314[];

#endif
