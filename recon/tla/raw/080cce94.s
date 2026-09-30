.syntax unified
	.thumb
	.global Func_080cce94
	.thumb_func
Func_080cce94:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r1, r5, #0
	movs r0, #0
	bl Func_080ccd78
	ldr r3, .L_080ccec4
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #106
	adds r3, r3, r2
	movs r2, #0
	ldrsh r1, [r3, r2]
	adds r6, r0, #0
	cmp r1, r5
	bne .L_080ccebe
	movs r0, #7
	bl Func_080ccd78
	cmp r0, #0
	bne .L_080ccec0
.L_080ccebe:
	adds r0, r6, #0
.L_080ccec0:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080ccec4:
	.4byte gPartyState
