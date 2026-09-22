	.irp EntryTarget, 0x0200b645, 0x020089ed, 0x020089f9, 0x02008a01, 0x0200b599, 0x020089f5
	overlay_veneer \EntryTarget
	.endr
