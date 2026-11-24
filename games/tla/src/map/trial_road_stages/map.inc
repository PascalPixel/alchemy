	.global Field_TrialRoadStages
Field_TrialRoadStages:
	map_header 0, 0, 84, 240, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 84, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 168, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TrialRoadStages, .LField_TrialRoadStages_CELL_TILES
	map_block Field_TrialRoadStages, .LField_TrialRoadStages_SHAPES
	map_block Field_TrialRoadStages, .LField_TrialRoadStages_GRID
	map_block Field_TrialRoadStages, .LField_TrialRoadStages_SHAPE_GRID
	map_block Field_TrialRoadStages, .LField_TrialRoadStages_ANIMATION
	map_block Field_TrialRoadStages, .LField_TrialRoadStages_BLEND
	map_block Field_TrialRoadStages, .LField_TrialRoadStages_SCENE
.LField_TrialRoadStages_CELL_TILES:
	.incbin "games/tla/src/map/trial_road_stages/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_TrialRoadStages_SHAPES:
	.incbin "games/tla/src/map/trial_road_stages/shapes.table.lz"
	.balign 4, 0
.LField_TrialRoadStages_GRID:
	.incbin "games/tla/src/map/trial_road_stages/map.grid.lz"
	.balign 4, 0
.LField_TrialRoadStages_SHAPE_GRID:
	.incbin "games/tla/src/map/trial_road_stages/shape_grid.plane.lz"
	.balign 4, 0
.LField_TrialRoadStages_ANIMATION:
	.incbin "games/tla/src/map/trial_road_stages/animation.script.lz"
	.balign 4, 0
.LField_TrialRoadStages_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TrialRoadStages_SCENE:
	.incbin "games/tla/src/map/trial_road_stages/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TrialRoad1Palette
Tileset_TrialRoad1Palette:
	.incbin "games/tla/src/graphics/tileset/trial_road_stages/palette.gbapal.lz"
	.byte 0xfa	@ PACKER: left in the buffer by Field_TrialRoadStages+0x13f
	.global Tileset_TrialRoad1TilesA
Tileset_TrialRoad1TilesA:
	.incbin "games/tla/src/graphics/tileset/trial_road_stages/tiles_a.4bpp.mtf"
	.global Tileset_TrialRoad1TilesB
Tileset_TrialRoad1TilesB:
	.incbin "games/tla/src/graphics/tileset/trial_road_stages/tiles_b.4bpp.mtf"
	.byte 0xc7	@ PACKER: left in the buffer by Tileset_TrialRoad1TilesA+0x1acb
	.global Tileset_TrialRoad1TilesC
Tileset_TrialRoad1TilesC:
	.incbin "games/tla/src/graphics/tileset/trial_road_stages/tiles_c.4bpp.mtf"
	.byte 0x7b, 0xcf	@ PACKER: left in the buffer by Tileset_TrialRoad1TilesB+0x71e
	.global Tileset_TrialRoad1TilesD
Tileset_TrialRoad1TilesD:
	.incbin "games/tla/src/graphics/tileset/trial_road_stages/tiles_d.4bpp.mtf"
	.byte 0x12, 0x05, 0x4a	@ PACKER: left in the buffer by Tileset_TrialRoad1TilesB+0x13bd
