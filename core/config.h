#pragma once

#define BOOT_LOADER_NAME L"MyBTLD"


extern char ImageBase;
extern char _kernel_end;

static const void* imageBegin = &ImageBase;
static const void* imageEnd = &_kernel_end;