.syntax unified
	.thumb
	.global Func_0803fb60
	.thumb_func
Func_0803fb60:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r5, [r3]
	movs r1, #160
	movs r2, #160
	lsls r1, r1, #3
	lsls r2, r2, #3
	adds r1, #116
	adds r2, #164
	adds r3, r5, r1
	adds r0, r5, r2
	ldrh r6, [r3]
	bl Func_08108030
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #180
	adds r0, r5, r3
	bl Func_08108038
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #196
	adds r0, r5, r1
	bl Func_08108038
	cmp r6, #0
	bne .L_0803fbb0
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #124
	adds r3, r5, r2
	ldrh r2, [r3]
	movs r3, #7
	ands r3, r2
	ldr r2, .L_0803fc34
	adds r0, r3, r2
	b .L_0803fbb2
.L_0803fbb0:
	ldr r0, .L_0803fc34
.L_0803fbb2:
	bl Resource_GetTableEntry
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #180
	adds r3, r5, r1
	ldr r3, [r3]
	movs r1, #128
	adds r2, r0, #0
	lsls r1, r1, #1
	ldrb r0, [r3, #14]
	bl VramBlock_LoadCached
	cmp r6, #1
	bne .L_0803fbe4
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #124
	adds r3, r5, r2
	ldrh r2, [r3]
	movs r3, #7
	ands r3, r2
	ldr r2, .L_0803fc34
	adds r0, r3, r2
	b .L_0803fbe6
.L_0803fbe4:
	ldr r0, .L_0803fc34
.L_0803fbe6:
	bl Resource_GetTableEntry
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #196
	adds r3, r5, r1
	ldr r3, [r3]
	movs r1, #128
	adds r2, r0, #0
	lsls r1, r1, #1
	ldrb r0, [r3, #14]
	bl VramBlock_LoadCached
	cmp r6, #1
	ble .L_0803fc24
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #148
	adds r2, r6, r1
	ldrsb r2, [r5, r2]
	lsls r3, r6, #1
	adds r3, r3, r6
	adds r3, r3, r2
	movs r2, #160
	lsls r2, r2, #3
	lsls r3, r3, #2
	adds r2, #212
	adds r3, r3, r2
	ldr r0, [r5, r3]
	bl Func_080450fc
.L_0803fc24:
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #124
	adds r2, r5, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	pop {r5, r6, pc}
.L_0803fc34:
	.4byte 0x000001cd
