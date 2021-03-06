
/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3, by John R. Hauser.

Copyright 2011, 2012, 2013, 2014 The Regents of the University of California
(Regents).  All Rights Reserved.  Redistribution and use in source and binary
forms, with or without modification, are permitted provided that the following
conditions are met:

Redistributions of source code must retain the above copyright notice,
this list of conditions, and the following two paragraphs of disclaimer.
Redistributions in binary form must reproduce the above copyright notice,
this list of conditions, and the following two paragraphs of disclaimer in the
documentation and/or other materials provided with the distribution.  Neither
the name of the Regents nor the names of its contributors may be used to
endorse or promote products derived from this software without specific prior
written permission.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE.  THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS".  REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

=============================================================================*/

#include <stdbool.h>
#include <stdint.h>
#include "platform.h"
#include "internals.h"
#include "specialize.h"
#include "softfloat.h"

float128_t extF80_to_f128( extFloat80_t a )
{
    union { struct extFloat80M s; extFloat80_t f; } uA;
    uint_fast16_t uiA64;
    uint_fast64_t uiA0;
    uint_fast16_t exp;
    uint_fast64_t sig;
    struct commonNaN commonNaN;
    struct uint128 uiZ;
    bool sign;
    struct uint128 sig128;
    union ui128_f128 uZ;

    uA.f = a;
    uiA64 = uA.s.signExp;
    uiA0  = uA.s.signif;
    exp = expExtF80UI64( uiA64 );
    sig = uiA0 & UINT64_C( 0x7FFFFFFFFFFFFFFF );
    if ( (exp == 0x7FFF) && sig ) {
        softfloat_extF80UIToCommonNaN( uiA64, uiA0, &commonNaN );
        uiZ = softfloat_commonNaNToF128UI( &commonNaN );
    } else {
        sign = signExtF80UI64( uiA64 );
        sig128 = softfloat_shortShiftLeft128( 0, sig, 49 );
        uiZ.v64 = packToF128UI64( sign, exp, sig128.v64 );
        uiZ.v0  = sig128.v0;
    }
    uZ.ui = uiZ;
    return uZ.f;

}

