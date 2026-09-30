.syntax unified
	.thumb
	.global Func_081019a4
	.thumb_func
Func_081019a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r4, #0
	mov r8, r3
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	sub sp, #4
	cmp r4, r3
	bge .L_081019fa
	movs r5, #129
	adds r7, r0, #0
	lsls r5, r5, #2
	adds r7, #160
	adds r6, r0, #0
	add r5, r8
.L_081019d2:
	movs r2, #1
	ldrh r1, [r5]
	adds r0, r6, #0
	negs r2, r2
	str r4, [sp, #0]
	bl Func_08103064
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	ldr r4, [sp, #0]
	add r3, r8
	ldrb r3, [r3]
	adds r4, #1
	strb r0, [r7]
	adds r5, #2
	adds r7, #1
	adds r6, #20
	cmp r4, r3
	blt .L_081019d2
.L_081019fa:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
