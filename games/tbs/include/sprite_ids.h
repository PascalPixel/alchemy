#ifndef ALCHEMY_SPRITE_IDS_H
#define ALCHEMY_SPRITE_IDS_H

/// @file
/// Sprite numbers, named where the code shows what they are. A sprite's
/// picture is graphics/sprite/<name>.png, its id's name in lowercase without
/// SPRITE_ (isaac.png for SPRITE_ISAAC).

/// Sprite ids, one for each row of the sprite bank (gSprite_AnimationMetadata,
/// in sprites.tsv's order) in order; a row the code doesn't name yet keeps its
/// number as its placeholder. A party member's field sprite shares its
/// character number, so the field draws a character id as its sprite. The
/// summons' figures, from SPRITE_ATALANTA on, are each summon's own sprite:
/// its event creates its objects of it (BattleEffect_SpawnObjects), each
/// playing one of its animations.
/// A row sprites.tsv leaves empty, with no size and no pictures, keeps its
/// number: it shows nothing to name.
enum SpriteId {
  SPRITE_ISAAC,
  SPRITE_GARET,
  SPRITE_IVAN,
  SPRITE_MIA,
  SPRITE_004, ///< empty: Felix's character number, whose sprite is SPRITE_FELIX
  SPRITE_JENNA,
  SPRITE_SHEBA,
  SPRITE_007, ///< empty: Piers's character number
  SPRITE_008,
  SPRITE_009,
  SPRITE_010,
  SPRITE_011,
  SPRITE_012,
  SPRITE_013,
  SPRITE_014,
  SPRITE_015,
  /// The leader riding one of Altin's mine carts, added as a layer of the
  /// cart's sprite (AltinPeak_RollCart).
  SPRITE_CART_RIDER,
  /// Isaac's grown sprite while FLAG_ISAAC_WEARS_TUNIC is set.
  SPRITE_ISAAC_TUNIC,
  /// Isaac as a child, shown in his place while FLAG_CHILDHOOD is set.
  SPRITE_ISAAC_CHILD,
  /// Garet as a child, shown in his place while FLAG_CHILDHOOD is set.
  SPRITE_GARET_CHILD,
  /// A treasure chest, shut, opening and open, with the pictures of the mimic
  /// some chests turn out to be.
  SPRITE_CHEST,
  /// The bubbles an actor shows over its head (Actor_ShowEmote).
  SPRITE_EMOTE,
  /// A blank 16-pixel square: an actor that shows nothing, such as a
  /// shop
  /// counter, or that a map draws an item's icon on, as for the Nuts that
  /// Catch takes.
  SPRITE_BLANK,
  /// A blank 32-pixel square (blank_32.png holds no drawn pixel). Its one
  /// actor, an unused one in Bilibin, gives it no role, so it keeps
  /// its size as its name; SPRITE_BLANK_LARGE is the same size, used for
  /// actors seen only by their shadows.
  SPRITE_BLANK_32,
  /// Rings spreading out on water: where someone wades (the field's own
  /// FIELD_SPRITE_RIPPLE), and around the leader at Mercury Lighthouse's pools,
  /// grey in its first animation and blue in its second.
  SPRITE_RIPPLE,
  /// Nothing in the source names this sprite.
  SPRITE_FOOTPRINTS,
  /// The sparkles of Tret's glamour that rain on Kolima and its forest.
  SPRITE_GOLDEN_RINGS,
  /// The dark doorway added as a second part of the leader's sprite as the
  /// leader leaves through an exit (Exit_PrepareAnimation).
  SPRITE_DOORWAY,
  /// The Psynergy Stones a map's treasure spots place.
  SPRITE_PSYNERGY_STONE,
  SPRITE_029,
  SPRITE_KRADEN,
  /// The sprite of Dora, Isaac's mother.
  SPRITE_DORA,
  SPRITE_SATUROS,
  SPRITE_MENARDI,
  /// The sprite of Felix behind his mask.
  SPRITE_FELIX_MASKED,
  /// The sprite of Felix as a young man.
  SPRITE_FELIX,
  SPRITE_FELIX_CHILD,
  SPRITE_BOY_WITH_ORANGE_HAIR,
  SPRITE_WOMAN_IN_GREEN_HEADSCARF,
  SPRITE_FEIZHI,
  SPRITE_FEH,
  SPRITE_HSU,
  SPRITE_WOMAN_WITH_ORANGE_PONYTAIL,
  SPRITE_ALEX,
  SPRITE_DONPA,
  SPRITE_LAYANA,
  SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN,
  /// The sprite of Kaja, the Tolbi ship's fighter.
  SPRITE_KAJA,
  SPRITE_JENNA_CHILD,
  SPRITE_HAMMET,
  SPRITE_DODONPA,
  SPRITE_NYUNPA,
  SPRITE_BABI,
  SPRITE_KYLE,
  SPRITE_MAN_IN_TEAL_SHIRT,
  SPRITE_TRET,
  SPRITE_LAUREL,
  SPRITE_IODEM,
  /// The sprite of McCoy.
  SPRITE_MCCOY,
  SPRITE_MAN_IN_GREEN_SHIRT,
  SPRITE_WOMAN_WITH_PINK_HAIR,
  SPRITE_HAMA,
  SPRITE_WARRIOR_IN_GOLDEN_HELMET,
  SPRITE_MAN_WITH_LONG_DARK_HAIR,
  SPRITE_PERSON_IN_PURPLE,
  /// Babi's second sheet, which Altmiller Cave places while he is invisible
  /// (ACTOR_INVISIBLE_BABI).
  SPRITE_INVISIBLE_BABI,
  SPRITE_YOUNG_MAN_WITH_BLUE_HAIR,
  SPRITE_YOUNG_MAN_WITH_RED_HAIR,
  SPRITE_BUNZA,
  SPRITE_WOMAN_WITH_PURPLE_HAIR,
  SPRITE_FARAN,
  /// Tret's face again: SPRITE_TRET's pictures and animations drawn 12 pixels
  /// higher (sprites.tsv's TRET_RAISED), as Kolima Forest shows him.
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
  SPRITE_MERCURY_DJINNI,
  SPRITE_MARS_DJINNI,
  SPRITE_JUPITER_DJINNI,
  SPRITE_094,
  SPRITE_095,
  SPRITE_096,
  SPRITE_097,
  SPRITE_098,
  SPRITE_099,
  SPRITE_100,
  SPRITE_BURLY_MAN_WITH_GREY_HAIR,
  SPRITE_GIRL_WITH_PIGTAILS,
  SPRITE_OLD_MAN_WITH_CANE,
  SPRITE_DOG,
  SPRITE_TREE,
  SPRITE_WOMAN_WITH_FAIR_HAIR,
  SPRITE_BOY_WITH_FAIR_HAIR,
  SPRITE_YOUNG_MAN_WITH_BROWN_HAIR,
  SPRITE_STOUT_MAN,
  SPRITE_SMALL_TREE,
  SPRITE_OLD_WOMAN,
  SPRITE_OLD_MAN_IN_BLUE_CAP,
  SPRITE_MAN_WITH_GLASSES,
  SPRITE_BALD_MUSCULAR_MAN,
  SPRITE_WOMAN_IN_GREEN_DRESS,
  SPRITE_STRONGMAN,
  SPRITE_OLD_MAN_WITH_WHITE_HAIR,
  SPRITE_WOMAN_WITH_PONYTAIL,
  SPRITE_MAN_WITH_DARK_HAIR,
  SPRITE_WOMAN_IN_BLUE_DRESS,
  SPRITE_MAN_IN_TALL_HAT,
  SPRITE_BEARDED_MAN_IN_LONG_ROBE,
  SPRITE_COOK,
  SPRITE_BALD_MAN_IN_RED,
  SPRITE_YOUNG_MAN_IN_GREEN,
  /// A second number for SPRITE_BALD_MAN_IN_RED, whose frames and animations
  /// it borrows (sprites.tsv), so the player sees the same man. The first
  /// number keeps most towns' weapon shops; this one stands in Kalay's houses
  /// and in Hammet's palace.
  SPRITE_BALD_MAN_IN_RED_KALAY,
  /// A second number for SPRITE_YOUNG_MAN_IN_GREEN, whose frames and
  /// animations it borrows (sprites.tsv), so the player sees the same man. The
  /// first number keeps most towns' armor shops; this one stands in Kalay's
  /// houses and in Hammet's palace.
  SPRITE_YOUNG_MAN_IN_GREEN_KALAY,
  SPRITE_WOMAN_IN_PINK_APRON,
  SPRITE_MAN_WITH_LUTE,
  SPRITE_YOUTH_WITH_HEADBAND,
  SPRITE_YOUTH_WITH_TIED_HAIR,
  SPRITE_CHEF,
  SPRITE_MAN_WITH_ORANGE_HAIR,
  SPRITE_MAN_IN_ORANGE,
  SPRITE_MAN_WITH_PICKAXE,
  SPRITE_CAT,
  SPRITE_137,
  SPRITE_138,
  SPRITE_139,
  SPRITE_140,
  SPRITE_141,
  SPRITE_142,
  SPRITE_MAN_IN_WHITE_TURBAN,
  SPRITE_MAN_IN_RED_BANDANA,
  SPRITE_MAN_IN_BLUE_BANDANA,
  SPRITE_MAN_IN_YELLOW_BANDANA,
  SPRITE_MAN_IN_GREEN_BANDANA,
  SPRITE_YOUNG_WOMAN_IN_GREEN,
  SPRITE_MONK_IN_WHITE_AND_ORANGE,
  SPRITE_SOLDIER_IN_SPIKED_HELMET,
  SPRITE_BALD_BOY_IN_ORANGE,
  /// The soldiers of Babi's Tolbi and of the Colosseum.
  SPRITE_SOLDIER,
  SPRITE_SOLDIER_IN_TURBAN,
  SPRITE_WAGON,
  SPRITE_BIRD,
  SPRITE_YOUNG_MAN_IN_BLUE_JACKET,
  SPRITE_PERSON_WITH_SWORD,
  SPRITE_MONK_IN_ORANGE_ROBE,
  SPRITE_WOMAN_WITH_BLACK_HAIR,
  SPRITE_MAN_WITH_TOPKNOT,
  SPRITE_SHIP,
  SPRITE_WOMAN_IN_GREEN_APRON,
  SPRITE_SOLDIER_WITH_TORCH,
  SPRITE_OLD_MAN_IN_BLUE,
  SPRITE_YOUNG_MAN_WITH_FAIR_HAIR,
  SPRITE_OLD_WOMAN_WITH_CANE,
  SPRITE_MAN_IN_BLACK_SUIT,
  SPRITE_BOY_IN_GREEN,
  SPRITE_GIRL_IN_PINK,
  SPRITE_MUSCULAR_MAN_WITH_TOPKNOT,
  SPRITE_MAN_CARRYING_A_PACK,
  SPRITE_172, ///< empty; only Vault's unused Vault_SpawnDriftingEffect makes an
              ///< actor of it
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
  /// Toadonpa in Lunpa Fortress, before his battle.
  SPRITE_TOADONPA,
  /// The Tolbi-bound Ship's oars and the sea monsters that attack it.
  SPRITE_RESTING_OAR,
  SPRITE_JELLYFISH_MONSTER,
  /// The water beasts that flood Altin and its mine, whose battles name them
  /// Living Statues.
  SPRITE_WATER_BEAST,
  /// The apes of Mogall Forest, which Force brings out of their hiding places.
  SPRITE_APE,
  SPRITE_LIZARD_MAN,
  /// The Ant Lions of Lamakan Desert, whose battle is slot 4 of
  /// ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN.
  SPRITE_ANT_LION,
  /// The Kraken, which the party fights on the ship's deck.
  SPRITE_KRAKEN,
  /// The Manticore of Lamakan Desert, fought by talking to it
  /// (LamakanDesert_FightManticore).
  SPRITE_MANTICORE,
  SPRITE_PINK_WINGED_MONSTER,
  SPRITE_BLUE_WINGED_MONSTER,
  SPRITE_LIZARD_MONSTER,
  /// Altin Peak's Hydros Statue, before its battle.
  SPRITE_HYDROS_STATUE,
  /// A big round rock: the rocks that fall across Vale's paths on the stormy
  /// night, and the Boulder of Mt. Aleph drawn at twice its size.
  SPRITE_ROCK,
  /// The statues of Sol Sanctum's statue room and Luna and Sol rooms.
  SPRITE_WINGED_STATUE,
  /// The centre statue of Sol Sanctum's statue room.
  SPRITE_SUN_DISC_STATUE,
  SPRITE_STONE_STATUE,
  /// The centre statue once it has sunk into the floor, only its disc showing.
  SPRITE_SUN_DISC,
  /// A log lying north to south, one the leader can hop onto
  /// (Field_TryJumpForward) or roll, as in Kolima Forest.
  SPRITE_LOG_NORTH_SOUTH,
  /// The boards Isaac moves into place on a Vale roof (Vale_RoofMended).
  SPRITE_ROOF_BOARDS,
  SPRITE_LOG_WEST_EAST,
  SPRITE_FLAME_AND_SMOKE,
  /// The effect FieldEffect_SetAttached attaches to an actor
  /// or shows
  /// once, as the leader flinches on hazard ground.
  SPRITE_ATTACHED_EFFECT,
  SPRITE_CHEST_OF_VALUABLES,
  /// What Move pushes in Goma Cave and many rooms after it, which the game
  /// calls a stone pillar ("It's just a stone pillar.").
  SPRITE_STONE_PILLAR,
  SPRITE_STUMP,
  /// Nothing in the source names this sprite.
  SPRITE_HANDS,
  /// Spray thrown up where water is struck: where Goma Cave's stumps land,
  /// at Kolima Forest's floodgate, and over the Tolbi-bound Ship's side.
  SPRITE_SPLASH,
  /// The hand Move reaches out with; Goma Cave's entrance and Vale place it
  /// to push something in an event.
  SPRITE_MOVE_HAND,
  /// The sixteen crystals thrown out in a ring as Sol Sanctum erupts, and one
  /// on the world map.
  SPRITE_BLUE_CRYSTAL,
  /// The vines that appear around Growth's target.
  SPRITE_VINE,
  /// The whirlwind Whirlwind spins up.
  SPRITE_WHIRLWIND,
  /// Nothing in the source names this sprite.
  SPRITE_FLAME_BURST,
  /// The fields of Psynergy that rise over the party as Kolima's curse falls.
  SPRITE_PSYNERGY_FIELD,
  /// The gold statue stolen from Vault's Sanctum: it rises from a chest in
  /// log_rooms_a.c and stands in the Sanctum again.
  SPRITE_SANCTUM_GOLD_STATUE,
  /// A puff of dust kicked up where an actor stands; the water beasts of
  /// Altin
  /// and its mine spit it as spray, drawn SPRITE_COLOR_BGR or
  /// SPRITE_COLOR_WHITE.
  SPRITE_DUST_PUFF,
  SPRITE_CRATE,
  /// The statues of Mercury Lighthouse.
  SPRITE_BLUE_STATUE,
  SPRITE_OAR,
  /// Isaac's tunic: placed beside his bed on the stormy night, and drawn as a
  /// second layer over the child sprites of Isaac and Garet while
  /// FLAG_ISAAC_WEARS_TUNIC is set (Actor_RegisterSpriteMetadata).
  SPRITE_TUNIC,
  /// A puddle of water, which Frost freezes into a pillar of ice.
  SPRITE_PUDDLE,
  /// The stone pipe sections Move pushes in Mercury Lighthouse to join its
  /// channels, and the short upright pipes beside them.
  SPRITE_PIPE_WEST_EAST,
  /// The pipe sections Move pushes in Mercury Lighthouse, also placed in the
  /// Lighthouse Aerie of Venus Lighthouse.
  SPRITE_PIPE_NORTH_SOUTH,
  /// The rim of an upright pipe's open end, drawn over the end a pushed pipe
  /// section comes to rest against.
  SPRITE_PIPE_END,
  /// The carts on the rails of Altin's mine.
  SPRITE_MINE_CART,
  /// Frost's particles.
  SPRITE_SNOWFLAKE,
  /// The figures that spiral in around Ply's target.
  SPRITE_WINGED_FIGURE,
  SPRITE_SNOWMAN,
  SPRITE_235,
  /// The logs that float on the water of Fuchin Falls Cave, Crossbone Isle's
  /// floating logs room and the Colosso's Lumber Water Crossing Area.
  SPRITE_FLOATING_LOG_WEST_EAST,
  SPRITE_FLOATING_LOG_NORTH_SOUTH,
  /// The sprout Growth grows.
  SPRITE_SPROUT,
  /// The cloud Douse gathers over its target.
  SPRITE_RAIN_CLOUD,
  /// The drops of Douse's rain, and those a Mercury Djinni leaves as it goes.
  SPRITE_DROPLET,
  SPRITE_CRATES,
  /// The platforms that float in Mercury Lighthouse's aerie and that carry the
  /// leader up to Venus Lighthouse's.
  SPRITE_FLOATING_PLATFORM,
  /// The statues Move or a push slides in Bilibin and its Treasury.
  SPRITE_GOLDEN_STATUE,
  /// Mercury Lighthouse's lit beacon, and the orb at the top of Venus
  /// Lighthouse.
  SPRITE_BLUE_ORB,
  /// The keys the scenes show: the barricade key McCoy tosses to the party,
  /// and Lunpa Fortress's Cell Key.
  SPRITE_KEY,
  SPRITE_TORCH,
  /// A fire burning on the ground, which Douse puts out.
  SPRITE_FIRE,
  /// The ivy Whirlwind blows away, in Goma Cave, Vault, Bilibin, Lunpa
  /// Fortress and Crossbone Isle, and a patch outside McCoy's Palace.
  SPRITE_IVY,
  /// The leaves Whirlwind scatters.
  SPRITE_LEAF,
  /// Halt's stopwatch.
  SPRITE_STOPWATCH,
  /// The mayor's urn, which rises from the first chest in log_rooms_a.c.
  SPRITE_URN,
  SPRITE_WISE_ONE,
  /// The pillars Move shifts in the Tunnel Ruins and the lighthouses. All three
  /// animations draw one picture; in SPRITE_COLOR_GREY, as Venus Lighthouse,
  /// Babi Lighthouse Entry and the Tunnel Ruins' east pair show it, the whole
  /// pillar is grey and its cap pale grey in a white rim.
  SPRITE_CAPPED_PILLAR,
  /// The Tunnel Ruins' small pillars, which the leader pushes or shifts with
  /// Move.
  SPRITE_SMALL_PILLAR,
  /// The Tunnel Ruins' pillars that sink into the water.
  SPRITE_SINKING_PILLAR,
  /// The pillars Move, Carry and pushes shift in Venus Lighthouse and on the
  /// Road to Babi Lighthouse. Animations 1 and 2 show a grey and an orange
  /// spiral on top; 3 to 8 a piece of gold track instead, along which Venus
  /// Lighthouse's lights run.
  SPRITE_SPIRAL_PILLAR,
  /// The platforms of Venus Lighthouse and the Tunnel Ruins that rise or sink
  /// with the weight set down on them.
  SPRITE_BALANCE_PLATFORM,
  /// The four statues around Venus Lighthouse's beacon.
  SPRITE_STATUE_WITH_WREATH,
  /// The log Force knocks down onto the Board Walk in Satrage's match.
  SPRITE_FALLING_LOG,
  SPRITE_260,
  SPRITE_WORLD_MAP_VILLAGE,
  SPRITE_262,
  SPRITE_WORLD_MAP_TOWN,
  SPRITE_WORLD_MAP_STUMP_VILLAGE,
  SPRITE_WORLD_MAP_SNOWY_VILLAGE,
  SPRITE_WORLD_MAP_BLUE_ROOFED_TOWN,
  SPRITE_WORLD_MAP_TEMPLE,
  SPRITE_WORLD_MAP_CAVE,
  SPRITE_WORLD_MAP_MERCURY_LIGHTHOUSE,
  SPRITE_WORLD_MAP_BARRICADE,
  SPRITE_WORLD_MAP_MOUNTAIN,
  SPRITE_WORLD_MAP_BABI_LIGHTHOUSE,
  /// SPRITE_WAGON drawn at 64 pixels, its back loaded with yellow crates:
  /// Bunza's wagon at the Kalay Tunnel Entrance.
  SPRITE_LARGE_WAGON,
  /// The signs of Lalivero, Tolbi Spring, Altin Peak and Kolima Forest's
  /// floodgate, and the signposts on the world map.
  SPRITE_SIGNPOST,
  SPRITE_WORLD_MAP_FOREST,
  SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE,
  SPRITE_WORLD_MAP_DOCK,
  /// A drawbridge over a river: raised in its first animation, let down
  /// across the river in its second.
  SPRITE_WORLD_MAP_BRIDGE,
  SPRITE_WORLD_MAP_GATEHOUSE,
  /// The letters that rise from a sleeper: Garet's grandfather, and Isaac after
  /// the last Colosso match.
  SPRITE_LETTER_Z,
  /// The particles Mind Read sends from its target to the caster, also shot in
  /// Fuchin Temple.
  SPRITE_CRESCENT,
  SPRITE_282,
  /// The impact of Lift and of Xian's training tree, and the battle's hit flash
  /// (BattleMotion_PlayHitFlash, which nothing calls).
  SPRITE_STAR_BURST,
  /// The particles of Growth and of a Djinni's capture, and the gate effects
  /// of Venus Lighthouse.
  SPRITE_PARTICLE,
  /// One sparkle: the glints of Halt and of the field's sparkles, and Kolima's
  /// curse dust.
  SPRITE_SPARKLE,
  SPRITE_GLINT,
  /// The lights that float about the Elemental Star Room.
  SPRITE_FLOATING_LIGHT,
  /// A blank 32-pixel square: an actor seen only by its shadow, such as
  /// the
  /// shadows under Altin Peak's falling boulder and its Hydros statue.
  SPRITE_BLANK_LARGE,
  /// The whirlwinds of the Suhalla Desert, which Douse stops, and the one
  /// drawn on the world map beside it.
  SPRITE_SANDSTORM,
  /// The Board Walk's platforms, the Scales of Justice's ledges and the pillar
  /// that rises in a Crossbone Isle puddle.
  SPRITE_WHITE_PLATFORM,
  /// The Wall Wedge's pistons.
  SPRITE_PISTON,
  /// The lights of Altmiller Cave's rock puzzle, coloured by their palette.
  SPRITE_LIGHT,
  /// The rocks of Altmiller Cave's rock puzzle, coloured by their palette
  /// while Reveal shines.
  SPRITE_TURNING_ROCK,
  /// The log Force knocks off the Moving Sidewalk Stage.
  SPRITE_STANDING_LOG,
  /// The boulders of Gondowan Cave that Lift raises.
  SPRITE_ROUND_BOULDER,
  /// The two halves of Babi Lighthouse's stone door, which slide apart.
  SPRITE_STONE_DOOR_HALF,
  /// What the party pushes in Altmiller Cave's lit rooms.
  SPRITE_FALLEN_COLUMN_NORTH_SOUTH,
  SPRITE_FALLEN_COLUMN_WEST_EAST,
  /// Vault Cave's pillars.
  SPRITE_FLAME_PILLAR,
  /// Isaac in battle. The party members' battle sprites follow in character
  /// order, Felix's row left empty.
  SPRITE_BATTLE_ISAAC,
  SPRITE_BATTLE_GARET,
  SPRITE_BATTLE_IVAN,
  SPRITE_BATTLE_MIA,
  SPRITE_304,
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
  /// The weapons drawn in a party member's hand as they attack, from here to
  /// SPRITE_WEAPON_JENNA_BLADE: each member's gBattleUnit_WeaponAnimations
  /// table (battle_tables.s) gives one for each enum BattleWeaponKind. Isaac's
  /// for long swords, and for staffs, ankhs and anything the list lacks.
  SPRITE_WEAPON_ISAAC_SWORD,
  /// Garet's for maces, staffs, ankhs and anything the list lacks.
  SPRITE_WEAPON_GARET_MACE,
  /// Ivan's for staffs and anything the list lacks.
  SPRITE_WEAPON_IVAN_STAFF,
  /// Mia's for swords, light blades, ankhs and anything the list lacks.
  SPRITE_WEAPON_MIA_ANKH,
  SPRITE_324,
  /// Jenna's for axes, maces, staffs, ankhs and anything the list lacks.
  SPRITE_WEAPON_JENNA_STAFF,
  SPRITE_WEAPON_ISAAC_AXE,
  SPRITE_WEAPON_ISAAC_MACE,
  SPRITE_WEAPON_GARET_AXE,
  /// Garet's for long swords and light blades.
  SPRITE_WEAPON_GARET_SWORD,
  SPRITE_WEAPON_IVAN_ANKH,
  /// Ivan's for swords, light blades, axes and maces.
  SPRITE_WEAPON_IVAN_BLADE,
  SPRITE_WEAPON_MIA_STAFF,
  /// Mia's for axes and maces.
  SPRITE_WEAPON_MIA_MACE,
  SPRITE_WEAPON_ISAAC_LIGHT_BLADE,
  /// Unused: no gBattleUnit_WeaponAnimations table holds it, so no attack shows
  /// it; the player would have seen this sword in a party member's hand.
  SPRITE_WEAPON_UNUSED_SWORD,
  /// Jenna's for swords and light blades.
  SPRITE_WEAPON_JENNA_BLADE,
  SPRITE_337,
  SPRITE_338,
  SPRITE_339,
  /// The status icons over a unit 32 pixels wide, in the order of
  /// battle_objects.c's ICON_ bits (BattleStatusIcon_Cycle); from
  /// SPRITE_STATUS_WIDE_POISON on, the same over wider units.
  SPRITE_STATUS_POISON,
  SPRITE_STATUS_VENOM,
  SPRITE_STATUS_STUN,
  SPRITE_STATUS_PSY_SEAL,
  SPRITE_STATUS_SLEEP,
  SPRITE_STATUS_DELUSION,
  SPRITE_STATUS_HAUNT,
  /// The curse's turns left, one to seven, as that many flames.
  SPRITE_STATUS_GRIM_REAPER_1,
  SPRITE_STATUS_GRIM_REAPER_2,
  SPRITE_STATUS_GRIM_REAPER_3,
  SPRITE_STATUS_GRIM_REAPER_4,
  SPRITE_STATUS_GRIM_REAPER_5,
  SPRITE_STATUS_GRIM_REAPER_6,
  SPRITE_STATUS_GRIM_REAPER_7,
  SPRITE_354,
  SPRITE_STATUS_WIDE_POISON,
  SPRITE_STATUS_WIDE_VENOM,
  SPRITE_STATUS_WIDE_STUN,
  SPRITE_STATUS_WIDE_PSY_SEAL,
  SPRITE_STATUS_WIDE_SLEEP,
  SPRITE_STATUS_WIDE_DELUSION,
  SPRITE_STATUS_WIDE_HAUNT,
  SPRITE_STATUS_WIDE_GRIM_REAPER_1,
  SPRITE_STATUS_WIDE_GRIM_REAPER_2,
  SPRITE_STATUS_WIDE_GRIM_REAPER_3,
  SPRITE_STATUS_WIDE_GRIM_REAPER_4,
  SPRITE_STATUS_WIDE_GRIM_REAPER_5,
  SPRITE_STATUS_WIDE_GRIM_REAPER_6,
  SPRITE_STATUS_WIDE_GRIM_REAPER_7,
  SPRITE_369,
  SPRITE_ATALANTA,
  SPRITE_THOR,
  SPRITE_BOREAS,
  SPRITE_KIRIN,
  SPRITE_374,
  SPRITE_CYBELE,
  SPRITE_PROCNE,
  SPRITE_TIAMAT,
  SPRITE_NEPTUNE,
  SPRITE_RAMSES,
  SPRITE_NEREID,
  SPRITE_METEOR,
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
  SPRITE_BATTLE_ROCKS,
  SPRITE_391,
  SPRITE_392,
  SPRITE_393,
  SPRITE_394,
  SPRITE_395,
  /// One of the two kinds of obstacle that move in Tolbi Spring while a token
  /// flies over it.
  SPRITE_CRAB,
  /// The other kind of Tolbi Spring obstacle.
  SPRITE_TURTLE,
  /// The coin tossed into Tolbi Spring, spinning, with its shadow and its
  /// sparkle when it settles; the toss draws it when the player tosses a coin.
  SPRITE_SPRING_COIN,
  /// The Lucky Medal tossed into Tolbi Spring, drawn as SPRITE_SPRING_COIN in
  /// gold.
  SPRITE_SPRING_LUCKY_MEDAL,
  /// The enemies' battle sprites follow, gBattleSprites's by enemy id, each
  /// named for the first enemy that shows it; their pictures are
  /// graphics/monster's, by gSpritePictureCache_Sources.
  SPRITE_BATTLE_TROLL,
  SPRITE_BATTLE_ZOMBIE,
  SPRITE_BATTLE_HARPY,
  SPRITE_BATTLE_GOLEM,
  SPRITE_BATTLE_GHOST,
  SPRITE_405,
  SPRITE_406,
  SPRITE_407,
  SPRITE_BATTLE_GRYPHON,
  SPRITE_BATTLE_RAT_SOLDIER,
  SPRITE_BATTLE_GHOUL,
  SPRITE_BATTLE_CHIMERA,
  SPRITE_412,
  SPRITE_413,
  SPRITE_414,
  SPRITE_BATTLE_DEADBEARD,
  SPRITE_416,
  SPRITE_BATTLE_MANTICORE,
  SPRITE_418,
  SPRITE_419,
  SPRITE_420,
  SPRITE_BATTLE_KOBOLD,
  SPRITE_422,
  SPRITE_423,
  SPRITE_BATTLE_GARGOYLE,
  SPRITE_425,
  SPRITE_426,
  SPRITE_427,
  SPRITE_428,
  SPRITE_BATTLE_LIZARD_MAN,
  SPRITE_BATTLE_ORC,
  SPRITE_431,
  SPRITE_432,
  /// The Venus Djinn's, as Quartz and Vine fight.
  SPRITE_BATTLE_VENUS_DJINNI,
  SPRITE_BATTLE_JUPITER_DJINNI,
  SPRITE_BATTLE_MARS_DJINNI,
  SPRITE_BATTLE_MERCURY_DJINNI,
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
  SPRITE_BATTLE_DREAD_HOUND,
  SPRITE_449,
  SPRITE_BATTLE_MAULER,
  SPRITE_BATTLE_SIREN,
  SPRITE_452,
  SPRITE_BATTLE_VERMIN,
  SPRITE_BATTLE_WILD_MUSHROOM,
  SPRITE_BATTLE_SLIME,
  SPRITE_BATTLE_CUTTLE,
  SPRITE_BATTLE_BAT,
  SPRITE_BATTLE_RAT,
  SPRITE_BATTLE_STONE_SOLDIER,
  SPRITE_BATTLE_DRONE_BEE,
  SPRITE_BATTLE_SPIDER,
  SPRITE_BATTLE_MOLE,
  SPRITE_BATTLE_DIRGE,
  SPRITE_BATTLE_GRUB,
  SPRITE_BATTLE_APE,
  SPRITE_BATTLE_SALAMANDER,
  SPRITE_BATTLE_LIVING_STATUE,
  SPRITE_BATTLE_GNOME,
  SPRITE_BATTLE_SKELETON,
  SPRITE_BATTLE_WILL_HEAD,
  SPRITE_BATTLE_TORNADO_LIZARD,
  SPRITE_BATTLE_ANT_LION,
  SPRITE_BATTLE_MIMIC,
  SPRITE_BATTLE_TOADONPA,
  SPRITE_BATTLE_AMAZE,
  /// The Kraken's left half; SPRITE_BATTLE_KRAKEN_RIGHT is drawn 64 pixels to
  /// its right (sprites.tsv).
  SPRITE_BATTLE_KRAKEN,
  SPRITE_BATTLE_KRAKEN_RIGHT,
  SPRITE_BATTLE_SATUROS,
  SPRITE_BATTLE_MENARDI,
  /// Isaac's battle sprite while he holds a Machete, drawn as part of it.
  SPRITE_ISAAC_MACHETE,
  SPRITE_BATTLE_TRET,
  /// Garet's battle sprite while he holds a Machete, drawn as part of it.
  SPRITE_GARET_MACHETE,
  /// The Fusion Dragon's left half; SPRITE_BATTLE_FUSION_DRAGON_RIGHT is drawn
  /// 64 pixels to its right (sprites.tsv).
  SPRITE_BATTLE_FUSION_DRAGON,
  SPRITE_BATTLE_FUSION_DRAGON_RIGHT,
  /// The Colosso finalists Azart, Satrage and Navampa in battle.
  SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
  SPRITE_BATTLE_BANDIT,
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
  SPRITE_LADY_STATUE,
  SPRITE_SHIPWRECK,
  SPRITE_GANGWAY,
  SPRITE_WATER_DROP,
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
  SPRITE_BATTLE_SHADOW,
  /// The shadow a body 32 pixels wide takes instead of SPRITE_BATTLE_SHADOW.
  SPRITE_BATTLE_SHADOW_NARROW
};

/// SPRITE_STUMP's animations beyond its standing frame (stump.tsv).
enum StumpAnimation {
  /// Sinks until only its cut top shows, level with the ground or the water,
  /// which leaves it afloat in Gondowan Cave's river.
  STUMP_ANIMATION_SINK = 2,
  STUMP_ANIMATION_SPLASH = 3 ///< drops into water with a splash
};

/// SPRITE_PUDDLE's animations (puddle.tsv): the puddle, the ice pillar Frost
/// makes of it, and the steps between.
enum PuddleAnimation {
  PUDDLE_ANIMATION_PUDDLE = 1,
  PUDDLE_ANIMATION_FREEZE = 2,  ///< rises from the puddle into an ice pillar
  PUDDLE_ANIMATION_LOW_ICE = 3, ///< held at the second step of freezing
  PUDDLE_ANIMATION_PILLAR = 5,  ///< the ice pillar, still
  PUDDLE_ANIMATION_MELT = 6     ///< sinks from the pillar back into a puddle
};

/// SPRITE_WATER_BEAST's animations beyond its standing frame (water_beast.tsv).
enum WaterBeastAnimation {
  WATER_BEAST_ANIMATION_LEAP = 2, ///< stretched out, legs down, as in mid-jump
  /// Two low crouches in turn, a quarter of a second each: as one spits
  /// water, and between its high jumps.
  WATER_BEAST_ANIMATION_SPIT = 3
};

/// SPRITE_WARRIOR_IN_GOLDEN_HELMET's animations at the end of a Colosso
/// finals match (warrior_in_golden_helmet.tsv).
enum GoldenHelmetWarriorAnimation {
  WARRIOR_ANIMATION_DEFEATED = 8,        ///< slumped to the ground
  WARRIOR_ANIMATION_FIGHTING_STANCE = 9, ///< braced to fight
  WARRIOR_ANIMATION_ARMS_RAISED = 10     ///< both arms raised in victory
};

/// SPRITE_BABI's animations that scenes set by number (babi.tsv).
enum BabiAnimation {
  /// Lying in bed, only his head on the pillow showing, its two frames
  /// alternating.
  BABI_ANIMATION_IN_BED = 5
};

/// SPRITE_LOG_NORTH_SOUTH's animations (log_north_south.tsv) as a log tips into
/// a river
/// (places/kolima-forest-entrance/events/KolimaForestEntrance_PushLog.gif).
enum LogAnimation {
  LOG_ANIMATION_ROLL_WEST = 3, ///< rolling over toward the west
  LOG_ANIMATION_FLOAT = 4,     ///< bobbing in the water
  /// Drops into the water with a ring of spray, then LOG_ANIMATION_FLOAT.
  LOG_ANIMATION_DROP_IN = 8
};

/// SPRITE_SPLASH's animations (splash.tsv), as Kolima Forest's floodgate
/// plays them
/// (places/kolima-forest-floodgate/events/KolimaForestFloodgate_FlipFloodgateSwitch.gif).
enum SplashAnimation {
  SPLASH_ANIMATION_BURST = 1, ///< one burst of spray, then nothing
  /// Spray churning for about 120 frames, then dying away to nothing.
  SPLASH_ANIMATION_CHURN = 2
};

/// The face animations of SPRITE_TRET and SPRITE_TRET_RAISED (tret.tsv):
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

/// SPRITE_LAUREL's face animations (laurel.tsv), as TretFaceAnimation's first
/// five.
enum LaurelFaceAnimation {
  LAUREL_FACE_OPEN = 1,
  LAUREL_FACE_HALF_CLOSED = 2,
  LAUREL_FACE_CLOSED = 3,
  LAUREL_FACE_LOOK_RIGHT = 4,
  LAUREL_FACE_LOOK_LEFT = 5
};

/// SPRITE_CHEST's animations (chest.tsv).
enum ChestAnimation {
  CHEST_ANIMATION_OPEN_EMPTY = 2, ///< the lid up over an empty chest, held
  /// The lid lifting on treasure, then open over it as it glints.
  CHEST_ANIMATION_OPEN_ON_TREASURE = 4,
  /// The lid lifted on the treasure for six frames, then shut again (the
  /// sprite's animation 1, closed).
  CHEST_ANIMATION_PEEK_AND_SHUT = 5
};

/// SPRITE_SHEBA's animations beyond enum ActorAnimation (sheba.tsv).
enum ShebaAnimation {
  /// Hanging from a ledge by one hand, seen from the side.
  SHEBA_ANIMATION_HANG_ONE_HAND = 5,
  SHEBA_ANIMATION_HANG = 6, ///< hanging by both hands, seen from behind
  /// Her grip slipping for a moment, then SHEBA_ANIMATION_HANG again.
  SHEBA_ANIMATION_SLIP = 7,
  SHEBA_ANIMATION_LIE = 8 ///< lying on her back
};

/// SPRITE_ISAAC_CHILD's animations beyond enum ActorAnimation
/// (isaac_child.tsv).
enum IsaacChildAnimation {
  ISAAC_CHILD_ANIMATION_RECOIL = 17, ///< arms flung out
  /// Asleep, only his head on the pillow.
  ISAAC_CHILD_ANIMATION_ASLEEP = 18,
  ISAAC_CHILD_ANIMATION_GET_UP = 19 ///< sitting up, then jumping out of bed
};

/// SPRITE_DORA's animations beyond enum ActorAnimation (dora.tsv).
enum DoraAnimation {
  DORA_ANIMATION_CLIMB = 5,          ///< climbing a ladder, seen from behind
  DORA_ANIMATION_CLIMB_OVER_TOP = 6, ///< stepping over the top of it
  DORA_ANIMATION_HANDS_UP = 7,       ///< both hands raised
  /// Only her head and hands showing, as she peeks back out.
  DORA_ANIMATION_PEEK = 8,
  DORA_ANIMATION_RUN = 9,     ///< in mid-stride
  DORA_ANIMATION_IN_BED = 10, ///< lying down, ill
  /// Seen from behind, her head bowed and turned aside.
  DORA_ANIMATION_HEAD_TURNED = 11,
  DORA_ANIMATION_HEAD_BOWED = 12, ///< seen from behind, her head bowed low
  /// DORA_ANIMATION_HEAD_TURNED and DORA_ANIMATION_HEAD_BOWED in turn, twice.
  DORA_ANIMATION_HEAD_SWAYING = 13,
  DORA_ANIMATION_STRAIGHTEN = 14 ///< straightening, still seen from behind
};

/// SPRITE_SATUROS's animations beyond enum ActorAnimation (saturos.tsv).
enum SaturosAnimation {
  /// Standing, two pictures alternating at long intervals.
  SATUROS_ANIMATION_BREATHE = 8,
  SATUROS_ANIMATION_LIE = 9, ///< lying on his side
  /// Falling into the lighthouse: upright, then half his height, then only
  /// his top above the floor.
  SATUROS_ANIMATION_TIP = 10,
  SATUROS_ANIMATION_SLUMP = 11,
  SATUROS_ANIMATION_SINK = 12
};

/// SPRITE_FELIX's animations beyond enum ActorAnimation (felix.tsv).
enum FelixAnimation {
  FELIX_ANIMATION_KNEEL = 5, ///< crouched on one knee
  FELIX_ANIMATION_LIE = 6    ///< lying face down, one arm out
};

/// SPRITE_OLD_MAN_WITH_CANE's animations beyond enum ActorAnimation
/// (old_man_with_cane.tsv).
enum OldManWithCaneAnimation {
  /// Lying in bed, only his head above the covers.
  OLD_MAN_WITH_CANE_ANIMATION_LIE_IN_BED = 5,
  OLD_MAN_WITH_CANE_ANIMATION_SIT_UP = 7, ///< sitting up in bed
  /// Lying back down, then OLD_MAN_WITH_CANE_ANIMATION_LIE_IN_BED.
  OLD_MAN_WITH_CANE_ANIMATION_LIE_BACK = 8,
  OLD_MAN_WITH_CANE_ANIMATION_ROW = 9 ///< pulling an oar from a seat
};

/// SPRITE_YOUNG_MAN_WITH_BROWN_HAIR's animations beyond enum ActorAnimation
/// (young_man_with_brown_hair.tsv).
enum YoungManWithBrownHairAnimation {
  /// Sitting bound, squirming.
  YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_TIED_UP = 7,
  /// Seen from behind, both arms raised against something before him.
  YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_HOLD_BACK = 8,
  /// Facing the player, his hands raised to his mouth to call out.
  YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_CALL_OUT = 9,
  YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_ROW = 10 ///< pulling an oar from a seat
};

/// SPRITE_STRONGMAN's animations beyond enum ActorAnimation (strongman.tsv).
enum StrongmanAnimation {
  STRONGMAN_ANIMATION_RUN = 5,      ///< in mid-stride
  STRONGMAN_ANIMATION_SPRAWLED = 6, ///< fallen back on the ground
  /// Propped up on one hand, the two facing opposite ways.
  STRONGMAN_ANIMATION_PROPPED_UP = 7,
  STRONGMAN_ANIMATION_PROPPED_UP_OTHER_WAY = 8,
  STRONGMAN_ANIMATION_SQUATS = 9 ///< his knees bending, over and over
};

/// SPRITE_TORCH's animations (torch.tsv).
enum TorchAnimation {
  TORCH_ANIMATION_UNLIT = 1, ///< a cold torch, its bowl grey with ash
  TORCH_ANIMATION_LIT = 2,   ///< a flame flickering in the bowl
  TORCH_ANIMATION_GO_OUT =
      3 ///< the flame dies in smoke, then TORCH_ANIMATION_UNLIT
};

/// SPRITE_MOVE_HAND's animations (move_hand.tsv).
enum MoveHandAnimation {
  MOVE_HAND_ANIMATION_OPEN = 1, ///< open, its fingers spread
  MOVE_HAND_ANIMATION_GRIP = 3, ///< its fingers bent as if gripping
  /// MOVE_HAND_ANIMATION_OPEN and MOVE_HAND_ANIMATION_GRIP in turn, as it
  /// strains against what it pushes.
  MOVE_HAND_ANIMATION_FLEX = 4,
  MOVE_HAND_ANIMATION_FIST = 5,
  /// Open, then gripping, then MOVE_HAND_ANIMATION_FIST, as it closes on what
  /// Catch catches.
  MOVE_HAND_ANIMATION_CLENCH = 6
};

#endif
