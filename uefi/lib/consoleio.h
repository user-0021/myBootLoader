#pragma once
#include <uefi/uefi_defined.h>
#include <uefi/console/console.h>


/*
 * 注意この関数はカーネルを読み込んでいない間のみの動作を想定しているためパフォーマンスが優れていない可能性があります
*/
void s_wcprintf(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* out,CHAR16* fmt,...);
