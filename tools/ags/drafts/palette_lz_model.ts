// DRAFT: a TypeScript port of ags lz.rs palette LZSS with end-of-stream rule variants, used to
// test why Title_IntroGraphicsC re-encodes differently in its last 216 bytes (see lz.rs).
// Port of ags lz.rs palette lzss with end-rule variants; compares token streams with a ROM stream.
export type Tok = [number, number]; // [length, distance]; length 1 = literal
export function tokensOf(d: Uint8Array): Tok[] {
  const out: Tok[] = []; let c = 0;
  for (;;) {
    const f = d[c++];
    if (f === 0) { for (let i = 0; i < 8; i++) out.push([1, 0]); c += 8; continue; }
    for (let bit = 7; bit >= 0; bit--) {
      if (!(f & (1 << bit))) { out.push([1, 0]); c++; continue; }
      const a = d[c++], b = d[c++]; const dist = ((a & 0xf0) << 4) | b; let len = a & 15;
      if (len === 0) { if (dist === 0) return out; len = d[c++] + 17; } else len++;
      out.push([len, dist]);
    }
  }
}
export function model(data: Uint8Array, variant: (ctx: any) => any): Tok[] {
  const n = data.length, WIN = 4092, RA = 272, MAXD = 4095, LONG = 272;
  const pairs = new Map<number, number[]>();
  for (let i = 0; i + 1 < n; i++) { const k = data[i] | (data[i + 1] << 8); let a = pairs.get(k); if (!a) pairs.set(k, (a = [])); a.push(i); }
  const lb = (arr: number[], v: number) => { let lo = 0, hi = arr.length; while (lo < hi) { const m = (lo + hi) >> 1; if (arr[m] < v) lo = m + 1; else hi = m; } return lo; };
  const search = (pos: number, longest: number, lowest: number): Tok => {
    const max = Math.min(longest, n - pos); let length = 1, dist = 0;
    if (pos + 1 < n) {
      const arr = pairs.get(data[pos] | (data[pos + 1] << 8));
      if (arr) {
        const s = lb(arr, lowest), e = lb(arr, pos);
        for (let j = e - 1; j >= s; j--) {
          const src = arr[j]; if (hidden[src]) continue;
          if (data[src + length] !== data[pos + length]) continue;
          let c = 2; while (c < max && pos + c < n && data[src + c] === data[pos + c]) c++;
          if (c > length) { length = c; dist = pos - src; }
          if (length === max) break;
        }
      }
    }
    return length >= 2 ? [length, dist] : [1, 0];
  };
  const oldestAt = (p: number) => Math.max(0, Math.min(p + RA, n) - (WIN + RA));
  const lowest = (old: number, at: number) => Math.max(old, at - MAXD);
  let deferred = false, pos = 0; const out: Tok[] = []; const hidden = new Uint8Array(n); const M = (variant as any).M ?? 1e9; const skip = (variant as any).skip ?? 1;
  while (pos < n) {
    const old = oldestAt(pos);
    let tok = search(pos, LONG, lowest(old, pos)); const count = tok[0];
    let lazy = !deferred; deferred = false;
    const v = variant({ pos, n, count, lazy });
    lazy = v.lazy ?? lazy;
    if (lazy && count > 1 && pos + count < n) {
      const alt = search(pos + 1, LONG, lowest(old, pos + 1))[0];
      let fol = search(pos + count, LONG, v.folLowest ? Math.max(lowest(old, pos + count), v.folLowest) : lowest(old, pos + count))[0];
      if (v.following !== undefined) fol = v.following(fol);
      if (alt > 2 && alt + 1 >= count + fol) { tok = [1, 0]; deferred = true; }
    }
    if (tok[0] >= M) for (let q = pos + skip; q < pos + tok[0]; q++) hidden[q] = 1; pos += tok[0]; out.push(tok);
  }
  return out;
}
