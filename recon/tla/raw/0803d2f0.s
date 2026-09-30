.syntax unified
	.thumb
	.global Localization_LookupEntryId
	.thumb_func
Localization_LookupEntryId:
	push {r5, lr}
	adds r3, r0, #0
	movs r4, #1
	subs r3, #47
	negs r4, r4
	movs r1, #0
	cmp r3, #7
	bhi .L_0803d346
	ldr r2, .L_0803d3b4
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0803d308:
	.4byte .L_0803d328
	.4byte .L_0803d330
	.4byte .L_0803d32c
	.4byte .L_0803d334
	.4byte .L_0803d338
	.4byte .L_0803d33c
	.4byte .L_0803d340
	.4byte .L_0803d344
.L_0803d328:
	movs r0, #4
	b .L_0803d346
.L_0803d32c:
	movs r0, #6
	b .L_0803d346
.L_0803d330:
	movs r0, #5
	b .L_0803d346
.L_0803d334:
	movs r0, #7
	b .L_0803d346
.L_0803d338:
	movs r0, #0
	b .L_0803d346
.L_0803d33c:
	movs r0, #1
	b .L_0803d346
.L_0803d340:
	movs r0, #2
	b .L_0803d346
.L_0803d344:
	movs r0, #3
.L_0803d346:
	cmp r0, #15
	bhi .L_0803d37c
	ldr r2, .L_0803d3b8
	movs r5, #0
	ldrsh r3, [r2, r5]
	movs r5, #1
	negs r5, r5
	cmp r3, r5
	beq .L_0803d3ae
	cmp r3, r0
	bne .L_0803d362
	movs r1, #2
	ldrsh r4, [r2, r1]
	b .L_0803d3ae
.L_0803d362:
	adds r1, #2
	lsls r3, r1, #1
	ldrsh r3, [r2, r3]
	movs r5, #1
	negs r5, r5
	cmp r3, r5
	beq .L_0803d3ae
	cmp r3, r0
	bne .L_0803d362
	adds r1, #1
	lsls r3, r1, #1
	ldrsh r4, [r2, r3]
	b .L_0803d3ae
.L_0803d37c:
	ldr r2, .L_0803d3bc
	movs r5, #0
	ldrsh r3, [r2, r5]
	movs r5, #1
	negs r5, r5
	cmp r3, r5
	beq .L_0803d3ae
	cmp r3, r0
	bne .L_0803d394
	movs r1, #2
	ldrsh r4, [r2, r1]
	b .L_0803d3ac
.L_0803d394:
	adds r1, #2
	lsls r3, r1, #1
	ldrsh r3, [r2, r3]
	movs r5, #1
	negs r5, r5
	cmp r3, r5
	beq .L_0803d3ae
	cmp r3, r0
	bne .L_0803d394
	adds r1, #1
	lsls r3, r1, #1
	ldrsh r4, [r2, r3]
.L_0803d3ac:
	adds r4, #128
.L_0803d3ae:
	adds r0, r4, #0
	pop {r5, pc}
	.2byte 0x0000
.L_0803d3b4:
	.4byte .L_0803d308
.L_0803d3b8:
	.4byte Data_0805eb58
.L_0803d3bc:
	.4byte Data_0805eb7c
