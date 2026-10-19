#ifndef ALCHEMY_PARTY_STATE_H
#define ALCHEMY_PARTY_STATE_H

#include "types.h"

struct PartyState {
  u8 unk000[4];
  u32 playTime;
  u8 unk008[8];
  s32 coins;
  u8 unk014[0x124];
  s32 counterB;
  s8 counterC;
  u8 unk13d[0xa3];
  s16 scene;
  s16 entrance;
  u16 savedScene;
  u16 savedEntrance;
  s16 primary;
  s16 secondary;
  s16 sceneGroup;
  u16 pair1ee[2];
  u16 pair1f2[2];
  s16 special;
  u8 unk1f8[2];
  s16 map;
  u8 unk1fc[0x12];
  s16 sceneCue;
  s16 cue;
  u8 renderMode;
  u8 unk213;
  s32 currentOwner;
  u8 activeOwners[8];
  u8 unk220[5];
  u8 paletteGlow[2];
  u8 unk227[3];
  u8 battleFxEnabled;
  u8 unk22b;
  u8 eventMode;
  u8 unk22d[3];
  u16 keys230;
  u16 unk232;
  u16 keys234;
  u16 keys236;
  u8 unk238[6];
  s16 returnCue; // the music to play on returning, -1 for the scene's own
  u16 packedEvents[2];
  u8 unk244[7];
  u8 battleRule24b;
  u8 unk24c[6];
  u16 hazardSteps;
  s16 pendingDjinnEvent;
  s16 unk256;
  s32 encounterSteps;
  u16 tally25c;
  s16 sceneVariant;
  s16 retreatScene;
  s16 retreatEntrance;
  s32 avoidDistance;
  s8 avoidByItem;
  u8 savedByte;
  s16 pausedObject; // the object field Psynergy has stopped, -1 for none
  s8 pendingThird;
  s8 cloaked; // Cloak: the leader is hidden
  u8 unk26e[2];
  u32 savedCallback;
  u8 counter274;
  u8 unk275;
  s16 ownerOverride;
  s32 savedX;
  s32 savedZ;
  s32 unk280;
  u8 unk284[0x38];
  u16 forgeItem;
  u8 unk2be[4];
  u8 unk2c2;
  u8 unk2c3;
  u16 linkTallies[8];
};

extern struct PartyState gPartyState;
s32 Party_CountActiveOwners(void);
s32 Party_ListActiveOwners(s16 *owners);
void Unit_LevelUpTo(s32 owner, s32 level);

#endif
