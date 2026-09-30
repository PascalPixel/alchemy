.syntax unified
	.thumb
	.global Func_080d5ff8
	.thumb_func
Func_080d5ff8:
	push {r5, lr}
	movs r0, #192
	lsls r0, r0, #18
	adds r3, r0, #0
	adds r3, #136
	ldr r1, [r3]
	movs r2, #240
	lsls r2, r2, #4
	adds r3, r1, r2
	ldrb r2, [r3]
	movs r5, #128
	lsls r3, r2, #4
	subs r3, r3, r2
	ldr r2, [r0, #108]
	lsls r3, r3, #7
	lsls r5, r5, #19
	adds r4, r1, r3
	adds r5, #20
	cmp r2, #0
	beq .L_080d6040
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080d6076
	movs r1, #178
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_080d6076
.L_080d6040:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r1, [r3, #10]
	movs r2, #197
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
	adds r1, r5, #0
	ldrh r2, [r3, #10]
	ldmia r4!, {r2}
	str r2, [r5]
	ldmia r4!, {r2}
	str r2, [r5]
	ldmia r4!, {r2}
	str r2, [r5]
	adds r0, r4, #0
	ldr r2, .L_080d6078
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_080d6076:
	pop {r5, pc}
.L_080d6078:
	.4byte 0xa6600003
