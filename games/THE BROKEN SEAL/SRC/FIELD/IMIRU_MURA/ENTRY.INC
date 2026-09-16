	.irp EntryTarget, 0x02008b71, 0x02008131, 0x02008165, 0x0200816d, 0x020081ed, 0x02008161
	overlay_veneer \EntryTarget
	.endr
