.syntax unified
	.thumb
	.global Func_0803975c
	.thumb_func
Func_0803975c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	lsls r3, r0, #1
	ldr r6, .L_080397d4
	adds r3, r3, r0
	lsls r1, r3, #1
	lsls r3, r3, #3
	mov r8, r3
	adds r5, r3, r6
	ldr r3, .L_080397d8
	sub sp, #8
	str r3, [sp, #0]
	movs r3, #32
	movs r2, #24
	movs r4, #132
	subs r3, r3, r1
	subs r2, r2, r1
	lsls r4, r4, #24
	lsls r3, r3, #2
	mov r9, r2
	mov r11, r4
	mov r10, r3
	movs r7, #29
.L_08039794:
	movs r3, #128
	mov r2, r9
	mov r4, r11
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r5, #0
	adds r1, r6, #0
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, [sp, #0]
	ldr r3, .L_080397dc
	add r0, r10
	mov r1, r8
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	ldr r3, [sp, #0]
	subs r7, #1
	adds r3, #128
	adds r6, #128
	adds r5, #128
	str r3, [sp, #0]
	cmp r7, #0
	bge .L_08039794
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080397d4:
	.4byte 0x06002520
.L_080397d8:
	.4byte 0x06002500
.L_080397dc:
	.4byte IwramFillWords
