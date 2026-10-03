.syntax unified
	.thumb
	.global Func_080fabe0
	.thumb_func
Func_080fabe0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	ldr r3, .L_080fac34
	movs r2, #31
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_080fac54
	movs r7, #0
.L_080fabf8:
	asrs r5, r7, #24
	movs r2, #172
	lsls r2, r2, #1
	lsls r3, r5, #1
	adds r3, r3, r2
	ldrsh r0, [r6, r3]
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_080fac44
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrh r3, [r3]
	ldr r1, .L_080fac30
	ands r1, r3
	bl Item_CanOwnerEquip
	cmp r0, #0
	beq .L_080fac38
	lsls r3, r5, #2
	adds r3, #248
	ldr r0, [r6, r3]
	movs r1, #3
	bl Animation_ApplyChildArgumentFar
	b .L_080fac44
	.2byte 0x0000
.L_080fac30:
	.4byte 0x000001ff
.L_080fac34:
	.4byte gFrameCount
.L_080fac38:
	lsls r3, r5, #2
	adds r3, #248
	ldr r0, [r6, r3]
	movs r1, #1
	bl Animation_ApplyChildArgumentFar
.L_080fac44:
	movs r2, #128
	lsls r2, r2, #17
	adds r3, r7, r2
	movs r2, #192
	lsls r2, r2, #18
	adds r7, r3, #0
	cmp r3, r2
	ble .L_080fabf8
.L_080fac54:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
