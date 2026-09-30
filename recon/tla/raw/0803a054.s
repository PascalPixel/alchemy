.syntax unified
	.thumb
	.global Func_0803a054
	.thumb_func
Func_0803a054:
	push {r5, lr}
	adds r4, r2, #0
	sub sp, #4
	adds r5, r0, #0
	cmp r4, #0
	ble .L_0803a07c
	mov r0, sp
	movs r2, #129
	movs r3, #128
	adds r0, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	strh r1, [r0]
	adds r3, #212
	adds r1, r5, #0
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	lsls r3, r4, #1
	adds r5, r5, r3
.L_0803a07c:
	adds r0, r5, #0
	add sp, #4
	pop {r5, pc}
	.2byte 0x0000
