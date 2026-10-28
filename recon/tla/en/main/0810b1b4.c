// 2026-10-26: read through typed structs as The Broken Seal's twin is (the
// shop work's icons, positions, action, burst member and its 24 sparks, each
// a 0x48-byte field effect sprite); differ unchanged at 18.
// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 18 of 194 in tla-en. First difference: this issues `ldr r2, [r1, #0]` earlier
// (row 25) than the native does (row 27).
// Near miss, differ 18 of 194: the sanctum's revival burst for a party member.
// It hides the cursor, plays the service's sound, darkens the member's icon,
// spawns 18 radial sparks around the member's position
// (BattleEffect_UpdateRadialMotion), then fades them, restores the icon and the
// cursor. ☀️'s Shop_RunPartyMemberIconBurst in games/tbs/src/shop/shop.c, with
// ⚓️'s work-block offsets (icons 0xf8, positions 0x118 and 0x128, burst flag
// 0x508, sparks from 0x50c), effect mode 0x2a0 and callback flags 0x480.
// Instruction order only: the anchor's reload against the 13 store, the
// member's save to r10, the sound id's signed load past the frame setup, and
// the spark position's constants against their stores. The ☀️ draft's FAKEMATCH
// union on the x store is gone: the plain store gives the same differ here.
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
void WaitFrames(s32 frames);
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
  WaitFrames(20);
  callbackPriority = 0x480;
  Task_Add(Shop_ResetEffects, callbackPriority);

  position.x = shop->partyMemberX[member] << 16;
  position.z = (shop->partyMemberY[member] << 16) + 0xfff40000;

  effectIndex = 0;
  effect = shop->effects;
  do {
    Func_0808a528((struct Effect_080b2f4c *)effect, 0x2a0, position.x,
                  position.z);
    Func_0808a520((struct Effect_080b2f4c *)effect,
                  BattleEffect_UpdateRadialMotion);
    Func_0808a518((struct Effect_080b2f4c *)effect, 7);
    Func_08009248(effect->object, (Random16() * 7) >> 16);
    effect->scaleY = 0xb333;
    effect->scaleX = 0xb333;
    WaitFrames(3);
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

  WaitFrames(20);
  Sound_PlayFar(126);
  shop->burstMember = -1;
  Func_08009248(shop->partyMemberIcons[member], 0);
  WaitFrames(20);

  {
    u8 *flagEntry = (u8 *)&shop->effects[0].active;
    struct BurstSprite *effectSlot = shop->effects;
    for (effectIndex = 23; effectIndex >= 0; effectIndex--) {
      s32 flag = *flagEntry;

      flag <<= 24;

      flagEntry += 0x48;
      if (flag != 0) {
        Func_0808a530((struct Effect_080b2f4c *)effectSlot);
      }
      effectSlot++;
    }
  }

  Task_Remove(Shop_ResetEffects);
  Func_08009280(shop->partyMemberIcons[member], 16);
  Shop_InitializeEffect();
  WaitFrames(30);
  shop->cursor.anchor->active = savedCursorMode;
}
