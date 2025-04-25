#include "consoleio.h"

#include <stddef.h>
#include <stdarg.h>

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* stdout = NULL;

#ifndef IGNORE_PRINT
void wcprintf(CHAR16* fmt,...){
	if(stdout == NULL)
		return;

	CHAR16 buffer[2] = {L'\0',L'\0'};

    va_list args;
    va_start(args, fmt); 

	while(*fmt != L'\0'){

		switch (*fmt)
		{
		case L'%':{
			fmt++;
			UINTN dispNumLen = 0; 
			BOOLEAN isLimited = 0,isFillZero = 0;

			//check head
			if(*fmt == L'0'){
				isFillZero = 1;
				fmt++;
			}

			//convert to int
			while (*fmt > L'0' && *fmt < L'9'){
				isLimited = 1;
				dispNumLen*=10;
				dispNumLen+=*fmt-L'0';
				fmt++;
			}

			switch (*fmt)
			{
			case L'%':
				buffer[0] = *fmt;
				stdout->OutputString(stdout,buffer);
				break;
			case L's':{
            	CHAR16* s = va_arg(args,CHAR16*);
				stdout->OutputString(stdout,s);
			}
			break;
			case L'd':
			case L'u':{
				INT64 d = va_arg(args,INT64);
				BOOLEAN isConverting = 0;
				UINTN digitChecker = 10000000000000000000U;

				//is negnum
				if(*fmt == L'd' && d & ((uint64_t)1<<63)){
					d*=-1;
					buffer[0] = L'-';
					stdout->OutputString(stdout,buffer);
				}

				//serch digit
				if(isLimited){
					UINTN i;
					digitChecker = 1;
					for(i=1;i > dispNumLen;i++)
						digitChecker*=10;
					
					if(isFillZero)
						isConverting = 1;
				}else{
					while (!isConverting)
					{
						if(d / digitChecker)
							isConverting = 1;
						else
							digitChecker/=10;

						if(!digitChecker){
							digitChecker = 1;
							isConverting = 1;
						}
					}
				}

				do{
					if(isConverting){
						buffer[0] = L'0' + (d/digitChecker)%10;
						stdout->OutputString(stdout,buffer);
						digitChecker/=10;
					}else{
						if((d/digitChecker)%10)
							isConverting = 1;
						else{
							digitChecker/=10;
							buffer[0] = L' ';
							stdout->OutputString(stdout,buffer);
						}

						if(!digitChecker){
							digitChecker = 1;
							isConverting = 1;
						}
					}
				}while(digitChecker);

				break;
			}
			case L'x':{
				UINT64 x = va_arg(args,UINT64);
				BOOLEAN isConverting = 0;
				UINTN digitChecker = 15;

				//serch digit
				if(isLimited){
					digitChecker = (digitChecker < dispNumLen) ? digitChecker : (dispNumLen - 1);
					
					if(isFillZero)
						isConverting = 1;
				}else{
					if(isFillZero)
						isConverting = 1;

					while (!isConverting)
					{
						if((x >> (digitChecker << 2)) & 0xF)
							isConverting = 1;
						else
							digitChecker--;

						if(!digitChecker){
							isConverting = 1;
						}
					}
				}

				do{
					if(isConverting){
						UINT8 byte = (x >> (digitChecker << 2)) & 0xF;
						if(byte > 9)
							buffer[0] = L'A' + (byte - 10);
						else
							buffer[0] = L'0' + byte;

						stdout->OutputString(stdout,buffer);
						digitChecker--;
					}else{
						if((x >> (digitChecker << 2)) & 0xF)
							isConverting = 1;
						else{
							digitChecker--;
							buffer[0] = L' ';
							stdout->OutputString(stdout,buffer);
						}

						if(!digitChecker){
							digitChecker = 1;
							isConverting = 1;
						}
					}
				}while(!(digitChecker & ((uint64_t)1<<63)));

				break;
			}
			
			default:
				break;
			}
			break;
		}
		
		default:
			buffer[0] = *fmt;
			stdout->OutputString(stdout,buffer);
			break;
		}

		fmt++;
	}

    va_end(args); 
}

#endif