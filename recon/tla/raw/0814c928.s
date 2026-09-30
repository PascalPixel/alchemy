.syntax unified
	.thumb
	.global Func_0814c928
	.thumb_func
Func_0814c928:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #92]
	ldr r1, [r3, #48]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #176
	adds r0, r2, r3
	ldr r3, [r0]
	cmp r3, #1
	bne .L_0814c956
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #172
	adds r3, r2, r4
	ldr r2, [r3]
	ldrh r3, [r1, #54]
	adds r3, r3, r2
	movs r2, #0
	strh r3, [r1, #54]
	str r2, [r0]
	b .L_0814c97a
.L_0814c956:
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #172
	adds r3, r2, r4
	ldr r2, [r3]
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldrh r3, [r1, #54]
	asrs r2, r2, #1
	adds r3, r3, r2
	strh r3, [r1, #54]
	ldr r3, [r0]
	cmp r3, #2
	bne .L_0814c976
	movs r3, #0
	b .L_0814c978
.L_0814c976:
	movs r3, #2
.L_0814c978:
	str r3, [r0]
.L_0814c97a:
	pop {pc}
