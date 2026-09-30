.syntax unified
	.thumb
	.global Func_0803d92c
	.thumb_func
Func_0803d92c:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #1
	movs r3, #192
	adds r0, #255
	lsls r3, r3, #18
	ands r0, r5
	ldr r6, [r3, #68]
	bl Item_Get
	cmp r5, #0
	beq .L_0803d95a
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #4
	ldrh r3, [r0, #6]
	adds r1, r6, r2
	ldr r2, .L_0803d988
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	str r3, [r1]
	b .L_0803d968
.L_0803d95a:
	ldr r2, .L_0803d988
	movs r1, #192
	lsls r1, r1, #3
	ldr r2, [r2]
	adds r1, #4
	adds r3, r6, r1
	str r2, [r3]
.L_0803d968:
	movs r2, #192
	movs r1, #192
	lsls r2, r2, #3
	lsls r1, r1, #3
	adds r3, r6, r2
	adds r1, #2
	movs r2, #2
	strh r2, [r3]
	adds r3, r6, r1
	strh r2, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl Func_0803db54
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0803d988:
	.4byte Data_0804eb58
