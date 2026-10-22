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
  struct RenderOutput *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

// The shop work block, as this function reads it.
struct ShopRuntime {
  u8 unk000[0xf8];
  s32 partyMemberIcons[8];
  u8 unk118[0x3c4];
  struct ShopCursor cursor;
  u8 unk4ec[0x1b];
  u8 partyAction;
};
extern u8 gData_03001f2c[];

struct Position {
  s32 x, y, z;
};
struct Effect_080b2f4c {
  u8 filler[0x48];
};
struct ShopBurstRuntime {
  u8 unk000[0x118];
  s16 memberX[8];
  s16 memberZ[8];
};

u32 Random16(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 sound_id);

void Shop_RestoreSceneTiles(s32 address);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 flags);
void Func_0808a528(struct Effect_080b2f4c *effect, s32 mode, s32 x, s32 z);
void Func_0808a520(struct Effect_080b2f4c *effect,
                   void (*callback)(struct Effect_080b2f4c *));
void Func_0808a518(struct Effect_080b2f4c *effect, s32 value);
void Func_08009248(s32 object, u32 frame_offset);
void AudioCommand_WaitForStateByteClear(void);
void Func_0808a530(struct Effect_080b2f4c *effect);
void Scheduler_RemoveCallback(void (*callback)(void));
void Func_08009280(s32 object, s32 arg);
void Shop_InitializeEffect(void);
void Shop_ResetEffects(void);
void BattleEffect_UpdateRadialMotion(struct Effect_080b2f4c *effect);

extern s8 Data_0810cf28[];

void Shop_RunPartyMemberIconBurst(s32 member) {
  struct ShopRuntime *shop;
  struct ShopBurstRuntime *burst;
  s8 savedKind;
  struct Position position;
  struct Effect_080b2f4c *effect;
  s32 callbackFlags;
  s32 i;

  shop = Ram_HeapSlots->menuRuntime;
  burst = (struct ShopBurstRuntime *)shop;
  savedKind = shop->cursor.anchor->active;
  *(u8 *)((u8 *)shop + 0x508) = 0xff;
  shop->cursor.anchor->active = 13;
  Audio_PlayCue(Data_0810cf28[(s8)shop->partyAction]);
  Shop_RestoreSceneTiles(0x00202108);
  Func_08009280((s32)shop->partyMemberIcons[member], 0);
  WaitFrames(20);
  callbackFlags = 0x480;
  Scheduler_AddOrUpdateCallback(Shop_ResetEffects, callbackFlags);

  position.x = (s32)burst->memberX[member] << 16;
  position.z = ((s32)burst->memberZ[member] << 16) + (s32)0xfff40000;

  i = 0;
  effect = (struct Effect_080b2f4c *)((u8 *)shop + 0x50c);
  do {
    Func_0808a528(effect, 0x2a0, position.x, position.z);
    Func_0808a520(effect, BattleEffect_UpdateRadialMotion);
    Func_0808a518(effect, 7);
    Func_08009248(*(s32 *)((u8 *)effect + 0), (Random16() * 7) >> 16);
    *(s32 *)((u8 *)effect + 44) = 0xb333;
    *(s32 *)((u8 *)effect + 40) = 0xb333;
    WaitFrames(3);
    if (i == 5) {
      *(u8 *)((u8 *)shop + 0x508) = (u8)member;
    }
    i++;
    effect = (struct Effect_080b2f4c *)((u8 *)effect + 0x48);
  } while (i <= 17);

  AudioCommand_WaitForStateByteClear();
  {
    u8 activeMode = 2;
    u8 *entry = (u8 *)shop + 0x54c;
    for (i = 23; i >= 0; i--) {
      if (*(s8 *)(entry + 5) != 0) {
        *(u8 *)(entry + 0) = activeMode;
      }
      entry += 0x48;
    }
  }

  WaitFrames(20);
  Audio_PlayCue(126);
  *(u8 *)((u8 *)shop + 0x508) = 0xff;
  Func_08009248(shop->partyMemberIcons[member], 0);
  WaitFrames(20);

  {
    u8 *flagEntry = (u8 *)shop + 0x551;
    struct Effect_080b2f4c *entry2 =
        (struct Effect_080b2f4c *)((u8 *)shop + 0x50c);
    for (i = 23; i >= 0; i--) {
      s32 flag = *flagEntry;

      flag <<= 24;

      flagEntry += 0x48;
      if (flag != 0) {
        Func_0808a530(entry2);
      }
      entry2 = (struct Effect_080b2f4c *)((u8 *)entry2 + 0x48);
    }
  }

  Scheduler_RemoveCallback(Shop_ResetEffects);
  Func_08009280((s32)shop->partyMemberIcons[member], 16);
  Shop_InitializeEffect();
  WaitFrames(30);
  shop->cursor.anchor->active = savedKind;
}
