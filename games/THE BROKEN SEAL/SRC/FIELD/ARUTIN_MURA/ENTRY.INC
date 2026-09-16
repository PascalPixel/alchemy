	.irp EntryTarget, 0x02008875, 0x02008031, 0x02008075, 0x0200807d, 0x0200811d, 0x02008071
	overlay_veneer \EntryTarget
	.endr
