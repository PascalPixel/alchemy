#ifndef ALCHEMY_SPRITE_IDS_H
#define ALCHEMY_SPRITE_IDS_H

/// @file
/// Sprite numbers of objects, named where the code shows what they are.
/// The characters' and Djinn's sprites are in character_ids.h. A sprite's
/// picture is graphics/sprite/spr_<number>.png, its number in decimal.

/// The sprites the field, its Psynergy and the maps' object event template
/// tables create objects of by number.
enum SpriteId {
  /// A treasure chest, shut, opening and open, with the frames of the mimic
  /// some chests turn out to be.
  /// ★ Named from what the player sees: a red treasure chest (its render)
  SPRITE_CHEST = 0x14,
  /// A blank 16-pixel square: an object event that shows nothing, such as a
  /// shop
  /// counter, or that a map draws an item's icon on, as for the Nuts that
  /// Catch takes.
  SPRITE_BLANK = 0x16,
  /// A blank 32-pixel square (spr_023.png holds no drawn pixel).
  SPRITE_BLANK_32 = 0x17,
  /// Rings spreading out on water: where someone wades (the field's own
  /// FIELD_SPRITE_RIPPLE), and around the leader at Mercury Lighthouse's pools,
  /// grey in its first animation and blue in its second.
  /// ★ Named from what the player sees: rings spreading on water
  /// (spr_024.png)
  SPRITE_RIPPLE = 0x18,
  /// ★ Named from the game: "Lizard Man" (MsgEnemyName)
  SPRITE_LIZARD_MAN = 0xc0,
  /// The soldiers of Babi's Tolbi and of the Colosseum.
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  SPRITE_SOLDIER = 0x98,
  /// A big round rock: the rocks that fall across Vale's paths on the stormy
  /// night, and the Boulder of Mt. Aleph drawn at twice its size.
  /// ★ Named from the game: "It's a huge rock" (MsgValeRockCannotBeMoved)
  SPRITE_ROCK = 0xc8,
  /// A blank 32-pixel square: an object event seen only by its shadow, such as
  /// the
  /// shadows under Altin Peak's falling boulder and its Hydros statue.
  /// ★ Named from what the player sees: nothing, every pixel transparent
  /// (spr_288.png)
  SPRITE_BLANK_LARGE = 0x120,
  /// The lights that float about the Elemental Star Room.
  /// ★ Named from what the player sees: a small glowing blue-green light (its
  /// sprite sheet, spr_287.png)
  SPRITE_FLOATING_LIGHT = 0x11f,
  /// The apes of Mogall Forest, which Force brings out of their hiding places.
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  SPRITE_APE = 0xbf,
  /// The water beasts that flood Altin and its mine, whose battles name them
  /// Living Statues.
  /// ★ Named from the game: "Are you the ones who defeated the water beasts?"
  /// (MsgAltinOnesWhoDefeatedWaterBeasts)
  SPRITE_WATER_BEAST = 0xbe,
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
  /// The Tolbi-bound Ship's oars and the sea monsters that attack it.
  /// ★ Named from what the player sees: a wooden oar held at rest, slanting
  /// (its render)
  SPRITE_RESTING_OAR = 0xbc,
  /// ★ Named from what the player sees: a blue jellyfish monster with
  /// trailing tentacles (its render)
  SPRITE_JELLYFISH_MONSTER = 0xbd,
  /// The Kraken, which the party fights on the ship's deck.
  /// ★ Named from the game: "You did good against that Kraken."
  /// (MsgTolbiShipThankIsaacDidGoodAgainst)
  SPRITE_KRAKEN = 0xc2,
  /// ★ Named from what the player sees: a pink winged monster (its render)
  SPRITE_PINK_WINGED_MONSTER = 0xc4,
  /// ★ Named from what the player sees: a blue winged monster with claws (its
  /// render)
  SPRITE_BLUE_WINGED_MONSTER = 0xc5,
  /// ★ Named from what the player sees: a green lizard-like monster (its
  /// render)
  SPRITE_LIZARD_MONSTER = 0xc6,
  /// ★ Named from what the player sees: an oar dipping into the water with a
  /// splash (its render)
  SPRITE_OAR = 0xe1,
  /// The signs of Lalivero, Tolbi Spring, Altin Peak and Kolima Forest's
  /// floodgate, and the signposts on the world map.
  /// ★ Named from what the player sees: a wooden sign on a post (its render)
  SPRITE_SIGNPOST = 0x112,
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
  /// The effect FieldEffect_ConfigureLinkedObject attaches to an object event
  /// or shows
  /// once, as the leader flinches on hazard ground.
  SPRITE_ATTACHED_EFFECT = 209,
  /// ★ Named from the game: "chest of valuables" (MsgValeSChestValuables)
  SPRITE_CHEST_OF_VALUABLES = 0xd2,
  /// What Move pushes in Goma Cave and many rooms after it, which the game
  /// calls a stone pillar ("It's just a stone pillar.").
  /// ★ Named from what the player sees: a tall stone pillar (its render)
  SPRITE_STONE_PILLAR = 0xd3,
  /// ★ Named from what the player sees: a tree stump (its render)
  SPRITE_STUMP = 0xd4,
  /// The hand Move reaches out with; Goma Cave's entrance and Vale place it
  /// to push something in a cutscene.
  /// ★ Named from what the player sees: a large open hand (spr_215.png)
  SPRITE_MOVE_HAND = 0xd7,
  /// Spray thrown up where water is struck: where Goma Cave's stumps land,
  /// at Kolima Forest's floodgate, and over the Tolbi-bound Ship's side.
  /// ★ Named from what the player sees: a splash of white and pale blue
  /// spray (spr_214.png)
  SPRITE_SPLASH = 0xd6,
  /// The fields of Psynergy that rise over the party as Kolima's curse falls.
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  SPRITE_PSYNERGY_FIELD = 0xdc,
  /// The sparkles of Tret's glamour that rain on Kolima and its forest.
  /// ★ Named from what the player sees: rings of golden light (spr_026.png)
  SPRITE_GOLDEN_RINGS = 0x1a,
  /// The statues Move or a push slides in Bilibin and its Treasury.
  /// ★ Named from what the player sees: a golden statue of a figure in a long
  /// robe (spr_243.png)
  SPRITE_GOLDEN_STATUE = 0xf3,
  /// A puff of dust kicked up where an object event stands; the water beasts of
  /// Altin
  /// and its mine spit it as spray, drawn SPRITE_COLOR_BGR or
  /// SPRITE_COLOR_WHITE.
  SPRITE_DUST_PUFF = 222,
  /// ★ Named from what the player sees: a wooden crate (spr_223.png)
  SPRITE_CRATE = 0xdf,
  /// The carts on the rails of Altin's mine.
  /// ★ Named from what the player sees: a wooden mine cart on iron wheels
  /// (spr_231.png)
  SPRITE_MINE_CART = 0xe7,
  /// ★ Named from what the player sees: a snowman with a red cap and a green
  /// scarf (spr_234.png)
  SPRITE_SNOWMAN = 0xea,
  /// The statues of Mercury Lighthouse.
  /// ★ Named from what the player sees: a blue statue of a woman holding a
  /// jar (its render)
  SPRITE_BLUE_STATUE = 0xe0,
  /// The stone pipe sections Move pushes in Mercury Lighthouse to join its
  /// channels, and the short upright pipes beside them.
  /// ★ Named from what the player sees: a stone pipe section lying west to
  /// east (spr_228.png)
  SPRITE_PIPE_WEST_EAST = 0xe4,
  /// The pipe sections Move pushes in Mercury Lighthouse, also placed in the
  /// Lighthouse Aerie of Venus Lighthouse.
  /// ★ Named from what the player sees: a stone pipe section lying north to
  /// south (spr_229.png)
  SPRITE_PIPE_NORTH_SOUTH = 0xe5,
  /// The rim of an upright pipe's open end, drawn over the end a pushed pipe
  /// section comes to rest against.
  /// ★ Named from what the player sees: the rim at the top of an upright pipe
  /// (spr_230.png; places/mercury-lighthouse-lower/map-4.png)
  SPRITE_PIPE_END = 0xe6,
  /// A puddle of water, which Frost freezes into a pillar of ice.
  /// ★ Named from what the player sees: a puddle of water (its render)
  SPRITE_PUDDLE = 0xe3,
  /// Isaac's tunic: placed beside his bed on the stormy night, and drawn as a
  /// second layer over the child sprites of Isaac and Garet while
  /// FLAG_ISAAC_WEARS_TUNIC is set (Object_RegisterSpriteMetadata).
  /// ★ Named from the game: "Don't go out without your tunic!"
  /// (MsgValeComeOnHurry)
  SPRITE_TUNIC = 0xe2,
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
  /// The keys the maps show: the barricade key McCoy tosses to the party,
  /// and Lunpa Fortress's Cell Key.
  /// ★ Named from what the player sees: a golden key (spr_245.png)
  SPRITE_KEY = 0xf5,
  /// ★ Named from what the player sees: an unlit torch on its stand (its
  /// render)
  SPRITE_TORCH = 0xf6,
  /// A fire burning on the ground, which Douse puts out.
  /// ★ Named from what the player sees: a flickering flame (spr_247.png)
  SPRITE_FIRE = 0xf7,
  /// The ivy Whirlwind blows away, in Goma Cave, Vault, Bilibin, Lunpa
  /// Fortress and Crossbone Isle, and a patch outside McCoy's Palace.
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  SPRITE_IVY = 0xf8,
  /// The pillars Move shifts in the Tunnel Ruins and the lighthouses.
  /// ★ Named from what the player sees: a stone pillar with a glowing yellow
  /// cap (its render)
  SPRITE_CAPPED_PILLAR = 0xfd,
  /// The pillars Move, Carry and pushes shift in Venus Lighthouse and on the
  /// Road to Babi Lighthouse. Animations 1 and 2 show a grey and an orange
  /// spiral on top; 3 to 8 a piece of gold track instead, along which Venus
  /// Lighthouse's lights run.
  /// ★ Named from what the player sees: a grey pillar with a spiral carved on
  /// top (spr_256.png)
  SPRITE_SPIRAL_PILLAR = 0x100,
  /// The platforms of Venus Lighthouse and the Tunnel Ruins that rise or sink
  /// with the weight set down on them.
  /// ★ Named from what the player sees: a small white pillar on its base
  /// (its render)
  SPRITE_BALANCE_PLATFORM = 0x101,
  /// The four statues around Venus Lighthouse's beacon.
  /// ★ Named from what the player sees: a statue of a woman with a green
  /// wreath, holding a sheaf (spr_258.png)
  SPRITE_STATUE_WITH_WREATH = 0x102,
  /// Mercury Lighthouse's lit beacon, and the orb at the top of Venus
  /// Lighthouse.
  /// ★ Named from what the player sees: a blue orb with white swirls, turning
  /// (spr_244.png)
  SPRITE_BLUE_ORB = 0xf4,
  /// The particles of Growth and of a Djinni's capture, and the gate effects
  /// of Venus Lighthouse.
  SPRITE_PARTICLE = 284,
  /// One sparkle: the glints of Halt and of the field's sparkles, and Kolima's
  /// curse dust.
  /// ★ Named from what the player sees: a sparkle of light that shrinks away
  /// (its render)
  SPRITE_SPARKLE = 285,
  /// ★ Named from what the player sees: a four-pointed glint that shrinks
  /// to a dot (spr_286.png)
  SPRITE_GLINT = 286,
  /// The whirlwinds of the Suhalla Desert, which Douse stops, and the one
  /// drawn on the world map beside it.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  SPRITE_SANDSTORM = 0x121,
  /// The lights of Altmiller Cave's rock puzzle, coloured by their palette.
  /// ★ Named from the game: "Lights will appear on the wall."
  /// (MsgAltmillerDontTrustAnyone)
  SPRITE_LIGHT = 0x124,
  /// The rocks of Altmiller Cave's rock puzzle, coloured by their palette
  /// while Reveal shines.
  /// ★ Named from the game: "Turn the five rocks in accordance with those
  /// colors." (MsgAltmillerDontTrustAnyone)
  SPRITE_TURNING_ROCK = 0x125,
  /// The boulders of Gondowan Cave that Lift raises.
  /// ★ Named from what the player sees: a round grey boulder (its render)
  SPRITE_ROUND_BOULDER = 0x127,
  /// What the party pushes in Altmiller Cave's lit rooms.
  /// ★ Named from what the player sees: a white stone column lying north to
  /// south (its render)
  SPRITE_FALLEN_COLUMN_NORTH_SOUTH = 0x129,
  /// ★ Named from what the player sees: a white stone column lying west to
  /// east (its render)
  SPRITE_FALLEN_COLUMN_WEST_EAST = 0x12a,
  /// The two halves of Babi Lighthouse's stone door, which slide apart.
  /// ★ Named from what the player sees: half of a dark stone door (its
  /// render)
  SPRITE_STONE_DOOR_HALF = 0x128,
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
  /// Tret's face again: SPRITE_TRET's pictures and animations drawn 12 pixels
  /// higher (sprites.tsv's SPR_071), as Kolima Forest shows him.
  SPRITE_TRET_RAISED = 0x47,
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
  /// (LOCALID_INVISIBLE_BABI).
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
  /// A second number for SPRITE_BALD_MAN_IN_RED, whose frames and animations
  /// it borrows (sprites.tsv), placed in Kalay's houses and Hammet Palace.
  /// ★ Named from what the player sees: a bald man in a red tunic with green
  /// sleeves (spr_124.png)
  SPRITE_BALD_MAN_IN_RED_2 = 0x7e,
  /// A second number for SPRITE_YOUNG_MAN_IN_GREEN, whose frames and
  /// animations it borrows (sprites.tsv).
  /// ★ Named from what the player sees: a young man with brown hair in a
  /// green shirt (spr_125.png)
  SPRITE_YOUNG_MAN_IN_GREEN_2 = 0x7f,
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
  /// SPRITE_WAGON drawn at 64 pixels, its back loaded with yellow crates:
  /// Bunza's wagon at the Kalay Tunnel Entrance.
  /// ★ Named from what the player sees: a large covered wagon drawn by a
  /// horse, full of crates (spr_273.png)
  SPRITE_LARGE_WAGON = 0x111,
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
  SPRITE_MAN_CARRYING_A_PACK = 0xab,

  // The world map's pictures of places and what it shows between them.

  /// ★ Named from what the player sees: houses with orange roofs among trees
  /// (spr_261.png)
  SPRITE_WORLD_MAP_VILLAGE = 0x105,
  /// ★ Named from what the player sees: white and blue buildings inside a wall
  /// (spr_263.png)
  SPRITE_WORLD_MAP_TOWN = 0x107,
  /// ★ Named from what the player sees: houses built into great tree stumps
  /// (spr_264.png)
  SPRITE_WORLD_MAP_STUMP_VILLAGE = 0x108,
  /// ★ Named from what the player sees: houses under snow among snowy trees
  /// (spr_265.png)
  SPRITE_WORLD_MAP_SNOWY_VILLAGE = 0x109,
  /// ★ Named from what the player sees: buildings with blue roofs among trees
  /// (spr_266.png)
  SPRITE_WORLD_MAP_BLUE_ROOFED_TOWN = 0x10a,
  /// ★ Named from what the player sees: a temple with a blue roof by a
  /// waterfall (spr_267.png)
  SPRITE_WORLD_MAP_TEMPLE = 0x10b,
  /// ★ Named from what the player sees: a grey rock arch over a cave mouth
  /// (spr_268.png)
  SPRITE_WORLD_MAP_CAVE = 0x10c,
  /// ★ Named from what the player sees: a tall blue-green tower, where Mercury
  /// Lighthouse's exit is (spr_269.png)
  SPRITE_WORLD_MAP_MERCURY_LIGHTHOUSE = 0x10d,
  /// ★ Named from what the player sees: a wooden barricade (spr_270.png)
  SPRITE_WORLD_MAP_BARRICADE = 0x10e,
  /// ★ Named from what the player sees: a snow-capped mountain (spr_271.png)
  SPRITE_WORLD_MAP_MOUNTAIN = 0x10f,
  /// ★ Named from what the player sees: a pink and white tower, where Babi
  /// Lighthouse's road begins (spr_272.png)
  SPRITE_WORLD_MAP_BABI_LIGHTHOUSE = 0x110,
  /// ★ Named from what the player sees: a clump of tall green trees
  /// (spr_275.png)
  SPRITE_WORLD_MAP_FOREST = 0x113,
  /// ★ Named from what the player sees: a tall purple and white tower, where
  /// Venus Lighthouse stands (spr_276.png)
  SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE = 0x114,
  /// ★ Named from what the player sees: a pier with a boat moored at it
  /// (spr_277.png)
  SPRITE_WORLD_MAP_DOCK = 0x115,
  /// A drawbridge over a river: raised in its first animation, let down
  /// across the river in its second.
  /// ★ Named from what the player sees: a drawbridge raised between its posts,
  /// then lying across the river (spr_278.png)
  SPRITE_WORLD_MAP_BRIDGE = 0x116,
  /// ★ Named from what the player sees: a grey gatehouse between green walls
  /// (spr_279.png)
  SPRITE_WORLD_MAP_GATEHOUSE = 0x117,
  /// One of the two kinds of obstacle that move in Tolbi Spring while a token
  /// flies over it.
  /// ★ Named from what the player sees: a red crab (spr_396.png)
  SPRITE_CRAB = 0x18c,
  /// The other kind of Tolbi Spring obstacle.
  /// ★ Named from what the player sees: a green turtle with an orange shell
  /// (spr_397.png)
  SPRITE_TURTLE = 0x18d,
  /// The coin tossed into Tolbi Spring, spinning, with its shadow and its
  /// sparkle when it settles; the toss draws it when the player tosses a coin.
  /// ★ Named from the game: "Face away from Tolbi Spring and toss a coin into
  /// it." (MsgTolbiFaceAwayTolbi)
  SPRITE_SPRING_COIN = 0x18e,
  /// The Lucky Medal tossed into Tolbi Spring, drawn as SPRITE_SPRING_COIN in
  /// gold.
  /// ★ Named from the game: "If you toss a Lucky Medal"
  /// (MsgTolbiTossLuckyMedal)
  SPRITE_SPRING_LUCKY_MEDAL = 0x18f,
  /// ★ Named from what the player sees: the wreck of a ship lying in the water
  /// (spr_501.png)
  SPRITE_SHIPWRECK = 0x1f5,
  /// ★ Named from what the player sees: a wooden gangway with rails, laid from
  /// the wharf over Babi's ship (spr_502.png)
  SPRITE_GANGWAY = 0x1f6,
  /// ★ Named from what the player sees: a drop of water that swells and bursts
  /// into a ring of spray (spr_503.png)
  SPRITE_WATER_DROP = 0x1f7,
  /// ★ Named from what the player sees: an island of green hills around a
  /// mountain (spr_504.png)
  SPRITE_WORLD_MAP_ISLAND = 0x1f8
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

/// SPRITE_BABI's animations that maps set by number (spr_052.tsv).
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
