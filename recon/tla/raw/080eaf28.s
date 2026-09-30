.syntax unified
	.thumb
	.global Func_080eaf28
	.thumb_func
Func_080eaf28:
	push {r5, r6, r7, lr}
	movs r2, #192
	lsls r2, r2, #18
	adds r4, r0, #0
	ldr r2, [r2, #108]
	ldr r3, [r4]
	movs r7, #255
	mov r12, r1
	lsls r7, r7, #8
	adds r1, r2, #0
	movs r6, #8
	asrs r5, r3, #20
	adds r7, #255
	adds r1, #52
.L_080eaf44:
	mov r3, r12
	ldmia r1!, {r0}
	cmp r3, #0
	beq .L_080eaf7a
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r5, r3
	bne .L_080eaf8e
	ldr r3, [r4, #4]
	cmp r3, #0
	bge .L_080eaf5c
	adds r3, r3, r7
.L_080eaf5c:
	asrs r2, r3, #16
	ldr r3, [r0, #12]
	cmp r3, #0
	bge .L_080eaf66
	adds r3, r3, r7
.L_080eaf66:
	asrs r3, r3, #16
	cmp r2, r3
	bne .L_080eaf8e
	ldr r2, [r4, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_080eaf8e
	b .L_080eaf96
.L_080eaf7a:
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r5, r3
	bne .L_080eaf8e
	ldr r2, [r4, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_080eaf96
.L_080eaf8e:
	adds r6, #1
	cmp r6, #63
	bls .L_080eaf44
	movs r0, #0
.L_080eaf96:
	pop {r5, r6, r7, pc}
