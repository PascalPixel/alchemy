#!/usr/bin/env bun
// Non-destructive Full-C history ledger. Commit objects are never rewritten.
// Each first-parent tree is measured from its tracked C ownership against the
// current audited, fixed executable denominator; legacy subject numbers are
// retained only as text and are never used as metric evidence.
import { readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { canonicalJson } from "./canonical_json.ts";
import { formatSubject, roundHalfUpPercent } from "./full_c_progress.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface RegionSize {
  address: number;
  size: number;
}

interface HistoryEntry {
  commit: string;
  first_parent_position: number;
  author_time: string;
  committer_time: string;
  original_subject: string;
  full_c_bytes: number;
  executable_bytes: number;
  remaining_bytes: number;
  percent: number;
  main_full_c_bytes: number;
  overlay_full_c_bytes: number;
  canonical_suffix: string;
  derivation_status: "measured";
  evidence: string[];
  correction?: string;
}

interface HistoryLedger {
  format: 1;
  metric: "full-c-byte-share";
  target: "gs1-en";
  denominator_commit: string;
  executable_bytes: number;
  history_scope: "first-parent";
  generated_from: string;
  entries: HistoryEntry[];
}

interface TreeEntry {
  oid: string;
  path: string;
}

function git(args: string[]): string {
  const process = Bun.spawnSync(["git", ...args], {
    cwd: ROOT,
    stdout: "pipe",
    stderr: "pipe",
  });
  if (process.exitCode !== 0) {
    throw new Error(process.stderr.toString().trim() || `git ${args.join(" ")} failed`);
  }
  return process.stdout.toString();
}

function regionMap(): Map<string, RegionSize> {
  const rows: any[] = [];
  for (const path of [
    "out/full/asm/manifest.json",
    "out/full/claimed/manifest.json",
  ]) {
    const document = JSON.parse(readFileSync(join(ROOT, path), "utf8"));
    rows.push(...(document.regions ?? []));
  }
  const result = new Map<string, RegionSize>();
  for (const row of rows) {
    const match = String(row.source).match(/([0-9a-f]{8})\.(?:c|s)$/i);
    if (!match) continue;
    const value = { address: Number(row.address), size: Number(row.size) };
    const previous = result.get(match[1].toLowerCase());
    if (previous && (previous.address !== value.address || previous.size !== value.size)) {
      throw new Error(`conflicting region boundary for ${match[1]}`);
    }
    result.set(match[1].toLowerCase(), value);
  }
  return result;
}

function commits(): Array<{
  commit: string;
  author: string;
  committer: string;
  subject: string;
}> {
  const format = "%H%x1f%aI%x1f%cI%x1f%s%x1e";
  return git(["log", "--first-parent", "--reverse", `--format=${format}`])
    .split("\x1e")
    .map((record) => record.trim())
    .filter(Boolean)
    .map((record) => {
      const [commit, author, committer, subject] = record.split("\x1f");
      return { commit, author, committer, subject };
    });
}

function tree(commit: string): TreeEntry[] {
  return git(["ls-tree", "-r", commit])
    .split(/\r?\n/)
    .filter(Boolean)
    .map((line) => {
      const match = /^[0-9]+\s+\w+\s+([0-9a-f]+)\t(.+)$/.exec(line);
      if (!match) throw new Error(`cannot parse ls-tree row: ${line}`);
      return { oid: match[1], path: match[2] };
    });
}

const blobCache = new Map<string, string>();
function blob(oid: string): string {
  let value = blobCache.get(oid);
  if (value === undefined) {
    value = git(["cat-file", "blob", oid]);
    blobCache.set(oid, value);
  }
  return value;
}

function acceptableHistoricalC(source: string): boolean {
  return ![
    /\bregister\b[^;\n]*\basm\s*\(/,
    /\b__asm__\b|\basm\s+volatile\b/,
    /\.incbin\b/,
    /\bM2C_ERROR\b/,
  ].some((pattern) => pattern.test(source));
}

function overlayPlaceholders(source: string): Map<string, number> {
  const result = new Map<string, number>();
  let owner = "";
  let inPlaceholder = false;
  for (const line of source.split(/\r?\n/)) {
    const label = /^\s*AlchemyC_([0-9a-f]{8}):\s*$/i.exec(line);
    if (label) {
      owner = label[1].toLowerCase();
      inPlaceholder = true;
      continue;
    }
    if (inPlaceholder && (/^\s*$/.test(line) || /^\s*\.L_[0-9a-z_.$]+:\s*$/i.test(line))) continue;
    const space = /^\s*\.space\s+(0x[0-9a-f]+|\d+)\s*$/i.exec(line);
    if (inPlaceholder && space) {
      result.set(owner, (result.get(owner) ?? 0) + Number.parseInt(space[1], 0));
      continue;
    }
    if (line.trim()) inPlaceholder = false;
  }
  return result;
}

function measuredTree(
  entries: TreeEntry[],
  sizes: ReadonlyMap<string, RegionSize>,
): { main: number; overlays: number; accepted: Set<string>; excluded: string[] } {
  let main = 0;
  const accepted = new Set<string>();
  const excluded: string[] = [];
  const paths = new Map(entries.map((entry) => [entry.path, entry]));
  for (const entry of entries) {
    const match = /^src\/([0-9a-f]{8})\.c$/i.exec(entry.path);
    if (!match) continue;
    const stem = match[1].toLowerCase();
    const region = sizes.get(stem);
    const source = blob(entry.oid);
    if (!acceptableHistoricalC(source)) {
      excluded.push(`${entry.path}:noncanonical-C`);
      continue;
    }
    if (!region) {
      excluded.push(`${entry.path}:no-audited-region`);
      continue;
    }
    main += region.size;
    accepted.add(entry.path);
  }

  let overlays = 0;
  for (const entry of entries) {
    const match = /^assets\/code\/(.+)_overlay\.s$/.exec(entry.path);
    if (!match) continue;
    const prefix = `assets/code/${match[1]}_c_`;
    const cAddresses = new Set(
      entries
        .map((candidate) => {
          const found = candidate.path.startsWith(prefix)
            ? candidate.path.match(/_c_([0-9a-f]{8})\.c$/i)
            : null;
          return found?.[1].toLowerCase();
        })
        .filter((value): value is string => value !== undefined),
    );
    const placeholders = overlayPlaceholders(blob(entry.oid));
    for (const address of cAddresses) {
      const size = placeholders.get(address);
      if (size === undefined) {
        excluded.push(`${prefix}${address}.c:no-placeholder`);
        continue;
      }
      const cPath = `${prefix}${address}.c`;
      const cEntry = paths.get(cPath);
      if (!cEntry || !acceptableHistoricalC(blob(cEntry.oid))) {
        excluded.push(`${cPath}:noncanonical-C`);
        continue;
      }
      overlays += size;
      accepted.add(cPath);
    }
  }
  return { main, overlays, accepted, excluded };
}

function csvCell(value: string | number): string {
  const text = String(value);
  return /[",\r\n]/.test(text) ? `"${text.replaceAll('"', '""')}"` : text;
}

function ledger(): HistoryLedger {
  const report = JSON.parse(readFileSync(join(ROOT, "metrics/gs1-en-progress.json"), "utf8"));
  if (report.audit !== "complete" || report.metric !== "full-c-byte-share") {
    throw new Error("current audited Full-C report is required");
  }
  const denominator = Number(report.executable_bytes);
  const sizes = regionMap();
  const history = commits();
  const rows: HistoryEntry[] = [];
  let previous = 0;
  let previousAccepted = new Set<string>();
  for (let index = 0; index < history.length; index++) {
    const commit = history[index];
    const measured = measuredTree(tree(commit.commit), sizes);
    const fullC = measured.main + measured.overlays;
    if (fullC > denominator) throw new Error(`${commit.commit}: numerator exceeds denominator`);
    const removed = [...previousAccepted].filter((path) => !measured.accepted.has(path));
    const correction = fullC < previous
      ? `measured C ownership decreased by ${previous - fullC} bytes; removed/reclassified paths: ${removed.join(", ")}`
      : undefined;
    if (fullC < previous && removed.length === 0) {
      throw new Error(`${commit.commit}: unexplained Full-C regression`);
    }
    rows.push({
      commit: commit.commit,
      first_parent_position: index + 1,
      author_time: commit.author,
      committer_time: commit.committer,
      original_subject: commit.subject,
      full_c_bytes: fullC,
      executable_bytes: denominator,
      remaining_bytes: denominator - fullC,
      percent: roundHalfUpPercent(fullC, denominator),
      main_full_c_bytes: measured.main,
      overlay_full_c_bytes: measured.overlays,
      canonical_suffix: formatSubject(fullC, denominator),
      derivation_status: "measured",
      evidence: [
        "commit tree src/<address>.c ownership mapped to audited main executable regions",
        "commit tree overlay C files mapped to same-tree AlchemyC placeholder spans",
        "noncanonical register-pinned/inline-assembly/fakematch C excluded",
        ...(measured.excluded.length ? [`excluded=${measured.excluded.join(";")}`] : []),
      ],
      ...(correction ? { correction } : {}),
    });
    previous = fullC;
    previousAccepted = measured.accepted;
  }
  return {
    format: 1,
    metric: "full-c-byte-share",
    target: "gs1-en",
    denominator_commit: git(["rev-parse", "HEAD"]).trim(),
    executable_bytes: denominator,
    history_scope: "first-parent",
    generated_from: git(["rev-parse", "HEAD^{tree}"]).trim(),
    entries: rows,
  };
}

function selfTest(): void {
  const placeholders = overlayPlaceholders(
    "AlchemyC_02000010:\n\t.space 2\n.L_02000012:\n\t.space 4\n\tbx lr\n",
  );
  if (placeholders.get("02000010") !== 6) throw new Error("overlay placeholder adapter failed");
  if (acceptableHistoricalC('register int x asm("r4");')) throw new Error("register pin accepted");
  if (!acceptableHistoricalC("int f(void) { return 1; }")) throw new Error("ordinary C rejected");
  console.log("self-test=ok history=full-c-byte-share");
}

function writeLedger(): void {
  const output = ledger();
  const jsonPath = join(ROOT, "docs/full-c-history.json");
  const csvPath = join(ROOT, "docs/full-c-history.csv");
  writeFileSync(jsonPath, canonicalJson(output));
  const columns = [
    "commit", "first_parent_position", "author_time", "committer_time",
    "original_subject", "full_c_bytes", "executable_bytes", "remaining_bytes",
    "percent", "main_full_c_bytes", "overlay_full_c_bytes", "canonical_suffix",
    "derivation_status", "correction",
  ] as const;
  const csv = [
    columns.join(","),
    ...output.entries.map((entry) =>
      columns.map((column) => csvCell(entry[column] ?? "")).join(",")),
  ].join("\n") + "\n";
  writeFileSync(csvPath, csv);
  const regressions = output.entries.filter((entry) => entry.correction).length;
  console.log(
    `history=${output.entries.length} measured=${output.entries.length} ` +
    `unmeasured=0 corrections=${regressions} denominator=${output.executable_bytes}`,
  );
}

if (import.meta.main) {
  if (Bun.argv[2] === "--self-test") selfTest();
  else if (Bun.argv.length === 2 || Bun.argv[2] === "--write") writeLedger();
  else throw new Error("usage: full_c_history.ts [--write|--self-test]");
}
