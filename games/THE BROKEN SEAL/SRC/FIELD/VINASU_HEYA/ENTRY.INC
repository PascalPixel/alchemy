	.irp EntryTarget, 0x0200b069, 0x02008e05, 0x02008e81, 0x02008e89, 0x0200af8d, 0x02008e7d
	overlay_veneer \EntryTarget
	.endr
