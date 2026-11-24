	.global Field_AttekaInletGeode
Field_AttekaInletGeode:
	map_header 0, 0, 128, 128, 3, 1, 0, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AttekaInletGeode, .LField_AttekaInletGeode_CELL_TILES
	map_block Field_AttekaInletGeode, .LField_AttekaInletGeode_SHAPES
	map_block Field_AttekaInletGeode, .LField_AttekaInletGeode_GRID
	map_block Field_AttekaInletGeode, .LField_AttekaInletGeode_SHAPE_GRID
	map_block Field_AttekaInletGeode, .LField_AttekaInletGeode_ANIMATION
	map_block Field_AttekaInletGeode, .LField_AttekaInletGeode_BLEND
	map_block Field_AttekaInletGeode, .LField_AttekaInletGeode_SCENE
.LField_AttekaInletGeode_CELL_TILES:
	.incbin "games/tla/src/map/atteka_inlet_geode/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AttekaInletGeode_SHAPES:
	.incbin "games/tla/src/map/atteka_inlet_geode/shapes.table.lz"
	.balign 4, 0
.LField_AttekaInletGeode_GRID:
	.incbin "games/tla/src/map/atteka_inlet_geode/map.grid.lz"
	.balign 4, 0
.LField_AttekaInletGeode_SHAPE_GRID:
	.incbin "games/tla/src/map/atteka_inlet_geode/shape_grid.plane.lz"
	.balign 4, 0
.LField_AttekaInletGeode_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_AttekaInletGeode_BLEND:
	.incbin "games/tla/src/map/idejima/blend.script.lz"
	.balign 4, 0
.LField_AttekaInletGeode_SCENE:
	.incbin "games/tla/src/map/atteka_inlet_geode/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AttekaInletGeodePalette
Tileset_AttekaInletGeodePalette:
	.incbin "games/tla/src/graphics/tileset/atteka_inlet_geode/palette.gbapal.lz"
	.global Tileset_AttekaInletGeodeTilesA
Tileset_AttekaInletGeodeTilesA:
	.incbin "games/tla/src/graphics/tileset/atteka_inlet_geode/tiles_a.4bpp.mtf"
	.byte 0x20, 0xce	@ PACKER: left in the buffer by Field_AttekaInletGeode+0x281e
	.global Tileset_AttekaInletGeodeTilesB
Tileset_AttekaInletGeodeTilesB:
	.incbin "games/tla/src/graphics/tileset/atteka_inlet_geode/tiles_b.4bpp.mtf"
	.global Tileset_AttekaInletGeodeTilesC
Tileset_AttekaInletGeodeTilesC:
	.incbin "games/tla/src/graphics/tileset/atteka_inlet_geode/tiles_c.4bpp.mtf"
	.global Tileset_AttekaInletGeodeTilesD
Tileset_AttekaInletGeodeTilesD:
	.incbin "games/tla/src/graphics/tileset/atteka_inlet_geode/tiles_d.4bpp.mtf"
	.byte 0xc2, 0x8d	@ PACKER: left in the buffer by Tileset_AttekaInletGeodeTilesB+0x120e
