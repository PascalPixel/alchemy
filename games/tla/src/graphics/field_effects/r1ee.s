@ Two 8x8 tiles of 16 colours, one under the other. Func_080e8db0 unpacks
@ them, loads them as one block of OBJ video memory and makes 64 sprites
@ that alternate the two, each with the 256-colour bit cleared and OBJ
@ palette 14. Where that palette's colours come from is not established:
@ r1ee.png carries the first row of the shared OBJ palette, as r1ed.png does.
	.section .rodata
	.global Resource_Data1EE
Resource_Data1EE:
	.incbin "games/tla/src/graphics/field_effects/r1ee.4bpp.lz"
	.byte 0x9f, 0x42	@ PACKER: left in the buffer by Resource_Data1ED+0x22
