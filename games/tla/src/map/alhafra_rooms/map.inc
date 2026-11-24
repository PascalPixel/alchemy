	.global Field_AlhafraRooms
Field_AlhafraRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AlhafraRooms, .LField_AlhafraRooms_CELL_TILES
	map_block Field_AlhafraRooms, .LField_AlhafraRooms_SHAPES
	map_block Field_AlhafraRooms, .LField_AlhafraRooms_GRID
	map_block Field_AlhafraRooms, .LField_AlhafraRooms_SHAPE_GRID
	map_block Field_AlhafraRooms, .LField_AlhafraRooms_ANIMATION
	map_block Field_AlhafraRooms, .LField_AlhafraRooms_BLEND
	map_block Field_AlhafraRooms, .LField_AlhafraRooms_SCENE
.LField_AlhafraRooms_CELL_TILES:
	.incbin "games/tla/src/map/alhafra_rooms/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AlhafraRooms_SHAPES:
	.incbin "games/tla/src/map/alhafra_rooms/shapes.table.lz"
	.balign 4, 0
.LField_AlhafraRooms_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/alhafra_rooms/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/alhafra_rooms/map.grid.lz"
	.endif
	.balign 4, 0
.LField_AlhafraRooms_SHAPE_GRID:
	.incbin "games/tla/src/map/alhafra_rooms/shape_grid.plane.lz"
	.balign 4, 0
.LField_AlhafraRooms_ANIMATION:
	.incbin "games/tla/src/map/mikasalla_rooms/animation.script.lz"
	.balign 4, 0
.LField_AlhafraRooms_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_AlhafraRooms_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/alhafra_rooms/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/alhafra_rooms/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_AlhafraPalette
Tileset_AlhafraPalette:
	.incbin "games/tla/src/graphics/tileset/alhafra/palette.gbapal.lz"
	.global Tileset_AlhafraTilesA
Tileset_AlhafraTilesA:
	.incbin "games/tla/src/graphics/tileset/alhafra/tiles_a.4bpp.mtf"
	.byte 0x70, 0x0e	@ PACKER: left in the buffer by Field_AlhafraRooms+0x15e6
