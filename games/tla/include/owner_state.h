#ifndef ALCHEMY_OWNER_STATE_H
#define ALCHEMY_OWNER_STATE_H

#include "types.h"
#include "battle_unit.h"
#include "layout_guard.h"

struct OwnerInventoryState {
  u8 unk000[0xd8];
  u16 inventory[15];
  u8 unk0f6[0x54];
  u16 classId;
};

struct OwnerActionState {
  u8 unk000[0x58];
  struct OwnerActionSlot actionSlots[32];
};

union OwnerDjinnFlags {
  struct {
    u32 owned[4];
    u32 active[4];
  } banks;
  u32 slots[8];
};

union OwnerDjinnCounts {
  struct {
    u8 owned[4];
    u8 active[4];
  } banks;
  u8 slots[8];
};

struct OwnerDjinnState {
  u8 unk000[0xf8];
  union OwnerDjinnFlags flags;
  union OwnerDjinnCounts counts;
};

enum { TRADE_OFFER_PENDING = -1 };

struct TradeOffer {
  u8 element;
  u8 djinn;
  u8 owner;
  s8 status;
};

struct TradeOfferState {
  u8 unk00[8];
  struct TradeOffer offers[72];
  u32 count;
};

struct ClassDefinition {
  u8 unk00[0x50];
  u8 weakEffects[4];
};

extern struct ClassDefinition gClass_DefinitionTable[];

LAYOUT_SIZE_GUARD(ClassDefinition_Size, struct ClassDefinition, 0x54);
LAYOUT_OFFSET_GUARD(OwnerDjinn_Owned, struct OwnerDjinnState, flags.banks.owned,
                    0xf8);
LAYOUT_OFFSET_GUARD(OwnerDjinn_Active, struct OwnerDjinnState,
                    flags.banks.active, 0x108);
LAYOUT_OFFSET_GUARD(OwnerDjinn_OwnedCounts, struct OwnerDjinnState,
                    counts.banks.owned, 0x118);
LAYOUT_OFFSET_GUARD(OwnerDjinn_ActiveCounts, struct OwnerDjinnState,
                    counts.banks.active, 0x11c);
LAYOUT_SIZE_GUARD(TradeOffer_Size, struct TradeOffer, 4);
LAYOUT_OFFSET_GUARD(TradeOfferState_Offers, struct TradeOfferState, offers, 8);
LAYOUT_OFFSET_GUARD(TradeOfferState_Count, struct TradeOfferState, count,
                    0x128);

void *UnitGet(s32 owner);
void Owner_RefreshDerivedData(s32 owner);
struct ClassDefinition *Owner_GetRecordStride84(s32 classId);
s32 Owner_SumDjinnCounts(s32 owner, s32 element);

const u16 *Djinn_GetDefinition(u32 element, u32 djinn);
u16 Djinn_GetDefinitionHeader(u32 element, u32 djinn);
s32 Djinn_AddToOwner(s32 owner, s32 element, s32 djinn);
s32 Djinn_AddToLeastLoadedOwner(s32 element, s32 djinn);
u32 Djinn_IsActive(s32 owner, s32 element, s32 djinn);
s32 Djinn_Activate(s32 owner, s32 element, s32 djinn);
u32 Djinn_Deactivate(s32 owner, s32 element, s32 djinn);
s32 Djinn_Transfer(s32 source, s32 element, s32 djinn, s32 target);

struct TradeOfferState *Trade_GetOfferState(s32 which);
u32 *Trade_AddOffer(u32 owner, u32 element, u32 djinn);
s32 Trade_RemoveOffer(s32 owner, s32 element, s32 djinn);
s32 Trade_CountPendingOffers(u8 *counts);

#endif
