.syntax unified
	.thumb
	.global ArcTan2
	.thumb_func
ArcTan2:
	push {r5, r6, lr}
	adds r6, r0, #0
	adds r5, r1, #0
	movs r4, #0
	cmp r6, #0
	beq .L_08014994
	movs r4, #128
	lsls r4, r4, #7
	cmp r5, #0
	beq .L_08014994
	cmp r5, #0
	bge .L_08014902
	negs r1, r5
.L_08014902:
	adds r0, r6, #0
	cmp r6, #0
	bge .L_0801490a
	negs r0, r6
.L_0801490a:
	lsls r0, r0, #8
	ldr r3, .L_080149ac
	mov lr, r3
	.2byte 0xf800
	adds r1, r0, #0
	movs r0, #251
	lsls r0, r0, #8
	movs r4, #128
	adds r0, #106
	lsls r4, r4, #7
	cmp r1, r0
	bgt .L_08014994
	ldr r3, .L_080149b0
	movs r2, #128
	ldrh r0, [r3]
	movs r4, #0
	lsls r2, r2, #6
	subs r3, #128
	cmp r1, r0
	ble .L_0801493a
	movs r0, #128
	lsls r0, r0, #1
	adds r4, r2, #0
	adds r3, r3, r0
.L_0801493a:
	ldrh r0, [r3]
	lsrs r2, r2, #1
	subs r3, #64
	cmp r1, r0
	ble .L_08014948
	adds r4, r4, r2
	adds r3, #128
.L_08014948:
	ldrh r0, [r3]
	lsrs r2, r2, #1
	subs r3, #32
	cmp r1, r0
	ble .L_08014956
	adds r4, r4, r2
	adds r3, #64
.L_08014956:
	ldrh r0, [r3]
	lsrs r2, r2, #1
	subs r3, #16
	cmp r1, r0
	ble .L_08014964
	adds r4, r4, r2
	adds r3, #32
.L_08014964:
	ldrh r0, [r3]
	lsrs r2, r2, #1
	subs r3, #8
	cmp r1, r0
	ble .L_08014972
	adds r4, r4, r2
	adds r3, #16
.L_08014972:
	ldrh r0, [r3]
	lsrs r2, r2, #1
	subs r3, #4
	cmp r1, r0
	ble .L_08014980
	adds r4, r4, r2
	adds r3, #8
.L_08014980:
	ldrh r0, [r3]
	subs r3, #2
	cmp r1, r0
	ble .L_0801498c
	adds r4, #128
	adds r3, #4
.L_0801498c:
	ldrh r0, [r3]
	cmp r1, r0
	ble .L_08014994
	adds r4, #64
.L_08014994:
	cmp r5, #0
	bge .L_0801499e
	movs r3, #128
	lsls r3, r3, #8
	subs r4, r3, r4
.L_0801499e:
	cmp r6, #0
	bge .L_080149a4
	negs r4, r4
.L_080149a4:
	lsls r0, r4, #16
	lsrs r0, r0, #16
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080149ac:
	.4byte IwramUnsignedDivide
.L_080149b0:
	.4byte Data_080179ea
