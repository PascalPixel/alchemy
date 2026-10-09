@ C2: Unpaired live smull halves r2/r0; add ip,#1 and bx ip return through a
@ non-procedure-standard register.
.syntax unified
	.arm

	.global IwramMulQ16ReturnIp
IwramMulQ16ReturnIp:
	smull	r2, r0, r1, r0
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	add	ip, ip, #1
	bx	ip
