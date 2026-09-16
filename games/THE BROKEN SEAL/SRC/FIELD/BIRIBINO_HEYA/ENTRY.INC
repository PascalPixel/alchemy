	.irp EntryTarget, 0x020084c9, 0x02008031, 0x0200803d, 0x02008045, 0x0200811d, 0x02008039
	overlay_veneer \EntryTarget
	.endr
