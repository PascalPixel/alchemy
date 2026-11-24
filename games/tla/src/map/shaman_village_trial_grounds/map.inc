	.global Field_ShamanVillageTrialGrounds
Field_ShamanVillageTrialGrounds:
	map_header 0, 0, 64, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShamanVillageTrialGrounds, .LField_ShamanVillageTrialGrounds_CELL_TILES
	map_block Field_ShamanVillageTrialGrounds, .LField_ShamanVillageTrialGrounds_SHAPES
	map_block Field_ShamanVillageTrialGrounds, .LField_ShamanVillageTrialGrounds_GRID
	map_block Field_ShamanVillageTrialGrounds, .LField_ShamanVillageTrialGrounds_SHAPE_GRID
	map_block Field_ShamanVillageTrialGrounds, .LField_ShamanVillageTrialGrounds_ANIMATION
	map_block Field_ShamanVillageTrialGrounds, .LField_ShamanVillageTrialGrounds_BLEND
	map_block Field_ShamanVillageTrialGrounds, .LField_ShamanVillageTrialGrounds_SCENE
.LField_ShamanVillageTrialGrounds_CELL_TILES:
	.incbin "games/tla/src/map/shaman_village_trial_grounds/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ShamanVillageTrialGrounds_SHAPES:
	.incbin "games/tla/src/map/shaman_village_trial_grounds/shapes.table.lz"
	.balign 4, 0
.LField_ShamanVillageTrialGrounds_GRID:
	.incbin "games/tla/src/map/shaman_village_trial_grounds/map.grid.lz"
	.balign 4, 0
.LField_ShamanVillageTrialGrounds_SHAPE_GRID:
	.incbin "games/tla/src/map/shaman_village_trial_grounds/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShamanVillageTrialGrounds_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_ShamanVillageTrialGrounds_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShamanVillageTrialGrounds_SCENE:
	.incbin "games/tla/src/map/shaman_village_trial_grounds/scene.scene.lz"
	.global Tileset_ShamanVillageTrialGroundsPalette
Tileset_ShamanVillageTrialGroundsPalette:
	.incbin "games/tla/src/graphics/tileset/shaman_village_trial_grounds/palette.gbapal.lz"
	.byte 0x92, 0x44, 0x80	@ PACKER: left in the buffer by Field_ShamanVillageTrialGrounds+0x135
	.global Tileset_ShamanVillageTrialGroundsTilesA
Tileset_ShamanVillageTrialGroundsTilesA:
	.incbin "games/tla/src/graphics/tileset/shaman_village_trial_grounds/tiles_a.4bpp.mtf"
	.global Tileset_ShamanVillageTrialGroundsTilesB
Tileset_ShamanVillageTrialGroundsTilesB:
	.incbin "games/tla/src/graphics/tileset/shaman_village_trial_grounds/tiles_b.4bpp.mtf"
	.byte 0xf2, 0x8b, 0xdd	@ PACKER: left in the buffer by Tileset_ShamanVillageTrialGroundsTilesA+0x1365
	.global Tileset_ShamanVillageTrialGroundsTilesC
Tileset_ShamanVillageTrialGroundsTilesC:
	.incbin "games/tla/src/graphics/tileset/shaman_village_trial_grounds/tiles_c.4bpp.mtf"
	.byte 0x16	@ PACKER: left in the buffer by Tileset_ShamanVillageTrialGroundsTilesB+0xc87
	.global Tileset_ShamanVillageTrialGroundsTilesD
Tileset_ShamanVillageTrialGroundsTilesD:
	.incbin "games/tla/src/graphics/tileset/shaman_village_trial_grounds/tiles_d.4bpp.mtf"
	.byte 0x0e	@ PACKER: left in the buffer by Tileset_ShamanVillageTrialGroundsTilesC+0x13f
