@ Releases a heap block by slot number or by pointer: both routines set the
@ block's region pointer (0x03001e50 for EWRAM, 0x03001e54 for IWRAM, chosen by
@ address bit 24) back to the block, and the slot form also clears its entry
@ in the slot table at 0x03001e50. The two separately called routines share
@ one literal: the first loads 0x03001e50 from the pool after the second, and
@ it branches on flags from lsrs r3, r1, #22 (0x08002dde). The approved
@ compilers give each function its own pool and compare before every branch.
@ The zero halfword that aligns the second routine (0x08002dee) never runs
@ and carries no credit. TBS only; TLA's version differs.
@ credit: handwritten — heap block release by slot and by pointer, sharing one literal pool
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08002dd8
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	ldr	r4, [pc, #32]
	lsls	r0, r0, #2
	ldr	r1, [r0, r4]
	lsrs	r3, r1, #22
	beq.n	.L_08002dec
	movs	r2, #0
	str	r2, [r0, r4]
	movs	r0, #4
	ands	r3, r0
	str	r1, [r3, r4]
.L_08002dec:
	bx	lr
AlchemyUncredited_08002dee:
	.2byte	0x0000
AlchemyUncreditedEnd_08002dee:
	.global Sys_Free
	.thumb_func
Sys_Free:
	ldr	r4, [pc, #8]
	movs	r1, #4
	lsrs	r2, r0, #22
	ands	r2, r1
	str	r0, [r2, r4]
	bx	lr
	.4byte	0x03001e50
