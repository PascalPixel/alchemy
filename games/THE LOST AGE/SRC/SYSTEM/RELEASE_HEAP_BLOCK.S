@ Releases a heap slot: loads the slot's block pointer from 0x03000000 + r0
@ and, when there is one, clears the slot and stores the block as its
@ region's free head (0x03000000 or 0x03000004, chosen by address bit 24).
@ lsrs r3, r1, #22; beq branches on the Z flag of the shift with no compare
@ between them, where both approved compilers emit each Thumb conditional
@ branch directly after its own cmp or cmn. After its two base-address
@ instructions it equals TBS SYSTEM/MEMORY/RELEASE_HEAP_BLOCK.S from
@ 0x08002ddc, whose base is the pooled 0x03001e50 and a scaled slot index.
@ The zero halfword after its return (0x08013162) never runs and carries no
@ credit.
@ credit: handwritten — IWRAM slot free helper
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x0801314c
	.global Runtime_ReleaseSlot
	.thumb_func
Runtime_ReleaseSlot:
	movs	r4, #3
	lsls	r4, r4, #24
	ldr	r1, [r0, r4]
	lsrs	r3, r1, #22
	beq.n	.L_08013160
	movs	r2, #0
	str	r2, [r0, r4]
	movs	r0, #4
	ands	r3, r0
	str	r1, [r3, r4]
.L_08013160:
	bx	lr
AlchemyUncredited_08013162:
	.2byte	0x0000
AlchemyUncreditedEnd_08013162:
