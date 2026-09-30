.syntax unified
	.thumb
	.global UiText_MeasureEntryDimensions
	.thumb_func
UiText_MeasureEntryDimensions:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	str r1, [sp, #16]
	str r2, [sp, #12]
	adds r7, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #60]
	ldr r3, .L_0803bad0
	movs r2, #15
	mov r12, r3
	mov r3, sp
	adds r3, #20
	mov r9, r2
	str r3, [sp, #8]
	movs r2, #28
	movs r4, #0
	movs r1, #0
	add r2, sp
	mov r11, r4
	movs r6, #0
	mov r10, r2
	mov r8, r1
.L_0803b952:
	movs r2, #244
	lsls r3, r0, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r2, [r5, r3]
	movs r3, #128
	lsls r3, r3, #1
	adds r0, #1
	adds r3, #255
	ands r0, r3
	cmp r2, #31
	bls .L_0803b996
	cmp r2, #176
	beq .L_0803b996
	cmp r2, #32
	bne .L_0803b978
	adds r6, #5
	adds r1, #1
	b .L_0803b952
.L_0803b978:
	ldr r3, .L_0803bad4
	subs r2, #32
	lsls r2, r2, #5
	ldrh r2, [r3, r2]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #60
	ldrh r3, [r3, r5]
	cmp r3, #1
	beq .L_0803b990
	cmp r3, #5
	bne .L_0803b992
.L_0803b990:
	adds r2, #1
.L_0803b992:
	adds r6, r6, r2
	b .L_0803b952
.L_0803b996:
	cmp r2, #28
	bhi .L_0803b952
	lsls r3, r2, #2
	mov r2, r12
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0803b9a4:
	.2byte 0xba90
	.2byte 0x0803
	.2byte 0xba48
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xba18
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xba60
	.2byte 0x0803
	.2byte 0xba6c
	.2byte 0x0803
	.2byte 0xba60
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xba56
	.2byte 0x0803
	.2byte 0xba60
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xba60
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xb952
	.2byte 0x0803
	.2byte 0xba56
	.2byte 0x0803
	.2byte 0x4653
	.2byte 0x4642
	.2byte 0x3101
	.2byte 0x5299
	.2byte 0x9b02
	.2byte 0x529e
	.2byte 0x42b4
	.2byte 0xd200
	.2byte 0x1c34
	.2byte 0x465b
	.2byte 0x2b02
	.2byte 0xd804
	.2byte 0x2201
	.2byte 0x4493
	.2byte 0x465b
	.2byte 0x005b
	.2byte 0x4698
	.2byte 0x4b25
	.2byte 0x220f
	.2byte 0x2100
	.2byte 0x2600
	.2byte 0x4491
	.2byte 0x469c
	.2byte 0xe784
	.2byte 0x9101
	.2byte 0x9400
	.2byte 0xf7fe
	.2byte 0xfcda
	.2byte 0x9901
	.2byte 0x9c00
	.2byte 0xe01c
	.2byte 0x2380
	.2byte 0x005b
	.2byte 0x3001
	.2byte 0x33ff
	.2byte 0x4018
	.2byte 0x2380
	.2byte 0x005b
	.2byte 0x3001
	.2byte 0x33ff
	.2byte 0x4018
	.2byte 0xe772
	.2byte 0x22f4
	.2byte 0x0043
	.2byte 0x0112
	.2byte 0x189b
	.2byte 0x5aea
	.2byte 0x23f0
	.2byte 0x011b
	.2byte 0x333c
	.2byte 0x195b
	.2byte 0x801a
	.2byte 0x2380
	.2byte 0x4a13
	.2byte 0x005b
	.2byte 0x3001
	.2byte 0x33ff
	.2byte 0x4018
	.2byte 0x4694
	.2byte 0xe760
	.2byte 0x4653
	.2byte 0x4642
	.2byte 0x3101
	.2byte 0x5299
	.2byte 0x9b02
	.2byte 0x529e
	.2byte 0x42b4
	.2byte 0xd200
	.2byte 0x1c34
	.2byte 0x792b
	.2byte 0x2b00
	.2byte 0xd000
	.2byte 0x3402
	.2byte 0x2298
	.2byte 0x0152
	.2byte 0x328c
	.2byte 0x18ab
	.2byte 0x781b
	.2byte 0x061b
	.2byte 0x161b
	.2byte 0x2b00
	.2byte 0xd00f
	.2byte 0x464b
	.2byte 0x2b2c
	.2byte 0xd90a
	.2byte 0x4b02
	.2byte 0x2220
	.2byte 0x80fb
	.2byte 0x4691
	.2byte 0xe007
	.2byte 0x0001
	.2byte 0x0000
.L_0803bad0:
	.4byte .L_0803b9a4
.L_0803bad4:
	.4byte UiText_Glyphs
