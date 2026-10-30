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
  /// crest (icon_16.png)
  PORTRAIT_JUPITER_DJINNI,
  /// ★ Named from what the player sees: a Venus Djinni, brown with blue eyes
  /// (icon_17.png)
  PORTRAIT_VENUS_DJINNI,
  /// ★ Named from what the player sees: a Mars Djinni, red and gold
  /// (icon_18.png)
  PORTRAIT_MARS_DJINNI,
  /// ★ Named from what the player sees: a Mercury Djinni, blue with spines
  /// (icon_19.png)
  PORTRAIT_MERCURY_DJINNI,
  /// ★ Named from what the player sees: Kraden, white-haired and white-bearded
  /// (icon_20.png)
  PORTRAIT_KRADEN,
  /// ★ Named from what the player sees: Saturos, blue-skinned with pale blue
  /// hair (icon_21.png)
  PORTRAIT_SATUROS,
  /// ★ Named from what the player sees: Menardi, with long fair hair
  /// (icon_22.png)
  PORTRAIT_MENARDI,
  /// ★ Named from what the player sees: Alex, with long pale blue hair
  /// (icon_23.png)
  PORTRAIT_ALEX,
  /// ★ Named from what the player sees: Dora, with fair hair (icon_24.png)
  PORTRAIT_DORA,
  /// ★ Named from what the player sees: Kyle, with fair hair (icon_25.png)
  PORTRAIT_KYLE,
  /// ★ Named from what the player sees: a woman with orange hair and a green
  /// collar (icon_26.png)
  PORTRAIT_WOMAN_IN_GREEN_HEADSCARF,
  /// ★ Named from what the player sees: a young woman with orange hair
  /// (icon_27.png)
  PORTRAIT_WOMAN_WITH_ORANGE_PONYTAIL,
  /// ★ Named from what the player sees: a boy with spiky orange hair
  /// (icon_28.png)
  PORTRAIT_BOY_WITH_ORANGE_HAIR,
  /// ★ Named from what the player sees: a man with brown hair and a moustache
  /// (icon_29.png)
  PORTRAIT_MAN_IN_TEAL_SHIRT,
  /// ★ Named from what the player sees: McCoy, stout with red hair
  /// (icon_30.png)
  PORTRAIT_MCCOY,
  /// ★ Named from what the player sees: a woman with curly purple hair and an
  /// earring (icon_31.png)
  PORTRAIT_WOMAN_WITH_PURPLE_HAIR,
  /// ★ Named from what the player sees: Tret's gentle face, green with a heavy
  /// brow (icon_32.png)
  PORTRAIT_TRET,
  /// ★ Named from what the player sees: Laurel's face, orange with green eyes
  /// (icon_33.png)
  PORTRAIT_LAUREL,
  /// ★ Named from what the player sees: Nyunpa, white-haired and white-bearded
  /// (icon_34.png)
  PORTRAIT_NYUNPA,
  /// ★ Named from what the player sees: Hammet, in a turban (icon_35.png)
  PORTRAIT_HAMMET,
  /// ★ Named from what the player sees: Layana, with long red hair
  /// (icon_36.png)
  PORTRAIT_LAYANA,
  /// ★ Named from what the player sees: Feh, with black hair (icon_37.png)
  PORTRAIT_FEH,
  /// ★ Named from what the player sees: Feizhi, with purple hair (icon_38.png)
  PORTRAIT_FEIZHI,
  /// ★ Named from what the player sees: Hsu, with brown hair cut straight
  /// (icon_39.png)
  PORTRAIT_HSU,
  /// ★ Named from what the player sees: Donpa, with long grey hair and a long
  /// moustache (icon_40.png)
  PORTRAIT_DONPA,
  /// ★ Named from what the player sees: Dodonpa, with dark blue hair and a
  /// moustache (icon_41.png)
  PORTRAIT_DODONPA,
  /// ★ Named from what the player sees: Babi, with long pink hair (icon_42.png)
  PORTRAIT_BABI,
  /// ★ Named from what the player sees: Iodem, with grey hair and a grey beard
  /// (icon_43.png)
  PORTRAIT_IODEM,
  /// ★ Named from what the player sees: Hama, with long purple hair
  /// (icon_44.png)
  PORTRAIT_HAMA,
  /// ★ Named from what the player sees: Kaja, bald and bearded (icon_45.png)
  PORTRAIT_KAJA,
  /// ★ Named from what the player sees: an old man with white hair and a white
  /// beard (icon_46.png)
  PORTRAIT_MAN_WITH_WHITE_BEARD_IN_GREEN,
  /// ★ Named from what the player sees: a young man with blue hair and a yellow
  /// headband (icon_47.png)
  PORTRAIT_YOUNG_MAN_WITH_BLUE_HAIR,
  /// ★ Named from what the player sees: a young man with red hair and a yellow
  /// headband (icon_48.png)
  PORTRAIT_YOUNG_MAN_WITH_RED_HAIR,
  /// ★ Named from what the player sees: a man with a white beard in a tall hat
  /// (icon_49.png)
  PORTRAIT_MAN_IN_TALL_HAT,
  /// ★ Named from what the player sees: a man with brown hair and a beard
  /// (icon_50.png)
  PORTRAIT_BEARDED_MAN_IN_LONG_ROBE,
  /// ★ Named from what the player sees: a bald man with a moustache
  /// (icon_51.png)
  PORTRAIT_BALD_MAN_IN_RED,
  /// ★ Named from what the player sees: a young man with fair hair and a
  /// moustache (icon_52.png)
  PORTRAIT_YOUNG_MAN_IN_GREEN,
  /// ★ Named from what the player sees: a young woman with brown hair
  /// (icon_53.png)
  PORTRAIT_WOMAN_WITH_PONYTAIL,
  /// ★ Named from what the player sees: a woman with red-brown hair
  /// (icon_54.png)
  PORTRAIT_WOMAN_IN_PINK_APRON,
  /// ★ Named from what the player sees: a man with dark hair and a full beard
  /// (icon_55.png)
  PORTRAIT_MAN_WITH_DARK_HAIR,
  PORTRAIT_ISAAC_CHILD,
  PORTRAIT_GARET_CHILD,
  /// ★ Named from what the player sees: Felix as a child (icon_58.png)
  PORTRAIT_FELIX_CHILD,
  /// ★ Named from what the player sees: Jenna as a child (icon_59.png)
  PORTRAIT_JENNA_CHILD,
  /// ★ Named from what the player sees: Felix behind his green mask
  /// (icon_60.png)
  PORTRAIT_FELIX_MASKED,
  /// ★ Named from what the player sees: Tret's wrathful face, with knotted
  /// brows and red eyes (icon_61.png)
  PORTRAIT_TRET_WRATHFUL,
  /// The row gPortrait_SpriteMap pairs with SPRITE_FELIX, drawn from
  /// icons.tsv's ICON_04 as PORTRAIT_FELIX is. ★ Named from what the player
  /// sees: Felix, his face unmasked (icon_04.png)
  PORTRAIT_FELIX_UNMASKED,
  /// ★ Named from what the player sees: Babi as a purple shape against red
  /// (icon_63.png)
  PORTRAIT_INVISIBLE_BABI,
  /// ★ Named from what the player sees: Faran, with fair hair and a moustache
  /// (icon_64.png)
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
