#ifndef ALCHEMY_MAIN_ROUTINE_WORK_H
#define ALCHEMY_MAIN_ROUTINE_WORK_H

#include "types.h"

/// gMainRoutineWork.battleType: how the next battle starts, as ☀️'s; it is
/// set back to BATTLE_TYPE_NORMAL as the battle ends.
enum BattleType {
  BATTLE_TYPE_NORMAL = 0,
  /// A battle the party cannot run from: a Mimic's, a hidden Mad Plant's and
  /// the bosses'.
  BATTLE_TYPE_NO_ESCAPE = 2,
  BATTLE_TYPE_LINK = 4, ///< a link battle, from the lobby
  BATTLE_TYPE_DEBUG = 5, ///< the debug battle menu's mark
  /// A battle the party is meant to lose: it is caught off guard and every
  /// hit downs its target, as when Karst and Agatio fight Isaac and Ivan.
  BATTLE_TYPE_LOST = 6
};

/// The first pending Djinni battle event: DJINNI_BATTLE_EVENT_FIRST plus the
/// Djinni's number (Field_RequestDjinnBattle).
#define DJINNI_BATTLE_EVENT_FIRST 300

/// gMainRoutineWork, the state a save file keeps (☀️'s struct MainRoutineWork,
/// 0x20 bytes further on from the shop credit): play time, coins, the scene and
/// the leader's position, the active party and the options.
struct MainRoutineWork {
  u8 unk000[4]; ///< 0 on a new game; not named by ⚓️ code
  u32 playTime; ///< frames
  /// Not named by ⚓️ code. Djinn_GetRecoveryTable's TradeOfferState starts at
  /// byte 4, its first 8 bytes running over coins.
  u8 unk008[8];
  s32 coins; ///< at most 999999 (Party_AdjustCoins)
  /// The party's Djinn recovery list (TradeOfferState from its offers on),
  /// read through Djinn_GetRecoveryTable; not named by ⚓️ code.
  u8 unk014[0x124];
  s32 counterB; ///< shop credit, at most 999999 (Party_AdjustShopCredit)
  s8 counterC;  ///< shop gifts, 0 to 28 (Party_AdjustShopGifts)
  /// Not named by ⚓️ code; a new game sets its first 15 bytes as ☀️'s
  /// prizeWeights (three each of 4, 8, 16, 32 and 64).
  u8 unk13d[0xa3];
  s16 scene;
  s16 entrance;
  u16 savedScene;    ///< where a defeat returns to without a defeat point
  u16 savedEntrance; ///< where a defeat returns to without a defeat point
  s16 primary;       ///< the default scene: the fallback of both destinations
  s16 secondary;  ///< the default entrance: the fallback of both destinations
  s16 sceneGroup; ///< the current scene's group; a new group resets its state
  /// The next destination (scene, entrance), -1 for none; the default scene
  /// and entrance stand in for either.
  u16 pair1ee[2];
  u16 pair1f2[2]; ///< the defeat destination (scene, entrance); -1 for none
  s16 battleBackground; ///< the battle background chosen for the scene or
                        ///< terrain
  u8 unk1f8[2];         ///< not named by ⚓️ code
  s16 map;
  s32 x; ///< the party's position on entering, as ☀️'s gMainRoutineWork
  s32 y;
  s32 z;
  s32 heading;
  u16 turn;     ///< 0 on taking an entrance, as ☀️'s turn
  s16 sceneCue; ///< the battle music chosen for the encounter
  s16 cue;      ///< the current scene's field music
  /// The leader's movement: 0 on foot, 2 climbing, 3 or 4 holding a
  /// workspace item or node; 7, 8 and 10 set by the world map and others.
  u8 renderMode;
  u8 unk213;          ///< not named by ⚓️ code
  s32 currentOwner;   ///< the leader, as ☀️'s selectedActor
  u8 activeOwners[8]; ///< the party in order
  u8 unk220[5];       ///< not named by ⚓️ code
  u8 paletteGlow[2];  ///< window hue and brightness (options 0 and 1)
  u8 unk227[3];       ///< not named by ⚓️ code
  u8 battleFxEnabled; ///< option 3, on or off; 1 on a new game
  u8 unk22b;          ///< 1 on a new game; not named by ⚓️ code
  /// Option 2, the message speed, 0 to 2: 1 on a new game; some scenes set 2
  /// and restore it after.
  u8 eventMode;
  u8 unk22d[3]; ///< byte 2 goes into the save summary's sendFlag
  /// The buttons for each field command, as KEY_ masks, set on a new game;
  /// read with gInput.pressed except dashKeys, held.
  ///@{
  u16 aKeys;              ///< examine; KEY_A
  u16 bKeys;              ///< KEY_B; held with aKeys, it makes flight climb
  u16 selectKeys;         ///< the menu; KEY_SELECT
  u16 startKeys;          ///< KEY_START
  u16 firstShortcutKeys;  ///< KEY_L
  u16 secondShortcutKeys; ///< KEY_R
  u16 dashKeys;           ///< KEY_B
  ///@}
  s16 returnCue; ///< the music to play on returning, -1 for the scene's own
  /// The shortcuts' assignments, run by firstShortcutKeys and
  /// secondShortcutKeys (PsynergyMenu_SetShortcut: kind << 10 | id).
  u16 packedEvents[2];
  /// Not named by ⚓️ code; byte 6 is option 4, the idle sleep, as ☀️'s
  /// idleSleepEnabled.
  u8 unk244[7];
  u8 battleType; ///< an enum BattleType, set back to BATTLE_TYPE_NORMAL at the end
  u8 unk24c[6];    ///< not named by ⚓️ code
  u16 hazardSteps; ///< reset on entering a new scene group
  /// The Djinni battle pending (DJINNI_BATTLE_EVENT_FIRST plus its number, flags
  /// in bits 15 to 11), or a Mimic's flag with CONDITION_FLAG_SET, resolved on
  /// returning to the field.
  s16 pendingDjinni;
  s16 sequenceResult; ///< the last battle's result, as ☀️'s; -1 for a defeat
  s32 encounterSteps; ///< reset on entering a new scene group
  /// Cleared with the battle's spoils, as ☀️'s pendingItem; not read by name
  /// in ⚓️.
  u16 tally25c;
  s16 sceneVariant;    ///< the current scene's variant from its record
  s16 retreatScene;    ///< where Retreat goes; the group's first entrance
  s16 retreatEntrance; ///< where Retreat goes; the group's first entrance
  s32 avoidDistance;   ///< distance left of Avoid, 16.16 pixels; 0 when off
  s8 avoidByItem;      ///< Avoid came from an item (flag 0x145)
  u8 savedByte;        ///< the paused object's first animation param
  s16 pausedObject;    ///< the object field Psynergy has stopped, -1 for none
  /// Nonzero runs packed action 0x208b with the pending actions, and idles
  /// the field's push and button handler; not written by name in ⚓️.
  s8 pendingThird;
  s8 cloaked;        ///< Cloak: the leader is hidden
  u8 unk26e[2];      ///< not named by ⚓️ code
  u32 savedCallback; ///< the paused object's update, restored on resuming
  /// The answers events count: telling Kraden on Idejima that the Western
  /// Sea is not where they go, agreeing with Alhafra's mayor that the party
  /// owes him, and others; Ancient Lemuria asks its question only while this
  /// is 4 or less. 0 on a new game, counted up by Func_080d2d70.
  // XXX: what the counted answers share; reading every site's question and
  // the Ancient Lemuria event that tests the count would settle it.
  u8 countedAnswers;
  u8 unk275; ///< not named by ⚓️ code
  /// Nonzero makes actor 8, not currentOwner, the one events control.
  s16 ownerOverride;
  s32 savedX;      ///< the world map leader's x, kept on leaving
  s32 savedZ;      ///< the world map leader's z, kept on leaving
  s32 unk280;      ///< cleared with savedX; not read by name in ⚓️
  u8 unk284[0x38]; ///< not named by ⚓️ code
  u16 forgeItem;   ///< the item left at the forge (Forge_SetItem)
  /// SceneId_WorldMap after a destination is chosen on the world map, 0xffff
  /// after cancelling (WorldPicture_Show); not read by name in ⚓️ code.
  u16 unk2be;
  u8 unk2c0[2]; ///< not named by ⚓️ code
  u8 unk2c2;    ///< 0, 1 or 2, set by the Islet Cave and Contigo scenes
  u8 unk2c3;    ///< 0 or 1, set by a Yampi Desert scene
  /// The link lobby's tallies, as ☀️'s: 3 cleared on choosing a monster
  /// battle, 0 and 3 counted up together, 6 read by the lobby.
  u16 linkTallies[8];
};

extern struct MainRoutineWork gMainRoutineWork;
s32 Party_CountMembers(void);
s32 Party_ListMembers(s16 *owners);
void Unit_LevelUpTo(s32 owner, s32 level);

#endif
