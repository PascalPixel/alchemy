	.global Field_TaopoSwampEntrance
Field_TaopoSwampEntrance:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TaopoSwampEntrance, .LField_TaopoSwampEntrance_CELL_TILES
	map_block Field_TaopoSwampEntrance, .LField_TaopoSwampEntrance_SHAPES
	map_block Field_TaopoSwampEntrance, .LField_TaopoSwampEntrance_GRID
	map_block Field_TaopoSwampEntrance, .LField_TaopoSwampEntrance_SHAPE_GRID
	map_block Field_TaopoSwampEntrance, .LField_TaopoSwampEntrance_ANIMATION
	map_block Field_TaopoSwampEntrance, .LField_TaopoSwampEntrance_BLEND
	map_block Field_TaopoSwampEntrance, .LField_TaopoSwampEntrance_SCENE
.LField_TaopoSwampEntrance_CELL_TILES:
	.incbin "games/tla/src/map/taopo_swamp_entrance/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_TaopoSwampEntrance_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_entrance/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwampEntrance_GRID:
	.incbin "games/tla/src/map/taopo_swamp_entrance/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwampEntrance_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_entrance/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwampEntrance_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_entrance/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwampEntrance_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwampEntrance_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_entrance/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TaopoSwampEntrancePalette
Tileset_TaopoSwampEntrancePalette:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_entrance/palette.gbapal.lz"
	.byte 0x18, 0x8c	@ PACKER: left in the buffer by Field_TaopoSwampEntrance+0x116
	.global Tileset_TaopoSwampEntranceTilesA
Tileset_TaopoSwampEntranceTilesA:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_entrance/tiles_a.4bpp.mtf"
	.byte 0x00, 0x1b, 0x22	@ PACKER: left in the buffer by Field_YallamRooms+0x2091
	.global Tileset_TaopoSwampEntranceTilesB
Tileset_TaopoSwampEntranceTilesB:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_entrance/tiles_b.4bpp.mtf"
	.global Tileset_TaopoSwampEntranceTilesC
Tileset_TaopoSwampEntranceTilesC:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_entrance/tiles_c.4bpp.mtf"
	.byte 0xf5	@ PACKER: left in the buffer by Tileset_TaopoSwampEntranceTilesB+0x697
	.global Tileset_TaopoSwampEntranceTilesD
Tileset_TaopoSwampEntranceTilesD:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_entrance/tiles_d.4bpp.mtf"
