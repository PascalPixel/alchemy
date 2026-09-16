	.irp EntryTarget, 0x020081e5, 0x02008031, 0x0200803d, 0x02008045, 0x0200806d, 0x02008039
	overlay_veneer \EntryTarget
	.endr
