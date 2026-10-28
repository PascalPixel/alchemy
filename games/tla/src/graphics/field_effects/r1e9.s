@ A bitmap of 32 x 32 pixels, one byte a pixel in rows of 32, not tiles.
@ Func_080e68d0 unpacks its 1,024 bytes into its work buffer and makes one
@ 32x32 sprite of 256 colours over an empty block of OBJ video memory. Each
@ frame its callback has Func_080e62f4 clear a second 32 x 32 bitmap and plot
@ the first into it column by column, the rows four pixels apart and moved by
@ the frame count and by a sine of the column; the second bitmap is what goes
@ to the sprite's video memory.
	.section .rodata
	.global Resource_Data1E9
Resource_Data1E9:
	.incbin "games/tla/src/graphics/field_effects/r1e9.bitmap.lz"
	.byte 0x59, 0x2f	@ PACKER: left in the buffer by Resource_Data1E8+0xe2
