	.global Field_Map279
Field_Map279:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map279, .LField_Map279_CELL_PICTURES
	map_block Field_Map279, .LField_Map279_SHAPES
	map_block Field_Map279, .LField_Map279_GRID
	map_block Field_Map279, .LField_Map279_SHAPE_GRID
	map_block Field_Map279, .LField_Map279_ANIMATION
	map_block Field_Map279, .LField_Map279_BLEND
	map_block Field_Map279, .LField_Map279_SCENE
.LField_Map279_CELL_PICTURES:
	.incbin "games/tla/src/map/map_279/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map279_SHAPES:
	.incbin "games/tla/src/map/map_279/shapes.table.lz"
	.balign 4, 0
.LField_Map279_GRID:
	.incbin "games/tla/src/map/map_279/map.grid.lz"
	.balign 4, 0
.LField_Map279_SHAPE_GRID:
	.incbin "games/tla/src/map/map_279/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map279_ANIMATION:
	.incbin "games/tla/src/map/map_279/animation.script.lz"
	.balign 4, 0
.LField_Map279_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Map279_SCENE:
	.incbin "games/tla/src/map/map_279/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set114Palette
Tileset_Set114Palette:
	.incbin "games/tla/src/graphics/tileset/set_114/palette.gbapal.lz"
	.byte 0x17, 0x3c	@ PACKER: left in the buffer by Field_Map279+0x152
	.global Tileset_Set114TilesA
Tileset_Set114TilesA:
	.incbin "games/tla/src/graphics/tileset/set_114/tiles_a.4bpp.mtf"
	.global Tileset_Set114TilesB
Tileset_Set114TilesB:
	.incbin "games/tla/src/graphics/tileset/set_114/tiles_b.4bpp.mtf"
	.byte 0x97	@ PACKER: left in the buffer by Tileset_Set114TilesA+0x166b
	.global Tileset_Set114TilesC
Tileset_Set114TilesC:
	.incbin "games/tla/src/graphics/tileset/set_114/tiles_c.4bpp.mtf"
	.global Tileset_Set114TilesD
Tileset_Set114TilesD:
	.incbin "games/tla/src/graphics/tileset/set_114/tiles_d.4bpp.mtf"
	.byte 0x16, 0x41	@ PACKER: left in the buffer by Tileset_Set114TilesC+0x776
