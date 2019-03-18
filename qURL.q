/ @see curl.c
\d .curl

DLL:`:qURL

/ verbose flag
/q) .curl.verbose[]		//get current value
/q) .curl.verbose 1b	//enable cURL verbose mode
verbose:DLL 2:(`set_verbose;1)

/ HTTP GET<p>
/ Some environment variables are respected:
/ <dl>
/	<dt>{@code SSL_CERT_DIR}</dt>
/	<dd>Directory where SSL CA certificates are found (CA certificates as separate files).</dd>
/	<dt>{@code SSL_CERT_FILE}</dt>
/	<dd>SSL CA certificate bundle to use (CA certificates stored as a single bundle).</dd>
/	<dt>{@code SSL_VERIFY_PEER}</dt>
/	<dd>{@literal 0} to disable SSL peer verification.</dd>
/ </dl>
/q) .j.k .curl.cget["http://httpbin.org/headers";("Accept-Encoding: gzip";"Connection: close")]
/q) .j.k .curl.cget["https://httpbin.org/get";()]
cget:DLL 2:(`cget;2)

/ HTTP POST
/q) .j.k .curl.cpost["https://httpbin.org/response-headers?key=value";();""]
/q) .j.k .curl.cpost["http://httpbin.org/post";(enlist"Content-Type: text/plain");"Hello World!"]
cpost:DLL 2:(`cpost;3)

\
__EOD__
/=============================================================================