.syntax unified
	.thumb
	.global Func_08038fa8
	.thumb_func
Func_08038fa8:
	push {lr}
	mov r12, r3
	mov r3, r9
	push {r3}
	mov r3, r12
	adds r3, r0, #0
	movs r0, #192
	mov r2, r9
	sub sp, #4
	lsls r0, r0, #2
	str r2, [sp, #0]
	adds r4, r1, #0
	adds r0, #255
	movs r2, #192
	ands r4, r0
	lsls r2, r2, #19
	ands r0, r3
	adds r2, #16
	lsls r0, r0, #5
	lsls r4, r4, #5
	adds r0, r0, r2
	subs r2, #16
	adds r1, r4, r2
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #192
	lsls r3, r3, #19
	adds r3, #12
	adds r4, r4, r3
	adds r0, r4, #0
	ldr r3, .L_08039000
	movs r1, #20
	mov lr, r3
	.2byte 0xf800
	add sp, #4
	pop {r3}
	mov r9, r3
	pop {pc}
.L_08039000:
	.4byte IwramClearWords
