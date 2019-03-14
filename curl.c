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

Z size_t wr(S p, size_t s, size_t n, V*d) {
	K*x = d, y;
	jv(x, y = kpn(p, s*n));
	r0(y);
	R s*n;
}

#define so(c,o,v) curl_easy_setopt(c,CURLOPT_##o,v)

// Dup a q string into a NULL-terminated C string.
K dupCharList(K c) {
	assert(c && c->t == KC && c->n >= 0);
	K d = kpn(kC(c), c->n + 1);	//make sure string is terminated
	kC(d)[c->n] = '\0';
	R d;
}

/*
 * @param x (String) URL
 * @param h (StringList) List of custom headers
 * @param y (String) POST body (NULL for GET requests)
 */
K cpost(K x, K h, K y) {
	C e[CURL_ERROR_SIZE];
	K r, a;
	struct curl_slist *headers = NULL;
	CURL *c;
	if (xt != KC || y && y->t != KC)
		R krr("type");
	if (!(c = curl_easy_init()))
		R krr("curl");
	so(c, VERBOSE, is_verbose);
	assert(h);
	if (h->t != 0)
		R krr("type");
	assert(h->n >= 0);
	BOOL ok = TRUE;
	DO(h->n, ok = ok && kK(h)[i] && kK(h)[i]->t == KC);
	if (!ok)
		R krr("type");
	else {
		J i = 0, _i = h->n;
		for (; i < _i; ++i) {
			K header = dupCharList(kK(h)[i]);
			headers = curl_slist_append(headers, kC(header));
			r0(header);
		}
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
