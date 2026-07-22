#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md, package.json.

const ZERO_OID = /^0+$/;
const BLOCKED_EXTENSIONS = new Set([
  "a", "bin", "bps", "bsdiff", "d", "diff", "dis", "dll", "dmp", "dump", "dylib", "elf", "exe",
  "gba", "gz", "ips", "lst", "log", "map", "o", "patch", "raw", "rom", "sav", "so", "sym", "tar",
  "tgz", "ups", "xdelta", "xdelta3", "zip", "7z",
]);
const BLOCKED_DIRECTORIES = new Set([
  ".cache", "alchemy-gcc", "analysis", "build", "builds", "cmatch", "comparisons", "compiler-output",
  "diffs", "disassembly", "dist", "dump", "dumps", "m2c", "objdump", "out", "private", "report",
  "reports", "rom", "roms", "toolchain", "toolchains", "work",
]);
const PRIVATE_REPORT = /(?:^|[._-])(?:analysis|comparison|diff|dump|report)(?:[._-]|$)/i;
const REPORT_EXTENSIONS = new Set(["csv", "json", "jsonl", "log", "tsv", "txt"]);

type GitResult = { stdout: Buffer; stderr: Buffer; exitCode: number };

function git(args: string[], input?: string): GitResult {
  return Bun.spawnSync(["git", ...args], {
    stdin: input === undefined ? undefined : Buffer.from(input),
    stdout: "pipe",
    stderr: "pipe",
  });
}

function output(result: GitResult, label: string): Buffer {
  if (result.exitCode !== 0) throw new Error(result.stderr.toString().trim() || `${label} failed`);
  return result.stdout;
}

function nulList(value: Uint8Array): string[] {
  return Buffer.from(value).toString().split("\0").filter((item) => item.length > 0);
}

function lines(value: Uint8Array): string[] {
  return Buffer.from(value).toString().split(/\r?\n/).map((item) => item.trim()).filter((item) => item.length > 0);
}

function extension(path: string): string {
  const leaf = path.slice(path.lastIndexOf("/") + 1);
  const dot = leaf.lastIndexOf(".");
  return dot < 0 ? "" : leaf.slice(dot + 1).toLowerCase();
}

function canonicalBinarySource(path: string): boolean {
  const normalized = path.replaceAll("\\", "/").toLowerCase();
  return normalized.startsWith("assets/maps/") &&
    ["metatiles.bin", "metatile_attributes.bin"].includes(normalized.slice(normalized.lastIndexOf("/") + 1));
}

export function publicationPathReason(path: string): string | undefined {
  const normalized = path.replaceAll("\\", "/");
  const components = normalized.split("/").filter((item) => item.length > 0);
  const leaf = components.at(-1)?.toLowerCase() ?? "";
  const directories = components.slice(0, -1).map((item) => item.toLowerCase());
  const suffix = extension(normalized);
  if (normalized.startsWith("/") || components.includes("..")) return "invalid repository path";
  if (directories.some((item) => BLOCKED_DIRECTORIES.has(item) || item.startsWith(".cmatch"))) {
    return "private or generated directory";
  }
  if (leaf === "baserom" || leaf.startsWith("baserom.")) return "private ROM name";
  if (leaf.includes(".gba.") || leaf.includes(".rom.")) return "private ROM name";
  if (BLOCKED_EXTENSIONS.has(suffix) && !canonicalBinarySource(normalized)) return "private or generated file type";
  if (REPORT_EXTENSIONS.has(suffix) && PRIVATE_REPORT.test(leaf)) return "private analysis report";
  return undefined;
}

function gbaImage(data: Uint8Array): boolean {
  if (data.length < 0xc0 || data.length % 0x8000 !== 0 || data.length > 0x04000000) return false;
  if (data[0xb2] !== 0x96 || data[0xb3] !== 0 || data[0xb4] !== 0) return false;
  for (let index = 0xb5; index <= 0xbb; index++) if (data[index] !== 0) return false;
  let sum = 0;
  for (let index = 0xa0; index <= 0xbc; index++) sum = (sum + data[index]) & 0xff;
  return data[0xbd] === ((-sum - 0x19) & 0xff);
}

export function publicationContentReason(data: Uint8Array): string | undefined {
  if (gbaImage(data)) return "GBA ROM image";
  if (data.length >= 4 && data[0] === 0x7f && data[1] === 0x45 && data[2] === 0x4c && data[3] === 0x46) {
    return "ELF build product";
  }
  if (data.length >= 8 && Buffer.from(data.subarray(0, 8)).toString("ascii") === "!<arch>\n") {
    return "archive or object library";
  }
  if (data.length >= 2 && data[0] === 0x4d && data[1] === 0x5a) return "native executable";
  if (data.length >= 4) {
    const magic = Buffer.from(data.subarray(0, 4)).readUInt32BE();
    if ([
      0xfeedface, 0xcefaedfe, 0xfeedfacf, 0xcffaedfe, 0xcafebabe, 0xbebafeca, 0xcafebabf, 0xbfbafeca,
      0x0061736d,
    ].includes(magic)) {
      return "native executable";
    }
  }
  return undefined;
}

export function publicationEntryReason(path: string, data: Uint8Array): string | undefined {
  const pathReason = publicationPathReason(path);
  if (pathReason !== undefined) return pathReason;
  if (["asm", "s"].includes(extension(path)) && /^\s*\.incbin\b/im.test(Buffer.from(data).toString())) {
    return "committed incbin payload";
  }
  return publicationContentReason(data);
}

function stagedPaths(): string[] {
  return nulList(output(git(["diff", "--cached", "--name-only", "--diff-filter=ACMRT", "-z"]), "staged path scan"));
}

function stagedBlob(path: string): Buffer {
  return output(git(["show", `:${path}`]), `staged blob ${path}`);
}

function changedPaths(commit: string): string[] {
  return nulList(output(git([
    "diff-tree", "--root", "--no-commit-id", "--name-only", "--diff-filter=ACMRT", "-r", "-z", commit,
  ]), `commit path scan ${commit}`));
}

function commitBlob(commit: string, path: string): Buffer {
  return output(git(["show", `${commit}:${path}`]), `commit blob ${commit}:${path}`);
}

function reject(entries: Array<{ scope: string; path: string; data: () => Buffer }>): void {
  const failures: string[] = [];
  for (const entry of entries) {
    const pathReason = publicationPathReason(entry.path);
    if (pathReason !== undefined) {
      failures.push(`${entry.scope} ${entry.path}: ${pathReason}`);
      continue;
    }
    const contentReason = publicationEntryReason(entry.path, entry.data());
    if (contentReason !== undefined) failures.push(`${entry.scope} ${entry.path}: ${contentReason}`);
  }
  if (failures.length > 0) throw new Error(`publication gate rejected:\n${failures.join("\n")}`);
}

function checkStaged(): void {
  reject(stagedPaths().map((path) => ({ scope: "staged", path, data: () => stagedBlob(path) })));
}

function revisions(local: string, remote: string): string[] {
  const args = ["rev-list", local];
  if (!ZERO_OID.test(remote)) args.push(`^${remote}`);
  return lines(output(git(args), `outgoing revision scan ${local}`));
}

async function checkPush(): Promise<void> {
  const updates = lines(Buffer.from(await Bun.stdin.text()));
  const commits = new Set<string>();
  for (const update of updates) {
    const fields = update.split(/\s+/);
    if (fields.length !== 4) throw new Error("invalid pre-push update");
    const [, local, , remote] = fields;
    if (ZERO_OID.test(local)) continue;
    for (const commit of revisions(local, remote)) commits.add(commit);
  }
  const entries: Array<{ scope: string; path: string; data: () => Buffer }> = [];
  for (const commit of commits) {
    for (const path of changedPaths(commit)) {
      entries.push({ scope: commit.slice(0, 12), path, data: () => commitBlob(commit, path) });
    }
  }
  reject(entries);
}

function mustRejectPath(path: string): void {
  if (publicationPathReason(path) === undefined) throw new Error(`private path accepted: ${path}`);
}

function mustAcceptPath(path: string): void {
  const reason = publicationPathReason(path);
  if (reason !== undefined) throw new Error(`source path rejected: ${path}: ${reason}`);
}

function selfTest(): void {
  for (const path of [
    "gs1-en.gba", "gs1-ja.gba", "gs1-de.gba", "gs1-es.gba", "gs1-fr.gba", "gs1-it.gba",
    "gs2-en.gba", "gs2-ja.gba", "gs2-de.gba", "gs2-es.gba", "gs2-fr.gba", "gs2-it.gba",
    "roms/private/gs1-en.gba", "out/diff.json", "work/rom.raw", "alchemy-gcc/bin/compiler",
    "analysis/regions.json", "reports/comparison.json", "dump.bin", "private-diff.json", "game.elf",
    "gs1-en.gba.lz", "regional.patch", "engine.bsdiff",
    ".cmatch-fresh/result.s", "comparisons/shared-runs.json", "compiler-output/function.s",
  ]) mustRejectPath(path);
  for (const path of [
    "src/main.c", "asm/080000c0.s", "assets/graphics/title.png", "assets/audio/theme.mid",
    "assets/audio/wave.wav", "assets/data/layout.json", "tools/compare_roms.ts", "tools/build_full.ts",
    "assets/data/resource_2/build_stamp.txt", "assets/maps/town/metatiles.bin",
    "assets/maps/town/metatile_attributes.bin", "rom.sha1", "PUBLICATION.md",
  ]) mustAcceptPath(path);
  const rom = Buffer.alloc(0x8000);
  rom[0xb2] = 0x96;
  let sum = 0;
  for (let index = 0xa0; index <= 0xbc; index++) sum = (sum + rom[index]) & 0xff;
  rom[0xbd] = (-sum - 0x19) & 0xff;
  if (publicationContentReason(rom) !== "GBA ROM image") throw new Error("disguised GBA ROM was accepted");
  if (publicationContentReason(Buffer.from([0x7f, 0x45, 0x4c, 0x46])) !== "ELF build product") {
    throw new Error("disguised ELF was accepted");
  }
  if (publicationContentReason(Buffer.from("!<arch>\n")) !== "archive or object library") {
    throw new Error("disguised archive was accepted");
  }
  if (publicationContentReason(Buffer.from("canonical source")) !== undefined) {
    throw new Error("ordinary source content was rejected");
  }
  if (publicationEntryReason("asm/08000000.s", Buffer.from(".incbin \"rom.gba\"\n")) !== "committed incbin payload") {
    throw new Error("committed incbin payload was accepted");
  }
}

async function main(args: string[]): Promise<void> {
  if (args.length !== 1) throw new Error("usage: check_publication.ts {--staged|--pre-push|--self-test}");
  if (args[0] === "--staged") checkStaged();
  else if (args[0] === "--pre-push") await checkPush();
  else if (args[0] === "--self-test") {
    selfTest();
    console.log("self-test=ok");
  } else throw new Error("usage: check_publication.ts {--staged|--pre-push|--self-test}");
}

if (import.meta.main) await main(Bun.argv.slice(2));
