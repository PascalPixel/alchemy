// Draft, differ 10 of 194, skeleton gap 5 (tla-en, 2026-11-01)
// Twin of Sanctum_RunMemberIconBurst in The Broken Seal
// (games/tbs/src/shop/shop/sanctum.c): the sanctum's burst on a party member.
// Same statements in the same order: cursor hidden, the service's cue, palette
// tint, icon stilled, 18 sparkles three frames apart, the jingle waited for, 24
// slots told to finish, SE 126, slots freed, palette and cursor restored. This
// game's work block has icons at 0xf8, positions at 0x118 and 0x128, the burst
// member at 0x508, sparks from 0x50c; particle mode 0x2a0, task flags 0x480.
// Left: two scheduling runs, nothing else; the RTL before sched2 is already in
// the listing's order. After each ldrsh of a position, sched2 pulls a split
// constant forward: movs/lsls #148 past the sp copy and x store, and the
// effects offset (#160) past the shift and add of z. Both happen because the
// constant's chain outranks the store and is ready while the load's result is
// queued for two cycles, so this is the model of load timing (K2), not source.
// Tried, no gain: z stored first, effect before effectIndex, position as an
// array or a union, pointers to the coordinate arrays, values in locals, z
// subtracted before the shift or in a second statement, padded Position, the
// twin's constant spellings (equal, kept).
#include "types.h"
#include "fixed_math.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ShopCursor {
  struct WindowSprite *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

struct Position {
  s32 x, y, z;
};
struct Effect_080b2f4c {
  u8 filler[0x48];
};

u32 Random16(void);
void wait(s32 frames);
void Sound_PlayFar(s32 sound_id);

void Shop_RestoreSceneTiles(s32 address);
s32 Task_Add(void (*func)(void), s32 flags);
void Func_0808a528(struct Effect_080b2f4c *effect, s32 mode, s32 x, s32 z);
void Func_0808a520(struct Effect_080b2f4c *effect,
                   void (*callback)(struct Effect_080b2f4c *));
void Func_0808a518(struct Effect_080b2f4c *effect, s32 value);
void Func_08009248(s32 object, u32 frame_offset);
void Sound_WaitForJingle(void);
void Func_0808a530(struct Effect_080b2f4c *effect);
void Task_Remove(void (*func)(void));
void Func_08009280(s32 object, s32 arg);
void Shop_InitializeEffect(void);
void Shop_ResetEffects(void);
void BattleEffect_UpdateRadialMotion(struct Effect_080b2f4c *effect);

extern s8 Data_0810cf28[];

struct BurstSprite {
  s32 object;
  s32 x, z, targetX, targetZ, originX, originZ, speed, speedMax, acceleration;
  s32 scaleX;
  s32 scaleY;
  u16 heading;
  s16 turnStepMax;
  void *callback;
  u16 age;
  s16 callbackDelay;
  s16 orbitRadius;
  s16 orbitAngle;
  s8 state;
  s8 stopAtTarget;
  s8 turnStepLimited;
  s8 updateMotion;
  s8 render;
  s8 active;
};

struct BurstShop {
  u8 unk000[0xf8];
  s32 partyMemberIcons[8];
  s16 partyMemberX[8];
  s16 partyMemberY[8];
  u8 unk138[0x4dc - 0x138];
  struct ShopCursor cursor;
  u8 unk4ec[0x507 - 0x4ec];
  s8 action;
  s8 burstMember;
  u8 unk509[3];
  struct BurstSprite effects[24];
};

void Func_0810b1b4(s32 member) {
  struct BurstShop *shop;
  s8 savedCursorMode;
  struct Position position;
  struct BurstSprite *effect;
  s32 callbackPriority;
  s32 effectIndex;

  shop = Ram_HeapSlots->menuRuntime;
  savedCursorMode = shop->cursor.anchor->active;
  shop->burstMember = -1;
  shop->cursor.anchor->active = 13;
  Sound_PlayFar(Data_0810cf28[shop->action]);
  Shop_RestoreSceneTiles(0x00202108);
  Func_08009280(shop->partyMemberIcons[member], 0);
  wait(20);
  callbackPriority = 0x480;
  Task_Add(Shop_ResetEffects, callbackPriority);

  position.x = shop->partyMemberX[member] << 16;
  position.z = (shop->partyMemberY[member] << 16) - (12 << 16);

  effectIndex = 0;
  effect = shop->effects;
  do {
    Func_0808a528((struct Effect_080b2f4c *)effect, 0x2a0, position.x,
                  position.z);
    Func_0808a520((struct Effect_080b2f4c *)effect,
                  BattleEffect_UpdateRadialMotion);
    Func_0808a518((struct Effect_080b2f4c *)effect, 7);
    Func_08009248(effect->object, (Random16() * 7) >> 16);
    effect->scaleY = Q16_ONE * 7 / 10;
    effect->scaleX = Q16_ONE * 7 / 10;
    wait(3);
    if (effectIndex == 5) {
      shop->burstMember = member;
    }
    effectIndex++;
    effect++;
  } while (effectIndex <= 17);

  Sound_WaitForJingle();
  {
    u8 activeMode = 2;
    u8 *entry = (u8 *)&shop->effects[0].state;
    for (effectIndex = 23; effectIndex >= 0; effectIndex--) {
      if (*(s8 *)(entry + 5) != 0) {
        *entry = activeMode;
      }
      entry += sizeof(struct BurstSprite);
    }
  }

  wait(20);
  Sound_PlayFar(126);
  shop->burstMember = -1;
  Func_08009248(shop->partyMemberIcons[member], 0);
  wait(20);

  {
    u8 *flagEntry = (u8 *)&shop->effects[0].active;
    struct BurstSprite *effectSlot = shop->effects;
    for (effectIndex = 23; effectIndex >= 0; effectIndex--) {
      s32 flag = *flagEntry;

      flag <<= 24;

      flagEntry += sizeof(struct BurstSprite);
      if (flag != 0) {
        Func_0808a530((struct Effect_080b2f4c *)effectSlot);
      }
      effectSlot++;
    }
  }

  Task_Remove(Shop_ResetEffects);
  Func_08009280(shop->partyMemberIcons[member], 16);
  Shop_InitializeEffect();
  wait(30);
  shop->cursor.anchor->active = savedCursorMode;
}
