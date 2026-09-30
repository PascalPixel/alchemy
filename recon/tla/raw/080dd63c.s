.syntax unified
	.thumb
	.global BattleFx_SnapScaleToFull
	.thumb_func
BattleFx_SnapScaleToFull:
	push {lr}
	cmp r0, #0
	beq .L_080dd664
	movs r3, #255
	ldr r2, [r0, #24]
	lsls r3, r3, #8
	adds r3, #255
	cmp r2, r3
	bgt .L_080dd660
	movs r1, #128
	lsls r1, r1, #5
	mov r12, r3
.L_080dd654:
	adds r3, r2, r1
	adds r2, r3, #0
	cmp r3, r12
	ble .L_080dd654
	str r3, [r0, #24]
	str r3, [r0, #28]
.L_080dd660:
	bl Object_CommitPosition
.L_080dd664:
	pop {pc}
	.2byte 0x0000
