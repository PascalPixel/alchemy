@ Field_DefaultMetatileAttributes: the default field map's metatile attributes,
@ packed by the resource packer.
	.section .rodata
	.global Field_DefaultMetatileAttributes
Field_DefaultMetatileAttributes:
	.incbin "MAP/DEFAULT/ATTRIBUTES.table.lz"
