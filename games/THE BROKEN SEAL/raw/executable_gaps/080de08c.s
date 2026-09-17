.syntax unified
.text
	.thumb
	.set sub_080de176, 0x080de176
	.global Fragment_080de08c
	.thumb_func
Fragment_080de08c:
	subs	r4, r5, #3
	lsls	r0, r0, #12
	.4byte 0x00007828
	lsls	r6, r1, #3
	movs	r0, r0
	.4byte 0x00000c56
	lsls	r4, r0, #3
	movs	r0, r0
	lsls	r3, r6, #1
	movs	r0, r0
	movs	r0, r3
	lsls	r1, r0, #8
	strb	r0, [r3, #2]
	movs	r0, r0
	strb	r4, [r0, #30]
	movs	r0, r0
	bcs sub_080de176
	lsrs	r4, r1, #32
	.4byte 0x03000168
	.4byte 0x10101010
	.4byte 0x000077a8
	.4byte 0x080eebd6
	.4byte 0x02010000
	.4byte 0x000001ff
	.4byte 0x00007fff
	.4byte 0xffffc000
