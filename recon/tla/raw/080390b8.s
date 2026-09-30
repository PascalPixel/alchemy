.syntax unified
	.thumb
	.global Func_080390b8
	.thumb_func
Func_080390b8:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #60]
	ldrb r4, [r7, #3]
	cmp r4, #0
	beq .L_08039116
	ldrb r3, [r7, #6]
	cmp r3, #0
	bne .L_08039116
	ldr r2, .L_08039118
	movs r3, #1
	adds r5, r7, #0
	ands r3, r4
	adds r5, #8
	mov r12, r2
	cmp r3, #0
	beq .L_080390de
	movs r4, #63
.L_080390de:
	movs r3, #63
	ands r4, r3
	movs r6, #128
	movs r3, #1
	lsrs r4, r4, #1
	mov lr, r3
	lsls r6, r6, #1
.L_080390ec:
	adds r3, r4, #0
	mov r2, lr
	ands r3, r2
	cmp r3, #0
	beq .L_0803910a
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	mov r1, r12
	adds r2, #64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_0803910a:
	lsrs r4, r4, #1
	adds r5, r5, r6
	add r12, r6
	cmp r4, #0
	bne .L_080390ec
	strb r4, [r7, #3]
.L_08039116:
	pop {r5, r6, r7, pc}
.L_08039118:
	.4byte 0x06002000
