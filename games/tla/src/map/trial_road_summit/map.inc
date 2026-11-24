	.global Field_TrialRoadSummit
Field_TrialRoadSummit:
	map_header 0, 0, 128, 64, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TrialRoadSummit, .LField_TrialRoadSummit_CELL_TILES
	map_block Field_TrialRoadSummit, .LField_TrialRoadSummit_SHAPES
	map_block Field_TrialRoadSummit, .LField_TrialRoadSummit_GRID
	map_block Field_TrialRoadSummit, .LField_TrialRoadSummit_SHAPE_GRID
	map_block Field_TrialRoadSummit, .LField_TrialRoadSummit_ANIMATION
	map_block Field_TrialRoadSummit, .LField_TrialRoadSummit_BLEND
	map_block Field_TrialRoadSummit, .LField_TrialRoadSummit_SCENE
.LField_TrialRoadSummit_CELL_TILES:
	.incbin "games/tla/src/map/trial_road_summit/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_TrialRoadSummit_SHAPES:
	.incbin "games/tla/src/map/trial_road_summit/shapes.table.lz"
	.balign 4, 0
.LField_TrialRoadSummit_GRID:
	.incbin "games/tla/src/map/trial_road_summit/map.grid.lz"
	.balign 4, 0
.LField_TrialRoadSummit_SHAPE_GRID:
	.incbin "games/tla/src/map/trial_road_summit/shape_grid.plane.lz"
	.balign 4, 0
.LField_TrialRoadSummit_ANIMATION:
	.incbin "games/tla/src/map/trial_road_gasp/animation.script.lz"
	.balign 4, 0
.LField_TrialRoadSummit_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TrialRoadSummit_SCENE:
	.incbin "games/tla/src/map/trial_road_summit/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TrialRoadSummitPalette
Tileset_TrialRoadSummitPalette:
	.incbin "games/tla/src/graphics/tileset/trial_road_summit/palette.gbapal.lz"
	.byte 0xd4, 0x99, 0xc0	@ PACKER: left in the buffer by Field_TrialRoad2+0x17d
	.global Tileset_TrialRoadSummitTilesA
Tileset_TrialRoadSummitTilesA:
	.incbin "games/tla/src/graphics/tileset/trial_road_summit/tiles_a.4bpp.mtf"
	.global Tileset_TrialRoadSummitTilesB
Tileset_TrialRoadSummitTilesB:
	.incbin "games/tla/src/graphics/tileset/trial_road_summit/tiles_b.4bpp.mtf"
	.byte 0x46	@ PACKER: left in the buffer by Tileset_TrialRoadGaspTilesA+0x1edb
	.global Tileset_TrialRoadSummitTilesC
Tileset_TrialRoadSummitTilesC:
	.incbin "games/tla/src/graphics/tileset/trial_road_summit/tiles_c.4bpp.mtf"
	.byte 0x0b, 0x24	@ PACKER: left in the buffer by Tileset_TrialRoadSummitTilesB+0x1632
	.global Tileset_TrialRoadSummitTilesD
Tileset_TrialRoadSummitTilesD:
	.incbin "games/tla/src/graphics/tileset/trial_road_summit/tiles_d.4bpp.mtf"
	.byte 0x24	@ PACKER: left in the buffer by Tileset_TrialRoadSummitTilesC+0x13f
