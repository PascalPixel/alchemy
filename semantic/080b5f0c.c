/*
 * Veneer audit (2026-08-01): the ROM branch at these copy sites enters the
 * `__call_via_r3` veneer with r3 = 0x03001388. That IWRAM address is the
 * relocated three-argument word-copy routine, represented below as an
 * ordinary function pointer. It is not data and it is never an argument.
 */
#include "layout_guard.h"
#include "types.h"

typedef void *(*WordCopy_080b5f0c)(
    void *destination,
    const void *source,
    s32 size);

struct BattleRosterMap_080b5f0c {
    u8 padding00[0x48];
    u8 encoded_slot_by_actor[8];
};

struct CharacterTransfer_080b5f0c {
    u8 payload[0x12a];
    u8 transfer_kind;
    u8 padding12b[0x29];
};

struct PartyMemberRef_080b5f0c {
    u8 padding00[2];
    u8 actor_id;
    u8 padding03;
};

struct PartyTransfer_080b5f0c {
    u8 padding000[8];
    struct PartyMemberRef_080b5f0c members[64];
    s32 member_count;
    u8 padding10c[0x34];
};

LAYOUT_OFFSET_GUARD(
    BattleRosterMap080b5f0c_EncodedSlots,
    struct BattleRosterMap_080b5f0c,
    encoded_slot_by_actor,
    0x48);
LAYOUT_OFFSET_GUARD(
    CharacterTransfer080b5f0c_TransferKind,
    struct CharacterTransfer_080b5f0c,
    transfer_kind,
    0x12a);
LAYOUT_SIZE_GUARD(
    CharacterTransfer080b5f0c_Size,
    struct CharacterTransfer_080b5f0c,
    0x154);
LAYOUT_SIZE_GUARD(
    PartyMemberRef080b5f0c_Size,
    struct PartyMemberRef_080b5f0c,
    4);
LAYOUT_OFFSET_GUARD(
    PartyTransfer080b5f0c_Members,
    struct PartyTransfer_080b5f0c,
    members,
    0x08);
LAYOUT_OFFSET_GUARD(
    PartyTransfer080b5f0c_MemberCount,
    struct PartyTransfer_080b5f0c,
    member_count,
    0x108);
LAYOUT_SIZE_GUARD(
    PartyTransfer080b5f0c_Size,
    struct PartyTransfer_080b5f0c,
    0x140);

extern struct BattleRosterMap_080b5f0c *Data_03001e74;

void Func_08002df0(void *allocation);
void Func_080030f8(u32 frames);
void *Func_08004970(s32 size);
s32 Func_080063bc(const void *packet, s32 size);
void Func_08006458(void);
struct PartyTransfer_080b5f0c *Func_08077000(s32 party);
const struct CharacterTransfer_080b5f0c *Func_08077008(s32 actor);
s32 Func_080b6a60(u16 actor_ids[8]);

static WordCopy_080b5f0c const WordCopyAt_03001388_080b5f0c =
    (WordCopy_080b5f0c)0x03001388;

static s32 SendTransfer_080b5f0c(const void *packet, s32 size, u32 frames) {
    if (Func_080063bc(packet, size) == -1) {
        return 0;
    }
    Func_08006458();
    Func_080030f8(frames);
    return 1;
}

/*
 * Send each requested character snapshot, pad the stream to at least three
 * records, then send the party record after remapping actor IDs to the
 * encoded roster slots established by the character packets.
 */
void Func_080b5f0c(void) {
    struct CharacterTransfer_080b5f0c *character_packet =
        Func_08004970(0x154);
    struct BattleRosterMap_080b5f0c *battle = Data_03001e74;
    u16 actor_ids[8];
    s32 actor_count;
    s32 sent_count = 0;
    s32 index;

    for (index = 0; index < 8; index++) {
        battle->encoded_slot_by_actor[index] = 0xff;
    }

    actor_count = Func_080b6a60(actor_ids);
    while (sent_count < actor_count) {
        u16 actor = actor_ids[sent_count];

        WordCopyAt_03001388_080b5f0c(
            character_packet,
            Func_08077008(actor),
            0x154);
        character_packet->transfer_kind = 2;
        battle->encoded_slot_by_actor[actor] = (u8)(sent_count - 0x80);

        if (!SendTransfer_080b5f0c(character_packet, 0x154, 2)) {
            break;
        }
        sent_count++;
    }

    while (sent_count <= 2) {
        character_packet->transfer_kind = 0;
        if (!SendTransfer_080b5f0c(character_packet, 0x154, 2)) {
            break;
        }
        sent_count++;
    }
    Func_08002df0(character_packet);

    {
        struct PartyTransfer_080b5f0c *party_packet = Func_08004970(0x140);

        WordCopyAt_03001388_080b5f0c(
            party_packet,
            Func_08077000(0),
            0x140);
        for (index = 0; index < party_packet->member_count; index++) {
            u8 actor = party_packet->members[index].actor_id;
            party_packet->members[index].actor_id =
                battle->encoded_slot_by_actor[actor];
        }

        if (Func_080063bc(party_packet, 0x140) != -1) {
            Func_08006458();
            Func_080030f8(1);
            Func_080030f8(2);
        }
        Func_08002df0(party_packet);
    }
}
