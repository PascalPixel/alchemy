.syntax unified
	.thumb
	.global Func_0811a720
	.thumb_func
Func_0811a720:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r0, [r5]
	adds r7, r1, #0
	cmp r0, #0
	bne .L_0811a73a
	movs r0, #240
	lsls r0, r0, #8
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Func_080200c0
.L_0811a73a:
	adds r6, r0, #0
	movs r3, #230
	lsls r3, r3, #8
	adds r3, #102
	movs r2, #0
	str r3, [r5, #24]
	str r6, [r5]
	cmp r7, #0
	bne .L_0811a756
	str r2, [r5, #16]
	str r2, [r5, #12]
	str r2, [r6, #8]
	ldr r3, [r5, #16]
	str r3, [r6, #16]
.L_0811a756:
	ldr r3, .L_0811a794
	str r2, [r5, #20]
	strh r2, [r5, #4]
	strh r2, [r5, #6]
	strh r2, [r5, #8]
	strh r2, [r5, #10]
	str r2, [r5, #32]
	strh r2, [r5, #28]
	str r2, [r5, #36]
	strh r2, [r5, #40]
	adds r2, r5, #0
	adds r2, #42
	strb r3, [r5, #30]
	movs r1, #0
	strb r3, [r2]
	movs r0, #0
	bl ArcTan2
	movs r3, #128
	lsls r3, r3, #8
	adds r2, r6, #0
	adds r0, r0, r3
	adds r2, #89
	movs r3, #3
	strh r0, [r6, #6]
	strb r3, [r2]
	subs r2, #4
	movs r3, #2
	strb r3, [r2]
	adds r2, r5, #0
	b .L_0811a798
.L_0811a794:
	.4byte 0x00000000
.L_0811a798:
	adds r2, #43
	movs r3, #1
	strb r3, [r2]
	ldr r1, .L_0811a7a8
	adds r0, r6, #0
	bl Object_SetCallback
	pop {r5, r6, r7, pc}
.L_0811a7a8:
	.4byte Data_0812cacc
