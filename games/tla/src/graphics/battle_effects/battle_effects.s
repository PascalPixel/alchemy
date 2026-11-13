	.section .rodata
	.global BattleEffect_GlowOrbSetSheet
BattleEffect_GlowOrbSetSheet:
	.incbin "games/tla/src/graphics/battle_effects/glow_orb_set_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/glow_orb_set.parts.lz"
@ An archer's two 88x96 pictures and three small pictures, in their own 64-colour
@ palette: the sizes BattleEffect_RunVengeance draws them at
	.global Resource_Data0B0
Resource_Data0B0:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0b0_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_0b0.parts.lz"
	.byte 0x46, 0xcd	@ PACKER: left in the buffer by Summon_ChainedDemonTiles+0x1c4e
@ Uncolored art using BattleEffect_RuneSheet colors (same effect).
	.global Resource_Data0B1
Resource_Data0B1:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0b1.bitmap.lz"
@ A 64x64 silhouette with no palette of its own. No code loads it: only the
@ resource directory names it. The PNG carries the palette of resource 0xB3,
@ the next resource and another silhouette, as a neighbour's, not as a use
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0b2.bitmap.lz"
@ Eighteen cels of a fluttering silhouette in their own 64-colour palette, at
@ the sizes and offsets of BattleEffect_RunUlysses's cel tables
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0b3_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_0b3.parts.lz"
	.byte 0x03, 0x71	@ PACKER: left in the buffer by Resource_Data0B2+0x196
@ Uncolored art using BattleEffect_MarsDjinnSheet colors (same effect).
	.global Resource_Data0B4
Resource_Data0B4:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0b4.bitmap.lz"
	.byte 0x7b, 0x3d, 0x80	@ PACKER: left in the buffer by Resource_Data0B0+0x829
	.global Resource_Data0B5
Resource_Data0B5:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0b5.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_0b5.bitmap.lz"
	.global BattleEffect_CharmRingSheet
BattleEffect_CharmRingSheet:
	.incbin "games/tla/src/graphics/battle_effects/charm_ring.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/charm_ring.bitmap.lz"
	.byte 0x76	@ PACKER: left in the buffer by Resource_Data0B5+0x637
	.global BattleEffect_WhirlBeamSheet
BattleEffect_WhirlBeamSheet:
	.incbin "games/tla/src/graphics/battle_effects/whirl_beam.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/whirl_beam.bitmap.lz"
	.byte 0x54, 0xaa, 0x14	@ PACKER: left in the buffer by Summon_ChainedDemonTiles+0x2169
	.global BattleEffect_EffectPalette00
BattleEffect_EffectPalette00:
	.incbin "games/tla/src/graphics/battle_effects/effect_palette00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/effect_palette00.bitmap.lz"
	.global BattleEffect_EffectPalette01
BattleEffect_EffectPalette01:
	.incbin "games/tla/src/graphics/battle_effects/effect_palette01.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/effect_palette01.bitmap.lz"
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0BA
Resource_Data0BA:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0ba.bitmap.lz"
	.byte 0x20	@ PACKER: left in the buffer by BattleEffect_WhirlBeamSheet+0x24b
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0BB
Resource_Data0BB:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0bb.bitmap.lz"
	.byte 0x63	@ PACKER: left in the buffer by Resource_Data0BA+0x183
	.global BattleEffect_ThinBoltSheet
BattleEffect_ThinBoltSheet:
	.incbin "games/tla/src/graphics/battle_effects/thin_bolt.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/thin_bolt.bitmap.lz"
	.byte 0x30, 0x19, 0x25	@ PACKER: left in the buffer by BattleEffect_WhirlBeamSheet+0x8b9
	.global BattleEffect_BoltSheet
BattleEffect_BoltSheet:
	.incbin "games/tla/src/graphics/battle_effects/bolt.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/bolt.bitmap.lz"
	.byte 0xcb, 0x31, 0x16	@ PACKER: left in the buffer by BattleEffect_WhirlBeamSheet+0x1be1
	.global BattleEffect_CurseSealSheet
BattleEffect_CurseSealSheet:
	.incbin "games/tla/src/graphics/battle_effects/curse_seal.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/curse_seal.bitmap.lz"
	.byte 0xe1, 0xef	@ PACKER: left in the buffer by BattleEffect_BoltSheet+0x1b5a
	.global BattleEffect_CrackedTextureSheet
BattleEffect_CrackedTextureSheet:
	.incbin "games/tla/src/graphics/battle_effects/cracked_texture.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/cracked_texture.bitmap.lz"
	.byte 0x0e, 0x30, 0x00	@ PACKER: left in the buffer by BattleEffect_CurseSealSheet+0x43d
	.global BattleEffect_CrossTextureSheet
BattleEffect_CrossTextureSheet:
	.incbin "games/tla/src/graphics/battle_effects/cross_texture.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/cross_texture.bitmap.lz"
	.byte 0x96, 0x1d	@ PACKER: left in the buffer by BattleEffect_CrackedTextureSheet+0x412
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0C1
Resource_Data0C1:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0c1.bitmap.lz"
	.byte 0xfc, 0x52, 0x05	@ PACKER: left in the buffer by BattleEffect_CrossTextureSheet+0x335
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0C2
Resource_Data0C2:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0c2.bitmap.lz"
	.byte 0xb0, 0x3d	@ PACKER: left in the buffer by BattleEffect_CurseSealSheet+0x10ca
@ A 64x64 ring with no palette of its own: BattleEffect_RunCharon maps it as a 64 by
@ 64 texture. That effect colours it with a ramp it computes; the PNG carries
@ BattleEffect_BoltSheet's palette, the one it loads most often
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0c3.bitmap.lz"
	.byte 0x08, 0x81	@ PACKER: left in the buffer by Resource_Data0C2+0x2e6
@ Uncolored art using BattleEffect_EffectPalette02 colors (same effect).
	.global Resource_Data0C4
Resource_Data0C4:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0c4.bitmap.lz"
	.byte 0xaa, 0x32	@ PACKER: left in the buffer by Resource_Data0C3+0x152
	.global BattleEffect_PetalSheet
BattleEffect_PetalSheet:
	.incbin "games/tla/src/graphics/battle_effects/petal.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/petal.bitmap.lz"
	.byte 0x68	@ PACKER: left in the buffer by Summon_ChainedDemonTiles+0x250b
	.global BattleEffect_CrescentRowSheet
BattleEffect_CrescentRowSheet:
	.incbin "games/tla/src/graphics/battle_effects/crescent_row.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/crescent_row.bitmap.lz"
	.byte 0x26	@ PACKER: left in the buffer by BattleEffect_PetalSheet+0x9bb
	.global BattleEffect_RuneCircleSheet
BattleEffect_RuneCircleSheet:
	.incbin "games/tla/src/graphics/battle_effects/rune_circle.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/rune_circle.bitmap.lz"
	.byte 0x6b, 0xa1	@ PACKER: left in the buffer by BattleEffect_PetalSheet+0x1142
@ Uncolored art using BattleEffect_RuneSheet colors (same effect).
	.global Resource_Data0C8
Resource_Data0C8:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0c8.bitmap.lz"
@ A 32x4 texture with no palette of its own, a band that brightens to its
@ middle: the effects that map it give it 32 by 4 texels (2^5 by 2^2), and
@ most of them colour it with BattleEffect_OrangePaletteB
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0c9.bitmap.lz"
	.byte 0x1e, 0x00, 0x84	@ PACKER: left in the buffer by Resource_Data0C8+0x29
@ A 32x4 texture with no palette of its own, a band that brightens to its
@ middle, as 0xC9 is: BattleEffect_RunCruelRuin maps it with the same 32 by 4 texels, and
@ BattleEffect_RunEclipse colours it with BattleEffect_BlueFlameSheet
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0ca.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0CB
Resource_Data0CB:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0cb.bitmap.lz"
	.byte 0x40, 0xe7, 0x09	@ PACKER: left in the buffer by Resource_Data0C8+0x7bd
	.global BattleEffect_CloudSwirlSheet
BattleEffect_CloudSwirlSheet:
	.incbin "games/tla/src/graphics/battle_effects/cloud_swirl.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/cloud_swirl.bitmap.lz"
	.byte 0xee	@ PACKER: left in the buffer by BattleEffect_PetalSheet+0x157b
	.global BattleEffect_DustBlobSheet
BattleEffect_DustBlobSheet:
	.incbin "games/tla/src/graphics/battle_effects/dust_blob.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/dust_blob.bitmap.lz"
	.byte 0x54, 0x7b, 0x1f	@ PACKER: left in the buffer by BattleEffect_CloudSwirlSheet+0x329
@ Uncolored art using BattleEffect_BlueFlameSheet colors (same effect).
	.global Resource_Data0CE
Resource_Data0CE:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0ce.bitmap.lz"
	.byte 0xf7, 0x1c	@ PACKER: left in the buffer by BattleEffect_CloudSwirlSheet+0x526
@ Uncolored art using BattleEffect_BlueFlameSheet colors (same effect).
	.global Resource_Data0CF
Resource_Data0CF:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0cf.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
@ Uncolored art using BattleEffect_BlueFlameSheet colors (same effect).
	.global Resource_Data0D0
Resource_Data0D0:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0d0.bitmap.lz"
	.global BattleEffect_FeatherSheet
BattleEffect_FeatherSheet:
	.incbin "games/tla/src/graphics/battle_effects/feather.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/feather.bitmap.lz"
	.byte 0xda, 0x25, 0x56	@ PACKER: left in the buffer by BattleEffect_CloudSwirlSheet+0x789
@ Uncolored art using BattleEffect_MarsDjinnSheet colors (same effect).
	.global Resource_Data0D2
Resource_Data0D2:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0d2.bitmap.lz"
@ Uncolored art using BattleEffect_BlueFlameSheet colors (same effect).
	.global Resource_Data0D3
Resource_Data0D3:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0d3.bitmap.lz"
	.byte 0x84, 0x2a, 0x0c	@ PACKER: left in the buffer by BattleEffect_FeatherSheet+0x41d
	.global BattleEffect_LightBurstSheet
BattleEffect_LightBurstSheet:
	.incbin "games/tla/src/graphics/battle_effects/light_burst.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/light_burst.bitmap.lz"
	.byte 0x38, 0x01, 0x26	@ PACKER: left in the buffer by BattleEffect_FeatherSheet+0x629
	.global BattleEffect_StarCrossSheet
BattleEffect_StarCrossSheet:
	.incbin "games/tla/src/graphics/battle_effects/star_cross.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/star_cross.bitmap.lz"
	.global BattleEffect_CornerGlowSheet
BattleEffect_CornerGlowSheet:
	.incbin "games/tla/src/graphics/battle_effects/corner_glow.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/corner_glow.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.section .rodata.a
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0D8
Resource_Data0D8:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0d8.bitmap.lz"
	.byte 0x05	@ PACKER: left in the buffer by BattleEffect_LightBurstSheet+0x57b
	.global BattleEffect_ImpFaceSheet
BattleEffect_ImpFaceSheet:
	.incbin "games/tla/src/graphics/battle_effects/imp_face.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/imp_face.bitmap.lz"
	.byte 0x07, 0xc7, 0x80	@ PACKER: left in the buffer by Resource_Data0D8+0x2a9
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0DA
Resource_Data0DA:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0da.bitmap.lz"
	.global BattleEffect_GreatSwordSheet
BattleEffect_GreatSwordSheet:
	.incbin "games/tla/src/graphics/battle_effects/great_sword.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/great_sword.bitmap.lz"
	.byte 0x70, 0xa8, 0x48	@ PACKER: left in the buffer by Resource_Data0DA+0x6bd
	.global Resource_Data0DC
Resource_Data0DC:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0dc_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_0dc.parts.lz"
	.byte 0x54	@ PACKER: left in the buffer by Summon_ChainedDemonTiles+0x2aaf
	.global Resource_Data0DD
Resource_Data0DD:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0dd_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_0dd.parts.lz"
	.byte 0xbf	@ PACKER: left in the buffer by Resource_Data0DC+0x11cb
	.global BattleEffect_SunCardSheet
BattleEffect_SunCardSheet:
	.incbin "games/tla/src/graphics/battle_effects/sun_card.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sun_card.bitmap.lz"
	.byte 0x01	@ PACKER: left in the buffer by Resource_Data0DD+0x65f
	.global BattleEffect_VineCardSheet
BattleEffect_VineCardSheet:
	.incbin "games/tla/src/graphics/battle_effects/vine_card.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/vine_card.bitmap.lz"
	.byte 0x1d, 0x6a, 0x00	@ PACKER: left in the buffer by Resource_Data0DD+0x1039
	.global BattleEffect_DragonCardSheet
BattleEffect_DragonCardSheet:
	.incbin "games/tla/src/graphics/battle_effects/dragon_card.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/dragon_card.bitmap.lz"
	.byte 0x01	@ PACKER: left in the buffer by BattleEffect_VineCardSheet+0x653
	.global BattleEffect_BoltCardSheet
BattleEffect_BoltCardSheet:
	.incbin "games/tla/src/graphics/battle_effects/bolt_card.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/bolt_card.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleEffect_ButterflyCardSheet
BattleEffect_ButterflyCardSheet:
	.incbin "games/tla/src/graphics/battle_effects/butterfly_card.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/butterfly_card.bitmap.lz"
	.byte 0xcc, 0x17, 0x03	@ PACKER: left in the buffer by BattleEffect_BoltCardSheet+0x61d
	.global BattleEffect_SheepCardSheet
BattleEffect_SheepCardSheet:
	.incbin "games/tla/src/graphics/battle_effects/sheep_card.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheep_card.bitmap.lz"
	.global BattleEffect_SkeletonCardSheet
BattleEffect_SkeletonCardSheet:
	.incbin "games/tla/src/graphics/battle_effects/skeleton_card.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/skeleton_card.bitmap.lz"
	.byte 0x7c, 0x04	@ PACKER: left in the buffer by BattleEffect_SheepCardSheet+0x6e6
	.global BattleEffect_SwordCardSheet
BattleEffect_SwordCardSheet:
	.incbin "games/tla/src/graphics/battle_effects/sword_card.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sword_card.bitmap.lz"
	.global BattleEffect_BoulderPicture
BattleEffect_BoulderPicture:
	.incbin "games/tla/src/graphics/battle_effects/boulder_picture.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/boulder_picture.bitmap.lz"
	.byte 0x1d, 0x6f	@ PACKER: left in the buffer by BattleEffect_SheepCardSheet+0xa1e
	.global BattleEffect_BladeSheet
BattleEffect_BladeSheet:
	.incbin "games/tla/src/graphics/battle_effects/blade.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blade.bitmap.lz"
	.global BattleEffect_ClawSlashSheet
BattleEffect_ClawSlashSheet:
	.incbin "games/tla/src/graphics/battle_effects/claw_slash.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/claw_slash.bitmap.lz"
	.byte 0x01	@ PACKER: left in the buffer by BattleEffect_BoulderPicture+0x6af
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0E9
Resource_Data0E9:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0e9.bitmap.lz"
	.byte 0x1f, 0x50	@ PACKER: left in the buffer by BattleEffect_ClawSlashSheet+0x2ca
	.global BattleEffect_CrystalBitmapSheet
BattleEffect_CrystalBitmapSheet:
	.incbin "games/tla/src/graphics/battle_effects/crystal.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/crystal.bitmap.lz"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.global BattleEffect_SmallFlameSheet
BattleEffect_SmallFlameSheet:
	.incbin "games/tla/src/graphics/battle_effects/small_flame.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/small_flame.bitmap.lz"
	.section .rodata.b
	.global BattleEffect_CornerRuneBSheet
BattleEffect_CornerRuneBSheet:
	.incbin "games/tla/src/graphics/battle_effects/corner_rune_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/corner_rune_b.bitmap.lz"
	.byte 0x47	@ PACKER: left in the buffer by BattleEffect_CornerRuneASheet+0x947
@ Uncolored art using BattleEffect_EffectPalette02 colors (same effect).
	.global Resource_Data0EE
Resource_Data0EE:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0ee.bitmap.lz"
	.byte 0x01, 0x2e	@ PACKER: left in the buffer by BattleEffect_CornerRuneBSheet+0x65a
@ Uncolored art using BattleEffect_EffectPalette02 colors (same effect).
	.global Resource_Data0EF
Resource_Data0EF:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0ef.bitmap.lz"
	.byte 0x41, 0x90	@ PACKER: left in the buffer by Resource_Data0EE+0x52a
	.global Resource_Data0F0
Resource_Data0F0:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f0_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f0.parts.lz"
	.byte 0x40, 0x83, 0x0f	@ PACKER: left in the buffer by Resource_Data0DC+0x22b9
	.global Resource_Data0F1
Resource_Data0F1:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f1_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f1.parts.lz"
	.byte 0xc2, 0x3a, 0x02	@ PACKER: left in the buffer by Resource_Data0F0+0x1791
@ Uncolored art using BattleEffect_MercuryDjinnSheet colors (same effect).
	.global Resource_Data0F2
Resource_Data0F2:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f2.bitmap.lz"
	.byte 0x2e, 0x15, 0x23	@ PACKER: left in the buffer by Resource_Data0F1+0x6e1
@ Five cels of a flame with no palette of their own, at the sizes and offsets
@ BattleEffect_RunWhiplash draws them at; that effect loads BattleEffect_EffectPalette02 to
@ colour them
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f3.parts.lz"
	.byte 0xa8	@ PACKER: left in the buffer by Resource_Data0F0+0x1f4b
	.global BattleEffect_SmokeBandSheet
BattleEffect_SmokeBandSheet:
	.incbin "games/tla/src/graphics/battle_effects/smoke_band.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/smoke_band.bitmap.lz"
	.global Resource_Data0F5
Resource_Data0F5:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f5_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f5.parts.lz"
	.byte 0xe4, 0x8a, 0x13	@ PACKER: left in the buffer by Resource_Data0F3+0x11d5
	.global BattleEffect_DustCloudSheet
BattleEffect_DustCloudSheet:
	.incbin "games/tla/src/graphics/battle_effects/dust_cloud.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/dust_cloud.bitmap.lz"
	.byte 0x9a, 0xcb	@ PACKER: left in the buffer by Resource_Data0F5+0x4ee
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0F7
Resource_Data0F7:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f7.bitmap.lz"
	.byte 0x00, 0x38	@ PACKER: left in the buffer by Resource_Data0F5+0x646
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0F8
Resource_Data0F8:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f8.bitmap.lz"
	.byte 0xad	@ PACKER: left in the buffer by Resource_Data0F5+0xc23
@ Uncolored art using BattleEffect_LightningBoltSheet colors (same effect).
	.global Resource_Data0F9
Resource_Data0F9:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f9.bitmap.lz"
	.byte 0xa3, 0x08, 0x1a	@ PACKER: left in the buffer by Resource_Data0F8+0x3c1
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0FA
Resource_Data0FA:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0fa.bitmap.lz"
	.byte 0xb2	@ PACKER: left in the buffer by Resource_Data0F9+0x1c7
@ Uncolored art using BattleEffect_BlueFlameSheet colors (same effect).
	.global Resource_Data0FB
Resource_Data0FB:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0fb.bitmap.lz"
	.byte 0x3e, 0xe1	@ PACKER: left in the buffer by Resource_Data0F8+0x54a
@ Uncolored art using BattleEffect_BlueFlameSheet colors (same effect).
	.global Resource_Data0FC
Resource_Data0FC:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0fc.bitmap.lz"
	.byte 0x6b, 0x06	@ PACKER: left in the buffer by Resource_Data0FB+0x34a
@ Fifteen cels of a glowing oval shrinking from 16x32 to 3x4, with no palette of
@ their own: the sizes and offsets of Func_08182898's cel tables. That effect
@ loads BattleEffect_BlueBeamSheet's palette to colour them
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0fd.parts.lz"
	.global BattleEffect_ArcFragmentsSheet
BattleEffect_ArcFragmentsSheet:
	.incbin "games/tla/src/graphics/battle_effects/arc_fragments.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/arc_fragments.bitmap.lz"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data0FD+0x643
@ A 17x2 strip in its own 64-colour palette: BattleEffect_RunProjectileVolley draws it in modes 15 to 17
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0ff.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_0ff.bitmap.lz"
	.byte 0x72	@ PACKER: left in the buffer by BattleEffect_ArcFragmentsSheet+0xa7
	.global BattleEffect_AxeSheet
BattleEffect_AxeSheet:
	.incbin "games/tla/src/graphics/battle_effects/axe.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/axe.bitmap.lz"
	.byte 0x10, 0x00	@ PACKER: left in the buffer by BattleEffect_ArcFragmentsSheet+0x316
	.global Resource_Data101
Resource_Data101:
	.incbin "games/tla/src/graphics/battle_effects/sheet_101.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_101.bitmap.lz"
	.byte 0xa5, 0x0a, 0x23	@ PACKER: left in the buffer by Resource_Data0F8+0x9a5
@ A 40x80 spear with no palette of its own: BattleEffect_RunOdyssey draws it at that
@ size, after copying Resource_Data101's palette into BG palette RAM
	.global Resource_Data102
Resource_Data102:
	.incbin "games/tla/src/graphics/battle_effects/sheet_102.bitmap.lz"
	.global Resource_Data103
Resource_Data103:
	.incbin "games/tla/src/graphics/battle_effects/sheet_103.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_103.8bpp.lz"
	.byte 0x31, 0x32, 0x8d	@ PACKER: left in the buffer by Resource_Data0F8+0xb2d
@ A 64x32 bar with a rounded end and no palette of its own: BattleEffect_RunCruelRuin
@ maps it as a 64 by 32 texture and shows it in a grey ramp it writes in
@ code; the PNG carries BattleEffect_CornerRuneASheet's palette, the one 64-colour
@ palette that effect loads
	.global Resource_Data104
Resource_Data104:
	.incbin "games/tla/src/graphics/battle_effects/sheet_104.bitmap.lz"
	.byte 0x89, 0x22, 0x69	@ PACKER: left in the buffer by Resource_Data103+0xfd
	.section .rodata.c
	.global BattleEffect_TileAnimationMask
BattleEffect_TileAnimationMask:
	.incbin "games/tla/src/graphics/battle_effects/tile_mask_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/tile_mask.parts.lz"
	.byte 0x38, 0xfa, 0x06	@ PACKER: left in the buffer by BattleEffect_CloudPuffSheet+0x845
	.global BattleEffect_CyanSparkSheet
BattleEffect_CyanSparkSheet:
	.incbin "games/tla/src/graphics/battle_effects/cyan_spark_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/cyan_spark.parts.lz"
	.global BattleEffect_VioletPaletteA
BattleEffect_VioletPaletteA:
	.incbin "games/tla/src/graphics/battle_effects/violet_palette_a.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/violet_palette_a.bitmap.lz"
	.global BattleEffect_RedPaletteA
BattleEffect_RedPaletteA:
	.incbin "games/tla/src/graphics/battle_effects/red_palette_a.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/red_palette_a.bitmap.lz"
	.global BattleEffect_YellowPaletteA
BattleEffect_YellowPaletteA:
	.incbin "games/tla/src/graphics/battle_effects/yellow_palette_a.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/yellow_palette_a.bitmap.lz"
	.global BattleEffect_GreyCrescentSheet
BattleEffect_GreyCrescentSheet:
	.incbin "games/tla/src/graphics/battle_effects/grey_crescent.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/grey_crescent.bitmap.lz"
	.byte 0x06	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0xd33
	.global BattleEffect_StarCrescentSheet
BattleEffect_StarCrescentSheet:
	.incbin "games/tla/src/graphics/battle_effects/star_crescent.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/star_crescent.bitmap.lz"
	.byte 0x03	@ PACKER: left in the buffer by BattleEffect_GreyCrescentSheet+0xadb
	.global BattleEffect_RevealMaskA
BattleEffect_RevealMaskA:
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_a.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_a.bitmap.lz"
	.byte 0x52, 0x22	@ PACKER: left in the buffer by BattleEffect_StarCrescentSheet+0x6c2
	.global BattleEffect_RevealMaskB
BattleEffect_RevealMaskB:
	.incbin "games/tla/src/graphics/battle_effects/reveal_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/reveal_b.bitmap.lz"
	.byte 0x00, 0x7b, 0x09	@ PACKER: left in the buffer by BattleEffect_RevealMaskA+0x2e5
	.global BattleEffect_RevealMaskC
BattleEffect_RevealMaskC:
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_c.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_c.bitmap.lz"
	.global BattleEffect_RevealMaskD
BattleEffect_RevealMaskD:
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_d.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_d.bitmap.lz"
	.global BattleEffect_RevealMaskE
BattleEffect_RevealMaskE:
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_e.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_e.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleEffect_RevealMaskF
BattleEffect_RevealMaskF:
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_f.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_f.bitmap.lz"
	.byte 0x30, 0xe1, 0x08	@ PACKER: left in the buffer by BattleEffect_RevealMaskE+0x3e9
	.global BattleEffect_LimeColumnSheet
BattleEffect_LimeColumnSheet:
	.incbin "games/tla/src/graphics/battle_effects/lime_column.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/lime_column.bitmap.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleEffect_RevealMaskF+0x3cf
	.global BattleEffect_RainSheet
BattleEffect_RainSheet:
	.incbin "games/tla/src/graphics/battle_effects/rain.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/rain.bitmap.lz"
	.byte 0x27	@ PACKER: left in the buffer by BattleEffect_StarCrescentSheet+0x89f
	.global BattleEffect_FlameSheetA
BattleEffect_FlameSheetA:
	.incbin "games/tla/src/graphics/battle_effects/flame_a.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/flame_a.bitmap.lz"
	.byte 0x2f, 0x18	@ PACKER: left in the buffer by BattleEffect_RainSheet+0x4a2
	.global BattleEffect_DemonFaceSheet
BattleEffect_DemonFaceSheet:
	.incbin "games/tla/src/graphics/battle_effects/demon_face.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/demon_face.bitmap.lz"
	.global BattleEffect_FlameBladeSheet
BattleEffect_FlameBladeSheet:
	.incbin "games/tla/src/graphics/battle_effects/flame_blade.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/flame_blade.bitmap.lz"
	.byte 0xff	@ PACKER: left in the buffer by BattleEffect_GreyCrescentSheet+0xc83
	.global BattleEffect_SwordSheet
BattleEffect_SwordSheet:
	.incbin "games/tla/src/graphics/battle_effects/sword.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sword.bitmap.lz"
	.global BattleEffect_IceTileSheet
BattleEffect_IceTileSheet:
	.incbin "games/tla/src/graphics/battle_effects/ice_tile_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/ice_tile.parts.lz"
	.byte 0x00, 0x01	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0x106a
	.global BattleEffect_FiveModeImage
BattleEffect_FiveModeImage:
	.incbin "games/tla/src/graphics/battle_effects/five_mode.bitmap.lz"
	.byte 0x96	@ PACKER: left in the buffer by BattleEffect_IceTileSheet+0xfc7
	.global BattleEffect_SpiderWebSheet
BattleEffect_SpiderWebSheet:
	.incbin "games/tla/src/graphics/battle_effects/spider_web.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/spider_web.bitmap.lz"
	.byte 0x18, 0x85	@ PACKER: left in the buffer by BattleEffect_FiveModeImage+0x25a
	.global BattleEffect_VioletLightningSheetA
BattleEffect_VioletLightningSheetA:
	.incbin "games/tla/src/graphics/battle_effects/violet_lightning_a.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/violet_lightning_a.bitmap.lz"
	.global BattleEffect_YellowSparkSheet
BattleEffect_YellowSparkSheet:
	.incbin "games/tla/src/graphics/battle_effects/yellow_spark_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/yellow_spark.parts.lz"
	.byte 0x17, 0x6e, 0xa0	@ PACKER: left in the buffer by BattleEffect_VioletLightningSheetA+0x2fd
	.global BattleEffect_ProjectileVolleyImage
BattleEffect_ProjectileVolleyImage:
	.incbin "games/tla/src/graphics/battle_effects/projectile_volley.bitmap.lz"
	.global BattleEffect_ShurikenSheet
BattleEffect_ShurikenSheet:
	.incbin "games/tla/src/graphics/battle_effects/shuriken.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/shuriken.bitmap.lz"
	.byte 0x47, 0x8d, 0x13	@ PACKER: left in the buffer by BattleEffect_VioletLightningSheetA+0x395
	.global BattleEffect_FlareSheet
BattleEffect_FlareSheet:
	.incbin "games/tla/src/graphics/battle_effects/flare_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/flare.parts.lz"
	.byte 0x26, 0x20	@ PACKER: left in the buffer by Resource_Data0F3+0x1d86
	.global BattleEffect_FlareImage
BattleEffect_FlareImage:
	.incbin "games/tla/src/graphics/battle_effects/flare_cels.bitmap.lz"
	.byte 0x05	@ PACKER: left in the buffer by BattleEffect_FlareSheet+0x277
	.global BattleEffect_VioletPaletteB
BattleEffect_VioletPaletteB:
	.incbin "games/tla/src/graphics/battle_effects/violet_palette_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/violet_palette_b.bitmap.lz"
	.global BattleEffect_FireSwirlSheet
BattleEffect_FireSwirlSheet:
	.incbin "games/tla/src/graphics/battle_effects/fire_swirl_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/fire_swirl.parts.lz"
	.byte 0x12, 0x21	@ PACKER: left in the buffer by BattleEffect_FlareSheet+0x442
	.global BattleEffect_VioletLightningSheetB
BattleEffect_VioletLightningSheetB:
	.incbin "games/tla/src/graphics/battle_effects/violet_lightning_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/violet_lightning_b.bitmap.lz"
	.byte 0xe5	@ PACKER: left in the buffer by BattleEffect_FlareSheet+0x1bd3
	.global BattleEffect_FireBurstSheet
BattleEffect_FireBurstSheet:
	.incbin "games/tla/src/graphics/battle_effects/fire_burst.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/fire_burst.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleEffect_BlueCrescentSheet
BattleEffect_BlueCrescentSheet:
	.incbin "games/tla/src/graphics/battle_effects/blue_crescent.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blue_crescent.bitmap.lz"
	.byte 0xe0, 0x02	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x21e
	.global BattleEffect_OverlayImageA
BattleEffect_OverlayImageA:
	.incbin "games/tla/src/graphics/battle_effects/overlay_a.parts.lz"
	.byte 0x88	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x43b
	.global BattleEffect_VioletMoteSheet
BattleEffect_VioletMoteSheet:
	.incbin "games/tla/src/graphics/battle_effects/violet_mote.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/violet_mote.bitmap.lz"
	.byte 0x28, 0x41, 0x42	@ PACKER: left in the buffer by BattleEffect_OverlayImageA+0x111
	.global BattleEffect_FireballSheet
BattleEffect_FireballSheet:
	.incbin "games/tla/src/graphics/battle_effects/fireball.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/fireball.bitmap.lz"
	.byte 0x32	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x5df
	.global BattleEffect_MemberBurstImage
BattleEffect_MemberBurstImage:
	.incbin "games/tla/src/graphics/battle_effects/member_burst.bitmap.lz"
	.byte 0x19, 0x14, 0x56	@ PACKER: left in the buffer by BattleEffect_FireballSheet+0x49d
	.global BattleEffect_SlashSheet
BattleEffect_SlashSheet:
	.incbin "games/tla/src/graphics/battle_effects/slash_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/slash.parts.lz"
	.byte 0xf1, 0x59	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0xd12
	.global BattleEffect_WaveSheet
BattleEffect_WaveSheet:
	.incbin "games/tla/src/graphics/battle_effects/wave.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/wave.bitmap.lz"
	.global BattleEffect_TargetBurstImage
BattleEffect_TargetBurstImage:
	.incbin "games/tla/src/graphics/battle_effects/target_burst.bitmap.lz"
	.byte 0x2f, 0x00	@ PACKER: left in the buffer by BattleEffect_SlashSheet+0x83e
	.global BattleEffect_PinkStarSheet
BattleEffect_PinkStarSheet:
	.incbin "games/tla/src/graphics/battle_effects/pink_star.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/pink_star.bitmap.lz"
	.byte 0x80	@ PACKER: left in the buffer by BattleEffect_TargetBurstImage+0x38f
	.global BattleEffect_WaterSpraySheet
BattleEffect_WaterSpraySheet:
	.incbin "games/tla/src/graphics/battle_effects/water_spray_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/water_spray.parts.lz"
	.global BattleEffect_BlueFlameSheet
BattleEffect_BlueFlameSheet:
	.incbin "games/tla/src/graphics/battle_effects/blue_flame_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blue_flame.parts.lz"
	.byte 0xb7, 0x7c	@ PACKER: left in the buffer by BattleEffect_WaterSpraySheet+0xaee
	.global BattleEffect_ExplosionSheet
BattleEffect_ExplosionSheet:
	.incbin "games/tla/src/graphics/battle_effects/explosion.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/explosion.bitmap.lz"
	.byte 0xec, 0x10, 0x3c	@ PACKER: left in the buffer by BattleEffect_WaterSpraySheet+0xdd5
	.global BattleEffect_MagentaSwirlSheet
BattleEffect_MagentaSwirlSheet:
	.incbin "games/tla/src/graphics/battle_effects/magenta_swirl.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/magenta_swirl.bitmap.lz"
	.byte 0x1c, 0x4c	@ PACKER: left in the buffer by BattleEffect_ExplosionSheet+0x82a
	.global BattleEffect_MagentaTailSheet
BattleEffect_MagentaTailSheet:
	.incbin "games/tla/src/graphics/battle_effects/magenta_tail.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/magenta_tail.bitmap.lz"
	.global Resource_Data134
Resource_Data134:
	.incbin "games/tla/src/graphics/battle_effects/dot_cells_134.parts.lz"
	.byte 0x59, 0x45	@ PACKER: left in the buffer by BattleEffect_MagentaTailSheet+0x1ba
	.global Resource_Data135
Resource_Data135:
	.incbin "games/tla/src/graphics/battle_effects/dot_cells_b.parts.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleEffect_ParticleSpritesC
BattleEffect_ParticleSpritesC:
	.incbin "games/tla/src/graphics/battle_effects/dot_cells_c.parts.lz"
	.global BattleEffect_ParticleSpritesD
BattleEffect_ParticleSpritesD:
	.incbin "games/tla/src/graphics/battle_effects/dot_cells_d.parts.lz"
	.byte 0x0c	@ PACKER: left in the buffer by BattleEffect_ParticleSpritesC+0x417
	.global BattleEffect_GreenPalette
BattleEffect_GreenPalette:
	.incbin "games/tla/src/graphics/battle_effects/green_palette.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/green_palette.bitmap.lz"
	.global BattleEffect_CrossedDaggersSheet
BattleEffect_CrossedDaggersSheet:
	.incbin "games/tla/src/graphics/battle_effects/crossed_daggers.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/crossed_daggers.bitmap.lz"
	.global BattleEffect_RuneSheet
BattleEffect_RuneSheet:
	.incbin "games/tla/src/graphics/battle_effects/rune.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/rune.bitmap.lz"
	.byte 0xf8, 0x44, 0x14	@ PACKER: left in the buffer by BattleEffect_CrossedDaggersSheet+0x3d9
	.global BattleEffect_FlameColumnSheet
BattleEffect_FlameColumnSheet:
	.incbin "games/tla/src/graphics/battle_effects/flame_column.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/flame_column.bitmap.lz"
	.byte 0x41	@ PACKER: left in the buffer by BattleEffect_RuneSheet+0x3ab
	.global BattleEffect_EarthWallSheet
BattleEffect_EarthWallSheet:
	.incbin "games/tla/src/graphics/battle_effects/earth_wall_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/earth_wall.parts.lz"
	.global BattleEffect_OrangePaletteA
BattleEffect_OrangePaletteA:
	.incbin "games/tla/src/graphics/battle_effects/orange_palette_a.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/orange_palette_a.bitmap.lz"
	.global BattleEffect_FlashBurstSheet
BattleEffect_FlashBurstSheet:
	.incbin "games/tla/src/graphics/battle_effects/flash_burst.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/flash_burst.bitmap.lz"
	.byte 0x60	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x144b
	.global BattleEffect_ThornSheet
BattleEffect_ThornSheet:
	.incbin "games/tla/src/graphics/battle_effects/thorn_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/thorn.parts.lz"
	.byte 0x78	@ PACKER: left in the buffer by BattleEffect_FlashBurstSheet+0xd9b
	.global BattleEffect_CometSheetA
BattleEffect_CometSheetA:
	.incbin "games/tla/src/graphics/battle_effects/comet_a.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/comet_a.bitmap.lz"
	.byte 0x2a	@ PACKER: left in the buffer by BattleEffect_ThornSheet+0x21b
	.section .rodata.d
	.global BattleEffect_CometSheetC
BattleEffect_CometSheetC:
	.incbin "games/tla/src/graphics/battle_effects/comet_c.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/comet_c.bitmap.lz"
	.global BattleEffect_WhirlwindSheet
BattleEffect_WhirlwindSheet:
	.incbin "games/tla/src/graphics/battle_effects/whirlwind_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/whirlwind.parts.lz"
	.global BattleEffect_GreenPillarSheet
BattleEffect_GreenPillarSheet:
	.incbin "games/tla/src/graphics/battle_effects/green_pillar_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/green_pillar.parts.lz"
	.byte 0x21, 0xf0, 0x28	@ PACKER: left in the buffer by BattleEffect_WhirlwindSheet+0x75d
	.global BattleEffect_VineSheet
BattleEffect_VineSheet:
	.incbin "games/tla/src/graphics/battle_effects/vine_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/vine.parts.lz"
	.byte 0xbe	@ PACKER: left in the buffer by BattleEffect_WhirlwindSheet+0xdcb
	.global BattleEffect_FirePillarSheetA
BattleEffect_FirePillarSheetA:
	.incbin "games/tla/src/graphics/battle_effects/fire_pillar_a_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/fire_pillar_a.parts.lz"
	.global BattleEffect_RedPaletteB
BattleEffect_RedPaletteB:
	.incbin "games/tla/src/graphics/battle_effects/red_palette_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/red_palette_b.bitmap.lz"
	.global BattleEffect_OrangePaletteB
BattleEffect_OrangePaletteB:
	.incbin "games/tla/src/graphics/battle_effects/orange_palette_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/orange_palette_b.bitmap.lz"
	.global BattleEffect_EffectPalette02
BattleEffect_EffectPalette02:
	.incbin "games/tla/src/graphics/battle_effects/effect_palette02.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/effect_palette02.bitmap.lz"
	.global BattleEffect_EffectPalette03
BattleEffect_EffectPalette03:
	.incbin "games/tla/src/graphics/battle_effects/effect_palette03.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/effect_palette03.bitmap.lz"
	.global BattleEffect_VioletCometSheet
BattleEffect_VioletCometSheet:
	.incbin "games/tla/src/graphics/battle_effects/violet_comet_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/violet_comet.parts.lz"
	.byte 0x5e, 0xda, 0x33	@ PACKER: left in the buffer by BattleEffect_FirePillarSheetA+0x3e5
	.global BattleEffect_FlameballSheet
BattleEffect_FlameballSheet:
	.incbin "games/tla/src/graphics/battle_effects/flameball.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/flameball.bitmap.lz"
	.byte 0x83	@ PACKER: left in the buffer by BattleEffect_FirePillarSheetA+0x557
	.global BattleEffect_BoulderSheet
BattleEffect_BoulderSheet:
	.incbin "games/tla/src/graphics/battle_effects/boulder_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/boulder.parts.lz"
	.section .rodata.e
	.global BattleEffect_StarDotSheet
BattleEffect_StarDotSheet:
	.incbin "games/tla/src/graphics/battle_effects/star_dot_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/star_dot.parts.lz"
	.byte 0x01, 0xba	@ PACKER: left in the buffer by BattleEffect_RockWallSheet+0x89e
	.global BattleEffect_MarsDjinnSheet
BattleEffect_MarsDjinnSheet:
	.incbin "games/tla/src/graphics/battle_effects/mars_djinn.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/mars_djinn.bitmap.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleEffect_StarDotSheet+0x3db
	.global BattleEffect_MarsDjinnSmallSheet
BattleEffect_MarsDjinnSmallSheet:
	.incbin "games/tla/src/graphics/battle_effects/mars_djinn_small.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/mars_djinn_small.bitmap.lz"
	.byte 0x8a, 0x14, 0x39	@ PACKER: left in the buffer by BattleEffect_MarsDjinnSheet+0x289
	.global BattleEffect_JupiterDjinnSheet
BattleEffect_JupiterDjinnSheet:
	.incbin "games/tla/src/graphics/battle_effects/jupiter_djinn.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/jupiter_djinn.bitmap.lz"
	.byte 0x13	@ PACKER: left in the buffer by BattleEffect_MarsDjinnSheet+0x3a3
	.global BattleEffect_JupiterDjinnSmallSheet
BattleEffect_JupiterDjinnSmallSheet:
	.incbin "games/tla/src/graphics/battle_effects/jupiter_djinn_small.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/jupiter_djinn_small.bitmap.lz"
	.byte 0x0b, 0x1f	@ PACKER: left in the buffer by BattleEffect_JupiterDjinnSheet+0x25a
	.global BattleEffect_MercuryDjinnSheet
BattleEffect_MercuryDjinnSheet:
	.incbin "games/tla/src/graphics/battle_effects/mercury_djinn.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/mercury_djinn.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleEffect_MercuryDjinnSmallSheet
BattleEffect_MercuryDjinnSmallSheet:
	.incbin "games/tla/src/graphics/battle_effects/mercury_djinn_small.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/mercury_djinn_small.bitmap.lz"
	.global BattleEffect_VenusDjinnSheet
BattleEffect_VenusDjinnSheet:
	.incbin "games/tla/src/graphics/battle_effects/venus_djinn.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/venus_djinn.bitmap.lz"
	.global BattleEffect_VenusDjinnSmallSheet
BattleEffect_VenusDjinnSmallSheet:
	.incbin "games/tla/src/graphics/battle_effects/venus_djinn_small.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/venus_djinn_small.bitmap.lz"
	.global BattleEffect_FireBlobSheet
BattleEffect_FireBlobSheet:
	.incbin "games/tla/src/graphics/battle_effects/fire_blob.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/fire_blob.bitmap.lz"
	.byte 0x1a	@ PACKER: left in the buffer by BattleEffect_RockWallSheet+0xb87
	.global BattleEffect_FireStreakSheet
BattleEffect_FireStreakSheet:
	.incbin "games/tla/src/graphics/battle_effects/fire_streak.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/fire_streak.bitmap.lz"
	.byte 0x00, 0x48	@ PACKER: left in the buffer by BattleEffect_WhirlwindSheet+0x15c2
	.global BattleEffect_LavaOrbSheet
BattleEffect_LavaOrbSheet:
	.incbin "games/tla/src/graphics/battle_effects/lava_orb_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/lava_orb.parts.lz"
	.global BattleEffect_YellowRingSheet
BattleEffect_YellowRingSheet:
	.incbin "games/tla/src/graphics/battle_effects/yellow_ring.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/yellow_ring.bitmap.lz"
	.global BattleEffect_YellowOrbSheet
BattleEffect_YellowOrbSheet:
	.incbin "games/tla/src/graphics/battle_effects/yellow_orb.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/yellow_orb.bitmap.lz"
	.byte 0x01, 0x9d	@ PACKER: left in the buffer by BattleEffect_LavaOrbSheet+0xa12
	.global BattleEffect_BubbleSheet
BattleEffect_BubbleSheet:
	.incbin "games/tla/src/graphics/battle_effects/bubble.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/bubble.bitmap.lz"
	.byte 0x14, 0xb1, 0x50	@ PACKER: left in the buffer by BattleEffect_YellowOrbSheet+0x381
	.global BattleEffect_ShieldSheet
BattleEffect_ShieldSheet:
	.incbin "games/tla/src/graphics/battle_effects/shield.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/shield.bitmap.lz"
	.byte 0x45, 0xb1, 0x12	@ PACKER: left in the buffer by BattleEffect_FireStreakSheet+0xcad
	.global BattleEffect_DaggerSheet
BattleEffect_DaggerSheet:
	.incbin "games/tla/src/graphics/battle_effects/dagger.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/dagger.bitmap.lz"
	.byte 0x0a, 0x77, 0x19	@ PACKER: left in the buffer by BattleEffect_ShieldSheet+0xb45
	.global BattleEffect_PaletteRampImage
BattleEffect_PaletteRampImage:
	.incbin "games/tla/src/graphics/battle_effects/spark_cells.parts.lz"
	.byte 0x01, 0x0d	@ PACKER: left in the buffer by BattleEffect_DaggerSheet+0x196
	.global BattleEffect_SmokeSheet
BattleEffect_SmokeSheet:
	.incbin "games/tla/src/graphics/battle_effects/smoke.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/smoke.bitmap.lz"
	.byte 0xc1	@ PACKER: left in the buffer by BattleEffect_DaggerSheet+0x88b
	.global BattleEffect_BluePalette
BattleEffect_BluePalette:
	.incbin "games/tla/src/graphics/battle_effects/blue_palette.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blue_palette.bitmap.lz"
	.global BattleEffect_LimePalette
BattleEffect_LimePalette:
	.incbin "games/tla/src/graphics/battle_effects/lime_palette.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/lime_palette.bitmap.lz"
	.global BattleEffect_PinkPalette
BattleEffect_PinkPalette:
	.incbin "games/tla/src/graphics/battle_effects/pink_palette.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/pink_palette.bitmap.lz"
	.global BattleEffect_VioletPaletteC
BattleEffect_VioletPaletteC:
	.incbin "games/tla/src/graphics/battle_effects/violet_palette_c.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/violet_palette_c.bitmap.lz"
	.global BattleEffect_CyanPalette
BattleEffect_CyanPalette:
	.incbin "games/tla/src/graphics/battle_effects/cyan_palette.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/cyan_palette.bitmap.lz"
	.global BattleEffect_TanPalette
BattleEffect_TanPalette:
	.incbin "games/tla/src/graphics/battle_effects/tan_palette.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/tan_palette.bitmap.lz"
	.global BattleEffect_EffectPalette04
BattleEffect_EffectPalette04:
	.incbin "games/tla/src/graphics/battle_effects/effect_palette04.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/effect_palette04.bitmap.lz"
	.global BattleEffect_EffectPalette05
BattleEffect_EffectPalette05:
	.incbin "games/tla/src/graphics/battle_effects/effect_palette05.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/effect_palette05.bitmap.lz"
	.global BattleEffect_WispSheet
BattleEffect_WispSheet:
	.incbin "games/tla/src/graphics/battle_effects/wisp.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/wisp.bitmap.lz"
	.byte 0x2e, 0x06	@ PACKER: left in the buffer by BattleEffect_SmokeSheet+0x7ae
	.global BattleEffect_GreenVineSheet
BattleEffect_GreenVineSheet:
	.incbin "games/tla/src/graphics/battle_effects/green_vine.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/green_vine.bitmap.lz"
	.byte 0x1c, 0xaf	@ PACKER: left in the buffer by BattleEffect_WispSheet+0x516
	.global BattleEffect_PortalSheet
BattleEffect_PortalSheet:
	.incbin "games/tla/src/graphics/battle_effects/portal_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/portal.parts.lz"
	.byte 0x3f, 0x10	@ PACKER: left in the buffer by BattleEffect_WispSheet+0x622
	.global BattleEffect_SkullSheet
BattleEffect_SkullSheet:
	.incbin "games/tla/src/graphics/battle_effects/skull.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/skull.bitmap.lz"
	.byte 0x6c, 0x09, 0x63	@ PACKER: left in the buffer by BattleEffect_PortalSheet+0x421
	.global BattleEffect_HeartSheet
BattleEffect_HeartSheet:
	.incbin "games/tla/src/graphics/battle_effects/heart.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/heart.bitmap.lz"
	.byte 0x09	@ PACKER: left in the buffer by BattleEffect_SkullSheet+0x297
	.global BattleEffect_CounterRevealSheet
BattleEffect_CounterRevealSheet:
	.incbin "games/tla/src/graphics/battle_effects/counter_reveal_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/counter_reveal.parts.lz"
	.global BattleEffect_RedCrescentSheetA
BattleEffect_RedCrescentSheetA:
	.incbin "games/tla/src/graphics/battle_effects/red_crescent_a_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/red_crescent_a.parts.lz"
	.byte 0x34	@ PACKER: left in the buffer by BattleEffect_CounterRevealSheet+0x49b
	.global BattleEffect_BlueBeastSheet
BattleEffect_BlueBeastSheet:
	.incbin "games/tla/src/graphics/battle_effects/blue_beast_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blue_beast.parts.lz"
	.global BattleEffect_RedCrescentSheetB
BattleEffect_RedCrescentSheetB:
	.incbin "games/tla/src/graphics/battle_effects/red_crescent_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/red_crescent_b.bitmap.lz"
	.byte 0xf0, 0x79	@ PACKER: left in the buffer by BattleEffect_BlueBeastSheet+0x3ea
	.global BattleEffect_SpiralSheet
BattleEffect_SpiralSheet:
	.incbin "games/tla/src/graphics/battle_effects/spiral.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/spiral.bitmap.lz"
	.global BattleEffect_VioletStreakSheet
BattleEffect_VioletStreakSheet:
	.incbin "games/tla/src/graphics/battle_effects/violet_streak.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/violet_streak.bitmap.lz"
	.byte 0x5b	@ PACKER: left in the buffer by BattleEffect_SpiralSheet+0xc8b
	.global BattleEffect_CrescentMoonSheet
BattleEffect_CrescentMoonSheet:
	.incbin "games/tla/src/graphics/battle_effects/crescent_moon.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/crescent_moon.bitmap.lz"
	.byte 0x2b	@ PACKER: left in the buffer by BattleEffect_VioletStreakSheet+0x4cf
	.global BattleEffect_ThornVineSheet
BattleEffect_ThornVineSheet:
	.incbin "games/tla/src/graphics/battle_effects/thorn_vine_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/thorn_vine.parts.lz"
	.byte 0x1e, 0x99, 0x03	@ PACKER: left in the buffer by BattleEffect_VioletStreakSheet+0x691
	.global BattleEffect_IceShardSheet
BattleEffect_IceShardSheet:
	.incbin "games/tla/src/graphics/battle_effects/ice_shard_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/ice_shard.parts.lz"
	.byte 0x3e	@ PACKER: left in the buffer by BattleEffect_VioletStreakSheet+0xa33
	.global BattleEffect_EmberStreakSheet
BattleEffect_EmberStreakSheet:
	.incbin "games/tla/src/graphics/battle_effects/ember_streak_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/ember_streak.parts.lz"
	.global BattleEffect_BlueArcSheetA
BattleEffect_BlueArcSheetA:
	.incbin "games/tla/src/graphics/battle_effects/blue_arc_a.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blue_arc_a.bitmap.lz"
	.byte 0x15, 0xc0, 0x04	@ PACKER: left in the buffer by BattleEffect_EmberStreakSheet+0x7d5
	.global BattleEffect_BlueArcSheetB
BattleEffect_BlueArcSheetB:
	.incbin "games/tla/src/graphics/battle_effects/blue_arc_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blue_arc_b.bitmap.lz"
	.byte 0xaa, 0x30	@ PACKER: left in the buffer by BattleEffect_EmberStreakSheet+0xace
	.global BattleEffect_GlowOrbSheet
BattleEffect_GlowOrbSheet:
	.incbin "games/tla/src/graphics/battle_effects/glow_orb.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/glow_orb.bitmap.lz"
	.byte 0xe0	@ PACKER: left in the buffer by BattleEffect_EmberStreakSheet+0xb3b
	.global BattleEffect_IceChipSheet
BattleEffect_IceChipSheet:
	.incbin "games/tla/src/graphics/battle_effects/ice_chip_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/ice_chip.parts.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleEffect_GlowOrbSheet+0x63f
	.global BattleEffect_IceBlockSheet
BattleEffect_IceBlockSheet:
	.incbin "games/tla/src/graphics/battle_effects/ice_block_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/ice_block.parts.lz"
	.byte 0x30, 0xae	@ PACKER: left in the buffer by BattleEffect_BlueBeastSheet+0x1586
	.global BattleEffect_SparkleDots
BattleEffect_SparkleDots:
	.incbin "games/tla/src/graphics/battle_effects/sparkle_dots.parts.lz"
	.byte 0x63, 0x34, 0x77	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x61
	.global BattleEffect_PinkBurstSheet
BattleEffect_PinkBurstSheet:
	.incbin "games/tla/src/graphics/battle_effects/pink_burst.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/pink_burst.bitmap.lz"
	.byte 0x57	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x36b
	.global BattleEffect_LightFanSheet
BattleEffect_LightFanSheet:
	.incbin "games/tla/src/graphics/battle_effects/light_fan.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/light_fan.bitmap.lz"
	.byte 0x09, 0x82, 0x76	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x59d
@ A 32x32 flare with no palette of its own: Func_0817e0c0 copies it row by
@ row, 32 bytes to a row, and colours it with a palette its argument picks,
@ BattleEffect_BlastSheet's for two of the six values
	.global Resource_Data181
Resource_Data181:
	.incbin "games/tla/src/graphics/battle_effects/sheet_181.bitmap.lz"
	.byte 0xf0	@ PACKER: left in the buffer by BattleEffect_LightFanSheet+0x203
	.global BattleEffect_DustPuffSheet
BattleEffect_DustPuffSheet:
	.incbin "games/tla/src/graphics/battle_effects/dust_puff.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/dust_puff.bitmap.lz"
	.byte 0x7e, 0xe0, 0x98	@ PACKER: left in the buffer by BattleEffect_LightFanSheet+0x339
	.global BattleEffect_GoldShellSheet
BattleEffect_GoldShellSheet:
	.incbin "games/tla/src/graphics/battle_effects/gold_shell_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/gold_shell.parts.lz"
	.byte 0x15, 0xb2	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x1016
	.global BattleEffect_BlastSheet
BattleEffect_BlastSheet:
	.incbin "games/tla/src/graphics/battle_effects/blast_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blast.parts.lz"
	.global BattleEffect_FlameSheetB
BattleEffect_FlameSheetB:
	.incbin "games/tla/src/graphics/battle_effects/flame_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/flame_b.bitmap.lz"
	.global BattleEffect_FirePillarSheetB
BattleEffect_FirePillarSheetB:
	.incbin "games/tla/src/graphics/battle_effects/fire_pillar_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/fire_pillar_b.bitmap.lz"
	.byte 0x0d, 0x03	@ PACKER: left in the buffer by BattleEffect_BlastSheet+0x11fe
	.global BattleEffect_RockSpireSheet
BattleEffect_RockSpireSheet:
	.incbin "games/tla/src/graphics/battle_effects/rock_spire.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/rock_spire.bitmap.lz"
	.byte 0xa0, 0x20, 0x72	@ PACKER: left in the buffer by BattleEffect_FirePillarSheetB+0x7a1
	.global BattleEffect_LightningBoltSheet
BattleEffect_LightningBoltSheet:
	.incbin "games/tla/src/graphics/battle_effects/lightning_bolt.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/lightning_bolt.bitmap.lz"
	.global BattleEffect_BeamImage
BattleEffect_BeamImage:
	.incbin "games/tla/src/graphics/battle_effects/beam_image.parts.lz"
	.byte 0x5f	@ PACKER: left in the buffer by BattleEffect_LightningBoltSheet+0x37b
	.global BattleEffect_TwelveModeImage
BattleEffect_TwelveModeImage:
	.incbin "games/tla/src/graphics/battle_effects/twelve_mode.bitmap.lz"
	.byte 0xa5	@ PACKER: left in the buffer by BattleEffect_LightningBoltSheet+0x42f
	.global BattleEffect_WindStreakSheet
BattleEffect_WindStreakSheet:
	.incbin "games/tla/src/graphics/battle_effects/wind_streak_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/wind_streak.parts.lz"
	.global BattleEffect_YellowPaletteB
BattleEffect_YellowPaletteB:
	.incbin "games/tla/src/graphics/battle_effects/yellow_palette_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/yellow_palette_b.bitmap.lz"
	.global BattleEffect_BlueRingSheet
BattleEffect_BlueRingSheet:
	.incbin "games/tla/src/graphics/battle_effects/blue_ring_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blue_ring.parts.lz"
	.global BattleEffect_RedPaletteC
BattleEffect_RedPaletteC:
	.incbin "games/tla/src/graphics/battle_effects/red_palette_c.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/red_palette_c.bitmap.lz"
	.global BattleEffect_VioletPaletteD
BattleEffect_VioletPaletteD:
	.incbin "games/tla/src/graphics/battle_effects/violet_palette_d.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/violet_palette_d.bitmap.lz"
	.global BattleEffect_BlueBeamSheet
BattleEffect_BlueBeamSheet:
	.incbin "games/tla/src/graphics/battle_effects/blue_beam_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blue_beam.parts.lz"
	.byte 0x01, 0x53, 0x33	@ PACKER: left in the buffer by BattleEffect_WindStreakSheet+0x6b5
	.global BattleEffect_VortexSheet
BattleEffect_VortexSheet:
	.incbin "games/tla/src/graphics/battle_effects/vortex.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/vortex.bitmap.lz"
	.byte 0xdd, 0x20	@ PACKER: left in the buffer by BattleEffect_WindStreakSheet+0x912
	.global BattleEffect_TornadoSheet
BattleEffect_TornadoSheet:
	.incbin "games/tla/src/graphics/battle_effects/tornado_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/tornado.parts.lz"
	.byte 0x7f, 0x02	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x1b32
	.global BattleEffect_CrystalSheet
BattleEffect_CrystalSheet:
	.incbin "games/tla/src/graphics/battle_effects/crystal_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/crystal.parts.lz"
	.byte 0x13, 0x42, 0x03	@ PACKER: left in the buffer by BattleEffect_TornadoSheet+0x104d
	.global BattleEffect_LightningPillarSheet
BattleEffect_LightningPillarSheet:
	.incbin "games/tla/src/graphics/battle_effects/lightning_pillar.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/lightning_pillar.bitmap.lz"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.global BattleEffect_StagedParticlePath
BattleEffect_StagedParticlePath:
	.incbin "games/tla/src/graphics/battle_effects/staged_particle_path.table"
	.global BattleEffect_DualStreamPath
BattleEffect_DualStreamPath:
	.incbin "games/tla/src/graphics/battle_effects/dual_stream_path.table"
