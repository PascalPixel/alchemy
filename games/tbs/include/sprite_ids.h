#ifndef ALCHEMY_SPRITE_IDS_H
#define ALCHEMY_SPRITE_IDS_H

/// @file
/// Sprite numbers of field objects, named where the code shows what they are.
/// The characters' and Djinn's sprites are in character_ids.h. A sprite's
/// picture is graphics/sprite/spr_<number>.png, its number in decimal.

/// The sprites the field, its Psynergy and the scenes' placement tables create
/// objects of by number.
enum SpriteId {
  /// A treasure chest, shut, opening and open, with the frames of the mimic
  /// some chests turn out to be.
  /// ★ Named from what the player sees: a red treasure chest (its render)
  SPRITE_CHEST = 0x14,
  /// A blank 16-pixel square: an actor that shows nothing, such as a shop
  /// counter, or that a scene draws an item's icon on, as for the Nuts that
  /// Catch takes.
  SPRITE_BLANK = 0x16,
  /// The soldiers of Babi's Tolbi, and the Colosseum's officials, drawn alike.
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  SPRITE_SOLDIER = 0x98,
  /// The statues of Sol Sanctum's statue room and Luna and Sol rooms.
  /// ★ Named from what the player sees: a winged statue holding a blue orb
  /// (its sprite sheet, spr_201.png)
  SPRITE_WINGED_STATUE = 0xc9,
  /// The centre statue of Sol Sanctum's statue room.
  /// ★ Named from what the player sees: a winged statue bearing a sun disc on
  /// its head (its sprite sheet, spr_202.png)
  SPRITE_SUN_DISC_STATUE = 0xca,
  /// The centre statue once it has sunk into the floor, only its disc showing.
  /// ★ Named from what the player sees: a round sun disc lying flat (its
  /// sprite sheet, spr_204.png)
  SPRITE_SUN_DISC = 0xcc,
  /// ★ Named from what the player sees: a carved stone statue of a figure,
  /// as Sol Sanctum's statues stand on its seals (its render)
  SPRITE_STONE_STATUE = 0xcb,
  /// The logs that float on the water of Fuchin Falls Cave, Crossbone Isle's
  /// floating logs room and the Colosso's Lumber Water Crossing Area.
  /// ★ Named from what the player sees: a yellow log floating west to east
  /// (its render)
  SPRITE_FLOATING_LOG_WEST_EAST = 0xec,
  /// ★ Named from what the player sees: a yellow log floating north to south
  /// (its render)
  SPRITE_FLOATING_LOG_NORTH_SOUTH = 0xed,
  /// A log lying north to south, one the leader can hop onto
  /// (Field_TryJumpForward) or roll, as in Kolima Forest.
  /// ★ Named from what the player sees: a log lying north to south (its
  /// render)
  SPRITE_LOG_NORTH_SOUTH = 0xcd,
  /// ★ Named from what the player sees: a log lying west to east (its render)
  SPRITE_LOG_WEST_EAST = 0xcf,
  /// The effect FieldFx_ConfigureLinkedObject attaches to an actor or shows
  /// once, as the leader flinches on hazard ground.
  SPRITE_ATTACHED_EFFECT = 209,
  /// What Move pushes in Goma Cave and many rooms after it, which the game
  /// calls a stone pillar ("It's just a stone pillar.").
  /// ★ Named from what the player sees: a tall stone pillar (its render)
  SPRITE_STONE_PILLAR = 0xd3,
  /// ★ Named from what the player sees: a tree stump (its render)
  SPRITE_STUMP = 0xd4,
  SPRITE_DUST_PUFF = 222, ///< a puff of dust kicked up where an actor stands
  /// The statues of Mercury Lighthouse.
  /// ★ Named from what the player sees: a blue statue of a woman holding a
  /// jar (its render)
  SPRITE_BLUE_STATUE = 0xe0,
  /// A puddle of water, which Frost freezes into a pillar of ice.
  /// ★ Named from what the player sees: a puddle of water (its render)
  SPRITE_PUDDLE = 0xe3,
  /// The sprout Growth grows.
  /// ★ Named from what the player sees: a green sprout (its render)
  SPRITE_SPROUT = 0xee,
  /// ★ Named from what the player sees: a stack of two wooden crates (its
  /// render)
  SPRITE_CRATES = 0xf1,
  /// The platforms that float in Mercury Lighthouse's aerie and that carry the
  /// leader up to Venus Lighthouse's.
  /// ★ Named from what the player sees: a round floating platform (its
  /// render)
  SPRITE_FLOATING_PLATFORM = 0xf2,
  /// ★ Named from what the player sees: an unlit torch on its stand (its
  /// render)
  SPRITE_TORCH = 0xf6,
  /// The pillars Move shifts in the Tunnel Ruins and the lighthouses.
  /// ★ Named from what the player sees: a stone pillar with a glowing yellow
  /// cap (its render)
  SPRITE_CAPPED_PILLAR = 0xfd,
  /// The platforms of Venus Lighthouse and the Tunnel Ruins that rise or sink
  /// with the weight set down on them.
  /// ★ Named from what the player sees: a small white pillar on its base
  /// (its render)
  SPRITE_BALANCE_PLATFORM = 0x101,
  /// The particles of Growth and of a Djinni's capture, and the gate effects
  /// of Venus Lighthouse.
  SPRITE_PARTICLE = 284,
  /// One sparkle: the glints of Halt and of the field's sparkles, and Kolima's
  /// curse dust.
  /// ★ Named from what the player sees: a sparkle of light that shrinks away
  /// (its render)
  SPRITE_SPARKLE = 285,
  /// The statues that chase the leader in Crossbone Isle, and those of the
  /// Tunnel Ruins and Venus Lighthouse.
  /// ★ Named from what the player sees: a statue of a woman in a long robe
  /// (its render)
  SPRITE_LADY_STATUE = 0x1f4,
  // People and animals.
  /// The sprite of Kraden.
  /// ★ Named from what the player sees: an old man with white hair and a
  /// white beard in a brown robe (spr_030.png)
  SPRITE_KRADEN = 0x1e,
  /// The sprite of Dora, Isaac's mother.
  /// ★ Named from what the player sees: a woman with fair hair tied up, a
  /// blue top and a pink striped skirt (spr_031.png)
  SPRITE_DORA = 0x1f,
  /// The sprite of Saturos.
  /// ★ Named from what the player sees: a blue-skinned man with pale blue
  /// hair in a long blue cloak (spr_032.png)
  SPRITE_SATUROS = 0x20,
  /// The sprite of Menardi.
  /// ★ Named from what the player sees: a woman with long fair hair in a red
  /// outfit (spr_033.png)
  SPRITE_MENARDI = 0x21,
  /// The sprite of Felix behind his mask.
  /// ★ Named from what the player sees: a young man with brown hair, his face
  /// behind a green mask, in a green cloak (spr_034.png)
  SPRITE_FELIX_MASKED = 0x22,
  /// The sprite of Felix as a young man.
  /// ★ Named from what the player sees: a young man with brown hair in a
  /// green cloak (spr_035.png)
  SPRITE_FELIX = 0x23,
  /// ★ Named from what the player sees: a small boy with spiky orange hair in
  /// a green shirt and shorts (spr_037.png)
  SPRITE_BOY_WITH_ORANGE_HAIR = 0x25,
  /// ★ Named from what the player sees: a woman with orange hair under a
  /// green headscarf, in a blue top (spr_038.png)
  SPRITE_WOMAN_IN_GREEN_HEADSCARF = 0x26,
  /// The sprite of Feizhi.
  /// ★ Named from what the player sees: a girl with purple hair tied up with
  /// a bow, in a yellow and blue dress (spr_039.png)
  SPRITE_FEIZHI = 0x27,
  /// The sprite of Feh.
  /// ★ Named from what the player sees: a young man with black hair in a
  /// yellow and green shirt (spr_040.png)
  SPRITE_FEH = 0x28,
  /// The sprite of Hsu.
  /// ★ Named from what the player sees: a young man with brown hair in an
  /// orange jacket and green trousers (spr_041.png)
  SPRITE_HSU = 0x29,
  /// ★ Named from what the player sees: a young woman with an orange ponytail
  /// in a green and purple dress (spr_042.png)
  SPRITE_WOMAN_WITH_ORANGE_PONYTAIL = 0x2a,
  /// The sprite of Alex.
  /// ★ Named from what the player sees: a young man with long pale blue hair
  /// in a white and purple outfit (spr_043.png)
  SPRITE_ALEX = 0x2b,
  /// The sprite of Donpa.
  /// ★ Named from what the player sees: an old man with long grey hair in a
  /// yellow robe, with a staff (spr_044.png)
  SPRITE_DONPA = 0x2c,
  /// The sprite of Layana.
  /// ★ Named from what the player sees: a woman with long curly red hair in a
  /// green dress (spr_045.png)
  SPRITE_LAYANA = 0x2d,
  /// ★ Named from what the player sees: an old man with white hair and a
  /// white beard in green clothes (spr_046.png)
  SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN = 0x2e,
  /// The sprite of Kaja, the Tolbi ship's fighter.
  /// ★ Named from what the player sees: a bald man with a beard in a green
  /// apron (spr_047.png)
  SPRITE_KAJA = 0x2f,
  /// The sprite of Hammet.
  /// ★ Named from what the player sees: a man in a turban and orange and
  /// yellow robes (spr_049.png)
  SPRITE_HAMMET = 0x31,
  /// The sprite of Dodonpa.
  /// ★ Named from what the player sees: a man with spiky dark blue hair and a
  /// moustache in a white top and a red cloak (spr_050.png)
  SPRITE_DODONPA = 0x32,
  /// The sprite of Nyunpa.
  /// ★ Named from what the player sees: an old man with white hair and a
  /// white beard in an orange robe (spr_051.png)
  SPRITE_NYUNPA = 0x33,
  /// The sprite of Babi.
  /// ★ Named from what the player sees: an old man with long pink hair in a
  /// red and gold robe, with a cane (spr_052.png)
  SPRITE_BABI = 0x34,
  /// The sprite of Kyle.
  /// ★ Named from what the player sees: a man with fair hair in a blue and
  /// white top and green trousers (spr_053.png)
  SPRITE_KYLE = 0x35,
  /// ★ Named from what the player sees: a man with brown hair in a teal shirt
  /// and brown trousers (spr_054.png)
  SPRITE_MAN_IN_TEAL_SHIRT = 0x36,
  /// The sprite of Tret.
  /// ★ Named from what the player sees: a great stone-coloured face with a
  /// heavy brow (spr_055.png)
  SPRITE_TRET = 0x37,
  /// The sprite of Laurel.
  /// ★ Named from what the player sees: an orange face with green eyes
  /// (spr_056.png)
  SPRITE_LAUREL = 0x38,
  /// The sprite of Iodem.
  /// ★ Named from what the player sees: a man with grey hair and a grey beard
  /// in a blue tunic and a yellow cape (spr_057.png)
  SPRITE_IODEM = 0x39,
  /// The sprite of McCoy.
  /// ★ Named from what the player sees: a stout man with red hair in a pink
  /// tunic (spr_058.png)
  SPRITE_MCCOY = 0x3a,
  /// ★ Named from what the player sees: a man with brown hair in a green
  /// shirt and blue trousers (spr_059.png)
  SPRITE_MAN_IN_GREEN_SHIRT = 0x3b,
  /// ★ Named from what the player sees: a woman with pink hair tied up, in a
  /// white apron and a blue skirt (spr_060.png)
  SPRITE_WOMAN_WITH_PINK_HAIR = 0x3c,
  /// The sprite of Hama.
  /// ★ Named from what the player sees: a woman with long purple hair in a
  /// white and green robe (spr_061.png)
  SPRITE_HAMA = 0x3d,
  /// ★ Named from what the player sees: a warrior in a golden helmet with a
  /// red crest (spr_062.png)
  SPRITE_WARRIOR_IN_GOLDEN_HELMET = 0x3e,
  /// ★ Named from what the player sees: a man with long dark hair in a green
  /// and orange outfit (spr_063.png)
  SPRITE_MAN_WITH_LONG_DARK_HAIR = 0x3f,
  /// ★ Named from what the player sees: a person with long dark purple hair
  /// in purple clothes (spr_064.png)
  SPRITE_PERSON_IN_PURPLE = 0x40,
  /// Babi's second sheet, which Altmiller Cave places while he is invisible
  /// (ACTOR_INVISIBLE_BABI).
  /// ★ Named from what the player sees: an old man with long pink hair in a
  /// red and gold robe, with a cane (spr_065.png)
  SPRITE_INVISIBLE_BABI = 0x41,
  /// ★ Named from what the player sees: a young man with spiky blue hair and
  /// a yellow headband in white (spr_066.png)
  SPRITE_YOUNG_MAN_WITH_BLUE_HAIR = 0x42,
  /// ★ Named from what the player sees: a young man with spiky red hair and a
  /// yellow headband (spr_067.png)
  SPRITE_YOUNG_MAN_WITH_RED_HAIR = 0x43,
  /// The sprite of Bunza.
  /// ★ Named from what the player sees: a man with dark hair combed back in a
  /// blue tunic (spr_068.png)
  SPRITE_BUNZA = 0x44,
  /// ★ Named from what the player sees: a woman with curly purple hair in a
  /// pink and purple dress (spr_069.png)
  SPRITE_WOMAN_WITH_PURPLE_HAIR = 0x45,
  /// The sprite of Faran.
  /// ★ Named from what the player sees: a man with long fair hair in an
  /// orange and brown robe (spr_070.png)
  SPRITE_FARAN = 0x46,
  /// ★ Named from what the player sees: a burly grey-haired man with bare
  /// arms folded, in a dark vest (spr_101.png)
  SPRITE_BURLY_MAN_WITH_GREY_HAIR = 0x65,
  /// ★ Named from what the player sees: a little girl with orange pigtails in
  /// a green dress (spr_102.png)
  SPRITE_GIRL_WITH_PIGTAILS = 0x66,
  /// ★ Named from what the player sees: a bent old man with a bald head and a
  /// white beard in a green robe, with a cane (spr_103.png)
  SPRITE_OLD_MAN_WITH_CANE = 0x67,
  /// ★ Named from what the player sees: a small white dog with brown ears
  /// (spr_104.png)
  SPRITE_DOG = 0x68,
  /// ★ Named from what the player sees: a tree with a full green crown
  /// (spr_105.png)
  SPRITE_TREE = 0x69,
  /// ★ Named from what the player sees: a woman with long fair hair and a
  /// blue headband in a white apron (spr_106.png)
  SPRITE_WOMAN_WITH_FAIR_HAIR = 0x6a,
  /// ★ Named from what the player sees: a small boy with fair hair in a blue
  /// shirt (spr_107.png)
  SPRITE_BOY_WITH_FAIR_HAIR = 0x6b,
  /// ★ Named from what the player sees: a young man with brown hair in a purple
  /// and white shirt (spr_108.png)
  SPRITE_YOUNG_MAN_WITH_BROWN_HAIR = 0x6c,
  /// ★ Named from what the player sees: a stout man with brown hair in an
  /// orange and blue tunic (spr_109.png)
  SPRITE_STOUT_MAN = 0x6d,
  /// ★ Named from what the player sees: a small tree (spr_110.png)
  SPRITE_SMALL_TREE = 0x6e,
  /// ★ Named from what the player sees: an old woman with white hair in a
  /// grey dress (spr_111.png)
  SPRITE_OLD_WOMAN = 0x6f,
  /// ★ Named from what the player sees: an old man with a white beard in a
  /// blue cap and a blue robe (spr_112.png)
  SPRITE_OLD_MAN_IN_BLUE_CAP = 0x70,
  /// ★ Named from what the player sees: a man with fair hair and glasses in a
  /// blue cap and a red and blue robe (spr_113.png)
  SPRITE_MAN_WITH_GLASSES = 0x71,
  /// ★ Named from what the player sees: a bald, muscular man, bare-chested,
  /// in green trousers (spr_114.png)
  SPRITE_BALD_MUSCULAR_MAN = 0x72,
  /// ★ Named from what the player sees: a woman with brown hair in a green
  /// dress and a white apron (spr_115.png)
  SPRITE_WOMAN_IN_GREEN_DRESS = 0x73,
  /// ★ Named from what the player sees: a stocky, muscular man with black
  /// hair in a red top, flexing his arms (spr_116.png)
  SPRITE_STRONGMAN = 0x74,
  /// ★ Named from what the player sees: an old man with white hair in a pale
  /// robe, with a cane (spr_117.png)
  SPRITE_OLD_MAN_WITH_WHITE_HAIR = 0x75,
  /// ★ Named from what the player sees: a young woman with a brown ponytail
  /// in a white top and a long skirt (spr_118.png)
  SPRITE_WOMAN_WITH_PONYTAIL = 0x76,
  /// ★ Named from what the player sees: a man with dark hair in a dark blue
  /// tunic (spr_119.png)
  SPRITE_MAN_WITH_DARK_HAIR = 0x77,
  /// ★ Named from what the player sees: a woman with brown hair in a long
  /// blue dress (spr_120.png)
  SPRITE_WOMAN_IN_BLUE_DRESS = 0x78,
  /// ★ Named from what the player sees: a man with a white beard in a tall
  /// hat and a long purple and blue robe (spr_121.png)
  SPRITE_MAN_IN_TALL_HAT = 0x79,
  /// ★ Named from what the player sees: a man with long brown hair and a
  /// beard in a red shawl and a long lavender robe (spr_122.png)
  SPRITE_BEARDED_MAN_IN_LONG_ROBE = 0x7a,
  /// ★ Named from what the player sees: a man in a white cook's cap and white
  /// clothes, with a frying pan (spr_123.png)
  SPRITE_COOK = 0x7b,
  /// ★ Named from what the player sees: a bald man in a red tunic with green
  /// sleeves (spr_124.png)
  SPRITE_BALD_MAN_IN_RED = 0x7c,
  /// ★ Named from what the player sees: a young man with brown hair in a
  /// green shirt (spr_125.png)
  SPRITE_YOUNG_MAN_IN_GREEN = 0x7d,
  /// ★ Named from what the player sees: a woman with red-brown hair in a pink
  /// dress and a white apron (spr_128.png)
  SPRITE_WOMAN_IN_PINK_APRON = 0x80,
  /// ★ Named from what the player sees: a man with fair hair in a blue
  /// feathered cap, holding a lute (spr_129.png)
  SPRITE_MAN_WITH_LUTE = 0x81,
  /// ★ Named from what the player sees: a young man with brown hair and a
  /// white headband in a white top and blue trousers (spr_130.png)
  SPRITE_YOUTH_WITH_HEADBAND = 0x82,
  /// ★ Named from what the player sees: a young man with brown hair tied back
  /// in a white top and blue trousers (spr_131.png)
  SPRITE_YOUTH_WITH_TIED_HAIR = 0x83,
  /// ★ Named from what the player sees: a man in a tall white chef's hat and
  /// white clothes with a red scarf, with a frying pan (spr_132.png)
  SPRITE_CHEF = 0x84,
  /// ★ Named from what the player sees: a man with a mop of orange hair in a
  /// yellow vest and green trousers (spr_133.png)
  SPRITE_MAN_WITH_ORANGE_HAIR = 0x85,
  /// ★ Named from what the player sees: a man with golden-brown hair in
  /// orange and red clothes (spr_134.png)
  SPRITE_MAN_IN_ORANGE = 0x86,
  /// ★ Named from what the player sees: a man with black hair and a white
  /// headband in an orange top, with a pickaxe (spr_135.png)
  SPRITE_MAN_WITH_PICKAXE = 0x87,
  /// ★ Named from what the player sees: a small white and grey cat
  /// (spr_136.png)
  SPRITE_CAT = 0x88,
  /// ★ Named from what the player sees: a man with a beard in a white turban
  /// and long white robes (spr_143.png)
  SPRITE_MAN_IN_WHITE_TURBAN = 0x8f,
  /// ★ Named from what the player sees: a man with tanned skin in a red
  /// bandana and white trousers (spr_144.png)
  SPRITE_MAN_IN_RED_BANDANA = 0x90,
  /// ★ Named from what the player sees: a man with tanned skin in a blue
  /// bandana and white trousers (spr_145.png)
  SPRITE_MAN_IN_BLUE_BANDANA = 0x91,
  /// ★ Named from what the player sees: a man with tanned skin in a yellow
  /// bandana and white trousers (spr_146.png)
  SPRITE_MAN_IN_YELLOW_BANDANA = 0x92,
  /// ★ Named from what the player sees: a man with tanned skin in a green
  /// bandana and a red vest (spr_147.png)
  SPRITE_MAN_IN_GREEN_BANDANA = 0x93,
  /// ★ Named from what the player sees: a young woman with long brown hair in
  /// a green dress (spr_148.png)
  SPRITE_YOUNG_WOMAN_IN_GREEN = 0x94,
  /// ★ Named from what the player sees: a bald monk in a white and orange
  /// robe (spr_149.png)
  SPRITE_MONK_IN_WHITE_AND_ORANGE = 0x95,
  /// ★ Named from what the player sees: a soldier in a spiked steel helmet,
  /// with a spear (spr_150.png)
  SPRITE_SOLDIER_IN_SPIKED_HELMET = 0x96,
  /// ★ Named from what the player sees: a small bald boy in an orange robe
  /// (spr_151.png)
  SPRITE_BALD_BOY_IN_ORANGE = 0x97,
  /// ★ Named from what the player sees: a soldier in a white turban, with a
  /// spear (spr_153.png)
  SPRITE_SOLDIER_IN_TURBAN = 0x99,
  /// ★ Named from what the player sees: a covered wagon drawn by a horse
  /// (spr_154.png)
  SPRITE_WAGON = 0x9a,
  /// ★ Named from what the player sees: a white bird with outspread wings
  /// (spr_155.png)
  SPRITE_BIRD = 0x9b,
  /// ★ Named from what the player sees: a young man with brown hair tied back
  /// in a blue jacket and a white apron (spr_156.png)
  SPRITE_YOUNG_MAN_IN_BLUE_JACKET = 0x9c,
  /// ★ Named from what the player sees: a person with long brown hair and a
  /// green scarf in a red top, with a sword (spr_157.png)
  SPRITE_PERSON_WITH_SWORD = 0x9d,
  /// ★ Named from what the player sees: a bald monk in an orange robe
  /// (spr_158.png)
  SPRITE_MONK_IN_ORANGE_ROBE = 0x9e,
  /// ★ Named from what the player sees: a young woman with black hair and a
  /// red hair ornament in a red and yellow dress (spr_159.png)
  SPRITE_WOMAN_WITH_BLACK_HAIR = 0x9f,
  /// ★ Named from what the player sees: a man with black hair in a topknot in
  /// an orange and green robe (spr_160.png)
  SPRITE_MAN_WITH_TOPKNOT = 0xa0,
  /// ★ Named from what the player sees: a ship with green sails (spr_161.png)
  SPRITE_SHIP = 0xa1,
  /// ★ Named from what the player sees: a woman with red-brown hair in a
  /// white top and a green apron (spr_162.png)
  SPRITE_WOMAN_IN_GREEN_APRON = 0xa2,
  /// ★ Named from what the player sees: a soldier in a steel helmet, with a
  /// torch (spr_163.png)
  SPRITE_SOLDIER_WITH_TORCH = 0xa3,
  /// ★ Named from what the player sees: an old man with grey hair in a
  /// topknot in blue and orange clothes (spr_164.png)
  SPRITE_OLD_MAN_IN_BLUE = 0xa4,
  /// ★ Named from what the player sees: a young man with fair hair in a white
  /// and orange tunic (spr_165.png)
  SPRITE_YOUNG_MAN_WITH_FAIR_HAIR = 0xa5,
  /// ★ Named from what the player sees: an old woman with white hair in a
  /// blue and red dress, with a cane (spr_166.png)
  SPRITE_OLD_WOMAN_WITH_CANE = 0xa6,
  /// ★ Named from what the player sees: a man with grey hair in a black suit
  /// (spr_167.png)
  SPRITE_MAN_IN_BLACK_SUIT = 0xa7,
  /// ★ Named from what the player sees: a small boy with black hair in a bun
  /// in green clothes (spr_168.png)
  SPRITE_BOY_IN_GREEN = 0xa8,
  /// ★ Named from what the player sees: a little girl with brown hair in a
  /// pink top (spr_169.png)
  SPRITE_GIRL_IN_PINK = 0xa9,
  /// ★ Named from what the player sees: a bald, muscular man with a black
  /// topknot, bare-chested (spr_170.png)
  SPRITE_MUSCULAR_MAN_WITH_TOPKNOT = 0xaa,
  /// ★ Named from what the player sees: a man in a green cap and a green and
  /// yellow robe, carrying a pack on his back (spr_171.png)
  SPRITE_MAN_CARRYING_A_PACK = 0xab
};

#endif
