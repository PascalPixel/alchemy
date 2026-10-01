.syntax unified
	.thumb
	.global Func_080e1154
	.thumb_func
Func_080e1154:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #100
	movs r1, #0
	ldrsh r6, [r3, r1]
	adds r1, r5, #0
	adds r1, #102
	ldrh r3, [r1]
	adds r2, r3, #1
	lsls r3, r3, #16
	asrs r0, r3, #16
	ldr r3, .L_080e11b4
	strh r2, [r1]
	movs r2, #253
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_080e11b8
	cmp r2, r3
	bne .L_080e1192
	movs r1, #7
	bl Math_Mod
	cmp r0, #0
	bne .L_080e11a2
	adds r0, r5, #0
	bl Func_080e1024
	b .L_080e11a2
.L_080e1192:
	movs r1, #5
	bl Math_Mod
	cmp r0, #0
	bne .L_080e11a2
	adds r0, r5, #0
	bl Func_080e1024
.L_080e11a2:
	cmp r6, #1
	bne .L_080e11b0
	ldrh r3, [r5, #6]
	movs r2, #192
	lsls r2, r2, #4
	adds r3, r3, r2
	strh r3, [r5, #6]
.L_080e11b0:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080e11b4:
	.4byte gPartyState
.L_080e11b8:
	.4byte 0x00000001
