	.global Field_IsletsPrettyStone
Field_IsletsPrettyStone:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_IsletsPrettyStone, .LField_IsletsPrettyStone_CELL_TILES
	map_block Field_IsletsPrettyStone, .LField_IsletsPrettyStone_SHAPES
	map_block Field_IsletsPrettyStone, .LField_IsletsPrettyStone_GRID
	map_block Field_IsletsPrettyStone, .LField_IsletsPrettyStone_SHAPE_GRID
	map_block Field_IsletsPrettyStone, .LField_IsletsPrettyStone_ANIMATION
	map_block Field_IsletsPrettyStone
	map_block Field_IsletsPrettyStone, .LField_IsletsPrettyStone_SCENE
.LField_IsletsPrettyStone_CELL_TILES:
	.incbin "games/tla/src/map/islets_pretty_stone/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_IsletsPrettyStone_SHAPES:
	.incbin "games/tla/src/map/islets_pretty_stone/shapes.table.lz"
	.balign 4, 0
.LField_IsletsPrettyStone_GRID:
	.incbin "games/tla/src/map/islets_pretty_stone/map.grid.lz"
	.balign 4, 0
.LField_IsletsPrettyStone_SHAPE_GRID:
	.incbin "games/tla/src/map/islets_pretty_stone/shape_grid.plane.lz"
	.balign 4, 0
.LField_IsletsPrettyStone_ANIMATION:
	.incbin "games/tla/src/map/islets_pretty_stone/animation.script.lz"
	.balign 4, 0
.LField_IsletsPrettyStone_SCENE:
	.incbin "games/tla/src/map/islets_pretty_stone/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteIsletsPrettyStone
Tileset_PaletteIsletsPrettyStone:
	.incbin "games/tla/src/graphics/tileset/palettes/islets_pretty_stone.gbapal.lz"
	.byte 0xac, 0x2f	@ PACKER: left in the buffer by Field_Islets2+0x116
