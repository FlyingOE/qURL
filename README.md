# qURL
A very simple libcurl wrapper based on [KxSystems/kdb](https://github.com/KxSystems/kdb/blob/master/c/c/curl.c)'s implementation.

## Why qURL
kdb+ started to support HTTPS requests navtively since V3.4t 2016.05.12.

But as mentioned in the [documentation](https://code.kx.com/v2/kb/ssl/#suitability-and-restrictions), the native TLS support does not support concurrent queries within slave threads. Furthermore, kdb+'s native TLS support [works only with OpenSSL 1.0.2](https://code.kx.com/v2/kb/ssl/#prerequisites), which might not be desirable.

qURL aims to work around both of these problems by using the popular [libcurl](https://curl.haxx.se/libcurl/) to do the heavy lifting.

### Rant
As much as I love the simplicity of q, I hate the k-like code format in Kx's Java/C code. [curl.c](repo/blob/master/curl.c) here has, thus, been heavily reformatted before addition of extra features. `^_*`

## Why `curl.c`
Given the specific requirements of my project, I need only a thin wrapper around libcurl to perform simple concurrent HTTPS queries. Kx's sample implementation, while simplistic, is a very good starting point.

## Why curl for Windows
As my project mainly runs on Windows, I opted to use [curl for Windows](https://curl.haxx.se/windows/) as the libcurl version to use. For simplicity, all dependent libraries (headers/libs/dlls) required to build qURL under Visual Studio are directly included in this project.

If there is any issue with such inclusion, please kind let me know!

## Limitations
Only simple GET and POST queries are supported at the moment.
