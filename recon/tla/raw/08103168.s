.syntax unified
	.thumb
	.global Func_08103168
	.thumb_func
Func_08103168:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	adds r4, r0, #0
	sub sp, #24
	mov r8, r1
	ldr r7, [r3]
	cmp r4, #0
	bne .L_081031c6
	movs r2, #175
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r7, r2
	ldrb r3, [r3]
	ldr r0, [r7, #56]
	movs r5, #1
	movs r6, #2
	str r1, [sp, #12]
	movs r2, #0
	movs r1, #0
	str r4, [sp, #4]
	str r4, [sp, #20]
	str r5, [sp, #0]
	str r6, [sp, #8]
	str r5, [sp, #16]
	bl Func_08103218
	movs r2, #151
	lsls r2, r2, #2
	ldr r4, [sp, #20]
	adds r3, r7, r2
	mov r2, r8
	ldr r0, [r7, #40]
	ldrb r3, [r3]
	movs r1, #0
	str r2, [sp, #12]
	movs r2, #0
	str r4, [sp, #0]
	str r5, [sp, #4]
	str r6, [sp, #8]
	str r4, [sp, #16]
	bl Func_08103218
	b .L_0810320c
.L_081031c6:
	movs r2, #140
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r7, r2
	movs r2, #4
	ldrb r3, [r3]
	ldr r0, [r7, #56]
	str r2, [sp, #8]
	mov r2, r8
	movs r5, #0
	movs r6, #1
	str r2, [sp, #12]
	movs r1, #0
	movs r2, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	str r6, [sp, #16]
	bl Func_08103218
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #22
	adds r3, r7, r2
	mov r2, r8
	ldr r0, [r7, #40]
	ldrb r3, [r3]
	movs r1, #0
	str r2, [sp, #12]
	movs r2, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	str r6, [sp, #8]
	str r5, [sp, #16]
	bl Func_08103218
.L_0810320c:
	movs r0, #1
	add sp, #24
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
