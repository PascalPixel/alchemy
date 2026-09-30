.syntax unified
	.thumb
	.global Func_080138b4
	.thumb_func
Func_080138b4:
	push {r5, lr}
	ldr r2, .L_08013998
	movs r5, #0
	ldr r4, [r2]
	ldr r3, [r2, #32]
	cmp r3, #0
	bgt .L_080138d6
	adds r1, r4, #0
	str r4, [r2, #12]
	cmp r3, #0
	bne .L_080138d0
	movs r3, #6
	str r3, [r2, #32]
	b .L_080138da
.L_080138d0:
	movs r3, #19
	str r3, [r2, #32]
	b .L_080138da
.L_080138d6:
	movs r1, #0
	str r1, [r2, #12]
.L_080138da:
	cmp r1, #0
	beq .L_08013984
	movs r3, #64
	ands r3, r1
	movs r2, #0
	cmp r3, #0
	beq .L_080138ea
	movs r2, #1
.L_080138ea:
	movs r3, #128
	ands r3, r1
	cmp r3, #0
	beq .L_080138f4
	adds r2, #1
.L_080138f4:
	movs r3, #32
	ands r3, r1
	cmp r3, #0
	beq .L_080138fe
	adds r2, #1
.L_080138fe:
	movs r3, #16
	ands r3, r1
	cmp r3, #0
	beq .L_08013908
	adds r2, #1
.L_08013908:
	ldr r0, .L_08013998
	str r1, [r0, #16]
	cmp r2, #1
	beq .L_08013930
	cmp r2, #1
	bcc .L_0801392a
	cmp r2, #2
	beq .L_08013938
	cmp r2, #3
	beq .L_08013958
	movs r3, #48
	str r3, [r0, #20]
	movs r2, #255
	ldr r3, [r0, #16]
	lsls r2, r2, #8
	adds r2, #15
	b .L_0801397e
.L_0801392a:
	movs r3, #48
	str r3, [r0, #20]
	b .L_08013988
.L_08013930:
	movs r3, #240
	ands r1, r3
	str r1, [r0, #20]
	b .L_08013988
.L_08013938:
	ldr r3, [r0, #20]
	ldr r2, [r0, #16]
	ands r3, r2
	cmp r3, #0
	bne .L_08013946
	movs r3, #48
	str r3, [r0, #20]
.L_08013946:
	ldr r3, [r0, #20]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	eors r3, r2
	ldr r2, [r0, #16]
	ands r2, r3
	str r2, [r0, #16]
	b .L_08013988
.L_08013958:
	ldr r3, [r0, #20]
	movs r2, #48
	ands r3, r2
	cmp r3, #0
	beq .L_08013964
	movs r5, #48
.L_08013964:
	ldr r3, [r0, #20]
	movs r2, #192
	ands r3, r2
	cmp r3, #0
	beq .L_08013970
	movs r5, #192
.L_08013970:
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	eors r2, r5
	ands r1, r2
	str r1, [r0, #20]
	ldr r3, [r0, #16]
.L_0801397e:
	ands r3, r2
	str r3, [r0, #16]
	b .L_08013988
.L_08013984:
	ldr r3, .L_08013998
	str r1, [r3, #16]
.L_08013988:
	ldr r3, .L_08013998
	adds r2, r4, #0
	ldr r1, [r3, #24]
	bics r2, r1
	str r2, [r3, #4]
	str r4, [r3, #24]
	pop {r5, pc}
	.2byte 0x0000
.L_08013998:
	.4byte gInput
