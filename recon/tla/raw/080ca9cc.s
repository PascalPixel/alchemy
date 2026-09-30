.syntax unified
	.thumb
	.global Func_080ca9cc
	.thumb_func
Func_080ca9cc:
	push {r5, r6, lr}
	ldr r3, .L_080caa28
	sub sp, #4
	str r3, [sp, #0]
	adds r6, r1, #0
	ldrb r2, [r3]
	movs r1, #1
	adds r3, #1
	lsls r2, r2, #24
	str r3, [sp, #0]
	negs r1, r1
	asrs r3, r2, #24
	cmp r3, r1
	beq .L_080caa1e
	mov r5, sp
	mov r12, r1
.L_080ca9ec:
	asrs r3, r2, #24
	cmp r3, r0
	bne .L_080caa0a
	adds r0, r5, #0
	bl Func_080cc994
	lsls r0, r0, #16
	str r0, [r6]
	adds r0, r5, #0
	bl Func_080cc994
	lsls r0, r0, #16
	str r0, [r6, #8]
	movs r0, #0
	b .L_080caa22
.L_080caa0a:
	ldr r3, [sp, #0]
	adds r2, r3, #4
	str r2, [sp, #0]
	adds r3, #5
	ldrb r2, [r2]
	str r3, [sp, #0]
	lsls r2, r2, #24
	asrs r3, r2, #24
	cmp r3, r12
	bne .L_080ca9ec
.L_080caa1e:
	movs r0, #1
	negs r0, r0
.L_080caa22:
	add sp, #4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080caa28:
	.4byte Data_0202e008
