.syntax unified
	.thumb
	.global Func_0811b4ac
	.thumb_func
Func_0811b4ac:
	push {lr}
	cmp r1, #7
	ble .L_0811b4b4
	adds r1, #120
.L_0811b4b4:
	movs r2, #0
.L_0811b4b6:
	movs r4, #0
	ldrsh r3, [r0, r4]
	adds r0, #2
	lsls r3, r3, #16
	lsrs r3, r3, #16
	cmp r3, #255
	beq .L_0811b4d2
	cmp r3, r1
	bne .L_0811b4cc
	movs r0, #1
	b .L_0811b4d4
.L_0811b4cc:
	adds r2, #1
	cmp r2, #13
	bls .L_0811b4b6
.L_0811b4d2:
	movs r0, #0
.L_0811b4d4:
	pop {pc}
	.2byte 0x0000
