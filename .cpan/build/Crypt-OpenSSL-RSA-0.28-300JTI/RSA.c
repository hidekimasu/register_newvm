/*
 * This file was generated automatically by ExtUtils::ParseXS version 3.18 from the
 * contents of RSA.xs. Do not edit this file, edit RSA.xs instead.
 *
 *    ANY CHANGES MADE HERE WILL BE LOST!
 *
 */

#line 1 "RSA.xs"
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include <openssl/objects.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/ripemd.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/ssl.h>

typedef struct
{
    RSA* rsa;
    int padding;
    int hashMode;
} rsaData;

/* Key names for the rsa hash structure */

#define KEY_KEY "_Key"
#define PADDING_KEY "_Padding"
#define HASH_KEY "_Hash_Mode"

#define PACKAGE_NAME "Crypt::OpenSSL::RSA"

void croakSsl(char* p_file, int p_line)
{
    const char* errorReason;
    /* Just return the top error on the stack */
    errorReason = ERR_reason_error_string(ERR_get_error());
    ERR_clear_error();
    croak("%s:%d: OpenSSL error: %s", p_file, p_line, errorReason);
}

#define CHECK_OPEN_SSL(p_result) if (!(p_result)) croakSsl(__FILE__, __LINE__);

#define PACKAGE_CROAK(p_message) croak("%s", (p_message))
#define CHECK_NEW(p_var, p_size, p_type) \
  if (New(0, p_var, p_size, p_type) == NULL) \
    { PACKAGE_CROAK("unable to alloc buffer"); }

#define THROW(p_result) if (!(p_result)) { error = 1; goto err; }

char _is_private(rsaData* p_rsa)
{
    return(p_rsa->rsa->d != NULL);
}

SV* make_rsa_obj(SV* p_proto, RSA* p_rsa)
{
    rsaData* rsa;

    CHECK_NEW(rsa, 1, rsaData);
    rsa->rsa = p_rsa;
    rsa->hashMode = NID_sha1;
    rsa->padding = RSA_PKCS1_OAEP_PADDING;
    return sv_bless(
        newRV_noinc(newSViv((IV) rsa)),
        (SvROK(p_proto) ? SvSTASH(SvRV(p_proto)) : gv_stashsv(p_proto, 1)));
}

int get_digest_length(int hash_method)
{
    switch(hash_method)
    {
        case NID_md5:
            return MD5_DIGEST_LENGTH;
            break;
        case NID_sha1:
            return SHA_DIGEST_LENGTH;
            break;
#ifdef SHA512_DIGEST_LENGTH
        case NID_sha224:
            return SHA224_DIGEST_LENGTH;
            break;
        case NID_sha256:
            return SHA256_DIGEST_LENGTH;
            break;
        case NID_sha384:
            return SHA384_DIGEST_LENGTH;
            break;
        case NID_sha512:
            return SHA512_DIGEST_LENGTH;
            break;
#endif
        case NID_ripemd160:
            return RIPEMD160_DIGEST_LENGTH;
            break;
        default:
            croak("Unknown digest hash code");
            break;
    }
}

unsigned char* get_message_digest(SV* text_SV, int hash_method)
{
    STRLEN text_length;
    unsigned char* text;

    text = (unsigned char*) SvPV(text_SV, text_length);

    switch(hash_method)
    {
        case NID_md5:
            return MD5(text, text_length, NULL);
            break;
        case NID_sha1:
            return SHA1(text, text_length, NULL);
            break;
#ifdef SHA512_DIGEST_LENGTH
        case NID_sha224:
            return SHA224(text, text_length, NULL);
            break;
        case NID_sha256:
            return SHA256(text, text_length, NULL);
            break;
        case NID_sha384:
            return SHA384(text, text_length, NULL);
            break;
        case NID_sha512:
            return SHA512(text, text_length, NULL);
            break;
#endif
        case NID_ripemd160:
            return RIPEMD160(text, text_length, NULL);
            break;
        default:
            croak("Unknown digest hash code");
            break;
    }
}

SV* bn2sv(BIGNUM* p_bn)
{
    return p_bn != NULL
        ? sv_2mortal(newSViv((IV) BN_dup(p_bn)))
        : &PL_sv_undef;
}

SV* extractBioString(BIO* p_stringBio)
{
    SV* sv;
    BUF_MEM* bptr;

    CHECK_OPEN_SSL(BIO_flush(p_stringBio) == 1);
    BIO_get_mem_ptr(p_stringBio, &bptr);
    sv = newSVpv(bptr->data, bptr->length);

    CHECK_OPEN_SSL(BIO_set_close(p_stringBio, BIO_CLOSE) == 1);
    BIO_free(p_stringBio);
    return sv;
}

RSA* _load_rsa_key(SV* p_keyStringSv,
                   RSA*(*p_loader)(BIO*, RSA**, pem_password_cb*, void*))
{
    STRLEN keyStringLength;
    char* keyString;

    RSA* rsa;
    BIO* stringBIO;

    keyString = SvPV(p_keyStringSv, keyStringLength);

    CHECK_OPEN_SSL(stringBIO = BIO_new_mem_buf(keyString, keyStringLength));

    rsa = p_loader(stringBIO, NULL, NULL, NULL);

    CHECK_OPEN_SSL(BIO_set_close(stringBIO, BIO_CLOSE) == 1);
    BIO_free(stringBIO);

    CHECK_OPEN_SSL(rsa);
    return rsa;
}

SV* rsa_crypt(rsaData* p_rsa, SV* p_from,
              int (*p_crypt)(int, const unsigned char*, unsigned char*, RSA*, int))
{
    STRLEN from_length;
    int to_length;
    int size;
    unsigned char* from;
    char* to;
    SV* sv;

    from = (unsigned char*) SvPV(p_from, from_length);
    size = RSA_size(p_rsa->rsa);
    CHECK_NEW(to, size, char);

    to_length = p_crypt(
       from_length, from, (unsigned char*) to, p_rsa->rsa, p_rsa->padding);

    if (to_length < 0)
    {
        Safefree(to);
        CHECK_OPEN_SSL(0);
    }
    sv = newSVpv(to, to_length);
    Safefree(to);
    return sv;
}


#line 220 "RSA.c"
#ifndef PERL_UNUSED_VAR
#  define PERL_UNUSED_VAR(var) if (0) var = var
#endif

#ifndef dVAR
#  define dVAR		dNOOP
#endif


/* This stuff is not part of the API! You have been warned. */
#ifndef PERL_VERSION_DECIMAL
#  define PERL_VERSION_DECIMAL(r,v,s) (r*1000000 + v*1000 + s)
#endif
#ifndef PERL_DECIMAL_VERSION
#  define PERL_DECIMAL_VERSION \
	  PERL_VERSION_DECIMAL(PERL_REVISION,PERL_VERSION,PERL_SUBVERSION)
#endif
#ifndef PERL_VERSION_GE
#  define PERL_VERSION_GE(r,v,s) \
	  (PERL_DECIMAL_VERSION >= PERL_VERSION_DECIMAL(r,v,s))
#endif
#ifndef PERL_VERSION_LE
#  define PERL_VERSION_LE(r,v,s) \
	  (PERL_DECIMAL_VERSION <= PERL_VERSION_DECIMAL(r,v,s))
#endif

/* XS_INTERNAL is the explicit static-linkage variant of the default
 * XS macro.
 *
 * XS_EXTERNAL is the same as XS_INTERNAL except it does not include
 * "STATIC", ie. it exports XSUB symbols. You probably don't want that
 * for anything but the BOOT XSUB.
 *
 * See XSUB.h in core!
 */


/* TODO: This might be compatible further back than 5.10.0. */
#if PERL_VERSION_GE(5, 10, 0) && PERL_VERSION_LE(5, 15, 1)
#  undef XS_EXTERNAL
#  undef XS_INTERNAL
#  if defined(__CYGWIN__) && defined(USE_DYNAMIC_LOADING)
#    define XS_EXTERNAL(name) __declspec(dllexport) XSPROTO(name)
#    define XS_INTERNAL(name) STATIC XSPROTO(name)
#  endif
#  if defined(__SYMBIAN32__)
#    define XS_EXTERNAL(name) EXPORT_C XSPROTO(name)
#    define XS_INTERNAL(name) EXPORT_C STATIC XSPROTO(name)
#  endif
#  ifndef XS_EXTERNAL
#    if defined(HASATTRIBUTE_UNUSED) && !defined(__cplusplus)
#      define XS_EXTERNAL(name) void name(pTHX_ CV* cv __attribute__unused__)
#      define XS_INTERNAL(name) STATIC void name(pTHX_ CV* cv __attribute__unused__)
#    else
#      ifdef __cplusplus
#        define XS_EXTERNAL(name) extern "C" XSPROTO(name)
#        define XS_INTERNAL(name) static XSPROTO(name)
#      else
#        define XS_EXTERNAL(name) XSPROTO(name)
#        define XS_INTERNAL(name) STATIC XSPROTO(name)
#      endif
#    endif
#  endif
#endif

/* perl >= 5.10.0 && perl <= 5.15.1 */


/* The XS_EXTERNAL macro is used for functions that must not be static
 * like the boot XSUB of a module. If perl didn't have an XS_EXTERNAL
 * macro defined, the best we can do is assume XS is the same.
 * Dito for XS_INTERNAL.
 */
#ifndef XS_EXTERNAL
#  define XS_EXTERNAL(name) XS(name)
#endif
#ifndef XS_INTERNAL
#  define XS_INTERNAL(name) XS(name)
#endif

/* Now, finally, after all this mess, we want an ExtUtils::ParseXS
 * internal macro that we're free to redefine for varying linkage due
 * to the EXPORT_XSUB_SYMBOLS XS keyword. This is internal, use
 * XS_EXTERNAL(name) or XS_INTERNAL(name) in your code if you need to!
 */

#undef XS_EUPXS
#if defined(PERL_EUPXS_ALWAYS_EXPORT)
#  define XS_EUPXS(name) XS_EXTERNAL(name)
#else
   /* default to internal */
#  define XS_EUPXS(name) XS_INTERNAL(name)
#endif

#ifndef PERL_ARGS_ASSERT_CROAK_XS_USAGE
#define PERL_ARGS_ASSERT_CROAK_XS_USAGE assert(cv); assert(params)

/* prototype to pass -Wmissing-prototypes */
STATIC void
S_croak_xs_usage(pTHX_ const CV *const cv, const char *const params);

STATIC void
S_croak_xs_usage(pTHX_ const CV *const cv, const char *const params)
{
    const GV *const gv = CvGV(cv);

    PERL_ARGS_ASSERT_CROAK_XS_USAGE;

    if (gv) {
        const char *const gvname = GvNAME(gv);
        const HV *const stash = GvSTASH(gv);
        const char *const hvname = stash ? HvNAME(stash) : NULL;

        if (hvname)
            Perl_croak(aTHX_ "Usage: %s::%s(%s)", hvname, gvname, params);
        else
            Perl_croak(aTHX_ "Usage: %s(%s)", gvname, params);
    } else {
        /* Pants. I don't think that it should be possible to get here. */
        Perl_croak(aTHX_ "Usage: CODE(0x%"UVxf")(%s)", PTR2UV(cv), params);
    }
}
#undef  PERL_ARGS_ASSERT_CROAK_XS_USAGE

#ifdef PERL_IMPLICIT_CONTEXT
#define croak_xs_usage(a,b)    S_croak_xs_usage(aTHX_ a,b)
#else
#define croak_xs_usage        S_croak_xs_usage
#endif

#endif

/* NOTE: the prototype of newXSproto() is different in versions of perls,
 * so we define a portable version of newXSproto()
 */
#ifdef newXS_flags
#define newXSproto_portable(name, c_impl, file, proto) newXS_flags(name, c_impl, file, proto, 0)
#else
#define newXSproto_portable(name, c_impl, file, proto) (PL_Sv=(SV*)newXS(name, c_impl, file), sv_setpv(PL_Sv, proto), (CV*)PL_Sv)
#endif /* !defined(newXS_flags) */

#line 362 "RSA.c"

XS_EUPXS(XS_Crypt__OpenSSL__RSA_new_private_key); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_new_private_key)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "proto, key_string_SV");
    {
	SV*	proto = ST(0)
;
	SV*	key_string_SV = ST(1)
;
	SV *	RETVAL;
#line 221 "RSA.xs"
    RETVAL = make_rsa_obj(
        proto, _load_rsa_key(key_string_SV, PEM_read_bio_RSAPrivateKey));
#line 379 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA__new_public_key_pkcs1); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA__new_public_key_pkcs1)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "proto, key_string_SV");
    {
	SV*	proto = ST(0)
;
	SV*	key_string_SV = ST(1)
;
	SV *	RETVAL;
#line 231 "RSA.xs"
    RETVAL = make_rsa_obj(
        proto, _load_rsa_key(key_string_SV, PEM_read_bio_RSAPublicKey));
#line 402 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA__new_public_key_x509); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA__new_public_key_x509)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "proto, key_string_SV");
    {
	SV*	proto = ST(0)
;
	SV*	key_string_SV = ST(1)
;
	SV *	RETVAL;
#line 241 "RSA.xs"
    RETVAL = make_rsa_obj(
        proto, _load_rsa_key(key_string_SV, PEM_read_bio_RSA_PUBKEY));
#line 425 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_DESTROY); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_DESTROY)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 250 "RSA.xs"
    RSA_free(p_rsa->rsa);
    Safefree(p_rsa);
#line 451 "RSA.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_get_private_key_string); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_get_private_key_string)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;
#line 257 "RSA.xs"
    BIO* stringBIO;
#line 467 "RSA.c"
	SV *	RETVAL;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 259 "RSA.xs"
    CHECK_OPEN_SSL(stringBIO = BIO_new(BIO_s_mem()));
    PEM_write_bio_RSAPrivateKey(
        stringBIO, p_rsa->rsa, NULL, NULL, 0, NULL, NULL);
    RETVAL = extractBioString(stringBIO);

#line 482 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_get_public_key_string); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_get_public_key_string)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;
#line 271 "RSA.xs"
    BIO* stringBIO;
#line 500 "RSA.c"
	SV *	RETVAL;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 273 "RSA.xs"
    CHECK_OPEN_SSL(stringBIO = BIO_new(BIO_s_mem()));
    PEM_write_bio_RSAPublicKey(stringBIO, p_rsa->rsa);
    RETVAL = extractBioString(stringBIO);

#line 514 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_get_public_key_x509_string); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_get_public_key_x509_string)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;
#line 284 "RSA.xs"
    BIO* stringBIO;
#line 532 "RSA.c"
	SV *	RETVAL;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 286 "RSA.xs"
    CHECK_OPEN_SSL(stringBIO = BIO_new(BIO_s_mem()));
    PEM_write_bio_RSA_PUBKEY(stringBIO, p_rsa->rsa);
    RETVAL = extractBioString(stringBIO);

#line 546 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_generate_key); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_generate_key)
{
    dVAR; dXSARGS;
    if (items < 2 || items > 3)
       croak_xs_usage(cv,  "proto, bitsSV, exponent = 65537");
    {
	SV*	proto = ST(0)
;
	SV*	bitsSV = ST(1)
;
	unsigned long	exponent;
#line 299 "RSA.xs"
    RSA* rsa;
#line 568 "RSA.c"
	SV *	RETVAL;

	if (items < 3)
	    exponent = 65537;
	else {
	    exponent = (unsigned long)SvUV(ST(2))
;
	}
#line 301 "RSA.xs"
    CHECK_OPEN_SSL(rsa = RSA_generate_key(SvIV(bitsSV), exponent, NULL, NULL));
    RETVAL = make_rsa_obj(proto, rsa);
#line 580 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA__new_key_from_parameters); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA__new_key_from_parameters)
{
    dVAR; dXSARGS;
    if (items != 6)
       croak_xs_usage(cv,  "proto, n, e, d, p, q");
    {
	SV*	proto = ST(0)
;
	BIGNUM*	n = INT2PTR(BIGNUM *,SvIV(ST(1)))
;
	BIGNUM*	e = INT2PTR(BIGNUM *,SvIV(ST(2)))
;
	BIGNUM*	d = INT2PTR(BIGNUM *,SvIV(ST(3)))
;
	BIGNUM*	p = INT2PTR(BIGNUM *,SvIV(ST(4)))
;
	BIGNUM*	q = INT2PTR(BIGNUM *,SvIV(ST(5)))
;
#line 316 "RSA.xs"
    RSA* rsa;
    BN_CTX* ctx;
    BIGNUM* p_minus_1 = NULL;
    BIGNUM* q_minus_1 = NULL;
    int error;
#line 613 "RSA.c"
	SV *	RETVAL;
#line 322 "RSA.xs"
{
    if (!(n && e))
    {
        croak("At least a modulous and public key must be provided");
    }
    CHECK_OPEN_SSL(rsa = RSA_new());
    rsa->n = n;
    rsa->e = e;
    if (p || q)
    {
        error = 0;
        THROW(ctx = BN_CTX_new());
        if (!p)
        {
            THROW(p = BN_new());
            THROW(BN_div(p, NULL, n, q, ctx));
        }
        else if (!q)
        {
            q = BN_new();
            THROW(BN_div(q, NULL, n, p, ctx));
        }
        rsa->p = p;
        rsa->q = q;
        THROW(p_minus_1 = BN_new());
        THROW(BN_sub(p_minus_1, p, BN_value_one()));
        THROW(q_minus_1 = BN_new());
        THROW(BN_sub(q_minus_1, q, BN_value_one()));
        if (!d)
        {
            THROW(d = BN_new());
            THROW(BN_mul(d, p_minus_1, q_minus_1, ctx));
            THROW(BN_mod_inverse(d, e, d, ctx));
        }
        rsa->d = d;
        THROW(rsa->dmp1 = BN_new());
        THROW(BN_mod(rsa->dmp1, d, p_minus_1, ctx));
        THROW(rsa->dmq1 = BN_new());
        THROW(BN_mod(rsa->dmq1, d, q_minus_1, ctx));
        THROW(rsa->iqmp = BN_new());
        THROW(BN_mod_inverse(rsa->iqmp, q, p, ctx));
        THROW(RSA_check_key(rsa) == 1);
     err:
        if (p_minus_1) BN_clear_free(p_minus_1);
        if (q_minus_1) BN_clear_free(q_minus_1);
        if (ctx) BN_CTX_free(ctx);
        if (error)
        {
            RSA_free(rsa);
            CHECK_OPEN_SSL(0);
        }
    }
    else
    {
        rsa->d = d;
    }
    RETVAL = make_rsa_obj(proto, rsa);
}
#line 674 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA__get_key_parameters); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA__get_key_parameters)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 387 "RSA.xs"
{
    RSA* rsa;
    rsa = p_rsa->rsa;
    XPUSHs(bn2sv(rsa->n));
    XPUSHs(bn2sv(rsa->e));
    XPUSHs(bn2sv(rsa->d));
    XPUSHs(bn2sv(rsa->p));
    XPUSHs(bn2sv(rsa->q));
    XPUSHs(bn2sv(rsa->dmp1));
    XPUSHs(bn2sv(rsa->dmq1));
    XPUSHs(bn2sv(rsa->iqmp));
}
#line 712 "RSA.c"
	PUTBACK;
	return;
    }
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_encrypt); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_encrypt)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "p_rsa, p_plaintext");
    {
	rsaData*	p_rsa;
	SV*	p_plaintext = ST(1)
;
	SV *	RETVAL;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 405 "RSA.xs"
    RETVAL = rsa_crypt(p_rsa, p_plaintext, RSA_public_encrypt);
#line 739 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_decrypt); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_decrypt)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "p_rsa, p_ciphertext");
    {
	rsaData*	p_rsa;
	SV*	p_ciphertext = ST(1)
;
	SV *	RETVAL;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 414 "RSA.xs"
    if (!_is_private(p_rsa))
    {
        croak("Public keys cannot decrypt");
    }
    RETVAL = rsa_crypt(p_rsa, p_ciphertext, RSA_private_decrypt);
#line 771 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_private_encrypt); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_private_encrypt)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "p_rsa, p_plaintext");
    {
	rsaData*	p_rsa;
	SV*	p_plaintext = ST(1)
;
	SV *	RETVAL;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 427 "RSA.xs"
    if (!_is_private(p_rsa))
    {
        croak("Public keys cannot private_encrypt");
    }
    RETVAL = rsa_crypt(p_rsa, p_plaintext, RSA_private_encrypt);
#line 803 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_public_decrypt); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_public_decrypt)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "p_rsa, p_ciphertext");
    {
	rsaData*	p_rsa;
	SV*	p_ciphertext = ST(1)
;
	SV *	RETVAL;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 440 "RSA.xs"
    RETVAL = rsa_crypt(p_rsa, p_ciphertext, RSA_public_decrypt);
#line 831 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_size); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_size)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;
	int	RETVAL;
	dXSTARG;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 448 "RSA.xs"
    RETVAL = RSA_size(p_rsa->rsa);
#line 858 "RSA.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_check_key); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_check_key)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;
	int	RETVAL;
	dXSTARG;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 456 "RSA.xs"
    if (!_is_private(p_rsa))
    {
        croak("Public keys cannot be checked");
    }
    RETVAL = RSA_check_key(p_rsa->rsa);
#line 888 "RSA.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA__random_seed); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA__random_seed)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "random_bytes_SV");
    {
	SV*	random_bytes_SV = ST(0)
;
#line 471 "RSA.xs"
    STRLEN random_bytes_length;
    char* random_bytes;
#line 907 "RSA.c"
	int	RETVAL;
	dXSTARG;
#line 474 "RSA.xs"
    random_bytes = SvPV(random_bytes_SV, random_bytes_length);
    RAND_seed(random_bytes, random_bytes_length);
    RETVAL = RAND_status();
#line 914 "RSA.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA__random_status); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA__random_status)
{
    dVAR; dXSARGS;
    if (items != 0)
       croak_xs_usage(cv,  "");
    {
	int	RETVAL;
	dXSTARG;
#line 485 "RSA.xs"
    RETVAL = RAND_status();
#line 932 "RSA.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_md5_hash); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_md5_hash)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 493 "RSA.xs"
    p_rsa->hashMode = NID_md5;
#line 956 "RSA.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_sha1_hash); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_sha1_hash)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 499 "RSA.xs"
    p_rsa->hashMode =  NID_sha1;
#line 979 "RSA.c"
    }
    XSRETURN_EMPTY;
}

#ifdef SHA512_DIGEST_LENGTH
#define XSubPPtmpAAAA 1


XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_sha224_hash); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_sha224_hash)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 507 "RSA.xs"
    p_rsa->hashMode =  NID_sha224;
#line 1005 "RSA.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_sha256_hash); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_sha256_hash)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 513 "RSA.xs"
    p_rsa->hashMode =  NID_sha256;
#line 1028 "RSA.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_sha384_hash); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_sha384_hash)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 519 "RSA.xs"
    p_rsa->hashMode =  NID_sha384;
#line 1051 "RSA.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_sha512_hash); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_sha512_hash)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 525 "RSA.xs"
    p_rsa->hashMode =  NID_sha512;
#line 1074 "RSA.c"
    }
    XSRETURN_EMPTY;
}

#endif

XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_ripemd160_hash); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_ripemd160_hash)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 533 "RSA.xs"
    p_rsa->hashMode =  NID_ripemd160;
#line 1098 "RSA.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_no_padding); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_no_padding)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 539 "RSA.xs"
    p_rsa->padding = RSA_NO_PADDING;
#line 1121 "RSA.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_pkcs1_padding); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_pkcs1_padding)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 545 "RSA.xs"
    p_rsa->padding = RSA_PKCS1_PADDING;
#line 1144 "RSA.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_pkcs1_oaep_padding); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_pkcs1_oaep_padding)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 551 "RSA.xs"
    p_rsa->padding = RSA_PKCS1_OAEP_PADDING;
#line 1167 "RSA.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_sslv23_padding); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_use_sslv23_padding)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 557 "RSA.xs"
    p_rsa->padding = RSA_SSLV23_PADDING;
#line 1190 "RSA.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_sign); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_sign)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "p_rsa, text_SV");
    {
	rsaData*	p_rsa;
	SV*	text_SV = ST(1)
;
#line 566 "RSA.xs"
    char* signature;
    unsigned char* digest;
    unsigned int signature_length;
#line 1210 "RSA.c"
	SV *	RETVAL;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 570 "RSA.xs"
{
    if (!_is_private(p_rsa))
    {
        croak("Public keys cannot sign messages.");
    }

    CHECK_NEW(signature, RSA_size(p_rsa->rsa), char);

    CHECK_OPEN_SSL(digest = get_message_digest(text_SV, p_rsa->hashMode));
    CHECK_OPEN_SSL(RSA_sign(p_rsa->hashMode,
                            digest,
                            get_digest_length(p_rsa->hashMode),
                            (unsigned char*) signature,
                            &signature_length,
                            p_rsa->rsa));
    RETVAL = newSVpvn(signature, signature_length);
    Safefree(signature);
}
#line 1238 "RSA.c"
	ST(0) = RETVAL;
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_verify); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_verify)
{
    dVAR; dXSARGS;
    if (items != 3)
       croak_xs_usage(cv,  "p_rsa, text_SV, sig_SV");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	rsaData*	p_rsa;
	SV*	text_SV = ST(1)
;
	SV*	sig_SV = ST(2)
;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 599 "RSA.xs"
{
    unsigned char* sig;
    unsigned char* digest;
    STRLEN sig_length;

    sig = (unsigned char*) SvPV(sig_SV, sig_length);
    if (RSA_size(p_rsa->rsa) < sig_length)
    {
        croak("Signature longer than key");
    }

    CHECK_OPEN_SSL(digest = get_message_digest(text_SV, p_rsa->hashMode));
    switch(RSA_verify(p_rsa->hashMode,
                      digest,
                      get_digest_length(p_rsa->hashMode),
                      sig,
                      sig_length,
                      p_rsa->rsa))
    {
        case 0:
            CHECK_OPEN_SSL(ERR_peek_error());
            XSRETURN_NO;
            break;
        case 1:
            XSRETURN_YES;
            break;
        default:
            CHECK_OPEN_SSL(0);
            break;
    }
}
#line 1299 "RSA.c"
	PUTBACK;
	return;
    }
}


XS_EUPXS(XS_Crypt__OpenSSL__RSA_is_private); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Crypt__OpenSSL__RSA_is_private)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "p_rsa");
    {
	rsaData*	p_rsa;
	int	RETVAL;
	dXSTARG;

    if (!(SvROK(ST(0)) && sv_derived_from(ST(0), PACKAGE_NAME)))
    {
        croak("argument is not a rsaData * object");
    }
    p_rsa = (rsaData *) SvIV(SvRV(ST(0)))
;
#line 635 "RSA.xs"
    RETVAL = _is_private(p_rsa);
#line 1325 "RSA.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

#ifdef __cplusplus
extern "C"
#endif
XS_EXTERNAL(boot_Crypt__OpenSSL__RSA); /* prototype to pass -Wmissing-prototypes */
XS_EXTERNAL(boot_Crypt__OpenSSL__RSA)
{
    dVAR; dXSARGS;
#if (PERL_REVISION == 5 && PERL_VERSION < 9)
    char* file = __FILE__;
#else
    const char* file = __FILE__;
#endif

    PERL_UNUSED_VAR(cv); /* -W */
    PERL_UNUSED_VAR(items); /* -W */
#ifdef XS_APIVERSION_BOOTCHECK
    XS_APIVERSION_BOOTCHECK;
#endif
    XS_VERSION_BOOTCHECK;

        newXS("Crypt::OpenSSL::RSA::new_private_key", XS_Crypt__OpenSSL__RSA_new_private_key, file);
        newXS("Crypt::OpenSSL::RSA::_new_public_key_pkcs1", XS_Crypt__OpenSSL__RSA__new_public_key_pkcs1, file);
        newXS("Crypt::OpenSSL::RSA::_new_public_key_x509", XS_Crypt__OpenSSL__RSA__new_public_key_x509, file);
        newXS("Crypt::OpenSSL::RSA::DESTROY", XS_Crypt__OpenSSL__RSA_DESTROY, file);
        newXS("Crypt::OpenSSL::RSA::get_private_key_string", XS_Crypt__OpenSSL__RSA_get_private_key_string, file);
        newXS("Crypt::OpenSSL::RSA::get_public_key_string", XS_Crypt__OpenSSL__RSA_get_public_key_string, file);
        newXS("Crypt::OpenSSL::RSA::get_public_key_x509_string", XS_Crypt__OpenSSL__RSA_get_public_key_x509_string, file);
        newXS("Crypt::OpenSSL::RSA::generate_key", XS_Crypt__OpenSSL__RSA_generate_key, file);
        newXS("Crypt::OpenSSL::RSA::_new_key_from_parameters", XS_Crypt__OpenSSL__RSA__new_key_from_parameters, file);
        newXS("Crypt::OpenSSL::RSA::_get_key_parameters", XS_Crypt__OpenSSL__RSA__get_key_parameters, file);
        newXS("Crypt::OpenSSL::RSA::encrypt", XS_Crypt__OpenSSL__RSA_encrypt, file);
        newXS("Crypt::OpenSSL::RSA::decrypt", XS_Crypt__OpenSSL__RSA_decrypt, file);
        newXS("Crypt::OpenSSL::RSA::private_encrypt", XS_Crypt__OpenSSL__RSA_private_encrypt, file);
        newXS("Crypt::OpenSSL::RSA::public_decrypt", XS_Crypt__OpenSSL__RSA_public_decrypt, file);
        newXS("Crypt::OpenSSL::RSA::size", XS_Crypt__OpenSSL__RSA_size, file);
        newXS("Crypt::OpenSSL::RSA::check_key", XS_Crypt__OpenSSL__RSA_check_key, file);
        newXS("Crypt::OpenSSL::RSA::_random_seed", XS_Crypt__OpenSSL__RSA__random_seed, file);
        newXS("Crypt::OpenSSL::RSA::_random_status", XS_Crypt__OpenSSL__RSA__random_status, file);
        newXS("Crypt::OpenSSL::RSA::use_md5_hash", XS_Crypt__OpenSSL__RSA_use_md5_hash, file);
        newXS("Crypt::OpenSSL::RSA::use_sha1_hash", XS_Crypt__OpenSSL__RSA_use_sha1_hash, file);
#if XSubPPtmpAAAA
        newXS("Crypt::OpenSSL::RSA::use_sha224_hash", XS_Crypt__OpenSSL__RSA_use_sha224_hash, file);
        newXS("Crypt::OpenSSL::RSA::use_sha256_hash", XS_Crypt__OpenSSL__RSA_use_sha256_hash, file);
        newXS("Crypt::OpenSSL::RSA::use_sha384_hash", XS_Crypt__OpenSSL__RSA_use_sha384_hash, file);
        newXS("Crypt::OpenSSL::RSA::use_sha512_hash", XS_Crypt__OpenSSL__RSA_use_sha512_hash, file);
#endif
        newXS("Crypt::OpenSSL::RSA::use_ripemd160_hash", XS_Crypt__OpenSSL__RSA_use_ripemd160_hash, file);
        newXS("Crypt::OpenSSL::RSA::use_no_padding", XS_Crypt__OpenSSL__RSA_use_no_padding, file);
        newXS("Crypt::OpenSSL::RSA::use_pkcs1_padding", XS_Crypt__OpenSSL__RSA_use_pkcs1_padding, file);
        newXS("Crypt::OpenSSL::RSA::use_pkcs1_oaep_padding", XS_Crypt__OpenSSL__RSA_use_pkcs1_oaep_padding, file);
        newXS("Crypt::OpenSSL::RSA::use_sslv23_padding", XS_Crypt__OpenSSL__RSA_use_sslv23_padding, file);
        newXS("Crypt::OpenSSL::RSA::sign", XS_Crypt__OpenSSL__RSA_sign, file);
        newXS("Crypt::OpenSSL::RSA::verify", XS_Crypt__OpenSSL__RSA_verify, file);
        newXS("Crypt::OpenSSL::RSA::is_private", XS_Crypt__OpenSSL__RSA_is_private, file);

    /* Initialisation Section */

#line 214 "RSA.xs"
    ERR_load_crypto_strings();

#if XSubPPtmpAAAA
#endif
#line 1393 "RSA.c"

    /* End of Initialisation Section */

#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

