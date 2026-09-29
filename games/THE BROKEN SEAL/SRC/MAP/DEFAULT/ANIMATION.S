@ Field_DefaultMapAnimation: the default field map's tile animation script,
@ packed by the resource packer.
	.section .rodata
	.global Field_DefaultMapAnimation
Field_DefaultMapAnimation:
	.incbin "MAP/DEFAULT/ANIMATION.table.lz"
