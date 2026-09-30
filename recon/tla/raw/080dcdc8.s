.syntax unified
	.thumb
	.global Func_080dcdc8
	.thumb_func
Func_080dcdc8:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #88]
	movs r2, #165
	lsls r2, r2, #2
	adds r3, r4, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080dce1a
	subs r2, #10
	adds r3, r4, r2
	ldrb r3, [r3]
	movs r2, #197
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r1, [r3, #10]
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r1
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r1, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r1
	strh r2, [r3, #10]
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #19
	ldrh r2, [r3, #10]
	adds r0, r4, r0
	adds r1, #16
	ldr r2, .L_080dce1c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_080dce1a:
	pop {pc}
.L_080dce1c:
	.4byte 0xa2600001
