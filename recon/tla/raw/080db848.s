.syntax unified
	.thumb
	.global BattleFx_ClearChildValueOnMismatch
	.thumb_func
BattleFx_ClearChildValueOnMismatch:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r5, [r3]
	movs r1, #30
	ldrsh r3, [r5, r1]
	cmp r3, #2
	bne .L_080db87c
	bl Func_080dca50
	ldr r3, .L_080db880
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #106
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r1, #26
	ldrsh r3, [r5, r1]
	cmp r2, r3
	beq .L_080db87c
	ldr r3, [r5, #20]
	movs r2, #0
	adds r3, #91
	strb r2, [r3]
.L_080db87c:
	pop {r5, pc}
	.2byte 0x0000
.L_080db880:
	.4byte gPartyState
