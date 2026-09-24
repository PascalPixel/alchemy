pub(super) const CLIENT: &str = r####"// Browser-native JavaScript emitted by Rust; no build dependency, copied game bytes or remote assets.
const canvas = document.querySelector('#map');
const context = canvas.getContext('2d');
const plane = document.createElement('canvas');
const scene = document.querySelector('#scene');
const mode = document.querySelector('#mode');
const palette = document.querySelector('#palette');
const filter = document.querySelector('#filter');
const status = document.querySelector('#map-status');
const inspector = document.querySelector('#inspect');
const game = document.querySelector('.map-live').dataset.game;
let map, pixels, family, overview, overviewSource, lut, visible = [], zoom = 1, x = 0, y = 0, drag, request;
const tables = new Map();
let bounds = {left:0, top:0, width:1, height:1};
// Room labels sit in map pixels, which are game pixels: glyphs cut from the
// served sheet (two device pixels per game pixel, sixteen frames a row).
const lettersMeta = document.querySelector('meta[name=letters]');
const letterAdvances = lettersMeta ? lettersMeta.dataset.advances.split(',').map(Number) : [];
const letterSheet = new Image();
if(lettersMeta) { letterSheet.src = lettersMeta.content; letterSheet.onload = () => draw(); }
const letterCell = lettersMeta ? Number(lettersMeta.dataset.cell) : 8, letterScale = lettersMeta ? Number(lettersMeta.dataset.scale) : 2;
// The menu font's frames are character codes; a code it lacks draws as '?'.
function knownFrame(code) { return code>=32&&code<=255&&code!==92&&!(code>=127&&code<160)&&letterAdvances[code]>1; }
function letterFrame(c) { const code=c.codePointAt(0); return knownFrame(code) ? code : 63; }
function lettersWidth(text) { let w=0; for(const c of text) w+=letterAdvances[letterFrame(c)]||0; return w; }
// The server's glyph-run markup for text written after load: the same
// sprite classes, the text kept for readers.
function letters(text) {
  const run=document.createElement('span');run.className='t u';
  const hidden=document.createElement('span');hidden.className='sr';hidden.textContent=text;
  const shown=document.createElement('span');shown.setAttribute('aria-hidden','true');
  if(!lettersMeta) return document.createTextNode(text);
  let word=document.createElement('b');
  for(const c of text){
    const code=c.codePointAt(0),known=knownFrame(code);
    if(known){const glyph=document.createElement('i');glyph.className='g'+code.toString(16).padStart(2,'0');word.append(glyph)}
    else{const fallback=document.createElement('span');fallback.className='f';fallback.textContent=c;word.append(fallback)}
    if(c===' '){shown.append(word);word=document.createElement('b')}
  }
  if(word.childNodes.length)shown.append(word);
  run.append(hidden,shown);return run;
}
function setLetters(element,text){element.replaceChildren(letters(text))}
function drawLetters(text, left, top) {
  if(!letterSheet.complete||!letterSheet.naturalWidth) return;
  let at=left; for(const c of text) { const frame=letterFrame(c); const size=letterCell*letterScale; context.drawImage(letterSheet, frame%16*size, Math.floor(frame/16)*size, size, size, at, top+(16-letterCell)/2, letterCell, letterCell); at+=letterAdvances[frame]||0; }
}
const raw = color => [color & 31, (color >> 5) & 31, (color >> 10) & 31];
function shown([r,g,b]) { if(!lut) return [r<<3,g<<3,b<<3]; const at=(r|g<<5|b<<10)*3; return [lut[at],lut[at+1],lut[at+2]]; }
function filterImage(source) {
  if(!lut) return source;
  const out=new OffscreenCanvas(source.width,source.height),context=out.getContext('2d');context.drawImage(source,0,0);
  const image=context.getImageData(0,0,out.width,out.height),data=image.data;
  for(let i=0;i<data.length;i+=4){if(!data[i+3])continue;const color=shown([data[i]>>3,data[i+1]>>3,data[i+2]>>3]);data[i]=color[0];data[i+1]=color[1];data[i+2]=color[2]}
  context.putImageData(image,0,0);return out;
}

function worldPoint(px, pz, floor=0) { return {x:.85*px-.22*pz, y:.32*px+.48*pz-floor*96}; }
function worldBounds(rooms) {
  const points=[];
  for(const room of rooms) for(const [dx,dz] of [[0,0],[room.width,0],[0,room.height],[room.width,room.height]]) points.push(worldPoint(room.x+dx,room.z+dz,room.floor||0));
  const left=Math.min(...points.map(p=>p.x)),top=Math.min(...points.map(p=>p.y));
  const right=Math.max(...points.map(p=>p.x)),bottom=Math.max(...points.map(p=>p.y));
  return {left,top,width:Math.max(1,right-left),height:Math.max(1,bottom-top)};
}
function draw() {
  context.setTransform(1,0,0,1,0,0);context.clearRect(0,0,canvas.width,canvas.height);context.imageSmoothingEnabled=false;
  if(mode.value==='scene' && map) {
    context.drawImage(plane,x,y,map.width*zoom,map.height*zoom);
    if(document.querySelector('#grid').checked && zoom>=.5) {
      context.strokeStyle='#ffffff55';context.beginPath();const step=16*zoom;
      for(let i=Math.max(0,Math.ceil(-x/step));i<=map.width/16&&x+i*step<canvas.width;i++){context.moveTo(x+i*step,y);context.lineTo(x+i*step,y+map.height*zoom)}
      for(let i=Math.max(0,Math.ceil(-y/step));i<=map.height/16&&y+i*step<canvas.height;i++){context.moveTo(x,y+i*step);context.lineTo(x+map.width*zoom,y+i*step)}context.stroke();
    }
  } else if(mode.value==='network' && overview) {
    context.drawImage(overview,x,y,bounds.width*zoom,bounds.height*zoom);
  } else if(mode.value==='world' && family) {
    const rooms=[...family.rooms].sort((a,b)=>(a.floor||0)-(b.floor||0)||a.z_index-b.z_index||a.z-b.z||a.x-b.x);
    for(const room of rooms) {
      const at=worldPoint(room.x,room.z,room.floor||0);
      context.save();context.setTransform(.85*zoom,.32*zoom,-.22*zoom,.48*zoom,x+(at.x-bounds.left)*zoom,y+(at.y-bounds.top)*zoom);context.drawImage(room.image,0,0);context.restore();
    }
    context.save();context.translate(x,y);context.scale(zoom,zoom);
    for(const room of rooms) {
      const at=worldPoint(room.x,room.z,room.floor||0),label=room.label;
      const left=Math.round(at.x-bounds.left),top=Math.round(at.y-bounds.top)-16;context.fillStyle='#103840dd';context.fillRect(left,top,lettersWidth(label)+4,16);drawLetters(label,left+2,top);
    }
    context.restore();
  }
}
function composite() {
  if(!map)return;const out=new Uint8ClampedArray(map.width*map.height*4);
  const order=map.layers.map((_,i)=>i).sort((a,b)=>map.layers[b].priority-map.layers[a].priority||map.layers[b].bg-map.layers[a].bg);
  const color=value=>raw(map.palettes[Number(palette.value)<0?value>>4:Number(palette.value)][value&15]);
  for(let at=0;at<map.width*map.height;at++) {
    let front,behind;for(const i of order)if(visible[i]&&pixels[i][at]){behind=front;front={value:pixels[i][at],bg:map.layers[i].bg}}if(!front)continue;
    let result=color(front.value),alpha=255,blend=map.blend;
    if(blend&&((blend.control>>front.bg)&1)){const eva=Math.min(16,blend.alpha&31),evb=Math.min(16,(blend.alpha>>8)&31);if(!behind||((blend.control>>(8+behind.bg))&1)){const under=behind?color(behind.value):[0,0,0];result=result.map((c,i)=>Math.min(31,Math.floor((c*eva+under[i]*evb)/16)));if(!behind)alpha=Math.floor(eva*255/16)}}out.set([...shown(result),alpha],at*4);
  }
  plane.width=map.width;plane.height=map.height;plane.getContext('2d').putImageData(new ImageData(out,map.width,map.height),0,0);draw();
}
function fit(){zoom=Math.min(canvas.width/bounds.width,canvas.height/bounds.height)*.94;x=(canvas.width-bounds.width*zoom)/2;y=(canvas.height-bounds.height*zoom)/2;draw()}
function scale(next,px=canvas.width/2,py=canvas.height/2){next=Math.max(.03,Math.min(16,next));x=px-(px-x)*next/zoom;y=py-(py-y)*next/zoom;zoom=next;draw()}
function sceneControls(show){for(const control of document.querySelectorAll('.scene-control'))control.hidden=!show}
const progress=document.querySelector('#map-progress');
// The bar sweeps while the server assembles, then fills as the payload arrives.
async function download(url,signal,label){
  progress.hidden=false;progress.classList.add('busy');progress.firstElementChild.style.width='';setLetters(status,`${label}…`);
  const response=await fetch(url,{signal,cache:'no-store'});if(!response.ok)throw Error(await response.text());
  const total=Number(response.headers.get('Content-Length'))||0;if(!total||!response.body)return response.arrayBuffer();
  progress.classList.remove('busy');const bytes=new Uint8Array(total),reader=response.body.getReader();let at=0;
  for(;;){const {done,value}=await reader.read();if(done)break;if(at+value.length>total)throw Error('Payload exceeds its length');bytes.set(value,at);at+=value.length;const percent=Math.round(at*100/total);progress.firstElementChild.style.width=`${percent}%`;progress.setAttribute('aria-valuenow',String(percent));setLetters(status,`${label}… ${percent}%`)}
  return bytes.buffer;
}

async function loadScene(signal) {
  const bytes=await download(`/maps/${game}/${scene.value}/layers`,signal,'Decoding scene'),size=new DataView(bytes).getUint32(0,true);map=JSON.parse(new TextDecoder().decode(new Uint8Array(bytes,4,size)));const count=map.width*map.height;
  if(map.format!==1||!count||count>16777216||4+size+count*map.layers.length!==bytes.byteLength)throw Error('Invalid live layer payload');
  pixels=map.layers.map((_,i)=>new Uint8Array(bytes,4+size+i*count,count));const order=map.layers.map((_,i)=>i).sort((a,b)=>map.layers[b].priority-map.layers[a].priority||map.layers[b].bg-map.layers[a].bg),backdrop=order.find(i=>map.layers[i].opaque);
  visible=map.layers.map((l,i)=>!l.opaque||i===backdrop||Boolean(map.blend&&((map.blend.control>>l.bg)&1)));palette.replaceChildren(new Option('Loaded palettes','-1'),...map.palettes.map((_,i)=>new Option(`Bank ${i}`,String(i))));document.querySelector('#layers').replaceChildren();
  for(const [i,layer] of map.layers.entries()){const label=document.createElement('label'),checkbox=document.createElement('input');checkbox.type='checkbox';checkbox.checked=visible[i];checkbox.addEventListener('change',()=>{visible[i]=checkbox.checked;composite()});label.append(checkbox,letters(`BG${layer.bg} (priority ${layer.priority})`));document.querySelector('#layers').append(label)}
  bounds={left:0,top:0,width:map.width,height:map.height};setLetters(status,`Scene ${map.scene} · map ${map.container} · ${map.width} × ${map.height} · ${map.unresolved} unresolved tile references.`);composite();
}
async function loadFamily(signal) {
  const bytes=await download(`/maps/${game}/${scene.value}/family`,signal,'Assembling rooms'),size=new DataView(bytes).getUint32(0,true);family=JSON.parse(new TextDecoder().decode(new Uint8Array(bytes,4,size)));if(family.format!==1)throw Error('Invalid assembled map payload');
  const base=4+size,overviewBytes=new Uint8Array(bytes,base,family.overview_bytes);overviewSource=await createImageBitmap(new Blob([overviewBytes],{type:'image/png'}));overview=filterImage(overviewSource);const roomBase=base+family.overview_bytes;
  for(const room of family.rooms){const imageBytes=new Uint8Array(bytes,roomBase+room.offset,room.bytes);room.source=await createImageBitmap(new Blob([imageBytes],{type:'image/png'}));room.image=filterImage(room.source);room.label=`${room.scenes.join(' ')} · L${room.floor??'?'}`}
  bounds=mode.value==='network'?{left:0,top:0,width:overview.width,height:overview.height}:worldBounds(family.rooms);const report=family.report;setLetters(status,`Scene ${family.scene} · ${family.rooms.length} cut rooms · ${report.links.length} door links · ${new Set(family.rooms.map(r=>r.floor)).size} floors.`);
}
async function load(){request?.abort();request=new AbortController();const current=request;map=undefined;family=undefined;overview=undefined;overviewSource=undefined;draw();sceneControls(mode.value==='scene');try{if(mode.value==='scene')await loadScene(current.signal);else await loadFamily(current.signal);if(current!==request)return;history.replaceState(null,'',`/maps/${game}/${scene.value}`);document.querySelector('.refresh').href=`/maps/${game}/${scene.value}`;fit()}catch(error){if(error.name!=='AbortError')setLetters(status,`Cannot render map: ${error.message}`)}finally{if(current===request)progress.hidden=true}}

async function selectFilter() {
  const id=filter.value;try{localStorage.setItem('alchemy-map-filter',id)}catch{}
  if(id&&!tables.has(id)){const response=await fetch(`/maps/filter/${id}`,{cache:'no-store'});if(!response.ok)throw Error(await response.text());tables.set(id,new Uint8Array(await response.arrayBuffer()))}
  lut=id?tables.get(id):undefined;if(overviewSource)overview=filterImage(overviewSource);for(const room of family?.rooms??[])if(room.source)room.image=filterImage(room.source);composite();draw();
}
filter.addEventListener('change',()=>selectFilter().catch(error=>{setLetters(status,`Cannot apply filter: ${error.message}`)}));
scene.addEventListener('change',load);mode.addEventListener('change',load);palette.addEventListener('change',composite);document.querySelector('#grid').addEventListener('change',draw);document.querySelector('#fit').addEventListener('click',fit);document.querySelector('#actual').addEventListener('click',()=>scale(1));document.querySelector('#smaller').addEventListener('click',()=>scale(zoom/1.5));document.querySelector('#larger').addEventListener('click',()=>scale(zoom*1.5));
canvas.addEventListener('wheel',event=>{event.preventDefault();scale(zoom*Math.exp(-event.deltaY*.002),event.offsetX,event.offsetY)},{passive:false});canvas.addEventListener('pointerdown',event=>{drag={id:event.pointerId,x:event.clientX,y:event.clientY};canvas.setPointerCapture(event.pointerId)});canvas.addEventListener('pointerup',()=>{drag=undefined});canvas.addEventListener('pointercancel',()=>{drag=undefined});canvas.addEventListener('pointermove',event=>{if(drag){x+=event.clientX-drag.x;y+=event.clientY-drag.y;drag.x=event.clientX;drag.y=event.clientY;draw()}if(mode.value!=='scene'||!map)return;const px=Math.floor((event.offsetX-x)/zoom),py=Math.floor((event.offsetY-y)/zoom);if(px<0||py<0||px>=map.width||py>=map.height)return;setLetters(inspector,`Pixel ${px}, ${py} · cell ${Math.floor(px/16)}, ${Math.floor(py/16)} · `+map.layers.map((l,i)=>{const v=pixels[i][py*map.width+px];return `BG${l.bg}: palette ${v>>4}, index ${v&15}`}).join(' · '))});
canvas.addEventListener('keydown',event=>{if(!['+','=','-','0','ArrowLeft','ArrowRight','ArrowUp','ArrowDown'].includes(event.key))return;event.preventDefault();if(event.key==='0')fit();else if(event.key==='+'||event.key==='=')scale(zoom*1.5);else if(event.key==='-')scale(zoom/1.5);else{x+=event.key==='ArrowLeft'?32:event.key==='ArrowRight'?-32:0;y+=event.key==='ArrowUp'?32:event.key==='ArrowDown'?-32:0;draw()}});
new ResizeObserver(()=>{const box=canvas.parentElement.getBoundingClientRect();canvas.width=Math.floor(box.width);canvas.height=Math.floor(box.height);draw()}).observe(canvas.parentElement);
try{const saved=localStorage.getItem('alchemy-map-filter');if(saved&&[...filter.options].some(o=>o.value===saved))filter.value=saved}catch{}
await selectFilter().catch(error=>{filter.value='';lut=undefined;setLetters(status,`Cannot apply filter: ${error.message}`)});await load();
"####;
