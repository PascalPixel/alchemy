	.irp EntryTarget, 0x0200bf25, 0x0200836d, 0x02008379, 0x02008381, 0x02008389, 0x02008375
	overlay_veneer \EntryTarget
	.endr
