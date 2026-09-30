.syntax unified
	.thumb
	.global Func_08125c0c
	.thumb_func
Func_08125c0c:
	push {lr}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #176
	ldr r3, [r3]
	ldr r3, [r3, #8]
	cmp r3, #2
	bne .L_08125c54
	ldr r4, [r2, #40]
	ldr r3, [r4]
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #6
	adds r0, r4, r0
	ldrh r3, [r0, #32]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #12
	strh r3, [r1]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	adds r0, #34
	ldr r2, .L_08125c58
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	adds r0, r4, #0
	lsls r2, r2, #24
	adds r3, #36
	adds r0, #16
	adds r1, #20
	adds r2, #4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08125c54:
	pop {pc}
	.2byte 0x0000
.L_08125c58:
	.4byte 0xa2600001
