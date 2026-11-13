	.section .rodata
	.global Summon_GreenHairedMaidenTiles
Summon_GreenHairedMaidenTiles:
	.incbin "games/tla/src/graphics/summon/green_haired_maiden.gbapal"
	.incbin "games/tla/src/graphics/summon/green_haired_maiden.parts.lz"
	.global Summon_SeaSerpentTiles
Summon_SeaSerpentTiles:
	.incbin "games/tla/src/graphics/summon/sea_serpent.gbapal"
	.incbin "games/tla/src/graphics/summon/sea_serpent.parts.lz"
	.byte 0x01	@ PACKER: left in the buffer by Summon_GreenHairedMaidenTiles+0x11ff
@ A 16-colour palette and one packed stream of 4,320 bytes. Three routines load
@ it, and each reads the stream's first 1,920 bytes as fifteen 16x16 sprites of
@ 16 colours (pink_petals.png): two copy them to OBJ video memory 128 bytes a
@ sprite, one with the palette in OBJ palette 15 and one with that row filled
@ white, and the third unpacks them into a 256-colour canvas under BG palette
@ 0. None reads further. The next 1,920 bytes are no picture of this summon:
@ Camelot's converter left them in its buffer, and they are bytes 6,104 to
@ 8,023 of Summon_SeaSerpentTiles' unpacked 256-colour tiles
@ (pink_petals_leftover.tsv). The last 480 bytes are zero.
	.global Summon_PinkPetalsTiles
Summon_PinkPetalsTiles:
	.incbin "games/tla/src/graphics/summon/pink_petals.gbapal"
	.incbin "games/tla/src/graphics/summon/pink_petals.parts.lz"
	.byte 0xe1, 0xb1	@ PACKER: left in the buffer by Summon_SeaSerpentTiles+0x77a
	.section .rodata.a
	.global Summon_RedSlashTiles
Summon_RedSlashTiles:
	.incbin "games/tla/src/graphics/summon/red_slash.gbapal"
	.incbin "games/tla/src/graphics/summon/red_slash.parts.lz"
	.byte 0xb2, 0x6a, 0xeb	@ PACKER: left in the buffer by Summon_CrimsonBeastTiles+0x529
	.global Summon_RedCrescentTiles
Summon_RedCrescentTiles:
	.incbin "games/tla/src/graphics/summon/red_crescent.gbapal"
	.incbin "games/tla/src/graphics/summon/red_crescent.parts.lz"
@ A 16-colour palette and one packed stream of 1,504 bytes. The game copies the
@ palette to OBJ palette 15 and the stream's first 512 bytes to video memory as
@ sixteen 8x8 sprites of 16 colours (white_arrows.png). It reads no more of the
@ stream. The next 512 bytes are no picture of this summon: Camelot's converter
@ left them in its buffer, and they are bytes 1,880 to 2,391 of
@ Summon_PinkPetalsTiles' unpacked stream, the end of its petals and the start
@ of its own leftover (white_arrows_leftover.tsv). The last 480 bytes are zero.
	.global Summon_WhiteArrowsTiles
Summon_WhiteArrowsTiles:
	.incbin "games/tla/src/graphics/summon/white_arrows.gbapal"
	.incbin "games/tla/src/graphics/summon/white_arrows.parts.lz"
	.byte 0x34, 0x88	@ PACKER: left in the buffer by Summon_RedCrescentTiles+0x25a
@ A 16-colour palette and one packed stream of 4,576 bytes. The game copies the
@ palette to OBJ palette 14 and at once fills that row with white, and copies
@ the stream's first 2,048 bytes to video memory as one 64x64 sprite of 16
@ colours (white_wisps.png). It reads no more of the stream. The next 2,048
@ bytes are no picture of this summon: Camelot's converter left them in its
@ buffer, and they are bytes 6,488 to 8,535 of Summon_CrimsonBeastTiles' unpacked
@ 256-colour tiles (white_wisps_leftover.tsv). The last 480 bytes are zero.
	.global Summon_WhiteWispsTiles
Summon_WhiteWispsTiles:
	.incbin "games/tla/src/graphics/summon/white_wisps.gbapal"
	.incbin "games/tla/src/graphics/summon/white_wisps.parts.lz"
	.byte 0xfc	@ PACKER: left in the buffer by Summon_RedCrescentTiles+0x577
	.section .rodata.b
@ A 16-colour palette and one packed stream of 608 bytes. The game copies the
@ palette to OBJ palette 14 and the stream's first 256 bytes to video memory, and
@ shows them through 16x8 sprites of 16 colours that all name the first two
@ tiles: the stream's first 64 bytes (missile.png). The other 544 bytes are no
@ picture: Camelot's converter left them in its buffer, and they are bytes of
@ the packed stream of Summon_ForestCanopyTiles, the 64 from its byte 1,056 and
@ then the 480 from its byte 1,960 (missile_leftover.tsv).
	.global Summon_MissileTiles
Summon_MissileTiles:
	.incbin "games/tla/src/graphics/summon/missile.gbapal"
	.incbin "games/tla/src/graphics/summon/missile.parts.lz"
	.byte 0x79, 0x10, 0x8d	@ PACKER: left in the buffer by Summon_BronzeAutomatonTiles+0x2b1
	.global Summon_GoldenDragonHeadTiles
Summon_GoldenDragonHeadTiles:
	.incbin "games/tla/src/graphics/summon/golden_dragon_head.gbapal"
	.incbin "games/tla/src/graphics/summon/golden_dragon_head.parts.lz"
	.byte 0x1e	@ PACKER: left in the buffer by Summon_BronzeAutomatonTiles+0x13df
	.section .rodata.c
	.global Summon_RobedGoddessTiles
Summon_RobedGoddessTiles:
	.incbin "games/tla/src/graphics/summon/robed_goddess.gbapal"
	.incbin "games/tla/src/graphics/summon/robed_goddess.parts.lz"
	.global Summon_LeafWarriorTiles
Summon_LeafWarriorTiles:
	.incbin "games/tla/src/graphics/summon/leaf_warrior.gbapal"
	.incbin "games/tla/src/graphics/summon/leaf_warrior.parts.lz"
	.byte 0xe3, 0xf6, 0x14	@ PACKER: left in the buffer by Summon_RobedGoddessTiles+0x166d
	.global Summon_BellMaidenTiles
Summon_BellMaidenTiles:
	.incbin "games/tla/src/graphics/summon/bell_maiden.gbapal"
	.incbin "games/tla/src/graphics/summon/bell_maiden.parts.lz"
	.byte 0x4e, 0x39	@ PACKER: left in the buffer by Summon_RobedGoddessTiles+0x1d22
	.global Summon_WingedKnightATiles
Summon_WingedKnightATiles:
	.incbin "games/tla/src/graphics/summon/winged_knight_a.gbapal"
	.incbin "games/tla/src/graphics/summon/winged_knight_a.parts.lz"
	.byte 0x45, 0xb9, 0x7f	@ PACKER: left in the buffer by Summon_RobedGoddessTiles+0x1efd
	.global Summon_WingedKnightBTiles
Summon_WingedKnightBTiles:
	.incbin "games/tla/src/graphics/summon/winged_knight_b.gbapal"
	.incbin "games/tla/src/graphics/summon/winged_knight_b.parts.lz"
	.byte 0x83	@ PACKER: left in the buffer by Summon_WingedKnightATiles+0x19ab
	.global Summon_WingedKnightCTiles
Summon_WingedKnightCTiles:
	.incbin "games/tla/src/graphics/summon/winged_knight_c.gbapal"
	.incbin "games/tla/src/graphics/summon/winged_knight_c.parts.lz"
	.byte 0xdb	@ PACKER: left in the buffer by Summon_WingedKnightBTiles+0x1577
	.global Summon_ChainedDemonTiles
Summon_ChainedDemonTiles:
	.incbin "games/tla/src/graphics/summon/chained_demon.gbapal"
	.incbin "games/tla/src/graphics/summon/chained_demon.parts.lz"
@ A 16-colour palette and one packed stream of 3,040 bytes. The game copies the
@ palette to OBJ palette 13 and the stream's first 2,560 bytes to video memory
@ as five 32x32 sprites of 16 colours (purple_armour.png). It reads no more of
@ the stream. The last 480 bytes are no picture of this summon: Camelot's
@ converter left them in its buffer, and they are bytes 15,904 to 16,383 of
@ Summon_ChainedDemonTiles' unpacked 256-colour tiles
@ (purple_armour_leftover.tsv).
	.global Summon_PurpleArmourTiles
Summon_PurpleArmourTiles:
	.incbin "games/tla/src/graphics/summon/purple_armour.gbapal"
	.incbin "games/tla/src/graphics/summon/purple_armour.parts.lz"
	.byte 0x75, 0x72	@ PACKER: left in the buffer by Summon_ChainedDemonTiles+0x592
@ A 16-colour palette and one packed stream of 3,552 bytes. The game copies the
@ palette to OBJ palette 14 and the stream's first 3,072 bytes to video memory
@ as six 32x32 sprites of 16 colours, drawn here as the three spikes they make
@ in pairs, upper half then lower (white_spikes.png). The last 480 bytes are
@ zero.
	.global Summon_WhiteSpikesTiles
Summon_WhiteSpikesTiles:
	.incbin "games/tla/src/graphics/summon/white_spikes.gbapal"
	.incbin "games/tla/src/graphics/summon/white_spikes.parts.lz"
@ A 16-colour palette and one packed stream of 3,040 bytes. The game copies the
@ palette to OBJ palette 12 and the stream's first 2,560 bytes to video memory
@ as five 32x32 sprites of 16 colours (golden_scales.png). The last 480 bytes
@ are zero.
	.global Summon_GoldenScalesTiles
Summon_GoldenScalesTiles:
	.incbin "games/tla/src/graphics/summon/golden_scales.gbapal"
	.incbin "games/tla/src/graphics/summon/golden_scales.parts.lz"
	.section .rodata.d
@ A 16-colour palette and one packed stream of 8,672 bytes. The game copies the
@ palette to OBJ palette 15 and the stream's first 8,192 bytes to video memory
@ as two groups of eight 32x32 sprites of 16 colours, 512 bytes a sprite. Side
@ by side each group is a strip 256 pixels wide, and the second continues the
@ first downwards: forest_canopy.png draws them as that one picture. The last
@ 480 bytes are zero.
	.global Summon_ForestCanopyTiles
Summon_ForestCanopyTiles:
	.incbin "games/tla/src/graphics/summon/forest_canopy.gbapal"
	.incbin "games/tla/src/graphics/summon/forest_canopy.parts.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x5e, 0x5f	@ PACKER: left in the buffer by LuckyDice_Sprites+0x173e
	.else
	.ifdef TLA_EDITION_DE
	.byte 0xd9, 0x3c	@ PACKER: left in the buffer by LuckyDice_Sprites+0x173e
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x78, 0x9c	@ PACKER: left in the buffer by LuckyDice_Sprites+0x173e
	.else
	.ifdef TLA_EDITION_FR
	.byte 0xf9, 0x84	@ PACKER: left in the buffer by LuckyDice_Sprites+0x173e
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x43, 0x7c	@ PACKER: left in the buffer by LuckyDice_Sprites+0x173e
	.else
	.byte 0x39, 0x4a	@ PACKER: left in the buffer by LuckyDice_Sprites+0x173e
	.endif
	.endif
	.endif
	.endif
	.endif
	.global Summon_BronzeAutomatonTiles
Summon_BronzeAutomatonTiles:
	.incbin "games/tla/src/graphics/summon/bronze_automaton.gbapal"
	.incbin "games/tla/src/graphics/summon/bronze_automaton.parts.lz"
	.ifdef TLA_EDITION_EN
	.byte 0xff	@ PACKER: left in the buffer by LuckyDice_Sprites+0x2ec7
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x46	@ PACKER: left in the buffer by LuckyDice_Sprites+0x2ec7
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x37	@ PACKER: left in the buffer by LuckyDice_Sprites+0x2ec7
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x81	@ PACKER: left in the buffer by LuckyDice_Sprites+0x2ec7
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x1d	@ PACKER: left in the buffer by LuckyDice_Sprites+0x2ec7
	.else
	.byte 0xbe	@ PACKER: left in the buffer by LuckyDice_Sprites+0x2ec7
	.endif
	.endif
	.endif
	.endif
	.endif
