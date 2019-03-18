//see http://code.kx.com/wiki/Cookbook/ExtendingWithC
//gcc -DKXVER=3 curl.c -o curl.so -lcurl -fPIC -shared
//{x set`:./curl 2:x,y}'[`cget`cpost;1 2]
//cget"http://kx.com"
//2019.03.13 - reformat source code
//2019.03.14 - add support for custom query headers
#include "./curl.h"
#include <assert.h>

volatile BOOL is_verbose = FALSE;

/*
 * @param x (Bool) verbose flag
 */
K set_verbose(K x) {
	assert(x);
	switch (xt) {
	case -KB:
		is_verbose = !!xg;
		break;
	case -KS:
	case 101:
		break;
	default:
		R krr("type");
	}
	R kb(is_verbose);
}

#define so(c,o,v) curl_easy_setopt(c,CURLOPT_##o,v)

// HTTP response body write callback
Z size_t wr(S p, size_t s, size_t n, V*d) {
	K*x = d, y;
	jv(x, y = kpn(p, s*n));
	r0(y);
	R s*n;
}

// Duplicate a q string (no terminator) into a NULL-terminated C string.
C *dupCharList(K c) {
	errno_t error;
	assert(c && c->t == KC && c->n >= 0);
	const size_t n = (size_t)c->n;
	C *buffer = (C*)malloc(n + 1);
	assert(buffer);
	error = memcpy_s(buffer, n + 1, kC(c), n);
	assert(!error);
	buffer[n] = '\0';
	R buffer;
}

// Special handling for "Accept-Encoding" header
BOOL handleAcceptEncoding(CURL *c, const S header) {
	Z const S tag = "ACCEPT-ENCODING:";
	C h[(6 + 1 + 8) + 1 + 1];	//length of `tag'+1
	size_t i, ws;
	BOOL handled;
	if (strlen(header) < _countof(h)) {
		R FALSE;
	}
	for (i = 0; i < _countof(h); ++i) {
		h[i] = toupper(header[i]);
	}
	h[_countof(h) - 1] = '\0';
	if (handled = strcmp(tag, h) == 0) {
		ws = strspn(header + strlen(tag), " \t");
		so(c, ACCEPT_ENCODING, header + strlen(tag) + ws);
	}
	R handled;
}

// Build custom headers list for libcurl
struct curl_slist *buildCustomHeaders(K h, CURL *c, C *e, size_t en) {
	struct curl_slist *headers = NULL;
	BOOL ok;
	J i;
	memset(e, '\0', en);
	assert(h);
	if (h->t != 0) {
		errno_t error = strcpy_s(e, en, "type");
		assert(!error);
		R headers;
	}
	assert(h->n >= 0);
	for (ok = TRUE, i = 0; i < h->n; ++i) {
		ok = ok && kK(h)[i] && kK(h)[i]->t == KC;
	}
	if (!ok) {
		errno_t error = strcpy_s(e, en, "type");
		assert(!error);
		R headers;
	}
	for (i = 0; i < h->n; ++i) {
		S h1 = dupCharList(kK(h)[i]);
		if (!handleAcceptEncoding(c, h1)) {
			headers = curl_slist_append(headers, h1);
		}
		free(h1);
	}
	R headers;
}

// Setup libcurl options for OpenSSL
CURL *setupOpenSSL(CURL *c) {
	C* buffer;
	K p;

	p = k(0, "getenv`SSL_CERT_DIR", NULL);
	if (p && p->t == KC && p->n > 0) {
		buffer = dupCharList(p);
		so(c, CAPATH, buffer);
		free(buffer);
	}
	r0(p);

	p = k(0, "getenv`SSL_CERT_FILE", NULL);
	if (p && p->t == KC && p->n > 0) {
		buffer = dupCharList(p);
		so(c, CAINFO, buffer);
		free(buffer);
	}
	r0(p);

	p = k(0, "getenv`SSL_VERIFY_PEER", NULL);
	if (p && p->t == KC && p->n > 0) {
		buffer = dupCharList(p);
		so(c, SSL_VERIFYPEER, atoi(buffer));
		free(buffer);
	}
	r0(p);

	R c;
}

/*
 * @param x (String) URL
 * @param h (StringList) List of custom headers
 * @param y (String) POST body (NULL for GET requests)
 */
K cpost(K x, K h, K y) {
	C e[CURL_ERROR_SIZE];
	K r, a;
	struct curl_slist *headers;
	CURL *c;
	if (xt != KC || y && y->t != KC)
		R krr("type");
	if (!(c = curl_easy_init()))
		R krr("curl");
	so(c, VERBOSE, is_verbose);
	headers = buildCustomHeaders(h, c, e, _countof(e));
	if (strlen(e) > 0) {
		assert(!headers);
		R krr(e);
	}
	so(c, HTTPHEADER, headers);
	r = ktn(KC, 0);
	a = ktn(KC, xn + 1);
	DO(xn, kC(a)[i] = kC(x)[i]);
	kC(a)[xn] = 0;
	if (y)
		so(c, POST, 1L),
		so(c, POSTFIELDS, kC(y)),
		so(c, POSTFIELDSIZE, y->n);
	so(c, URL, kC(a));
	so(c, ERRORBUFFER, e);
	so(c, FOLLOWLOCATION, 1L);
#if 0
	so(c, SSL_VERIFYPEER, 0L);
	so(c, SSL_VERIFYHOST, 0L);
#else
	setupOpenSSL(c);
#endif
	so(c, WRITEFUNCTION, wr);
	so(c, WRITEDATA, &r);
	if (CURLE_OK != curl_easy_perform(c))
		r0(r),
		r = krr(e);
	r0(a);
	curl_slist_free_all(headers);
	headers = NULL;
	curl_easy_cleanup(c);
	R r;
}

/*
 * @see cpost
 */
K cget(K x, K h) {
	R cpost(x, h, 0);
}
