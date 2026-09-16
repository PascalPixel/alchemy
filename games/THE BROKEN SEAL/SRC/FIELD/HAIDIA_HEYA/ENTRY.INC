	.irp EntryTarget, 0x02008369, 0x020080cd, 0x020080d9, 0x020080fd, 0x02008141, 0x020080d5
	overlay_veneer \EntryTarget
	.endr
