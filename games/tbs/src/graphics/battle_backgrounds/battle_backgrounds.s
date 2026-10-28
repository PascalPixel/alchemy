	.section .rodata
	.global Battle_SuhallaGateBackdrop
Battle_SuhallaGateBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/suhalla_gate.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/suhalla_gate.bitmap.d7"
	.byte 0x41, 0x83	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x4442
	.global Battle_PirateShipBackdrop
Battle_PirateShipBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/pirate_ship.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/pirate_ship.bitmap.d7"
	.global Battle_CrossboneIsleCaveBackdrop
Battle_CrossboneIsleCaveBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/crossbone_isle_cave.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/crossbone_isle_cave.bitmap.d7"
	.global Battle_ColossoBackdrop
Battle_ColossoBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/colosso.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/colosso.bitmap.d7"
	.global Battle_VenusLighthouseAerieStormBackdrop
Battle_VenusLighthouseAerieStormBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/venus_lighthouse_aerie_storm.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/venus_lighthouse_aerie_storm.bitmap.d7"
	.byte 0x92, 0x11	@ PACKER: left in the buffer by Battle_ColossoBackdrop+0x3d3a
	.global Battle_VenusLighthouseAerieBackdrop
Battle_VenusLighthouseAerieBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/venus_lighthouse_aerie.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/venus_lighthouse_aerie.bitmap.d7"
	.byte 0x34, 0x54	@ PACKER: left in the buffer by Battle_VenusLighthouseAerieStormBackdrop+0x35ee
	.global Battle_VenusLighthouseBackdrop
Battle_VenusLighthouseBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/venus_lighthouse.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/venus_lighthouse.bitmap.d7"
	.global Battle_SuhallaDesertBackdrop
Battle_SuhallaDesertBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/suhalla_desert.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/suhalla_desert.bitmap.d7"
	.global Battle_AltmillerCaveBackdrop
Battle_AltmillerCaveBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/altmiller_cave.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/altmiller_cave.bitmap.d7"
	.byte 0x41, 0x40	@ PACKER: left in the buffer by Battle_ColossoBackdrop+0x40d6
	.global Battle_TunnelRuinsBackdrop
Battle_TunnelRuinsBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/tunnel_ruins.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/tunnel_ruins.bitmap.d7"
	.global Battle_BabiLighthouseBackdrop
Battle_BabiLighthouseBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/babi_lighthouse.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/babi_lighthouse.bitmap.d7"
	.global Battle_LamakanDesertBackdrop
Battle_LamakanDesertBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/lamakan_desert.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/lamakan_desert.bitmap.d7"
	.global Battle_MogallForestBackdrop
Battle_MogallForestBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/mogall_forest.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/mogall_forest.bitmap.d7"
	.byte 0x50, 0x00	@ PACKER: left in the buffer by Battle_BabiLighthouseBackdrop+0x4326
	.global Battle_MercuryLighthouseAerieBackdrop
Battle_MercuryLighthouseAerieBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/mercury_lighthouse_aerie.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/mercury_lighthouse_aerie.bitmap.d7"
	.global Battle_TolbiShipInteriorsBackdrop
Battle_TolbiShipInteriorsBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/tolbi_ship_interiors.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/tolbi_ship_interiors.bitmap.d7"
	.byte 0x39, 0x66	@ PACKER: left in the buffer by Battle_MogallForestBackdrop+0x4076
	.global Battle_ImilFuchinFallsBackdrop
Battle_ImilFuchinFallsBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/imil_fuchin_falls.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/imil_fuchin_falls.bitmap.d7"
	.byte 0x45, 0x15	@ PACKER: left in the buffer by Battle_TolbiShipInteriorsBackdrop+0x383e
	.global Battle_KolimaForestBackdrop
Battle_KolimaForestBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/kolima_forest.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/kolima_forest.bitmap.d7"
	.global Battle_TretTreeInteriorsBackdrop
Battle_TretTreeInteriorsBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/tret_tree_interiors.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/tret_tree_interiors.bitmap.d7"
	.byte 0x58, 0xe7	@ PACKER: left in the buffer by Battle_KolimaForestBackdrop+0x49fa
	.global Battle_GomaCaveBackdrop
Battle_GomaCaveBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/goma_cave.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/goma_cave.bitmap.d7"
	.global Battle_InteriorBackdrop
Battle_InteriorBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/interior.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/interior.bitmap.d7"
	.global Battle_AltinPeakBackdrop
Battle_AltinPeakBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/altin_peak.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/altin_peak.bitmap.d7"
	.byte 0x54, 0x14	@ PACKER: left in the buffer by Battle_GomaCaveBackdrop+0x3b4a
	.global Battle_TolbiShipDeckBackdrop
Battle_TolbiShipDeckBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/tolbi_ship_deck.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/tolbi_ship_deck.bitmap.d7"
	.global Battle_MercuryLighthouseBackdrop
Battle_MercuryLighthouseBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/mercury_lighthouse.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/mercury_lighthouse.bitmap.d7"
	.byte 0x5d, 0xf5	@ PACKER: left in the buffer by Battle_GomaCaveBackdrop+0x4032
	.balign 4, 0
	.global Battle_ValeStormBackdrop
Battle_ValeStormBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/vale_storm.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/vale_storm.bitmap.d7"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.global Battle_TretTreeBranchBackdrop
Battle_TretTreeBranchBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/tret_tree_branch.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/tret_tree_branch.bitmap.d7"
	.byte 0xfa, 0x8e	@ PACKER: left in the buffer by Battle_ValeStormBackdrop+0x42b2
	.global Battle_BeachBackdrop
Battle_BeachBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/beach.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/beach.bitmap.d7"
	.byte 0x10, 0xb5	@ PACKER: left in the buffer by Battle_TretTreeBranchBackdrop+0x397e
	.global Battle_SnowfieldBackdrop
Battle_SnowfieldBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/snowfield.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/snowfield.bitmap.d7"
	.global Battle_GrasslandBackdrop
Battle_GrasslandBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/grassland.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/grassland.bitmap.d7"
	.global Battle_SolSanctumBackdrop
Battle_SolSanctumBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/sol_sanctum.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/sol_sanctum.bitmap.d7"
	.byte 0x10, 0x15	@ PACKER: left in the buffer by Battle_GrasslandBackdrop+0x42b6
	.global Battle_ForestBackdrop
Battle_ForestBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/forest.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/forest.bitmap.d7"
	.global Battle_VioletSkyBackdrop
Battle_VioletSkyBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/violet_sky.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/violet_sky.bitmap.d7"
	.global Battle_DesertBackdrop
Battle_DesertBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/desert.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/desert.bitmap.d7"
	.global Battle_DuskCloudsBackdrop
Battle_DuskCloudsBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/dusk_clouds.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/dusk_clouds.bitmap.d7"
	.byte 0x33, 0x00	@ PACKER: left in the buffer by Battle_ForestBackdrop+0x39aa
	.global Battle_AerialViewBackdrop
Battle_AerialViewBackdrop:
	.incbin "games/tbs/src/graphics/battle_backgrounds/aerial_view.gbapal"
	.incbin "games/tbs/src/graphics/battle_backgrounds/aerial_view.bitmap.d7"
