;/**************************************************************************
; *
; *	XVID MPEG-4 VIDEO CODEC
; *	mmx/xmm/3dnow 8x8 block-based halfpel interpolation
; *
; *	This program is an implementation of a part of one or more MPEG-4
; *	Video tools as specified in ISO/IEC 14496-2 standard.  Those intending
; *	to use this software module in hardware or software products are
; *	advised that its use may infringe existing patents or copyrights, and
; *	any such use would be at such party's own risk.  The original
; *	developer of this software module and his/her company, and subsequent
; *	editors and their companies, will have no liability for use of this
; *	software or modifications or derivatives thereof.
; *
; *	This program is free software; you can redistribute it and/or modify
; *	it under the terms of the GNU General Public License as published by
; *	the Free Software Foundation; either version 2 of the License, or
; *	(at your option) any later version.
; *
; *	This program is distributed in the hope that it will be useful,
; *	but WITHOUT ANY WARRANTY; without even the implied warranty of
; *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; *	GNU General Public License for more details.
; *
; *	You should have received a copy of the GNU General Public License
; *	along with this program; if not, write to the Free Software
; *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
; *
; *************************************************************************/

;/**************************************************************************
; *
; *	History:
; *
; * 04.02.2002  added xmm and additional 3dnow optimizations (Isibaar)
; * 27.12.2001	mofified "compensate_halfpel"
; *	22.12.2001	inital version; (c)2001 peter ross <pross@cs.rmit.edu.au>
; *
; *************************************************************************/


bits 32

%macro cglobal 1 
	%ifdef PREFIX
		global _%1 
		%define %1 _%1
	%else
		global %1
	%endif
%endmacro

section .data


align 16


;===========================================================================
; (1 - r) rounding table
;===========================================================================

rounding1_mmx
times 4 dw 1
times 4 dw 0

;===========================================================================
; (2 - r) rounding table  
;===========================================================================

rounding2_mmx
times 4 dw 2
times 4 dw 1

mmx_one
times 8 db 1

section .text

%macro  CALC_AVG 6
	punpcklbw %3, %6
	punpckhbw %4, %6

	paddusw %1, %3		; mm01 += mm23
	paddusw %2, %4
	paddusw %1, %5		; mm01 += rounding
	paddusw %2, %5
		
	psrlw %1, 1			; mm01 >>= 1
	psrlw %2, 1

%endmacro



;===========================================================================
;
; void interpolate8x8_halfpel_h_xmm(uint8_t * const dst,
;						const uint8_t * const src,
;						const uint32_t stride,
;						const uint32_t rounding);
;
;===========================================================================

align 16
cglobal interpolate8x8_halfpel_h_xmm
interpolate8x8_halfpel_h_xmm
		push	esi
		push	edi

		mov	eax, [esp + 8 + 16]		; rounding
		
		or al,al
		jnz near interpolate8x8_halfpel_h_mmx.start
		
		; pavgb only works when rounding = 0

		mov	edi, [esp + 8 + 4]		; dst
		mov	esi, [esp + 8 + 8]		; src
		mov	edx, [esp + 8 + 12]		; stride

		mov eax, 4
		mov ecx, edx
		shl edx, 1

.loop
		movq mm0, [esi]
		movq mm1, [esi + ecx]
		pavgb mm0, [esi + 1]			; mm0 = avg([src], [src+1])
		pavgb mm1, [esi + ecx + 1]	; mm0 = avg([src+stride], [src+stride+1])

		movq [edi], mm0
		movq [edi + ecx], mm1

		add esi, edx	; src += 2*stride
		add edi, edx	; dst += 2*stride
		
		dec eax
		jnz	.loop

		pop edi
		pop esi

		ret


;===========================================================================
;
; void interpolate8x8_halfpel_h_3dn(uint8_t * const dst,
;						const uint8_t * const src,
;						const uint32_t stride,
;						const uint32_t rounding);
;
;===========================================================================

align 16
cglobal interpolate8x8_halfpel_h_3dn
interpolate8x8_halfpel_h_3dn
		push	esi
		push	edi

		mov	eax, [esp + 8 + 16]		; rounding
		
		or al,al
		jnz near interpolate8x8_halfpel_h_mmx.start
		
		; pavgusb only works when rounding = 0

		mov	edi, [esp + 8 + 4]		; dst
		mov	esi, [esp + 8 + 8]		; src
		mov	edx, [esp + 8 + 12]		; stride

		mov ecx, edx
		mov eax, 4
		shl edx, 1

.loop
		movq mm0, [esi]
		movq mm1, [esi + ecx]
		pavgusb mm0, [esi + 1]			; mm0 = avg([src], [src+1])
		pavgusb mm1, [esi + ecx + 1]	; mm0 = avg([src+stride], [src+stride+1])

		movq [edi], mm0
		movq [edi + ecx], mm1

		add esi, edx	; src += 2*stride
		add edi, edx	; dst += 2*stride
		
		dec eax
		jnz	.loop

		pop edi
		pop esi

		ret


;===========================================================================
;
; void interpolate8x8_halfpel_h_mmx(uint8_t * const dst,
;						const uint8_t * const src,
;						const uint32_t stride,
;						const uint32_t rounding);
;
;===========================================================================

align 16
cglobal interpolate8x8_halfpel_h_mmx
interpolate8x8_halfpel_h_mmx

		push	esi
		push	edi

		mov	eax, [esp + 8 + 16]		; rounding

interpolate8x8_halfpel_h_mmx.start
		movq mm7, [rounding1_mmx + eax * 8]

		mov	edi, [esp + 8 + 4]		; dst
		mov	esi, [esp + 8 + 8]		; src
		mov	edx, [esp + 8 + 12]	; stride

		mov eax, 8
		pxor	mm6, mm6		; zero

.loop
		movq mm0, [esi]
		movq mm2, [esi + 1]
		movq mm1, mm0
		movq mm3, mm2

		punpcklbw mm0, mm6	; mm01 = [src]
		punpckhbw mm1, mm6	; mm23 = [src + 1]

		CALC_AVG mm0, mm1, mm2, mm3, mm7, mm6

		packuswb mm0, mm1
		movq [edi], mm0			; [dst] = mm01

		add esi, edx		; src += stride
		add edi, edx		; dst += stride

		dec eax
		jnz .loop

		pop edi
		pop esi

		ret


;===========================================================================
;
; void interpolate8x8_halfpel_v_xmm(uint8_t * const dst,
;						const uint8_t * const src,
;						const uint32_t stride,
;						const uint32_t rounding);
;
;===========================================================================

align 16
cglobal interpolate8x8_halfpel_v_xmm
interpolate8x8_halfpel_v_xmm
		push	esi
		push	edi

		mov	eax, [esp + 8 + 16]		; rounding
		
		or al,al
		jnz near interpolate8x8_halfpel_v_mmx.start
		
		; pavgb only works when rounding = 0

		mov	edi, [esp + 8 + 4]		; dst
		mov	esi, [esp + 8 + 8]		; src
		mov	edx, [esp + 8 + 12]		; stride
		
		mov eax, 8

.loop
		movq mm0, [esi]
		pavgb mm0, [esi + edx]		; mm0 = avg([src], [src+stride])
		movq [edi], mm0

		add edi, edx	; dst += stride
		add esi, edx	; src += stride
		
		dec eax
		jnz	.loop

		pop edi
		pop esi

		ret


;===========================================================================
;
; void interpolate8x8_halfpel_v_3dn(uint8_t * const dst,
;						const uint8_t * const src,
;						const uint32_t stride,
;						const uint32_t rounding);
;
;===========================================================================

align 16
cglobal interpolate8x8_halfpel_v_3dn
interpolate8x8_halfpel_v_3dn
		push	esi
		push	edi

		mov	eax, [esp + 8 + 16]		; rounding
		
		or al,al
		jnz interpolate8x8_halfpel_v_mmx.start
		
		; pavgusb only works when rounding = 0

		mov	edi, [esp + 8 + 4]		; dst
		mov	esi, [esp + 8 + 8]		; src
		mov	edx, [esp + 8 + 12]	; stride
		
		mov eax, 8

.loop
		movq mm0, [esi]
		pavgusb mm0, [esi + edx]			; mm0 = avg([src], [src+stride])
		movq [edi], mm0

		add edi, edx	; dst += stride
		add esi, edx	; src += stride
		
		dec eax
		jnz	.loop

		pop edi
		pop esi

		ret


;===========================================================================
;
; void interpolate8x8_halfpel_v_mmx(uint8_t * const dst,
;						const uint8_t * const src,
;						const uint32_t stride,
;						const uint32_t rounding);
;
;===========================================================================

align 16
cglobal interpolate8x8_halfpel_v_mmx
interpolate8x8_halfpel_v_mmx

		push	esi
		push	edi

		mov	eax, [esp + 8 + 16]		; rounding

interpolate8x8_halfpel_v_mmx.start
		movq mm7, [rounding1_mmx + eax * 8]

		mov	edi, [esp + 8 + 4]		; dst
		mov	esi, [esp + 8 + 8]		; src
		mov	edx, [esp + 8 + 12]	; stride

		mov eax, 8

		pxor	mm6, mm6		; zero

		
.loop
		movq mm0, [esi]
		movq mm2, [esi + edx]
		movq mm1, mm0
		movq mm3, mm2

		punpcklbw mm0, mm6	; mm01 = [src]
		punpckhbw mm1, mm6	; mm23 = [src + 1]

		CALC_AVG mm0, mm1, mm2, mm3, mm7, mm6

		packuswb mm0, mm1
		movq [edi], mm0			; [dst] = mm01

		add esi, edx		; src += stride
		add edi, edx		; dst += stride

		dec eax
		jnz .loop

		pop edi
		pop esi

		ret


;===========================================================================
;
; void interpolate8x8_halfpel_hv_mmx(uint8_t * const dst,
;						const uint8_t * const src,
;						const uint32_t stride, 
;						const uint32_t rounding);
;
;
;===========================================================================

align 16
cglobal interpolate8x8_halfpel_hv_mmx
interpolate8x8_halfpel_hv_mmx

		push	esi
		push	edi

		mov	eax, [esp + 8 + 16]		; rounding
interpolate8x8_halfpel_hv_mmx.start

		movq mm7, [rounding2_mmx + eax * 8]

		mov	edi, [esp + 8 + 4]		; dst
		mov	esi, [esp + 8 + 8]		; src

		mov eax, 8

		pxor	mm6, mm6		; zero
		
		mov edx, [esp + 8 + 12]	; stride		
		
.loop
		; current row

		movq mm0, [esi]
		movq mm2, [esi + 1]

		movq mm1, mm0
		movq mm3, mm2

		punpcklbw mm0, mm6		; mm01 = [src]
		punpcklbw mm2, mm6		; mm23 = [src + 1]
		punpckhbw mm1, mm6
		punpckhbw mm3, mm6

		paddusw mm0, mm2		; mm01 += mm23
		paddusw mm1, mm3

		; next row

		movq mm4, [esi + edx]
		movq mm2, [esi + edx + 1]
		
		movq mm5, mm4
		movq mm3, mm2
		
		punpcklbw mm4, mm6		; mm45 = [src + stride]
		punpcklbw mm2, mm6		; mm23 = [src + stride + 1]
		punpckhbw mm5, mm6
		punpckhbw mm3, mm6

		paddusw mm4, mm2		; mm45 += mm23
		paddusw mm5, mm3

		; add current + next row

		paddusw mm0, mm4		; mm01 += mm45
		paddusw mm1, mm5
		paddusw mm0, mm7		; mm01 += rounding2
		paddusw mm1, mm7
		
		psrlw mm0, 2			; mm01 >>= 2
		psrlw mm1, 2

		packuswb mm0, mm1
		movq [edi], mm0			; [dst] = mm01

		add esi, edx		; src += stride
		add edi, edx		; dst += stride

		dec eax
		jnz .loop

		pop edi
		pop esi

		ret

