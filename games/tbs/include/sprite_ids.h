#ifndef ALCHEMY_SPRITE_IDS_H
#define ALCHEMY_SPRITE_IDS_H

/// @file
/// Sprite numbers, named where the code shows what they are. A sprite's
/// picture is graphics/sprite/spr_<number>.png, its number in decimal.

/// Sprite ids, one for each row of the sprite bank (gSprite_AnimationMetadata,
/// in sprites.tsv's order) in order; a row the code doesn't name yet keeps its
/// number as its placeholder. A party member's field sprite shares its
/// character number, so the field draws a character id as its sprite. The
/// summons' figures, from SPRITE_ATALANTA on, are each summon's own sprite:
/// its event creates its objects of it (BattleEffect_SpawnObjects), each
/// playing one of its animations.
enum SpriteId {
  /// ★ Named from what the player sees: Isaac (spr_000.png)
  SPRITE_ISAAC,
  /// ★ Named from what the player sees: Garet (spr_001.png)
  SPRITE_GARET,
  /// ★ Named from what the player sees: Ivan (spr_002.png)
  SPRITE_IVAN,
  /// ★ Named from what the player sees: Mia (spr_003.png)
  SPRITE_MIA,
  SPRITE_004,
  /// ★ Named from what the player sees: Jenna (spr_005.png)
  SPRITE_JENNA,
  /// ★ Named from what the player sees: Sheba (spr_006.png)
  SPRITE_SHEBA,
  SPRITE_007,
  SPRITE_008,
  SPRITE_009,
  SPRITE_010,
  SPRITE_011,
  SPRITE_012,
  SPRITE_013,
  SPRITE_014,
  SPRITE_015,
  SPRITE_016,
  /// Isaac's grown sprite while FLAG_ISAAC_WEARS_TUNIC is set.
  SPRITE_ISAAC_TUNIC,
  /// Isaac as a child, shown in his place while FLAG_CHILDHOOD is set.
  SPRITE_ISAAC_CHILD,
  /// Garet as a child, shown in his place while FLAG_CHILDHOOD is set.
  SPRITE_GARET_CHILD,
  /// A treasure chest, shut, opening and open, with the pictures of the mimic
  /// some chests turn out to be.
  /// ★ Named from what the player sees: a red treasure chest (its render)
  SPRITE_CHEST,
  SPRITE_021,
  /// A blank 16-pixel square: an actor that shows nothing, such as a
  /// shop
  /// counter, or that a map draws an item's icon on, as for the Nuts that
  /// Catch takes.
  SPRITE_BLANK,
  /// A blank 32-pixel square (spr_023.png holds no drawn pixel). Its one
  /// actor, an unused one in Bilibin, gives it no role, so it keeps
  /// its size as its name; SPRITE_BLANK_LARGE is the same size, used for
  /// actors seen only by their shadows.
  SPRITE_BLANK_32,
  /// Rings spreading out on water: where someone wades (the field's own
  /// FIELD_SPRITE_RIPPLE), and around the leader at Mercury Lighthouse's pools,
  /// grey in its first animation and blue in its second.
  /// ★ Named from what the player sees: rings spreading on water
  /// (spr_024.png)
  SPRITE_RIPPLE,
  SPRITE_025,
  /// The sparkles of Tret's glamour that rain on Kolima and its forest.
  /// ★ Named from what the player sees: rings of golden light (spr_026.png)
  SPRITE_GOLDEN_RINGS,
  SPRITE_027,
  SPRITE_028,
  SPRITE_029,
  /// The sprite of Kraden.
  /// ★ Named from what the player sees: an old man with white hair and a
  /// white beard in a brown robe (spr_030.png)
  SPRITE_KRADEN,
  /// The sprite of Dora, Isaac's mother.
  /// ★ Named from what the player sees: a woman with fair hair tied up, a
  /// blue top and a pink striped skirt (spr_031.png)
  SPRITE_DORA,
  /// The sprite of Saturos.
  /// ★ Named from what the player sees: a blue-skinned man with pale blue
  /// hair in a long blue cloak (spr_032.png)
  SPRITE_SATUROS,
  /// The sprite of Menardi.
  /// ★ Named from what the player sees: a woman with long fair hair in a red
  /// outfit (spr_033.png)
  SPRITE_MENARDI,
  /// The sprite of Felix behind his mask.
  /// ★ Named from what the player sees: a young man with brown hair, his face
  /// behind a green mask, in a green cloak (spr_034.png)
  SPRITE_FELIX_MASKED,
  /// The sprite of Felix as a young man.
  /// ★ Named from what the player sees: a young man with brown hair in a
  /// green cloak (spr_035.png)
  SPRITE_FELIX,
  /// Felix as a child.
  SPRITE_FELIX_CHILD,
  /// ★ Named from what the player sees: a small boy with spiky orange hair in
  /// a green shirt and shorts (spr_037.png)
  SPRITE_BOY_WITH_ORANGE_HAIR,
  /// ★ Named from what the player sees: a woman with orange hair under a
  /// green headscarf, in a blue top (spr_038.png)
  SPRITE_WOMAN_IN_GREEN_HEADSCARF,
  /// The sprite of Feizhi.
  /// ★ Named from what the player sees: a girl with purple hair tied up with
  /// a bow, in a yellow and blue dress (spr_039.png)
  SPRITE_FEIZHI,
  /// The sprite of Feh.
  /// ★ Named from what the player sees: a young man with black hair in a
  /// yellow and green shirt (spr_040.png)
  SPRITE_FEH,
  /// The sprite of Hsu.
  /// ★ Named from what the player sees: a young man with brown hair in an
  /// orange jacket and green trousers (spr_041.png)
  SPRITE_HSU,
  /// ★ Named from what the player sees: a young woman with an orange ponytail
  /// in a green and purple dress (spr_042.png)
  SPRITE_WOMAN_WITH_ORANGE_PONYTAIL,
  /// The sprite of Alex.
  /// ★ Named from what the player sees: a young man with long pale blue hair
  /// in a white and purple outfit (spr_043.png)
  SPRITE_ALEX,
  /// The sprite of Donpa.
  /// ★ Named from what the player sees: an old man with long grey hair in a
  /// yellow robe, with a staff (spr_044.png)
  SPRITE_DONPA,
  /// The sprite of Layana.
  /// ★ Named from what the player sees: a woman with long curly red hair in a
  /// green dress (spr_045.png)
  SPRITE_LAYANA,
  /// ★ Named from what the player sees: an old man with white hair and a
  /// white beard in green clothes (spr_046.png)
  SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN,
  /// The sprite of Kaja, the Tolbi ship's fighter.
  /// ★ Named from what the player sees: a bald man with a beard in a green
  /// apron (spr_047.png)
  SPRITE_KAJA,
  /// Jenna as a child.
  SPRITE_JENNA_CHILD,
  /// The sprite of Hammet.
  /// ★ Named from what the player sees: a man in a turban and orange and
  /// yellow robes (spr_049.png)
  SPRITE_HAMMET,
  /// The sprite of Dodonpa.
  /// ★ Named from what the player sees: a man with spiky dark blue hair and a
  /// moustache in a white top and a red cloak (spr_050.png)
  SPRITE_DODONPA,
  /// The sprite of Nyunpa.
  /// ★ Named from what the player sees: an old man with white hair and a
  /// white beard in an orange robe (spr_051.png)
  SPRITE_NYUNPA,
  /// The sprite of Babi.
  /// ★ Named from what the player sees: an old man with long pink hair in a
  /// red and gold robe, with a cane (spr_052.png)
  SPRITE_BABI,
  /// The sprite of Kyle.
  /// ★ Named from what the player sees: a man with fair hair in a blue and
  /// white top and green trousers (spr_053.png)
  SPRITE_KYLE,
  /// ★ Named from what the player sees: a man with brown hair in a teal shirt
  /// and brown trousers (spr_054.png)
  SPRITE_MAN_IN_TEAL_SHIRT,
  /// The sprite of Tret.
  /// ★ Named from what the player sees: a great stone-coloured face with a
  /// heavy brow (spr_055.png)
  SPRITE_TRET,
  /// The sprite of Laurel.
  /// ★ Named from what the player sees: an orange face with green eyes
  /// (spr_056.png)
  SPRITE_LAUREL,
  /// The sprite of Iodem.
  /// ★ Named from what the player sees: a man with grey hair and a grey beard
  /// in a blue tunic and a yellow cape (spr_057.png)
  SPRITE_IODEM,
  /// The sprite of McCoy.
  /// ★ Named from what the player sees: a stout man with red hair in a pink
  /// tunic (spr_058.png)
  SPRITE_MCCOY,
  /// ★ Named from what the player sees: a man with brown hair in a green
  /// shirt and blue trousers (spr_059.png)
  SPRITE_MAN_IN_GREEN_SHIRT,
  /// ★ Named from what the player sees: a woman with pink hair tied up, in a
  /// white apron and a blue skirt (spr_060.png)
  SPRITE_WOMAN_WITH_PINK_HAIR,
  /// The sprite of Hama.
  /// ★ Named from what the player sees: a woman with long purple hair in a
  /// white and green robe (spr_061.png)
  SPRITE_HAMA,
  /// ★ Named from what the player sees: a warrior in a golden helmet with a
  /// red crest (spr_062.png)
  SPRITE_WARRIOR_IN_GOLDEN_HELMET,
  /// ★ Named from what the player sees: a man with long dark hair in a green
  /// and orange outfit (spr_063.png)
  SPRITE_MAN_WITH_LONG_DARK_HAIR,
  /// ★ Named from what the player sees: a person with long dark purple hair
  /// in purple clothes (spr_064.png)
  SPRITE_PERSON_IN_PURPLE,
  /// Babi's second sheet, which Altmiller Cave places while he is invisible
  /// (ACTOR_INVISIBLE_BABI).
  /// ★ Named from what the player sees: an old man with long pink hair in a
  /// red and gold robe, with a cane (spr_065.png)
  SPRITE_INVISIBLE_BABI,
  /// ★ Named from what the player sees: a young man with spiky blue hair and
  /// a yellow headband in white (spr_066.png)
  SPRITE_YOUNG_MAN_WITH_BLUE_HAIR,
  /// ★ Named from what the player sees: a young man with spiky red hair and a
  /// yellow headband (spr_067.png)
  SPRITE_YOUNG_MAN_WITH_RED_HAIR,
  /// The sprite of Bunza.
  /// ★ Named from what the player sees: a man with dark hair combed back in a
  /// blue tunic (spr_068.png)
  SPRITE_BUNZA,
  /// ★ Named from what the player sees: a woman with curly purple hair in a
  /// pink and purple dress (spr_069.png)
  SPRITE_WOMAN_WITH_PURPLE_HAIR,
  /// The sprite of Faran.
  /// ★ Named from what the player sees: a man with long fair hair in an
  /// orange and brown robe (spr_070.png)
  SPRITE_FARAN,
  /// Tret's face again: SPRITE_TRET's pictures and animations drawn 12 pixels
  /// higher (sprites.tsv's SPR_071), as Kolima Forest shows him.
  SPRITE_TRET_RAISED,
  SPRITE_072,
  SPRITE_073,
  SPRITE_074,
  SPRITE_075,
  SPRITE_076,
  SPRITE_077,
  SPRITE_078,
  SPRITE_079,
  SPRITE_080,
  SPRITE_081,
  SPRITE_082,
  SPRITE_083,
  SPRITE_084,
  SPRITE_085,
  SPRITE_086,
  SPRITE_087,
  SPRITE_088,
  SPRITE_089,
  /// A Venus Djinni; gDjinn_ElementSprites gives each element's Djinni
  /// sprite.
  SPRITE_VENUS_DJINNI,
  /// A Mercury Djinni.
  SPRITE_MERCURY_DJINNI,
  /// A Mars Djinni.
  SPRITE_MARS_DJINNI,
  /// A Jupiter Djinni.
  SPRITE_JUPITER_DJINNI,
  SPRITE_094,
  SPRITE_095,
  SPRITE_096,
  SPRITE_097,
  SPRITE_098,
  SPRITE_099,
  SPRITE_100,
  /// ★ Named from what the player sees: a burly grey-haired man with bare
  /// arms folded, in a dark vest (spr_101.png)
  SPRITE_BURLY_MAN_WITH_GREY_HAIR,
  /// ★ Named from what the player sees: a little girl with orange pigtails in
  /// a green dress (spr_102.png)
  SPRITE_GIRL_WITH_PIGTAILS,
  /// ★ Named from what the player sees: a bent old man with a bald head and a
  /// white beard in a green robe, with a cane (spr_103.png)
  SPRITE_OLD_MAN_WITH_CANE,
  /// ★ Named from what the player sees: a small white dog with brown ears
  /// (spr_104.png)
  SPRITE_DOG,
  /// ★ Named from what the player sees: a tree with a full green crown
  /// (spr_105.png)
  SPRITE_TREE,
  /// ★ Named from what the player sees: a woman with long fair hair and a
  /// blue headband in a white apron (spr_106.png)
  SPRITE_WOMAN_WITH_FAIR_HAIR,
  /// ★ Named from what the player sees: a small boy with fair hair in a blue
  /// shirt (spr_107.png)
  SPRITE_BOY_WITH_FAIR_HAIR,
  /// ★ Named from what the player sees: a young man with brown hair in a purple
  /// and white shirt (spr_108.png)
  SPRITE_YOUNG_MAN_WITH_BROWN_HAIR,
  /// ★ Named from what the player sees: a stout man with brown hair in an
  /// orange and blue tunic (spr_109.png)
  SPRITE_STOUT_MAN,
  /// ★ Named from what the player sees: a small tree (spr_110.png)
  SPRITE_SMALL_TREE,
  /// ★ Named from what the player sees: an old woman with white hair in a
  /// grey dress (spr_111.png)
  SPRITE_OLD_WOMAN,
  /// ★ Named from what the player sees: an old man with a white beard in a
  /// blue cap and a blue robe (spr_112.png)
  SPRITE_OLD_MAN_IN_BLUE_CAP,
  /// ★ Named from what the player sees: a man with fair hair and glasses in a
  /// blue cap and a red and blue robe (spr_113.png)
  SPRITE_MAN_WITH_GLASSES,
  /// ★ Named from what the player sees: a bald, muscular man, bare-chested,
  /// in green trousers (spr_114.png)
  SPRITE_BALD_MUSCULAR_MAN,
  /// ★ Named from what the player sees: a woman with brown hair in a green
  /// dress and a white apron (spr_115.png)
  SPRITE_WOMAN_IN_GREEN_DRESS,
  /// ★ Named from what the player sees: a stocky, muscular man with black
  /// hair in a red top, flexing his arms (spr_116.png)
  SPRITE_STRONGMAN,
  /// ★ Named from what the player sees: an old man with white hair in a pale
  /// robe, with a cane (spr_117.png)
  SPRITE_OLD_MAN_WITH_WHITE_HAIR,
  /// ★ Named from what the player sees: a young woman with a brown ponytail
  /// in a white top and a long skirt (spr_118.png)
  SPRITE_WOMAN_WITH_PONYTAIL,
  /// ★ Named from what the player sees: a man with dark hair in a dark blue
  /// tunic (spr_119.png)
  SPRITE_MAN_WITH_DARK_HAIR,
  /// ★ Named from what the player sees: a woman with brown hair in a long
  /// blue dress (spr_120.png)
  SPRITE_WOMAN_IN_BLUE_DRESS,
  /// ★ Named from what the player sees: a man with a white beard in a tall
  /// hat and a long purple and blue robe (spr_121.png)
  SPRITE_MAN_IN_TALL_HAT,
  /// ★ Named from what the player sees: a man with long brown hair and a
  /// beard in a red shawl and a long lavender robe (spr_122.png)
  SPRITE_BEARDED_MAN_IN_LONG_ROBE,
  /// ★ Named from what the player sees: a man in a white cook's cap and white
  /// clothes, with a frying pan (spr_123.png)
  SPRITE_COOK,
  /// ★ Named from what the player sees: a bald man in a red tunic with green
  /// sleeves (spr_124.png)
  SPRITE_BALD_MAN_IN_RED,
  /// ★ Named from what the player sees: a young man with brown hair in a
  /// green shirt (spr_125.png)
  SPRITE_YOUNG_MAN_IN_GREEN,
  /// A second number for SPRITE_BALD_MAN_IN_RED, whose frames and animations
  /// it borrows (sprites.tsv), so the player sees the same man. The first
  /// number keeps most towns' weapon shops; this one stands in Kalay's houses
  /// and in Hammet's palace.
  /// ★ Named from what the player sees: a bald man in a red tunic with green
  /// sleeves (spr_124.png)
  SPRITE_BALD_MAN_IN_RED_KALAY,
  /// A second number for SPRITE_YOUNG_MAN_IN_GREEN, whose frames and
  /// animations it borrows (sprites.tsv), so the player sees the same man. The
  /// first number keeps most towns' armor shops; this one stands in Kalay's
  /// houses and in Hammet's palace.
  /// ★ Named from what the player sees: a young man with brown hair in a
  /// green shirt (spr_125.png)
  SPRITE_YOUNG_MAN_IN_GREEN_KALAY,
  /// ★ Named from what the player sees: a woman with red-brown hair in a pink
  /// dress and a white apron (spr_128.png)
  SPRITE_WOMAN_IN_PINK_APRON,
  /// ★ Named from what the player sees: a man with fair hair in a blue
  /// feathered cap, holding a lute (spr_129.png)
  SPRITE_MAN_WITH_LUTE,
  /// ★ Named from what the player sees: a young man with brown hair and a
  /// white headband in a white top and blue trousers (spr_130.png)
  SPRITE_YOUTH_WITH_HEADBAND,
  /// ★ Named from what the player sees: a young man with brown hair tied back
  /// in a white top and blue trousers (spr_131.png)
  SPRITE_YOUTH_WITH_TIED_HAIR,
  /// ★ Named from what the player sees: a man in a tall white chef's hat and
  /// white clothes with a red scarf, with a frying pan (spr_132.png)
  SPRITE_CHEF,
  /// ★ Named from what the player sees: a man with a mop of orange hair in a
  /// yellow vest and green trousers (spr_133.png)
  SPRITE_MAN_WITH_ORANGE_HAIR,
  /// ★ Named from what the player sees: a man with golden-brown hair in
  /// orange and red clothes (spr_134.png)
  SPRITE_MAN_IN_ORANGE,
  /// ★ Named from what the player sees: a man with black hair and a white
  /// headband in an orange top, with a pickaxe (spr_135.png)
  SPRITE_MAN_WITH_PICKAXE,
  /// ★ Named from what the player sees: a small white and grey cat
  /// (spr_136.png)
  SPRITE_CAT,
  SPRITE_137,
  SPRITE_138,
  SPRITE_139,
  SPRITE_140,
  SPRITE_141,
  SPRITE_142,
  /// ★ Named from what the player sees: a man with a beard in a white turban
  /// and long white robes (spr_143.png)
  SPRITE_MAN_IN_WHITE_TURBAN,
  /// ★ Named from what the player sees: a man with tanned skin in a red
  /// bandana and white trousers (spr_144.png)
  SPRITE_MAN_IN_RED_BANDANA,
  /// ★ Named from what the player sees: a man with tanned skin in a blue
  /// bandana and white trousers (spr_145.png)
  SPRITE_MAN_IN_BLUE_BANDANA,
  /// ★ Named from what the player sees: a man with tanned skin in a yellow
  /// bandana and white trousers (spr_146.png)
  SPRITE_MAN_IN_YELLOW_BANDANA,
  /// ★ Named from what the player sees: a man with tanned skin in a green
  /// bandana and a red vest (spr_147.png)
  SPRITE_MAN_IN_GREEN_BANDANA,
  /// ★ Named from what the player sees: a young woman with long brown hair in
  /// a green dress (spr_148.png)
  SPRITE_YOUNG_WOMAN_IN_GREEN,
  /// ★ Named from what the player sees: a bald monk in a white and orange
  /// robe (spr_149.png)
  SPRITE_MONK_IN_WHITE_AND_ORANGE,
  /// ★ Named from what the player sees: a soldier in a spiked steel helmet,
  /// with a spear (spr_150.png)
  SPRITE_SOLDIER_IN_SPIKED_HELMET,
  /// ★ Named from what the player sees: a small bald boy in an orange robe
  /// (spr_151.png)
  SPRITE_BALD_BOY_IN_ORANGE,
  /// The soldiers of Babi's Tolbi and of the Colosseum.
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  SPRITE_SOLDIER,
  /// ★ Named from what the player sees: a soldier in a white turban, with a
  /// spear (spr_153.png)
  SPRITE_SOLDIER_IN_TURBAN,
  /// ★ Named from what the player sees: a covered wagon drawn by a horse
  /// (spr_154.png)
  SPRITE_WAGON,
  /// ★ Named from what the player sees: a white bird with outspread wings
  /// (spr_155.png)
  SPRITE_BIRD,
  /// ★ Named from what the player sees: a young man with brown hair tied back
  /// in a blue jacket and a white apron (spr_156.png)
  SPRITE_YOUNG_MAN_IN_BLUE_JACKET,
  /// ★ Named from what the player sees: a person with long brown hair and a
  /// green scarf in a red top, with a sword (spr_157.png)
  SPRITE_PERSON_WITH_SWORD,
  /// ★ Named from what the player sees: a bald monk in an orange robe
  /// (spr_158.png)
  SPRITE_MONK_IN_ORANGE_ROBE,
  /// ★ Named from what the player sees: a young woman with black hair and a
  /// red hair ornament in a red and yellow dress (spr_159.png)
  SPRITE_WOMAN_WITH_BLACK_HAIR,
  /// ★ Named from what the player sees: a man with black hair in a topknot in
  /// an orange and green robe (spr_160.png)
  SPRITE_MAN_WITH_TOPKNOT,
  /// ★ Named from what the player sees: a ship with green sails (spr_161.png)
  SPRITE_SHIP,
  /// ★ Named from what the player sees: a woman with red-brown hair in a
  /// white top and a green apron (spr_162.png)
  SPRITE_WOMAN_IN_GREEN_APRON,
  /// ★ Named from what the player sees: a soldier in a steel helmet, with a
  /// torch (spr_163.png)
  SPRITE_SOLDIER_WITH_TORCH,
  /// ★ Named from what the player sees: an old man with grey hair in a
  /// topknot in blue and orange clothes (spr_164.png)
  SPRITE_OLD_MAN_IN_BLUE,
  /// ★ Named from what the player sees: a young man with fair hair in a white
  /// and orange tunic (spr_165.png)
  SPRITE_YOUNG_MAN_WITH_FAIR_HAIR,
  /// ★ Named from what the player sees: an old woman with white hair in a
  /// blue and red dress, with a cane (spr_166.png)
  SPRITE_OLD_WOMAN_WITH_CANE,
  /// ★ Named from what the player sees: a man with grey hair in a black suit
  /// (spr_167.png)
  SPRITE_MAN_IN_BLACK_SUIT,
  /// ★ Named from what the player sees: a small boy with black hair in a bun
  /// in green clothes (spr_168.png)
  SPRITE_BOY_IN_GREEN,
  /// ★ Named from what the player sees: a little girl with brown hair in a
  /// pink top (spr_169.png)
  SPRITE_GIRL_IN_PINK,
  /// ★ Named from what the player sees: a bald, muscular man with a black
  /// topknot, bare-chested (spr_170.png)
  SPRITE_MUSCULAR_MAN_WITH_TOPKNOT,
  /// ★ Named from what the player sees: a man in a green cap and a green and
  /// yellow robe, carrying a pack on his back (spr_171.png)
  SPRITE_MAN_CARRYING_A_PACK,
  SPRITE_172,
  SPRITE_173,
  SPRITE_174,
  SPRITE_175,
  SPRITE_176,
  SPRITE_177,
  SPRITE_178,
  SPRITE_179,
  SPRITE_180,
  SPRITE_181,
  SPRITE_182,
  SPRITE_183,
  SPRITE_184,
  SPRITE_185,
  SPRITE_186,
  SPRITE_187,
  /// The Tolbi-bound Ship's oars and the sea monsters that attack it.
  /// ★ Named from what the player sees: a wooden oar held at rest, slanting
  /// (its render)
  SPRITE_RESTING_OAR,
  /// ★ Named from what the player sees: a blue jellyfish monster with
  /// trailing tentacles (its render)
  SPRITE_JELLYFISH_MONSTER,
  /// The water beasts that flood Altin and its mine, whose battles name them
  /// Living Statues.
  /// ★ Named from the game: "Are you the ones who defeated the water beasts?"
  /// (MsgAltinOnesWhoDefeatedWaterBeasts)
  SPRITE_WATER_BEAST,
  /// The apes of Mogall Forest, which Force brings out of their hiding places.
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  SPRITE_APE,
  /// ★ Named from the game: "Lizard Man" (MsgEnemyName)
  SPRITE_LIZARD_MAN,
  SPRITE_193,
  /// The Kraken, which the party fights on the ship's deck.
  /// ★ Named from the game: "You did good against that Kraken."
  /// (MsgTolbiShipThankIsaacDidGoodAgainst)
  SPRITE_KRAKEN,
  SPRITE_195,
  /// ★ Named from what the player sees: a pink winged monster (its render)
  SPRITE_PINK_WINGED_MONSTER,
  /// ★ Named from what the player sees: a blue winged monster with claws (its
  /// render)
  SPRITE_BLUE_WINGED_MONSTER,
  /// ★ Named from what the player sees: a green lizard-like monster (its
  /// render)
  SPRITE_LIZARD_MONSTER,
  SPRITE_199,
  /// A big round rock: the rocks that fall across Vale's paths on the stormy
  /// night, and the Boulder of Mt. Aleph drawn at twice its size.
  /// ★ Named from the game: "It's a huge rock" (MsgValeRockCannotBeMoved)
  SPRITE_ROCK,
  /// The statues of Sol Sanctum's statue room and Luna and Sol rooms.
  /// ★ Named from what the player sees: a winged statue holding a blue orb
  /// (its sprite sheet, spr_201.png)
  SPRITE_WINGED_STATUE,
  /// The centre statue of Sol Sanctum's statue room.
  /// ★ Named from what the player sees: a winged statue bearing a sun disc on
  /// its head (its sprite sheet, spr_202.png)
  SPRITE_SUN_DISC_STATUE,
  /// ★ Named from what the player sees: a carved stone statue of a figure,
  /// as Sol Sanctum's statues stand on its seals (its render)
  SPRITE_STONE_STATUE,
  /// The centre statue once it has sunk into the floor, only its disc showing.
  /// ★ Named from what the player sees: a round sun disc lying flat (its
  /// sprite sheet, spr_204.png)
  SPRITE_SUN_DISC,
  /// A log lying north to south, one the leader can hop onto
  /// (Field_TryJumpForward) or roll, as in Kolima Forest.
  /// ★ Named from what the player sees: a log lying north to south (its
  /// render)
  SPRITE_LOG_NORTH_SOUTH,
  SPRITE_206,
  /// ★ Named from what the player sees: a log lying west to east (its render)
  SPRITE_LOG_WEST_EAST,
  SPRITE_208,
  /// The effect FieldEffect_SetAttached attaches to an actor
  /// or shows
  /// once, as the leader flinches on hazard ground.
  SPRITE_ATTACHED_EFFECT,
  /// ★ Named from the game: "chest of valuables" (MsgValeSChestValuables)
  SPRITE_CHEST_OF_VALUABLES,
  /// What Move pushes in Goma Cave and many rooms after it, which the game
  /// calls a stone pillar ("It's just a stone pillar.").
  /// ★ Named from what the player sees: a tall stone pillar (its render)
  SPRITE_STONE_PILLAR,
  /// ★ Named from what the player sees: a tree stump (its render)
  SPRITE_STUMP,
  SPRITE_213,
  /// Spray thrown up where water is struck: where Goma Cave's stumps land,
  /// at Kolima Forest's floodgate, and over the Tolbi-bound Ship's side.
  /// ★ Named from what the player sees: a splash of white and pale blue
  /// spray (spr_214.png)
  SPRITE_SPLASH,
  /// The hand Move reaches out with; Goma Cave's entrance and Vale place it
  /// to push something in an event.
  /// ★ Named from what the player sees: a large open hand (spr_215.png)
  SPRITE_MOVE_HAND,
  SPRITE_216,
  SPRITE_217,
  SPRITE_218,
  SPRITE_219,
  /// The fields of Psynergy that rise over the party as Kolima's curse falls.
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  SPRITE_PSYNERGY_FIELD,
  SPRITE_221,
  /// A puff of dust kicked up where an actor stands; the water beasts of
  /// Altin
  /// and its mine spit it as spray, drawn SPRITE_COLOR_BGR or
  /// SPRITE_COLOR_WHITE.
  SPRITE_DUST_PUFF,
  /// ★ Named from what the player sees: a wooden crate (spr_223.png)
  SPRITE_CRATE,
  /// The statues of Mercury Lighthouse.
  /// ★ Named from what the player sees: a blue statue of a woman holding a
  /// jar (its render)
  SPRITE_BLUE_STATUE,
  /// ★ Named from what the player sees: an oar dipping into the water with a
  /// splash (its render)
  SPRITE_OAR,
  /// Isaac's tunic: placed beside his bed on the stormy night, and drawn as a
  /// second layer over the child sprites of Isaac and Garet while
  /// FLAG_ISAAC_WEARS_TUNIC is set (Actor_RegisterSpriteMetadata).
  /// ★ Named from the game: "Don't go out without your tunic!"
  /// (MsgValeComeOnHurry)
  SPRITE_TUNIC,
  /// A puddle of water, which Frost freezes into a pillar of ice.
  /// ★ Named from what the player sees: a puddle of water (its render)
  SPRITE_PUDDLE,
  /// The stone pipe sections Move pushes in Mercury Lighthouse to join its
  /// channels, and the short upright pipes beside them.
  /// ★ Named from what the player sees: a stone pipe section lying west to
  /// east (spr_228.png)
  SPRITE_PIPE_WEST_EAST,
  /// The pipe sections Move pushes in Mercury Lighthouse, also placed in the
  /// Lighthouse Aerie of Venus Lighthouse.
  /// ★ Named from what the player sees: a stone pipe section lying north to
  /// south (spr_229.png)
  SPRITE_PIPE_NORTH_SOUTH,
  /// The rim of an upright pipe's open end, drawn over the end a pushed pipe
  /// section comes to rest against.
  /// ★ Named from what the player sees: the rim at the top of an upright pipe
  /// (spr_230.png; places/mercury-lighthouse-lower/map-4.png)
  SPRITE_PIPE_END,
  /// The carts on the rails of Altin's mine.
  /// ★ Named from what the player sees: a wooden mine cart on iron wheels
  /// (spr_231.png)
  SPRITE_MINE_CART,
  SPRITE_232,
  SPRITE_233,
  /// ★ Named from what the player sees: a snowman with a red cap and a green
  /// scarf (spr_234.png)
  SPRITE_SNOWMAN,
  SPRITE_235,
  /// The logs that float on the water of Fuchin Falls Cave, Crossbone Isle's
  /// floating logs room and the Colosso's Lumber Water Crossing Area.
  /// ★ Named from what the player sees: a yellow log floating west to east
  /// (its render)
  SPRITE_FLOATING_LOG_WEST_EAST,
  /// ★ Named from what the player sees: a yellow log floating north to south
  /// (its render)
  SPRITE_FLOATING_LOG_NORTH_SOUTH,
  /// The sprout Growth grows.
  /// ★ Named from what the player sees: a green sprout (its render)
  SPRITE_SPROUT,
  SPRITE_239,
  SPRITE_240,
  /// ★ Named from what the player sees: a stack of two wooden crates (its
  /// render)
  SPRITE_CRATES,
  /// The platforms that float in Mercury Lighthouse's aerie and that carry the
  /// leader up to Venus Lighthouse's.
  /// ★ Named from what the player sees: a round floating platform (its
  /// render)
  SPRITE_FLOATING_PLATFORM,
  /// The statues Move or a push slides in Bilibin and its Treasury.
  /// ★ Named from what the player sees: a golden statue of a figure in a long
  /// robe (spr_243.png)
  SPRITE_GOLDEN_STATUE,
  /// Mercury Lighthouse's lit beacon, and the orb at the top of Venus
  /// Lighthouse.
  /// ★ Named from what the player sees: a blue orb with white swirls, turning
  /// (spr_244.png)
  SPRITE_BLUE_ORB,
  /// The keys the scenes show: the barricade key McCoy tosses to the party,
  /// and Lunpa Fortress's Cell Key.
  /// ★ Named from what the player sees: a golden key (spr_245.png)
  SPRITE_KEY,
  /// ★ Named from what the player sees: an unlit torch on its stand (its
  /// render)
  SPRITE_TORCH,
  /// A fire burning on the ground, which Douse puts out.
  /// ★ Named from what the player sees: a flickering flame (spr_247.png)
  SPRITE_FIRE,
  /// The ivy Whirlwind blows away, in Goma Cave, Vault, Bilibin, Lunpa
  /// Fortress and Crossbone Isle, and a patch outside McCoy's Palace.
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  SPRITE_IVY,
  SPRITE_249,
  SPRITE_250,
  SPRITE_251,
  SPRITE_252,
  /// The pillars Move shifts in the Tunnel Ruins and the lighthouses.
  /// ★ Named from what the player sees: a stone pillar with a glowing yellow
  /// cap (its render)
  SPRITE_CAPPED_PILLAR,
  SPRITE_254,
  SPRITE_255,
  /// The pillars Move, Carry and pushes shift in Venus Lighthouse and on the
  /// Road to Babi Lighthouse. Animations 1 and 2 show a grey and an orange
  /// spiral on top; 3 to 8 a piece of gold track instead, along which Venus
  /// Lighthouse's lights run.
  /// ★ Named from what the player sees: a grey pillar with a spiral carved on
  /// top (spr_256.png)
  SPRITE_SPIRAL_PILLAR,
  /// The platforms of Venus Lighthouse and the Tunnel Ruins that rise or sink
  /// with the weight set down on them.
  /// ★ Named from what the player sees: a small white pillar on its base
  /// (its render)
  SPRITE_BALANCE_PLATFORM,
  /// The four statues around Venus Lighthouse's beacon.
  /// ★ Named from what the player sees: a statue of a woman with a green
  /// wreath, holding a sheaf (spr_258.png)
  SPRITE_STATUE_WITH_WREATH,
  SPRITE_259,
  SPRITE_260,
  /// ★ Named from what the player sees: houses with orange roofs among trees
  /// (spr_261.png)
  SPRITE_WORLD_MAP_VILLAGE,
  SPRITE_262,
  /// ★ Named from what the player sees: white and blue buildings inside a wall
  /// (spr_263.png)
  SPRITE_WORLD_MAP_TOWN,
  /// ★ Named from what the player sees: houses built into great tree stumps
  /// (spr_264.png)
  SPRITE_WORLD_MAP_STUMP_VILLAGE,
  /// ★ Named from what the player sees: houses under snow among snowy trees
  /// (spr_265.png)
  SPRITE_WORLD_MAP_SNOWY_VILLAGE,
  /// ★ Named from what the player sees: buildings with blue roofs among trees
  /// (spr_266.png)
  SPRITE_WORLD_MAP_BLUE_ROOFED_TOWN,
  /// ★ Named from what the player sees: a temple with a blue roof by a
  /// waterfall (spr_267.png)
  SPRITE_WORLD_MAP_TEMPLE,
  /// ★ Named from what the player sees: a grey rock arch over a cave mouth
  /// (spr_268.png)
  SPRITE_WORLD_MAP_CAVE,
  /// ★ Named from what the player sees: a tall blue-green tower, where Mercury
  /// Lighthouse's exit is (spr_269.png)
  SPRITE_WORLD_MAP_MERCURY_LIGHTHOUSE,
  /// ★ Named from what the player sees: a wooden barricade (spr_270.png)
  SPRITE_WORLD_MAP_BARRICADE,
  /// ★ Named from what the player sees: a snow-capped mountain (spr_271.png)
  SPRITE_WORLD_MAP_MOUNTAIN,
  /// ★ Named from what the player sees: a pink and white tower, where Babi
  /// Lighthouse's road begins (spr_272.png)
  SPRITE_WORLD_MAP_BABI_LIGHTHOUSE,
  /// SPRITE_WAGON drawn at 64 pixels, its back loaded with yellow crates:
  /// Bunza's wagon at the Kalay Tunnel Entrance.
  /// ★ Named from what the player sees: a large covered wagon drawn by a
  /// horse, full of crates (spr_273.png)
  SPRITE_LARGE_WAGON,
  /// The signs of Lalivero, Tolbi Spring, Altin Peak and Kolima Forest's
  /// floodgate, and the signposts on the world map.
  /// ★ Named from what the player sees: a wooden sign on a post (its render)
  SPRITE_SIGNPOST,
  /// ★ Named from what the player sees: a clump of tall green trees
  /// (spr_275.png)
  SPRITE_WORLD_MAP_FOREST,
  /// ★ Named from what the player sees: a tall purple and white tower, where
  /// Venus Lighthouse stands (spr_276.png)
  SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE,
  /// ★ Named from what the player sees: a pier with a boat moored at it
  /// (spr_277.png)
  SPRITE_WORLD_MAP_DOCK,
  /// A drawbridge over a river: raised in its first animation, let down
  /// across the river in its second.
  /// ★ Named from what the player sees: a drawbridge raised between its posts,
  /// then lying across the river (spr_278.png)
  SPRITE_WORLD_MAP_BRIDGE,
  /// ★ Named from what the player sees: a grey gatehouse between green walls
  /// (spr_279.png)
  SPRITE_WORLD_MAP_GATEHOUSE,
  SPRITE_280,
  SPRITE_281,
  SPRITE_282,
  SPRITE_283,
  /// The particles of Growth and of a Djinni's capture, and the gate effects
  /// of Venus Lighthouse.
  SPRITE_PARTICLE,
  /// One sparkle: the glints of Halt and of the field's sparkles, and Kolima's
  /// curse dust.
  /// ★ Named from what the player sees: a sparkle of light that shrinks away
  /// (its render)
  SPRITE_SPARKLE,
  /// ★ Named from what the player sees: a four-pointed glint that shrinks
  /// to a dot (spr_286.png)
  SPRITE_GLINT,
  /// The lights that float about the Elemental Star Room.
  /// ★ Named from what the player sees: a small glowing blue-green light (its
  /// sprite sheet, spr_287.png)
  SPRITE_FLOATING_LIGHT,
  /// A blank 32-pixel square: an actor seen only by its shadow, such as
  /// the
  /// shadows under Altin Peak's falling boulder and its Hydros statue.
  /// ★ Named from what the player sees: nothing, every pixel transparent
  /// (spr_288.png)
  SPRITE_BLANK_LARGE,
  /// The whirlwinds of the Suhalla Desert, which Douse stops, and the one
  /// drawn on the world map beside it.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  SPRITE_SANDSTORM,
  SPRITE_290,
  SPRITE_291,
  /// The lights of Altmiller Cave's rock puzzle, coloured by their palette.
  /// ★ Named from the game: "Lights will appear on the wall."
  /// (MsgAltmillerDontTrustAnyone)
  SPRITE_LIGHT,
  /// The rocks of Altmiller Cave's rock puzzle, coloured by their palette
  /// while Reveal shines.
  /// ★ Named from the game: "Turn the five rocks in accordance with those
  /// colors." (MsgAltmillerDontTrustAnyone)
  SPRITE_TURNING_ROCK,
  SPRITE_294,
  /// The boulders of Gondowan Cave that Lift raises.
  /// ★ Named from what the player sees: a round grey boulder (its render)
  SPRITE_ROUND_BOULDER,
  /// The two halves of Babi Lighthouse's stone door, which slide apart.
  /// ★ Named from what the player sees: half of a dark stone door (its
  /// render)
  SPRITE_STONE_DOOR_HALF,
  /// What the party pushes in Altmiller Cave's lit rooms.
  /// ★ Named from what the player sees: a white stone column lying north to
  /// south (its render)
  SPRITE_FALLEN_COLUMN_NORTH_SOUTH,
  /// ★ Named from what the player sees: a white stone column lying west to
  /// east (its render)
  SPRITE_FALLEN_COLUMN_WEST_EAST,
  SPRITE_299,
  /// Isaac in battle. The party members' battle sprites follow in character
  /// order, Felix's row left empty.
  /// ★ Named from what the player sees: Isaac in battle (spr_300.png)
  SPRITE_BATTLE_ISAAC,
  /// ★ Named from what the player sees: Garet in battle (spr_301.png)
  SPRITE_BATTLE_GARET,
  /// ★ Named from what the player sees: Ivan in battle (spr_302.png)
  SPRITE_BATTLE_IVAN,
  /// ★ Named from what the player sees: Mia in battle (spr_303.png)
  SPRITE_BATTLE_MIA,
  SPRITE_304,
  /// ★ Named from what the player sees: Jenna in battle (spr_305.png)
  SPRITE_BATTLE_JENNA,
  SPRITE_306,
  SPRITE_307,
  SPRITE_308,
  SPRITE_309,
  SPRITE_310,
  SPRITE_311,
  SPRITE_312,
  SPRITE_313,
  SPRITE_314,
  SPRITE_315,
  SPRITE_316,
  SPRITE_317,
  SPRITE_318,
  SPRITE_319,
  SPRITE_320,
  SPRITE_321,
  SPRITE_322,
  SPRITE_323,
  SPRITE_324,
  SPRITE_325,
  SPRITE_326,
  SPRITE_327,
  SPRITE_328,
  SPRITE_329,
  SPRITE_330,
  SPRITE_331,
  SPRITE_332,
  SPRITE_333,
  SPRITE_334,
  SPRITE_335,
  SPRITE_336,
  SPRITE_337,
  SPRITE_338,
  SPRITE_339,
  SPRITE_340,
  SPRITE_341,
  SPRITE_342,
  SPRITE_343,
  SPRITE_344,
  SPRITE_345,
  SPRITE_346,
  SPRITE_347,
  SPRITE_348,
  SPRITE_349,
  SPRITE_350,
  SPRITE_351,
  SPRITE_352,
  SPRITE_353,
  SPRITE_354,
  SPRITE_355,
  SPRITE_356,
  SPRITE_357,
  SPRITE_358,
  SPRITE_359,
  SPRITE_360,
  SPRITE_361,
  SPRITE_362,
  SPRITE_363,
  SPRITE_364,
  SPRITE_365,
  SPRITE_366,
  SPRITE_367,
  SPRITE_368,
  SPRITE_369,
  /// ★ Named from the game: "Atalanta" (MsgDjinnName)
  SPRITE_ATALANTA,
  /// ★ Named from the game: "Thor" (MsgDjinnName)
  SPRITE_THOR,
  /// ★ Named from the game: "Boreas" (MsgDjinnName)
  SPRITE_BOREAS,
  /// ★ Named from the game: "Kirin" (MsgDjinnName)
  SPRITE_KIRIN,
  SPRITE_374,
  /// ★ Named from the game: "Cybele" (MsgDjinnName)
  SPRITE_CYBELE,
  /// ★ Named from the game: "Procne" (MsgDjinnName)
  SPRITE_PROCNE,
  /// ★ Named from the game: "Tiamat" (MsgDjinnName)
  SPRITE_TIAMAT,
  /// ★ Named from the game: "Neptune" (MsgDjinnName)
  SPRITE_NEPTUNE,
  /// ★ Named from the game: "Ramses" (MsgDjinnName)
  SPRITE_RAMSES,
  /// ★ Named from the game: "Nereid" (MsgDjinnName)
  SPRITE_NEREID,
  /// ★ Named from the game: "Meteor" (MsgDjinnName)
  SPRITE_METEOR,
  /// ★ Named from the game: "Judgment" (MsgDjinnName)
  SPRITE_JUDGMENT,
  SPRITE_383,
  SPRITE_384,
  SPRITE_385,
  SPRITE_386,
  SPRITE_387,
  SPRITE_388,
  SPRITE_389,
  /// The rocks Gaia throws up, Ramses' rocks and Judgment's debris, in three
  /// animations.
  /// ★ Named from what the player sees: three orange rocks (spr_390.png)
  SPRITE_BATTLE_ROCKS,
  SPRITE_391,
  SPRITE_392,
  SPRITE_393,
  SPRITE_394,
  SPRITE_395,
  /// One of the two kinds of obstacle that move in Tolbi Spring while a token
  /// flies over it.
  /// ★ Named from what the player sees: a red crab (spr_396.png)
  SPRITE_CRAB,
  /// The other kind of Tolbi Spring obstacle.
  /// ★ Named from what the player sees: a green turtle with an orange shell
  /// (spr_397.png)
  SPRITE_TURTLE,
  /// The coin tossed into Tolbi Spring, spinning, with its shadow and its
  /// sparkle when it settles; the toss draws it when the player tosses a coin.
  /// ★ Named from the game: "Face away from Tolbi Spring and toss a coin into
  /// it." (MsgTolbiFaceAwayTolbi)
  SPRITE_SPRING_COIN,
  /// The Lucky Medal tossed into Tolbi Spring, drawn as SPRITE_SPRING_COIN in
  /// gold.
  /// ★ Named from the game: "If you toss a Lucky Medal"
  /// (MsgTolbiTossLuckyMedal)
  SPRITE_SPRING_LUCKY_MEDAL,
  SPRITE_400,
  SPRITE_401,
  SPRITE_402,
  SPRITE_403,
  SPRITE_404,
  SPRITE_405,
  SPRITE_406,
  SPRITE_407,
  SPRITE_408,
  SPRITE_409,
  SPRITE_410,
  SPRITE_411,
  SPRITE_412,
  SPRITE_413,
  SPRITE_414,
  SPRITE_415,
  SPRITE_416,
  SPRITE_417,
  SPRITE_418,
  SPRITE_419,
  SPRITE_420,
  SPRITE_421,
  SPRITE_422,
  SPRITE_423,
  SPRITE_424,
  SPRITE_425,
  SPRITE_426,
  SPRITE_427,
  SPRITE_428,
  SPRITE_429,
  SPRITE_430,
  SPRITE_431,
  SPRITE_432,
  SPRITE_433,
  SPRITE_434,
  SPRITE_435,
  SPRITE_436,
  SPRITE_437,
  SPRITE_438,
  SPRITE_439,
  SPRITE_440,
  SPRITE_441,
  SPRITE_442,
  SPRITE_443,
  SPRITE_444,
  SPRITE_445,
  SPRITE_446,
  SPRITE_447,
  SPRITE_448,
  SPRITE_449,
  SPRITE_450,
  SPRITE_451,
  SPRITE_452,
  SPRITE_453,
  SPRITE_454,
  SPRITE_455,
  SPRITE_456,
  SPRITE_457,
  SPRITE_458,
  SPRITE_459,
  SPRITE_460,
  SPRITE_461,
  SPRITE_462,
  SPRITE_463,
  SPRITE_464,
  SPRITE_465,
  SPRITE_466,
  SPRITE_467,
  SPRITE_468,
  SPRITE_469,
  SPRITE_470,
  SPRITE_471,
  SPRITE_472,
  SPRITE_473,
  SPRITE_474,
  SPRITE_475,
  SPRITE_476,
  SPRITE_477,
  SPRITE_478,
  SPRITE_479,
  /// Isaac's battle sprite while he holds a Machete, drawn as part of it.
  SPRITE_ISAAC_MACHETE,
  SPRITE_481,
  /// Garet's battle sprite while he holds a Machete, drawn as part of it.
  SPRITE_GARET_MACHETE,
  SPRITE_483,
  SPRITE_484,
  SPRITE_485,
  SPRITE_486,
  SPRITE_487,
  SPRITE_488,
  SPRITE_489,
  SPRITE_490,
  SPRITE_491,
  SPRITE_492,
  SPRITE_493,
  SPRITE_494,
  SPRITE_495,
  SPRITE_496,
  SPRITE_497,
  SPRITE_498,
  SPRITE_499,
  /// The statues that chase the leader in Crossbone Isle, and those of the
  /// Tunnel Ruins and Venus Lighthouse.
  /// ★ Named from what the player sees: a statue of a woman in a long robe
  /// (its render)
  SPRITE_LADY_STATUE,
  /// ★ Named from what the player sees: the wreck of a ship lying in the water
  /// (spr_501.png)
  SPRITE_SHIPWRECK,
  /// ★ Named from what the player sees: a wooden gangway with rails, laid from
  /// the wharf over Babi's ship (spr_502.png)
  SPRITE_GANGWAY,
  /// ★ Named from what the player sees: a drop of water that swells and bursts
  /// into a ring of spray (spr_503.png)
  SPRITE_WATER_DROP,
  /// ★ Named from what the player sees: an island of green hills around a
  /// mountain (spr_504.png)
  SPRITE_WORLD_MAP_ISLAND,
  SPRITE_505,
  SPRITE_506,
  SPRITE_507,
  SPRITE_508,
  SPRITE_509,
  /// The shadow a unit casts on the ground in battle, BattleObject.shadow as a
  /// battle object is made: the sprite BattleObject_SpawnObjectsForList adds as
  /// a layer of every one-part body (SPRITE_BATTLE_SHADOW_NARROW on a body 32
  /// pixels wide). Its 64 by 64 picture holds only a 32 by 6 ellipse along its
  /// bottom edge, SPRITE_BATTLE_SHADOW_NARROW's a 16 by 3 one. It stays on the
  /// ground while the unit is off it (BattleObject_UpdateSpritePriorities), and
  /// adding it clears the sprite's own shadow (Sprite.flags bit 0), so a unit
  /// casts one.
  /// ★ Named from what the player sees: the black ellipse under each unit's
  /// feet (a battle's render)
  SPRITE_BATTLE_SHADOW,
  /// The shadow a body 32 pixels wide takes instead of SPRITE_BATTLE_SHADOW.
  SPRITE_BATTLE_SHADOW_NARROW
};

/// SPRITE_STUMP's animations beyond its standing frame (spr_212.tsv).
enum StumpAnimation {
  /// Sinks until only its cut top shows, level with the ground or the water,
  /// which leaves it afloat in Gondowan Cave's river.
  STUMP_ANIMATION_SINK = 2,
  STUMP_ANIMATION_SPLASH = 3 ///< drops into water with a splash
};

/// SPRITE_PUDDLE's animations (spr_227.tsv): the puddle, the ice pillar Frost
/// makes of it, and the steps between.
enum PuddleAnimation {
  PUDDLE_ANIMATION_PUDDLE = 1,
  PUDDLE_ANIMATION_FREEZE = 2,  ///< rises from the puddle into an ice pillar
  PUDDLE_ANIMATION_LOW_ICE = 3, ///< held at the second step of freezing
  PUDDLE_ANIMATION_PILLAR = 5,  ///< the ice pillar, still
  PUDDLE_ANIMATION_MELT = 6     ///< sinks from the pillar back into a puddle
};

/// SPRITE_WATER_BEAST's animations beyond its standing frame (spr_190.tsv).
enum WaterBeastAnimation {
  WATER_BEAST_ANIMATION_LEAP = 2, ///< stretched out, legs down, as in mid-jump
  /// Two low crouches in turn, a quarter of a second each: as one spits
  /// water, and between its high jumps.
  WATER_BEAST_ANIMATION_SPIT = 3
};

/// SPRITE_WARRIOR_IN_GOLDEN_HELMET's animations at the end of a Colosso
/// finals match (spr_062.tsv).
enum GoldenHelmetWarriorAnimation {
  WARRIOR_ANIMATION_DEFEATED = 8,        ///< slumped to the ground
  WARRIOR_ANIMATION_FIGHTING_STANCE = 9, ///< braced to fight
  WARRIOR_ANIMATION_ARMS_RAISED = 10     ///< both arms raised in victory
};

/// SPRITE_BABI's animations that scenes set by number (spr_052.tsv).
enum BabiAnimation {
  /// Lying in bed, only his head on the pillow showing, its two frames
  /// alternating.
  BABI_ANIMATION_IN_BED = 5
};

/// SPRITE_LOG_NORTH_SOUTH's animations (spr_205.tsv) as a log tips into a
/// river
/// (places/kolima-forest-entrance/events/KolimaForestEntrance_PushLog.gif).
enum LogAnimation {
  LOG_ANIMATION_ROLL_WEST = 3, ///< rolling over toward the west
  LOG_ANIMATION_FLOAT = 4,     ///< bobbing in the water
  /// Drops into the water with a ring of spray, then LOG_ANIMATION_FLOAT.
  LOG_ANIMATION_DROP_IN = 8
};

/// SPRITE_SPLASH's animations (spr_214.tsv), as Kolima Forest's floodgate
/// plays them
/// (places/kolima-forest-floodgate/events/KolimaForestFloodgate_FlipFloodgateSwitch.gif).
enum SplashAnimation {
  SPLASH_ANIMATION_BURST = 1, ///< one burst of spray, then nothing
  /// Spray churning for about 120 frames, then dying away to nothing.
  SPLASH_ANIMATION_CHURN = 2
};

/// The face animations of SPRITE_TRET and SPRITE_TRET_RAISED (spr_055.tsv):
/// his gentle face, then his wrathful one, with knotted brows and red eyes.
/// Left and right are as the face is drawn on screen.
enum TretFaceAnimation {
  TRET_FACE_OPEN = 1,
  TRET_FACE_HALF_CLOSED = 2,
  TRET_FACE_CLOSED = 3,
  TRET_FACE_LOOK_RIGHT = 4,
  TRET_FACE_LOOK_LEFT = 5,
  TRET_FACE_WRATHFUL_OPEN = 6,
  TRET_FACE_WRATHFUL_CLOSED = 8,
  TRET_FACE_WRATHFUL_LOOK_RIGHT = 9,
  TRET_FACE_WRATHFUL_LOOK_LEFT = 10,
  /// Between the two faces: his brows half knotted, his eyes shut.
  TRET_FACE_TURNING_WRATHFUL = 11
};

/// SPRITE_LAUREL's face animations (spr_056.tsv), as TretFaceAnimation's first
/// five.
enum LaurelFaceAnimation {
  LAUREL_FACE_OPEN = 1,
  LAUREL_FACE_HALF_CLOSED = 2,
  LAUREL_FACE_CLOSED = 3,
  LAUREL_FACE_LOOK_RIGHT = 4,
  LAUREL_FACE_LOOK_LEFT = 5
};

/// SPRITE_TORCH's animations (spr_246.tsv).
enum TorchAnimation {
  TORCH_ANIMATION_UNLIT = 1, ///< a cold torch, its bowl grey with ash
  TORCH_ANIMATION_LIT = 2,   ///< a flame flickering in the bowl
  TORCH_ANIMATION_GO_OUT =
      3 ///< the flame dies in smoke, then TORCH_ANIMATION_UNLIT
};

#endif
