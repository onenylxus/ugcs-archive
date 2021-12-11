.386
.model flat, stdcall
option casemap:none
include windows.inc
include kernel32.inc
include msvcrt.inc
include masm32.inc
includelib masm32.lib
includelib msvcrt.lib
includelib ucrtd.lib
.data
FormatPrint db "Nicholas", 10, "United College", 10, "Mathematics", 0
.code
start:
invoke StdOut, addr FormatPrint
invoke ExitProcess, NULL
end start