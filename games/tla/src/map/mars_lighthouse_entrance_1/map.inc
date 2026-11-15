	.global Field_MarsLighthouseEntrance1
Field_MarsLighthouseEntrance1:
	map_header 0, 0, 88, 104, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseEntrance1, .LField_MarsLighthouseEntrance1_CELL_PICTURES
	map_block Field_MarsLighthouseEntrance1, .LField_MarsLighthouseEntrance1_SHAPES
	map_block Field_MarsLighthouseEntrance1, .LField_MarsLighthouseEntrance1_GRID
	map_block Field_MarsLighthouseEntrance1, .LField_MarsLighthouseEntrance1_SHAPE_GRID
	map_block Field_MarsLighthouseEntrance1, .LField_MarsLighthouseEntrance1_ANIMATION
	map_block Field_MarsLighthouseEntrance1
	map_block Field_MarsLighthouseEntrance1, .LField_MarsLighthouseEntrance1_SCENE
.LField_MarsLighthouseEntrance1_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance1_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_1/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance1_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_1/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance1_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance1_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MarsLighthouseEntrance1Palette
Tileset_MarsLighthouseEntrance1Palette:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_entrance_1/palette.gbapal.lz"
	.global Tileset_MarsLighthouseEntrance1TilesA
Tileset_MarsLighthouseEntrance1TilesA:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_entrance_1/tiles_a.4bpp.mtf"
	.byte 0x40, 0xff	@ PACKER: left in the buffer by Field_MarsLighthouseEntrance1+0xac2
