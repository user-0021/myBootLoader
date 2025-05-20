#pragma once

//CR0 レジスタ
#define CR0_PE     (1UL << 0)   // 保護モード有効 (Protection Enable)
#define CR0_MP     (1UL << 1)   // モニタコプロセッサ (Monitor Coprocessor)
#define CR0_EM     (1UL << 2)   // エミュレーション (Emulation)
#define CR0_TS     (1UL << 3)   // タスクスイッチ (Task Switched)
#define CR0_ET     (1UL << 4)   // 拡張タイプ (Extension Type) - 常に1
#define CR0_NE     (1UL << 5)   // 数値エラー (Numeric Error)
#define CR0_WP     (1UL << 16)  // 書き込み保護 (Write Protect)
#define CR0_AM     (1UL << 18)  // アライメントマスク (Alignment Mask)
#define CR0_NW     (1UL << 29)  // キャッシュ無効 (Not Write-through)
#define CR0_CD     (1UL << 30)  // キャッシュディセーブル (Cache Disable)
#define CR0_PG     (1UL << 31)  // ページング有効 (Paging)

//CR4 レジスタ
#define CR4_VME        (1UL << 0)   // 仮想8086モード拡張
#define CR4_PVI        (1UL << 1)   // 保護モード仮想割り込み
#define CR4_TSD        (1UL << 2)   // TSC禁止
#define CR4_DE         (1UL << 3)   // デバッグ拡張
#define CR4_PSE        (1UL << 4)   // ページサイズ拡張
#define CR4_PAE        (1UL << 5)   // 物理アドレス拡張（64bitでは必須）
#define CR4_MCE        (1UL << 6)   // マシンチェック例外
#define CR4_PGE        (1UL << 7)   // グローバルページ
#define CR4_PCE        (1UL << 8)   // パフォーマンス監視
#define CR4_OSFXSR     (1UL << 9)   // SSE命令セット使用
#define CR4_OSXMMEXCPT (1UL << 10)  // SSE例外の有効化
#define CR4_UMIP       (1UL << 11)  // ユーザモードでの機密命令防止
#define CR4_LA57       (1UL << 12)  // 5レベルページング有効化
#define CR4_VMXE       (1UL << 13)  // 仮想化有効化 (Intel VMX)
#define CR4_SMXE       (1UL << 14)  // セキュアモード (TXT)
#define CR4_FSGSBASE   (1UL << 16)  // FS/GSベースレジスタ読み書き許可
#define CR4_PCIDE      (1UL << 17)  // プロセスコンテキスト識別子 (PCID)
#define CR4_OSXSAVE    (1UL << 18)  // XSAVEと関連命令のサポート
#define CR4_SMEP       (1UL << 20)  // Supervisor Mode Execution Prevention
#define CR4_SMAP       (1UL << 21)  // Supervisor Mode Access Prevention
#define CR4_PKE        (1UL << 22)  // Protection Key Enable
#define CR4_CET        (1UL << 23)  // Enable Control-flow Enforcement Technology
#define CR4_PKS	       (1UL << 24)  // Enable protection keys for supervisor-mode pages

// MSRアドレス
#define MSR_EFER           0xC0000080 // Extended Feature Enable Register
#define MSR_STAR           0xC0000081
#define MSR_LSTAR          0xC0000082
#define MSR_CSTAR          0xC0000083
#define MSR_SFMASK         0xC0000084
#define MSR_FS_BASE        0xC0000100
#define MSR_GS_BASE        0xC0000101
#define MSR_KERNEL_GS_BASE 0xC0000102

// EFERのビットフラグ
#define EFER_SCE       (1UL << 0)  // SYSCALL/SYSRET 有効
#define EFER_LME       (1UL << 8)  // Long mode enable
#define EFER_LMA       (1UL << 10) // Long mode active（読み取り専用）
#define EFER_NXE       (1UL << 11) // No-execute enable
