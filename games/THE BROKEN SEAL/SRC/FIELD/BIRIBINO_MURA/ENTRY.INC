	.irp EntryTarget, 0x02008891, 0x02008089, 0x02008105, 0x0200811d, 0x02008199, 0x020080dd
	overlay_veneer \EntryTarget
	.endr
