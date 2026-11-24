	.global Field_AttekaInlet
Field_AttekaInlet:
	map_header 0, 0, 104, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 104, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AttekaInlet, .LField_AttekaInlet_CELL_TILES
	map_block Field_AttekaInlet, .LField_AttekaInlet_SHAPES
	map_block Field_AttekaInlet, .LField_AttekaInlet_GRID
	map_block Field_AttekaInlet, .LField_AttekaInlet_SHAPE_GRID
	map_block Field_AttekaInlet, .LField_AttekaInlet_ANIMATION
	map_block Field_AttekaInlet, .LField_AttekaInlet_BLEND
	map_block Field_AttekaInlet, .LField_AttekaInlet_SCENE
.LField_AttekaInlet_CELL_TILES:
	.incbin "games/tla/src/map/atteka_inlet/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AttekaInlet_SHAPES:
	.incbin "games/tla/src/map/atteka_inlet/shapes.table.lz"
	.balign 4, 0
.LField_AttekaInlet_GRID:
	.incbin "games/tla/src/map/atteka_inlet/map.grid.lz"
	.balign 4, 0
.LField_AttekaInlet_SHAPE_GRID:
	.incbin "games/tla/src/map/atteka_inlet/shape_grid.plane.lz"
	.balign 4, 0
.LField_AttekaInlet_ANIMATION:
	.incbin "games/tla/src/map/atteka_inlet/animation.script.lz"
	.balign 4, 0
.LField_AttekaInlet_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AttekaInlet_SCENE:
	.incbin "games/tla/src/map/atteka_inlet/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AttekaInletPalette
Tileset_AttekaInletPalette:
	.incbin "games/tla/src/graphics/tileset/atteka_inlet/palette.gbapal.lz"
	.byte 0xa8, 0xad, 0x01	@ PACKER: left in the buffer by Field_AttekaInlet1+0x141
	.global Tileset_AttekaInletTilesA
Tileset_AttekaInletTilesA:
	.incbin "games/tla/src/graphics/tileset/atteka_inlet/tiles_a.4bpp.mtf"
	.byte 0x40, 0x00	@ PACKER: left in the buffer by Field_AttekaInlet1+0x18ba
