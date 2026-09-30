.syntax unified
	.thumb
	.global Func_08143d80
	.thumb_func
Func_08143d80:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #96]
	ldr r5, [r3, #36]
	adds r3, #176
	ldr r6, [r3]
	mov r8, r2
	bl Func_081434d8
	movs r3, #206
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrh r1, [r5]
	movs r0, #2
	movs r2, #0
	bl Func_08118028 + 0x10
	ldr r2, .L_08143df0
	movs r1, #0
	movs r3, #32
	strh r3, [r2, #6]
	str r1, [r6, #12]
	ldr r3, .L_08143df4
	movs r1, #64
	ldr r0, .L_08143df8
	mov lr, r3
	.2byte 0xf800
	movs r1, #128
	movs r2, #1
	negs r2, r2
	ldr r3, .L_08143dfc
	ldr r0, .L_08143e00
	lsls r1, r1, #2
	mov lr, r3
	.2byte 0xf800
	ldr r5, .L_08143e04
	movs r2, #128
	ldr r7, .L_08143dec
	lsls r2, r2, #1
	movs r0, #0
	movs r6, #0
	mov r12, r2
	movs r4, #0
.L_08143ddc:
	mov r3, r12
	movs r1, #0
	adds r2, r4, r3
.L_08143de2:
	cmp r1, #15
	ble .L_08143e08
	adds r3, r0, r5
	strh r7, [r3]
	b .L_08143e0c
.L_08143dec:
	.4byte 0x000000ff
.L_08143df0:
	.4byte Data_03001120
.L_08143df4:
	.4byte IwramClearWords
.L_08143df8:
	.4byte 0x06003fc0
.L_08143dfc:
	.4byte IwramFillWords
.L_08143e00:
	.4byte 0x0600f900
.L_08143e04:
	.4byte 0x0600fb00
.L_08143e08:
	adds r3, r0, r5
	strh r2, [r3]
.L_08143e0c:
	adds r1, #1
	adds r2, #1
	adds r0, #2
	cmp r1, #32
	bne .L_08143de2
	adds r6, #1
	adds r4, #16
	cmp r6, #16
	bne .L_08143ddc
	ldr r3, .L_08143e54
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_08143e58
	adds r2, #10
	strh r3, [r2]
	ldr r3, .L_08143e5c
	adds r2, #70
	strh r3, [r2]
	ldr r1, .L_08143e60
	movs r3, #128
	ldr r2, .L_08143e64
	lsls r3, r3, #19
	adds r3, #64
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r3, .L_08143e68
	movs r2, #128
	lsls r2, r2, #19
	b .L_08143e6c
	.2byte 0x0000
.L_08143e54:
	.4byte 0x00007741
.L_08143e58:
	.4byte 0x00001f81
.L_08143e5c:
	.4byte 0x00003f42
.L_08143e60:
	.4byte 0x000000f0
.L_08143e64:
	.4byte 0x00001088
.L_08143e68:
	.4byte 0x00003537
.L_08143e6c:
	adds r2, #72
	strh r3, [r2]
	ldr r3, .L_08143e9c
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_08143ea0
	adds r2, #8
	movs r1, #128
	strh r3, [r2]
	ldr r5, .L_08143ea4
	mov r0, r8
	lsls r1, r1, #7
	mov lr, r5
	.2byte 0xf800
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, .L_08143ea8
	mov lr, r5
	.2byte 0xf800
	movs r0, #1
	bl WaitFrames
	b .L_08143eac
	.2byte 0x0000
.L_08143e9c:
	.4byte 0x00003f21
.L_08143ea0:
	.4byte 0x0000100e
.L_08143ea4:
	.4byte IwramClearWords
.L_08143ea8:
	.4byte 0x06004000
.L_08143eac:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
