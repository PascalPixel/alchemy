.syntax unified
	.thumb
	.global Func_080e5cf0
	.thumb_func
Func_080e5cf0:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	movs r1, #192
	lsls r1, r1, #5
	adds r1, #172
	adds r4, r0, r1
	movs r2, #0
	ldrsh r3, [r4, r2]
	cmp r3, #0
	bne .L_080e5d3c
	subs r1, #4
	adds r3, r0, r1
	ldr r2, [r3]
	ldr r1, .L_080e5d4c
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r2, #12]
	ldr r1, .L_080e5d50
	ldr r3, [r2, #24]
	adds r3, r3, r1
	str r3, [r2, #24]
	ldr r3, [r2, #28]
	adds r3, r3, r1
	str r3, [r2, #28]
	movs r2, #192
	lsls r2, r2, #5
	adds r2, #174
	adds r3, r0, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #8
	bne .L_080e5d3c
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	strh r3, [r4]
.L_080e5d3c:
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #174
	adds r2, r0, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	pop {pc}
.L_080e5d4c:
	.4byte 0xfffd8000
.L_080e5d50:
	.4byte 0xfffffc00
