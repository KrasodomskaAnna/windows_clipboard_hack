#pragma warning(disable:4996)
#include <stdio.h>
#include <windows.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#define SIZE 256

char* getClip()
{
    char* res = NULL;
	if(!OpenClipboard(0))
		return res;
	HANDLE hMem = GetClipboardData(CF_OEMTEXT);
	if(!hMem) {
		CloseClipboard();
		return res;
	}
	char* p = (char*)GlobalLock(hMem);
	if(!p) {
		CloseClipboard();
		return res;
	}
	GlobalUnlock(hMem);
	CloseClipboard();
	return p;
}

int main()
{
	char* w = getClip();
	printf(w);
	char* wzor = "Ala ma kota";
	char nowy[SIZE] = "", txt[SIZE];
	const char podmianka[] = "Pies zjadl kota.";
	strcpy(txt, &w[0]);
	int idx = 0;
	for (int i = 0; i < strlen(txt); i++) {
		if (txt[i] == wzor[0]) {
			int match = 1;
			for (size_t j = 0; j < strlen(wzor); j++) {
				if (txt[i+j] != wzor[j]) {
					match = 0;
					break;
				}
			}
			if (!match) {
				nowy[idx] = txt[i];
				idx++;
			} else {
				for (size_t j = 0; j < strlen(podmianka); j++) {
					nowy[idx] = podmianka[j];
					idx++;
				}
				i += strlen(wzor)-1;
			}
		}
		else {
			nowy[idx] = txt[i];
			idx++;
		}
	}
    HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, strlen(nowy) + 1);
	if(!hMem)
		return -1;
	char* p = (char*)GlobalLock(hMem);
	if(!p) {
		GlobalFree(hMem);
		return -1;
	}
	for(size_t i = 0; i <= strlen(nowy); ++i)
		p[i] = nowy[i];
	GlobalUnlock(hMem);
	if(!OpenClipboard(0)) {
		GlobalFree(hMem);
		return -1;
	}
	EmptyClipboard();
	SetClipboardData(CF_OEMTEXT, hMem);
	CloseClipboard();
	return 0;
}