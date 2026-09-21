.syntax unified
	.thumb
	.set Func_0808d394, 0x0808d394
	.set Func_08185000, 0x08185000
	.global BattleFx_CopyLinkedObjectPosition
	.thumb_func
BattleFx_CopyLinkedObjectPosition:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldr r6, [r5, #104]
	cmp r6, #0
	beq .L_done
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [r6, #8]
	str r3, [r5, #8]
	adds r3, r5, #0
	adds r3, #102
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_0808d394
	movs r3, #0
	ldrsh r0, [r0, r3]
	bl Func_08185000
	movs r2, #8
	ldrsb r2, [r0, r2]
	ldr r3, [r6, #12]
	lsls r2, r2, #16
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r6, #20]
	str r3, [r5, #20]
	ldr r3, [r6, #16]
	str r3, [r5, #16]
.L_done:
	movs r0, #0
	pop {r5, r6}
	pop {r1}
	bx r1
