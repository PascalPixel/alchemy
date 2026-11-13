	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_ValePrologue
Field_ValePrologue:
	map_header 0, 0, 60, 188, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ValePrologue, .LField_ValePrologue_CELL_PICTURES
	map_offset Field_ValePrologue, .LField_ValePrologue_SHAPES
	map_offset Field_ValePrologue, .LField_ValePrologue_GRID
	map_offset Field_ValePrologue, .LField_ValePrologue_ANIMATION
	map_offset Field_ValePrologue
	map_offset Field_ValePrologue, .LField_ValePrologue_MARKERS
.LField_ValePrologue_CELL_PICTURES:
	.incbin "games/tbs/src/map/vale_prologue/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ValePrologue_SHAPES:
	.incbin "games/tbs/src/map/vale_prologue/shapes.table.lz"
	.balign 4, 0
.LField_ValePrologue_GRID:
	.incbin "games/tbs/src/map/vale_prologue/map.grid.lz"
	.balign 4, 0
.LField_ValePrologue_ANIMATION:
	.incbin "games/tbs/src/map/vale_prologue/tileset_anims.script.lz"
	.balign 4, 0
.LField_ValePrologue_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_ValeProloguePalette
Tileset_ValeProloguePalette:
	.incbin "games/tbs/src/graphics/tileset/vale_prologue/palette.gbapal.lz"
	.byte 0x3f, 0x55, 0x78	@ PACKER: left in the buffer by Field_ValePrologue+0x18d
	.global Tileset_ValePrologueTilesA
Tileset_ValePrologueTilesA:
	.incbin "games/tbs/src/graphics/tileset/vale_prologue/tiles_a.4bpp.mtf"
	.global Tileset_ValePrologueTilesB
Tileset_ValePrologueTilesB:
	.incbin "games/tbs/src/graphics/tileset/vale_prologue/tiles_b.4bpp.mtf"
	.byte 0x5d, 0x04, 0x53	@ PACKER: left in the buffer by Tileset_ValePrologueTilesA+0x2529
	.global Tileset_ValePrologueTilesC
Tileset_ValePrologueTilesC:
	.incbin "games/tbs/src/graphics/tileset/vale_prologue/tiles_c.4bpp.mtf"
	.global Tileset_ValePrologueTilesD
Tileset_ValePrologueTilesD:
	.incbin "games/tbs/src/graphics/tileset/vale_prologue/tiles_d.4bpp.mtf"
	.byte 0x14, 0x85, 0xc2	@ PACKER: left in the buffer by Tileset_ValePrologueTilesC+0x1e8d
