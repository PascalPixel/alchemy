@ The countdown clock's characters 0 to 9 and the colon, 8x16 sprites of 16
@ colours, one under another. Func_020052a4 (overlay 671) unpacks them, and
@ each tick Func_02005090 copies the glyph of each character of the time,
@ (character - '0') * 64 bytes in, to the clock's sprites, which name OBJ
@ palette 0: the first row of the shared OBJ palette, which the picture
@ carries.
	.section .rodata
	.global Field_ClockDigits
Field_ClockDigits:
	.incbin "games/tla/src/graphics/field/clock_digits.4bpp.lz"
	.byte 0x00, 0x1e, 0x21	@ PACKER: left in the buffer by UiText_SmallFont+0xbd
