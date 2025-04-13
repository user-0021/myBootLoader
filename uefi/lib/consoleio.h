#pragma once
#include <uefi/uefi_defined.h>
#include <uefi/console/console.h>

extern EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* stdout;

/*
 * 注意この関数はカーネルを読み込んでいない間のみの動作を想定しているためパフォーマンスが優れていない可能性があります
*/
void wcprintf(CHAR16* fmt,...);

