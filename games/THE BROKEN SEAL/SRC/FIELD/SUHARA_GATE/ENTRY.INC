	.irp EntryTarget, 0x02008125, 0x02008041, 0x02008085, 0x0200808d, 0x020080e5, 0x02008081
	overlay_veneer \EntryTarget
	.endr
