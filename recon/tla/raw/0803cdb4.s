.syntax unified
	.thumb
	.global Func_0803cdb4
	.thumb_func
Func_0803cdb4:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #60]
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #137
	adds r3, r6, r2
	ldrb r3, [r3]
	adds r5, r0, #0
	movs r7, #0
	cmp r3, #0
	beq .L_0803cdd8
	bl Audio_Check
	cmp r0, #0
	bne .L_0803cdd8
	movs r7, #1
.L_0803cdd8:
	ldr r1, .L_0803ce18
	ldrb r3, [r6, #4]
	ldr r2, [r1, #4]
	cmp r3, #0
	beq .L_0803cde4
	ldr r2, [r1, #28]
.L_0803cde4:
	movs r3, #129
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r2
	cmp r3, #0
	beq .L_0803cdf2
	movs r7, #1
.L_0803cdf2:
	ldr r3, [r1]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0803ce06
	ldrh r3, [r5, #20]
	cmp r3, #1
	bne .L_0803ce08
	movs r7, #1
	b .L_0803ce08
.L_0803ce06:
	strh r2, [r5, #20]
.L_0803ce08:
	cmp r7, #0
	beq .L_0803ce14
	movs r3, #0
	strh r3, [r5, #20]
	movs r0, #1
	b .L_0803ce16
.L_0803ce14:
	movs r0, #0
.L_0803ce16:
	pop {r5, r6, r7, pc}
.L_0803ce18:
	.4byte gInput
