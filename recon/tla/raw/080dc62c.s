.syntax unified
	.thumb
	.global Func_080dc62c
	.thumb_func
Func_080dc62c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r6, [r3]
	adds r5, r1, #0
	strh r0, [r6, #24]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl ObjectTable_Get
	strh r5, [r6, #26]
	lsls r5, r5, #16
	adds r7, r0, #0
	asrs r5, r5, #16
	str r7, [r6, #16]
	adds r0, r5, #0
	bl ObjectTable_Get
	ldrh r1, [r7, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r1, r2
	ldr r2, .L_080dc690
	str r0, [r6, #20]
	ands r3, r2
	strh r3, [r6]
	movs r3, #128
	lsls r3, r3, #5
	adds r1, r1, r3
	ldr r3, .L_080dc694
	ands r1, r3
	strh r1, [r6, #2]
	cmp r7, #0
	beq .L_080dc698
	adds r3, r7, #0
	adds r3, #35
	ldrb r2, [r3]
	movs r1, #227
	lsls r1, r1, #3
	adds r3, r6, r1
	strb r2, [r3]
	ldr r3, [r7, #80]
	adds r1, #1
	ldrb r3, [r3, #9]
	adds r2, r6, r1
	lsls r3, r3, #28
	lsrs r3, r3, #30
	strb r3, [r2]
	b .L_080dc698
.L_080dc690:
	.4byte 0xffffc000
.L_080dc694:
	.4byte 0xffffe000
.L_080dc698:
	cmp r0, #0
	beq .L_080dc6be
	ldr r3, [r0, #108]
	str r3, [r6, #56]
	ldr r3, [r0]
	str r3, [r6, #60]
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	ldrb r2, [r3, #5]
	adds r3, r6, #0
	adds r3, #64
	strb r2, [r3]
	ldr r3, [r0, #8]
	str r3, [r6, #4]
	ldr r3, [r0, #16]
	str r3, [r6, #12]
	ldr r3, [r0, #12]
	str r3, [r6, #8]
	b .L_080dc6d6
.L_080dc6be:
	ldr r3, [r7, #8]
	movs r0, #128
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	lsls r0, r0, #13
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	ldrh r1, [r6]
	str r3, [r6, #8]
	adds r2, r6, #4
	bl Func_0801489c
.L_080dc6d6:
	pop {r5, r6, r7, pc}
