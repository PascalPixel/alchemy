/// @file
/// Party members, items, Djinn and game-state operations.

/// Returns the party's Djinn recovery bank, the standby Djinn with their turns
/// to recover and the summon flags, or an enemy record read as one for the
/// enemy side.
void *Djinn_GetRecoveryTableFar(s32 arg0);
#ifndef OWN_Unit_GetFar
/// Returns the unit record of a playable character or of an enemy in the
/// current battle, or NULL for anything else; nearly every bank reads units
/// this way.
struct BattleUnit *Unit_GetFar(u32 owner);
#endif
/// Works a unit's stats out again from its base stats, equipment, set Djinn,
/// class and battle stat levels, keeping its HP and PP at the same fraction.
void Unit_CalculateFar(s32 owner);
/// Returns an item's definition; an inventory entry may be passed, and its
/// equipped and quantity bits are ignored.
struct ItemDefinition *Item_GetDirectFar(s32 item);
/// Returns how many of the item in `slot` the unit carries, 0 for an empty
/// slot (Inventory_GetQuantity); the sell screen reads a stack's size with it.
s32 Shop_GetSelectionState(s32 owner, s32 slot);
/// Gives a unit one item, on its first matching stack or in its first free
/// slot, and returns the slot, or -1 when there was no room.
s32 Inventory_AddItemFar(s32 owner_id, s32 item_id);
/// Gives an item to the first party member with room and returns who took it,
/// or -1; spoils, chests and the scenes' gifts land this way.
s32 PartyInventory_AddFar(s32 item_id);
/// Returns the first slot holding the item, equipped or not, or -1.
s32 Inventory_FindFar(s32 owner, s32 item_id);
/// Returns who carries the item, asking the selected character first, or -1;
/// scenes use it to find a key item.
s32 PartyInventory_FindOwnerFar(s32 item_id);
/// Takes one of the item from whoever carries it first, as a scene uses up a
/// key item; always returns 0.
s32 PartyInventory_RemoveFar(s32 item_id);
/// Equips the item in `slot` in place of the one of its type, then rebuilds
/// the unit's Psynergy and stats. Returns 0, -1 when the unit can't equip it,
/// or -2 when the item it would replace is cursed on.
s32 Inventory_EquipFar(s32 owner, s32 slot);
/// Takes one item out of `slot` and recomputes the unit's stats: returns 1 when
/// a stack shrank, 2 when the slot emptied, -1 for an empty slot.
s32 Inventory_RemoveFar(s32 owner, s32 slot);
/// Marks the item in `slot` broken, as when a used item breaks; an empty slot
/// gives -1.
s32 Inventory_BreakFar(s32 owner, s32 slot);
/// Clears the broken mark of the item in `slot` for the shop's repair, or
/// returns -1 when the slot is empty.
s32 Inventory_RepairFar(s32 owner, s32 slot);
/// Returns the id of the unit's equipped item of `type`, or 0; the battle draws
/// a member's weapon from it.
s32 Inventory_GetEquippedItemFar(struct BattleUnit *inv, s32 type);
/// Returns the definition of an ability by its low 14 bits, the first for an id
/// past the table (BattleAction_GetDirect); every bank reads abilities so.
struct BattleAct *BattleAction_Get(s32 actionId);
/// Teaches a character an action for good and rebuilds its list, returning the
/// action's place there, or -1 when the list is full.
s32 Unit_LearnPsynergyFar();
/// Returns 1 when the action is in the unit's action list; events check a
/// member's Psynergy with it.
s32 Unit_KnowsActionFar(s32 owner, s32 value);
/// Clears the game state and sets a new game's defaults: Isaac alone in the
/// party, the standard buttons, the message speed and the play time clock.
void GameState_InitializeDefaultsFar(void);
/// Levels a character up once if its experience reaches the next level, and
/// returns `value` filled with the gains, or NULL; the spoils call it per level
/// reached.
struct LevelUpResult *Unit_TryLevelUpFar(s32 owner_id,
                                         struct LevelUpResult *value);
/// Returns 1 when the flag is set; the whole game tests flags here, the
/// scenes through the event layer.
s32 GameFlag_TestFar(s32 flag);
/// Sets the flag and returns the number of the byte it's in.
s32 GameFlag_SetBitFar(s32 flag);
/// Clears the flag, returning nothing where GameFlag_SetBitFar returns its
/// byte.
void GameFlag_ClearBitFar(s32 flag);
/// Returns the whole byte the flag is in, a counter kept among the flags, as
/// the encounter code and the Colosso and Battle Arena scenes use.
s32 GameFlag_GetByteFar(s32 flag);
/// Writes the whole byte the flag is in, such as the Battle Arena's win count.
void GameFlag_SetByteFar(s32 flag, s32 value);
/// Adds `delta` to the unit's HP, kept between 0 and its maximum, updates its
/// gauges and returns the new HP.
s32 Unit_AddHpFar(s32 owner, s32 delta);
/// Adds `delta` to the unit's PP, kept between 0 and its maximum, updates its
/// gauges and returns the new PP.
s16 Unit_AddPpFar(s32 owner, s32 delta);
/// Recomputes the unit's HP and PP gauges from its current HP and PP, after
/// something writes them directly, so Unit_Calculate keeps the same fractions.
void Unit_UpdateRatiosFar(s32 owner_id);
/// Sets the unit's HP, kept between 0 and its maximum, and updates its gauges.
void Unit_SetHpFar(struct BattleUnit *unit, s32 hp);
/// Fills an enemy battle unit from an enemy's definition: stats, name with its
/// suffix letter, items, rewards and elements. Returns 0 for an id out of
/// range, 1 otherwise.
s32 BattleUnit_SetFar(s32 unitId, s32 enemyId, s32 suffix);
/// Returns the size of the party, counted from its FLAG_IN_PARTY flags.
s32 Party_CountActiveOwnersFar(void);
/// Adds a character to the end of the party when one joins, setting its flag,
/// and returns the party size.
s32 Party_AddActiveOwnerFar(s32 value);
/// Writes the party members to `owners`, ended by BATTLE_UNIT_LIST_END, and
/// returns how many there are; every screen that lists the party starts here.
s32 Party_ListActiveOwnersFar(s16 *owners);
/// Rolls whether a character's weapon unleashes on an Attack, returning its
/// unleash action or 1 for a plain attack.
s32 RollWeaponUnleashFar(struct BattleUnit *owner);
/// Takes a character out of the party when one leaves, clearing its flag and
/// closing the gap, and returns the party size.
s32 Party_RemoveActiveOwnerFar(s32 value);
/// Returns the element the unit attacks with: an enemy's from its kind, a
/// character's from its weapon, ELEMENT_NONE for none.
s32 Item_GetEquippedElementFar(s32 id);
/// Rolls whether an ability's effect takes hold on a target, from the base
/// rate, the element levels, luck and weakness, scaled by `success_scale`
/// percent (Battle_RollEffectSuccess).
s32 Battle_RollEffectSuccessFar(s32 caster, s32 target, s32 resistance_category,
                                s32 effect_id, s32 success_scale);
/// Returns an attack's damage: attack less defense, plus twice the added power,
/// scaled by the element difference and halved (Battle_CalculateAttackDamage).
s32 Battle_CalculateAttackDamageFar(s32 atk, s32 def, s32 add, s32 bonus);
/// Returns the power of Psynergy damage, scaled by the element difference and
/// a multiplier where 256 is 1x (Battle_CalculatePsynergyPower).
s32 Battle_CalculatePsynergyPowerFar(s32 power, s32 bonus, s32 rate);
/// Returns the HP healed, scaled by the healer's power in the element and a
/// multiplier where 256 is 1x (Battle_CalculateRecovery); battle and the menu
/// heal with it.
s32 Battle_CalculateRecoveryFar(s32 power, s32 element, s32 rate);
/// Returns an enemy's definition, the first row for an id past the table.
const u8 *Enemy_GetDefinitionFar(s32 selector);
/// Returns the battle's next random number, from its own seed, which link
/// battles exchange before resolving a round.
s32 Battle_RandomFar(void);
/// Gives a unit a Djinni, returning -1 when it has it already or holds ten of
/// that element.
s32 Djinn_AddToOwnerFar(s32 owner, s32 index, s32 bit);
/// Sets a ready Djinni on the unit and works out its class again; returns 1
/// when it was set.
s32 Djinn_ActivateFar(s32 owner, s32 index, s32 bit);
/// Takes a set Djinni off the unit and works out its class again; returns 1
/// when it had been set.
u32 Djinn_DeactivateFar(s32 owner, s32 index, s32 bit);
/// Takes a Djinni off its side's standby and recovery list; returns 1 when it
/// was there.
s32 DjinnRecovery_RemoveFar(s32 owner, s32 index, s32 bit);
/// Puts a Djinni at the end of its side's list on standby and returns a pointer
/// to the list's count.
u32 *DjinnRecovery_AddStandbyFar(u32 kind, u32 first, u32 second);
/// Moves a Djinni from one unit to another, set or on standby as it was, for
/// the Djinn menu's Give and Trade; returns 0, or -1 when it can't.
s32 Djinn_TransferFar(s32 source, s32 index, s32 bit, s32 target);
/// Writes the summons the party's Djinn allow to `output` in menu order, ended
/// by SUMMON_END, and returns how many; `output` needs room for 17 bytes.
s32 Summon_ListLearnedFar(u8 *output);
/// Returns a summon's definition, or NULL past the sixteenth
/// (SummonDefinition_GetNear).
const struct SummonDefinition *SummonDefinition_Get(u32 summon_id);
/// Returns the ability a Djinni uses when it's unleashed.
s32 Djinn_GetAbilityIdFar(u32 group, u32 index);
/// Raises a character a level at a time up to `lv`, then recomputes its stats.
void Unit_LevelUpToFar(s32 unit, s32 lv);
/// Returns the unit's whole element level in one element, 0 for ELEMENT_NONE;
/// the status screens show it.
s32 Unit_GetElementLevelFar(s32 owner, s32 index);
/// Returns 1 when the Djinni is set on the unit.
u32 Djinn_IsActiveFar(s32 owner, s32 index, s32 bit);
/// Returns 1 when the unit holds the Djinni and it can be set: not set already,
/// not recovering and not held by a summon.
s32 Djinn_IsReadyFar(s32 owner, s32 index, s32 bit);
/// Returns whether the character's class may equip the item, 0 for any unit
/// past the eight characters (Item_CanOwnerEquipDirect).
s32 Item_CanOwnerEquip(s32 unit, s32 item);
/// Returns 1 for anything but a weapon or armor, and for those whether the
/// character can equip it; the buy screen greys out members with it.
s32 Item_IsCompatibleWithOwnerFar(s32 unit, s32 item);
/// Returns the slot of the unit's equipped item of `type`, or -1.
s32 Inventory_FindEquippedFar(s32 owner, s32 type);
/// Adds `amount` to the party's coins, kept between 0 and 999999, and returns
/// the new total.
s32 Party_AdjustCoinsFar(s32 amount);
/// Returns which enum EquipmentGroup an item belongs to, so a shop knows what
/// comparison to show.
s32 Item_GetEquipmentGroupFar(s32 item_id);
/// Adds `delta` to a rare item's counter, how many of it the shops may still
/// sell, and returns the new count, or 0 for an ordinary item.
s32 Item_AdjustCounterFar(s32 item_id, s32 delta);
/// Returns how many items the unit carries, its filled slots up to the first
/// empty one.
s32 Inventory_CountFar(s32 owner);
/// Returns 1 when the selected character or any party member has a free slot.
s32 PartyInventory_HasSpaceFar(void);
/// Returns the experience a character needs for `level`, or -1 for an enemy or
/// a level above 99.
u32 Unit_GetExperienceForLevelFar(s32 owner, s32 level);
/// Restores every party member's HP and PP; with 1 it also cures poison and
/// haunting.
void Party_RestoreHpPpFar(s32 arg0);
/// Restores the PP of each member whose element's lighthouse is lit, at the top
/// of the Mercury and Venus Lighthouses.
void Party_RestorePpOfLitElementsFar(void);
/// Sets FLAG_CHILDHOOD and recomputes Isaac, Garet and Jenna in their childhood
/// classes, as a new game starts the prologue.
void Party_StartChildhoodFar(void);
/// Sets the party up for Vale three years after the prologue: the Short Sword,
/// Move, Retreat and Mind Read taught, and 300 coins.
void Party_EndChildhoodFar(void);
/// Gives a newly found Djinni to the member holding the fewest Djinn with room
/// in its element and returns who got it: -1 when it was found before, -2 when
/// nobody has room.
s32 Djinn_AddToLeastLoadedOwnerFar(s32 element, s32 djinn);
/// Returns how many Djinn of an element the party holds, or of all four for -1.
s32 Party_SumDjinnCountsFar(s32 index);
/// Adds `amount` to the shop credit purchases build up and Game Tickets spend,
/// kept between 0 and 999999, and returns the new credit.
s32 Party_AdjustShopCreditFar(s32 amount);
/// Adds `amount` to the count of Game Tickets given, which stops at 28 and
/// picks the next ticket's credit cost, and returns the new count.
s32 Party_AdjustShopGiftsFar(s32 amount);
/// Returns 0 when the item in `slot` may be dropped, -1 for an empty slot, -3
/// for one equipped and cursed on, or -4 for an indispensable item.
s32 Inventory_CheckDiscardFar(s32 owner, s32 slot);
/// Removes one item as Inventory_Remove does and puts a rare item back where
/// the shops can sell it again.
s32 Inventory_DiscardFar(s32 owner, s32 slot);
/// Returns 1 for Revive, Quartz and Dew, which only work on a downed unit.
s32 Battle_IsReviveEffectFar(s32 effect_id);
/// Rebuilds a character's Psynergy list from its class, its level and the items
/// it has equipped; enemies are left alone.
s32 Unit_RefreshPsynergyFar(s32 owner);
/// Returns the party's average level, rounded down; random battles and the
/// Lucky Dice prizes follow it.
u32 Party_GetAverageLevelFar(void);
/// Counts the unit's matching unstackable items, or the quantity in its first
/// matching stack.
s32 Inventory_CountItemFar(s32 owner, s32 item_id);
/// Counts the item across the whole party, as the slot machine counts Game
/// Tickets.
s32 PartyInventory_CountItemFar(s32 item_id);
/// Discards one of the item from whoever carries it first, as a scene takes
/// it back; always returns 0.
s32 PartyInventory_DiscardFar(s32 item_id);
/// Sets FLAG_LURE_CAP while any member wears the Lure Cap and clears it
/// otherwise, so the encounter rate follows the cap.
void GameFlag_RefreshLureCapFar(void);
/// Returns the unleash chance, in percent, that a unit's equipment adds.
s32 Equipment_GetUnleashRateBonusFar(struct BattleUnit *owner);
/// Returns the build stamp's time packed into a halfword, which a new game
/// keeps as its start stamp and the save screens compare.
u32 Runtime_GetBuildStampTimeFar();
/// Returns how many inventory slots are free across the party, which caps the
/// prizes of the Tolbi spring game and the lucky wheels.
s32 PartyInventory_CountFreeSlotsFar(void);
/// Puts aside the party's inventories as Isaac enters the Colosso, leaving
/// only Psynergy items and the Short Sword, and restores HP and PP.
void InventorySnapshot_SaveFar(void);
/// Gives back the inventories InventorySnapshot_Save put aside and clears
/// FLAG_COLOSSO_HOLDS_EQUIPMENT.
void InventorySnapshot_RestoreFar(void);
