	.global Field_TreasureIsleEntrance
Field_TreasureIsleEntrance:
	map_header 0, 0, 96, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TreasureIsleEntrance, .LField_TreasureIsleEntrance_CELL_TILES
	map_block Field_TreasureIsleEntrance, .LField_TreasureIsleEntrance_SHAPES
	map_block Field_TreasureIsleEntrance, .LField_TreasureIsleEntrance_GRID
	map_block Field_TreasureIsleEntrance, .LField_TreasureIsleEntrance_SHAPE_GRID
	map_block Field_TreasureIsleEntrance, .LField_TreasureIsleEntrance_ANIMATION
	map_block Field_TreasureIsleEntrance, .LField_TreasureIsleEntrance_BLEND
	map_block Field_TreasureIsleEntrance, .LField_TreasureIsleEntrance_SCENE
.LField_TreasureIsleEntrance_CELL_TILES:
	.incbin "games/tla/src/map/treasure_isle_entrance/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_TreasureIsleEntrance_SHAPES:
	.incbin "games/tla/src/map/treasure_isle_entrance/shapes.table.lz"
	.balign 4, 0
.LField_TreasureIsleEntrance_GRID:
	.incbin "games/tla/src/map/treasure_isle_entrance/map.grid.lz"
	.balign 4, 0
.LField_TreasureIsleEntrance_SHAPE_GRID:
	.incbin "games/tla/src/map/treasure_isle_entrance/shape_grid.plane.lz"
	.balign 4, 0
.LField_TreasureIsleEntrance_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_TreasureIsleEntrance_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TreasureIsleEntrance_SCENE:
	.incbin "games/tla/src/map/treasure_isle_entrance/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TreasureIsleEntrancePalette
Tileset_TreasureIsleEntrancePalette:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_entrance/palette.gbapal.lz"
	.byte 0x9e, 0x4d	@ PACKER: left in the buffer by Field_TreasureIsleEntrance+0x142
	.global Tileset_TreasureIsleEntranceTilesA
Tileset_TreasureIsleEntranceTilesA:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_entrance/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_TreasureIsleEntranceTilesB
Tileset_TreasureIsleEntranceTilesB:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_entrance/tiles_b.4bpp.mtf"
	.global Tileset_TreasureIsleEntranceTilesC
Tileset_TreasureIsleEntranceTilesC:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_entrance/tiles_c.4bpp.mtf"
	.byte 0x06, 0x6b, 0x35	@ PACKER: left in the buffer by Tileset_TreasureIsleEntranceTilesB+0xbe5
	.global Tileset_TreasureIsleEntranceTilesD
Tileset_TreasureIsleEntranceTilesD:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_entrance/tiles_d.4bpp.mtf"
	.byte 0x88, 0x5f	@ PACKER: left in the buffer by Tileset_TreasureIsleEntranceTilesC+0x3fa
