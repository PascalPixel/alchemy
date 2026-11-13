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
  /// ★ Named from what the player sees: Isaac (isaac.png)
  SPRITE_ISAAC,
  /// ★ Named from what the player sees: Garet (garet.png)
  SPRITE_GARET,
  /// ★ Named from what the player sees: Ivan (ivan.png)
  SPRITE_IVAN,
  /// ★ Named from what the player sees: Mia (mia.png)
  SPRITE_MIA,
  SPRITE_004, ///< empty: Felix's character number, whose sprite is SPRITE_FELIX
  /// ★ Named from what the player sees: Jenna (jenna.png)
  SPRITE_JENNA,
  /// ★ Named from what the player sees: Sheba (sheba.png)
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
  /// ★ Named from what the player sees: Isaac's head and shoulders over the
  /// cart's side, turned every way (cart_rider.png)
  SPRITE_CART_RIDER,
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
  /// The bubbles an actor shows over its head (Actor_ShowEmote).
  /// ★ Named from what the player sees: speech bubbles holding a face, an
  /// exclamation mark, a question mark, a heart, an angry face and a light bulb
  /// (emote.png)
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
  /// ★ Named from what the player sees: rings spreading on water
  /// (ripple.png)
  SPRITE_RIPPLE,
  /// Nothing in the source names this sprite.
  /// ★ Named from what the player sees: small white prints, one and then a pair
  /// (footprints.png)
  SPRITE_FOOTPRINTS,
  /// The sparkles of Tret's glamour that rain on Kolima and its forest.
  /// ★ Named from what the player sees: rings of golden light (golden_rings.png)
  SPRITE_GOLDEN_RINGS,
  /// The dark doorway added as a second part of the leader's sprite as the
  /// leader leaves through an exit (Exit_PrepareAnimation).
  /// ★ Named from what the player sees: a black arched doorway (doorway.png)
  SPRITE_DOORWAY,
  /// The Psynergy Stones a map's treasure spots place.
  /// ★ Named from the game: "a Psynergy Stone." (MsgGotPsynergyStone)
  SPRITE_PSYNERGY_STONE,
  SPRITE_029,
  /// The sprite of Kraden.
  /// ★ Named from what the player sees: an old man with white hair and a
  /// white beard in a brown robe (kraden.png)
  SPRITE_KRADEN,
  /// The sprite of Dora, Isaac's mother.
  /// ★ Named from what the player sees: a woman with fair hair tied up, a
  /// blue top and a pink striped skirt (dora.png)
  SPRITE_DORA,
  /// The sprite of Saturos.
  /// ★ Named from what the player sees: a blue-skinned man with pale blue
  /// hair in a long blue cloak (saturos.png)
  SPRITE_SATUROS,
  /// The sprite of Menardi.
  /// ★ Named from what the player sees: a woman with long fair hair in a red
  /// outfit (menardi.png)
  SPRITE_MENARDI,
  /// The sprite of Felix behind his mask.
  /// ★ Named from what the player sees: a young man with brown hair, his face
  /// behind a green mask, in a green cloak (felix_masked.png)
  SPRITE_FELIX_MASKED,
  /// The sprite of Felix as a young man.
  /// ★ Named from what the player sees: a young man with brown hair in a
  /// green cloak (felix.png)
  SPRITE_FELIX,
  /// Felix as a child.
  SPRITE_FELIX_CHILD,
  /// ★ Named from what the player sees: a small boy with spiky orange hair in
  /// a green shirt and shorts (boy_with_orange_hair.png)
  SPRITE_BOY_WITH_ORANGE_HAIR,
  /// ★ Named from what the player sees: a woman with orange hair under a
  /// green headscarf, in a blue top (woman_in_green_headscarf.png)
  SPRITE_WOMAN_IN_GREEN_HEADSCARF,
  /// The sprite of Feizhi.
  /// ★ Named from what the player sees: a girl with purple hair tied up with
  /// a bow, in a yellow and blue dress (feizhi.png)
  SPRITE_FEIZHI,
  /// The sprite of Feh.
  /// ★ Named from what the player sees: a young man with black hair in a
  /// yellow and green shirt (feh.png)
  SPRITE_FEH,
  /// The sprite of Hsu.
  /// ★ Named from what the player sees: a young man with brown hair in an
  /// orange jacket and green trousers (hsu.png)
  SPRITE_HSU,
  /// ★ Named from what the player sees: a young woman with an orange ponytail
  /// in a green and purple dress (woman_with_orange_ponytail.png)
  SPRITE_WOMAN_WITH_ORANGE_PONYTAIL,
  /// The sprite of Alex.
  /// ★ Named from what the player sees: a young man with long pale blue hair
  /// in a white and purple outfit (alex.png)
  SPRITE_ALEX,
  /// The sprite of Donpa.
  /// ★ Named from what the player sees: an old man with long grey hair in a
  /// yellow robe, with a staff (donpa.png)
  SPRITE_DONPA,
  /// The sprite of Layana.
  /// ★ Named from what the player sees: a woman with long curly red hair in a
  /// green dress (layana.png)
  SPRITE_LAYANA,
  /// ★ Named from what the player sees: an old man with white hair and a
  /// white beard in green clothes (man_with_white_beard_in_green.png)
  SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN,
  /// The sprite of Kaja, the Tolbi ship's fighter.
  /// ★ Named from what the player sees: a bald man with a beard in a green
  /// apron (kaja.png)
  SPRITE_KAJA,
  /// Jenna as a child.
  SPRITE_JENNA_CHILD,
  /// The sprite of Hammet.
  /// ★ Named from what the player sees: a man in a turban and orange and
  /// yellow robes (hammet.png)
  SPRITE_HAMMET,
  /// The sprite of Dodonpa.
  /// ★ Named from what the player sees: a man with spiky dark blue hair and a
  /// moustache in a white top and a red cloak (dodonpa.png)
  SPRITE_DODONPA,
  /// The sprite of Nyunpa.
  /// ★ Named from what the player sees: an old man with white hair and a
  /// white beard in an orange robe (nyunpa.png)
  SPRITE_NYUNPA,
  /// The sprite of Babi.
  /// ★ Named from what the player sees: an old man with long pink hair in a
  /// red and gold robe, with a cane (babi.png)
  SPRITE_BABI,
  /// The sprite of Kyle.
  /// ★ Named from what the player sees: a man with fair hair in a blue and
  /// white top and green trousers (kyle.png)
  SPRITE_KYLE,
  /// ★ Named from what the player sees: a man with brown hair in a teal shirt
  /// and brown trousers (man_in_teal_shirt.png)
  SPRITE_MAN_IN_TEAL_SHIRT,
  /// The sprite of Tret.
  /// ★ Named from what the player sees: a great stone-coloured face with a
  /// heavy brow (tret.png)
  SPRITE_TRET,
  /// The sprite of Laurel.
  /// ★ Named from what the player sees: an orange face with green eyes
  /// (laurel.png)
  SPRITE_LAUREL,
  /// The sprite of Iodem.
  /// ★ Named from what the player sees: a man with grey hair and a grey beard
  /// in a blue tunic and a yellow cape (iodem.png)
  SPRITE_IODEM,
  /// The sprite of McCoy.
  /// ★ Named from what the player sees: a stout man with red hair in a pink
  /// tunic (mccoy.png)
  SPRITE_MCCOY,
  /// ★ Named from what the player sees: a man with brown hair in a green
  /// shirt and blue trousers (man_in_green_shirt.png)
  SPRITE_MAN_IN_GREEN_SHIRT,
  /// ★ Named from what the player sees: a woman with pink hair tied up, in a
  /// white apron and a blue skirt (woman_with_pink_hair.png)
  SPRITE_WOMAN_WITH_PINK_HAIR,
  /// The sprite of Hama.
  /// ★ Named from what the player sees: a woman with long purple hair in a
  /// white and green robe (hama.png)
  SPRITE_HAMA,
  /// ★ Named from what the player sees: a warrior in a golden helmet with a
  /// red crest (warrior_in_golden_helmet.png)
  SPRITE_WARRIOR_IN_GOLDEN_HELMET,
  /// ★ Named from what the player sees: a man with long dark hair in a green
  /// and orange outfit (man_with_long_dark_hair.png)
  SPRITE_MAN_WITH_LONG_DARK_HAIR,
  /// ★ Named from what the player sees: a person with long dark purple hair
  /// in purple clothes (person_in_purple.png)
  SPRITE_PERSON_IN_PURPLE,
  /// Babi's second sheet, which Altmiller Cave places while he is invisible
  /// (ACTOR_INVISIBLE_BABI).
  /// ★ Named from what the player sees: an old man with long pink hair in a
  /// red and gold robe, with a cane (invisible_babi.png)
  SPRITE_INVISIBLE_BABI,
  /// ★ Named from what the player sees: a young man with spiky blue hair and
  /// a yellow headband in white (young_man_with_blue_hair.png)
  SPRITE_YOUNG_MAN_WITH_BLUE_HAIR,
  /// ★ Named from what the player sees: a young man with spiky red hair and a
  /// yellow headband (young_man_with_red_hair.png)
  SPRITE_YOUNG_MAN_WITH_RED_HAIR,
  /// The sprite of Bunza.
  /// ★ Named from what the player sees: a man with dark hair combed back in a
  /// blue tunic (bunza.png)
  SPRITE_BUNZA,
  /// ★ Named from what the player sees: a woman with curly purple hair in a
  /// pink and purple dress (woman_with_purple_hair.png)
  SPRITE_WOMAN_WITH_PURPLE_HAIR,
  /// The sprite of Faran.
  /// ★ Named from what the player sees: a man with long fair hair in an
  /// orange and brown robe (faran.png)
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
  /// arms folded, in a dark vest (burly_man_with_grey_hair.png)
  SPRITE_BURLY_MAN_WITH_GREY_HAIR,
  /// ★ Named from what the player sees: a little girl with orange pigtails in
  /// a green dress (girl_with_pigtails.png)
  SPRITE_GIRL_WITH_PIGTAILS,
  /// ★ Named from what the player sees: a bent old man with a bald head and a
  /// white beard in a green robe, with a cane (old_man_with_cane.png)
  SPRITE_OLD_MAN_WITH_CANE,
  /// ★ Named from what the player sees: a small white dog with brown ears
  /// (dog.png)
  SPRITE_DOG,
  /// ★ Named from what the player sees: a tree with a full green crown
  /// (tree.png)
  SPRITE_TREE,
  /// ★ Named from what the player sees: a woman with long fair hair and a
  /// blue headband in a white apron (woman_with_fair_hair.png)
  SPRITE_WOMAN_WITH_FAIR_HAIR,
  /// ★ Named from what the player sees: a small boy with fair hair in a blue
  /// shirt (boy_with_fair_hair.png)
  SPRITE_BOY_WITH_FAIR_HAIR,
  /// ★ Named from what the player sees: a young man with brown hair in a purple
  /// and white shirt (young_man_with_brown_hair.png)
  SPRITE_YOUNG_MAN_WITH_BROWN_HAIR,
  /// ★ Named from what the player sees: a stout man with brown hair in an
  /// orange and blue tunic (stout_man.png)
  SPRITE_STOUT_MAN,
  /// ★ Named from what the player sees: a small tree (small_tree.png)
  SPRITE_SMALL_TREE,
  /// ★ Named from what the player sees: an old woman with white hair in a
  /// grey dress (old_woman.png)
  SPRITE_OLD_WOMAN,
  /// ★ Named from what the player sees: an old man with a white beard in a
  /// blue cap and a blue robe (old_man_in_blue_cap.png)
  SPRITE_OLD_MAN_IN_BLUE_CAP,
  /// ★ Named from what the player sees: a man with fair hair and glasses in a
  /// blue cap and a red and blue robe (man_with_glasses.png)
  SPRITE_MAN_WITH_GLASSES,
  /// ★ Named from what the player sees: a bald, muscular man, bare-chested,
  /// in green trousers (bald_muscular_man.png)
  SPRITE_BALD_MUSCULAR_MAN,
  /// ★ Named from what the player sees: a woman with brown hair in a green
  /// dress and a white apron (woman_in_green_dress.png)
  SPRITE_WOMAN_IN_GREEN_DRESS,
  /// ★ Named from what the player sees: a stocky, muscular man with black
  /// hair in a red top, flexing his arms (strongman.png)
  SPRITE_STRONGMAN,
  /// ★ Named from what the player sees: an old man with white hair in a pale
  /// robe, with a cane (old_man_with_white_hair.png)
  SPRITE_OLD_MAN_WITH_WHITE_HAIR,
  /// ★ Named from what the player sees: a young woman with a brown ponytail
  /// in a white top and a long skirt (woman_with_ponytail.png)
  SPRITE_WOMAN_WITH_PONYTAIL,
  /// ★ Named from what the player sees: a man with dark hair in a dark blue
  /// tunic (man_with_dark_hair.png)
  SPRITE_MAN_WITH_DARK_HAIR,
  /// ★ Named from what the player sees: a woman with brown hair in a long
  /// blue dress (woman_in_blue_dress.png)
  SPRITE_WOMAN_IN_BLUE_DRESS,
  /// ★ Named from what the player sees: a man with a white beard in a tall
  /// hat and a long purple and blue robe (man_in_tall_hat.png)
  SPRITE_MAN_IN_TALL_HAT,
  /// ★ Named from what the player sees: a man with long brown hair and a
  /// beard in a red shawl and a long lavender robe (bearded_man_in_long_robe.png)
  SPRITE_BEARDED_MAN_IN_LONG_ROBE,
  /// ★ Named from what the player sees: a man in a white cook's cap and white
  /// clothes, with a frying pan (cook.png)
  SPRITE_COOK,
  /// ★ Named from what the player sees: a bald man in a red tunic with green
  /// sleeves (bald_man_in_red.png)
  SPRITE_BALD_MAN_IN_RED,
  /// ★ Named from what the player sees: a young man with brown hair in a
  /// green shirt (young_man_in_green.png)
  SPRITE_YOUNG_MAN_IN_GREEN,
  /// A second number for SPRITE_BALD_MAN_IN_RED, whose frames and animations
  /// it borrows (sprites.tsv), so the player sees the same man. The first
  /// number keeps most towns' weapon shops; this one stands in Kalay's houses
  /// and in Hammet's palace.
  /// ★ Named from what the player sees: a bald man in a red tunic with green
  /// sleeves (bald_man_in_red.png)
  SPRITE_BALD_MAN_IN_RED_KALAY,
  /// A second number for SPRITE_YOUNG_MAN_IN_GREEN, whose frames and
  /// animations it borrows (sprites.tsv), so the player sees the same man. The
  /// first number keeps most towns' armor shops; this one stands in Kalay's
  /// houses and in Hammet's palace.
  /// ★ Named from what the player sees: a young man with brown hair in a
  /// green shirt (young_man_in_green.png)
  SPRITE_YOUNG_MAN_IN_GREEN_KALAY,
  /// ★ Named from what the player sees: a woman with red-brown hair in a pink
  /// dress and a white apron (woman_in_pink_apron.png)
  SPRITE_WOMAN_IN_PINK_APRON,
  /// ★ Named from what the player sees: a man with fair hair in a blue
  /// feathered cap, holding a lute (man_with_lute.png)
  SPRITE_MAN_WITH_LUTE,
  /// ★ Named from what the player sees: a young man with brown hair and a
  /// white headband in a white top and blue trousers (youth_with_headband.png)
  SPRITE_YOUTH_WITH_HEADBAND,
  /// ★ Named from what the player sees: a young man with brown hair tied back
  /// in a white top and blue trousers (youth_with_tied_hair.png)
  SPRITE_YOUTH_WITH_TIED_HAIR,
  /// ★ Named from what the player sees: a man in a tall white chef's hat and
  /// white clothes with a red scarf, with a frying pan (chef.png)
  SPRITE_CHEF,
  /// ★ Named from what the player sees: a man with a mop of orange hair in a
  /// yellow vest and green trousers (man_with_orange_hair.png)
  SPRITE_MAN_WITH_ORANGE_HAIR,
  /// ★ Named from what the player sees: a man with golden-brown hair in
  /// orange and red clothes (man_in_orange.png)
  SPRITE_MAN_IN_ORANGE,
  /// ★ Named from what the player sees: a man with black hair and a white
  /// headband in an orange top, with a pickaxe (man_with_pickaxe.png)
  SPRITE_MAN_WITH_PICKAXE,
  /// ★ Named from what the player sees: a small white and grey cat
  /// (cat.png)
  SPRITE_CAT,
  SPRITE_137,
  SPRITE_138,
  SPRITE_139,
  SPRITE_140,
  SPRITE_141,
  SPRITE_142,
  /// ★ Named from what the player sees: a man with a beard in a white turban
  /// and long white robes (man_in_white_turban.png)
  SPRITE_MAN_IN_WHITE_TURBAN,
  /// ★ Named from what the player sees: a man with tanned skin in a red
  /// bandana and white trousers (man_in_red_bandana.png)
  SPRITE_MAN_IN_RED_BANDANA,
  /// ★ Named from what the player sees: a man with tanned skin in a blue
  /// bandana and white trousers (man_in_blue_bandana.png)
  SPRITE_MAN_IN_BLUE_BANDANA,
  /// ★ Named from what the player sees: a man with tanned skin in a yellow
  /// bandana and white trousers (man_in_yellow_bandana.png)
  SPRITE_MAN_IN_YELLOW_BANDANA,
  /// ★ Named from what the player sees: a man with tanned skin in a green
  /// bandana and a red vest (man_in_green_bandana.png)
  SPRITE_MAN_IN_GREEN_BANDANA,
  /// ★ Named from what the player sees: a young woman with long brown hair in
  /// a green dress (young_woman_in_green.png)
  SPRITE_YOUNG_WOMAN_IN_GREEN,
  /// ★ Named from what the player sees: a bald monk in a white and orange
  /// robe (monk_in_white_and_orange.png)
  SPRITE_MONK_IN_WHITE_AND_ORANGE,
  /// ★ Named from what the player sees: a soldier in a spiked steel helmet,
  /// with a spear (soldier_in_spiked_helmet.png)
  SPRITE_SOLDIER_IN_SPIKED_HELMET,
  /// ★ Named from what the player sees: a small bald boy in an orange robe
  /// (bald_boy_in_orange.png)
  SPRITE_BALD_BOY_IN_ORANGE,
  /// The soldiers of Babi's Tolbi and of the Colosseum.
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  SPRITE_SOLDIER,
  /// ★ Named from what the player sees: a soldier in a white turban, with a
  /// spear (soldier_in_turban.png)
  SPRITE_SOLDIER_IN_TURBAN,
  /// ★ Named from what the player sees: a covered wagon drawn by a horse
  /// (wagon.png)
  SPRITE_WAGON,
  /// ★ Named from what the player sees: a white bird with outspread wings
  /// (bird.png)
  SPRITE_BIRD,
  /// ★ Named from what the player sees: a young man with brown hair tied back
  /// in a blue jacket and a white apron (young_man_in_blue_jacket.png)
  SPRITE_YOUNG_MAN_IN_BLUE_JACKET,
  /// ★ Named from what the player sees: a person with long brown hair and a
  /// green scarf in a red top, with a sword (person_with_sword.png)
  SPRITE_PERSON_WITH_SWORD,
  /// ★ Named from what the player sees: a bald monk in an orange robe
  /// (monk_in_orange_robe.png)
  SPRITE_MONK_IN_ORANGE_ROBE,
  /// ★ Named from what the player sees: a young woman with black hair and a
  /// red hair ornament in a red and yellow dress (woman_with_black_hair.png)
  SPRITE_WOMAN_WITH_BLACK_HAIR,
  /// ★ Named from what the player sees: a man with black hair in a topknot in
  /// an orange and green robe (man_with_topknot.png)
  SPRITE_MAN_WITH_TOPKNOT,
  /// ★ Named from what the player sees: a ship with green sails (ship.png)
  SPRITE_SHIP,
  /// ★ Named from what the player sees: a woman with red-brown hair in a
  /// white top and a green apron (woman_in_green_apron.png)
  SPRITE_WOMAN_IN_GREEN_APRON,
  /// ★ Named from what the player sees: a soldier in a steel helmet, with a
  /// torch (soldier_with_torch.png)
  SPRITE_SOLDIER_WITH_TORCH,
  /// ★ Named from what the player sees: an old man with grey hair in a
  /// topknot in blue and orange clothes (old_man_in_blue.png)
  SPRITE_OLD_MAN_IN_BLUE,
  /// ★ Named from what the player sees: a young man with fair hair in a white
  /// and orange tunic (young_man_with_fair_hair.png)
  SPRITE_YOUNG_MAN_WITH_FAIR_HAIR,
  /// ★ Named from what the player sees: an old woman with white hair in a
  /// blue and red dress, with a cane (old_woman_with_cane.png)
  SPRITE_OLD_WOMAN_WITH_CANE,
  /// ★ Named from what the player sees: a man with grey hair in a black suit
  /// (man_in_black_suit.png)
  SPRITE_MAN_IN_BLACK_SUIT,
  /// ★ Named from what the player sees: a small boy with black hair in a bun
  /// in green clothes (boy_in_green.png)
  SPRITE_BOY_IN_GREEN,
  /// ★ Named from what the player sees: a little girl with brown hair in a
  /// pink top (girl_in_pink.png)
  SPRITE_GIRL_IN_PINK,
  /// ★ Named from what the player sees: a bald, muscular man with a black
  /// topknot, bare-chested (muscular_man_with_topknot.png)
  SPRITE_MUSCULAR_MAN_WITH_TOPKNOT,
  /// ★ Named from what the player sees: a man in a green cap and a green and
  /// yellow robe, carrying a pack on his back (man_carrying_a_pack.png)
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
  /// ★ Named from the game: "Toadonpa" (MsgEnemyName)
  SPRITE_TOADONPA,
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
  /// The Ant Lions of Lamakan Desert, whose battle is slot 4 of
  /// ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN.
  /// ★ Named from the game: "Ant Lion" (MsgEnemyName)
  SPRITE_ANT_LION,
  /// The Kraken, which the party fights on the ship's deck.
  /// ★ Named from the game: "You did good against that Kraken."
  /// (MsgTolbiShipThankIsaacDidGoodAgainst)
  SPRITE_KRAKEN,
  /// The Manticore of Lamakan Desert, fought by talking to it
  /// (LamakanDesert_FightManticore).
  /// ★ Named from the game: "Manticore" (MsgEnemyName)
  SPRITE_MANTICORE,
  /// ★ Named from what the player sees: a pink winged monster (its render)
  SPRITE_PINK_WINGED_MONSTER,
  /// ★ Named from what the player sees: a blue winged monster with claws (its
  /// render)
  SPRITE_BLUE_WINGED_MONSTER,
  /// ★ Named from what the player sees: a green lizard-like monster (its
  /// render)
  SPRITE_LIZARD_MONSTER,
  /// Altin Peak's Hydros Statue, before its battle.
  /// ★ Named from the game: "Hydros Statue" (MsgEnemyName)
  SPRITE_HYDROS_STATUE,
  /// A big round rock: the rocks that fall across Vale's paths on the stormy
  /// night, and the Boulder of Mt. Aleph drawn at twice its size.
  /// ★ Named from the game: "It's a huge rock" (MsgValeRockCannotBeMoved)
  SPRITE_ROCK,
  /// The statues of Sol Sanctum's statue room and Luna and Sol rooms.
  /// ★ Named from what the player sees: a winged statue holding a blue orb
  /// (its sprite sheet, winged_statue.png)
  SPRITE_WINGED_STATUE,
  /// The centre statue of Sol Sanctum's statue room.
  /// ★ Named from what the player sees: a winged statue bearing a sun disc on
  /// its head (its sprite sheet, sun_disc_statue.png)
  SPRITE_SUN_DISC_STATUE,
  /// ★ Named from what the player sees: a carved stone statue of a figure,
  /// as Sol Sanctum's statues stand on its seals (its render)
  SPRITE_STONE_STATUE,
  /// The centre statue once it has sunk into the floor, only its disc showing.
  /// ★ Named from what the player sees: a round sun disc lying flat (its
  /// sprite sheet, sun_disc.png)
  SPRITE_SUN_DISC,
  /// A log lying north to south, one the leader can hop onto
  /// (Field_TryJumpForward) or roll, as in Kolima Forest.
  /// ★ Named from what the player sees: a log lying north to south (its
  /// render)
  SPRITE_LOG_NORTH_SOUTH,
  /// The boards Isaac moves into place on a Vale roof (Vale_RoofMended).
  /// ★ Named from what the player sees: two golden planks (roof_boards.png)
  SPRITE_ROOF_BOARDS,
  /// ★ Named from what the player sees: a log lying west to east (its render)
  SPRITE_LOG_WEST_EAST,
  /// ★ Named from what the player sees: a small flame, then wisps of smoke
  /// (flame_and_smoke.png)
  SPRITE_FLAME_AND_SMOKE,
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
  /// Nothing in the source names this sprite.
  /// ★ Named from what the player sees: hands grasping, clenched and open, the
  /// last two blue (hands.png)
  SPRITE_HANDS,
  /// Spray thrown up where water is struck: where Goma Cave's stumps land,
  /// at Kolima Forest's floodgate, and over the Tolbi-bound Ship's side.
  /// ★ Named from what the player sees: a splash of white and pale blue
  /// spray (splash.png)
  SPRITE_SPLASH,
  /// The hand Move reaches out with; Goma Cave's entrance and Vale place it
  /// to push something in an event.
  /// ★ Named from what the player sees: a large open hand (move_hand.png)
  SPRITE_MOVE_HAND,
  /// The sixteen crystals thrown out in a ring as Sol Sanctum erupts, and one
  /// on the world map.
  /// ★ Named from what the player sees: a blue crystal that bursts into
  /// sparkles (blue_crystal.png)
  SPRITE_BLUE_CRYSTAL,
  /// The vines that appear around Growth's target.
  /// ★ Named from what the player sees: a green vine that grows leaves and
  /// withers (vine.png)
  SPRITE_VINE,
  /// The whirlwind Whirlwind spins up.
  /// ★ Named from what the player sees: a small white whirlwind (whirlwind.png)
  SPRITE_WHIRLWIND,
  /// Nothing in the source names this sprite.
  /// ★ Named from what the player sees: a flame that bursts into sparks and
  /// dies to a wisp (flame_burst.png)
  SPRITE_FLAME_BURST,
  /// The fields of Psynergy that rise over the party as Kolima's curse falls.
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  SPRITE_PSYNERGY_FIELD,
  /// The gold statue stolen from Vault's Sanctum: it rises from a chest in
  /// log_rooms_a.c and stands in the Sanctum again.
  /// ★ Named from the game: "Found a gold statue."
  /// (MsgVaultYouIsaacRightWontForget)
  SPRITE_SANCTUM_GOLD_STATUE,
  /// A puff of dust kicked up where an actor stands; the water beasts of
  /// Altin
  /// and its mine spit it as spray, drawn SPRITE_COLOR_BGR or
  /// SPRITE_COLOR_WHITE.
  SPRITE_DUST_PUFF,
  /// ★ Named from what the player sees: a wooden crate (crate.png)
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
  /// east (pipe_west_east.png)
  SPRITE_PIPE_WEST_EAST,
  /// The pipe sections Move pushes in Mercury Lighthouse, also placed in the
  /// Lighthouse Aerie of Venus Lighthouse.
  /// ★ Named from what the player sees: a stone pipe section lying north to
  /// south (pipe_north_south.png)
  SPRITE_PIPE_NORTH_SOUTH,
  /// The rim of an upright pipe's open end, drawn over the end a pushed pipe
  /// section comes to rest against.
  /// ★ Named from what the player sees: the rim at the top of an upright pipe
  /// (pipe_end.png; places/mercury-lighthouse-lower/map-4.png)
  SPRITE_PIPE_END,
  /// The carts on the rails of Altin's mine.
  /// ★ Named from what the player sees: a wooden mine cart on iron wheels
  /// (mine_cart.png)
  SPRITE_MINE_CART,
  /// Frost's particles.
  /// ★ Named from what the player sees: a snowflake (snowflake.png)
  SPRITE_SNOWFLAKE,
  /// The figures that spiral in around Ply's target.
  /// ★ Named from what the player sees: a little winged figure with teal hair
  /// (winged_figure.png)
  SPRITE_WINGED_FIGURE,
  /// ★ Named from what the player sees: a snowman with a red cap and a green
  /// scarf (snowman.png)
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
  /// The cloud Douse gathers over its target.
  /// ★ Named from what the player sees: a grey cloud (rain_cloud.png)
  SPRITE_RAIN_CLOUD,
  /// The drops of Douse's rain, and those a Mercury Djinni leaves as it goes.
  /// ★ Named from what the player sees: a drop of water (droplet.png)
  SPRITE_DROPLET,
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
  /// robe (golden_statue.png)
  SPRITE_GOLDEN_STATUE,
  /// Mercury Lighthouse's lit beacon, and the orb at the top of Venus
  /// Lighthouse.
  /// ★ Named from what the player sees: a blue orb with white swirls, turning
  /// (blue_orb.png)
  SPRITE_BLUE_ORB,
  /// The keys the scenes show: the barricade key McCoy tosses to the party,
  /// and Lunpa Fortress's Cell Key.
  /// ★ Named from what the player sees: a golden key (key.png)
  SPRITE_KEY,
  /// ★ Named from what the player sees: an unlit torch on its stand (its
  /// render)
  SPRITE_TORCH,
  /// A fire burning on the ground, which Douse puts out.
  /// ★ Named from what the player sees: a flickering flame (fire.png)
  SPRITE_FIRE,
  /// The ivy Whirlwind blows away, in Goma Cave, Vault, Bilibin, Lunpa
  /// Fortress and Crossbone Isle, and a patch outside McCoy's Palace.
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  SPRITE_IVY,
  /// The leaves Whirlwind scatters.
  /// ★ Named from what the player sees: a green leaf, turning (leaf.png)
  SPRITE_LEAF,
  /// Halt's stopwatch.
  /// ★ Named from what the player sees: a gold stopwatch, its hand going round
  /// (stopwatch.png)
  SPRITE_STOPWATCH,
  /// The mayor's urn, which rises from the first chest in log_rooms_a.c.
  /// ★ Named from the game: "Found the mayor's precious urn."
  /// (MsgVaultYouIsaacRightWontForget)
  SPRITE_URN,
  /// ★ Named from the game: "Could that be . . . the Wise One?!?"
  /// (MsgSolSanctumKradenCouldThatBeThe)
  SPRITE_WISE_ONE,
  /// The pillars Move shifts in the Tunnel Ruins and the lighthouses. All three
  /// animations draw one picture; in SPRITE_COLOR_GREY, as Venus Lighthouse,
  /// Babi Lighthouse Entry and the Tunnel Ruins' east pair show it, the whole
  /// pillar is grey and its cap pale grey in a white rim.
  /// ★ Named from what the player sees: a stone pillar on an orange base, with
  /// a yellow cap in a white rim (capped_pillar.png)
  SPRITE_CAPPED_PILLAR,
  /// The Tunnel Ruins' small pillars, which the leader pushes or shifts with
  /// Move.
  /// ★ Named from what the player sees: a small stone pillar with a white top
  /// on a wooden base (small_pillar.png)
  SPRITE_SMALL_PILLAR,
  /// The Tunnel Ruins' pillars that sink into the water.
  /// ★ Named from what the player sees: a stone pillar with a dark grating
  /// under a white rim, then with blue water in the rim (sinking_pillar.png)
  SPRITE_SINKING_PILLAR,
  /// The pillars Move, Carry and pushes shift in Venus Lighthouse and on the
  /// Road to Babi Lighthouse. Animations 1 and 2 show a grey and an orange
  /// spiral on top; 3 to 8 a piece of gold track instead, along which Venus
  /// Lighthouse's lights run.
  /// ★ Named from what the player sees: a grey pillar with a spiral carved on
  /// top (spiral_pillar.png)
  SPRITE_SPIRAL_PILLAR,
  /// The platforms of Venus Lighthouse and the Tunnel Ruins that rise or sink
  /// with the weight set down on them.
  /// ★ Named from what the player sees: a white platform on a carved grey base
  /// (its render)
  SPRITE_BALANCE_PLATFORM,
  /// The four statues around Venus Lighthouse's beacon.
  /// ★ Named from what the player sees: a statue of a woman with a green
  /// wreath, holding a sheaf (statue_with_wreath.png)
  SPRITE_STATUE_WITH_WREATH,
  /// The log Force knocks down onto the Board Walk in Satrage's match.
  /// ★ Named from what the player sees: a log standing on end, then lying on
  /// its side (falling_log.png)
  SPRITE_FALLING_LOG,
  SPRITE_260,
  /// ★ Named from what the player sees: houses with orange roofs among trees
  /// (world_map_village.png)
  SPRITE_WORLD_MAP_VILLAGE,
  SPRITE_262,
  /// ★ Named from what the player sees: white and blue buildings inside a wall
  /// (world_map_town.png)
  SPRITE_WORLD_MAP_TOWN,
  /// ★ Named from what the player sees: houses built into great tree stumps
  /// (world_map_stump_village.png)
  SPRITE_WORLD_MAP_STUMP_VILLAGE,
  /// ★ Named from what the player sees: houses under snow among snowy trees
  /// (world_map_snowy_village.png)
  SPRITE_WORLD_MAP_SNOWY_VILLAGE,
  /// ★ Named from what the player sees: buildings with blue roofs among trees
  /// (world_map_blue_roofed_town.png)
  SPRITE_WORLD_MAP_BLUE_ROOFED_TOWN,
  /// ★ Named from what the player sees: a temple with a blue roof by a
  /// waterfall (world_map_temple.png)
  SPRITE_WORLD_MAP_TEMPLE,
  /// ★ Named from what the player sees: a grey rock arch over a cave mouth
  /// (world_map_cave.png)
  SPRITE_WORLD_MAP_CAVE,
  /// ★ Named from what the player sees: a tall blue-green tower, where Mercury
  /// Lighthouse's exit is (world_map_mercury_lighthouse.png)
  SPRITE_WORLD_MAP_MERCURY_LIGHTHOUSE,
  /// ★ Named from what the player sees: a wooden barricade (world_map_barricade.png)
  SPRITE_WORLD_MAP_BARRICADE,
  /// ★ Named from what the player sees: a snow-capped mountain (world_map_mountain.png)
  SPRITE_WORLD_MAP_MOUNTAIN,
  /// ★ Named from what the player sees: a pink and white tower, where Babi
  /// Lighthouse's road begins (world_map_babi_lighthouse.png)
  SPRITE_WORLD_MAP_BABI_LIGHTHOUSE,
  /// SPRITE_WAGON drawn at 64 pixels, its back loaded with yellow crates:
  /// Bunza's wagon at the Kalay Tunnel Entrance.
  /// ★ Named from what the player sees: a large covered wagon drawn by a
  /// horse, full of crates (large_wagon.png)
  SPRITE_LARGE_WAGON,
  /// The signs of Lalivero, Tolbi Spring, Altin Peak and Kolima Forest's
  /// floodgate, and the signposts on the world map.
  /// ★ Named from what the player sees: a wooden sign on a post (its render)
  SPRITE_SIGNPOST,
  /// ★ Named from what the player sees: a clump of tall green trees
  /// (world_map_forest.png)
  SPRITE_WORLD_MAP_FOREST,
  /// ★ Named from what the player sees: a tall purple and white tower, where
  /// Venus Lighthouse stands (world_map_venus_lighthouse.png)
  SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE,
  /// ★ Named from what the player sees: a pier with a boat moored at it
  /// (world_map_dock.png)
  SPRITE_WORLD_MAP_DOCK,
  /// A drawbridge over a river: raised in its first animation, let down
  /// across the river in its second.
  /// ★ Named from what the player sees: a drawbridge raised between its posts,
  /// then lying across the river (world_map_bridge.png)
  SPRITE_WORLD_MAP_BRIDGE,
  /// ★ Named from what the player sees: a grey gatehouse between green walls
  /// (world_map_gatehouse.png)
  SPRITE_WORLD_MAP_GATEHOUSE,
  /// The letters that rise from a sleeper: Garet's grandfather, and Isaac after
  /// the last Colosso match.
  /// ★ Named from what the player sees: a white letter Z (letter_z.png)
  SPRITE_LETTER_Z,
  /// The particles Mind Read sends from its target to the caster, also shot in
  /// Fuchin Temple.
  /// ★ Named from what the player sees: pale crescents (crescent.png)
  SPRITE_CRESCENT,
  SPRITE_282,
  /// The impact of Lift and of Xian's training tree, and the battle's hit flash
  /// (BattleMotion_PlayHitFlash, which nothing calls).
  /// ★ Named from what the player sees: a star burst that grows and breaks
  /// apart over ten frames (star_burst.png)
  SPRITE_STAR_BURST,
  /// The particles of Growth and of a Djinni's capture, and the gate effects
  /// of Venus Lighthouse.
  SPRITE_PARTICLE,
  /// One sparkle: the glints of Halt and of the field's sparkles, and Kolima's
  /// curse dust.
  /// ★ Named from what the player sees: a sparkle of light that shrinks away
  /// (its render)
  SPRITE_SPARKLE,
  /// ★ Named from what the player sees: a four-pointed glint that shrinks
  /// to a dot (glint.png)
  SPRITE_GLINT,
  /// The lights that float about the Elemental Star Room.
  /// ★ Named from what the player sees: a small glowing blue-green light (its
  /// sprite sheet, floating_light.png)
  SPRITE_FLOATING_LIGHT,
  /// A blank 32-pixel square: an actor seen only by its shadow, such as
  /// the
  /// shadows under Altin Peak's falling boulder and its Hydros statue.
  /// ★ Named from what the player sees: nothing, every pixel transparent
  /// (blank_large.png)
  SPRITE_BLANK_LARGE,
  /// The whirlwinds of the Suhalla Desert, which Douse stops, and the one
  /// drawn on the world map beside it.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  SPRITE_SANDSTORM,
  /// The Board Walk's platforms, the Scales of Justice's ledges and the pillar
  /// that rises in a Crossbone Isle puddle.
  /// ★ Named from what the player sees: a white square platform (white_platform.png)
  SPRITE_WHITE_PLATFORM,
  /// The Wall Wedge's pistons.
  /// ★ Named from the game: "The pistons move back and forth."
  /// (MsgColossoPlaceCalledWall)
  SPRITE_PISTON,
  /// The lights of Altmiller Cave's rock puzzle, coloured by their palette.
  /// ★ Named from the game: "Lights will appear on the wall."
  /// (MsgAltmillerDontTrustAnyone)
  SPRITE_LIGHT,
  /// The rocks of Altmiller Cave's rock puzzle, coloured by their palette
  /// while Reveal shines.
  /// ★ Named from the game: "Turn the five rocks in accordance with those
  /// colors." (MsgAltmillerDontTrustAnyone)
  SPRITE_TURNING_ROCK,
  /// The log Force knocks off the Moving Sidewalk Stage.
  /// ★ Named from what the player sees: a log standing on end (standing_log.png)
  SPRITE_STANDING_LOG,
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
  /// Vault Cave's pillars.
  /// ★ Named from what the player sees: a pink stone pillar with a flame
  /// burning on top (flame_pillar.png)
  SPRITE_FLAME_PILLAR,
  /// Isaac in battle. The party members' battle sprites follow in character
  /// order, Felix's row left empty.
  /// ★ Named from what the player sees: Isaac in battle (battle_isaac.png)
  SPRITE_BATTLE_ISAAC,
  /// ★ Named from what the player sees: Garet in battle (battle_garet.png)
  SPRITE_BATTLE_GARET,
  /// ★ Named from what the player sees: Ivan in battle (battle_ivan.png)
  SPRITE_BATTLE_IVAN,
  /// ★ Named from what the player sees: Mia in battle (battle_mia.png)
  SPRITE_BATTLE_MIA,
  SPRITE_304,
  /// ★ Named from what the player sees: Jenna in battle (battle_jenna.png)
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
  /// ★ Named from what the player sees: a sword with a green jewel in its hilt
  /// (weapon_isaac_sword.png)
  SPRITE_WEAPON_ISAAC_SWORD,
  /// Garet's for maces, staffs, ankhs and anything the list lacks.
  /// ★ Named from what the player sees: a spiked ball on a handle (weapon_garet_mace.png)
  SPRITE_WEAPON_GARET_MACE,
  /// Ivan's for staffs and anything the list lacks.
  /// ★ Named from what the player sees: a staff with a red forked head
  /// (weapon_ivan_staff.png)
  SPRITE_WEAPON_IVAN_STAFF,
  /// Mia's for swords, light blades, ankhs and anything the list lacks.
  /// ★ Named from what the player sees: a silver ankh on a staff (weapon_mia_ankh.png)
  SPRITE_WEAPON_MIA_ANKH,
  SPRITE_324,
  /// Jenna's for axes, maces, staffs, ankhs and anything the list lacks.
  /// ★ Named from what the player sees: a golden staff with a crescent head
  /// (weapon_jenna_staff.png)
  SPRITE_WEAPON_JENNA_STAFF,
  /// ★ Named from what the player sees: a double-bladed axe (weapon_isaac_axe.png)
  SPRITE_WEAPON_ISAAC_AXE,
  /// ★ Named from what the player sees: a mace with a spiked head (weapon_isaac_mace.png)
  SPRITE_WEAPON_ISAAC_MACE,
  /// ★ Named from what the player sees: a long-hafted axe (weapon_garet_axe.png)
  SPRITE_WEAPON_GARET_AXE,
  /// Garet's for long swords and light blades.
  /// ★ Named from what the player sees: a broad sword (weapon_garet_sword.png)
  SPRITE_WEAPON_GARET_SWORD,
  /// ★ Named from what the player sees: a golden ankh on a staff (weapon_ivan_ankh.png)
  SPRITE_WEAPON_IVAN_ANKH,
  /// Ivan's for swords, light blades, axes and maces.
  /// ★ Named from what the player sees: a slender sword (weapon_ivan_blade.png)
  SPRITE_WEAPON_IVAN_BLADE,
  /// ★ Named from what the player sees: a staff with a blue crystal head
  /// (weapon_mia_staff.png)
  SPRITE_WEAPON_MIA_STAFF,
  /// Mia's for axes and maces.
  /// ★ Named from what the player sees: a mace (weapon_mia_mace.png)
  SPRITE_WEAPON_MIA_MACE,
  /// ★ Named from what the player sees: a straight sword with a brown hilt
  /// (weapon_isaac_light_blade.png)
  SPRITE_WEAPON_ISAAC_LIGHT_BLADE,
  /// Unused: no gBattleUnit_WeaponAnimations table holds it, so no attack shows
  /// it; the player would have seen this sword in a party member's hand.
  /// ★ Named from what the player sees: a sword with a golden hilt
  /// (weapon_unused_sword.png)
  SPRITE_WEAPON_UNUSED_SWORD,
  /// Jenna's for swords and light blades.
  /// ★ Named from what the player sees: a short sword (weapon_jenna_blade.png)
  SPRITE_WEAPON_JENNA_BLADE,
  SPRITE_337,
  SPRITE_338,
  SPRITE_339,
  /// The status icons over a unit 32 pixels wide, in the order of objects.c's
  /// ICON_ bits (BattleStatusIcon_Cycle); from SPRITE_STATUS_WIDE_POISON on,
  /// the same over wider units.
  /// ★ Named from what the player sees: a green skull trailing bubbles
  /// (status_poison.png)
  SPRITE_STATUS_POISON,
  /// ★ Named from what the player sees: a red skull trailing bubbles
  /// (status_venom.png)
  SPRITE_STATUS_VENOM,
  /// ★ Named from what the player sees: yellow zigzags either side
  /// (status_stun.png)
  SPRITE_STATUS_STUN,
  /// ★ Named from what the player sees: a pink seal (status_psy_seal.png)
  SPRITE_STATUS_PSY_SEAL,
  /// ★ Named from what the player sees: blue letters Z (status_sleep.png)
  SPRITE_STATUS_SLEEP,
  /// ★ Named from what the player sees: a purple cloud (status_delusion.png)
  SPRITE_STATUS_DELUSION,
  /// ★ Named from what the player sees: a pale wisp (status_haunt.png)
  SPRITE_STATUS_HAUNT,
  /// The curse's turns left, one to seven, as that many flames.
  /// ★ Named from what the player sees: one flame (status_grim_reaper_1.png)
  SPRITE_STATUS_GRIM_REAPER_1,
  /// ★ Named from what the player sees: two flames (status_grim_reaper_2.png)
  SPRITE_STATUS_GRIM_REAPER_2,
  /// ★ Named from what the player sees: three flames (status_grim_reaper_3.png)
  SPRITE_STATUS_GRIM_REAPER_3,
  /// ★ Named from what the player sees: four flames (status_grim_reaper_4.png)
  SPRITE_STATUS_GRIM_REAPER_4,
  /// ★ Named from what the player sees: five flames (status_grim_reaper_5.png)
  SPRITE_STATUS_GRIM_REAPER_5,
  /// ★ Named from what the player sees: six flames (status_grim_reaper_6.png)
  SPRITE_STATUS_GRIM_REAPER_6,
  /// ★ Named from what the player sees: seven flames (status_grim_reaper_7.png)
  SPRITE_STATUS_GRIM_REAPER_7,
  SPRITE_354,
  /// ★ Named from what the player sees: a green skull trailing bubbles
  /// (status_wide_poison.png)
  SPRITE_STATUS_WIDE_POISON,
  /// ★ Named from what the player sees: a red skull trailing bubbles
  /// (status_wide_venom.png)
  SPRITE_STATUS_WIDE_VENOM,
  /// ★ Named from what the player sees: yellow zigzags either side
  /// (status_wide_stun.png)
  SPRITE_STATUS_WIDE_STUN,
  /// ★ Named from what the player sees: a pink seal (status_wide_psy_seal.png)
  SPRITE_STATUS_WIDE_PSY_SEAL,
  /// ★ Named from what the player sees: blue letters Z (status_wide_sleep.png)
  SPRITE_STATUS_WIDE_SLEEP,
  /// ★ Named from what the player sees: a purple cloud (status_wide_delusion.png)
  SPRITE_STATUS_WIDE_DELUSION,
  /// ★ Named from what the player sees: a pale wisp (status_wide_haunt.png)
  SPRITE_STATUS_WIDE_HAUNT,
  /// ★ Named from what the player sees: one flame (status_wide_grim_reaper_1.png)
  SPRITE_STATUS_WIDE_GRIM_REAPER_1,
  /// ★ Named from what the player sees: two flames (status_wide_grim_reaper_2.png)
  SPRITE_STATUS_WIDE_GRIM_REAPER_2,
  /// ★ Named from what the player sees: three flames (status_wide_grim_reaper_3.png)
  SPRITE_STATUS_WIDE_GRIM_REAPER_3,
  /// ★ Named from what the player sees: four flames (status_wide_grim_reaper_4.png)
  SPRITE_STATUS_WIDE_GRIM_REAPER_4,
  /// ★ Named from what the player sees: five flames (status_wide_grim_reaper_5.png)
  SPRITE_STATUS_WIDE_GRIM_REAPER_5,
  /// ★ Named from what the player sees: six flames (status_wide_grim_reaper_6.png)
  SPRITE_STATUS_WIDE_GRIM_REAPER_6,
  /// ★ Named from what the player sees: seven flames (status_wide_grim_reaper_7.png)
  SPRITE_STATUS_WIDE_GRIM_REAPER_7,
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
  /// ★ Named from what the player sees: three orange rocks (battle_rocks.png)
  SPRITE_BATTLE_ROCKS,
  SPRITE_391,
  SPRITE_392,
  SPRITE_393,
  SPRITE_394,
  SPRITE_395,
  /// One of the two kinds of obstacle that move in Tolbi Spring while a token
  /// flies over it.
  /// ★ Named from what the player sees: a red crab (crab.png)
  SPRITE_CRAB,
  /// The other kind of Tolbi Spring obstacle.
  /// ★ Named from what the player sees: a green turtle with an orange shell
  /// (turtle.png)
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
  /// The enemies' battle sprites follow, gBattleSprites's by enemy id, each
  /// named for the first enemy that shows it; their pictures are
  /// graphics/monster's, by gSpritePictureCache_Sources.
  /// ★ Named from the game: "Troll" (MsgEnemyName)
  SPRITE_BATTLE_TROLL,
  /// ★ Named from the game: "Zombie" (MsgEnemyName)
  SPRITE_BATTLE_ZOMBIE,
  /// ★ Named from the game: "Harpy" (MsgEnemyName)
  SPRITE_BATTLE_HARPY,
  /// ★ Named from the game: "Golem" (MsgEnemyName)
  SPRITE_BATTLE_GOLEM,
  /// ★ Named from the game: "Ghost" (MsgEnemyName)
  SPRITE_BATTLE_GHOST,
  SPRITE_405,
  SPRITE_406,
  SPRITE_407,
  /// ★ Named from the game: "Gryphon" (MsgEnemyName)
  SPRITE_BATTLE_GRYPHON,
  /// ★ Named from the game: "Rat Soldier" (MsgEnemyName)
  SPRITE_BATTLE_RAT_SOLDIER,
  /// ★ Named from the game: "Ghoul" (MsgEnemyName)
  SPRITE_BATTLE_GHOUL,
  /// ★ Named from the game: "Chimera" (MsgEnemyName)
  SPRITE_BATTLE_CHIMERA,
  SPRITE_412,
  SPRITE_413,
  SPRITE_414,
  /// ★ Named from the game: "Deadbeard" (MsgEnemyName)
  SPRITE_BATTLE_DEADBEARD,
  SPRITE_416,
  /// ★ Named from the game: "Manticore" (MsgEnemyName)
  SPRITE_BATTLE_MANTICORE,
  SPRITE_418,
  SPRITE_419,
  SPRITE_420,
  /// ★ Named from the game: "Kobold" (MsgEnemyName)
  SPRITE_BATTLE_KOBOLD,
  SPRITE_422,
  SPRITE_423,
  /// ★ Named from the game: "Gargoyle" (MsgEnemyName)
  SPRITE_BATTLE_GARGOYLE,
  SPRITE_425,
  SPRITE_426,
  SPRITE_427,
  SPRITE_428,
  /// ★ Named from the game: "Lizard Man" (MsgEnemyName)
  SPRITE_BATTLE_LIZARD_MAN,
  /// ★ Named from the game: "Orc" (MsgEnemyName)
  SPRITE_BATTLE_ORC,
  SPRITE_431,
  SPRITE_432,
  /// The Venus Djinn's, as Quartz and Vine fight.
  /// ★ Named from the game: "Quartz" (MsgDjinnName)
  SPRITE_BATTLE_VENUS_DJINNI,
  /// ★ Named from the game: "Breeze" (MsgDjinnName)
  SPRITE_BATTLE_JUPITER_DJINNI,
  /// ★ Named from the game: "Forge" (MsgDjinnName)
  SPRITE_BATTLE_MARS_DJINNI,
  /// ★ Named from the game: "Sleet" (MsgDjinnName)
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
  /// ★ Named from the game: "Dread Hound" (MsgEnemyName)
  SPRITE_BATTLE_DREAD_HOUND,
  SPRITE_449,
  /// ★ Named from the game: "Mauler" (MsgEnemyName)
  SPRITE_BATTLE_MAULER,
  /// ★ Named from the game: "Siren" (MsgEnemyName)
  SPRITE_BATTLE_SIREN,
  SPRITE_452,
  /// ★ Named from the game: "Vermin" (MsgEnemyName)
  SPRITE_BATTLE_VERMIN,
  /// ★ Named from the game: "Wild Mushroom" (MsgEnemyName)
  SPRITE_BATTLE_WILD_MUSHROOM,
  /// ★ Named from the game: "Slime" (MsgEnemyName)
  SPRITE_BATTLE_SLIME,
  /// ★ Named from the game: "Cuttle" (MsgEnemyName)
  SPRITE_BATTLE_CUTTLE,
  /// ★ Named from the game: "Bat" (MsgEnemyName)
  SPRITE_BATTLE_BAT,
  /// ★ Named from the game: "Rat" (MsgEnemyName)
  SPRITE_BATTLE_RAT,
  /// ★ Named from the game: "Stone Soldier" (MsgEnemyName)
  SPRITE_BATTLE_STONE_SOLDIER,
  /// ★ Named from the game: "Drone Bee" (MsgEnemyName)
  SPRITE_BATTLE_DRONE_BEE,
  /// ★ Named from the game: "Spider" (MsgEnemyName)
  SPRITE_BATTLE_SPIDER,
  /// ★ Named from the game: "Mole" (MsgEnemyName)
  SPRITE_BATTLE_MOLE,
  /// ★ Named from the game: "Dirge" (MsgEnemyName)
  SPRITE_BATTLE_DIRGE,
  /// ★ Named from the game: "Grub" (MsgEnemyName)
  SPRITE_BATTLE_GRUB,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  SPRITE_BATTLE_APE,
  /// ★ Named from the game: "Salamander" (MsgEnemyName)
  SPRITE_BATTLE_SALAMANDER,
  /// ★ Named from the game: "Living Statue" (MsgEnemyName)
  SPRITE_BATTLE_LIVING_STATUE,
  /// ★ Named from the game: "Gnome" (MsgEnemyName)
  SPRITE_BATTLE_GNOME,
  /// ★ Named from the game: "Skeleton" (MsgEnemyName)
  SPRITE_BATTLE_SKELETON,
  /// ★ Named from the game: "Will Head" (MsgEnemyName)
  SPRITE_BATTLE_WILL_HEAD,
  /// ★ Named from the game: "Tornado Lizard" (MsgEnemyName)
  SPRITE_BATTLE_TORNADO_LIZARD,
  /// ★ Named from the game: "Ant Lion" (MsgEnemyName)
  SPRITE_BATTLE_ANT_LION,
  /// ★ Named from the game: "Mimic" (MsgEnemyName)
  SPRITE_BATTLE_MIMIC,
  /// ★ Named from the game: "Toadonpa" (MsgEnemyName)
  SPRITE_BATTLE_TOADONPA,
  /// ★ Named from the game: "Amaze" (MsgEnemyName)
  SPRITE_BATTLE_AMAZE,
  /// The Kraken's left half; SPRITE_BATTLE_KRAKEN_RIGHT is drawn 64 pixels to
  /// its right (sprites.tsv).
  /// ★ Named from the game: "Kraken" (MsgEnemyName)
  SPRITE_BATTLE_KRAKEN,
  /// ★ Named from what the player sees: the Kraken's spotted body and a
  /// tentacle, its right half (graphics/monster/slime.png)
  SPRITE_BATTLE_KRAKEN_RIGHT,
  /// ★ Named from the game: "Saturos" (MsgEnemyName)
  SPRITE_BATTLE_SATUROS,
  /// ★ Named from the game: "Menardi" (MsgEnemyName)
  SPRITE_BATTLE_MENARDI,
  /// Isaac's battle sprite while he holds a Machete, drawn as part of it.
  SPRITE_ISAAC_MACHETE,
  /// ★ Named from the game: "Tret" (MsgEnemyName)
  SPRITE_BATTLE_TRET,
  /// Garet's battle sprite while he holds a Machete, drawn as part of it.
  SPRITE_GARET_MACHETE,
  /// The Fusion Dragon's left half; SPRITE_BATTLE_FUSION_DRAGON_RIGHT is drawn
  /// 64 pixels to its right (sprites.tsv).
  /// ★ Named from the game: "Fusion Dragon" (MsgEnemyName)
  SPRITE_BATTLE_FUSION_DRAGON,
  /// ★ Named from what the player sees: the Fusion Dragon's head and forelegs,
  /// its right half (graphics/monster/purple_beast.png)
  SPRITE_BATTLE_FUSION_DRAGON_RIGHT,
  /// The Colosso finalists Azart, Satrage and Navampa in battle.
  /// ★ Named from what the player sees: a warrior in a golden helmet with a red
  /// crest, with a sword (graphics/monster/red_warrior.png)
  SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
  /// ★ Named from the game: "Bandit" (MsgEnemyName)
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
  /// ★ Named from what the player sees: a statue of a woman in a long robe
  /// (its render)
  SPRITE_LADY_STATUE,
  /// ★ Named from what the player sees: the wreck of a ship lying in the water
  /// (shipwreck.png)
  SPRITE_SHIPWRECK,
  /// ★ Named from what the player sees: a wooden gangway with rails, laid from
  /// the wharf over Babi's ship (gangway.png)
  SPRITE_GANGWAY,
  /// ★ Named from what the player sees: a drop of water that swells and bursts
  /// into a ring of spray (water_drop.png)
  SPRITE_WATER_DROP,
  /// ★ Named from what the player sees: an island of green hills around a
  /// mountain (world_map_island.png)
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

/// SPRITE_LOG_NORTH_SOUTH's animations (log_north_south.tsv) as a log tips into a
/// river
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

/// SPRITE_ISAAC_CHILD's animations beyond enum ActorAnimation (isaac_child.tsv).
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
