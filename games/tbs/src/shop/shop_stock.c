// The Game Ticket counter, the gift items and every shop's stock. Part of shop.c.

#include "shop_internal.h"

// The shop credit each Game Ticket costs, by how many have been given: five at each price: 300,
// then 500 doubling to 8000. The gift count stops at 28 (gmPartyAdjustShopGifts), so every ticket
// from the 29th costs entry 28's 8000 and entry 29 is never read.
// clang-format off
static const Sint16 GameTicketCreditCosts[30] = {
	300, 300, 300, 300, 300,
	500, 500, 500, 500, 500,
	1000, 1000, 1000, 1000, 1000,
	2000, 2000, 2000, 2000, 2000,
	4000, 4000, 4000, 4000, 4000,
	8000, 8000, 8000, 8000, 8000,
};

// Unused: a list of items ended by 0, which nothing reads: the Long Sword to the Muramasa, the Short Sword, Mint, the Demon Axe, Asura's Armor, the Cotton Shirt, the Travel Vest, Herb, Nut and the id after the Water of Life.
const Sint16 ShopUnusedItemIds[21] = {
	ITEM_LONG_SWORD, ITEM_BROAD_SWORD, ITEM_CLAYMORE, ITEM_GREAT_SWORD, ITEM_SHAMSHIR,
	ITEM_SILVER_BLADE, ITEM_MASAMUNE, ITEM_ARCTIC_BLADE, ITEM_GAIA_BLADE,
	ITEM_SOL_BLADE, ITEM_MURAMASA, ITEM_SHORT_SWORD, ITEM_MINT, ITEM_DEMON_AXE,
	ITEM_ASURAS_ARMOR, ITEM_COTTON_SHIRT, ITEM_TRAVEL_VEST, ITEM_HERB, ITEM_NUT,
	ITEM_WATER_OF_LIFE + 1, ITEM_NONE,
};
// clang-format on

// Gives a Game Ticket once the party's spending in shops reaches the next ticket's cost, and lets
// the player choose who takes it.
void mnShopGiveGameTicket(WINDOW *window, WINDOW *itemWindow)
{
	SHOP_STATE *shop = MenuState;
	Sint32      creditCost;
	Uint32      savedSelectedItem;
	Sint32      redraw;
	Sint32      unitId;
	Sint32      memberIndex;
	Sint32      msgBase;
	Sint32      slot;

	redraw            = 1;
	unitId            = 0;
	savedSelectedItem = shop->selectedItem;
	creditCost        = GameTicketCreditCosts[MainRoutineStateWork.holdings.shopGifts];
	memberIndex       = 0;

	// A ticket is paid for by the coins already spent, which it uses up; the party's coins are left
	// alone.
	if (creditCost > MainRoutineStateWork.holdings.shopCredit)
		return;
	shop->selectedItem = ITEM_GAME_TICKET;
	mnMessageArgumentPushStub(ITEM_GAME_TICKET, MESSAGE_ARGUMENT_ITEM);
	msgBase = MsgTokenGift;
	// "Let me give you this Game Ticket as a token of my gratitude."
	mnShopShowKeeperMessageHidingCursor(msgBase);
	mnMessageArgumentPushStub(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
	// "So who gets this Game Ticket?"
	mnShopShowKeeperMessageHidingCursor(msgBase + 1);

	while (1)
	{
		if (redraw != 0)
		{
			redraw      = 0;
			memberIndex = (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
			unitId      = shop->partyMemberIds[memberIndex];
			mnCursorPlace(window, memberIndex * 24 - 12, 0);
			shop->cursorFrame = 3;
			mnShopDrawParty(window, memberIndex, shop->selectedItem);
			mnShopDrawCarriedItem(itemWindow, unitId, shop->selectedItem);
		}
		if ((PerPush & KEY_A) != 0)
		{
			// The ticket goes into the pack to see that it fits, and comes out again until the
			// shopkeeper has handed it over.
			slot = gmInventoryAddItemStub(unitId, shop->selectedItem);
			if (slot < 0)
			{
				sySoundPlayStub(SE_MENU_CANCEL);
				mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
				mnMessageArgumentPushStub(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
				if (gmInventoryCountStub(unitId) == INVENTORY_SLOTS)
					// "Your pack is full, (the member). Who will you give this to?"
					mnShopShowKeeperMessage(MsgPackFull);
				else
					// "You can't carry any more of this item, (the member), so who gets it?"
					mnShopShowKeeperMessage(MsgCannotCarryItem);
				continue;
			}
			gmInventoryRemoveStub(unitId, slot);
			sySoundPlayStub(SE_SHOP_PURCHASE);
			// "Then here you go!"
			mnShopShowKeeperMessageHidingCursor(MsgHereYouGo);
			gmInventoryAddItemStub(unitId, shop->selectedItem);
			gmPartyAdjustShopCreditStub(-creditCost);
			gmPartyAdjustShopGiftsStub(1);
			break;
		}
		if ((PerPush & KEY_B) != 0)
		{
			// "What! You don't want it?"
			mnShopShowKeeperMessageHidingCursor(MsgDontWantIt);
			sySoundPlayStub(SE_MENU_CANCEL);
			break;
		}
		if ((PerRepeat & KEY_LEFT) != 0)
		{
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			memberIndex--;
			redraw = 1;
		}
		if ((PerRepeat & KEY_RIGHT) != 0)
		{
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			memberIndex++;
			redraw = 1;
		}
		sySynch(1);
	}
	shop->selectedItem = savedSelectedItem;
}

// The shops' stock, one row per shop: up to 24 items, then up to 8 artifacts (each list ending with
// 0 when shorter), then the shop's type, which an item shop, SHOP_TYPE_ITEMS, leaves at 0.
enum
{
	SHOP_STOCK_ITEMS          = 0,  // the first ordinary item's column
	SHOP_STOCK_ITEM_COUNT     = 24, // ordinary items a row holds
	SHOP_STOCK_ARTIFACTS      = 24, // the first artifact's column
	SHOP_STOCK_ARTIFACT_COUNT = 8,  // artifacts a row holds
	// The shop's type (SHOP_TYPE).
	SHOP_STOCK_TYPE           = 32,
	SHOP_STOCK_COLUMNS        = 33,
	// Rows, by the stock row a script passes to mnShopRun; SHOP_STOCK_ROW_EMPTY stands in for a row
	// out of range.
	SHOP_STOCK_ROWS           = 35
};

// clang-format off
// Every shop's stock, a row by stock row: its items, artifacts and type.
static const Sint16 StockTable[SHOP_STOCK_ROWS][SHOP_STOCK_COLUMNS] = {
	// SHOP_STOCK_ROW_EMPTY: sells nothing
	{0},
	// Vale's weapon shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_LONG_SWORD, ITEM_SHORT_SWORD, ITEM_MACE, ITEM_WOODEN_STICK,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
	},
	// Vale's armor shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_COTTON_SHIRT, ITEM_TRAVEL_VEST, ITEM_ONE_PIECE_DRESS, ITEM_WOODEN_SHIELD,
		ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
	},
	// Vale's item shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE,
	},
	// Vault's weapon shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_LONG_SWORD, ITEM_SHORT_SWORD, ITEM_BATTLE_AXE, ITEM_MACE,
		[SHOP_STOCK_ARTIFACTS] = ITEM_MAGIC_ROD,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
	},
	// Vault's armor shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_PADDED_GLOVES, ITEM_LEATHER_ARMLET,
		ITEM_LEATHER_CAP, ITEM_CIRCLET,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
	},
	// Vault's item shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE,
	},
	// Bilibin's weapon shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_LONG_SWORD, ITEM_BROAD_SWORD, ITEM_HUNTERS_SWORD, ITEM_BATTLE_AXE,
		ITEM_HEAVY_MACE,
		[SHOP_STOCK_ARTIFACTS] = ITEM_WITCHS_WAND,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
	},
	// Bilibin's armor shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_LEATHER_ARMOR, ITEM_TRAVEL_VEST, ITEM_TRAVEL_ROBE, ITEM_BRONZE_SHIELD, ITEM_LEATHER_GLOVES,
		ITEM_LEATHER_ARMLET, ITEM_OPEN_HELM, ITEM_WOODEN_CAP, ITEM_CIRCLET,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
	},
	// Bilibin's item shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR,
		ITEM_SACRED_FEATHER,
	},
	// Imil's weapon shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_BROAD_SWORD, ITEM_HUNTERS_SWORD, ITEM_BATTLE_AXE, ITEM_HEAVY_MACE,
		[SHOP_STOCK_ARTIFACTS] = ITEM_BLESSED_ANKH,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
	},
	// Imil's armor shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_LEATHER_ARMOR, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_LEATHER_GLOVES, ITEM_LEATHER_ARMLET,
		ITEM_OPEN_HELM, ITEM_WOODEN_CAP, ITEM_CIRCLET,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
	},
	// Imil's item shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR,
		ITEM_SACRED_FEATHER,
	},
	// Kolima's weapon shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_BROAD_SWORD, ITEM_HUNTERS_SWORD, ITEM_BROAD_AXE, ITEM_HEAVY_MACE,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
	},
	// Kolima's armor shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_LEATHER_ARMOR, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_LEATHER_GLOVES, ITEM_ARMLET,
		ITEM_BRONZE_HELM, ITEM_WOODEN_CAP,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
	},
	// Kolima's item shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR,
		ITEM_SACRED_FEATHER,
	},
	// Xian's weapon shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_BROAD_SWORD, ITEM_BATTLE_RAPIER, ITEM_BROAD_AXE, ITEM_BATTLE_MACE,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
	},
	// Xian's armor shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_ADEPTS_CLOTHES, ITEM_SILK_ROBE, ITEM_IRON_SHIELD, ITEM_ARMLET,
		ITEM_BRONZE_HELM, ITEM_WOODEN_CAP, ITEM_SILVER_CIRCLET,
		[SHOP_STOCK_ARTIFACTS] = ITEM_CHINA_DRESS,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
	},
	// Xian's item shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR,
		ITEM_SACRED_FEATHER,
	},
	// Altin's weapon shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_CLAYMORE, ITEM_BATTLE_RAPIER, ITEM_BROAD_AXE, ITEM_BATTLE_MACE,
		[SHOP_STOCK_ARTIFACTS] = ITEM_PSYNERGY_ROD,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
	},
	// Altin's armor shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_CHAIN_MAIL, ITEM_ADEPTS_CLOTHES, ITEM_SILK_ROBE,
		ITEM_IRON_SHIELD, ITEM_GAUNTLETS, ITEM_ARMLET, ITEM_IRON_HELM, ITEM_SILVER_CIRCLET,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
	},
	// Altin's item shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR,
		ITEM_SACRED_FEATHER,
	},
	// Kalay's weapon shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_CLAYMORE, ITEM_BATTLE_RAPIER, ITEM_GREAT_AXE, ITEM_BATTLE_MACE,
		[SHOP_STOCK_ARTIFACTS] = ITEM_FROST_WAND,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
	},
	// Kalay's armor shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_CHAIN_MAIL, ITEM_JERKIN, ITEM_IRON_SHIELD,
		ITEM_GAUNTLETS, ITEM_HEAVY_ARMLET, ITEM_IRON_HELM, ITEM_MAIL_CAP, ITEM_SILVER_CIRCLET,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
	},
	// Kalay's item shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR,
		ITEM_SACRED_FEATHER,
	},
	// Lunpa's item shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR,
		ITEM_SACRED_FEATHER,
	},
	// Tolbi's weapon shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_GREAT_SWORD, ITEM_MASTER_RAPIER, ITEM_GREAT_AXE, ITEM_WAR_MACE,
		[SHOP_STOCK_ARTIFACTS] = ITEM_ANGELIC_ANKH,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
	},
	// Tolbi's armor shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_ARMORED_SHELL, ITEM_SILVER_VEST, ITEM_JERKIN, ITEM_KNIGHTS_SHIELD, ITEM_GAUNTLETS,
		ITEM_HEAVY_ARMLET, ITEM_STEEL_HELM, ITEM_MAIL_CAP, ITEM_GUARDIAN_CIRCLET,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
	},
	// Tolbi's item shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR,
		ITEM_SACRED_FEATHER,
		[SHOP_STOCK_ARTIFACTS] = ITEM_VIAL, ITEM_WATER_OF_LIFE,
	},
	// Lunpa's weapon shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_GREAT_SWORD, ITEM_MASTER_RAPIER, ITEM_GREAT_AXE, ITEM_WAR_MACE,
		[SHOP_STOCK_ARTIFACTS] = ITEM_SHAMSHIR, ITEM_NINJA_BLADE, ITEM_DRAGON_AXE, ITEM_DEMONIC_STAFF,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
	},
	// Lunpa's armor shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_PLATE_MAIL, ITEM_SILVER_ARMLET, ITEM_SILVER_HELM, ITEM_PLATINUM_CIRCLET,
		[SHOP_STOCK_ARTIFACTS] = ITEM_BLESSED_ROBE, ITEM_WAR_GLOVES,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
	},
	// Suhalla's shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR,
		ITEM_SACRED_FEATHER,
		[SHOP_STOCK_ARTIFACTS] = ITEM_VIAL, ITEM_POTION, ITEM_WATER_OF_LIFE,
		ITEM_PSY_CRYSTAL,
	},
	// Lalivero's weapon shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_GREAT_SWORD, ITEM_MASTER_RAPIER, ITEM_GREAT_AXE, ITEM_WAR_MACE,
		[SHOP_STOCK_ARTIFACTS] = ITEM_SILVER_BLADE, ITEM_SWIFT_SWORD, ITEM_RIGHTEOUS_MACE,
		ITEM_CRYSTAL_ROD,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
	},
	// Lalivero's armor shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_STEEL_ARMOR, ITEM_KNIGHTS_HELM, ITEM_PLATINUM_CIRCLET,
		[SHOP_STOCK_ARTIFACTS] = ITEM_MAGICAL_CASSOCK, ITEM_MIRRORED_SHIELD, ITEM_SPIRIT_ARMLET,
		ITEM_JEWELED_CROWN,
		[SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
	},
	// Lalivero's item shop
	{
		[SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR,
		ITEM_SACRED_FEATHER,
		[SHOP_STOCK_ARTIFACTS] = ITEM_VIAL, ITEM_POTION, ITEM_WATER_OF_LIFE,
		ITEM_PSY_CRYSTAL,
	},
};
// clang-format on

// Returns how many stock rows there are; mnShopRun refuses a row past them.
Sint32 mnShopStockGetRowCount(void)
{
	return SHOP_STOCK_ROWS;
}

// Adds a shop's artifacts to the shared stock on its first visit, so an item two shops list can be
// bought twice.
void mnShopStockAddArtifacts(Sint32 stockRow)
{
	const Sint16 *stockEntries;
	const Sint16 *artifactCursor;
	Sint32        itemId;
	Sint32        artifactIndex;
	Sint32        initializedFlag;

	initializedFlag = stockRow + FLAG_SHOP_ARTIFACTS_STOCKED;
	if (gmFlagGetStub(initializedFlag) == 0)
	{
		gmFlagSetStub(initializedFlag);
		artifactIndex = 0;
		itemId        = StockTable[stockRow][SHOP_STOCK_ARTIFACTS];
		if (itemId != 0)
		{
			stockEntries   = StockTable[stockRow];
			artifactCursor = stockEntries + SHOP_STOCK_ARTIFACTS;
			do
			{
				gmItemAdjustRareItemStockStub(itemId, 1);
				artifactIndex++;
				if (artifactIndex > SHOP_STOCK_ARTIFACT_COUNT - 1)
					break;
				artifactCursor++;
				itemId = *artifactCursor;
			} while (itemId != 0);
		}
	}
}

// Copies the shop's ordinary stock, up to 24 items, to `output` with a 0 after, and returns how
// many.
Sint32 mnShopStockCopyItems(Sint32 stockRow, Sint16 *output)
{
	const Sint16 *src;
	Sint16       *dest;
	Sint32        count;
	Uint32        item;

	count = 0;
	if (StockTable[stockRow][SHOP_STOCK_ITEMS] != 0)
	{
		dest = output;
		src  = StockTable[stockRow];
		do
		{
			item  = (Uint16)*src;
			*dest = item;
			count++;
			src++;
			dest++;
			if (count > SHOP_STOCK_ITEM_COUNT - 1)
				break;
		} while (*src != 0);
	}
	output[count] = 0;
	return count;
}

// Returns the shop's type from the last column of its stock row (enum SHOP_TYPE).
Sint32 mnShopStockGetType(Sint32 stockRow)
{
	return StockTable[stockRow][SHOP_STOCK_TYPE];
}
