#!/usr/bin/env bash
# usage: cmpov.sh <overlay> <offsetHex> <span> <draft.c>
cd /home/user/alchemy
/home/user/alchemy-gcc/dist/xgcc -B/home/user/alchemy-gcc/dist/ -O2 -mthumb -mthumb-interwork \
  -mcpu=arm7tdmi -fno-builtin -nostdinc -ffreestanding -fcall-used-r4 -Iinclude ${EXTRA_CFLAGS:-} -S "$4" -o /tmp/m.s 2>/dev/null
grep -E "^	[a-z]" /tmp/m.s | sed 's/\t/ /g;s/^ *//' > /tmp/mine.txt
bun tools/overlay_show.ts "$1" "$2" -n "$3" 2>/dev/null | sed 's/^ *[0-9a-f]*:\t[0-9a-f ]*\t//' \
  | grep -v "^--- pool\|^  0x" | sed 's/\t/ /g;s/  *@.*//' > /tmp/refall.txt
python3 - "${5:-8}" <<'PY'
import re,sys
r=[l.strip() for l in open('/tmp/refall.txt') if l.strip()]
m=[l.strip() for l in open('/tmp/mine.txt') if l.strip()]
r=r[:len(m)+6]
ALU=('and','orr','eor','add','sub','lsl','lsr','asr','mul','bic','adc','sbc','ror')
def n(s):
    s=re.sub(r'\b(movs|adds|subs|lsls|lsrs|asrs|ands|orrs|eors|negs|muls|bics)\b',lambda x:x.group(1)[:-1],s)
    s=re.sub(r'\.n\b','',s); s=re.sub(r'@.*$','',s)
    s=re.sub(r'(bl|b|beq|bne|bgt|ble|blt|bge|bhi|bls|bcc|bcs|bmi|bpl) [^,]+$',r'\1 X',s)
    s=re.sub(r'sub sp, sp,','sub sp,',s); s=re.sub(r'add sp, sp,','add sp,',s)
    s=re.sub(r'ldr (r\d+), \.L\d+(\+\d+)?',r'ldr \1, POOL',s)
    s=re.sub(r'ldr (r\d+), \[pc, #\d+\]',r'ldr \1, POOL',s)
    # objdump renders Thumb `mov rd,rn` as `adds rd,rn,#0`; unify to mov.
    s=re.sub(r'^add (r\d+), (r\d+), #0$',r'mov \1, \2',s.strip())
    s=re.sub(r'\[(r\d+|sp), #0\]',r'[\1]',s)
    # unify 2-operand ALU into the 3-operand spelling gcc prints
    m=re.match(r'^(%s) (r\d+), (.+)$'%'|'.join(ALU),s)
    if m and ',' not in m.group(3):
        s='%s %s, %s, %s'%(m.group(1),m.group(2),m.group(2),m.group(3))
    return re.sub(r'\s+',' ',s).strip()
d=[];
for i in range(max(len(r),len(m))):
    a=n(r[i]) if i<len(r) else '---'; b=n(m[i]) if i<len(m) else '---'
    if a!=b: d.append((i+1,a,b))
print('ref=%d mine=%d groups_differing=%d'%(len(r),len(m),len(d)))
for i,a,b in d[:int(sys.argv[1] if len(sys.argv)>1 else 8)]: print('%4d  %-30s | %s'%(i,a,b))
PY
