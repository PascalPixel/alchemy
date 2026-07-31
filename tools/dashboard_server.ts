// Local progress dashboard: the README's three box trees (core / overlays /
// assets), the /tmp kanban board as post-it columns, and the /tmp group chat
// as a Slack-style feed. Bun server, hyperscript client, mtime hot reload,
// everything fitted to one viewport. Untracked local convenience.
// Run: bun work/claude/progress_server.ts
import { statSync, existsSync, readFileSync } from "node:fs";
import { join, dirname } from "node:path";

const ROOT = join(dirname(Bun.fileURLToPath(import.meta.url)), "..");
const TREES = ["core", "overlays", "assets"] as const;
const svgFile = (tree: string) => join(ROOT, "assets", "readme", `gs1-en-${tree}.svg`);
const KANBAN = "/tmp/ALCHEMY_KANBAN.md";
const BOOT = String(Date.now());
const CHAT = "/tmp/ALCHEMY_GROUP_CHAT.csv";

// Dialogue portraits from the tracked resource_f0 images family
// (identified by Pascal 2026-07-31): 00 Isaac, 01 Garet, 02 Ivan, 03 Mia, 12 Kraden.
const PORTRAITS: Record<string, string> = {
  venus: "resource_f0_images_image_00.png",
  mars: "resource_f0_images_image_01.png",
  jupiter: "resource_f0_images_image_02.png",
  mercury: "resource_f0_images_image_03.png",
  vale: "resource_f0_images_image_12.png",
};
const KRADEN_SVG = `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 48 48">
  <path d="M12 48 Q12 34 24 34 Q36 34 36 48 Z" fill="#8a6d4a"/>
  <circle cx="24" cy="21" r="9.5" fill="#f2c9a0"/>
  <path d="M14 21 Q13 9 24 9 Q35 9 34 21 Q32 13 24 12.5 Q16 13 14 21 Z" fill="#cfc8bf"/>
  <ellipse cx="24" cy="29.5" rx="7.5" ry="4" fill="#cfc8bf"/>
  <circle cx="20.5" cy="22" r="1.3" fill="#2b2b2b"/><circle cx="27.5" cy="22" r="1.3" fill="#2b2b2b"/>
  <circle cx="20.5" cy="22" r="3" fill="none" stroke="#555" stroke-width="1"/>
  <circle cx="27.5" cy="22" r="3" fill="none" stroke="#555" stroke-width="1"/>
  <line x1="23.5" y1="22" x2="24.5" y2="22" stroke="#555" stroke-width="1"/>
</svg>`;

const pageHtml = () => `<!doctype html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1"><title>coverage</title><style>
  @font-face{font-family:'Weyard';src:url('/weyard.otf') format('opentype');font-weight:400;font-style:italic}
  *{box-sizing:border-box;font-smooth:never;-webkit-font-smoothing:none;font-synthesis:none}
  html,body{margin:0;background:#ffffff;height:100%}
  #root{height:100vh;display:flex;flex-direction:column;gap:10px;padding:10px;max-width:1700px;margin:0 auto}
  .trees{display:grid;grid-template-columns:1fr 1fr 1fr;gap:10px;flex:0 0 auto}
  .trees img{width:100%;aspect-ratio:16/9;object-fit:contain;display:block;border-radius:4px}
  .panel{background:var(--edge);border-radius:10px;overflow:hidden;padding:0 5px 5px;display:flex;flex-direction:column;min-height:0}
  .panel>.titlebar{color:#fff;font:italic 400 16px/15px 'Weyard',system-ui,sans-serif;text-transform:uppercase;letter-spacing:.06em;display:flex;align-items:center;height:18px;padding:0.5px 5px 0;flex:0 0 auto}
  .panel>*:not(.titlebar){border-radius:5px}
  .p-core{--edge:#6d4fc2}
  .p-overlays{--edge:#1f7f93}
  .p-assets{--edge:#bb2f77}
  .p-chat{--edge:#6e6e6e}
  .p-board{--edge:#2e5c42}
  .main{display:grid;grid-template-columns:minmax(0,1fr) clamp(320px,28%,460px);gap:10px;flex:1;min-height:0}
  .sprite{image-rendering:pixelated}
  /* kanban */
  .board{display:flex;flex-direction:column;background:#3f7d5a;padding:8px 10px;min-height:0;flex:1}
  .cols{display:grid;grid-auto-flow:column;grid-auto-columns:1fr;gap:8px;flex:1;min-height:0}
  .col{display:flex;flex-direction:column;min-height:0}
  .col .name{font:italic 400 16px/15px 'Weyard',system-ui,sans-serif;color:#dcebe2;text-transform:uppercase;letter-spacing:.06em;margin-bottom:5px}
  .col .stack{display:flex;flex-direction:column;gap:6px;overflow-y:auto;min-height:0;padding:1px 2px 6px}
  .postit{padding:8px 9px 7px;font:italic 400 16px/15px 'Weyard',system-ui,sans-serif;color:#333;position:relative;z-index:0}
  .postit::before{content:"";position:absolute;inset:0;z-index:-1;background:#fffef8;border-radius:2px;box-shadow:0 2px 4px rgba(0,0,0,.35);transform:rotate(-2.2deg)}
  .postit:nth-child(even)::before{transform:rotate(1.8deg)}
  .postit:nth-child(3n)::before{transform:rotate(-1.2deg)}
  .postit:nth-child(5n)::before{transform:rotate(2.6deg)}
  .postit .owners{display:flex;gap:3px;justify-content:flex-end;margin-top:4px}
  .postit .owners img{width:20px;height:20px;border-radius:3px}
  /* chat */
  .chat{background:#f6f6f6;padding:10px 14px;overflow-y:auto;min-height:0;display:flex;flex-direction:column;gap:10px;flex:1}
  .msg{display:flex;gap:10px;align-items:flex-start}
  .msg .ava{width:36px;height:36px;flex:none;border-radius:6px;overflow:hidden}
  .msg .ava img{width:36px;height:36px;display:block}
  .msg .ava svg{width:36px;height:36px}
  .msg .body{font:italic 400 16px/15px 'Weyard',system-ui,sans-serif;color:#222;white-space:pre-wrap;min-width:0}
  .msg .head{font:italic 400 16px/15px 'Weyard',system-ui,sans-serif;color:#111;margin-bottom:1px}
  .msg .head .time{color:#999;margin-left:8px;font-size:16px}
  @media (max-width:760px){
    #root{height:auto;padding:8px;gap:8px}
    .trees{grid-template-columns:1fr;gap:8px}
    .main{display:flex;flex-direction:column;gap:8px;min-height:auto}
    .panel{min-height:auto}
    .board{flex:none}
    .cols{display:grid;grid-auto-flow:column;grid-auto-columns:72vw;overflow-x:auto;flex:none;padding-bottom:4px}
    .col .stack{overflow-y:visible;max-height:none}
    .p-chat .chat{flex:none;max-height:65vh}
  }
</style></head><body><div id="root"></div><script>
const h=(tag,attrs={},...kids)=>{
  const e=document.createElement(tag);
  for(const [k,v] of Object.entries(attrs)) e.setAttribute(k,v);
  for(const k of kids.flat()) e.appendChild(typeof k==='string'?document.createTextNode(k):k);
  return e;
};
const TREES=['core','overlays','assets'];
const SERVED='${BOOT}';
function ago(ts){
  const d=new Date(ts);if(isNaN(d.getTime()))return null;
  const s=(Date.now()-d.getTime())/1000;
  if(s<60)return 'just now';
  if(s<3600)return Math.floor(s/60)+'m ago';
  if(s<86400)return Math.floor(s/3600)+'h ago';
  return Math.floor(s/86400)+'d ago';
}
setInterval(()=>{document.querySelectorAll('.time').forEach(e=>{const t=ago(e.getAttribute('data-ts'));if(t)e.textContent=t;})},30000);
const CAST={vale:'Kraden',mercury:'Mia',venus:'Isaac',mars:'Garet',jupiter:'Ivan'};
const avatar=(who,cls)=>{
  if(who==='vale'){const s=h('span',{class:cls||''});s.innerHTML=window.KRADEN;return s.firstElementChild?s:s;}
  return h('img',{class:'sprite '+(cls||''),src:'/avatar/'+who+'.png',alt:CAST[who]||who});
};
function parseChat(text){
  const msgs=[];let cur=null;
  for(const line of text.split('\\n')){
    const m=line.match(/^###\\s+(.+?)\\s+[—–-]+\\s+@([A-Za-z]+)/);
    if(m){cur={time:m[1],who:m[2].toLowerCase(),lines:[]};msgs.push(cur);}
    else if(cur)cur.lines.push(line);
  }
  return msgs;
}
function msgEl(m){
  const who=m.who,name=CAST[who]||who;
  const ava=h('div',{class:'ava'});
  ava.appendChild(h('img',{class:'sprite',src:'/avatar/'+who+'.png?v='+(window.AVA_V||0),alt:name}));
  return h('div',{class:'msg'},ava,
    h('div',{},
      h('div',{class:'head'},name+' · @'+who,h('span',{class:'time','data-ts':m.ok?m.time:'',title:m.time},m.ok?(ago(m.time)||m.time):'⏱ unstamped')),
      h('div',{class:'body'},m.text||'')));
}
function parseKanban(text){
  const all=text.split('\\n');
  const caption=(all.find(l=>l.startsWith('#'))||'').replace(/^#+\\s*/,'');
  const rows=all.filter(l=>l.trim().startsWith('|')&&!/^\\|[\\s:|-]+\\|$/.test(l.trim()))
    .map(l=>l.split('|').slice(1,-1).map(c=>c.trim()));
  if(rows.length<1)return{caption,cols:[]};
  const heads=rows[0];
  const cols=heads.map((t,i)=>({title:t,cards:rows.slice(1).map(r=>r[i]||'').filter(c=>c&&c!=='—'&&c!=='-')}));
  return{caption,cols};
}
function postit(text){
  const owners=[...text.matchAll(/@([a-z]+)/g)].map(m=>m[1]).filter(w=>CAST[w]);
  const clean=text.replace(/\`@[a-z]+\`/g,'').replace(/@[a-z]+/g,'').replace(/\`/g,'').replace(/\\s{2,}/g,' ').trim();
  const kids=[document.createTextNode(clean)];
  const card=h('div',{class:'postit'},clean);
  if(owners.length){
    const row=h('div',{class:'owners'});
    for(const o of [...new Set(owners)])
      row.appendChild(h('img',{class:'sprite',src:'/avatar/'+o+'.png?v='+(window.AVA_V||0),alt:o}));
    card.appendChild(row);
  }
  return card;
}
let last='';
async function tick(){
  try{
    const mt=await (await fetch('/mtimes')).json();
    if(mt.page&&mt.page!==SERVED){location.reload();return;}
    const key=JSON.stringify(mt);
    if(key!==last){
      last=key;
      const kb=parseKanban(await (await fetch('/kanban')).text());
      const msgs=await (await fetch('/chat')).json();
      let prev=0;
      for(const m of msgs){
        const t=new Date(m.time).getTime();
        m.ok=!isNaN(t)&&t<Date.now()+120000&&t>=prev-60000;
        if(m.ok)prev=t;
      }
      window.AVA_V=mt.avatars||0;
      const chatEl=h('div',{class:'chat'},msgs.map(msgEl));
      document.getElementById('root').replaceChildren(
        h('div',{class:'trees'},TREES.map(t=>h('div',{class:'panel p-'+t},
          h('div',{class:'titlebar'},({core:'Core ROM — asm › semantic › exact › humanized',overlays:'Overlays — asm › semantic › exact › humanized',assets:'Assets — bytes › b&w › color › objects'})[t]),
          h('img',{src:'/svg/'+t+'?t='+mt[t],alt:t})))),
        h('div',{class:'main'},
          h('div',{class:'panel p-board'},
            h('div',{class:'titlebar'},kb.caption),
            h('div',{class:'board'},
            h('div',{class:'cols'},kb.cols.map(c=>
              h('div',{class:'col'},
                h('div',{class:'name'},c.title),
                h('div',{class:'stack'},c.cards.map(postit))))))),
          h('div',{class:'panel p-chat'},h('div',{class:'titlebar'},'Team chat — #alchemy'),chatEl)));
      chatEl.scrollTop=chatEl.scrollHeight;
    }
  }catch(e){}
  setTimeout(tick,2000);
}
tick();
</script></body></html>`;

Bun.serve({
  port: 4649,
  fetch(request) {
    const path = new URL(request.url).pathname;
    const tree = path.startsWith("/svg/") ? path.slice(5).replace(/\W/g, "") : undefined;
    if (tree && (TREES as readonly string[]).includes(tree))
      return new Response(Bun.file(svgFile(tree)), { headers: { "content-type": "image/svg+xml", "cache-control": "no-store" } });
    if (path === "/avatar/vale.svg")
      return new Response(KRADEN_SVG, { headers: { "content-type": "image/svg+xml", "cache-control": "max-age=3600" } });
    const who = /^\/avatar\/(\w+)\.png$/.exec(path)?.[1];
    if (who && PORTRAITS[who])
      return new Response(Bun.file(join(ROOT, "assets", "graphics", PORTRAITS[who])), { headers: { "content-type": "image/png", "cache-control": "no-store" } });
    if (path === "/weyard.otf")
      return new Response(Bun.file(join(ROOT, "assets", "fonts", "weyard.otf")), { headers: { "content-type": "font/otf", "cache-control": "max-age=300" } });
    if (path === "/kanban")
      return new Response(existsSync(KANBAN) ? Bun.file(KANBAN) : "no kanban yet", { headers: { "content-type": "text/plain; charset=utf-8", "cache-control": "no-store" } });
    if (path === "/chat")
      return Response.json(readChatCsv(), { headers: { "cache-control": "no-store" } });
    if (path === "/mtimes")
      return Response.json(mtimes(), { headers: { "cache-control": "no-store" } });
    return new Response(pageHtml(), { headers: { "content-type": "text/html; charset=utf-8", "cache-control": "no-store" } });
  },
});

function readChatCsv(): { time: string; who: string; text: string }[] {
  if (!existsSync(CHAT)) return [];
  const raw = readFileSync(CHAT, "utf8");
  const rows: string[][] = [];
  let field = "", row: string[] = [], quoted = false;
  for (let i = 0; i < raw.length; i++) {
    const ch = raw[i];
    if (quoted) {
      if (ch === '"') { if (raw[i + 1] === '"') { field += '"'; i++; } else quoted = false; }
      else field += ch;
    } else if (ch === '"') quoted = true;
    else if (ch === ",") { row.push(field); field = ""; }
    else if (ch === "\n") { row.push(field); field = ""; if (row.some((c) => c !== "")) rows.push(row); row = []; }
    else if (ch !== "\r") field += ch;
  }
  if (field !== "" || row.length) { row.push(field); if (row.some((c) => c !== "")) rows.push(row); }
  return rows.slice(1).map(([time, who, text]) => ({ time: time ?? "", who: (who ?? "").toLowerCase(), text: text ?? "" }));
}

function mtimes() {
  return Object.fromEntries(
    TREES.map((tree) => {
      const path = svgFile(tree);
      return [tree, existsSync(path) ? statSync(path).mtimeMs : 0] as [string, number];
    }).concat([
      ["kanban", existsSync(KANBAN) ? statSync(KANBAN).mtimeMs : 0],
      ["chat", existsSync(CHAT) ? statSync(CHAT).mtimeMs : 0],
      ["page", BOOT],
      ["avatars", Object.values(PORTRAITS).reduce((a, f) => { const p = join(ROOT, "assets", "graphics", f); return a + (existsSync(p) ? statSync(p).mtimeMs : 0); }, 0)],
    ]),
  );
}

console.log("box-tree dashboard on http://localhost:4649");
