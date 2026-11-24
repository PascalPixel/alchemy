	.global Field_IsletCaveSentinel
Field_IsletCaveSentinel:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_IsletCaveSentinel, .LField_IsletCaveSentinel_CELL_TILES
	map_block Field_IsletCaveSentinel, .LField_IsletCaveSentinel_SHAPES
	map_block Field_IsletCaveSentinel, .LField_IsletCaveSentinel_GRID
	map_block Field_IsletCaveSentinel, .LField_IsletCaveSentinel_SHAPE_GRID
	map_block Field_IsletCaveSentinel, .LField_IsletCaveSentinel_ANIMATION
	map_block Field_IsletCaveSentinel, .LField_IsletCaveSentinel_BLEND
	map_block Field_IsletCaveSentinel, .LField_IsletCaveSentinel_SCENE
.LField_IsletCaveSentinel_CELL_TILES:
	.incbin "games/tla/src/map/islet_cave_sentinel/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_IsletCaveSentinel_SHAPES:
	.incbin "games/tla/src/map/islet_cave_sentinel/shapes.table.lz"
	.balign 4, 0
.LField_IsletCaveSentinel_GRID:
	.incbin "games/tla/src/map/islet_cave_sentinel/map.grid.lz"
	.balign 4, 0
.LField_IsletCaveSentinel_SHAPE_GRID:
	.incbin "games/tla/src/map/islet_cave_sentinel/shape_grid.plane.lz"
	.balign 4, 0
.LField_IsletCaveSentinel_ANIMATION:
	.incbin "games/tla/src/map/islet_cave_sentinel/animation.script.lz"
	.balign 4, 0
.LField_IsletCaveSentinel_BLEND:
	.incbin "games/tla/src/map/islet_cave_sentinel/blend.script.lz"
	.balign 4, 0
.LField_IsletCaveSentinel_SCENE:
	.incbin "games/tla/src/map/islet_cave_sentinel/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteIsletCaveSentinel
Tileset_PaletteIsletCaveSentinel:
	.incbin "games/tla/src/graphics/tileset/palettes/islet_cave_sentinel.gbapal.lz"
	.byte 0xbc, 0x70	@ PACKER: left in the buffer by Field_IsletCaveSentinel+0x172
