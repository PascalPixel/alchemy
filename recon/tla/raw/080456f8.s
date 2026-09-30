.syntax unified
	.thumb
	.global Func_080456f8
	.thumb_func
Func_080456f8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r3, r1, #0
	lsls r5, r3, #3
	subs r5, r5, r3
	adds r4, r0, #0
	mov r8, r2
	lsls r5, r5, #2
	movs r2, #130
	adds r6, r4, r5
	lsls r3, r3, #4
	lsls r2, r2, #1
	sub sp, #4
	adds r6, r6, r2
	mov r1, r8
	adds r2, r3, #0
	str r4, [sp, #0]
	bl Func_0804562c
	movs r2, #142
	ldr r4, [sp, #0]
	lsls r2, r2, #1
	adds r3, r5, r2
	mov r2, r8
	str r2, [r4, r3]
	ldr r3, .L_08045758
	str r3, [r6, #4]
	movs r3, #0
	str r3, [r6, #8]
	movs r3, #136
	lsls r3, r3, #1
	adds r5, r5, r3
	ldrh r0, [r4, r5]
	mov r1, r8
	bl Func_080455dc
	ldr r3, .L_08045754
	ldrh r2, [r6, #8]
	ands r0, r3
	ldr r3, .L_0804575c
	add sp, #4
	ands r3, r2
	orrs r3, r0
	strh r3, [r6, #8]
	b .L_08045760
.L_08045754:
	.4byte 0x000003ff
.L_08045758:
	.4byte 0x80002000
.L_0804575c:
	.4byte 0xfffffc00
.L_08045760:
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
