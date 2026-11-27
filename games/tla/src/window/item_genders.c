// The grammatical gender of each item's name in the German and Italian releases, which picks the
// possessive ("your") a message writes before the item (LocalizedWordItemPossessives): "dein" or
// "deine" in German, "Il tuo" or "la tua" in Italian. The names in the comments are the English
// release's.

#include "types.h"
#include "edition.h"

// The grammatical gender of an item's name.
typedef enum ITEM_GENDER
{
    ITEM_MASCULINE = 0, // "dein", "Il tuo"
    ITEM_FEMININE  = 1, // "deine", "la tua"
    ITEM_NEUTER    = 2  // "dein"; "Il tuo" in Italian
} ITEM_GENDER;

// clang-format off
#if LANGUAGE == LANGUAGE_GERMAN
// Each item's gender, by item id. BUG: the rows start at the Long Sword, not at item 0, so each item takes the gender of the one after it ("dein Silberklinge", for the neuter Fire Brand); the last row is for no item. BUGFIX adds item 0's row.
const Sint8 LocalizedWordItemGenders[] = {
#if FIX_GERMAN_ITEM_GENDER_OFF_BY_ONE
    ITEM_MASCULINE, // ???
#endif
    ITEM_NEUTER, // Long Sword
    ITEM_NEUTER, // Broad Sword
    ITEM_NEUTER, // Claymore
    ITEM_NEUTER, // Great Sword
    ITEM_NEUTER, // Shamshir
    ITEM_FEMININE, // Silver Blade
    ITEM_NEUTER, // Fire Brand
    ITEM_FEMININE, // Arctic Blade
    ITEM_FEMININE, // Gaia Blade
    ITEM_FEMININE, // Sol Blade
    ITEM_NEUTER, // Muramasa
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Machete
    ITEM_NEUTER, // Short Sword
    ITEM_NEUTER, // Hunter's Sword
    ITEM_NEUTER, // Battle Rapier
    ITEM_NEUTER, // Master Rapier
    ITEM_FEMININE, // Ninja Blade
    ITEM_NEUTER, // Swift Sword
    ITEM_NEUTER, // Elven Rapier
    ITEM_FEMININE, // Assassin Blade
    ITEM_FEMININE, // Mystery Blade
    ITEM_FEMININE, // Kikuichimonji
    ITEM_NEUTER, // Masamune
    ITEM_NEUTER, // Bandit's Sword
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Battle Axe
    ITEM_FEMININE, // Broad Axe
    ITEM_FEMININE, // Great Axe
    ITEM_FEMININE, // Dragon Axe
    ITEM_FEMININE, // Giant Axe
    ITEM_FEMININE, // Vulcan Axe
    ITEM_FEMININE, // Burning Axe
    ITEM_FEMININE, // Demon Axe
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Mace
    ITEM_FEMININE, // Heavy Mace
    ITEM_MASCULINE, // Battle Mace
    ITEM_FEMININE, // War Mace
    ITEM_FEMININE, // Righteous Mace
    ITEM_FEMININE, // Grievous Mace
    ITEM_FEMININE, // Blessed Mace
    ITEM_FEMININE, // Wicked Mace
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Wooden Stick
    ITEM_MASCULINE, // Magic Rod
    ITEM_MASCULINE, // Witch's Wand
    ITEM_MASCULINE, // Blessed Ankh
    ITEM_MASCULINE, // Psynergy Rod
    ITEM_MASCULINE, // Frost Wand
    ITEM_MASCULINE, // Angelic Ankh
    ITEM_MASCULINE, // Demonic Staff
    ITEM_MASCULINE, // Crystal Rod
    ITEM_MASCULINE, // Zodiac Wand
    ITEM_MASCULINE, // Shaman's Rod
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Leather Armor
    ITEM_FEMININE, // Psynergy Armor
    ITEM_NEUTER, // Chain Mail
    ITEM_FEMININE, // Armored Shell
    ITEM_MASCULINE, // Plate Mail
    ITEM_FEMININE, // Steel Armor
    ITEM_FEMININE, // Spirit Armor
    ITEM_FEMININE, // Dragon Scales
    ITEM_MASCULINE, // Demon Mail
    ITEM_FEMININE, // Asura's Armor
    ITEM_FEMININE, // Spiked Armor
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // Cotton Shirt
    ITEM_FEMININE, // Travel Vest
    ITEM_MASCULINE, // Fur Coat
    ITEM_NEUTER, // Adept's Clothes
    ITEM_NEUTER, // Elven Shirt
    ITEM_FEMININE, // Silver Vest
    ITEM_FEMININE, // Water Jacket
    ITEM_FEMININE, // Storm Gear
    ITEM_MASCULINE, // Kimono
    ITEM_NEUTER, // Ninja Garb
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // One-Piece Dress
    ITEM_NEUTER, // Travel Robe
    ITEM_FEMININE, // Silk Robe
    ITEM_NEUTER, // China Dress
    ITEM_NEUTER, // Jerkin
    ITEM_NEUTER, // Cocktail Dress
    ITEM_FEMININE, // Blessed Robe
    ITEM_MASCULINE, // Magical Cassock
    ITEM_FEMININE, // Mysterious Robe
    ITEM_FEMININE, // Feathered Robe
    ITEM_FEMININE, // Oracle's Robe
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Wooden Shield
    ITEM_MASCULINE, // Bronze Shield
    ITEM_MASCULINE, // Iron Shield
    ITEM_MASCULINE, // Knight's Shield
    ITEM_MASCULINE, // Mirrored Shield
    ITEM_MASCULINE, // Dragon Shield
    ITEM_MASCULINE, // Earth Shield
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Padded Gloves
    ITEM_MASCULINE, // Leather Gloves
    ITEM_MASCULINE, // Gauntlets
    ITEM_MASCULINE, // Vambrace
    ITEM_MASCULINE, // War Gloves
    ITEM_MASCULINE, // Spirit Gloves
    ITEM_MASCULINE, // Battle Gloves
    ITEM_MASCULINE, // Aura Gloves
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Leather Armlet
    ITEM_MASCULINE, // Armlet
    ITEM_MASCULINE, // Heavy Armlet
    ITEM_MASCULINE, // Silver Armlet
    ITEM_MASCULINE, // Spirit Armlet
    ITEM_MASCULINE, // Virtuous Armlet
    ITEM_MASCULINE, // Guardian Armlet
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Open Helm
    ITEM_MASCULINE, // Bronze Helm
    ITEM_MASCULINE, // Iron Helm
    ITEM_MASCULINE, // Steel Helm
    ITEM_MASCULINE, // Silver Helm
    ITEM_MASCULINE, // Knight's Helm
    ITEM_MASCULINE, // Warrior's Helm
    ITEM_MASCULINE, // Adept's Helm
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Leather Cap
    ITEM_FEMININE, // Wooden Cap
    ITEM_FEMININE, // Mail Cap
    ITEM_FEMININE, // Jeweled Crown
    ITEM_FEMININE, // Ninja Hood
    ITEM_FEMININE, // Lucky Cap
    ITEM_FEMININE, // Thunder Crown
    ITEM_MASCULINE, // Prophet's Hat
    ITEM_FEMININE, // Lure Cap
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Circlet
    ITEM_MASCULINE, // Silver Circlet
    ITEM_MASCULINE, // Guardian Circlet
    ITEM_MASCULINE, // Platinum Circlet
    ITEM_MASCULINE, // Mythril Circlet
    ITEM_FEMININE, // Glittering Tiara
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // Herb
    ITEM_FEMININE, // Nut
    ITEM_NEUTER, // Vial
    ITEM_NEUTER, // Potion
    ITEM_NEUTER, // Hermes' Water
    ITEM_FEMININE, // Empty Bottle
    ITEM_MASCULINE, // Psy Crystal
    ITEM_NEUTER, // Antidote
    ITEM_NEUTER, // Elixir
    ITEM_NEUTER, // Water of Life
    ITEM_MASCULINE, // Mist Potion
    ITEM_NEUTER, // Power Bread
    ITEM_MASCULINE, // Cookie
    ITEM_MASCULINE, // Apple
    ITEM_FEMININE, // Hard Nut
    ITEM_FEMININE, // Mint
    ITEM_MASCULINE, // Lucky Pepper
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Lash Pebble
    ITEM_MASCULINE, // Pound Cube
    ITEM_MASCULINE, // Orb of Force
    ITEM_MASCULINE, // Douse Drop
    ITEM_MASCULINE, // Frost Jewel
    ITEM_MASCULINE, // Lifting Gem
    ITEM_NEUTER, // Halt Gem
    ITEM_MASCULINE, // Cloak Ball
    ITEM_MASCULINE, // Carry Stone
    ITEM_FEMININE, // Catch Beads
    ITEM_MASCULINE, // Tremor Bit
    ITEM_NEUTER, // Scoop Gem
    ITEM_NEUTER, // Cyclone Chip
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Burst Brooch
    ITEM_MASCULINE, // Grindstone
    ITEM_FEMININE, // Hover Jade
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Teleport Lapis
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Venus Star
    ITEM_MASCULINE, // Mercury Star
    ITEM_MASCULINE, // Mythril Bag
    ITEM_MASCULINE, // Mythril Bag
    ITEM_MASCULINE, // Mythril Bag
    ITEM_NEUTER, // Small Jewel
    ITEM_FEMININE, // Smoke Bomb
    ITEM_FEMININE, // Sleep Bomb
    ITEM_NEUTER, // Game Ticket
    ITEM_FEMININE, // Lucky Medal
    ITEM_NEUTER, // Dragon's Eye
    ITEM_MASCULINE, // Bone
    ITEM_MASCULINE, // Anchor Charm
    ITEM_MASCULINE, // Corn
    ITEM_MASCULINE, // Cell Key
    ITEM_NEUTER, // Boat Ticket
    ITEM_FEMININE, // Sacred Feather
    ITEM_MASCULINE, // Mystic Draught
    ITEM_MASCULINE, // Oil Drop
    ITEM_FEMININE, // Weasel's Claw
    ITEM_MASCULINE, // Bramble Seed
    ITEM_MASCULINE, // Crystal Powder
    ITEM_MASCULINE, // Black Crystal
    ITEM_MASCULINE, // Red Key
    ITEM_MASCULINE, // Blue Key
    ITEM_MASCULINE, // Mythril Bag
    ITEM_MASCULINE, // Jupiter Star
    ITEM_MASCULINE, // Mars Star
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // Mythril Shirt
    ITEM_NEUTER, // Silk Shirt
    ITEM_NEUTER, // Running Shirt
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Hyper Boots
    ITEM_MASCULINE, // Quick Boots
    ITEM_MASCULINE, // Fur Boots
    ITEM_MASCULINE, // Turtle Boots
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Adept Ring
    ITEM_MASCULINE, // War Ring
    ITEM_MASCULINE, // Sleep Ring
    ITEM_MASCULINE, // Healing Ring
    ITEM_MASCULINE, // Unicorn Ring
    ITEM_MASCULINE, // Fairy Ring
    ITEM_MASCULINE, // Cleric's Ring
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // Huge Sword
    ITEM_FEMININE, // Mythril Blade
    ITEM_NEUTER, // Levatine
    ITEM_MASCULINE, // Darksword
    ITEM_NEUTER, // Excalibur
    ITEM_NEUTER, // Robber's Blade
    ITEM_NEUTER, // Soul Brand
    ITEM_NEUTER, // Storm Brand
    ITEM_MASCULINE, // Hestia Blade
    ITEM_NEUTER, // Lightning Sword
    ITEM_FEMININE, // Rune Blade
    ITEM_NEUTER, // Cloud Brand
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // Sylph Rapier
    ITEM_NEUTER, // Burning Sword
    ITEM_NEUTER, // Pirate's Sword
    ITEM_MASCULINE, // Corsair's Edge
    ITEM_MASCULINE, // Pirate's Sabre
    ITEM_NEUTER, // Hypnos' Sword
    ITEM_MASCULINE, // Mist Sabre
    ITEM_NEUTER, // Phaeton's Blade
    ITEM_NEUTER, // Tisiphone Edge
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Apollo's Axe
    ITEM_FEMININE, // Gaia's Axe
    ITEM_FEMININE, // Stellar Axe
    ITEM_FEMININE, // Captain's Axe
    ITEM_FEMININE, // Viking Axe
    ITEM_FEMININE, // Disk Axe
    ITEM_FEMININE, // Themis' Axe
    ITEM_FEMININE, // Mighty Axe
    ITEM_FEMININE, // Tartarus Axe
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Comet Mace
    ITEM_FEMININE, // Tungsten Mace
    ITEM_FEMININE, // Demon Mace
    ITEM_FEMININE, // Hagbone Mace
    ITEM_FEMININE, // Blow Mace
    ITEM_FEMININE, // Rising Mace
    ITEM_FEMININE, // Thanatos Mace
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Cloud Wand
    ITEM_MASCULINE, // Salamander Rod
    ITEM_MASCULINE, // Nebula Wand
    ITEM_FEMININE, // Dracomace
    ITEM_MASCULINE, // Glower Staff
    ITEM_MASCULINE, // Goblin's Rod
    ITEM_MASCULINE, // Meditation Rod
    ITEM_FEMININE, // Fireman's Pole
    ITEM_MASCULINE, // Atropos' Rod
    ITEM_NEUTER, // Lachesis' Rule
    ITEM_FEMININE, // Clotho's Distaff
    ITEM_MASCULINE, // Staff of Anubis
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Trident
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Planet Armor
    ITEM_MASCULINE, // Dragon Mail
    ITEM_MASCULINE, // Chronos Mail
    ITEM_FEMININE, // Stealth Armor
    ITEM_FEMININE, // Xylion Armor
    ITEM_MASCULINE, // Ixion Mail
    ITEM_NEUTER, // Phantasmal Mail
    ITEM_FEMININE, // Erebus Armor
    ITEM_MASCULINE, // Valkyrie Mail
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Faery Vest
    ITEM_FEMININE, // Mythril Clothes
    ITEM_MASCULINE, // Full Metal Vest
    ITEM_MASCULINE, // Wild Coat
    ITEM_NEUTER, // Floral Dress
    ITEM_MASCULINE, // Festival Coat
    ITEM_FEMININE, // Erinyes Tunic
    ITEM_NEUTER, // Triton's Ward
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Dragon Robe
    ITEM_FEMININE, // Ardagh Robe
    ITEM_FEMININE, // Muni Robe
    ITEM_MASCULINE, // Aeolian Cassock
    ITEM_FEMININE, // Iris Robe
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Luna Shield
    ITEM_MASCULINE, // Dragon Shield
    ITEM_FEMININE, // Flame Shield
    ITEM_MASCULINE, // Terra Shield
    ITEM_MASCULINE, // Cosmos Shield
    ITEM_MASCULINE, // Fujin Shield
    ITEM_MASCULINE, // Aegis Shield
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Aerial Gloves
    ITEM_FEMININE, // Titan Gloves
    ITEM_MASCULINE, // Big Bang Gloves
    ITEM_MASCULINE, // Crafted Gloves
    ITEM_FEMININE, // Riot Gloves
    ITEM_MASCULINE, // Spirit Gloves
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // Clear Bracelet
    ITEM_MASCULINE, // Mythril Armlet
    ITEM_NEUTER, // Bone Armlet
    ITEM_MASCULINE, // Jester's Armlet
    ITEM_NEUTER, // Leda's Bracelet
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Dragon Helm
    ITEM_MASCULINE, // Mythril Helm
    ITEM_MASCULINE, // Fear Helm
    ITEM_MASCULINE, // Millenium Helm
    ITEM_MASCULINE, // Viking Helm
    ITEM_MASCULINE, // Gloria Helm
    ITEM_MASCULINE, // Minerva Helm
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Floating Hat
    ITEM_FEMININE, // Nurse's Cap
    ITEM_FEMININE, // Thorn Crown
    ITEM_FEMININE, // Otafuku Mask
    ITEM_FEMININE, // Hiotoko Mask
    ITEM_FEMININE, // Crown of Glory
    ITEM_FEMININE, // Alastor's Hood
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Pure Circlet
    ITEM_NEUTER, // Astral Circlet
    ITEM_MASCULINE, // Psychic Circlet
    ITEM_MASCULINE, // Demon Circlet
    ITEM_MASCULINE, // Clarity Circlet
    ITEM_MASCULINE, // Brilliant Circlet
    ITEM_NEUTER, // Berserker Band
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // Divine Camisole
    ITEM_NEUTER, // Herbed Shirt
    ITEM_NEUTER, // Golden Shirt
    ITEM_NEUTER, // Casual Shirt
    ITEM_MASCULINE, // Leather Boots
    ITEM_MASCULINE, // Dragon Boots
    ITEM_MASCULINE, // Safety Boots
    ITEM_MASCULINE, // Knight's Greave
    ITEM_MASCULINE, // Silver Greave
    ITEM_FEMININE, // Ninja Sandals
    ITEM_MASCULINE, // Golden Boots
    ITEM_MASCULINE, // Spirit Ring
    ITEM_MASCULINE, // Stardust Ring
    ITEM_MASCULINE, // Aroma Ring
    ITEM_MASCULINE, // Rainbow Ring
    ITEM_MASCULINE, // Soul Ring
    ITEM_MASCULINE, // Guardian Ring
    ITEM_MASCULINE, // Golden Ring
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // Rusty Sword
    ITEM_NEUTER, // Rusty Sword
    ITEM_NEUTER, // Rusty Sword
    ITEM_NEUTER, // Rusty Sword
    ITEM_FEMININE, // Rusty Axe
    ITEM_FEMININE, // Rusty Axe
    ITEM_FEMININE, // Rusty Mace
    ITEM_FEMININE, // Rusty Mace
    ITEM_MASCULINE, // Rusty Staff
    ITEM_MASCULINE, // Rusty Staff
    ITEM_MASCULINE, // Rusty Staff
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Tear Stone
    ITEM_MASCULINE, // Star Dust
    ITEM_FEMININE, // Sylph Feather
    ITEM_FEMININE, // Dragon Skin
    ITEM_FEMININE, // Salamander Tail
    ITEM_NEUTER, // Golem Core
    ITEM_NEUTER, // Mythril Silver
    ITEM_FEMININE, // Dark Matter
    ITEM_NEUTER, // Orihalcon
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Right Prong
    ITEM_MASCULINE, // Left Prong
    ITEM_MASCULINE, // Center Prong
    ITEM_NEUTER, // ?
    ITEM_FEMININE, // Mysterious Card
    ITEM_FEMININE, // Trainer's Whip
    ITEM_NEUTER, // Tomegathericon
    ITEM_NEUTER, // ?
    ITEM_NEUTER, // ?
    ITEM_MASCULINE, // Healing Fungus
    ITEM_MASCULINE, // Laughing Fungus
    ITEM_FEMININE, // Signal Whistle
    ITEM_MASCULINE, // Dancing Idol
    ITEM_MASCULINE, // Pretty Stone
    ITEM_NEUTER, // Red Cloth
    ITEM_FEMININE, // Milk
    ITEM_FEMININE, // Li'l Turtle
    ITEM_MASCULINE, // Aquarius Stone
    ITEM_NEUTER, // Large Bread
    ITEM_FEMININE, // Sea God's Tear
    ITEM_MASCULINE, // Ruin Key
    ITEM_MASCULINE, // Magma Ball
    ITEM_NEUTER, // ???
};
#elif LANGUAGE == LANGUAGE_ITALIAN
// Each item's gender, by item id. BUG: this is The Broken Seal's German table, its rows The Broken Seal's items, so Italian gives an item the gender of that game's item of the same id in German. The names here are The Broken Seal's. UB: an item past the Cleric's Ring, the last of its 269 rows, reads beyond the table; UBFIX gives every item of this game a row.
const Sint8 LocalizedWordItemGenders[] = {
    ITEM_MASCULINE, // ???
    ITEM_NEUTER, // Long Sword
    ITEM_NEUTER, // Broad Sword
    ITEM_NEUTER, // Claymore
    ITEM_NEUTER, // Great Sword
    ITEM_NEUTER, // Shamshir
    ITEM_FEMININE, // Silver Blade
    ITEM_NEUTER, // Masamune
    ITEM_FEMININE, // Arctic Blade
    ITEM_FEMININE, // Gaia Blade
    ITEM_FEMININE, // Sol Blade
    ITEM_NEUTER, // Muramasa
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_FEMININE, // Machete
    ITEM_NEUTER, // Short Sword
    ITEM_NEUTER, // Hunter's Sword
    ITEM_NEUTER, // Battle Rapier
    ITEM_NEUTER, // Master Rapier
    ITEM_FEMININE, // Ninja Blade
    ITEM_NEUTER, // Swift Sword
    ITEM_NEUTER, // Elven Rapier
    ITEM_FEMININE, // Assassin Blade
    ITEM_FEMININE, // Mystery Blade
    ITEM_FEMININE, // Kikuichimonji
    ITEM_NEUTER, // Kusanagi
    ITEM_NEUTER, // Bandit's Sword
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_FEMININE, // Battle Axe
    ITEM_FEMININE, // Broad Axe
    ITEM_FEMININE, // Great Axe
    ITEM_FEMININE, // Dragon Axe
    ITEM_FEMININE, // Giant Axe
    ITEM_FEMININE, // Vulcan Axe
    ITEM_FEMININE, // Burning Axe
    ITEM_FEMININE, // Demon Axe
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_FEMININE, // Mace
    ITEM_FEMININE, // Heavy Mace
    ITEM_MASCULINE, // Battle Mace
    ITEM_FEMININE, // War Mace
    ITEM_FEMININE, // Righteous Mace
    ITEM_FEMININE, // Grievous Mace
    ITEM_FEMININE, // Blessed Mace
    ITEM_FEMININE, // Wicked Mace
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // Wooden Stick
    ITEM_MASCULINE, // Magic Rod
    ITEM_MASCULINE, // Witch's Wand
    ITEM_MASCULINE, // Blessed Ankh
    ITEM_MASCULINE, // Psynergy Rod
    ITEM_MASCULINE, // Frost Wand
    ITEM_MASCULINE, // Angelic Ankh
    ITEM_MASCULINE, // Demonic Staff
    ITEM_MASCULINE, // Crystal Rod
    ITEM_MASCULINE, // Zodiac Wand
    ITEM_MASCULINE, // Shaman's Rod
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_FEMININE, // Leather Armor
    ITEM_FEMININE, // Psynergy Armor
    ITEM_MASCULINE, // Chain Mail
    ITEM_FEMININE, // Armored Shell
    ITEM_MASCULINE, // Plate Mail
    ITEM_FEMININE, // Steel Armor
    ITEM_FEMININE, // Spirit Armor
    ITEM_FEMININE, // Dragon Scales
    ITEM_MASCULINE, // Demon Mail
    ITEM_FEMININE, // Asura's Armor
    ITEM_FEMININE, // Spiked Armor
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_NEUTER, // Cotton Shirt
    ITEM_FEMININE, // Travel Vest
    ITEM_MASCULINE, // Fur Coat
    ITEM_FEMININE, // Adept's Clothes
    ITEM_NEUTER, // Elven Shirt
    ITEM_FEMININE, // Silver Vest
    ITEM_FEMININE, // Water Jacket
    ITEM_FEMININE, // Storm Gear
    ITEM_MASCULINE, // Kimono
    ITEM_NEUTER, // Ninja Garb
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_NEUTER, // One-Piece Dress
    ITEM_NEUTER, // Travel Robe
    ITEM_FEMININE, // Silk Robe
    ITEM_NEUTER, // China Dress
    ITEM_NEUTER, // Jerkin
    ITEM_NEUTER, // Cocktail Dress
    ITEM_FEMININE, // Blessed Robe
    ITEM_MASCULINE, // Magical Cassock
    ITEM_FEMININE, // Mysterious Robe
    ITEM_FEMININE, // Feathered Robe
    ITEM_FEMININE, // Oracle's Robe
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // Wooden Shield
    ITEM_MASCULINE, // Bronze Shield
    ITEM_MASCULINE, // Iron Shield
    ITEM_MASCULINE, // Knight's Shield
    ITEM_MASCULINE, // Mirrored Shield
    ITEM_MASCULINE, // Dragon Shield
    ITEM_MASCULINE, // Earth Shield
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_FEMININE, // Padded Gloves
    ITEM_FEMININE, // Leather Gloves
    ITEM_FEMININE, // Gauntlets
    ITEM_MASCULINE, // Vambrace
    ITEM_FEMININE, // War Gloves
    ITEM_FEMININE, // Spirit Gloves
    ITEM_FEMININE, // Battle Gloves
    ITEM_FEMININE, // Aura Gloves
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // Leather Armlet
    ITEM_MASCULINE, // Armlet
    ITEM_MASCULINE, // Heavy Armlet
    ITEM_MASCULINE, // Silver Armlet
    ITEM_MASCULINE, // Spirit Armlet
    ITEM_MASCULINE, // Virtuous Armlet
    ITEM_MASCULINE, // Guardian Armlet
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // Open Helm
    ITEM_MASCULINE, // Bronze Helm
    ITEM_MASCULINE, // Iron Helm
    ITEM_NEUTER, // Steel Helm
    ITEM_MASCULINE, // Silver Helm
    ITEM_MASCULINE, // Knight's Helm
    ITEM_MASCULINE, // Warrior's Helm
    ITEM_MASCULINE, // Adept's Helm
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_FEMININE, // Leather Cap
    ITEM_FEMININE, // Wooden Cap
    ITEM_FEMININE, // Mail Cap
    ITEM_FEMININE, // Jeweled Crown
    ITEM_FEMININE, // Ninja Hood
    ITEM_FEMININE, // Lucky Cap
    ITEM_FEMININE, // Thunder Crown
    ITEM_MASCULINE, // Prophet's Hat
    ITEM_FEMININE, // Lure Cap
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // Circlet
    ITEM_MASCULINE, // Silver Circlet
    ITEM_MASCULINE, // Guardian Circlet
    ITEM_MASCULINE, // Platinum Circlet
    ITEM_MASCULINE, // Mythril Circlet
    ITEM_FEMININE, // Glittering Tiara
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_NEUTER, // Herb
    ITEM_FEMININE, // Nut
    ITEM_MASCULINE, // Vial
    ITEM_MASCULINE, // Potion
    ITEM_NEUTER, // Hermes' Water
    ITEM_FEMININE, // Empty Bottle
    ITEM_MASCULINE, // Psy Crystal
    ITEM_NEUTER, // Antidote
    ITEM_NEUTER, // Elixir
    ITEM_NEUTER, // Water of Life
    ITEM_MASCULINE, // ?
    ITEM_NEUTER, // Power Bread
    ITEM_NEUTER, // Cookie
    ITEM_MASCULINE, // Apple
    ITEM_FEMININE, // Hard Nut
    ITEM_FEMININE, // Mint
    ITEM_MASCULINE, // Lucky Pepper
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // Orb of Force
    ITEM_MASCULINE, // Douse Drop
    ITEM_MASCULINE, // Frost Jewel
    ITEM_MASCULINE, // Lifting Gem
    ITEM_NEUTER, // Halt Gem
    ITEM_MASCULINE, // Cloak Ball
    ITEM_MASCULINE, // Carry Stone
    ITEM_FEMININE, // Catch Beads
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // Venus Star
    ITEM_MASCULINE, // Mercury Star
    ITEM_MASCULINE, // Mars Star
    ITEM_MASCULINE, // Jupiter Star
    ITEM_MASCULINE, // Mythril Bag
    ITEM_NEUTER, // Small Jewel
    ITEM_FEMININE, // Smoke Bomb
    ITEM_FEMININE, // Sleep Bomb
    ITEM_NEUTER, // Game Ticket
    ITEM_FEMININE, // Lucky Medal
    ITEM_NEUTER, // Dragon's Eye
    ITEM_MASCULINE, // Bone
    ITEM_MASCULINE, // Anchor Charm
    ITEM_MASCULINE, // Corn
    ITEM_MASCULINE, // Cell Key
    ITEM_NEUTER, // Boat Ticket
    ITEM_FEMININE, // Sacred Feather
    ITEM_MASCULINE, // Mystic Draught
    ITEM_MASCULINE, // Oil Drop
    ITEM_FEMININE, // Weasel's Claw
    ITEM_MASCULINE, // Bramble Seed
    ITEM_MASCULINE, // Crystal Powder
    ITEM_MASCULINE, // Black Orb
    ITEM_MASCULINE, // Red Key
    ITEM_MASCULINE, // Blue Key
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_NEUTER, // Mythril Shirt
    ITEM_NEUTER, // Silk Shirt
    ITEM_NEUTER, // Running Shirt
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_FEMININE, // Hyper Boots
    ITEM_FEMININE, // Quick Boots
    ITEM_FEMININE, // Fur Boots
    ITEM_FEMININE, // Turtle Boots
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // ?
    ITEM_MASCULINE, // Adept Ring
    ITEM_MASCULINE, // War Ring
    ITEM_MASCULINE, // Sleep Ring
    ITEM_MASCULINE, // Healing Ring
    ITEM_MASCULINE, // Unicorn Ring
    ITEM_MASCULINE, // Fairy Ring
    ITEM_MASCULINE, // Cleric's Ring
#if FIX_ITALIAN_ITEM_GENDER_READ_PAST_END
    [ITEM_MAGMA_BALL] = ITEM_MASCULINE,
#endif
};
#endif
// clang-format on
