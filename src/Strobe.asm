; STROBE.ASM: rutina per fer un strobe genèric
;
; ATENCIÓ: aquest fitxer NO surt a la memòria de la pràctica. El mòdul
; original el va donar el professor (EC 96/97, Lab. E/S 3) i no s'ha
; conservat. Això és una RECONSTRUCCIÓ feta el 2026 a partir de la
; interfície descrita a STROBE.H i de l'ús que en fa Int_9.c:
;       Strobe (0x61, 7);
; Segueix les convencions dels altres mòduls: model LARGE, crida far,
; paràmetres a la pila a l'estil C.

; Declaracions públiques

public _Strobe

_DATA segment word public 'DATA'
_DATA ends

_BSS segment word public 'BSS'
_BSS ends

DGROUP group _DATA, _BSS

assume cs: STROBE_ASM, ds: DGROUP

STROBE_ASM segment byte public 'CODE'

; +--------+
; | Strobe |
; +--------+
; void Strobe (WORD AdrPort, BYTE Bit0_7);
; Posa successivament a 1 i a 0 el bit Bit0_7 del port AdrPort.
; La resta de bits del port es deixen com estaven.

_Strobe_Param Struc
        Dw ?            ; Bp
        Dd ?            ; Adreça de retorn
 _Strobe_AdrPort Dw ?   ; Adreça del port d'E/S (Word)
 _Strobe_Bit0_7  Dw ?   ; Número de bit, de 0 a 7 (Byte)
_Strobe_Param EndS

_Strobe proc far
    push bp             ; BP es modificarà
    mov bp, sp          ; enllaç dinàmic per accedir als paràmetres
    push ax
    push cx
    push dx

    mov dx, [bp+_Strobe_AdrPort]         ; dx <- port
    mov cl, byte ptr [bp+_Strobe_Bit0_7] ; cl <- número de bit
    and cl, 7           ; només de 0 a 7
    mov ah, 1
    shl ah, cl          ; ah <- màscara amb un 1 al bit demanat

    pushf               ; guardar flags
    cli                 ; que no ens interrompin a mig strobe
    in al, dx           ; llegir el valor actual del port
    or al, ah
    out dx, al          ; bit a 1
    jmp short $+2       ; petit retard d'E/S
    not ah
    and al, ah
    out dx, al          ; bit a 0
    popf                ; recuperar flags

    pop dx
    pop cx
    pop ax
    pop bp
    ret                 ; retorna. Els paràmetres els treu qui crida
_Strobe endp

STROBE_ASM ends

    end    ; final de fitxer
