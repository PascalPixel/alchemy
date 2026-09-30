.syntax unified
	.thumb
	.global Func_080b06e8
	.thumb_func
Func_080b06e8:
	push {r5, lr}
	adds r5, r1, #0
	movs r1, #42
	adds r2, r0, #0
	adds r1, #255
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080b0718
	adds r1, #33
	adds r3, r2, r1
	ldrh r0, [r3]
	bl Func_080ad3a8
	movs r2, #0
	adds r0, #62
.L_080b0708:
	ldrb r3, [r0]
	cmp r3, r5
	beq .L_080b0730
	adds r2, #1
	adds r0, #1
	cmp r2, #2
	ble .L_080b0708
	b .L_080b073a
.L_080b0718:
	movs r1, #42
	adds r1, #255
	adds r3, r2, r1
	ldrb r0, [r3]
	bl Owner_GetRecordStride84
	movs r2, #0
	adds r0, #80
.L_080b0728:
	ldrb r3, [r0]
	adds r0, #1
	cmp r3, r5
	bne .L_080b0734
.L_080b0730:
	movs r0, #1
	b .L_080b073c
.L_080b0734:
	adds r2, #1
	cmp r2, #2
	ble .L_080b0728
.L_080b073a:
	movs r0, #0
.L_080b073c:
	pop {r5, pc}
	.2byte 0x0000
