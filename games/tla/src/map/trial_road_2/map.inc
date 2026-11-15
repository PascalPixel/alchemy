	.global Field_TrialRoad2
Field_TrialRoad2:
	map_header 0, 0, 128, 64, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TrialRoad2, .LField_TrialRoad2_CELL_PICTURES
	map_block Field_TrialRoad2, .LField_TrialRoad2_SHAPES
	map_block Field_TrialRoad2, .LField_TrialRoad2_GRID
	map_block Field_TrialRoad2, .LField_TrialRoad2_SHAPE_GRID
	map_block Field_TrialRoad2, .LField_TrialRoad2_ANIMATION
	map_block Field_TrialRoad2, .LField_TrialRoad2_BLEND
	map_block Field_TrialRoad2, .LField_TrialRoad2_SCENE
.LField_TrialRoad2_CELL_PICTURES:
	.incbin "games/tla/src/map/trial_road_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TrialRoad2_SHAPES:
	.incbin "games/tla/src/map/trial_road_2/shapes.table.lz"
	.balign 4, 0
.LField_TrialRoad2_GRID:
	.incbin "games/tla/src/map/trial_road_2/map.grid.lz"
	.balign 4, 0
.LField_TrialRoad2_SHAPE_GRID:
	.incbin "games/tla/src/map/trial_road_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_TrialRoad2_ANIMATION:
	.incbin "games/tla/src/map/trial_road_gasp/animation.script.lz"
	.balign 4, 0
.LField_TrialRoad2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TrialRoad2_SCENE:
	.incbin "games/tla/src/map/trial_road_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TrialRoad2Palette
Tileset_TrialRoad2Palette:
	.incbin "games/tla/src/graphics/tileset/trial_road_2/palette.gbapal.lz"
	.byte 0xd4, 0x99, 0xc0	@ PACKER: left in the buffer by Field_TrialRoad2+0x17d
	.global Tileset_TrialRoad2TilesA
Tileset_TrialRoad2TilesA:
	.incbin "games/tla/src/graphics/tileset/trial_road_2/tiles_a.4bpp.mtf"
	.global Tileset_TrialRoad2TilesB
Tileset_TrialRoad2TilesB:
	.incbin "games/tla/src/graphics/tileset/trial_road_2/tiles_b.4bpp.mtf"
	.byte 0x46	@ PACKER: left in the buffer by Tileset_TrialRoadGaspTilesA+0x1edb
	.global Tileset_TrialRoad2TilesC
Tileset_TrialRoad2TilesC:
	.incbin "games/tla/src/graphics/tileset/trial_road_2/tiles_c.4bpp.mtf"
	.byte 0x0b, 0x24	@ PACKER: left in the buffer by Tileset_TrialRoad2TilesB+0x1632
	.global Tileset_TrialRoad2TilesD
Tileset_TrialRoad2TilesD:
	.incbin "games/tla/src/graphics/tileset/trial_road_2/tiles_d.4bpp.mtf"
	.byte 0x24	@ PACKER: left in the buffer by Tileset_TrialRoad2TilesC+0x13f
