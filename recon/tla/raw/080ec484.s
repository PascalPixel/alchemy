.syntax unified
	.thumb
	.global Func_080ec484
	.thumb_func
Func_080ec484:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r5, r2, #0
	cmp r0, #0
	beq .L_080ec4ce
	ldr r4, [r0]
	cmp r4, #0
	beq .L_080ec4ce
	movs r3, #128
	lsls r3, r3, #1
	ldr r7, .L_080ec4d0
	adds r3, #255
	mov r12, r3
.L_080ec49e:
	ldrh r3, [r4, #6]
	adds r1, r4, #0
	adds r3, r3, r6
	strh r3, [r4, #6]
	ldrh r3, [r4, #8]
	adds r1, #16
	adds r3, r3, r5
	strh r3, [r4, #8]
	mov r3, r12
	ldrh r0, [r1, #6]
	ldr r4, [r4]
	lsls r2, r0, #23
	lsrs r2, r2, #23
	adds r2, r2, r6
	ands r2, r3
	adds r3, r7, #0
	ands r3, r0
	orrs r3, r2
	strh r3, [r1, #6]
	ldrb r3, [r1, #4]
	adds r3, r3, r5
	strb r3, [r1, #4]
	cmp r4, #0
	bne .L_080ec49e
.L_080ec4ce:
	pop {r5, r6, r7, pc}
.L_080ec4d0:
	.4byte 0xfffffe00
