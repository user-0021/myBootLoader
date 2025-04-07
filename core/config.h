#pragma once

#define BOOT_LOADER_NAME L"MyBTLD"


extern char ImageBase;
extern char _kernel_end;

static __attribute__((__unused__)) const void* imageBegin = &ImageBase;
static __attribute__((__unused__)) const void* imageEnd = &_kernel_end;