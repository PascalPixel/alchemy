#!/usr/bin/env bun
// Pre-size continuation-shaped manifest rows into whole semantic owners.
//
// A manifest row is not a function. `split_first`, `merge_with_continuations`
// and `merge_with_owner` rows are fragments of an owner whose live frame and
// register state cross row boundaries, so their advertised byte size cannot be
// used to budget, rank or assign them: `080e47b8` is advertised at 768 bytes
// and its real span is 7,762 bytes across 16 rows. HANDOVER section 0 therefore
// requires transitive sizing and a pool map before any such row is assigned.
// This tool produces both.
//
//   bun tools/semantic_owner_scope.ts             # report every open owner
//   bun tools/semantic_owner_scope.ts 080d77b4    # one owner, with its rows
//   bun tools/semantic_owner_scope.ts --json      # machine-readable
//   bun tools/semantic_owner_scope.ts --self-test
//
// Grouping rule, which mirrors how the rows were classified in the first place:
// an owner starts at a row carrying a prologue that saves lr, and absorbs the
// following rows until the row holding its epilogue. Rows whose reconstruction
// is nothing but `.inst.n` halfwords carry no mnemonics and are reported as
// DATA — those are embedded literal pools and alignment, and they are excluded
// from the owner's executable ranges exactly as the registered owners in
// `semantic/main-regions.json` exclude theirs.
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { basename, dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const CONTINUATION = new Set([
  "split_first",
  "merge_with_continuations",
  "merge_with_owner",
]);

interface Region {
  address: number;
  size: number;
  source: string;
  kind: string;
  retention: string;
}

export interface RowFacts {
  stem: string;
  address: number;
  size: number;
  retention: string;
  kind: string;
  /** Saves lr, so it can begin an owner. */
  hasPrologue: boolean;
  /** Restores pc or returns, so it can end one. */
  hasEpilogue: boolean;
  /** Reconstruction is raw halfwords only: a pool or alignment, never C. */
  isData: boolean;
  /**
   * Looks like a literal pool even though it decoded to mnemonics. Halfwords of
   * a pool disassemble as perfectly plausible `lsrs`/`movs` pairs, so `isData`
   * alone undercounts pools badly in rows that came from a regular `asm/*.s`
   * rather than an executable-gap reconstruction: two such rows in `080be378`
   * (108 and 48 bytes) were counted as code until their PC-relative provenance
   * was resolved manually.
   */
  suspectedPool: boolean;
  calls: number;
}

/**
 * Thumb functions in this target return three ways, and recognising only the
 * first two reports perfectly ordinary owners as needing a boundary audit:
 *   pop {..., pc}      — the common form
 *   bx lr              — leaf functions
 *   pop {rN} ; bx rN   — interworking returns, used by every owner that saves
 *                        high registers (`080a7478` returns via `bx r1`)
 * The third is only an epilogue in the company of a stack restore; a bare
 * `bx rN` elsewhere is a jump-table dispatch, so requiring a nearby `pop`
 * keeps dispatchers from closing an owner early.
 */
export function hasEpilogue(body: string): boolean {
  if (/^\s*pop\s*\{[^}]*\bpc\b/m.test(body)) return true;
  if (/^\s*bx\s+lr\b/m.test(body)) return true;
  const lines = body.split("\n");
  for (let index = 0; index < lines.length; index++) {
    const branch = /^\s*bx\s+(r\d+)\b/.exec(lines[index]);
    if (branch === null) continue;
    const preceding = lines.slice(Math.max(0, index - 4), index).join("\n");
    if (new RegExp(`^\\s*pop\\s*\\{[^}]*\\b${branch[1]}\\b`, "m").test(preceding)) return true;
  }
  return false;
}

/**
 * A row that is only `.inst.n` directives has no decoded mnemonics at all.
 * Distinguishing it matters more than it looks: such a row is counted in the
 * manifest as an executable gap continuation, so it inflates the remaining-work
 * figure while being literally unconvertible.
 */
export function rowFactsFromAssembly(
  text: string,
  region: Pick<Region, "address" | "size" | "kind" | "retention">,
): RowFacts {
  const body = text
    .split("\n")
    .filter((line) => !/^\s*(@|\.(syntax|text|thumb|set|global|align))/.test(line))
    .join("\n");
  const mnemonics = body
    .split("\n")
    .filter((line) => /^\s*[a-z][a-z0-9.]*\s/i.test(line) && !/^\s*\.inst/.test(line));
  return {
    stem: region.address.toString(16).padStart(8, "0"),
    address: region.address,
    size: region.size,
    retention: region.retention,
    kind: region.kind,
    hasPrologue: /^\s*push\s*\{[^}]*\blr\b/m.test(body),
    hasEpilogue: hasEpilogue(body),
    isData: mnemonics.length === 0,
    // No call, no branch, no stack traffic: nothing that could make the row a
    // reachable piece of a function. Memory ops are deliberately NOT part of
    // the test — pool words decode to `ldr`-shaped instructions often enough
    // that excluding them missed a whole 68-byte pool row in `080d765c`.
    // Conservative by design: it flags for review, never excludes silently.
    suspectedPool:
      mnemonics.length > 0 &&
      !/^\s*(bl|b|b[a-z]{2}|bx|blx|push|pop)\b/m.test(body),
    calls: [...body.matchAll(/^\s*bl\s+\S+/gm)].length,
  };
}

export interface Owner {
  entry: string;
  rows: RowFacts[];
  /** Sum of the rows that actually hold instructions. */
  executableBytes: number;
  /** Sum of the pool/alignment rows, excluded from the owner. */
  excludedBytes: number;
  advertisedBytes: number;
  calls: number;
  closed: boolean;
  /** Bytes counted as executable that look like literal pools. */
  suspectedPoolBytes: number;
}

/** Group consecutive rows into owners: prologue opens, epilogue closes. */
export function groupOwners(rows: RowFacts[]): Owner[] {
  const ordered = [...rows].sort((left, right) => left.address - right.address);
  const owners: Owner[] = [];
  let current: RowFacts[] = [];
  const flush = (closed: boolean) => {
    if (current.length === 0) return;
    const executable = current.filter((row) => !row.isData);
    owners.push({
      entry: current[0].stem,
      rows: current,
      executableBytes: executable.reduce((sum, row) => sum + row.size, 0),
      excludedBytes: current
        .filter((row) => row.isData)
        .reduce((sum, row) => sum + row.size, 0),
      advertisedBytes: current[0].size,
      calls: current.reduce((sum, row) => sum + row.calls, 0),
      closed,
      suspectedPoolBytes: current
        .filter((row) => !row.isData && row.suspectedPool)
        .reduce((sum, row) => sum + row.size, 0),
    });
    current = [];
  };
  for (const row of ordered) {
    if (row.hasPrologue && current.length > 0) flush(false);
    current.push(row);
    if (row.hasEpilogue) flush(true);
  }
  flush(false);
  return owners;
}

function admittedStems(): Set<string> {
  const stems = new Set<string>();
  for (const directory of ["src", join("semantic", "main")]) {
    const path = join(ROOT, directory);
    if (!existsSync(path)) continue;
    for (const name of readdirSync(path)) {
      if (/^08[0-9a-f]{6}\.c$/i.test(name)) stems.add(basename(name, ".c").toLowerCase());
    }
  }
  return stems;
}

interface AddressRange { start: number; end: number }

/** Registered semantic coverage plus reviewed ranges that deliberately stay non-C. */
function registeredCoverage(): {
  ownerSpans: { lo: number; hi: number }[];
  executableRanges: AddressRange[];
  nonCRanges: AddressRange[];
} {
  const path = join(ROOT, "semantic", "main-regions.json");
  if (!existsSync(path)) return { ownerSpans: [], executableRanges: [], nonCRanges: [] };
  const registry = JSON.parse(readFileSync(path, "utf8")) as {
    non_c_ranges?: { address: string; size: number; kind: string; evidence: string }[];
    main_owners: { executable_ranges: { address: string; size: number }[] }[];
  };
  const executableRanges = registry.main_owners.flatMap((owner) =>
    owner.executable_ranges.map((range) => ({
      start: parseInt(range.address, 16),
      end: parseInt(range.address, 16) + range.size,
    }))
  );
  const ownerSpans = registry.main_owners.map((owner) => {
    const ranges = owner.executable_ranges.map((range) => ({
      start: parseInt(range.address, 16),
      end: parseInt(range.address, 16) + range.size,
    }));
    return {
      lo: Math.min(...ranges.map((range) => range.start)),
      hi: Math.max(...ranges.map((range) => range.end)),
    };
  });
  const nonCRanges = (registry.non_c_ranges ?? []).map((range) => {
    const start = parseInt(range.address, 16);
    if (!/^0x08[0-9a-f]{6}$/i.test(range.address) ||
        !Number.isSafeInteger(range.size) || range.size <= 0 ||
        range.kind.trim() === "" || range.evidence.trim() === "") {
      throw new Error(`invalid non-C range ${JSON.stringify(range)}`);
    }
    return { start, end: start + range.size };
  });
  for (let index = 0; index < nonCRanges.length; index++) {
    for (let other = index + 1; other < nonCRanges.length; other++) {
      if (nonCRanges[index].start < nonCRanges[other].end &&
          nonCRanges[other].start < nonCRanges[index].end) {
        throw new Error("overlapping non-C ranges in semantic/main-regions.json");
      }
    }
    if (executableRanges.some((range) =>
      nonCRanges[index].start < range.end && range.start < nonCRanges[index].end)) {
      throw new Error("non-C range overlaps registered semantic executable coverage");
    }
  }
  return { ownerSpans, executableRanges, nonCRanges };
}

function overlaps(region: Pick<Region, "address" | "size">, range: AddressRange): boolean {
  return region.address < range.end && range.start < region.address + region.size;
}

function censusDeclaredClosed(): boolean {
  const path = join(ROOT, "semantic", "main-regions.json");
  if (!existsSync(path)) return false;
  const registry = JSON.parse(readFileSync(path, "utf8")) as {
    ordinary_census?: { status?: string; check?: string; evidence?: string };
  };
  return registry.ordinary_census?.status === "closed" &&
    registry.ordinary_census.check === "bun run semantic:check" &&
    (registry.ordinary_census.evidence?.trim().length ?? 0) > 0;
}

export function openOwners(): Owner[] {
  const manifest = JSON.parse(
    readFileSync(join(ROOT, "out", "full", "asm", "manifest.json"), "utf8"),
  ) as { regions: Region[] };
  const admitted = admittedStems();
  const coverage = registeredCoverage();
  // Group over EVERY region, not just the open continuation ones. An owner's
  // epilogue frequently lives in a neighbouring row of a different retention —
  // grouping only the open rows makes such an owner look unclosed, which reads
  // as "needs an audit" when in fact the boundary is already known. Five owners
  // were misreported that way before this was widened.
  const rows: RowFacts[] = [];
  const open = new Set<string>();
  for (const region of manifest.regions) {
    const stem = region.address.toString(16).padStart(8, "0");
    const source = region.source.startsWith("/") ? region.source : join(ROOT, region.source);
    if (!existsSync(source)) continue;
    rows.push(rowFactsFromAssembly(readFileSync(source, "utf8"), region));
    if (!CONTINUATION.has(region.retention)) continue;
    if (admitted.has(stem)) continue;
    // Already accounted for as part of a registered whole owner.
    //
    // Test the row's START against the registered range, not its end. A
    // registered `hi` is the end of the owner's EXECUTABLE ranges, while
    // `region.size` is the manifest row's full size INCLUDING its trailing
    // literal pool — which the registration deliberately excludes. Requiring
    // `address + size <= hi` therefore fails forever for any owner whose last
    // row ends in a pool, and reports it as open work permanently.
    //
    // Measured when this was found: five owners were false positives this way
    // (`0800ebec` overhanging by 2 bytes, `08026080` by 14, `080d0ee0` by 16,
    // `080d4ce8` by 32, `080d765c` by 8), accounting for **11,012 of the 12,842
    // bytes the tool reported as open**. The genuine remainder was ~1,830 bytes
    // across 11 small owners. A boundary tool that overstates the work by 6x
    // sends the queue toward rows that are already done.
    // A manifest row can begin with alignment before one or more registered
    // leaf functions (0801c9be, 08021dfa, 080dbb9a, 080e72de). Checking only
    // the row start therefore reports already-admitted code as open. Accept an
    // overlap with any executable range, while retaining the bounding-span
    // rule for continuation rows split by an excluded interior pool.
    if (coverage.executableRanges.some((range) => overlaps(region, range))) continue;
    if (coverage.ownerSpans.some((span) => region.address >= span.lo && region.address < span.hi)) continue;
    if (coverage.nonCRanges.some((range) => overlaps(region, range))) continue;
    open.add(stem);
  }
  // Report only owners that still contain unconverted continuation work, and
  // drop groups with no executable bytes at all: those are stranded literal
  // pools and alignment that the manifest labels as executable gaps. They can
  // never become C, so counting them as open work overstates the remainder.
  return groupOwners(rows)
    .filter((owner) => owner.rows.some((row) => open.has(row.stem)))
    .filter((owner) => owner.executableBytes > 0);
}

function selfTest(): void {
  const code = rowFactsFromAssembly(
    "\t.thumb\nRegion_08000000:\n\tpush {r4, lr}\n\tbl sub_08001234\n\tpop {r4, pc}\n",
    { address: 0x08000000, size: 16, kind: "k", retention: "split_first" },
  );
  if (!code.hasPrologue || !code.hasEpilogue || code.isData || code.calls !== 1)
    throw new Error("code row misread");
  if (!hasEpilogue("\tpop {r1}\n\tbx r1\n")) throw new Error("interworking return missed");
  if (hasEpilogue("\tldr r3, [r4, #0]\n\tbx r3\n")) throw new Error("dispatcher must not close an owner");
  const pool = rowFactsFromAssembly(
    "\t.thumb\nFragment_08000010:\n\t.inst.n 0x1eec\n\t.inst.n 0x0300\n",
    { address: 0x08000010, size: 4, kind: "k", retention: "merge_with_owner" },
  );
  if (!pool.isData || pool.hasPrologue) throw new Error("pool row misread");
  // A pool inside a regular asm row decodes to plausible mnemonics.
  const decodedPool = rowFactsFromAssembly(
    "\t.thumb\n.L_0800:\n\tlsrs r0, r0, #1\n\tmovs r0, #0\n",
    { address: 0x08000030, size: 4, kind: "k", retention: "merge_with_owner" },
  );
  if (decodedPool.isData) throw new Error("decoded pool must not be isData");
  if (!decodedPool.suspectedPool) throw new Error("decoded pool must be suspected");
  const realCode = rowFactsFromAssembly(
    "\t.thumb\n.L_0800:\n\tldr r0, [r1, #0]\n\tbl sub_08001234\n",
    { address: 0x08000040, size: 8, kind: "k", retention: "split_first" },
  );
  if (realCode.suspectedPool) throw new Error("real code must not be suspected");
  // A pool whose words decode to memory ops must still be suspected.
  const ldrPool = rowFactsFromAssembly(
    "\t.thumb\n.L_0800:\n\tldr r0, [r4, #4]\n\tmovs r0, r0\n",
    { address: 0x08000050, size: 4, kind: "k", retention: "merge_with_owner" },
  );
  if (!ldrPool.suspectedPool) throw new Error("ldr-shaped pool must be suspected");
  // A prologue row plus a trailing pool plus the epilogue row is ONE owner.
  const owners = groupOwners([
    { ...code, address: 0x08000000, stem: "08000000", hasEpilogue: false },
    { ...pool, address: 0x08000010, stem: "08000010" },
    { ...code, address: 0x08000020, stem: "08000020", hasPrologue: false },
  ]);
  if (owners.length !== 1) throw new Error(`expected 1 owner, got ${owners.length}`);
  if (owners[0].excludedBytes !== 4) throw new Error("pool not excluded");
  if (!owners[0].closed) throw new Error("owner should be closed by its epilogue");
  if (!overlaps({ address: 0x08000000, size: 8 }, { start: 0x08000004, end: 0x0800000c }))
    throw new Error("range overlap missed");
  if (overlaps({ address: 0x08000000, size: 4 }, { start: 0x08000004, end: 0x08000008 }))
    throw new Error("touching ranges must not overlap");
  console.log("self-test=ok");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const owners = openOwners();
  if (args.includes("--check")) {
    if (!censusDeclaredClosed())
      throw new Error("main-image semantic census has no reviewed closed declaration");
    if (owners.length !== 0) {
      const bytes = owners.reduce((sum, owner) => sum + owner.executableBytes, 0);
      throw new Error(`main-image semantic census is open: ${owners.length} owners, ${bytes} bytes`);
    }
    console.log("core_semantic_census=closed owners=0 executable_bytes=0");
    return;
  }
  if (args.includes("--json")) {
    console.log(JSON.stringify(owners, null, 2));
    return;
  }
  const wanted = args.find((argument) => /^[0-9a-f]{8}$/i.test(argument));
  const shown = wanted
    ? owners.filter((owner) => owner.rows.some((row) => row.stem === wanted.toLowerCase()))
    : owners;
  for (const owner of shown) {
    const flag = owner.closed ? "" : "  UNCLOSED(no epilogue found)";
    console.log(
      `owner ${owner.entry}  advertised=${owner.advertisedBytes}  executable=${owner.executableBytes}` +
        `  excluded_pool=${owner.excludedBytes}  rows=${owner.rows.length}  calls=${owner.calls}` +
        (owner.suspectedPoolBytes > 0 ? `  suspected_pool=${owner.suspectedPoolBytes}` : "") +
        flag,
    );
    if (wanted) {
      for (const row of owner.rows) {
        console.log(
          `    ${row.stem}  ${String(row.size).padStart(5)}  ` +
            `${row.isData ? "DATA " : row.suspectedPool ? "POOL?" : "code "}` +
            `${row.retention}${row.hasPrologue ? " prologue" : ""}${row.hasEpilogue ? " epilogue" : ""}`,
        );
      }
    }
  }
  const executable = shown.reduce((sum, owner) => sum + owner.executableBytes, 0);
  const excluded = shown.reduce((sum, owner) => sum + owner.excludedBytes, 0);
  const unclosed = shown.filter((owner) => !owner.closed).length;
  console.log(
    `\nowners=${shown.length} executable_bytes=${executable} excluded_pool_bytes=${excluded} unclosed=${unclosed}`,
  );
}

if (import.meta.main) main();
