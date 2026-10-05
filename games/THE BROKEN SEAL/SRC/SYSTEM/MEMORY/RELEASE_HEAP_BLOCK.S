.syntax unified
	.thumb
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	.global Func_08002dd8
	.thumb_func
Func_08002dd8:
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
	.global Party_Do
	.thumb_func
Party_Do:
	.global Runtime_BumpFree
	.thumb_func
Runtime_BumpFree:
	ldr	r4, [pc, #8]
	movs	r1, #4
	lsrs	r2, r0, #22
	ands	r2, r1
	str	r0, [r2, r4]
	bx	lr
	.4byte	gWorkSlot
