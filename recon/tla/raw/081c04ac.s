.syntax unified
	.thumb
	.global Func_081c04ac
	.thumb_func
Func_081c04ac:
	adds r2, r0, #0
	lsls r1, r1, #16
	lsrs r1, r1, #16
	ldr r3, [r2, #52]
	ldr r0, .L_081c04c8
	cmp r3, r0
	bne .L_081c04c4
	strh r1, [r2, #38]
	strh r1, [r2, #36]
	movs r0, #128
	lsls r0, r0, #1
	strh r0, [r2, #40]
.L_081c04c4:
	bx lr
	.2byte 0x0000
.L_081c04c8:
	.4byte 0x68736d53
	.4byte 0x4814b570
	.4byte 0x42492102
	.4byte 0x49134008
	.4byte 0xf7ff4a13
	.4byte 0x4813fffe
