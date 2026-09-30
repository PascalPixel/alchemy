.syntax unified
	.thumb
	.global Func_081280d8
	.thumb_func
Func_081280d8:
	push {lr}
	movs r3, #193
	lsls r3, r3, #1
	cmp r0, r3
	bls .L_081280e8
	ldr r3, .L_081280f8
	ldrh r0, [r3]
	b .L_081280f4
.L_081280e8:
	ldr r3, .L_081280f8
	lsls r2, r0, #3
	adds r2, r2, r3
	ldrb r0, [r2, #3]
	lsls r0, r0, #27
	lsrs r0, r0, #28
.L_081280f4:
	pop {pc}
	.2byte 0x0000
.L_081280f8:
	.4byte Summon_EntryTable
