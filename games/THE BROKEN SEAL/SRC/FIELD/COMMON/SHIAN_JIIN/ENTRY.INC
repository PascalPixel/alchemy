	.irp EntryTarget, 0x0200be59, 0x0200834d, 0x02008381, 0x02008389, 0x0200a575, 0x0200837d
	overlay_veneer \EntryTarget
	.endr
