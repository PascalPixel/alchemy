.syntax unified
	.thumb
	.global Func_081a6ab4
	.thumb_func
Func_081a6ab4:
	push {r5, r6, lr}
	sub sp, #12
	adds r6, r0, #0
	movs r3, #0
	add r0, sp, #8
	mov r12, r2
	str r3, [r0]
	movs r2, #133
	movs r3, #128
	mov r4, sp
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r5, r1, #0
	adds r3, #212
	adds r1, r4, #0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldrb r3, [r4, #1]
	movs r2, #63
	ands r2, r3
	ldrb r3, [r4, #3]
	movs r1, #192
	orrs r3, r1
	strb r3, [r4, #3]
	movs r3, #32
	orrs r2, r3
	movs r3, #13
	negs r3, r3
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	movs r3, #3
	orrs r2, r3
	ldr r3, .L_081a6b30
	strb r2, [r4, #1]
	adds r2, r6, #0
	ldrh r1, [r4, #4]
	ands r2, r3
	ldr r3, .L_081a6b38
	subs r5, #32
	ands r3, r1
	orrs r3, r2
	strh r3, [r4, #4]
	ldr r3, .L_081a6b34
	ldrh r2, [r4, #2]
	ands r5, r3
	ldr r3, .L_081a6b3c
	lsrs r6, r6, #4
	ands r3, r2
	movs r2, #224
	orrs r3, r5
	add r12, r2
	strh r3, [r4, #2]
	lsls r2, r2, #19
	mov r3, r12
	strb r3, [r4]
	adds r1, r6, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	b .L_081a6b40
.L_081a6b30:
	.4byte 0x000003ff
.L_081a6b34:
	.4byte 0x000001ff
.L_081a6b38:
	.4byte 0xfffffc00
.L_081a6b3c:
	.4byte 0xfffffe00
.L_081a6b40:
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	add sp, #12
	pop {r5, r6, pc}
