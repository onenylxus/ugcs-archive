.386
.model flat, stdcall
option casemap:none
include windows.inc
include kernel32.inc
include msvcrt.inc
includelib msvcrt.lib
includelib ucrtd.lib

.data
InputStatement db "Memory Address Access(input positive number or input -1:exit program): ", 0
MemoryAddressFormat  db "%d", 0
MemoryAddress dd "%d", 0
OutputFormat1 db "cache tag status: ", 0
OutputFormat2 db "%d ", 0
OutputFormat3 db 10 , 0
OutputFormat4 db "Accumulated Cache Hits: %d", 10, 0

CacheSize dd 64, 0 ; Number of total cache blocks
SetSize dd 4; Number of cache blocks in a set
CacheTags dd 64 dup(-1) ; Each cache block is associated with a tag 
;CacheBlocks dd 64 dup(-1) ; Please note that a cache should contain tags and 
						    ; data block, but we don't use blocks here for simplicity
CacheTimes dd 64 dup(-1)

BlockSize db 5, 0 ; Suppose this value is n. It means that the size of a block is 2^n bytes

CacheHits dd 0 ; Record the number of cache hits
CurrentTimeStamp dd 0 ; Record the current time stamp

.code
start:
	jmp input

input:
	mov EAX, CurrentTimeStamp 
	inc EAX
	mov CurrentTimeStamp, EAX ; update the time stamp
	mov EBP, offset CacheTags  ; Hint: EBP could be updated in the process, so initialize here
	mov ESI, offset CacheTimes ; Hint: ESI could be updated in the process, so initialize here
	invoke crt_printf, addr InputStatement
	invoke crt_scanf, addr MemoryAddressFormat, addr MemoryAddress
	mov EAX, MemoryAddress ; EAX stores the input, which is the requested memory address
	cmp EAX, -1
	je ExitProgram

	call SearchSets
	call SearchTags

	IfHit:
		call CacheHit
		jmp Result
	IfMiss:
		call CacheMiss
		jmp Result

	Result:
		jmp PrintCacheHits

SearchSets proc
	; Search which set is the input memory address being mapped to
	; *** fill your code here ***

	mov CL, BlockSize
	shr EAX, CL
	mov EBX, EAX
	and EBX, 15 ; 16 sets
	shl EBX, 3
	shr EAX, 4
	add EBP, EBX
	add ESI, EBX
	mov ECX, 0
	ret
SearchSets endp

SearchTags proc
	; Search all tags in the set to find out cache hit or miss
	; if cache hit, jmp to CacheHit
	; if cache miss, jmp to CacheMiss and do the FIFO replacement algorithm
	; *** fill your code here ***

	cmp EAX, [EBP + 4 * ECX]
	je IfHit
	inc ECX
	cmp ECX, 4
	jne SearchTags
	mov ECX, 0
	jmp IfMiss
SearchTags endp

CacheHit proc
	; Handle the cache hit
	; Do something if there is a cache hit
	; *** fill your code here ***

	inc CacheHits
	mov EAX, CurrentTimeStamp
	ret
CacheHit endp

CacheMiss proc ; Handle the cache miss by FIFO replacement algorithm
	; There are 2 things to do in this function:
	; 1. Use FIFO to find the cache block to be replaced
	; 2. Replace the old cache block with the new input block
	; *** fill your code here ***

	mov ECX, 0
	mov EBX, ECX
	mov EDX, [ESI + 4 * ECX]
my_repeat:
	inc ECX
	cmp ECX, 4
	je done
	cmp EDX, [ESI + 4 * ECX]
	jle my_repeat
	mov EDX, [ESI + 4 * ECX]
	mov EBX, ECX
	jmp my_repeat
done:
	mov [EBP + 4 * EBX], EAX
	mov EDX, CurrentTimeStamp
	mov [ESI + 4 * EBX], EDX
	ret
CacheMiss endp

PrintCacheHits:
	; By doing the following, you can save the values in the registers
	push EAX
	push EBX
	push ECX
	push EDX 
	mov EAX, CacheHits
	invoke crt_printf, addr OutputFormat4, EAX
	; By doing the following, you can restore the values in the registers
	pop EDX
	pop ECX
	pop EBX
	pop EAX
	jmp input

; This function will be be used during the testing
; But this function is preserved in case that you want to debug by using this
PrintCacheStatus: ; Print all the tags in the CacheTags
	invoke crt_printf, addr OutputFormat1
	mov EBX, 0
RepeatPrint:
	mov EAX, [EBP + 4*EBX] ; move the corresponding value in CacheTags into EBX 
	invoke crt_printf, addr OutputFormat2, EAX ; print out the value
	inc EBX ; increase to the next Tag
	cmp EBX, CacheSize ; find out whether all tags are printed
	jne RepeatPrint ; if not, jump back to print the next tag
	invoke crt_printf, addr OutputFormat3 
	jmp input ; if yes, jump back to get the next memory address

ExitProgram:
	invoke ExitProcess, NULL

end start