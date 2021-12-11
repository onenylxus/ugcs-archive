.386
.model flat, stdcall
option casemap:none
include windows.inc
include kernel32.inc
include user32.inc
.data
MsgCaption db "Song Window", 0
MsgBoxText db "Lemon Tree", 0
.code
start:
invoke MessageBox , NULL,addr MsgBoxText , addr MsgCaption , MB_OK
invoke ExitProcess, NULL
end start