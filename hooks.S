.intel_syntax noprefix
.text
# Save full CPU register state, including FPU/SSE state. DF clear for C.
# EAX/ECX/EDX etc are restored before the stolen instructions execute.
.macro hook name, id
.if \id != 2
.globl _\name
_\name:
.endif
 pushfd
 .if (\id != 0) && (\id != 1) && (\id != 2) && (\id != 27) && (\id != 28) && (\id != 29) && (\id != 30) && (\id != 31) && (\id != 32) && (\id != 33) && (\id != 34) && (\id != 35) && (\id != 36)
 cmp dword ptr [_active], 0
 je 1f
 .endif
 pushad
 mov eax, esp
 sub esp, 528
 and esp, -16
 mov [esp+512], eax
 fxsave [esp]
 fninit
 cld
 push eax
 push \id
 call _observe
 add esp, 8
 fxrstor [esp]
 mov esp, [esp+512]
 popad
1:
 popfd
.endm
hook h_frame, 0
 sub esp,0x20
 push ebx
 push ebp
 push esi
 push edi
 jmp dword ptr [_back]
hook h_root,1
 mov eax,[ecx]
 lea edx,[esp+0x14]
 push edx
 push ebp
 jmp dword ptr [_back+4]
.globl _h_root_end
_h_root_end:
 call dword ptr [eax+0x24]
 hook h_root_end,2
 mov al,[esi+0x60]
 jmp dword ptr [_back+8]
.globl _h_surface
_h_surface:
 pushfd
 cmp dword ptr [_active],0
 jne 5f
 cmp dword ptr [_indicator_scope],0
 jne 5f
 popfd
 jmp _native_surface
5:
 popfd
 jmp _scale_surface
.globl _native_surface
_native_surface:
 push ecx
 mov eax,[esp+0x24]
 jmp dword ptr [_back+12]
hook h_input,4
 mov eax,dword ptr [0x6e7b88]
 jmp dword ptr [_back+16]
hook h_ownership,5
 push ecx
 add eax,esi
 push edx
 push eax
 jmp dword ptr [_back+20]
.globl _h_font
_h_font:
 pushfd
 cmp dword ptr [_active],0
 jne 5f
 popfd
 jmp _native_font
5:
 popfd
 jmp _scale_font
.globl _native_font
_native_font:
 sub esp,0x1c
 mov eax,[esp+0x2c]
 jmp dword ptr [_back+24]

hook h_map_pass,7
 test ebp,ebp
 jz 2f
 jmp dword ptr [_map_alt]
2:
 fld dword ptr [esi+0x120]
 jmp dword ptr [_back+28]
# Fast reject all unrelated 2D/3D submissions before saving the full CPU state.
.globl _h_map_quad
_h_map_quad:
 pushfd
 cmp dword ptr [esp+4],0x4a1a1c
 je 3f
 cmp dword ptr [esp+4],0x4a1a2f
 je 3f
 cmp dword ptr [esp+4],0x4a1c00
 je 3f
 cmp dword ptr [esp+4],0x4a1cdc
 je 3f
 popfd
 jmp 4f
3:
 popfd
hook h_map_quad_filtered,8
4:
 sub esp,8
 push ebx
 mov ebx,[esp+0x18]
 jmp dword ptr [_back+32]
hook h_map_input,9
 fld dword ptr [esp+0x30]
 fsub dword ptr [esp+0x24]
 jmp dword ptr [_back+36]

hook h_menu_input,10
 push ebp
 push esi
 mov esi,ecx
 mov al,[esi+0x2d]
 jmp dword ptr [_back+40]
.globl _h_menu_contain
_h_menu_contain:
 jmp _scale_menu_contain
.globl _native_menu_contain
_native_menu_contain:
 mov al,[ecx+0x2d]
 sub esp,0x20
 jmp dword ptr [_back+44]
hook h_menu_point,12
 call dword ptr [0x67f30c]
 jmp dword ptr [_back+48]

hook h_cursor,13
 mov eax,[esp+0x10]
 mov ecx,[esi+0x50]
 jmp dword ptr [_back+52]

hook h_window_local,14
 mov eax,[esp+0x1c]
 mov edx,[ecx]
 push eax
 jmp dword ptr [_back+56]
hook h_window_drag_start,15
 mov edx,[esp+0x10]
 mov eax,[esp+0x14]
 jmp dword ptr [_back+60]
hook h_window_move,16
 mov eax,[esp+4]
 mov edx,[ecx+0x50]
 jmp dword ptr [_back+64]
# Read-only font-path diagnostics. Original entry instructions replayed.
hook h_font_path_a,17
 sub esp,0x54
 push ebx
 push ebp
 push esi
 jmp dword ptr [_back+68]
hook h_font_path_b,18
 sub esp,0x5c
 push ebx
 push ebp
 push esi
 push edi
 jmp dword ptr [_back+72]

# Original DrawTextA call sites, with same stack and stdcall argument cleanup.
.globl _h_drawtext_a
_h_drawtext_a:
 pushfd
 cmp dword ptr [_active],0
 jne 5f
 popfd
 call dword ptr [0x67f2b4]
 jmp dword ptr [_back+76]
5:
 popfd
 call _hi_drawtext@20
 jmp dword ptr [_back+76]
.globl _h_drawtext_b
_h_drawtext_b:
 pushfd
 cmp dword ptr [_active],0
 jne 5f
 popfd
 call dword ptr [0x67f2b4]
 jmp dword ptr [_back+80]
5:
 popfd
 call _hi_drawtext@20
 jmp dword ptr [_back+80]

# Tabbed statistics windows: map second dispatch, not raw drag arguments.
hook h_tab_input,21
 sub ebx,edx
 mov edx,[esi+0xa8]
 jmp dword ptr [_back+84]
.globl _h_tab_contain
_h_tab_contain:
 jmp _scale_tab_contain
.globl _native_tab_contain
_native_tab_contain:
 mov al,[ecx+0x2d]
 sub esp,0x20
 jmp dword ptr [_back+88]
hook h_tab_point,12
 call dword ptr [0x67f30c]
 jmp dword ptr [_back+92]
# Render-specific rect reads. Original CPU/FPU state restored, scoped geometry restored at layer end.
hook h_adv_preview,22
 mov ecx,[ebp+0xe8]
 jmp dword ptr [_back+96]
hook h_mon_preview,23
 mov ecx,[esi+0xe4]
 jmp dword ptr [_back+100]
hook h_profit_graph,24
 fild dword ptr [esp+0x18]
 push edi
 jmp dword ptr [_back+104]
hook h_king_graph,25
 mov eax,[ebx+0x9c]
 jmp dword ptr [_back+108]
hook h_needs_graph,26
 mov eax,[esi+0xa4]
 jmp dword ptr [_back+112]
# Native HUD keyboard dispatch; independent of UI scaling state.
.globl _h_layer_key
_h_layer_key:
 jmp _game_layer_key
.globl _native_layer_key
_native_layer_key:
 mov eax,dword ptr [0x6e7b88]
 jmp dword ptr [_back+116]
.globl _h_button_key
_h_button_key:
 jmp _game_button_key
.globl _native_button_key
_native_button_key:
 push esi
 mov esi,ecx
 mov al,[esi+0x14]
 jmp dword ptr [_back+120]

# Always run camera hooks, including native UI mode.
hook h_camera_begin,27
 mov esi,[esp+0x10]
 mov eax,[ebp+0x6c]
 jmp dword ptr [_back+124]
hook h_camera_end,28
 mov ecx,[ebp+0x84]
 jmp dword ptr [_back+128]
.globl _h_camera_event
_h_camera_event:
 jmp _camera_event
.globl _native_camera_event
_native_camera_event:
 push ecx
 fld dword ptr [0x67fc78]
 jmp dword ptr [_back+132]

# Original cdecl mouse message handler, relative call reconstructed.
.globl _h_mouse_event
_h_mouse_event:
 jmp _mouse_event
.globl _native_mouse_event
_native_mouse_event:
 sub esp,0x18
 mov eax,0x4c7ba0
 call eax
 jmp dword ptr [_back+136]
# The native dynamic_cast and destroyed-building check already succeeded.
hook h_research_sample,29
 mov ecx,[ebp+0x20]
 mov eax,0x5edbd0
 call eax
 jmp dword ptr [_back+140]
hook h_gold_sample,30
 sub esp,8
 push esi
 mov esi,ecx
 jmp dword ptr [_back+144]

# Reset on native camera initialization (new level/load).
hook h_bookmark_init,31
 push ebx
 push ebp
 push esi
 push edi
 mov eax,ecx
 jmp dword ptr [_back+148]
# Replace only the native HUD Home setter call, preserving thiscall cleanup.
.globl _h_bookmark_home
_h_bookmark_home:
 call _bookmark_home
 jmp dword ptr [_back+152]

# Native upgrade-button render wrapper (thiscall, 3 arguments).
.globl _h_upgrade_draw
_h_upgrade_draw:
 jmp _upgrade_draw
.globl _native_upgrade_draw
_native_upgrade_draw:
 push esi
 mov esi,ecx
 mov eax,[esi+8]
 jmp dword ptr [_back+156]
# Native cheat parser (cdecl, character event pointer).
.globl _h_cheat_event
_h_cheat_event:
 jmp _cheat_event
.globl _native_cheat_event
_native_cheat_event:
 mov eax,[esp+4]
 mov ecx,[eax]
 jmp dword ptr [_back+160]

# Read-only lifecycle entry/callsite diagnostics, independent of UI scaling.
hook h_gold_credit,32
 sub esp,8
 fld dword ptr [esp+0xc]
 jmp dword ptr [_back+164]
hook h_research_complete,33
 mov eax,dword ptr fs:[0]
 jmp dword ptr [_back+168]
hook h_research_start,34
 mov eax,dword ptr [0x6e7b88]
 jmp dword ptr [_back+172]

hook h_research_live,35
 mov al,[ebp+0x780]
 jmp dword ptr [_back+176]
hook h_money_keyboard,36
 lea eax,[esp+0x20]
 push eax
 jmp dword ptr [_back+180]

# Shared native save/load wrapper. AL is the native success result; ret 8.
.globl _h_game_save
_h_game_save:
 jmp _game_save
.globl _native_game_save
_native_game_save:
 sub esp,8
 push ebx
 mov ebx,[esp+0x14]
 jmp dword ptr [_back+184]

