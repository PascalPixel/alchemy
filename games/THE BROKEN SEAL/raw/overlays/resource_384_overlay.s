.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000308, 0x02000308
	.set sub_02000310, 0x02000310
	.set sub_02000320, 0x02000320
	.set sub_02000342, 0x02000342
	.set sub_02000344, 0x02000344
	.set sub_02000352, 0x02000352
	.set sub_02000354, 0x02000354
	.set sub_0200035e, 0x0200035e
	.set sub_02000362, 0x02000362
	.set sub_0200036e, 0x0200036e
	.set sub_0200037e, 0x0200037e
	.set sub_02000384, 0x02000384
	.set sub_0200038a, 0x0200038a
	.set sub_0200038c, 0x0200038c
	.set sub_0200038e, 0x0200038e
	.set sub_020003ac, 0x020003ac
	.set sub_020003b4, 0x020003b4
	.set sub_020003ba, 0x020003ba
	.set sub_020003d4, 0x020003d4
	.set sub_020003d8, 0x020003d8
	.set sub_020003dc, 0x020003dc
	.set sub_020003e4, 0x020003e4
	.set sub_020003e8, 0x020003e8
	.set sub_020003fe, 0x020003fe
	.set sub_02000404, 0x02000404
	.set sub_02000408, 0x02000408
	.set sub_02000418, 0x02000418
	.set sub_0200041e, 0x0200041e
	.set sub_02000424, 0x02000424
	.set sub_0200042a, 0x0200042a
	.set sub_0200043a, 0x0200043a
	.set sub_0200043e, 0x0200043e
	.set sub_0200044a, 0x0200044a
	.set sub_0200045a, 0x0200045a
	.set sub_02000466, 0x02000466
	.set sub_0200046a, 0x0200046a
	.set sub_0200047a, 0x0200047a
	.set sub_02000492, 0x02000492
	.set sub_0200049c, 0x0200049c
	.set sub_0200049e, 0x0200049e
	.set sub_020004a4, 0x020004a4
	.set sub_020004b4, 0x020004b4
	.set sub_020004c2, 0x020004c2
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/ENTRY.INC"
AlchemyC_02000030:
	.space 0x8
AlchemyC_02000038:
	.space 0x4
AlchemyC_0200003c:
	.space 0x8
AlchemyC_02000044:
	.space 0x28
AlchemyC_0200006c:
	.space 0x28
AlchemyC_02000094:
	.space 0x7c
AlchemyC_02000110:
	.space 0x30
AlchemyC_02000140:
	.space 0x30
AlchemyC_02000170:
	.space 0x20
AlchemyC_02000190:
	.space 0x20
AlchemyC_020001b0:
	.space 0x20
AlchemyC_020001d0:
	.space 0x12
	.2byte 0x0000
AlchemyC_020001e4:
	.space 0x7c
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/IMPORT.INC"
AlchemyData_020002d0:
	.space 0x200
