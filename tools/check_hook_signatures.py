"""Check hook signatures against the user-supplied executable."""
import re,struct,pathlib,hashlib,sys
p=pathlib.Path(__file__).resolve().parents[1]
exe=pathlib.Path(sys.argv[1]).read_bytes(); pe=struct.unpack_from('<I',exe,0x3c)[0]; n=struct.unpack_from('<H',exe,pe+6)[0]; opt=struct.unpack_from('<H',exe,pe+20)[0]; sections=[]
assert exe[pe:pe+4]==b'PE\0\0' and struct.unpack_from('<H',exe,pe+4)[0]==0x14c
assert struct.unpack_from('<I',exe,pe+24+28)[0]==0x400000
for i in range(n):
 o=pe+24+opt+40*i; name=exe[o:o+8].rstrip(b'\0');vs,va,sz,raw=struct.unpack_from('<IIII',exe,o+8);sections.append((name,va,sz,raw))
for at,fn,length,bs in re.findall(r'\{(0x[0-9a-f]+),(h_\w+),(\d+),\{([^}]+)\}',(p/'modernization.c').read_text()):
 a=int(at,16); b=bytes(int(x,0) for x in bs.split(',')); rva=a-0x400000; sec=next(s for s in sections if s[1]<=rva<s[1]+s[2]); f=sec[3]+rva-sec[1]; actual=exe[f:f+int(length)]; assert b==actual,(at,fn,b.hex(),actual.hex()); print(at,fn,len(b),actual.hex(), 'PASS')
print('baseline SHA256',hashlib.sha256(exe).hexdigest())
