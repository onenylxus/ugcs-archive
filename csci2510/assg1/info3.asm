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
FormatPrint db "Decimal:%u", 10, "Hexadecimal:%x", 10, "Octal:%o", 0
number dw 2022
.code
start:
invoke crt_printf, addr FormatPrint, number, number, number
invoke ExitProcess, NULL
end start