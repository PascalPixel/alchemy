	.irp EntryTarget, 0x02008e8d, 0x0200834d, 0x020083a5, 0x020083ad, 0x02008e5d, 0x020083a1
	overlay_veneer \EntryTarget
	.endr
