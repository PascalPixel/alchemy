	.irp EntryTarget, 0x0200886d, 0x02008041, 0x02008099, 0x020080a1, 0x0200819d, 0x02008095
	overlay_veneer \EntryTarget
	.endr
