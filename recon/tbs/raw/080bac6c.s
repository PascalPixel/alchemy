.syntax unified
	.thumb
	.global BattleActor_RemoveFromLists
	.thumb_func
BattleActor_RemoveFromLists:
	push {r5, r6, lr}
	ldr r3, .L_080bac98
	adds r6, r0, #0
	ldr r5, [r3]
	bl Func_08077008
	movs r3, #149
	lsls r3, r3, #1
	adds r2, r0, r3
	ldr r1, .L_080bac94
	movs r3, #0
	strb r3, [r2]
	movs r2, #88
	b .L_080bac8a
.L_080bac88:
	adds r2, #2
.L_080bac8a:
	ldrsh r3, [r2, r5]
	cmp r3, r6
	bne .L_080bac9c
	strh r1, [r2, r5]
	b .L_080bacc4
.L_080bac94:
	.4byte 0x000000fe
.L_080bac98:
	.4byte Data_03001e74_a
.L_080bac9c:
	cmp r3, #255
	bne .L_080bac88
	movs r1, #0
	adds r0, r5, #2
.L_080baca4:
	lsls r3, r1, #1
	adds r2, r3, #0
	adds r2, #100
	ldrsh r3, [r0, r2]
	cmp r3, r6
	bne .L_080bacb6
	ldr r3, .L_080bacc0
	strh r3, [r0, r2]
	b .L_080bacc4
.L_080bacb6:
	adds r1, #1
	cmp r3, #255
	bne .L_080baca4
	b .L_080bace2
	.2byte 0x0000
.L_080bacc0:
	.4byte 0x000000fe
.L_080bacc4:
	adds r0, r6, #0
	bl Summon_ReleaseCharge
	movs r2, #187
	movs r1, #0
	movs r0, #255
	lsls r2, r2, #2
.L_080bacd2:
	ldrsh r3, [r2, r5]
	cmp r3, r6
	bne .L_080bacda
	strh r0, [r2, r5]
.L_080bacda:
	adds r1, #1
	adds r2, #16
	cmp r1, #19
	bls .L_080bacd2
.L_080bace2:
	pop {r5, r6}
	pop {r0}
	bx r0
