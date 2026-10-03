#ifndef ALCHEMY_GAME_STATE_H
#define ALCHEMY_GAME_STATE_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "BATTLE_UNIT.H"

/* The part of the saved game that records where the party is. */
struct GameState {
    u32 build_stamp;
    u32 play_time;
    u8 unknown_008[8];
    /* The party's coins. */
    s32 coins;
    u8 unknown_014[0x104];
    /* Purchases earn credit toward the shop's gifts. */
    s32 shop_credit;
    s8 shop_gifts;
    u8 unknown_11d[0x0f];
    /* The prizes the last lucky wheels game won, ended by -1; -2 once handed out. */
    s8 won_prizes[0x10];
    u8 unknown_13c[0x84];
    s16 scene;
    s16 entrance;
    s16 saved_scene;
    s16 saved_entrance;
    /* Where a scene change goes when it names no scene or entrance. */
    s16 default_scene;
    s16 default_entrance;
    /* The area of the last scene, used to keep its local story flags. */
    s16 scene_group;
    /* The scene and entrance a scene change asked for, -1 for the default. */
    s16 next_scene;
    s16 next_entrance;
    /* Where the party wakes after falling in battle, -1 for the default. */
    s16 defeat_scene;
    s16 defeat_entrance;
    /* The value Event_GetSpecialValue returns, which battle effects set. */
    s16 special;
    u8 unknown_1d8[2];
    /* The map the current scene shows, from its row of the scene table. */
    u16 map;
    /* The party's position on the map, in 16.16 fixed point. */
    s32 x;
    s32 y;
    s32 z;
    /* The party's heading and turn, which a restored view sets. */
    u32 heading;
    u16 turn;
    /* The cue a scene fading in from white plays first. */
    s16 scene_cue;
    u8 unknown_1f0[2];
    /* How the leader moves: 1 on a ladder, 2 in the Suhara desert's sand. */
    u8 movement_mode;
    u8 unknown_1f3;
    /* The actor whose position actor contact is measured from. */
    s32 selected_actor;
    /* The ids of the party's active members, 0xff after the last. */
    u8 active_owners[8];
    u8 unknown_200[5];
    /* The two values the palette glow is restored from. */
    u8 palette_glow[2];
    u8 unknown_207[8];
    u8 unknown_20f;
    u8 unknown_210[0xc];
    /* Keys that make the leader dash on the world map. */
    u16 dash_keys;
    /* The cue a scene the party returns to plays, -1 for the scene's own. */
    s16 return_cue;
    /* The two Psynergy shortcuts: the owner in the top six bits, the
       ability in the low ten. */
    u16 first_shortcut;
    u16 second_shortcut;
    /* Set once the world map's perspective scene is up. */
    u8 perspective_scene;
    u8 unknown_225[5];
    u8 saved_options;
    /* How the next battle begins: 2 when a tile's ambush starts it. */
    u8 battle_start;
    u16 unknown_22c;
    u16 unknown_22e;
    u16 unknown_230;
    s16 unknown_232;
    /* The Djinni event the field resolves once the scene is up. */
    s16 pending_djinn_event;
    /* Why the scene is changing: -1 when the party fell in battle. */
    s16 scene_change_reason;
    s32 unknown_238;
    /* The item the field hands the party once the scene is up. */
    u16 pending_item;
    s16 unknown_23e;
    /* Where Retreat returns the party. */
    s16 retreat_scene;
    s16 retreat_entrance;
    s32 unknown_244;
    u8 unknown_248[2];
    s16 unknown_24a;
    /* Set while Cloak hides the party; no monsters appear. */
    s16 cloaked;
    u8 unknown_24e[0x56];
    /* The link lobby's tallies, which its rounds count and its attendants
       report (MENU/LINK_LOBBY). */
    u16 link_tallies[8];
    u8 unknown_2b4[0x0c];
    struct BattleUnit owners[8];
};

LAYOUT_OFFSET_GUARD(GameState_BuildStamp, struct GameState, build_stamp, 0);
LAYOUT_OFFSET_GUARD(GameState_PlayTime, struct GameState, play_time, 4);
LAYOUT_OFFSET_GUARD(GameState_SavedOptions, struct GameState, saved_options, 0x22a);
LAYOUT_OFFSET_GUARD(GameState_Coins, struct GameState, coins, 0x10);
LAYOUT_OFFSET_GUARD(GameState_ShopCredit, struct GameState, shop_credit, 0x118);
LAYOUT_OFFSET_GUARD(GameState_ShopGifts, struct GameState, shop_gifts, 0x11c);
LAYOUT_OFFSET_GUARD(GameState_Owners, struct GameState, owners, 0x2c0);
LAYOUT_SIZE_GUARD(GameState_Size, struct GameState, 0xd20);
LAYOUT_OFFSET_GUARD(GameState_WonPrizes, struct GameState, won_prizes, 0x12c);
LAYOUT_OFFSET_GUARD(GameState_SceneGroup, struct GameState, scene_group, 0x1cc);
LAYOUT_OFFSET_GUARD(GameState_DashKeys, struct GameState, dash_keys, 0x21c);
LAYOUT_OFFSET_GUARD(GameState_Unknown244, struct GameState, unknown_244, 0x244);
LAYOUT_OFFSET_GUARD(GameState_Entrance, struct GameState, entrance, 0x1c2);
LAYOUT_OFFSET_GUARD(GameState_Special, struct GameState, special, 0x1d6);
LAYOUT_OFFSET_GUARD(GameState_X, struct GameState, x, 0x1dc);
LAYOUT_OFFSET_GUARD(GameState_MovementMode, struct GameState, movement_mode, 0x1f2);
LAYOUT_OFFSET_GUARD(GameState_SelectedActor, struct GameState, selected_actor, 0x1f4);
LAYOUT_OFFSET_GUARD(GameState_ActiveOwners, struct GameState, active_owners, 0x1f8);
LAYOUT_OFFSET_GUARD(GameState_FirstShortcut, struct GameState, first_shortcut, 0x220);
LAYOUT_OFFSET_GUARD(GameState_RetreatEntrance, struct GameState, retreat_entrance, 0x242);
LAYOUT_OFFSET_GUARD(GameState_Cloaked, struct GameState, cloaked, 0x24c);
LAYOUT_OFFSET_GUARD(GameState_LinkTallies, struct GameState, link_tallies, 0x2a4);

extern struct GameState gGameState;

#endif
