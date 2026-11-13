#ifndef ALCHEMY_MODULE_SCENE_TRIAL_ROAD_H
#define ALCHEMY_MODULE_SCENE_TRIAL_ROAD_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

struct Actor;
struct FieldSprite;
union FieldObject;

struct EventWork184 {
  u8 unk000[388];
  s16 value;
};

/// The head of a mover's slot, as Func_02004224 fills it.
struct TrialRoadMover {
  s16 id;
  s16 value;
  s16 unk04;
  s16 unk06;
  s16 unk08;
  s16 unk0a;
};

/// A view of the map work's byte 23.
struct MapWorkByte23 {
  u8 unk00[0x17];
  u8 bits0 : 2;
  // XXX: what bits 2 and 3 select; the map code that reads them would settle
  // it.
  u8 bits2 : 2;
  u8 bits4 : 4;
};

struct TrialRoadStage {
  s16 order[8];
  s16 pattern;
  s16 mirrored;
  s16 timer;
  s16 current;
  s16 stage;
  s16 unk1a;
  s16 best;
};

// The state as this function reads it: its first 16 bytes are the dealt order.
struct TrialRoadDeal {
  s16 order[8];
  s16 pattern;
  s16 mirrored;
  s16 field14;
  s16 field16[2];
  s16 field1a;
  s16 field1c;
};

struct TrialRoadGauge {
  u16 handle;
  u16 field02;
  u8 unk004[0x1c0];
  void *buffer;
};

/// The sprite's priority bits, read as the byte that holds them.
struct EffectSpriteBits {
  u8 unk0 : 2;
  u8 priority : 2;
} __attribute__((packed));

/// A sprite's attribute 2, as the byte bits view reaches it.
struct EffectSpriteAttributes {
  u8 unk00[9];
  struct EffectSpriteBits bits;
};

struct TrialRoadReplay {
  s16 mode;
  s16 mirrored;
  s16 id;
  s16 point;
  s16 still;
  s16 resource;
};

struct TrialRoadLinkWork {
  u8 unk00[0xe4];
  u16 rival;
  u16 unke6;
  u16 linkAngle;
  s16 resource;
  s32 linkX;
  s32 linkZ;
  s16 path[0x1c20];
};

/// One queued sprite: the three words Oam_AddSpriteAtDepth adds to OAM.
struct TrialRoadSprite {
  u32 words[3];
};

struct TrialRoadLink {
  u8 unk00[0xd8];
  s16 vramEntry;
  u16 unkda;
  u8 unkdc[4];
  u16 unke0;
  u16 a;
  u16 b;
  u8 unke6[6];
  s32 height;
  s32 unkf0;
};

struct TrialRoadControl {
  s16 state;
  s16 field02;
  s16 field04;
  s16 field06;
  s16 field08;
  s16 field0a;
};

s32 TrialRoad_IsMirrored(void);

void Func_02000418(void);

void Func_02000768(void);

void Func_020007e8(void);

void Func_020008a4(void);

void Func_02000c34(void);

void Func_02000c48(void);

void Func_02000c5c(void);

void Func_02000c70(void);

void Func_02000c84(void);

void Func_02000cc0(void);

void Func_0200106c(void);

void Func_02001080(void);

void Func_02001090(void);

void Func_020010a4(void);

void Func_020010b4(void);

void Func_020010c8(void);

void Func_020010d8(void);

void Func_020011c0(void);

void Func_020011dc(void);

void Func_020011f8(void);

void Func_020014e4(void);

void Func_020015a8(void);

void Func_020015b8(void);

void Func_020015d8(void);

void Func_0200163c(void);

void Func_020016c8(void);

void Func_020017e0(void);

extern struct ActorPlacement gData_02006484[];
extern struct ActorPlacement gData_020065d4[];
extern struct ActorPlacement gData_02006754[];
extern struct ActorPlacement gData_020068bc[];
extern struct ActorPlacement gData_02006a3c[];
extern struct ActorPlacement gData_02006b2c[];
extern struct ActorPlacement gData_02006c4c[];
extern struct ActorPlacement gData_02006d9c[];
extern struct Trigger gData_02007050[];
extern struct Trigger gData_02007200[];
extern struct Trigger gData_020073bc[];
extern struct Trigger gData_0200756c[];
extern struct Trigger gData_02007740[];
extern struct Trigger gData_0200789c[];
extern struct Trigger gData_02007a64[];
extern struct Trigger gData_02007bcc[];

void InventorySnapshot_Restore();

void TrialRoad_Idle(void);

s32 Func_020000fc(s32 fallback);

/// Func_080af0e4 through its far call: empties an inventory slot and returns 2.
s32 Func_02005870(s32 owner, s32 slot);

extern s16 gData_02007734[];

void TrialRoad_ReachPeakFirst(void);
void TrialRoad_ReachPeakSecond(void);

void Func_02000924(s32 id);

void Func_0200095c(s32 id);

void Func_02000d70(void);

void Func_02004188(s32 a, s32 b);

void Func_02005a08(void);
void Func_02005a10(s32 a, s32 b, s32 c);

extern u32 gData_02005b44[];

extern u16 gData_02007e28[];

void PaletteCycle_StepAt(void);
void Func_02001754();
void TrialRoad_PlaceMoapaAtPeak(void);
void TrialRoad_BeatMoapa(void);
void TrialRoad_LoseToMoapa(void);
void TrialRoad_ShowTraps(void);

void Func_02001dd0(void);

void Func_02001eb0(void);

extern u32 gData_02005b84[];
s32 Func_020056e8(void);
void Func_020056c4(s32 a);
void TrialRoad_DealStageOrder(u32 seed);

void Func_02005828(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_020055c4(s32 a, s32 b);

void Func_02000218(s32 side, s32 phase);

void Import_FieldCameraWaitForArrival(void);

extern u32 gData_02005c88[];

extern u32 gData_02005be8[];

extern u32 gData_02005d28[];
extern u32 gData_02005e40[];

extern u32 gData_02005ed4[];

void Import_DecompressTaggedLz(const void *source, void *destination);
s32 Func_02005a18(struct Actor *object);
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
void Func_02005958(s32 id);
s32 Func_02005a20(s32 layer, s32 shape, s32 height);
struct Actor *Func_02005a28(s32 *position, s32 mode);

void Func_02004a94(s32 value);
void Func_02004d6c(s32 offset, s32 layer);
void Func_02004bc8(void);
void Func_02004ce0(s32 a, s32 b, s32 c);

u32 Sprite_ShareVramCacheEntry(struct FieldSprite *sprite, u32 frame);

void Func_02004a14(union FieldObject *object);

void Actor_StartIdle(struct Actor *object);

extern const u16 Data_020062b4[];

void Func_02004ddc(void);
void Func_02001904(void);
void Func_02001974(void);
void Func_02001a70(void);
void Func_02001afc(void);
void Func_02001bdc(void);
void Func_02001bec(void);
void Func_02001c6c(void);

void Func_02001d8c(void);

void Func_02003da0(void);

#endif
