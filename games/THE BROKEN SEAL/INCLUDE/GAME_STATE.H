#ifndef ALCHEMY_GAME_STATE_H
#define ALCHEMY_GAME_STATE_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* The part of the saved game that records where the party is. */
struct GameState {
    u8 unknown_000[0x10];
    /* The party's coins. */
    s32 coins;
    u8 unknown_014[0x118];
    /* The prizes the last lucky wheels game won, ended by -1; -2 once handed out. */
    s8 won_prizes[0x10];
    u8 unknown_13c[0x84];
    s16 scene;
    s16 entrance;
    s16 saved_scene;
    s16 saved_entrance;
    u8 unknown_1c8[0x0e];
    /* The value Event_GetSpecialValue returns, which battle effects set. */
    s16 special;
    u8 unknown_1d8[4];
    /* The party's position on the map, in 16.16 fixed point. */
    s32 x;
    s32 y;
    s32 z;
    /* The party's heading and turn, which a restored view sets. */
    u32 heading;
    u16 turn;
    u8 unknown_1ee[4];
    /* How the leader moves: 1 on a ladder, 2 in the Suhara desert's sand. */
    u8 movement_mode;
    u8 unknown_1f3;
    /* The actor whose position actor contact is measured from. */
    s32 selected_actor;
    /* The ids of the party's active members, 0xff after the last. */
    u8 active_owners[8];
    u8 unknown_200[0x2c];
    u16 unknown_22c;
    u16 unknown_22e;
    u16 unknown_230;
    s16 unknown_232;
    u8 unknown_234[0x0a];
    s16 unknown_23e;
    /* Where Retreat returns the party. */
    s16 retreat_scene;
    s16 retreat_entrance;
    u8 unknown_244[8];
    /* Set while Cloak hides the party; no monsters appear. */
    s16 cloaked;
    u8 unknown_24e[0x56];
    /* The link lobby's tallies, which its rounds count and its attendants
       report (MENU/LINK_LOBBY). */
    u16 link_tallies[8];
};

LAYOUT_OFFSET_GUARD(GameState_Coins, struct GameState, coins, 0x10);
LAYOUT_OFFSET_GUARD(GameState_WonPrizes, struct GameState, won_prizes, 0x12c);
LAYOUT_OFFSET_GUARD(GameState_Entrance, struct GameState, entrance, 0x1c2);
LAYOUT_OFFSET_GUARD(GameState_Special, struct GameState, special, 0x1d6);
LAYOUT_OFFSET_GUARD(GameState_X, struct GameState, x, 0x1dc);
LAYOUT_OFFSET_GUARD(GameState_MovementMode, struct GameState, movement_mode, 0x1f2);
LAYOUT_OFFSET_GUARD(GameState_SelectedActor, struct GameState, selected_actor, 0x1f4);
LAYOUT_OFFSET_GUARD(GameState_ActiveOwners, struct GameState, active_owners, 0x1f8);
LAYOUT_OFFSET_GUARD(GameState_RetreatEntrance, struct GameState, retreat_entrance, 0x242);
LAYOUT_OFFSET_GUARD(GameState_Cloaked, struct GameState, cloaked, 0x24c);
LAYOUT_OFFSET_GUARD(GameState_LinkTallies, struct GameState, link_tallies, 0x2a4);

extern struct GameState gGameState;

#endif
