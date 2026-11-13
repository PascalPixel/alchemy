	.section .rodata
@ A copy of The Broken Seal's gBattleEffect_SparkTiles
	.global gBattleEffect_ArcTiles
gBattleEffect_ArcTiles:
	.incbin "games/tla/src/graphics/field_effects/spark.4bpp32x16"
@ A copy of The Broken Seal's gBattleEffect_ArcSparkTiles
	.global gData_080ed90c
gData_080ed90c:
	.incbin "games/tla/src/graphics/field_effects/arc_spark.4bpp32x16"
	.section .rodata.a
@ A 64x64 disc of 256 colours, shaded in colours 224 to 255: the 4,096
@ bytes of the shared effect scene's sprite canvas (battle_fx_work[1]),
@ which Func_080eb824 copies to OBJ video memory. Func_080e572c, 080e5d5c,
@ 080e7844 and overlay 685 unpack it there. In Func_080e572c the callback
@ Func_080e4d9c writes 32 colours it computes from the sine table to OBJ
@ palettes 14 and 15 each frame, from a counter the scene starts at 0; the
@ picture carries the shared OBJ palette and those colours of the first
@ frame.
	.global FieldEffect_GlowDisc
FieldEffect_GlowDisc:
	.incbin "games/tla/src/graphics/field_effects/glow_disc.8bpp.lz"
@ Four pictures of a 32x32 sprite of 256 colours, one under another: two of an
@ orb of rings in colours 224 to 234, then two filled with colour 2.
@ Func_080e9af4 unpacks them into one block of OBJ video memory and makes a
@ 32x32 sprite that shows the first two in turn. Its callback Func_080e9940
@ writes sixteen colours it computes from the table gData_080f0fe0 to OBJ
@ palette 14 each frame, from a counter that starts at 0. The picture
@ carries the shared OBJ palette, those colours of the first frame, and black
@ for palette 15, which it does not use.
	.global FieldEffect_RingOrb
FieldEffect_RingOrb:
	.incbin "games/tla/src/graphics/field_effects/ring_orb.8bpp.lz"
@ Two quarters of a ring, 64x64 each, one under the other: 128 background
@ tiles of 256 colours, drawn in colours 243 to 255. Func_080e82cc unpacks
@ them and copies all 8,192 bytes to background tiles 0 to 127. Its
@ callback Func_080e807c writes sixteen colours it computes to background
@ and OBJ palette 15 each frame, from a counter that starts at 0: the picture
@ carries the colours of the first frame, and black for the 240 it does not
@ use.
	.global FieldEffect_RingArcs
FieldEffect_RingArcs:
	.incbin "games/tla/src/graphics/field_effects/ring_arcs.8bpp.lz"
	.global FieldEffect_RedFlameBurst
FieldEffect_RedFlameBurst:
	.incbin "games/tla/src/graphics/field_effects/red_flame_burst.8bpp.lz"
	.global FieldEffect_VioletFlameBurst
FieldEffect_VioletFlameBurst:
	.incbin "games/tla/src/graphics/field_effects/violet_flame_burst.8bpp.lz"
	.byte 0x05	@ PACKER: left in the buffer by FieldEffect_RedFlameBurst+0x22f
	.global FieldEffect_YellowFlameBurst
FieldEffect_YellowFlameBurst:
	.incbin "games/tla/src/graphics/field_effects/yellow_flame_burst.8bpp.lz"
	.byte 0x50	@ PACKER: left in the buffer by FieldEffect_VioletFlameBurst+0x20b
	.global FieldEffect_BlueFlameBurst
FieldEffect_BlueFlameBurst:
	.incbin "games/tla/src/graphics/field_effects/blue_flame_burst.8bpp.lz"
	.byte 0x27	@ PACKER: left in the buffer by FieldEffect_YellowFlameBurst+0x203
	.global FieldEffect_Leaves
FieldEffect_Leaves:
	.incbin "games/tla/src/graphics/field_effects/leaves.8bpp.lz"
	.byte 0x08, 0xd7	@ PACKER: left in the buffer by FieldEffect_BlueFlameBurst+0x8e
	.global FieldEffect_DustPuffA
FieldEffect_DustPuffA:
	.incbin "games/tla/src/graphics/field_effects/dust_puff.8bpp.lz"
	.global FieldEffect_DustPuffB
FieldEffect_DustPuffB:
	.incbin "games/tla/src/graphics/field_effects/dust_puff.8bpp.lz"
@ Eight 16x16 pictures of 16 colours, one under another: a bordered box whose
@ diagonal fill grows over four pictures, then four empty pictures. No loader of
@ the resource was found; r1e3.png carries the first row of the shared OBJ
@ palette, as r1ed.png does.
	.global Resource_Data1E3
Resource_Data1E3:
	.incbin "games/tla/src/graphics/field_effects/r1e3.4bpp16x16.lz"
	.byte 0x88	@ PACKER: left in the buffer by FieldEffect_DustPuffB+0x6f
	.global FieldEffect_GoldStar
FieldEffect_GoldStar:
	.incbin "games/tla/src/graphics/field_effects/gold_star.8bpp.lz"
	.byte 0x3e	@ PACKER: left in the buffer by Resource_Data1E3+0x5b
	.global FieldEffect_GoldCrystal
FieldEffect_GoldCrystal:
	.incbin "games/tla/src/graphics/field_effects/gold_crystal.8bpp.lz"
	.byte 0xc7, 0xb9	@ PACKER: left in the buffer by FieldEffect_GoldStar+0x32
	.global FieldEffect_FlamingArrow
FieldEffect_FlamingArrow:
	.incbin "games/tla/src/graphics/field_effects/flaming_arrow.8bpp.lz"
	.global FieldEffect_BowDraw
FieldEffect_BowDraw:
	.incbin "games/tla/src/graphics/field_effects/bow_draw.8bpp.lz"
	.byte 0x68, 0xea	@ PACKER: left in the buffer by FieldEffect_DustPuffB+0x196
@ Eight pictures of a 16x16 explosion of 16 colours, one under another.
@ Func_080e572c unpacks them into one block of OBJ video memory and makes 32
@ sprites of them, each with the 256-colour bit cleared and OBJ palette 15;
@ its callback Func_080e4d9c copies the 16 colours of Data_080f0fa0 to OBJ
@ palette 15, and the picture carries them. Overlays 685, 6a6 and 6a7 load
@ the sheet too.
	.global FieldEffect_Explosion
FieldEffect_Explosion:
	.incbin "games/tla/src/graphics/field_effects/explosion.4bpp16x16.lz"
@ A bitmap of 32 x 32 pixels, one byte a pixel in rows of 32, not tiles.
@ Func_080e68d0 unpacks its 1,024 bytes into its work buffer and makes one
@ 32x32 sprite of 256 colours over an empty block of OBJ video memory. Each
@ frame its callback has Func_080e62f4 clear a second 32 x 32 bitmap and plot
@ the first into it column by column, the rows four pixels apart and moved by
@ the frame count and by a sine of the column; the second bitmap is what goes
@ to the sprite's video memory.
	.global Resource_Data1E9
Resource_Data1E9:
	.incbin "games/tla/src/graphics/field_effects/r1e9.bitmap.lz"
	.byte 0x59, 0x2f	@ PACKER: left in the buffer by FieldEffect_Explosion+0xe2
	.global FieldEffect_SparkleFade
FieldEffect_SparkleFade:
	.incbin "games/tla/src/graphics/field_effects/sparkle_fade.8bpp.lz"
	.section .rodata.b
@ Four pictures of one 16x16 sprite of 16 colours, 512 bytes. Func_080e82cc
@ unpacks them, loads them as one block of OBJ video memory and makes 32
@ sprites of them, each with the 256-colour bit cleared and OBJ palette 15.
@ Its callback Func_080e807c steps every sprite through the four pictures, four
@ frames a picture, and each frame writes sixteen colours it computes to OBJ
@ palette 15. The game stores no palette for this sheet: r1ed.png carries the
@ first row of the shared OBJ palette, which the 256-colour sheets of this
@ folder carry whole.
	.global Resource_Data1ED
Resource_Data1ED:
	.incbin "games/tla/src/graphics/field_effects/r1ed.4bpp16x16.lz"
@ Two 8x8 tiles of 16 colours, one under the other. Func_080e8db0 unpacks
@ them, loads them as one block of OBJ video memory and makes 64 sprites
@ that alternate the two, each with the 256-colour bit cleared and OBJ
@ palette 14. Where that palette's colours come from is not established:
@ r1ee.png carries the first row of the shared OBJ palette, as r1ed.png does.
	.global Resource_Data1EE
Resource_Data1EE:
	.incbin "games/tla/src/graphics/field_effects/r1ee.4bpp.lz"
	.byte 0x9f, 0x42	@ PACKER: left in the buffer by Resource_Data1ED+0x22
	.section .rodata.c
	.global FieldEffect_FallingRock
FieldEffect_FallingRock:
	.incbin "games/tla/src/graphics/field_effects/falling_rock.8bpp.lz"
	.byte 0xde, 0x47, 0x2f	@ PACKER: left in the buffer by FieldEffect_ColouredOrbs+0xe1
	.global FieldEffect_WaterSplash
FieldEffect_WaterSplash:
	.incbin "games/tla/src/graphics/field_effects/water_splash.8bpp.lz"
	.byte 0x1b, 0x3e, 0x01	@ PACKER: left in the buffer by FieldEffect_FallingRock+0xcd
	.global FieldEffect_IceShard
FieldEffect_IceShard:
	.incbin "games/tla/src/graphics/field_effects/ice_shard.8bpp.lz"
	.byte 0x08	@ PACKER: unused post-stream residue; Resource_DecodeType01 uses decoded RAM and output length; writer unknown
	.global FieldEffect_WhiteSparkle
FieldEffect_WhiteSparkle:
	.incbin "games/tla/src/graphics/field_effects/white_sparkle.8bpp.lz"
	.byte 0xd2, 0xe2	@ PACKER: left in the buffer by FieldEffect_IceShard+0x2e
	.global FieldEffect_Runes
FieldEffect_Runes:
	.incbin "games/tla/src/graphics/field_effects/runes.8bpp.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global FieldEffect_Pebbles
FieldEffect_Pebbles:
	.incbin "games/tla/src/graphics/field_effects/pebbles.8bpp.lz"
	.byte 0x49	@ PACKER: left in the buffer by FieldEffect_Runes+0x9b
	.global FieldEffect_GoldenTrident
FieldEffect_GoldenTrident:
	.incbin "games/tla/src/graphics/field_effects/golden_trident.8bpp.lz"
@ A 32x64 sprite of 256 colours: a column shaded in colours 224 to 255.
@ Overlay 685's Func_02002d34 unpacks it, turns each colour 240 to 255 into
@ 96 to 111 and makes one 32x64 sprite of 256 colours. Its callback
@ Func_02002de4 writes 32 colours it computes from the sine table to OBJ
@ colours 96 to 111 and 224 to 239 each frame, from a counter that starts at
@ 0. The picture carries the shared OBJ palette and, for its colours 224 to
@ 255, the colours the sprite shows for them on the first frame.
	.global FieldEffect_FireColumn
FieldEffect_FireColumn:
	.incbin "games/tla/src/graphics/field_effects/fire_column.8bpp.lz"
	.byte 0xee, 0xd3	@ PACKER: left in the buffer by FieldEffect_GlowDisc+0x3fe
	.section .rodata.d
	.global FieldEffect_StarSparkleFade
FieldEffect_StarSparkleFade:
	.incbin "games/tla/src/graphics/field_effects/star_sparkle_fade.8bpp.lz"
	.ifdef TLA_EDITION_JA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.byte 0x01, 0x00, 0x00	@ PACKER: unused post-stream residue; Resource_DecodeType01 uses decoded RAM and output length; writer unknown
	.endif
	.global FieldEffect_WhitePuff
FieldEffect_WhitePuff:
	.incbin "games/tla/src/graphics/field_effects/white_puff.8bpp.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x00, 0x0b	@ PACKER: left in the buffer by FieldEffect_Explosion+0x1d2
	.else
	.byte 0x12, 0x40	@ PACKER: unused post-stream residue; Resource_DecodeType01 uses decoded RAM and output length; writer unknown
	.endif
