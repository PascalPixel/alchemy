	.irp EntryTarget, 0x0200a021, 0x020080e5, 0x02008205, 0x0200820d, 0x02009679, 0x020081c9
	overlay_veneer \EntryTarget
	.endr
