	.irp EntryTarget, 0x0200a759, 0x02008031, 0x02008041, 0x02008049, 0x02008311, 0x02008039
	overlay_veneer \EntryTarget
	.endr
