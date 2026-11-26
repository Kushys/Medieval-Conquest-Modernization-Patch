import pathlib,struct,subprocess,csv,re,hashlib
P=pathlib.Path(__file__).resolve().parent
# COFF short import libraries with undecorated DLL entry names, stdcall symbols.
imports={'KERNEL32.dll':{'ReadFile':20,'GetFileSize':8,'GetFileTime':16,'FlushFileBuffers':4,'MoveFileExA':12,'DeleteFileA':4,'GetModuleHandleA':4,'VirtualProtect':16,'FlushInstructionCache':12,'GetCurrentProcess':0,'ReadProcessMemory':20,'CreateFileA':28,'WriteFile':20,'CloseHandle':4,'GetTickCount':0,'GetCurrentProcessId':0,'GetCurrentThreadId':0,'GetModuleFileNameA':12,'DisableThreadLibraryCalls':4,'GetPrivateProfileStringA':24,'GetPrivateProfileIntA':16,'WritePrivateProfileStringA':16},'USER32.dll':{'GetAsyncKeyState':4,'GetForegroundWindow':0,'GetWindowThreadProcessId':8,'GetCursorPos':4,'ScreenToClient':8,'GetClientRect':8,'DrawTextA':20},'GDI32.dll':{'CreateCompatibleDC':4,'CreateDIBSection':24,'SelectObject':8,'GetCurrentObject':8,'GetObjectA':12,'CreateFontIndirectA':4,'DeleteObject':4,'DeleteDC':4,'SetBkMode':8,'SetTextColor':8,'GdiFlush':0,'GetTextExtentExPointA':28,'ExtTextOutA':32}}
B=P/'build';B.mkdir(exist_ok=True)
libs=[]
for dll,entries in imports.items():
 objs=[]
 for n,a in entries.items():
  s=f'_{n}@{a}\0{dll}\0'.encode();o=B/(n+'.obj');o.write_bytes(struct.pack('<HHHHIIHH',0,65535,0,0x14c,0,len(s),0,12)+s);objs.append(str(o))
 lib=B/(dll+'.lib');subprocess.run(['llvm-ar','rcs',str(lib),*objs],check=True);libs.append(str(lib))
for n in ['modernization.c','hooks.S']:
 subprocess.run(['clang','--target=i686-pc-windows-msvc','-O2','-ffreestanding','-fno-stack-protector','-fno-builtin','-mno-sse','-mno-sse2','-c',str(P/n),'-o',str(B/(n+'.obj'))],check=True)
subprocess.run(['lld-link','/dll','/entry:DllMain@12','/nodefaultlib','/machine:x86','/safeseh:no','/dynamicbase','/nxcompat','/out:'+str(P/'MC_Modernization.asi'),str(B/'modernization.c.obj'),str(B/'hooks.S.obj'),*libs],check=True)
print('SHA256',hashlib.sha256((P/'MC_Modernization.asi').read_bytes()).hexdigest())
