; Declaracions públiques

public _BufferPantalla
public _EscriureCar
public _EscriureString
public _EsborrarPantalla
public _QuinCarAtr
public _BufPant

_DATA   segment word public 'DATA'
_DATA   ends

_BSS    segment word public 'BSS'
 _BufPant dw   ? ; direcció de vídeo
_BSS    ends

DGROUP group _DATA, _BSS

assume cs:_PANTALLA_ASM, ds:DGROUP


_PANTALLA_ASM segment byte public 'CODE'

; +-----------------+
; | _BufferPantalla |
; +-----------------+

_BufferPantalla proc far
    push bx
    push es

    mov ax, 0000h            ; Segment d'inici
    mov es, ax              ; vinga
    mov bx, 449h             ; Desplaçament 449h
    mov bl, byte ptr es:[bx]   ; el fico a bl
    cmp bl, 2               ; con 2
    je _BufferPantalla_Color
    cmp bl, 3               ; o con 3
    jne _BufferPantalla_Caso_7
_BufferPantalla_Color:
    mov dx, 0B800h           ; Es de color
    jmp _BufferPantalla_Fin
_BufferPantalla_Caso_7:
    cmp bl, 7               ; Con 7
    jne _BufferPantalla_Error
    mov dx, 0B000h           ; Es monocroma
    jmp _BufferPantalla_Fin
_BufferPantalla_Error:
    mov dx, 0000h            ; Se retorna un puntero a NULL (0)
_BufferPantalla_Fin:

    xor ax, ax              ; Ax a cero. Será el desplaçament
    mov _BufPant, dx

    pop es
    pop bx
    ret                   ; retorna.
_BufferPantalla endp



; +--------------+
; | _EscriureCar |
; +--------------+

_TParam_EscriureCar struc
              dw ?   ; Bp
              dd ?   ; Adreça de retorn
   _EscriureCar_fila dw ?   ; Fila    (Byte)
 _EscriureCar_columna dw ?   ; Columna  (Byte)
 _EscriureCar_caracter dw ?   ; Caracter (Byte)
  _EscriureCar_atribut dw ?   ; Atribut (Byte)
_TParam_EscriureCar ends

_EscriureCar proc far
    push bp
    mov bp, sp
    push ax
    push bx
    push di
    push es

    mov ax, _BufPant
    mov es, ax

    mov al, byte ptr [bp+_EscriureCar_fila]  ; al <- fila
    mov bl, 80
    mul bl                         ; ax <- fila * 80
    mov bl, byte ptr [bp+_EscriureCar_columna]
    xor bh, bh
    add ax, bx            ; ax <- fila * 80 + columna
    sal ax, 1
    mov di, ax
    mov al, byte ptr [bp+_EscriureCar_caracter]
    mov ah, byte ptr [bp+_EscriureCar_atribut]
    stosw

    pop es
    pop di
    pop bx
    pop ax
    pop bp
    ret
_EscriureCar endp



; +-----------------+
; | _EscriureString |
; +-----------------+

_TParam_EscriureString struc
                dw   ?  ; Bp
                dd   ?  ; Adreça de retorn
   _EscriureString_fila dw   ?  ; Fila  (Byte)
 _EscriureString_columna dw   ?  ; Columana (Byte)
   _EscriureString_Desp dw   ?  ; Desplaçament de l'string
   _EscriureString_Segm dw   ?  ; Segment de l'string
 _EscriureString_atribut dw   ?  ; Atribut (Byte)
_TParam_EscriureString ends

_EscriureString proc far
    push bp
    mov bp, sp
    push ax
    push bx
    push di
    push ds
    push es
    push si

    mov ax, _BufPant
    mov es, ax

    mov ax, [bp+_EscriureString_Segm]
    mov ds, ax
    mov si, [bp+_EscriureString_Desp]  ; [ds:si] origen

    mov al, byte ptr [bp+_EscriureString_fila]  ; al <- fila
    mov bl, 80
    mul bl                          ; ax <- fila * 80
    mov bl, byte ptr [bp+_EscriureString_columna]
    xor bh, bh
    add ax, bx            ; ax <- fila * 80 + columna
    sal ax, 1
    mov di, ax            ; [es:di] destí
    mov ah, byte ptr [bp+_EscriureString_atribut] ; [ah] atribut

_EscriureString_Loop:
    cld                ; Per escriure cap endavant (cmp pot variar-lo)
    lodsb               ; Llegir següent caracter
    cmp al, 0            ; Comparar amb la fi
    je _EscriureString_Fin  ; Si es veritat, adeu
    cld                ; Per escriure cap endavant
    stosw               ; Sino, escriure amb atribut
    jmp _EscriureString_Loop ; Altre vegada
_EscriureString_Fin:

    pop si
    pop es
    pop ds
    pop di
    pop bx
    pop ax
    pop bp
    ret
_EscriureString endp



; +-------------------+
; | _EsborrarPantalla |
; +-------------------+

_TParam_EsborrarPantalla struc
                  dw  ?  ; Bp
                  dd  ?  ; Retorna adreça
 _EsborrarPantalla_atribut dw  ?  ; Atribut (Byte)
_TParam_EsborrarPantalla ends

_EsborrarPantalla proc far
    push bp
    mov bp, sp
    push ax
    push cx
    push di
    push es
    mov ax, _BufPant
    mov es, ax

    mov ah, byte ptr [bp+_EsborrarPantalla_atribut]  ; atribut
    mov al, 20h                      ; Spai
    mov cx, 2000                      ; 80 x 25 words
    xor di, di                   ; Al començament
    cld                         ; Cap andavant
    rep stosw                    ; Toma!!!

    pop es
    pop di
    pop cx
    pop ax
    pop bp
    ret
_EsborrarPantalla endp



; +-------------+
; | _QuinCarAtr |
; +-------------+

_TParam_QuinCarAtr struc
              dw  ?  ; Bp
              dd  ?  ; Adreça de retorn
   _QuinCarAtr_fila dw  ?  ; fila (Byte)
 _QuinCarAtr_columna dw  ?  ; columna (byte)
_TParam_QuinCarAtr ends

_QuinCarAtr proc far
    push bp
    mov bp, sp
    push bx
    push di
    push es

    mov ax, _BufPant
    mov es, ax

    mov al, byte ptr [bp+_QuinCarAtr_fila]  ; al <- fila
    mov bl, 80
    mul bl                          ; ax <- fila * 80
    mov bl, byte ptr [bp+_QuinCarAtr_columna]
    xor bh, bh              ; bx <- Columna
    add ax, bx              ; ax <- fila * 80 + columna
    sal ax, 1              ; ax <- ax * 2
    mov di, ax
    mov ax, es:[di]

    pop es
    pop di
    pop bx
    pop bp
    ret
_QuinCarAtr endp

_PANTALLA_ASM ends

    end    ; final de fitxer
