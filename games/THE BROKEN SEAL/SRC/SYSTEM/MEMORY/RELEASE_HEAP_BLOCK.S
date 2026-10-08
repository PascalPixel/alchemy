.syntax unified
	.thumb
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
	.global MemFree
	.thumb_func
MemFree:
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
	.ifdef TBS_EDITION_DE
	.set GAMEPAK_STOP, 1
	.endif
	.ifdef TBS_EDITION_ES
	.set GAMEPAK_STOP, 1
	.endif
	.ifdef GAMEPAK_STOP
@ カートリッジ割り込みなら音を止めて止まる
	.arm
	.global Runtime_StopOnGamePakIrq
Runtime_StopOnGamePakIrq:
	mov	ip, #0x04000000
	add	r3, ip, #0x200
	ldr	r2, [r3]
	and	r1, r2, r2, lsr #16
	mov	r2, #0
	ands	r0, r1, #0x2000
	strbne	r0, [r3, #-0x17c]
.L_08002e1c:
	bne	.L_08002e1c
	.endif
