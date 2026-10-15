#ifndef ALCHEMY_PARTY_STATE_H
#define ALCHEMY_PARTY_STATE_H

#include "types.h"

struct PartyState {
    u8 unknown_000[4];
    u32 play_time;
    u8 unknown_008[8];
    s32 coins;
    u8 unknown_014[0x124];
    s32 counter_b;
    s8 counter_c;
    u8 unknown_13d[0xa3];
    s16 scene;
    s16 entrance;
    u16 saved_scene;
    u16 saved_entrance;
    s16 primary;
    s16 secondary;
    s16 scene_group;
    u16 pair_1ee[2];
    u16 pair_1f2[2];
    s16 special;
    u8 unknown_1f8[2];
    s16 map;
    u8 unknown_1fc[0x12];
    s16 scene_cue;
    s16 cue;
    u8 render_mode;
    u8 unknown_213;
    s32 current_owner;
    u8 active_owners[8];
    u8 unknown_220[0xa];
    u8 battle_fx_enabled;
    u8 unknown_22b;
    u8 event_mode;
    u8 unknown_22d[3];
    u16 keys_230;
    u16 unknown_232;
    u16 keys_234;
    u16 keys_236;
    u8 unknown_238[8];
    u16 packed_events[2];
    u8 unknown_244[7];
    u8 battle_rule_24b;
    u8 unknown_24c[8];
    s16 pending_djinn_event;
    s16 unknown_256;
    s32 encounter_steps;
    u16 tally_25c;
    u8 unknown_25e[6];
    s32 avoid_distance;
    s8 avoid_by_item;
    u8 saved_byte;
    u8 unknown_26a[2];
    s8 pending_third;
    u8 unknown_26d[3];
    u32 saved_callback;
    u8 counter_274;
    u8 unknown_275;
    s16 owner_override;
    s32 saved_x;
    s32 saved_z;
    s32 unknown_280;
    u8 unknown_284[0x38];
    u16 forge_item;
    u8 unknown_2be[4];
    u8 unknown_2c2;
    u8 unknown_2c3;
    u16 link_tallies[8];
};

extern struct PartyState gPartyState;
s32 Party_CountActiveOwners(void);
s32 Party_ListActiveOwners(s16 *owners);
void Party_AdvanceOwnerCountToTarget(s32 owner, s32 level);

#endif
