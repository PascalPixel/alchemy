#ifndef ALCHEMY_CHARACTER_IDS_H
#define ALCHEMY_CHARACTER_IDS_H

/// @file
/// Character numbers, with each character's Japanese name, and the portraits.

/// The characters, one for each row of gCharacter_DefinitionTable
/// (character_table.c) in order; unit ids below ENEMY_ID_FIRST are these.
enum CharacterId {
  ISAAC, ///< Robin
  GARET, ///< Jerald
  IVAN,  ///< Ivan
  MIA,   ///< Mary
  FELIX, ///< Garcia
  JENNA, ///< Jasmine
  SHEBA, ///< Sheba
  PIERS  ///< Picard
};

/// The portraits, one for each row of the portrait icons (Ui_Icons,
/// graphics/ui/icons/icons.tsv) in order; a row the code doesn't name yet keeps
/// its number as its placeholder, and rows 8 to 15 and 65 to 79 are empty. A
/// character's portrait is its character id. Icon_LoadWithPalette takes a
/// portrait number below 128 as its row, and the other speakers' portraits,
/// numbered from 128 (Portrait_GetForSpeaker), as the rows from
/// PORTRAIT_JUPITER_DJINNI on; each of those is the face of the speaker whose
/// sprite gPortrait_SpriteMap pairs with it, and takes that sprite's name.
enum PortraitId {
  PORTRAIT_ISAAC,
  PORTRAIT_GARET,
  PORTRAIT_IVAN,
  PORTRAIT_MIA,
  PORTRAIT_FELIX,
  PORTRAIT_JENNA,
  PORTRAIT_SHEBA,
  PORTRAIT_PIERS,
  PORTRAIT_08,
  PORTRAIT_09,
  PORTRAIT_10,
  PORTRAIT_11,
  PORTRAIT_12,
  PORTRAIT_13,
  PORTRAIT_14,
  PORTRAIT_15,
  /// ★ Named from what the player sees: a Jupiter Djinni, white with a fin-like
  /// crest (jupiter_djinni.png)
  PORTRAIT_JUPITER_DJINNI,
  /// ★ Named from what the player sees: a Venus Djinni, brown with blue eyes
  /// (venus_djinni.png)
  PORTRAIT_VENUS_DJINNI,
  /// ★ Named from what the player sees: a Mars Djinni, red and gold
  /// (mars_djinni.png)
  PORTRAIT_MARS_DJINNI,
  /// ★ Named from what the player sees: a Mercury Djinni, blue with spines
  /// (mercury_djinni.png)
  PORTRAIT_MERCURY_DJINNI,
  /// ★ Named from what the player sees: Kraden, white-haired and white-bearded
  /// (kraden.png)
  PORTRAIT_KRADEN,
  /// ★ Named from what the player sees: Saturos, blue-skinned with pale blue
  /// hair (saturos.png)
  PORTRAIT_SATUROS,
  /// ★ Named from what the player sees: Menardi, with long fair hair
  /// (menardi.png)
  PORTRAIT_MENARDI,
  /// ★ Named from what the player sees: Alex, with long pale blue hair
  /// (alex.png)
  PORTRAIT_ALEX,
  /// ★ Named from what the player sees: Dora, with fair hair (dora.png)
  PORTRAIT_DORA,
  /// ★ Named from what the player sees: Kyle, with fair hair (kyle.png)
  PORTRAIT_KYLE,
  /// ★ Named from what the player sees: a woman with orange hair and a green
  /// collar (woman_in_green_headscarf.png)
  PORTRAIT_WOMAN_IN_GREEN_HEADSCARF,
  /// ★ Named from what the player sees: a young woman with orange hair
  /// (woman_with_orange_ponytail.png)
  PORTRAIT_WOMAN_WITH_ORANGE_PONYTAIL,
  /// ★ Named from what the player sees: a boy with spiky orange hair
  /// (boy_with_orange_hair.png)
  PORTRAIT_BOY_WITH_ORANGE_HAIR,
  /// ★ Named from what the player sees: a man with brown hair and a moustache
  /// (man_in_teal_shirt.png)
  PORTRAIT_MAN_IN_TEAL_SHIRT,
  /// ★ Named from what the player sees: McCoy, stout with red hair
  /// (mccoy.png)
  PORTRAIT_MCCOY,
  /// ★ Named from what the player sees: a woman with curly purple hair and an
  /// earring (woman_with_purple_hair.png)
  PORTRAIT_WOMAN_WITH_PURPLE_HAIR,
  /// ★ Named from what the player sees: Tret's gentle face, green with a heavy
  /// brow (tret.png)
  PORTRAIT_TRET,
  /// ★ Named from what the player sees: Laurel's face, orange with green eyes
  /// (laurel.png)
  PORTRAIT_LAUREL,
  /// ★ Named from what the player sees: Nyunpa, white-haired and white-bearded
  /// (nyunpa.png)
  PORTRAIT_NYUNPA,
  /// ★ Named from what the player sees: Hammet, in a turban (hammet.png)
  PORTRAIT_HAMMET,
  /// ★ Named from what the player sees: Layana, with long red hair
  /// (layana.png)
  PORTRAIT_LAYANA,
  /// ★ Named from what the player sees: Feh, with black hair (feh.png)
  PORTRAIT_FEH,
  /// ★ Named from what the player sees: Feizhi, with purple hair (feizhi.png)
  PORTRAIT_FEIZHI,
  /// ★ Named from what the player sees: Hsu, with brown hair cut straight
  /// (hsu.png)
  PORTRAIT_HSU,
  /// ★ Named from what the player sees: Donpa, with long grey hair and a long
  /// moustache (donpa.png)
  PORTRAIT_DONPA,
  /// ★ Named from what the player sees: Dodonpa, with dark blue hair and a
  /// moustache (dodonpa.png)
  PORTRAIT_DODONPA,
  /// ★ Named from what the player sees: Babi, with long pink hair (babi.png)
  PORTRAIT_BABI,
  /// ★ Named from what the player sees: Iodem, with grey hair and a grey beard
  /// (iodem.png)
  PORTRAIT_IODEM,
  /// ★ Named from what the player sees: Hama, with long purple hair
  /// (hama.png)
  PORTRAIT_HAMA,
  /// ★ Named from what the player sees: Kaja, bald and bearded (kaja.png)
  PORTRAIT_KAJA,
  /// ★ Named from what the player sees: an old man with white hair and a white
  /// beard (man_with_white_beard_in_green.png)
  PORTRAIT_MAN_WITH_WHITE_BEARD_IN_GREEN,
  /// ★ Named from what the player sees: a young man with blue hair and a yellow
  /// headband (young_man_with_blue_hair.png)
  PORTRAIT_YOUNG_MAN_WITH_BLUE_HAIR,
  /// ★ Named from what the player sees: a young man with red hair and a yellow
  /// headband (young_man_with_red_hair.png)
  PORTRAIT_YOUNG_MAN_WITH_RED_HAIR,
  /// ★ Named from what the player sees: a man with a white beard in a tall hat
  /// (man_in_tall_hat.png)
  PORTRAIT_MAN_IN_TALL_HAT,
  /// ★ Named from what the player sees: a man with brown hair and a beard
  /// (bearded_man_in_long_robe.png)
  PORTRAIT_BEARDED_MAN_IN_LONG_ROBE,
  /// ★ Named from what the player sees: a bald man with a moustache
  /// (bald_man_in_red.png)
  PORTRAIT_BALD_MAN_IN_RED,
  /// ★ Named from what the player sees: a young man with fair hair and a
  /// moustache (young_man_in_green.png)
  PORTRAIT_YOUNG_MAN_IN_GREEN,
  /// ★ Named from what the player sees: a young woman with brown hair
  /// (woman_with_ponytail.png)
  PORTRAIT_WOMAN_WITH_PONYTAIL,
  /// ★ Named from what the player sees: a woman with red-brown hair
  /// (woman_in_pink_apron.png)
  PORTRAIT_WOMAN_IN_PINK_APRON,
  /// ★ Named from what the player sees: a man with dark hair and a full beard
  /// (man_with_dark_hair.png)
  PORTRAIT_MAN_WITH_DARK_HAIR,
  PORTRAIT_ISAAC_CHILD,
  PORTRAIT_GARET_CHILD,
  /// ★ Named from what the player sees: Felix as a child (felix_child.png)
  PORTRAIT_FELIX_CHILD,
  /// ★ Named from what the player sees: Jenna as a child (jenna_child.png)
  PORTRAIT_JENNA_CHILD,
  /// ★ Named from what the player sees: Felix behind his green mask
  /// (felix_masked.png)
  PORTRAIT_FELIX_MASKED,
  /// ★ Named from what the player sees: Tret's wrathful face, with knotted
  /// brows and red eyes (tret_wrathful.png)
  PORTRAIT_TRET_WRATHFUL,
  /// The row gPortrait_SpriteMap pairs with SPRITE_FELIX, drawn from
  /// icons.tsv's FELIX, as PORTRAIT_FELIX is. ★ Named from what the player
  /// sees: Felix, his face unmasked (felix.png)
  PORTRAIT_FELIX_UNMASKED,
  /// ★ Named from what the player sees: Babi as a purple shape against red
  /// (invisible_babi.png)
  PORTRAIT_INVISIBLE_BABI,
  /// ★ Named from what the player sees: Faran, with fair hair and a moustache
  /// (faran.png)
  PORTRAIT_FARAN,
  PORTRAIT_65,
  PORTRAIT_66,
  PORTRAIT_67,
  PORTRAIT_68,
  PORTRAIT_69,
  PORTRAIT_70,
  PORTRAIT_71,
  PORTRAIT_72,
  PORTRAIT_73,
  PORTRAIT_74,
  PORTRAIT_75,
  PORTRAIT_76,
  PORTRAIT_77,
  PORTRAIT_78,
  PORTRAIT_79
};

#endif
