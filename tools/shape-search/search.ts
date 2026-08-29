// Greedy source-shape search against the real scorer.
//
// The scorer costs about 0.08s when its release binary is invoked directly,
// so a few hundred legal respellings of an owner cost seconds, not minutes.
// This searches that space automatically and keeps only what lowers
// differing_halfwords.
// Generates ordinary-C variants of one owner and keeps whatever lowers
// differing_halfwords. No forced registers, no scheduling flags: every
// candidate is a legal respelling.
//
//   bun search.ts <owner-hex> [rounds]
const { execFileSync } = require("child_process");
const fs = require("fs");

const REPO = "/Users/pixel/Developer/PascalPixel/alchemy";
const owner = process.argv[2];
const rounds = Number(process.argv[3] ?? 4);
const file = `${REPO}/games/gs1/recon/en/main/${owner}.c`;
const scorer = `${REPO}/out/cargo-target/release/compiler`;

function score(): number {
  try {
    const out = execFileSync(
      scorer,
      [
        "candidate-show",
        `games/gs1/recon/en/main/${owner}.c`,
        "--owner",
        owner,
      ],
      { cwd: REPO, encoding: "utf8", stdio: ["ignore", "pipe", "pipe"] },
    );
    const m = out.match(/differing_halfwords=(\d+)/);
    return m ? Number(m[1]) : Infinity;
  } catch {
    return Infinity;
  }
}

// ---- variant generators -----------------------------------------------
type Variant = { label: string; text: string };

function bodyRange(src: string): [number, number] | null {
  const open = src.indexOf(
    "{",
    src.lastIndexOf("\n)") >= 0 ? src.lastIndexOf("\n)") : 0,
  );
  if (open < 0) return null;
  let depth = 0;
  for (let i = open; i < src.length; i++) {
    if (src[i] === "{") depth++;
    else if (src[i] === "}") {
      depth--;
      if (depth === 0) return [open + 1, i];
    }
  }
  return null;
}

/** Top-level statements inside the function body, as [start,end) spans. */
function statements(src: string, from: number, to: number): [number, number][] {
  const spans: [number, number][] = [];
  let depth = 0;
  let start = from;
  for (let i = from; i < to; i++) {
    const c = src[i];
    if (c === "{") depth++;
    else if (c === "}") {
      depth--;
      if (depth === 0) {
        spans.push([start, i + 1]);
        start = i + 1;
      }
    } else if (c === ";" && depth === 0) {
      spans.push([start, i + 1]);
      start = i + 1;
    }
  }
  return spans.filter(([a, b]) => src.slice(a, b).trim().length > 0);
}

function swaps(src: string): Variant[] {
  const range = bodyRange(src);
  if (!range) return [];
  const spans = statements(src, range[0], range[1]);
  const out: Variant[] = [];
  for (let i = 0; i + 1 < spans.length; i++) {
    const [a1, b1] = spans[i];
    const [a2, b2] = spans[i + 1];
    const first = src.slice(a1, b1);
    const second = src.slice(a2, b2);
    // never reorder across a declaration/statement boundary illegally in C89:
    // a declaration may not follow a statement.
    const isDecl = (t: string) =>
      /^\s*(?:const\s+)?(?:struct\s+\w+\s*\*?|unsigned\s+\w+|signed\s+\w+|[su]\d+|u8|void|int|char|short|long)\s+\**\w+/.test(
        t,
      );
    if (isDecl(second) && !isDecl(first)) continue;
    out.push({
      label: `swap@${i}`,
      text: src.slice(0, a1) + second + first + src.slice(b2),
    });
  }
  return out;
}

function retypes(src: string): Variant[] {
  const out: Variant[] = [];
  const pairs: [RegExp, string, string][] = [
    [/\bs32 (\w+);/g, "s32", "u32"],
    [/\bu32 (\w+);/g, "u32", "s32"],
    [/\bs16 (\w+);/g, "s16", "u16"],
    [/\bu16 (\w+);/g, "u16", "s16"],
    [/\bs32 (\w+);/g, "s32", "s16"],
  ];
  for (const [re, from, to] of pairs) {
    const names = [...src.matchAll(re)].map((m) => m[1]);
    for (const n of new Set(names)) {
      const decl = new RegExp(`\\b${from} ${n};`);
      out.push({
        label: `retype ${n}:${from}->${to}`,
        text: src.replace(decl, `${to} ${n};`),
      });
    }
  }
  return out;
}

function commutes(src: string): Variant[] {
  const out: Variant[] = [];
  const re =
    /([A-Za-z_][\w.\->\[\]]*) (\*|\+|\||&|\^) ([A-Za-z_][\w.\->\[\]]*)/g;
  let m: RegExpExecArray | null;
  const seen = new Set<string>();
  while ((m = re.exec(src))) {
    const whole = m[0];
    if (seen.has(whole)) continue;
    seen.add(whole);
    const flipped = `${m[3]} ${m[2]} ${m[1]}`;
    out.push({
      label: `commute ${whole}`,
      text: src.split(whole).join(flipped),
    });
  }
  return out;
}

function distantSwaps(src: string): Variant[] {
  const range = bodyRange(src);
  if (!range) return [];
  const spans = statements(src, range[0], range[1]);
  const isDecl = (t: string) =>
    /^\s*(?:const\s+)?(?:struct\s+\w+\s*\*?|unsigned\s+\w+|signed\s+\w+|[su]\d+|u8|void|int|char|short|long)\s+\**\w+/.test(
      t,
    );
  const out: Variant[] = [];
  for (let i = 0; i < spans.length; i++) {
    for (let j = i + 2; j < Math.min(spans.length, i + 6); j++) {
      const a = src.slice(spans[i][0], spans[i][1]);
      const b = src.slice(spans[j][0], spans[j][1]);
      if (isDecl(a) !== isDecl(b)) continue;
      const text =
        src.slice(0, spans[i][0]) +
        b +
        src.slice(spans[i][1], spans[j][0]) +
        a +
        src.slice(spans[j][1]);
      out.push({ label: `move@${i}<->${j}`, text });
    }
  }
  return out;
}

function guardForms(src: string): Variant[] {
  const out: Variant[] = [];
  // invert simple equality guards with an else arm
  const re =
    /if \(([^()]+?) == ([^()]+?)\) \{([\s\S]*?)\n(\s*)\} else \{([\s\S]*?)\n\4\}/g;
  let m: RegExpExecArray | null;
  while ((m = re.exec(src))) {
    const flipped = `if (${m[1]} != ${m[2]}) {${m[5]}\n${m[4]}} else {${m[3]}\n${m[4]}}`;
    out.push({ label: "invert if/else", text: src.replace(m[0], flipped) });
  }
  // relational mirror
  for (const [a, b] of [
    ["<", ">"],
    [">", "<"],
    ["<=", ">="],
    [">=", "<="],
  ]) {
    const rel = new RegExp(
      `if \\((\\w[\\w.\\->\\[\\]]*) \\${a} (\\w[\\w.\\->\\[\\]]*)\\)`,
      "g",
    );
    let k: RegExpExecArray | null;
    while ((k = rel.exec(src))) {
      out.push({
        label: `mirror ${k[0]}`,
        text: src.replace(k[0], `if (${k[2]} ${b} ${k[1]})`),
      });
    }
  }
  return out;
}

function compoundForms(src: string): Variant[] {
  const out: Variant[] = [];
  const re = /(\s+)([\w.\->\[\]]+) (\+|\||&|\^|\*|-)= ([^;]+);/g;
  let m: RegExpExecArray | null;
  while ((m = re.exec(src))) {
    out.push({
      label: `expand ${m[2]} ${m[3]}=`,
      text: src.replace(m[0], `${m[1]}${m[2]} = ${m[2]} ${m[3]} ${m[4]};`),
    });
  }
  const re2 = /(\s+)([\w.\->\[\]]+) = \2 (\+|\||&|\^|\*) ([^;]+);/g;
  while ((m = re2.exec(src))) {
    out.push({
      label: `contract ${m[2]}`,
      text: src.replace(m[0], `${m[1]}${m[2]} ${m[3]}= ${m[4]};`),
    });
  }
  return out;
}

// ---- greedy search -----------------------------------------------------
const original = fs.readFileSync(file, "utf8");
let best = original;
let bestScore = score();
console.log(`${owner} baseline dh=${bestScore}`);
if (!isFinite(bestScore)) {
  console.log("  baseline does not compile; nothing to search");
  process.exit(0);
}

let tried = 0;
for (let round = 0; round < rounds && bestScore > 0; round++) {
  const variants = [
    ...swaps(best),
    ...distantSwaps(best),
    ...guardForms(best),
    ...compoundForms(best),
    ...retypes(best),
    ...commutes(best),
  ];
  let improved = false;
  for (const v of variants) {
    if (v.text === best) continue;
    fs.writeFileSync(file, v.text);
    tried++;
    const s = score();
    if (s < bestScore) {
      console.log(`  round ${round}: ${v.label} -> dh=${s} (was ${bestScore})`);
      bestScore = s;
      best = v.text;
      improved = true;
      break; // restart generation from the improved source
    }
  }
  if (!improved) break;
}
fs.writeFileSync(file, best);
console.log(`${owner} FINAL dh=${bestScore} after ${tried} variants`);
