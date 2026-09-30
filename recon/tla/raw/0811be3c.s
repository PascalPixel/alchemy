.syntax unified
	.thumb
	.global GetBattleObjectSlot
	.thumb_func
GetBattleObjectSlot:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #36]
	adds r1, r2, #0
	adds r1, #132
	cmp r0, #7
	ble .L_0811be4e
	subs r0, #120
.L_0811be4e:
	adds r0, #116
	ldrb r3, [r2, r0]
	cmp r3, #255
	bne .L_0811be5a
	movs r0, #0
	b .L_0811be64
.L_0811be5a:
	ldrb r2, [r2, r0]
	movs r3, #44
	adds r0, r2, #0
	muls r0, r3
	adds r0, r1, r0
.L_0811be64:
	pop {pc}
	.2byte 0x0000
