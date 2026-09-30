.syntax unified
	.thumb
	.global Owner_GetLevelThreshold
	.thumb_func
Owner_GetLevelThreshold:
	push {r5, lr}
	adds r5, r1, #0
	bl Owner_GetState
	movs r1, #42
	adds r2, r0, #0
	adds r1, #255
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080af912
	movs r0, #0
	cmp r5, #0
	ble .L_080af916
	cmp r5, #99
	bgt .L_080af912
	movs r3, #165
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	cmp r3, #7
	bhi .L_080af912
	adds r2, r3, #0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #5
	adds r3, r3, r2
	ldr r1, .L_080af918
	adds r3, r3, r5
	lsls r3, r3, #2
	subs r3, #4
	ldr r0, [r1, r3]
	b .L_080af916
.L_080af912:
	movs r0, #1
	negs r0, r0
.L_080af916:
	pop {r5, pc}
.L_080af918:
	.4byte Data_080b12c8
