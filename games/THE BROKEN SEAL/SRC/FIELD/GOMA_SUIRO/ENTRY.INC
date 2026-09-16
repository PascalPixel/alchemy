	.irp EntryTarget, 0x02008415, 0x02008315, 0x02008349, 0x02008351, 0x020083e5, 0x02008345
	overlay_veneer \EntryTarget
	.endr
