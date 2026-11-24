	.global Field_TrialRoadGasp
Field_TrialRoadGasp:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TrialRoadGasp, .LField_TrialRoadGasp_CELL_TILES
	map_block Field_TrialRoadGasp, .LField_TrialRoadGasp_SHAPES
	map_block Field_TrialRoadGasp, .LField_TrialRoadGasp_GRID
	map_block Field_TrialRoadGasp, .LField_TrialRoadGasp_SHAPE_GRID
	map_block Field_TrialRoadGasp, .LField_TrialRoadGasp_ANIMATION
	map_block Field_TrialRoadGasp, .LField_TrialRoadGasp_BLEND
	map_block Field_TrialRoadGasp, .LField_TrialRoadGasp_SCENE
.LField_TrialRoadGasp_CELL_TILES:
	.incbin "games/tla/src/map/trial_road_gasp/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_TrialRoadGasp_SHAPES:
	.incbin "games/tla/src/map/trial_road_gasp/shapes.table.lz"
	.balign 4, 0
.LField_TrialRoadGasp_GRID:
	.incbin "games/tla/src/map/trial_road_gasp/map.grid.lz"
	.balign 4, 0
.LField_TrialRoadGasp_SHAPE_GRID:
	.incbin "games/tla/src/map/trial_road_gasp/shape_grid.plane.lz"
	.balign 4, 0
.LField_TrialRoadGasp_ANIMATION:
	.incbin "games/tla/src/map/trial_road_gasp/animation.script.lz"
	.balign 4, 0
.LField_TrialRoadGasp_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TrialRoadGasp_SCENE:
	.incbin "games/tla/src/map/trial_road_gasp/scene.scene.lz"
	.global Tileset_TrialRoadGaspPalette
Tileset_TrialRoadGaspPalette:
	.incbin "games/tla/src/graphics/tileset/trial_road_gasp/palette.gbapal.lz"
	.global Tileset_TrialRoadGaspTilesA
Tileset_TrialRoadGaspTilesA:
	.incbin "games/tla/src/graphics/tileset/trial_road_gasp/tiles_a.4bpp.mtf"
	.byte 0x5b	@ PACKER: left in the buffer by Tileset_TrialRoad1TilesA+0x1fd7
	.global Tileset_TrialRoadGaspTilesB
Tileset_TrialRoadGaspTilesB:
	.incbin "games/tla/src/graphics/tileset/trial_road_gasp/tiles_b.4bpp.mtf"
	.byte 0x16	@ PACKER: left in the buffer by Tileset_TrialRoadGaspTilesA+0x107f
	.global Tileset_TrialRoadGaspTilesC
Tileset_TrialRoadGaspTilesC:
	.incbin "games/tla/src/graphics/tileset/trial_road_gasp/tiles_c.4bpp.mtf"
	.byte 0x10, 0x00, 0x00	@ PACKER: unused post-stream residue; Decompress_MoveToFrontLz uses decoded RAM and output length; writer unknown
	.global Tileset_TrialRoadGaspTilesD
Tileset_TrialRoadGaspTilesD:
	.incbin "games/tla/src/graphics/tileset/trial_road_gasp/tiles_d.4bpp.mtf"
	.byte 0x02, 0x09	@ PACKER: left in the buffer by Tileset_TrialRoadGaspTilesC+0xb6e
