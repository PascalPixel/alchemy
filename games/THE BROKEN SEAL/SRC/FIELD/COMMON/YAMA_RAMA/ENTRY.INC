	.irp EntryTarget, 0x020091b1, 0x0200807d, 0x020080b1, 0x020080b9, 0x02009181, 0x020080ad
	overlay_veneer \EntryTarget
	.endr
