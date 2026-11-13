	.section .rodata
	.global Battle_PurpleMoonBackdrop
Battle_PurpleMoonBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/purple_moon.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/purple_moon.bitmap.d7"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Battle_SuhallaGateBackdrop
Battle_SuhallaGateBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/suhalla_gate.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/suhalla_gate.bitmap.d7"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Battle_PirateShipBackdrop
Battle_PirateShipBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/pirate_ship.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/pirate_ship.bitmap.d7"
	.global Battle_CrossboneIsleCaveBackdrop
Battle_CrossboneIsleCaveBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/crossbone_isle_cave.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/crossbone_isle_cave.bitmap.d7"
	.global Battle_ColossoBackdrop
Battle_ColossoBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/colosso.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/colosso.bitmap.d7"
	.global Battle_VenusLighthouseAerieStormBackdrop
Battle_VenusLighthouseAerieStormBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/venus_lighthouse_aerie_storm.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/venus_lighthouse_aerie_storm.bitmap.d7"
	.byte 0x92, 0x11	@ PACKER: left in the buffer by Battle_ColossoBackdrop+0x3d3a
	.global Battle_VenusLighthouseAerieBackdrop
Battle_VenusLighthouseAerieBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/venus_lighthouse_aerie.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/venus_lighthouse_aerie.bitmap.d7"
	.byte 0x34, 0x54	@ PACKER: left in the buffer by Battle_VenusLighthouseAerieStormBackdrop+0x35ee
	.global Battle_VenusLighthouseBackdrop
Battle_VenusLighthouseBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/venus_lighthouse.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/venus_lighthouse.bitmap.d7"
	.global Battle_SuhallaDesertBackdrop
Battle_SuhallaDesertBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/suhalla_desert.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/suhalla_desert.bitmap.d7"
	.global Battle_AltmillerCaveBackdrop
Battle_AltmillerCaveBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/altmiller_cave.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/altmiller_cave.bitmap.d7"
	.byte 0x41, 0x40	@ PACKER: left in the buffer by Battle_ColossoBackdrop+0x40d6
	.global Battle_TunnelRuinsBackdrop
Battle_TunnelRuinsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/tunnel_ruins.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/tunnel_ruins.bitmap.d7"
	.global Battle_BabiLighthouseBackdrop
Battle_BabiLighthouseBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/babi_lighthouse.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/babi_lighthouse.bitmap.d7"
	.global Battle_LamakanDesertBackdrop
Battle_LamakanDesertBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/lamakan_desert.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/lamakan_desert.bitmap.d7"
	.global Battle_MogallForestBackdrop
Battle_MogallForestBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/mogall_forest.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/mogall_forest.bitmap.d7"
	.byte 0x50, 0x00	@ PACKER: left in the buffer by Battle_BabiLighthouseBackdrop+0x4326
	.global Battle_MercuryLighthouseAerieBackdrop
Battle_MercuryLighthouseAerieBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/mercury_lighthouse_aerie.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/mercury_lighthouse_aerie.bitmap.d7"
	.global Battle_TolbiShipInteriorsBackdrop
Battle_TolbiShipInteriorsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/tolbi_ship_interiors.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/tolbi_ship_interiors.bitmap.d7"
	.byte 0x39, 0x66	@ PACKER: left in the buffer by Battle_MogallForestBackdrop+0x4076
	.global Battle_ImilFuchinFallsBackdrop
Battle_ImilFuchinFallsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/imil_fuchin_falls.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/imil_fuchin_falls.bitmap.d7"
	.byte 0x45, 0x15	@ PACKER: left in the buffer by Battle_TolbiShipInteriorsBackdrop+0x383e
	.global Battle_KolimaForestBackdrop
Battle_KolimaForestBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/kolima_forest.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/kolima_forest.bitmap.d7"
	.global Battle_TretTreeInteriorsBackdrop
Battle_TretTreeInteriorsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/tret_tree_interiors.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/tret_tree_interiors.bitmap.d7"
	.byte 0x58, 0xe7	@ PACKER: left in the buffer by Battle_KolimaForestBackdrop+0x49fa
	.global Battle_InteriorBackdrop
Battle_InteriorBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/interior.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/interior.bitmap.d7"
	.global Battle_AltinPeakBackdrop
Battle_AltinPeakBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/altin_peak.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/altin_peak.bitmap.d7"
	.byte 0x13, 0x24	@ PACKER: left in the buffer by Battle_TretTreeInteriorsBackdrop+0x3b4a
	.global Battle_TolbiShipDeckBackdrop
Battle_TolbiShipDeckBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/tolbi_ship_deck.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/tolbi_ship_deck.bitmap.d7"
	.global Battle_MercuryLighthouseBackdrop
Battle_MercuryLighthouseBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/mercury_lighthouse.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/mercury_lighthouse.bitmap.d7"
	.byte 0xb8, 0xf6	@ PACKER: left in the buffer by Battle_TretTreeInteriorsBackdrop+0x4032
	.global Battle_ValeStormBackdrop
Battle_ValeStormBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/vale_storm.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/vale_storm.bitmap.d7"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.global Battle_TretTreeBranchBackdrop
Battle_TretTreeBranchBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/tret_tree_branch.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/tret_tree_branch.bitmap.d7"
	.byte 0xfa, 0x8e	@ PACKER: left in the buffer by Battle_ValeStormBackdrop+0x42b2
	.global Battle_BeachBackdrop
Battle_BeachBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/beach.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/beach.bitmap.d7"
	.byte 0x10, 0xb5	@ PACKER: left in the buffer by Battle_TretTreeBranchBackdrop+0x397e
	.global Battle_SnowfieldBackdrop
Battle_SnowfieldBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/snowfield.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/snowfield.bitmap.d7"
	.global Battle_GrasslandBackdrop
Battle_GrasslandBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/grassland.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/grassland.bitmap.d7"
	.global Battle_SolSanctumBackdrop
Battle_SolSanctumBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/sol_sanctum.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/sol_sanctum.bitmap.d7"
	.byte 0x10, 0x15	@ PACKER: left in the buffer by Battle_GrasslandBackdrop+0x42b6
	.global Battle_ForestBackdrop
Battle_ForestBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/forest.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/forest.bitmap.d7"
	.global Battle_VioletSkyBackdrop
Battle_VioletSkyBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/violet_sky.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/violet_sky.bitmap.d7"
	.global Battle_GrasslandBBackdrop
Battle_GrasslandBBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/grassland_b.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/grassland_b.bitmap.d7"
	.byte 0x3e, 0x77	@ PACKER: left in the buffer by Battle_ForestBackdrop+0x3b9e
	.global Battle_RockyBeachBackdrop
Battle_RockyBeachBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/rocky_beach.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/rocky_beach.bitmap.d7"
	.byte 0x20, 0x03	@ PACKER: left in the buffer by Battle_GrasslandBBackdrop+0x33ce
	.global Battle_DenseForestBackdrop
Battle_DenseForestBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/dense_forest.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/dense_forest.bitmap.d7"
	.global Battle_FireCavePoolBackdrop
Battle_FireCavePoolBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/fire_cave_pool.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/fire_cave_pool.bitmap.d7"
	.global Battle_CanyonMeadowBackdrop
Battle_CanyonMeadowBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/canyon_meadow.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/canyon_meadow.bitmap.d7"
	.global Battle_BlueCaveBackdrop
Battle_BlueCaveBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/blue_cave.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/blue_cave.bitmap.d7"
	.byte 0x17, 0x0c	@ PACKER: left in the buffer by Battle_DenseForestBackdrop+0x4666
	.global Battle_RedMesaBackdrop
Battle_RedMesaBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/red_mesa.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/red_mesa.bitmap.d7"
	.byte 0xed, 0xc5	@ PACKER: left in the buffer by Battle_BlueCaveBackdrop+0x3a66
	.global Battle_PinkRocksBackdrop
Battle_PinkRocksBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/pink_rocks.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/pink_rocks.bitmap.d7"
	.byte 0x45, 0x40	@ PACKER: left in the buffer by Battle_RedMesaBackdrop+0x39ee
	.global Battle_PinkTotemBackdrop
Battle_PinkTotemBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/pink_totem.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/pink_totem.bitmap.d7"
	.global Battle_StoneTownBackdrop
Battle_StoneTownBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/stone_town.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/stone_town.bitmap.d7"
	.global Battle_PalmCliffBackdrop
Battle_PalmCliffBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/palm_cliff.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/palm_cliff.bitmap.d7"
	.global Battle_ToyRoomBackdrop
Battle_ToyRoomBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/toy_room.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/toy_room.bitmap.d7"
	.byte 0x15, 0xdd	@ PACKER: left in the buffer by Battle_PalmCliffBackdrop+0x394a
	.global Battle_ShipCabinBackdrop
Battle_ShipCabinBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/ship_cabin.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/ship_cabin.bitmap.d7"
	.byte 0x03, 0x5c	@ PACKER: left in the buffer by Battle_PalmCliffBackdrop+0x3f2e
	.global Battle_ShipDeckBackdrop
Battle_ShipDeckBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/ship_deck.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/ship_deck.bitmap.d7"
	.byte 0x9d, 0x41	@ PACKER: left in the buffer by Battle_ShipCabinBackdrop+0x3a22
	.global Battle_GoldRocksBackdrop
Battle_GoldRocksBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/gold_rocks.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/gold_rocks.bitmap.d7"
	.section .rodata.a
	.global Battle_FaceTempleBackdrop
Battle_FaceTempleBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/face_temple.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/face_temple.bitmap.d7"
	.global Battle_MistyHillsBackdrop
Battle_MistyHillsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/misty_hills.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/misty_hills.bitmap.d7"
	.byte 0xd7, 0x34	@ PACKER: left in the buffer by Battle_FaceTempleBackdrop+0x3f86
	.global Battle_LavaCaveBackdrop
Battle_LavaCaveBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/lava_cave.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/lava_cave.bitmap.d7"
	.global Battle_WaterfallGorgeBackdrop
Battle_WaterfallGorgeBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/waterfall_gorge.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/waterfall_gorge.bitmap.d7"
	.global Battle_MossCaveBackdrop
Battle_MossCaveBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/moss_cave.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/moss_cave.bitmap.d7"
	.byte 0x33, 0xd8	@ PACKER: left in the buffer by Battle_LavaCaveBackdrop+0x47c6
	.global Battle_BlueRuinsBackdrop
Battle_BlueRuinsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/blue_ruins.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/blue_ruins.bitmap.d7"
	.byte 0xf4, 0xa1	@ PACKER: left in the buffer by Battle_MossCaveBackdrop+0x3eba
	.global Battle_GreenRuinsBackdrop
Battle_GreenRuinsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/green_ruins.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/green_ruins.bitmap.d7"
	.byte 0x11, 0x10	@ PACKER: left in the buffer by Battle_BlueRuinsBackdrop+0x3dfa
	.global Battle_StatueCliffBackdrop
Battle_StatueCliffBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/statue_cliff.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/statue_cliff.bitmap.d7"
	.byte 0x03, 0x51	@ PACKER: left in the buffer by Battle_MossCaveBackdrop+0x44ce
	.global Battle_LavaTempleBackdrop
Battle_LavaTempleBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/lava_temple.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/lava_temple.bitmap.d7"
	.global Battle_ArrowBeachBackdrop
Battle_ArrowBeachBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/arrow_beach.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/arrow_beach.bitmap.d7"
	.byte 0x55, 0x24	@ PACKER: left in the buffer by Battle_LavaTempleBackdrop+0x375a
	.global Battle_StormySeaBackdrop
Battle_StormySeaBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/stormy_sea.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/stormy_sea.bitmap.d7"
	.byte 0x54, 0x28	@ PACKER: left in the buffer by Battle_MossCaveBackdrop+0x462a
	.global Battle_AngelHallBackdrop
Battle_AngelHallBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/angel_hall.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/angel_hall.bitmap.d7"
	.global Battle_PurpleLighthouseBackdrop
Battle_PurpleLighthouseBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/purple_lighthouse.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/purple_lighthouse.bitmap.d7"
	.byte 0xbe, 0x55	@ PACKER: left in the buffer by Battle_AngelHallBackdrop+0x3cb2
	.global Battle_StormyLighthouseBackdrop
Battle_StormyLighthouseBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/stormy_lighthouse.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/stormy_lighthouse.bitmap.d7"
	.byte 0x4a, 0xb4	@ PACKER: left in the buffer by Battle_PurpleLighthouseBackdrop+0x3bfa
	.global Battle_SerpentStarsBackdrop
Battle_SerpentStarsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/serpent_stars.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/serpent_stars.bitmap.d7"
	.byte 0x96, 0xf4	@ PACKER: left in the buffer by Battle_StormyLighthouseBackdrop+0x39ae
	.global Battle_IceHallBackdrop
Battle_IceHallBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/ice_hall.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/ice_hall.bitmap.d7"
	.byte 0xe8, 0xaa	@ PACKER: left in the buffer by Battle_StormySeaBackdrop+0x444e
	.global Battle_OrbAltarBackdrop
Battle_OrbAltarBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/orb_altar.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/orb_altar.bitmap.d7"
	.byte 0x11, 0x5d	@ PACKER: left in the buffer by Battle_IceHallBackdrop+0x3bf6
	.global Battle_PurpleCaveBackdrop
Battle_PurpleCaveBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/purple_cave.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/purple_cave.bitmap.d7"
	.global Battle_BeastWallBackdrop
Battle_BeastWallBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/beast_wall.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/beast_wall.bitmap.d7"
	.global Battle_DragonHallABackdrop
Battle_DragonHallABackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/dragon_hall_a.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/dragon_hall_a.bitmap.d7"
	.byte 0x05, 0x41	@ PACKER: left in the buffer by Battle_BeastWallBackdrop+0x3d8e
	.global Battle_DragonHallBBackdrop
Battle_DragonHallBBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/dragon_hall_b.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/dragon_hall_b.bitmap.d7"
	.byte 0x44, 0xf4	@ PACKER: left in the buffer by Battle_DragonHallABackdrop+0x3c4e
	.global Battle_GorgePoolBackdrop
Battle_GorgePoolBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/gorge_pool.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/gorge_pool.bitmap.d7"
	.global Battle_SpikeHallBackdrop
Battle_SpikeHallBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/spike_hall.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/spike_hall.bitmap.d7"
	.byte 0x1c, 0x4d	@ PACKER: left in the buffer by Battle_GorgePoolBackdrop+0x428a
	.global Battle_SnowfieldBBackdrop
Battle_SnowfieldBBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/snowfield_b.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/snowfield_b.bitmap.d7"
	.global Battle_BlizzardBackdrop
Battle_BlizzardBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/blizzard.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/blizzard.bitmap.d7"
	.global Battle_SnowfieldCBackdrop
Battle_SnowfieldCBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/snowfield_c.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/snowfield_c.bitmap.d7"
	.byte 0x15, 0x54	@ PACKER: left in the buffer by Battle_IceHallBackdrop+0x439a
	.global Battle_SerpentStarsBBackdrop
Battle_SerpentStarsBBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/serpent_stars_b.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/serpent_stars_b.bitmap.d7"
	.global Battle_SunbeamCloudsBackdrop
Battle_SunbeamCloudsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/sunbeam_clouds.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/sunbeam_clouds.bitmap.d7"
	.byte 0x00, 0x00	@ PACKER: left in the buffer by Battle_SerpentStarsBBackdrop+0x2f5a
	.global Battle_RedHazeBackdrop
Battle_RedHazeBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/red_haze.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/red_haze.bitmap.d7"
	.global Battle_DesertBackdrop
Battle_DesertBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/desert.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/desert.bitmap.d7"
	.global Battle_DuskCloudsBackdrop
Battle_DuskCloudsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/dusk_clouds.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/dusk_clouds.bitmap.d7"
	.byte 0xc2, 0x4a	@ PACKER: left in the buffer by Battle_SerpentStarsBBackdrop+0x39aa
	.global Battle_AerialViewBackdrop
Battle_AerialViewBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/aerial_view.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/aerial_view.bitmap.d7"
	.global Battle_SunsetBackdrop
Battle_SunsetBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/sunset.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/sunset.bitmap.d7"
	.byte 0x3b, 0x47	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x28a2
	.global Battle_StarfieldFlareBackdrop
Battle_StarfieldFlareBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/starfield_flare.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/starfield_flare.bitmap.d7"
	.global Battle_EarthHorizonBackdrop
Battle_EarthHorizonBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/earth_horizon.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/earth_horizon.bitmap.d7"
