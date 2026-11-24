; TECLAT.ASM: rutines de control del teclat per ENQUESTA
;
; ATENCIÓ: aquest fitxer NO surt a la memòria de la pràctica. El mòdul
; original el va donar el professor (EC 96/97, Lab. E/S 3) i no s'ha
; conservat. Això és una RECONSTRUCCIÓ feta el 2026 a partir de la
; interfície descrita a TECLAT.H i de l'ús que en fa Tecles.c
; (demanar_tecles). Coses que s'han hagut de suposar:
;   - Per saber si hi ha una tecla es mira el bit 0 del port 64h
;     (controlador de teclat d'un AT o posterior). En un XT original
;     caldria consultar l'IRR del 8259.
;   - La taula de traducció a ASCII és la del teclat americà, amb les
;     lletres en majúscules i el teclat numèric traduït als seus dígits.
; Segueix les convencions dels altres mòduls: model LARGE, crida far,
; paràmetres a la pila a l'estil C, resultat a AL/AX.

; Declaracions públiques

public _LlegirTeclatENQ
public _ASCII
public _IniTeclatENQ
public _FiTeclatENQ

_DATA segment word public 'DATA'
_DATA ends

_BSS segment word public 'BSS'
_BSS ends

DGROUP group _DATA, _BSS

assume cs: TECLAT_ASM, ds: DGROUP

TECLAT_ASM segment byte public 'CODE'

; Taula de traducció de codi de rastreig (scan code, joc 1) a ASCII.
; Un 0 vol dir que la tecla no té traducció. Es guarda al segment de
; codi i es llegeix amb cs: per no dependre del valor de ds.

_Teclat_Taula label byte
    db 0, 27                                ; 00h: res, ESC
    db '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'  ; 02h..0Bh
    db '-', '=', 8, 9                       ; 0Ch..0Fh: - = retrocés tabulador
    db 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'  ; 10h..19h
    db '[', ']', 13, 0                      ; 1Ah..1Dh: [ ] retorn Ctrl
    db 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'       ; 1Eh..26h
    db ';', 39, '`', 0, 92                  ; 27h..2Bh: ; ' ` Maj-esq \
    db 'Z', 'X', 'C', 'V', 'B', 'N', 'M'    ; 2Ch..32h
    db ',', '.', '/', 0, '*'                ; 33h..37h: , . / Maj-dre *
    db 0, ' ', 0                            ; 38h..3Ah: Alt, espai, BloqMaj
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0         ; 3Bh..44h: F1..F10
    db 0, 0                                 ; 45h..46h: BloqNum, BloqDespl
    db '7', '8', '9', '-'                   ; 47h..4Ah: teclat numèric
    db '4', '5', '6', '+'                   ; 4Bh..4Eh
    db '1', '2', '3', '0', '.'              ; 4Fh..53h
_Teclat_Taula_Long equ $ - _Teclat_Taula    ; 84 entrades (00h..53h)

; +-----------------+
; | LlegirTeclatENQ |
; +-----------------+
; BYTE LlegirTeclatENQ (void);
; Espera (enquesta) fins que es premi una tecla i en retorna el codi de
; rastreig. Només fa cas dels codis "make"; els "break" (bit 7 a 1) es
; llegeixen i es descarten.

_LlegirTeclatENQ proc far
_LlegirTeclatENQ_Espera:
    in al, 64h          ; estat del controlador de teclat
    test al, 01h        ; hi ha una dada per llegir?
    jz _LlegirTeclatENQ_Espera

    in al, 60h          ; llegir el codi de rastreig
    mov ah, al          ; el guardo mentre faig el strobe

    in al, 61h          ; reconeixement al teclat: strobe del bit 7
    or al, 80h          ; del port 61h (igual que fa RSI_9)
    out 61h, al
    jmp short $+2       ; petit retard d'E/S
    and al, 7Fh
    out 61h, al

    mov al, ah          ; recupero el codi
    test al, 80h        ; és un "break" (tecla que puja)?
    jnz _LlegirTeclatENQ_Espera ; doncs no en faig cas

    xor ah, ah          ; resultat a AL, AH a zero
    ret
_LlegirTeclatENQ endp


; +-------+
; | ASCII |
; +-------+
; char ASCII (BYTE ScanCode);
; Tradueix a ASCII el codi de rastreig. Si no té traducció retorna 0.

_ASCII_Param Struc
        Dw ?            ; Bp
        Dd ?            ; Adreça de retorn
 _ASCII_ScanCode Dw ?   ; Codi de rastreig (Byte)
_ASCII_Param EndS

_ASCII proc far
    push bp             ; BP es modificarà
    mov bp, sp          ; enllaç dinàmic per accedir als paràmetres
    push bx

    xor ax, ax          ; per defecte, sense traducció
    mov bl, byte ptr [bp+_ASCII_ScanCode]
    cmp bl, _Teclat_Taula_Long ; fora de la taula?
    jae _ASCII_Fi
    xor bh, bh
    mov al, cs:_Teclat_Taula[bx] ; al <- taula[codi]
_ASCII_Fi:

    pop bx
    pop bp
    ret                 ; retorna. Els paràmetres els treu qui crida
_ASCII endp


; +--------------+
; | IniTeclatENQ |
; +--------------+
; void IniTeclatENQ (void);
; Inhibeix les peticions d'interrupció del teclat (IRQ 1) al registre
; de màscares del controlador d'interrupcions, per treballar per enquesta.

_IniTeclatENQ proc far
    push ax

    pushf               ; guardar flags
    cli
    in al, 21h          ; registre de màscares d'interrupcions (IMR)
    or al, 02h          ; inhibir la IRQ 1 (teclat)
    out 21h, al
    popf                ; recuperar flags

    pop ax
    ret
_IniTeclatENQ endp


; +-------------+
; | FiTeclatENQ |
; +-------------+
; void FiTeclatENQ (void);
; Torna a permetre les peticions d'interrupció del teclat (IRQ 1).

_FiTeclatENQ proc far
    push ax

    pushf               ; guardar flags
    cli
    in al, 21h          ; registre de màscares d'interrupcions (IMR)
    and al, 0FDh        ; desinhibir la IRQ 1 (teclat)
    out 21h, al
    popf                ; recuperar flags

    pop ax
    ret
_FiTeclatENQ endp

TECLAT_ASM ends

    end    ; final de fitxer
