.syntax unified
	.thumb
	.global Func_08125c5c
	.thumb_func
Func_08125c5c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #168
	ldr r3, [r3]
	ldr r3, [r3]
	subs r0, r3, #1
	cmp r0, #31
	bhi .L_08125c88
	ldr r2, .L_08125c8c
	lsrs r0, r0, #2
	lsls r0, r0, #5
	adds r0, r0, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r1, .L_08125c90
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08125c88:
	pop {pc}
	.2byte 0x0000
.L_08125c8c:
	.4byte Data_0812cc74
.L_08125c90:
	.4byte 0x06005000
