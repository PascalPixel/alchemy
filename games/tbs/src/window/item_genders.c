/* @file */
/* The grammatical gender of each item's name, by item id, in the German and */
/* Italian releases (ITEM_GENDER), which picks the possessive a message */
/* writes before the item. The names in the comments are the English */
/* release's. */

#include "window/articles.h"

/* clang-format off */
/* Each item's gender, by item id. */
#if LANGUAGE == LANGUAGE_GERMAN
const s8 LocalizedWord_ItemGenders[] = {
    ITEM_MASCULINE, /* ??? */
    ITEM_NEUTER, /* Long Sword */
    ITEM_NEUTER, /* Broad Sword */
    ITEM_NEUTER, /* Claymore */
    ITEM_NEUTER, /* Great Sword */
    ITEM_NEUTER, /* Shamshir */
    ITEM_FEMININE, /* Silver Blade */
    ITEM_NEUTER, /* Masamune */
    ITEM_FEMININE, /* Arctic Blade */
    ITEM_FEMININE, /* Gaia Blade */
    ITEM_FEMININE, /* Sol Blade */
    ITEM_NEUTER, /* Muramasa */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Machete */
    ITEM_NEUTER, /* Short Sword */
    ITEM_NEUTER, /* Hunter's Sword */
    ITEM_NEUTER, /* Battle Rapier */
    ITEM_NEUTER, /* Master Rapier */
    ITEM_FEMININE, /* Ninja Blade */
    ITEM_NEUTER, /* Swift Sword */
    ITEM_NEUTER, /* Elven Rapier */
    ITEM_FEMININE, /* Assassin Blade */
    ITEM_FEMININE, /* Mystery Blade */
    ITEM_FEMININE, /* Kikuichimonji */
    ITEM_NEUTER, /* Kusanagi */
    ITEM_NEUTER, /* Bandit's Sword */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Battle Axe */
    ITEM_FEMININE, /* Broad Axe */
    ITEM_FEMININE, /* Great Axe */
    ITEM_FEMININE, /* Dragon Axe */
    ITEM_FEMININE, /* Giant Axe */
    ITEM_FEMININE, /* Vulcan Axe */
    ITEM_FEMININE, /* Burning Axe */
    ITEM_FEMININE, /* Demon Axe */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Mace */
    ITEM_FEMININE, /* Heavy Mace */
    ITEM_MASCULINE, /* Battle Mace */
    ITEM_FEMININE, /* War Mace */
    ITEM_FEMININE, /* Righteous Mace */
    ITEM_FEMININE, /* Grievous Mace */
    ITEM_FEMININE, /* Blessed Mace */
    ITEM_FEMININE, /* Wicked Mace */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Wooden Stick */
    ITEM_MASCULINE, /* Magic Rod */
    ITEM_MASCULINE, /* Witch's Wand */
    ITEM_MASCULINE, /* Blessed Ankh */
    ITEM_MASCULINE, /* Psynergy Rod */
    ITEM_MASCULINE, /* Frost Wand */
    ITEM_MASCULINE, /* Angelic Ankh */
    ITEM_MASCULINE, /* Demonic Staff */
    ITEM_MASCULINE, /* Crystal Rod */
    ITEM_MASCULINE, /* Zodiac Wand */
    ITEM_MASCULINE, /* Shaman's Rod */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Leather Armor */
    ITEM_FEMININE, /* Psynergy Armor */
    ITEM_MASCULINE, /* Chain Mail */
    ITEM_FEMININE, /* Armored Shell */
    ITEM_MASCULINE, /* Plate Mail */
    ITEM_FEMININE, /* Steel Armor */
    ITEM_FEMININE, /* Spirit Armor */
    ITEM_FEMININE, /* Dragon Scales */
    ITEM_MASCULINE, /* Demon Mail */
    ITEM_FEMININE, /* Asura's Armor */
    ITEM_FEMININE, /* Spiked Armor */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_NEUTER, /* Cotton Shirt */
    ITEM_FEMININE, /* Travel Vest */
    ITEM_MASCULINE, /* Fur Coat */
    ITEM_FEMININE, /* Adept's Clothes */
    ITEM_NEUTER, /* Elven Shirt */
    ITEM_FEMININE, /* Silver Vest */
    ITEM_FEMININE, /* Water Jacket */
    ITEM_FEMININE, /* Storm Gear */
    ITEM_MASCULINE, /* Kimono */
    ITEM_NEUTER, /* Ninja Garb */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_NEUTER, /* One-Piece Dress */
    ITEM_NEUTER, /* Travel Robe */
    ITEM_FEMININE, /* Silk Robe */
    ITEM_NEUTER, /* China Dress */
    ITEM_NEUTER, /* Jerkin */
    ITEM_NEUTER, /* Cocktail Dress */
    ITEM_FEMININE, /* Blessed Robe */
    ITEM_MASCULINE, /* Magical Cassock */
    ITEM_FEMININE, /* Mysterious Robe */
    ITEM_FEMININE, /* Feathered Robe */
    ITEM_FEMININE, /* Oracle's Robe */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Wooden Shield */
    ITEM_MASCULINE, /* Bronze Shield */
    ITEM_MASCULINE, /* Iron Shield */
    ITEM_MASCULINE, /* Knight's Shield */
    ITEM_MASCULINE, /* Mirrored Shield */
    ITEM_MASCULINE, /* Dragon Shield */
    ITEM_MASCULINE, /* Earth Shield */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Padded Gloves */
    ITEM_FEMININE, /* Leather Gloves */
    ITEM_FEMININE, /* Gauntlets */
    ITEM_MASCULINE, /* Vambrace */
    ITEM_FEMININE, /* War Gloves */
    ITEM_FEMININE, /* Spirit Gloves */
    ITEM_FEMININE, /* Battle Gloves */
    ITEM_FEMININE, /* Aura Gloves */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Leather Armlet */
    ITEM_MASCULINE, /* Armlet */
    ITEM_MASCULINE, /* Heavy Armlet */
    ITEM_MASCULINE, /* Silver Armlet */
    ITEM_MASCULINE, /* Spirit Armlet */
    ITEM_MASCULINE, /* Virtuous Armlet */
    ITEM_MASCULINE, /* Guardian Armlet */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Open Helm */
    ITEM_MASCULINE, /* Bronze Helm */
    ITEM_MASCULINE, /* Iron Helm */
    ITEM_NEUTER, /* Steel Helm */
    ITEM_MASCULINE, /* Silver Helm */
    ITEM_MASCULINE, /* Knight's Helm */
    ITEM_MASCULINE, /* Warrior's Helm */
    ITEM_MASCULINE, /* Adept's Helm */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Leather Cap */
    ITEM_FEMININE, /* Wooden Cap */
    ITEM_FEMININE, /* Mail Cap */
    ITEM_FEMININE, /* Jeweled Crown */
    ITEM_FEMININE, /* Ninja Hood */
    ITEM_FEMININE, /* Lucky Cap */
    ITEM_FEMININE, /* Thunder Crown */
    ITEM_MASCULINE, /* Prophet's Hat */
    ITEM_FEMININE, /* Lure Cap */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Circlet */
    ITEM_MASCULINE, /* Silver Circlet */
    ITEM_MASCULINE, /* Guardian Circlet */
    ITEM_MASCULINE, /* Platinum Circlet */
    ITEM_MASCULINE, /* Mythril Circlet */
    ITEM_FEMININE, /* Glittering Tiara */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_NEUTER, /* Herb */
    ITEM_FEMININE, /* Nut */
    ITEM_MASCULINE, /* Vial */
    ITEM_MASCULINE, /* Potion */
    ITEM_NEUTER, /* Hermes' Water */
    ITEM_FEMININE, /* Empty Bottle */
    ITEM_MASCULINE, /* Psy Crystal */
    ITEM_NEUTER, /* Antidote */
    ITEM_NEUTER, /* Elixir */
    ITEM_NEUTER, /* Water of Life */
    ITEM_MASCULINE, /* ? */
    ITEM_NEUTER, /* Power Bread */
    ITEM_NEUTER, /* Cookie */
    ITEM_MASCULINE, /* Apple */
    ITEM_FEMININE, /* Hard Nut */
    ITEM_FEMININE, /* Mint */
    ITEM_MASCULINE, /* Lucky Pepper */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Orb of Force */
    ITEM_MASCULINE, /* Douse Drop */
    ITEM_MASCULINE, /* Frost Jewel */
    ITEM_MASCULINE, /* Lifting Gem */
    ITEM_NEUTER, /* Halt Gem */
    ITEM_MASCULINE, /* Cloak Ball */
    ITEM_MASCULINE, /* Carry Stone */
    ITEM_FEMININE, /* Catch Beads */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Venus Star */
    ITEM_MASCULINE, /* Mercury Star */
    ITEM_MASCULINE, /* Mars Star */
    ITEM_MASCULINE, /* Jupiter Star */
    ITEM_MASCULINE, /* Mythril Bag */
    ITEM_NEUTER, /* Small Jewel */
    ITEM_FEMININE, /* Smoke Bomb */
    ITEM_FEMININE, /* Sleep Bomb */
    ITEM_NEUTER, /* Game Ticket */
    ITEM_FEMININE, /* Lucky Medal */
    ITEM_NEUTER, /* Dragon's Eye */
    ITEM_MASCULINE, /* Bone */
    ITEM_MASCULINE, /* Anchor Charm */
    ITEM_MASCULINE, /* Corn */
    ITEM_MASCULINE, /* Cell Key */
    ITEM_NEUTER, /* Boat Ticket */
    ITEM_FEMININE, /* Sacred Feather */
    ITEM_MASCULINE, /* Mystic Draught */
    ITEM_MASCULINE, /* Oil Drop */
    ITEM_FEMININE, /* Weasel's Claw */
    ITEM_MASCULINE, /* Bramble Seed */
    ITEM_MASCULINE, /* Crystal Powder */
    ITEM_MASCULINE, /* Black Orb */
    ITEM_MASCULINE, /* Red Key */
    ITEM_MASCULINE, /* Blue Key */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_NEUTER, /* Mythril Shirt */
    ITEM_NEUTER, /* Silk Shirt */
    ITEM_NEUTER, /* Running Shirt */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Hyper Boots */
    ITEM_FEMININE, /* Quick Boots */
    ITEM_FEMININE, /* Fur Boots */
    ITEM_FEMININE, /* Turtle Boots */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Adept Ring */
    ITEM_MASCULINE, /* War Ring */
    ITEM_MASCULINE, /* Sleep Ring */
    ITEM_MASCULINE, /* Healing Ring */
    ITEM_MASCULINE, /* Unicorn Ring */
    ITEM_MASCULINE, /* Fairy Ring */
    ITEM_MASCULINE, /* Cleric's Ring */
};
#elif LANGUAGE == LANGUAGE_ITALIAN
const s8 LocalizedWord_ItemGenders[] = {
    ITEM_MASCULINE, /* ??? */
    ITEM_FEMININE, /* Long Sword */
    ITEM_FEMININE, /* Broad Sword */
    ITEM_FEMININE, /* Claymore */
    ITEM_FEMININE, /* Great Sword */
    ITEM_FEMININE, /* Shamshir */
    ITEM_FEMININE, /* Silver Blade */
    ITEM_FEMININE, /* Masamune */
    ITEM_FEMININE, /* Arctic Blade */
    ITEM_FEMININE, /* Gaia Blade */
    ITEM_FEMININE, /* Sol Blade */
    ITEM_FEMININE, /* Muramasa */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Machete */
    ITEM_FEMININE, /* Short Sword */
    ITEM_FEMININE, /* Hunter's Sword */
    ITEM_MASCULINE, /* Battle Rapier */
    ITEM_MASCULINE, /* Master Rapier */
    ITEM_FEMININE, /* Ninja Blade */
    ITEM_FEMININE, /* Swift Sword */
    ITEM_MASCULINE, /* Elven Rapier */
    ITEM_FEMININE, /* Assassin Blade */
    ITEM_FEMININE, /* Mystery Blade */
    ITEM_FEMININE, /* Kikuichimonji */
    ITEM_FEMININE, /* Kusanagi */
    ITEM_FEMININE, /* Bandit's Sword */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Battle Axe */
    ITEM_FEMININE, /* Broad Axe */
    ITEM_FEMININE, /* Great Axe */
    ITEM_FEMININE, /* Dragon Axe */
    ITEM_FEMININE, /* Giant Axe */
    ITEM_FEMININE, /* Vulcan Axe */
    ITEM_FEMININE, /* Burning Axe */
    ITEM_FEMININE, /* Demon Axe */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Mace */
    ITEM_FEMININE, /* Heavy Mace */
    ITEM_FEMININE, /* Battle Mace */
    ITEM_FEMININE, /* War Mace */
    ITEM_FEMININE, /* Righteous Mace */
    ITEM_FEMININE, /* Grievous Mace */
    ITEM_FEMININE, /* Blessed Mace */
    ITEM_FEMININE, /* Wicked Mace */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Wooden Stick */
    ITEM_MASCULINE, /* Magic Rod */
    ITEM_MASCULINE, /* Witch's Wand */
    ITEM_MASCULINE, /* Blessed Ankh */
    ITEM_FEMININE, /* Psynergy Rod */
    ITEM_FEMININE, /* Frost Wand */
    ITEM_MASCULINE, /* Angelic Ankh */
    ITEM_MASCULINE, /* Demonic Staff */
    ITEM_FEMININE, /* Crystal Rod */
    ITEM_MASCULINE, /* Zodiac Wand */
    ITEM_MASCULINE, /* Shaman's Rod */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Leather Armor */
    ITEM_FEMININE, /* Psynergy Armor */
    ITEM_FEMININE, /* Chain Mail */
    ITEM_FEMININE, /* Armored Shell */
    ITEM_FEMININE, /* Plate Mail */
    ITEM_FEMININE, /* Steel Armor */
    ITEM_FEMININE, /* Spirit Armor */
    ITEM_FEMININE, /* Dragon Scales */
    ITEM_FEMININE, /* Demon Mail */
    ITEM_FEMININE, /* Asura's Armor */
    ITEM_FEMININE, /* Spiked Armor */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Cotton Shirt */
    ITEM_FEMININE, /* Travel Vest */
    ITEM_MASCULINE, /* Fur Coat */
    ITEM_FEMININE, /* Adept's Clothes */
    ITEM_FEMININE, /* Elven Shirt */
    ITEM_FEMININE, /* Silver Vest */
    ITEM_FEMININE, /* Water Jacket */
    ITEM_FEMININE, /* Storm Gear */
    ITEM_MASCULINE, /* Kimono */
    ITEM_MASCULINE, /* Ninja Garb */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* One-Piece Dress */
    ITEM_MASCULINE, /* Travel Robe */
    ITEM_MASCULINE, /* Silk Robe */
    ITEM_MASCULINE, /* China Dress */
    ITEM_MASCULINE, /* Jerkin */
    ITEM_MASCULINE, /* Cocktail Dress */
    ITEM_MASCULINE, /* Blessed Robe */
    ITEM_FEMININE, /* Magical Cassock */
    ITEM_MASCULINE, /* Mysterious Robe */
    ITEM_MASCULINE, /* Feathered Robe */
    ITEM_MASCULINE, /* Oracle's Robe */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Wooden Shield */
    ITEM_MASCULINE, /* Bronze Shield */
    ITEM_MASCULINE, /* Iron Shield */
    ITEM_MASCULINE, /* Knight's Shield */
    ITEM_MASCULINE, /* Mirrored Shield */
    ITEM_MASCULINE, /* Dragon Shield */
    ITEM_MASCULINE, /* Earth Shield */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Padded Gloves */
    ITEM_MASCULINE, /* Leather Gloves */
    ITEM_MASCULINE, /* Gauntlets */
    ITEM_MASCULINE, /* Vambrace */
    ITEM_MASCULINE, /* War Gloves */
    ITEM_MASCULINE, /* Spirit Gloves */
    ITEM_MASCULINE, /* Battle Gloves */
    ITEM_MASCULINE, /* Aura Gloves */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Leather Armlet */
    ITEM_MASCULINE, /* Armlet */
    ITEM_MASCULINE, /* Heavy Armlet */
    ITEM_MASCULINE, /* Silver Armlet */
    ITEM_MASCULINE, /* Spirit Armlet */
    ITEM_MASCULINE, /* Virtuous Armlet */
    ITEM_MASCULINE, /* Guardian Armlet */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Open Helm */
    ITEM_MASCULINE, /* Bronze Helm */
    ITEM_MASCULINE, /* Iron Helm */
    ITEM_MASCULINE, /* Steel Helm */
    ITEM_MASCULINE, /* Silver Helm */
    ITEM_MASCULINE, /* Knight's Helm */
    ITEM_MASCULINE, /* Warrior's Helm */
    ITEM_MASCULINE, /* Adept's Helm */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Leather Cap */
    ITEM_MASCULINE, /* Wooden Cap */
    ITEM_MASCULINE, /* Mail Cap */
    ITEM_FEMININE, /* Jeweled Crown */
    ITEM_MASCULINE, /* Ninja Hood */
    ITEM_MASCULINE, /* Lucky Cap */
    ITEM_FEMININE, /* Thunder Crown */
    ITEM_MASCULINE, /* Prophet's Hat */
    ITEM_MASCULINE, /* Lure Cap */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Circlet */
    ITEM_FEMININE, /* Silver Circlet */
    ITEM_FEMININE, /* Guardian Circlet */
    ITEM_FEMININE, /* Platinum Circlet */
    ITEM_FEMININE, /* Mythril Circlet */
    ITEM_FEMININE, /* Glittering Tiara */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Herb */
    ITEM_FEMININE, /* Nut */
    ITEM_FEMININE, /* Vial */
    ITEM_FEMININE, /* Potion */
    ITEM_FEMININE, /* Hermes' Water */
    ITEM_FEMININE, /* Empty Bottle */
    ITEM_FEMININE, /* Psy Crystal */
    ITEM_MASCULINE, /* Antidote */
    ITEM_MASCULINE, /* Elixir */
    ITEM_FEMININE, /* Water of Life */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Power Bread */
    ITEM_MASCULINE, /* Cookie */
    ITEM_FEMININE, /* Apple */
    ITEM_FEMININE, /* Hard Nut */
    ITEM_FEMININE, /* Mint */
    ITEM_MASCULINE, /* Lucky Pepper */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Orb of Force */
    ITEM_FEMININE, /* Douse Drop */
    ITEM_FEMININE, /* Frost Jewel */
    ITEM_FEMININE, /* Lifting Gem */
    ITEM_FEMININE, /* Halt Gem */
    ITEM_FEMININE, /* Cloak Ball */
    ITEM_FEMININE, /* Carry Stone */
    ITEM_FEMININE, /* Catch Beads */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Venus Star */
    ITEM_MASCULINE, /* Mercury Star */
    ITEM_MASCULINE, /* Mars Star */
    ITEM_MASCULINE, /* Jupiter Star */
    ITEM_FEMININE, /* Mythril Bag */
    ITEM_MASCULINE, /* Small Jewel */
    ITEM_FEMININE, /* Smoke Bomb */
    ITEM_FEMININE, /* Sleep Bomb */
    ITEM_MASCULINE, /* Game Ticket */
    ITEM_FEMININE, /* Lucky Medal */
    ITEM_MASCULINE, /* Dragon's Eye */
    ITEM_MASCULINE, /* Bone */
    ITEM_FEMININE, /* Anchor Charm */
    ITEM_MASCULINE, /* Corn */
    ITEM_FEMININE, /* Cell Key */
    ITEM_MASCULINE, /* Boat Ticket */
    ITEM_FEMININE, /* Sacred Feather */
    ITEM_MASCULINE, /* Mystic Draught */
    ITEM_FEMININE, /* Oil Drop */
    ITEM_MASCULINE, /* Weasel's Claw */
    ITEM_MASCULINE, /* Bramble Seed */
    ITEM_FEMININE, /* Crystal Powder */
    ITEM_FEMININE, /* Black Orb */
    ITEM_FEMININE, /* Red Key */
    ITEM_FEMININE, /* Blue Key */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_FEMININE, /* Mythril Shirt */
    ITEM_FEMININE, /* Silk Shirt */
    ITEM_FEMININE, /* Running Shirt */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Hyper Boots */
    ITEM_MASCULINE, /* Quick Boots */
    ITEM_MASCULINE, /* Fur Boots */
    ITEM_MASCULINE, /* Turtle Boots */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* ? */
    ITEM_MASCULINE, /* Adept Ring */
    ITEM_MASCULINE, /* War Ring */
    ITEM_MASCULINE, /* Sleep Ring */
    ITEM_MASCULINE, /* Healing Ring */
    ITEM_MASCULINE, /* Unicorn Ring */
    ITEM_MASCULINE, /* Fairy Ring */
    ITEM_MASCULINE, /* Cleric's Ring */
};
#endif
/* clang-format on */
