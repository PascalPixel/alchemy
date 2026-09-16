	.irp EntryTarget, 0x0200921d, 0x02008b51, 0x02008b5d, 0x02008b65, 0x020090c1, 0x02008b59
	overlay_veneer \EntryTarget
	.endr
