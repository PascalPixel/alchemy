// Allocator lens: read GCC 2.96's own pass dumps instead of probing it.
//
//   bun tools/allocator-lens/lens.ts <owner-hex> [candidate.c]
//
// Compiles the candidate with -da (no compiler change: the dump machinery
// ships in the staged cc1) and reports, per pseudo register: the source
// variable it carries, its creation order, class costs and preference from
// local alloc, its position in global alloc's ordering, its conflict set,
// and the hard register it received. Reload decisions and spill sites are
// listed with their insns. This replaces guessing with the allocator's own
// record: to move a value into a different register, change the property
// this report shows is decisive — order, preference, or a conflict edge.
const { execFileSync } = require("child_process");
const fs = require("fs");

const REPO = "/Users/pixel/Developer/PascalPixel/alchemy";
const owner = process.argv[2];
const source = process.argv[3] ?? `games/gs1/recon/en/main/${owner}.c`;
const work = `${process.env.TMPDIR ?? "/tmp"}/lens-${owner}`;
fs.rmSync(work, { recursive: true, force: true });
fs.mkdirSync(work, { recursive: true });

function run(bin: string, args: string[], cwd = REPO) {
  return execFileSync(bin, args, {
    cwd,
    encoding: "utf8",
    maxBuffer: 64 << 20,
  });
}

const src = `${REPO}/${source}`;
const dir = src.slice(0, src.lastIndexOf("/"));
run(`${REPO}/alchemy-gcc/dist/cpp`, [
  "-nostdinc",
  `-I${REPO}/games/gs1/include`,
  `-I${dir}`,
  src,
  `${work}/in.i`,
]);
run(
  `${REPO}/alchemy-gcc/dist/cc1`,
  [
    "-O2",
    "-mthumb",
    "-mthumb-interwork",
    "-mcpu=arm7tdmi",
    "-fno-builtin",
    "-fcall-used-r4",
    "-quiet",
    "-da",
    "-o",
    `${work}/out.s`,
    `${work}/in.i`,
  ],
  work,
);

const read = (pass: string) => {
  const file = fs.readdirSync(work).find((f: string) => f.endsWith(`.${pass}`));
  return file ? fs.readFileSync(`${work}/${file}`, "utf8") : "";
};

// Pseudo -> source variable, from RTL (reg/v:SI NN [ name ]) annotations.
const names = new Map<number, string>();
const creation: number[] = [];
for (const m of read("rtl").matchAll(
  /\(reg\/?v?[^ ]* ([0-9]+)(?: \[ ([^\]]+) \])?/g,
)) {
  const pseudo = Number(m[1]);
  if (pseudo < 32) continue;
  if (!creation.includes(pseudo)) creation.push(pseudo);
  if (m[2] && !names.has(pseudo)) names.set(pseudo, m[2].trim());
}

// Local alloc: class costs and preferences.
const lreg = read("lreg");
const costs = new Map<number, string>();
for (const m of lreg.matchAll(/Register ([0-9]+) costs: (.+)/g)) {
  const interesting = m[2]
    .split(" ")
    .filter((c: string) => /^(LO_REGS|HI_REGS|MEM|GENERAL_REGS):/.test(c))
    .join(" ");
  costs.set(Number(m[1]), interesting);
}
const prefs = new Map<number, string>();
for (const m of lreg.matchAll(/Register ([0-9]+) pref ([A-Z_]+)/g))
  prefs.set(Number(m[1]), m[2]);

// Global alloc: ordering, conflicts, final assignment.
const greg = read("greg");
const orderMatch = greg.match(/;; \d+ regs to allocate: ([0-9 ]+)/);
const order = orderMatch ? orderMatch[1].trim().split(/\s+/).map(Number) : [];
const conflicts = new Map<number, string>();
for (const m of greg.matchAll(/;; ([0-9]+) conflicts: (.+)/g))
  conflicts.set(Number(m[1]), m[2].trim());
// Local alloc assigns most pseudos; global alloc the call-crossing ones.
const assigned = new Map<number, number>();
for (const text of [lreg, greg])
  for (const m of text.matchAll(/;; Register ([0-9]+) in ([0-9]+)/g))
    assigned.set(Number(m[1]), Number(m[2]));
const spills = [...greg.matchAll(/Spilling for insn ([0-9]+)\./g)].map(
  (m) => m[1],
);
const reloads = [
  ...greg.matchAll(/Using reg ([0-9]+) for reload ([0-9]+)/g),
].map((m) => `r${m[1]} for reload ${m[2]}`);

const REGS = [
  "r0",
  "r1",
  "r2",
  "r3",
  "r4",
  "r5",
  "r6",
  "r7",
  "r8",
  "r9",
  "sl",
  "fp",
  "ip",
  "sp",
  "lr",
  "pc",
];
console.log(
  `${owner}: ${creation.length} pseudos, global order: ${order.join(" ")}`,
);
console.log("pseudo  hard  var                     pref      order  costs");
for (const p of creation) {
  const hard = assigned.has(p)
    ? (REGS[assigned.get(p)!] ?? `h${assigned.get(p)}`)
    : "-";
  const pos = order.indexOf(p);
  console.log(
    `${String(p).padEnd(7)} ${hard.padEnd(5)} ${(names.get(p) ?? "").padEnd(23)} ${(prefs.get(p) ?? "").padEnd(9)} ${(pos >= 0 ? String(pos) : "-").padEnd(6)} ${costs.get(p) ?? ""}`,
  );
}
if (spills.length) console.log(`spill insns: ${spills.join(" ")}`);
if (reloads.length) console.log(`reloads: ${reloads.join("; ")}`);
console.log(
  `dumps kept in ${work} (cse=03, cse2=09, combine=13, lreg=17, greg=18, sched2=23)`,
);
