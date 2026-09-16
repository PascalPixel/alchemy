	.irp EntryTarget, 0x0200a74d, 0x02009441, 0x020094bd, 0x020094c5, 0x0200a581, 0x020094b9
	overlay_veneer \EntryTarget
	.endr
