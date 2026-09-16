	.irp EntryTarget, 0x020080b5, 0x02008031, 0x0200803d, 0x02008045, 0x020080ad, 0x02008039
	overlay_veneer \EntryTarget
	.endr
