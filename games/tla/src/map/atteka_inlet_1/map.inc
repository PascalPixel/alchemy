	.global Field_AttekaInlet1
Field_AttekaInlet1:
	map_header 0, 0, 104, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 104, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AttekaInlet1, .LField_AttekaInlet1_CELL_PICTURES
	map_block Field_AttekaInlet1, .LField_AttekaInlet1_SHAPES
	map_block Field_AttekaInlet1, .LField_AttekaInlet1_GRID
	map_block Field_AttekaInlet1, .LField_AttekaInlet1_SHAPE_GRID
	map_block Field_AttekaInlet1, .LField_AttekaInlet1_ANIMATION
	map_block Field_AttekaInlet1, .LField_AttekaInlet1_BLEND
	map_block Field_AttekaInlet1, .LField_AttekaInlet1_SCENE
.LField_AttekaInlet1_CELL_PICTURES:
	.incbin "games/tla/src/map/atteka_inlet_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AttekaInlet1_SHAPES:
	.incbin "games/tla/src/map/atteka_inlet_1/shapes.table.lz"
	.balign 4, 0
.LField_AttekaInlet1_GRID:
	.incbin "games/tla/src/map/atteka_inlet_1/map.grid.lz"
	.balign 4, 0
.LField_AttekaInlet1_SHAPE_GRID:
	.incbin "games/tla/src/map/atteka_inlet_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_AttekaInlet1_ANIMATION:
	.incbin "games/tla/src/map/atteka_inlet_1/animation.script.lz"
	.balign 4, 0
.LField_AttekaInlet1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AttekaInlet1_SCENE:
	.incbin "games/tla/src/map/atteka_inlet_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AttekaInlet1Palette
Tileset_AttekaInlet1Palette:
	.incbin "games/tla/src/graphics/tileset/atteka_inlet_1/palette.gbapal.lz"
	.byte 0xa8, 0xad, 0x01	@ PACKER: left in the buffer by Field_AttekaInlet1+0x141
	.global Tileset_AttekaInlet1TilesA
Tileset_AttekaInlet1TilesA:
	.incbin "games/tla/src/graphics/tileset/atteka_inlet_1/tiles_a.4bpp.mtf"
	.byte 0x40, 0x00	@ PACKER: left in the buffer by Field_AttekaInlet1+0x18ba
