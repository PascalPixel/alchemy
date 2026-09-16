	.irp EntryTarget, 0x02008f71, 0x02008cc5, 0x02008d41, 0x02008d49, 0x02008ec5, 0x02008d3d
	overlay_veneer \EntryTarget
	.endr
