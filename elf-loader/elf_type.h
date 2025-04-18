#pragma once
#include <uefi_defined.h>

#define EI_NIDENT       16

enum Elf_Ident {
	EI_MAG0			= 0, // 0x7F
	EI_MAG1			= 1, // 'E'
	EI_MAG2			= 2, // 'L'
	EI_MAG3			= 3, // 'F'
	EI_CLASS		= 4, // Architecture (32/64)
	EI_DATA			= 5, // Byte Order
	EI_VERSION		= 6, // ELF Version
	EI_OSABI		= 7, // OS Specific
	EI_ABIVERSION	= 8, // OS Specific
	EI_PAD			= 9  // Padding
};

// MAGIC NUMBER
#define ELFMAG0	0x7F // e_ident[EI_MAG0]
#define ELFMAG1	'E'  // e_ident[EI_MAG1]
#define ELFMAG2	'L'  // e_ident[EI_MAG2]
#define ELFMAG3	'F'  // e_ident[EI_MAG3]

// CLASS
#define ELF32	1    // e_ident[EI_CLASS]
#define ELF64	2    // e_ident[EI_CLASS]

// DATA
#define ELF_LITTLE	1    // e_ident[EI_DATA]
#define ELF_BIG		2    // e_ident[EI_DATA]

// VERSION
#define ELF_VIRSION_NOW	1    // e_ident[EI_VIRSION]

//e_typeのとり得る値 オブジェクトファイルの種類
#define ET_NONE   0      //ファイルタイプが存在しない 
#define ET_REL    1      //再配置可能ファイル 
#define ET_EXEC   2      //実行可能ファイル 
#define ET_DYN    3      //共有オブジェクトファイル 
#define ET_CORE   4      //コアファイル 
#define ET_LOPROC 0xff00 //プロセッサに固有 
#define ET_HIPROC 0xffff //プロセッサに固有

//e_machineのとり得る値 個々のファイルに必要なアーキテクチャを指定します
#define EM_NONE          0x00    // No machine
#define EM_M32           0x01    // AT&T WE 32100
#define EM_SPARC         0x02    // SPARC
#define EM_386           0x03    // Intel 80386
#define EM_68K           0x04    // Motorola 68000
#define EM_88K           0x05    // Motorola 88000
#define EM_IAMCU         0x06    // Intel MCU
#define EM_860           0x07    // Intel 80860
#define EM_MIPS          0x08    // MIPS I Architecture
#define EM_S370          0x09    // IBM System/370 Processor
#define EM_MIPS_RS3_LE   0x0A    // MIPS RS3000 Little-endian
#define EM_PARISC        0x0F    // Hewlett-Packard PA-RISC
#define EM_VPP500        0x11    // Fujitsu VPP500
#define EM_SPARC32PLUS   0x12    // Enhanced instruction set SPARC
#define EM_960           0x13    // Intel 80960
#define EM_PPC           0x14    // PowerPC
#define EM_PPC64         0x15    // PowerPC64
#define EM_S390          0x16    // IBM System/390
#define EM_ARM           0x28    // ARM
#define EM_ALPHA         0x29    // Digital Alpha
#define EM_SH            0x2A    // Hitachi SH
#define EM_SPARCV9       0x2B    // SPARC V9 64-bit
#define EM_TRICORE       0x2C    // Siemens Tricore
#define EM_ARC           0x2D    // Argonaut RISC Core
#define EM_IA_64         0x32    // Intel IA-64
#define EM_X86_64        0x3E    // AMD x86-64
#define EM_VAX           0x4B    // Digital VAX
#define EM_AVR           0x53    // Atmel AVR
#define EM_M32R          0x58    // Mitsubishi M32R
#define EM_OPENRISC      0x5C    // OpenRISC
#define EM_E2K           0x8D    // Elbrus 2000
#define EM_RISCV         0xF3    // RISC-V
#define EM_BPF           0xF7    // Linux BPF
#define EM_CSKY          0xFC    // C-SKY

// p_type
#define PT_NULL    0x00000000  // 未使用
#define PT_LOAD    0x00000001  // ロード可能セグメント
#define PT_DYNAMIC 0x00000002  // 動的リンク情報
#define PT_INTERP  0x00000003  // インタプリタ情報
#define PT_NOTE    0x00000004  // 補足情報（NOTEセクション）
#define PT_SHLIB   0x00000005  // 予約（使用されない）
#define PT_PHDR    0x00000006  // プログラムヘッダテーブル自身
#define PT_TLS     0x00000007  // TLS情報
#define PT_NUM     0x00000008  // タイプ数上限
// OS/Processor固有（範囲）
#define PT_LOOS    0x60000000  // OS固有最低値
#define PT_HIOS    0x6FFFFFFF  // OS固有最大値
#define PT_LOPROC  0x70000000  // CPU固有最低値
#define PT_HIPROC  0x7FFFFFFF  // CPU固有最大値

// p_flags
#define PF_X        0x01       // 実行可能
#define PF_W        0x02       // 書き込み可能
#define PF_R        0x04       // 読み取り可能

// sh_type
#define SHT_NULL          0x00000000  // 無効なセクション
#define SHT_PROGBITS      0x00000001  // プログラム情報（コードやデータ）
#define SHT_SYMTAB        0x00000002  // シンボルテーブル
#define SHT_STRTAB        0x00000003  // 文字列テーブル
#define SHT_RELA          0x00000004  // RELA型の再配置情報
#define SHT_HASH          0x00000005  // シンボルハッシュテーブル
#define SHT_DYNAMIC       0x00000006  // 動的リンク情報
#define SHT_NOTE          0x00000007  // 補足情報
#define SHT_NOBITS        0x00000008  // ファイルに存在しないBSSなど
#define SHT_REL           0x00000009  // REL型の再配置情報
#define SHT_SHLIB         0x0000000A  // 予約
#define SHT_DYNSYM        0x0000000B  // 動的リンク用シンボルテーブル
#define SHT_INIT_ARRAY    0x0000000E  // 初期化関数配列
#define SHT_FINI_ARRAY    0x0000000F  // 終了関数配列
#define SHT_PREINIT_ARRAY 0x00000010  // プレ初期化関数配列
#define SHF_ORDERED       0x00000011  // 特別注文要件 (Solaris)
#define SHF_EXCLUDE       0x00000012  // セクションは参照または割り当てられない限り除外されます (Solaris)

// sh_flags
#define SHF_WRITE            0x00000001  // 書き込み可能
#define SHF_ALLOC            0x00000002  // メモリに展開される
#define SHF_EXECINSTR        0x00000004  // 実行可能（命令列）
#define SHF_MERGE            0x00000010  // 同一内容の結合可能
#define SHF_STRINGS          0x00000020  // NULL終端文字列を含む
#define SHF_INFO_LINK        0x00000040  // sh_infoがリンクを含む
#define SHF_LINK_ORDER       0x00000080  // リンク順依存
#define SHF_OS_NONCONFORMING 0x00000100  // OS非準拠セクション
#define SHF_GROUP            0x00000200  // セクショングループの一部
#define SHF_TLS              0x00000400  // TLS用セクション
#define SHF_GROUP            0x00000200  // セクショングループの一部
#define SHF_TLS              0x00000400  // TLS用セクション
// マスク系
#define SHF_MASKOS           0x0FF00000  // OS固有フラグマスク
#define SHF_MASKPROC         0xF0000000  // CPU固有フラグマスク


typedef UINT32 Elf32_Addr; //符号なしプログラムアドレス 
typedef UINT16 Elf32_Half; //符号なし、中程度の整数 
typedef UINT32 Elf32_Off;  //符号なしファイルオフセット 
typedef INT32  Elf32_Sword;//符号付き整数 
typedef UINT32 Elf32_Word; //符号なし整数 
typedef UINT8  Elf32_Char; //符号なし、短い整数 

typedef UINT64 Elf64_Addr;  //符号なしプログラムアドレス 
typedef UINT16 Elf64_Half;  //符号なし、中程度の整数 
typedef UINT64 Elf64_Off;   //符号なしファイルオフセット 
typedef INT32  Elf64_Sword; //符号付き整数 
typedef UINT32 Elf64_Word;  //符号付き整数 
typedef UINT64 Elf64_Xword; //符号なし整数 
typedef INT64  Elf64_Sxword;//符号なし整数 
typedef UINT8  Elf64_Char;  //符号なし、短い整数 
 
typedef struct {
	Elf32_Char      e_ident[EI_NIDENT]; 
	Elf32_Half      e_type;
	Elf32_Half      e_machine;
	Elf32_Word      e_version;
	Elf32_Addr      e_entry;
	Elf32_Off       e_phoff;
	Elf32_Off       e_shoff;
	Elf32_Word      e_flags;
	Elf32_Half      e_ehsize;
	Elf32_Half      e_phentsize;
	Elf32_Half      e_phnum;
	Elf32_Half      e_shentsize;
	Elf32_Half      e_shnum;
	Elf32_Half      e_shstrndx;
} Elf32_Ehdr;

typedef struct {
	Elf64_Char      e_ident[EI_NIDENT]; 
	Elf64_Half      e_type;
	Elf64_Half      e_machine;
	Elf64_Word      e_version;
	Elf64_Addr      e_entry;
	Elf64_Off       e_phoff;
	Elf64_Off       e_shoff;
	Elf64_Word      e_flags;
	Elf64_Half      e_ehsize;
	Elf64_Half      e_phentsize;
	Elf64_Half      e_phnum;
	Elf64_Half      e_shentsize;
	Elf64_Half      e_shnum;
	Elf64_Half      e_shstrndx;
} Elf64_Ehdr;

typedef struct {
    Elf64_Word   p_type;    // セグメントの種類
    Elf64_Word   p_flags;   // セグメントのフラグ（実行/書込/読込）
    Elf64_Off    p_offset;  // ファイル中のオフセット
    Elf64_Addr   p_vaddr;   // 仮想アドレス
    Elf64_Addr   p_paddr;   // 物理アドレス（通常無視）
    Elf64_Xword  p_filesz;  // ファイル中のサイズ
    Elf64_Xword  p_memsz;   // メモリ中のサイズ
    Elf64_Xword  p_align;   // アライメント
} Elf64_Phdr;

typedef struct {
    Elf64_Word   sh_name;       // セクション名（文字列テーブルのオフセット）
    Elf64_Word   sh_type;       // セクションタイプ
    Elf64_Xword  sh_flags;      // セクション属性（ALLOC, EXEC, WRITEなど）
    Elf64_Addr   sh_addr;       // メモリ上のアドレス（ロードされる場合）
    Elf64_Off    sh_offset;     // ファイル内のオフセット
    Elf64_Xword  sh_size;       // セクションのサイズ（バイト）
    Elf64_Word   sh_link;       // 関連セクションのインデックス（シンボルテーブル等）
    Elf64_Word   sh_info;       // 補助情報（セクション数など）
    Elf64_Xword  sh_addralign;  // アライメント制約
    Elf64_Xword  sh_entsize;    // 固定サイズのエントリを持つ場合、そのサイズ（例：シンボル）
} Elf64_Shdr;