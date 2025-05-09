#pragma once
#include <stdint.h>

// Magic number
#define IMAGE_DOS_HEAD_MAGIC ((UINT16)0x5A4D)
#define IMAGE_PE_HEAD_MAGIC ((UINT16)0x00004550)
#define IMAGE_OPTIONAL_HEADER_MAGIC32 ((UINT16)0x10B)
#define IMAGE_OPTIONAL_HEADER_MAGIC64 ((UINT16)0x20B)

// Menber size
#define IMAGE_SIZEOF_SHORT_NAME 8
#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16

// Directory index
#define IMAGE_DIRECTORY_ENTRY_EXPORT		 0   // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT	 	 1   // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE		 2   // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION	     3   // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY		 4   // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC	     5   // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG		     6   // Debug Directory
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE	 7   // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR	     8   // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS			 9   // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG	 10  // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11  // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT			 12  // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13  // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14  // COM Runtime descriptor

// Machin type
#define IMAGE_FILE_MACHINE_UNKNOWN 	   0x0		//このフィールドの内容は、どのマシンタイプにも適用できると想定されています
#define IMAGE_FILE_MACHINE_ALPHA 	   0x184	//アルファ AXP、32 ビット アドレス空間
#define IMAGE_FILE_MACHINE_ALPHA64 	   0x284	//アルファ 64、64 ビット アドレス空間
#define IMAGE_FILE_MACHINE_AM33 	   0x1d3	//Matsushita AM33
#define IMAGE_FILE_MACHINE_AMD64 	   0x8664	//X64
#define IMAGE_FILE_MACHINE_ARM 		   0x1c0	//ARM リトル エンディアン
#define IMAGE_FILE_MACHINE_ARM64 	   0xaa64	//ARM64 リトル エンディアン
#define IMAGE_FILE_MACHINE_ARMNT 	   0x1c4	//ARM Thumb-2 リトル エンディアン
#define IMAGE_FILE_MACHINE_AXP64 	   0x284	//AXP 64 (アルファ 64 と同じ)
#define IMAGE_FILE_MACHINE_EBC 		   0xebc	//EFI バイト コード
#define IMAGE_FILE_MACHINE_I386 	   0x14c	//Intel 386 以降のプロセッサと互換性のあるプロセッサ
#define IMAGE_FILE_MACHINE_IA64 	   0x200	//Intel Itanium プロセッサ ファミリ
#define IMAGE_FILE_MACHINE_LOONGARCH32 0x6232	//LoongArch 32 ビット プロセッサ ファミリ
#define IMAGE_FILE_MACHINE_LOONGARCH64 0x6264	//LoongArch 64 ビット プロセッサ ファミリ
#define IMAGE_FILE_MACHINE_M32R		   0x9041	//三菱 M32R リトルエンディアン
#define IMAGE_FILE_MACHINE_MIPS16	   0x266	//MIPS16
#define IMAGE_FILE_MACHINE_MIPSFPU	   0x366	//FPU を使用した MIPS
#define IMAGE_FILE_MACHINE_MIPSFPU16   0x466	//FPU を使用した MIPS16
#define IMAGE_FILE_MACHINE_POWERPC 	   0x1f0	//Power PC リトル エンディアン
#define IMAGE_FILE_MACHINE_POWERPCFP   0x1f1	//浮動小数点をサポートする電源 PC
#define IMAGE_FILE_MACHINE_R3000BE 	   0x160	//MIPS I 互換 32 ビット ビッグ エンディアン
#define IMAGE_FILE_MACHINE_R3000 	   0x162	//MIPS I 互換 32 ビット リトル エンディアン
#define IMAGE_FILE_MACHINE_R4000 	   0x166	//MIPS III 互換 64 ビット リトル エンディアン
#define IMAGE_FILE_MACHINE_R10000 	   0x168	//MIPS IV 互換 64 ビット リトル エンディアン
#define IMAGE_FILE_MACHINE_RISCV32 	   0x5032	//RISC-V 32 ビット アドレス空間
#define IMAGE_FILE_MACHINE_RISCV64 	   0x5064	//RISC-V 64 ビット アドレス空間
#define IMAGE_FILE_MACHINE_RISCV128    0x5128	//RISC-V 128 ビット アドレス空間
#define IMAGE_FILE_MACHINE_SH3 		   0x1a2	//Hitachi SH3
#define IMAGE_FILE_MACHINE_SH3DSP 	   0x1a3	//Hitachi SH3 DSP
#define IMAGE_FILE_MACHINE_SH4 		   0x1a6	//Hitachi SH4
#define IMAGE_FILE_MACHINE_SH5 		   0x1a8	//Hitachi SH5
#define IMAGE_FILE_MACHINE_THUMB 	   0x1c2	//Thumb
#define IMAGE_FILE_MACHINE_WCEMIPSV2   0x169	//MIPS リトルエンディアン WCE v2

// file Chara
#define IMAGE_FILE_RELOCS_STRIPPED  	   0x0001//イメージのみ、Windows CE、および Microsoft Windows NT 以降。 これは、ファイルにベース再配置が含まれていないため、優先ベース アドレスで読み込む必要があることを示します。 ベース アドレスが使用できない場合、ローダーはエラーを報告します。 リンカーの既定の動作では、実行可能ファイル (EXE) からベース再配置を削除します。
#define IMAGE_FILE_EXECUTABLE_IMAGE  	   0x0002//イメージのみ。 これは、イメージ ファイルが有効であり、実行できることを示します。 このフラグが設定されていない場合は、リンカー エラーを示します。
#define IMAGE_FILE_LINE_NUMS_STRIPPED  	   0x0004//COFF 行番号が削除されました。 このフラグは非推奨であり、0 にする必要があります。
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED 	   0x0008//ローカル シンボルの COFF シンボル テーブルエントリが削除されました。 このフラグは非推奨であり、0 にする必要があります。
#define IMAGE_FILE_AGGRESSIVE_WS_TRIM  	   0x0010//互換性のために残されています。 ワーキング セットを積極的にトリミングします。 このフラグは Windows 2000 以降では非推奨であり、0 にする必要があります。
#define IMAGE_FILE_LARGE_ADDRESS_AWARE 	   0x0020//アプリケーションは > 2-GB のアドレスを処理できます。 
#define IMAGE_FILE_RESURVED 			   0x0040//このフラグは、今後使用するために予約されています。
#define IMAGE_FILE_BYTES_REVERSED_LO	   0x0080//リトル エンディアン: 最下位ビット (LSB) は、メモリ内の最上位ビット (MSB) の前にあります。 このフラグは非推奨であり、0 にする必要があります。
#define IMAGE_FILE_32BIT_MACHINE 		   0x0100//マシンは、32 ビットワード アーキテクチャに基づいています。
#define IMAGE_FILE_DEBUG_STRIPPED 		   0x0200//デバッグ情報がイメージ ファイルから削除されます。
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP 0x0400//イメージがリムーバブル メディア上にある場合は、イメージを完全に読み込み、スワップ ファイルにコピーします。
#define IMAGE_FILE_NET_RUN_FROM_SWAP 	   0x0800//イメージがネットワーク メディア上にある場合は、イメージを完全に読み込み、スワップ ファイルにコピーします。
#define IMAGE_FILE_SYSTEM 				   0x1000//イメージ ファイルはシステム ファイルであり、ユーザー プログラムではありません。
#define IMAGE_FILE_DLL 					   0x2000//イメージ ファイルはダイナミック リンク ライブラリ (DLL) です。 このようなファイルは、ほとんどの目的で実行可能ファイルと見なされますが、直接実行することはできません。
#define IMAGE_FILE_UP_SYSTEM_ONLY 		   0x4000//ファイルは、単一プロセッサ コンピューターでのみ実行する必要があります。
#define IMAGE_FILE_BYTES_REVERSED_HI 	   0x8000//ビッグ エンディアン: MSB はメモリ内の LSB の前にあります。 このフラグは非推奨であり、0 にする必要があります。


typedef struct _IMAGE_DOS_HEADER {	  		// DOS .EXE header
	uint16_t   e_magic;						// Magic number
	uint16_t   e_cblp;					 	// Bytes on last page of file
	uint16_t   e_cp;						// Pages in file
	uint16_t   e_crlc;					  	// Relocations
	uint16_t   e_cparhdr;				   	// Size of header in paragraphs
	uint16_t   e_minalloc;				  	// Minimum extra paragraphs needed
	uint16_t   e_maxalloc;				  	// Maximum extra paragraphs needed
	uint16_t   e_ss;						// Initial (relative) SS value
	uint16_t   e_sp;						// Initial SP value
	uint16_t   e_csum;					  	// Checksum
	uint16_t   e_ip;						// Initial IP value
	uint16_t   e_cs;						// Initial (relative) CS value
	uint16_t   e_lfarlc;					// File address of relocation table
	uint16_t   e_ovno;					  	// Overlay number
	uint16_t   e_res[4];					// Reserved uint16_ts
	uint16_t   e_oemid;					 	// OEM identifier (for e_oeminfo)
	uint16_t   e_oeminfo;				   	// OEM information; e_oemid specific
	uint16_t   e_res2[10];				  	// Reserved uint16_ts
	uint32_t   e_lfanew;					// Offset to the NT header
} IMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
	uint16_t Machine;
	uint16_t NumberOfSections;
	uint32_t TimeDateStamp;
	uint32_t PointerToSymbolTable;
	uint32_t NumberOfSymbols;
	uint16_t SizeOfOptionalHeader;
	uint16_t Characteristics;
} IMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
	uint32_t VirtualAddress;
	uint32_t Size;
} IMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER32 {
	uint16_t Magic;
	uint8_t  MajorLinkerVersion;
	uint8_t  MinorLinkerVersion;
	uint32_t SizeOfCode;
	uint32_t SizeOfInitializedData;
	uint32_t SizeOfUninitializedData;
	uint32_t AddressOfEntryPoint;
	uint32_t BaseOfCode;
	uint32_t BaseOfData;
	uint32_t ImageBase;
	uint32_t SectionAlignment;
	uint32_t FileAlignment;
	uint16_t MajorOperatingSystemVersion;
	uint16_t MinorOperatingSystemVersion;
	uint16_t MajorImageVersion;
	uint16_t MinorImageVersion;
	uint16_t MajorSubsystemVersion;
	uint16_t MinorSubsystemVersion;
	uint32_t Win32VersionValue;
	uint32_t SizeOfImage;
	uint32_t SizeOfHeaders;
	uint32_t CheckSum;
	uint16_t Subsystem;
	uint16_t DllCharacteristics;
	uint32_t SizeOfStackReserve;
	uint32_t SizeOfStackCommit;
	uint32_t SizeOfHeapReserve;
	uint32_t SizeOfHeapCommit;
	uint32_t LoaderFlags;
	uint32_t NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
	uint16_t Magic;
	uint8_t  MajorLinkerVersion;
	uint8_t  MinorLinkerVersion;
	uint32_t SizeOfCode;
	uint32_t SizeOfInitializedData;
	uint32_t SizeOfUninitializedData;
	uint32_t AddressOfEntryPoint;
	uint32_t BaseOfCode;
	uint64_t ImageBase;
	uint32_t SectionAlignment;
	uint32_t FileAlignment;
	uint16_t MajorOperatingSystemVersion;
	uint16_t MinorOperatingSystemVersion;
	uint16_t MajorImageVersion;
	uint16_t MinorImageVersion;
	uint16_t MajorSubsystemVersion;
	uint16_t MinorSubsystemVersion;
	uint32_t Win32VersionValue;
	uint32_t SizeOfImage;
	uint32_t SizeOfHeaders;
	uint32_t CheckSum;
	uint16_t Subsystem;
	uint16_t DllCharacteristics;
	uint64_t SizeOfStackReserve;
	uint64_t SizeOfStackCommit;
	uint64_t SizeOfHeapReserve;
	uint64_t SizeOfHeapCommit;
	uint32_t LoaderFlags;
	uint32_t NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_NT_HEADERS32 {
	uint32_t Signature;				  // "PE\0\0" = 0x00004550
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} IMAGE_NT_HEADERS32;


typedef struct _IMAGE_NT_HEADERS64 {
	uint32_t Signature;				  // "PE\0\0" = 0x00004550
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64;

typedef struct _IMAGE_SECTION_HEADER {
	uint8_t  Name[IMAGE_SIZEOF_SHORT_NAME];
	union {
		uint32_t PhysicalAddress;
		uint32_t VirtualSize;
	} Misc;
	uint32_t VirtualAddress;
	uint32_t SizeOfRawData;
	uint32_t PointerToRawData;
	uint32_t PointerToRelocations;
	uint32_t PointerToLinenumbers;
	uint16_t NumberOfRelocations;
	uint16_t NumberOfLinenumbers;
	uint32_t Characteristics;
} IMAGE_SECTION_HEADER;