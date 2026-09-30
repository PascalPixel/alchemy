.syntax unified
	.thumb
	.global Func_080d20a4
	.thumb_func
Func_080d20a4:
	push {lr}
	movs r3, #26
	muls r3, r1
	adds r3, r3, r0
	ldr r0, .L_080d20f8
	lsls r3, r3, #5
	adds r1, r3, r0
	lsrs r3, r2, #2
	lsls r3, r3, #1
	adds r1, r1, r3
	movs r3, #3
	ands r3, r2
	cmp r3, #1
	beq .L_080d20da
	cmp r3, #1
	bcc .L_080d20ce
	cmp r3, #2
	beq .L_080d20e4
	cmp r3, #3
	beq .L_080d20ee
	b .L_080d20f2
.L_080d20ce:
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #240
	ands r3, r2
	b .L_080d20f0
.L_080d20da:
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	ands r3, r2
	b .L_080d20f0
.L_080d20e4:
	ldrh r2, [r1]
	movs r3, #240
	lsls r3, r3, #8
	ands r3, r2
	b .L_080d20f0
.L_080d20ee:
	ldr r3, .L_080d20f4
.L_080d20f0:
	strh r3, [r1]
.L_080d20f2:
	pop {pc}
.L_080d20f4:
	.4byte 0x00000000
.L_080d20f8:
	.4byte 0x06000200
