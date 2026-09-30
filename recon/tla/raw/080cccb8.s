.syntax unified
	.thumb
	.global Func_080cccb8
	.thumb_func
Func_080cccb8:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	movs r2, #1
	ldr r7, [r3, #108]
	movs r1, #0
	negs r2, r2
	adds r4, r0, #0
	movs r6, #8
	mov r12, r1
	mov lr, r2
.L_080cccce:
	ldmia r7!, {r0}
	cmp r0, #0
	beq .L_080ccd2a
	cmp r4, #7
	bgt .L_080cccfc
	movs r5, #0
	ldrsh r3, [r0, r5]
	ldrh r2, [r0]
	cmp r3, lr
	beq .L_080ccd2a
	movs r1, #1
	negs r1, r1
.L_080ccce6:
	lsls r3, r2, #16
	asrs r3, r3, #16
	cmp r3, r4
	beq .L_080ccd36
	adds r0, #24
	movs r5, #0
	ldrsh r3, [r0, r5]
	ldrh r2, [r0]
	cmp r3, r1
	bne .L_080ccce6
	b .L_080ccd2a
.L_080cccfc:
	ldrh r2, [r0]
	lsls r3, r2, #16
	asrs r3, r3, #16
	adds r1, r2, #0
	cmp r3, lr
	beq .L_080ccd2a
	movs r5, #1
	negs r5, r5
.L_080ccd0c:
	lsls r3, r1, #16
	movs r1, #224
	lsls r1, r1, #11
	cmp r3, r1
	ble .L_080ccd1c
	cmp r6, r4
	beq .L_080ccd36
	adds r6, #1
.L_080ccd1c:
	adds r0, #24
	ldrh r2, [r0]
	lsls r3, r2, #16
	asrs r3, r3, #16
	adds r1, r2, #0
	cmp r3, r5
	bne .L_080ccd0c
.L_080ccd2a:
	movs r2, #1
	add r12, r2
	mov r3, r12
	cmp r3, #3
	ble .L_080cccce
	ldrh r2, [r0]
.L_080ccd36:
	lsls r3, r2, #16
	movs r5, #1
	asrs r3, r3, #16
	negs r5, r5
	cmp r3, r5
	bne .L_080ccd44
	movs r0, #0
.L_080ccd44:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
