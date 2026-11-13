	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_CrossboneIsle
Field_CrossboneIsle:
	map_header 0, 0, 64, 78, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 76, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 152, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsle, .LField_CrossboneIsle_CELL_PICTURES
	map_offset Field_CrossboneIsle, .LField_CrossboneIsle_SHAPES
	map_offset Field_CrossboneIsle, .LField_CrossboneIsle_GRID
	map_offset Field_CrossboneIsle, .LField_CrossboneIsle_ANIMATION
	map_offset Field_CrossboneIsle
	map_offset Field_CrossboneIsle, .LField_CrossboneIsle_MARKERS
.LField_CrossboneIsle_CELL_PICTURES:
	.incbin "games/tbs/src/map/crossbone_isle/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_CrossboneIsle_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsle_GRID:
	.incbin "games/tbs/src/map/crossbone_isle/map.grid.lz"
	.balign 4, 0
.LField_CrossboneIsle_ANIMATION:
	.incbin "games/tbs/src/map/crossbone_isle/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsle_MARKERS:
	treasure_spot 14, 20, 100
	treasure_spot 22, 22, 101
	treasure_spot 53, 56, 101
	treasure_spots_end
	.global Tileset_PaletteCrossboneIsle
Tileset_PaletteCrossboneIsle:
	.incbin "games/tbs/src/graphics/tileset/palettes/crossbone_isle.gbapal.lz"
	.byte 0x94, 0x4b, 0xc1	@ PACKER: left in the buffer by Field_CrossboneIsle+0x191
