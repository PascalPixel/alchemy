/// @file
/// The grammatical gender of each item's name, by item id, in the German and
/// Italian releases, a kind from 0 to 2 that picks the article or ending a
/// message writes with the item. The names in the
/// comments are the English release's.

#include "types.h"
#include "edition.h"

// clang-format off
#if LANGUAGE == LANGUAGE_GERMAN
const s8 gLocalizedWord_ItemGenders[] = {
    0, // ???
    2, // Long Sword
    2, // Broad Sword
    2, // Claymore
    2, // Great Sword
    2, // Shamshir
    1, // Silver Blade
    2, // Masamune
    1, // Arctic Blade
    1, // Gaia Blade
    1, // Sol Blade
    2, // Muramasa
    0, // ?
    0, // ?
    0, // ?
    1, // Machete
    2, // Short Sword
    2, // Hunter's Sword
    2, // Battle Rapier
    2, // Master Rapier
    1, // Ninja Blade
    2, // Swift Sword
    2, // Elven Rapier
    1, // Assassin Blade
    1, // Mystery Blade
    1, // Kikuichimonji
    2, // Kusanagi
    2, // Bandit's Sword
    0, // ?
    0, // ?
    0, // ?
    1, // Battle Axe
    1, // Broad Axe
    1, // Great Axe
    1, // Dragon Axe
    1, // Giant Axe
    1, // Vulcan Axe
    1, // Burning Axe
    1, // Demon Axe
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    1, // Mace
    1, // Heavy Mace
    0, // Battle Mace
    1, // War Mace
    1, // Righteous Mace
    1, // Grievous Mace
    1, // Blessed Mace
    1, // Wicked Mace
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Wooden Stick
    0, // Magic Rod
    0, // Witch's Wand
    0, // Blessed Ankh
    0, // Psynergy Rod
    0, // Frost Wand
    0, // Angelic Ankh
    0, // Demonic Staff
    0, // Crystal Rod
    0, // Zodiac Wand
    0, // Shaman's Rod
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    1, // Leather Armor
    1, // Psynergy Armor
    0, // Chain Mail
    1, // Armored Shell
    0, // Plate Mail
    1, // Steel Armor
    1, // Spirit Armor
    1, // Dragon Scales
    0, // Demon Mail
    1, // Asura's Armor
    1, // Spiked Armor
    0, // ?
    0, // ?
    0, // ?
    2, // Cotton Shirt
    1, // Travel Vest
    0, // Fur Coat
    1, // Adept's Clothes
    2, // Elven Shirt
    1, // Silver Vest
    1, // Water Jacket
    1, // Storm Gear
    0, // Kimono
    2, // Ninja Garb
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    2, // One-Piece Dress
    2, // Travel Robe
    1, // Silk Robe
    2, // China Dress
    2, // Jerkin
    2, // Cocktail Dress
    1, // Blessed Robe
    0, // Magical Cassock
    1, // Mysterious Robe
    1, // Feathered Robe
    1, // Oracle's Robe
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Wooden Shield
    0, // Bronze Shield
    0, // Iron Shield
    0, // Knight's Shield
    0, // Mirrored Shield
    0, // Dragon Shield
    0, // Earth Shield
    0, // ?
    0, // ?
    1, // Padded Gloves
    1, // Leather Gloves
    1, // Gauntlets
    0, // Vambrace
    1, // War Gloves
    1, // Spirit Gloves
    1, // Battle Gloves
    1, // Aura Gloves
    0, // ?
    0, // Leather Armlet
    0, // Armlet
    0, // Heavy Armlet
    0, // Silver Armlet
    0, // Spirit Armlet
    0, // Virtuous Armlet
    0, // Guardian Armlet
    0, // ?
    0, // ?
    0, // Open Helm
    0, // Bronze Helm
    0, // Iron Helm
    2, // Steel Helm
    0, // Silver Helm
    0, // Knight's Helm
    0, // Warrior's Helm
    0, // Adept's Helm
    0, // ?
    0, // ?
    0, // ?
    1, // Leather Cap
    1, // Wooden Cap
    1, // Mail Cap
    1, // Jeweled Crown
    1, // Ninja Hood
    1, // Lucky Cap
    1, // Thunder Crown
    0, // Prophet's Hat
    1, // Lure Cap
    0, // ?
    0, // Circlet
    0, // Silver Circlet
    0, // Guardian Circlet
    0, // Platinum Circlet
    0, // Mythril Circlet
    1, // Glittering Tiara
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    2, // Herb
    1, // Nut
    0, // Vial
    0, // Potion
    2, // Hermes' Water
    1, // Empty Bottle
    0, // Psy Crystal
    2, // Antidote
    2, // Elixir
    2, // Water of Life
    0, // ?
    2, // Power Bread
    2, // Cookie
    0, // Apple
    1, // Hard Nut
    1, // Mint
    0, // Lucky Pepper
    0, // ?
    0, // ?
    0, // ?
    0, // Orb of Force
    0, // Douse Drop
    0, // Frost Jewel
    0, // Lifting Gem
    2, // Halt Gem
    0, // Cloak Ball
    0, // Carry Stone
    1, // Catch Beads
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Venus Star
    0, // Mercury Star
    0, // Mars Star
    0, // Jupiter Star
    0, // Mythril Bag
    2, // Small Jewel
    1, // Smoke Bomb
    1, // Sleep Bomb
    2, // Game Ticket
    1, // Lucky Medal
    2, // Dragon's Eye
    0, // Bone
    0, // Anchor Charm
    0, // Corn
    0, // Cell Key
    2, // Boat Ticket
    1, // Sacred Feather
    0, // Mystic Draught
    0, // Oil Drop
    1, // Weasel's Claw
    0, // Bramble Seed
    0, // Crystal Powder
    0, // Black Orb
    0, // Red Key
    0, // Blue Key
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    2, // Mythril Shirt
    2, // Silk Shirt
    2, // Running Shirt
    0, // ?
    0, // ?
    0, // ?
    1, // Hyper Boots
    1, // Quick Boots
    1, // Fur Boots
    1, // Turtle Boots
    0, // ?
    0, // ?
    0, // Adept Ring
    0, // War Ring
    0, // Sleep Ring
    0, // Healing Ring
    0, // Unicorn Ring
    0, // Fairy Ring
    0, // Cleric's Ring
};
#elif LANGUAGE == LANGUAGE_ITALIAN
const s8 gLocalizedWord_ItemGenders[] = {
    0, // ???
    1, // Long Sword
    1, // Broad Sword
    1, // Claymore
    1, // Great Sword
    1, // Shamshir
    1, // Silver Blade
    1, // Masamune
    1, // Arctic Blade
    1, // Gaia Blade
    1, // Sol Blade
    1, // Muramasa
    0, // ?
    0, // ?
    0, // ?
    0, // Machete
    1, // Short Sword
    1, // Hunter's Sword
    0, // Battle Rapier
    0, // Master Rapier
    1, // Ninja Blade
    1, // Swift Sword
    0, // Elven Rapier
    1, // Assassin Blade
    1, // Mystery Blade
    1, // Kikuichimonji
    1, // Kusanagi
    1, // Bandit's Sword
    0, // ?
    0, // ?
    0, // ?
    1, // Battle Axe
    1, // Broad Axe
    1, // Great Axe
    1, // Dragon Axe
    1, // Giant Axe
    1, // Vulcan Axe
    1, // Burning Axe
    1, // Demon Axe
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    1, // Mace
    1, // Heavy Mace
    1, // Battle Mace
    1, // War Mace
    1, // Righteous Mace
    1, // Grievous Mace
    1, // Blessed Mace
    1, // Wicked Mace
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Wooden Stick
    0, // Magic Rod
    0, // Witch's Wand
    0, // Blessed Ankh
    1, // Psynergy Rod
    1, // Frost Wand
    0, // Angelic Ankh
    0, // Demonic Staff
    1, // Crystal Rod
    0, // Zodiac Wand
    0, // Shaman's Rod
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    1, // Leather Armor
    1, // Psynergy Armor
    1, // Chain Mail
    1, // Armored Shell
    1, // Plate Mail
    1, // Steel Armor
    1, // Spirit Armor
    1, // Dragon Scales
    1, // Demon Mail
    1, // Asura's Armor
    1, // Spiked Armor
    0, // ?
    0, // ?
    0, // ?
    1, // Cotton Shirt
    1, // Travel Vest
    0, // Fur Coat
    1, // Adept's Clothes
    1, // Elven Shirt
    1, // Silver Vest
    1, // Water Jacket
    1, // Storm Gear
    0, // Kimono
    0, // Ninja Garb
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // One-Piece Dress
    0, // Travel Robe
    0, // Silk Robe
    0, // China Dress
    0, // Jerkin
    0, // Cocktail Dress
    0, // Blessed Robe
    1, // Magical Cassock
    0, // Mysterious Robe
    0, // Feathered Robe
    0, // Oracle's Robe
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Wooden Shield
    0, // Bronze Shield
    0, // Iron Shield
    0, // Knight's Shield
    0, // Mirrored Shield
    0, // Dragon Shield
    0, // Earth Shield
    0, // ?
    0, // ?
    0, // Padded Gloves
    0, // Leather Gloves
    0, // Gauntlets
    0, // Vambrace
    0, // War Gloves
    0, // Spirit Gloves
    0, // Battle Gloves
    0, // Aura Gloves
    0, // ?
    0, // Leather Armlet
    0, // Armlet
    0, // Heavy Armlet
    0, // Silver Armlet
    0, // Spirit Armlet
    0, // Virtuous Armlet
    0, // Guardian Armlet
    0, // ?
    0, // ?
    0, // Open Helm
    0, // Bronze Helm
    0, // Iron Helm
    0, // Steel Helm
    0, // Silver Helm
    0, // Knight's Helm
    0, // Warrior's Helm
    0, // Adept's Helm
    0, // ?
    0, // ?
    0, // ?
    0, // Leather Cap
    0, // Wooden Cap
    0, // Mail Cap
    1, // Jeweled Crown
    0, // Ninja Hood
    0, // Lucky Cap
    1, // Thunder Crown
    0, // Prophet's Hat
    0, // Lure Cap
    0, // ?
    1, // Circlet
    1, // Silver Circlet
    1, // Guardian Circlet
    1, // Platinum Circlet
    1, // Mythril Circlet
    1, // Glittering Tiara
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    1, // Herb
    1, // Nut
    1, // Vial
    1, // Potion
    1, // Hermes' Water
    1, // Empty Bottle
    1, // Psy Crystal
    0, // Antidote
    0, // Elixir
    1, // Water of Life
    0, // ?
    0, // Power Bread
    0, // Cookie
    1, // Apple
    1, // Hard Nut
    1, // Mint
    0, // Lucky Pepper
    0, // ?
    0, // ?
    0, // ?
    1, // Orb of Force
    1, // Douse Drop
    1, // Frost Jewel
    1, // Lifting Gem
    1, // Halt Gem
    1, // Cloak Ball
    1, // Carry Stone
    1, // Catch Beads
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Venus Star
    0, // Mercury Star
    0, // Mars Star
    0, // Jupiter Star
    1, // Mythril Bag
    0, // Small Jewel
    1, // Smoke Bomb
    1, // Sleep Bomb
    0, // Game Ticket
    1, // Lucky Medal
    0, // Dragon's Eye
    0, // Bone
    1, // Anchor Charm
    0, // Corn
    1, // Cell Key
    0, // Boat Ticket
    1, // Sacred Feather
    0, // Mystic Draught
    1, // Oil Drop
    0, // Weasel's Claw
    0, // Bramble Seed
    1, // Crystal Powder
    1, // Black Orb
    1, // Red Key
    1, // Blue Key
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    1, // Mythril Shirt
    1, // Silk Shirt
    1, // Running Shirt
    0, // ?
    0, // ?
    0, // ?
    0, // Hyper Boots
    0, // Quick Boots
    0, // Fur Boots
    0, // Turtle Boots
    0, // ?
    0, // ?
    0, // Adept Ring
    0, // War Ring
    0, // Sleep Ring
    0, // Healing Ring
    0, // Unicorn Ring
    0, // Fairy Ring
    0, // Cleric's Ring
};
#endif
// clang-format on
