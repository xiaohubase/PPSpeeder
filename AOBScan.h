#include "stdafx.h"
//////////////////////////////////////////////////////////////////////////
/*
���ߣ�xiexiaolesky
QQȺ��678200
*/
// Sunday�㷨  ��������ƥ�������ƥ���򷵻ش��������ƫ���������û��ƥ���򷵻� 0 ��
#define MAX_SIZE_BYTE 256
byte *_GetStep(byte *bytes,DWORD len)
{
	static byte Step[MAX_SIZE_BYTE];
	for (UINT i=0;i<MAX_SIZE_BYTE;i++)
	{
		Step[i]=(byte)len+1;//���������������ֵΪ ��С������+1��
	}
	for (UINT i = 0; i<len; i++)
	{
		Step[bytes[i]]=(byte)(len-i);//�������������ֵΪ �����ұ�����λ��(��1��ʼ)��
	}
	return Step;
}

DWORD _ReadOffset(byte *arrayBytes,byte *bytes,byte *byteStep,DWORD len,DWORD Start_Offset)
{
	for (UINT i=Start_Offset;i<4096-len;i=i)
	{
		UINT p=0;
		for (UINT j=0;j<len;j++)
		{		
			if(arrayBytes[i+j]!=bytes[j]) {
				i+=byteStep[arrayBytes[i+len]];
				break;
			}
			else {p++;}
		}
		if (p==len)	{return i;}
	}
	return -1;
}

//�ж��Ƿ�ɶ� ����ɶ��ͷ��� ��ҳ��ƫ��
DWORD _ReadPage(HANDLE m_hProcess, DWORD BaseAddr, byte* bytes,byte *byteStep,DWORD len,DWORD Start_Offset)
{ 
     BYTE arrayBytes[4096];
	 //ÿ�ζ�ȡ1ҳ�ڴ�
     if (!::ReadProcessMemory(m_hProcess, (LPVOID)BaseAddr,arrayBytes, 4096, NULL))
     {
         return  -1;//��ҳ���ɶ�
	 }
	return _ReadOffset(arrayBytes,bytes,byteStep,len,Start_Offset);
} 
//�����ַ������ֽ�����ָ�� �� ������ĳ��ȸ�ֵ (����д�� ���ܴ������⣡)
byte * _StringtoBytes(char *str,DWORD *arrlen)
{
	int p=0,b=0;
	char chars[3]={'\0','\0','\0'};
	byte tmpBytes[1024];
	while(str[p]!='\0')
	{
		if (str[p]!=' '&&str[p]!=',')
		{
			chars[0]=str[p];
			if (str[p+1]!='\0' &&str[p+01]!=' ')
			{
				chars[1]=str[++p];
			}		
			tmpBytes[b]=(byte)strtol(chars,NULL,16);
			b++;
		}
		p++;
	}
	arrlen[0]=b;
	byte *retVal=new byte[b];
	for (int i=0;i<b;i++)
	{
		retVal[i]=tmpBytes[i];
	}
	return retVal;
}

DWORD _AOBScan(DWORD processId, char* str,DWORD startAddr,DWORD endAddr)
{
	if (processId == NULL) return - 1;//����IDΪ��
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_OPERATION |
		PROCESS_VM_WRITE |
		PROCESS_VM_READ, FALSE, processId);
	if (hProcess != NULL)
	{	
		//�趨������Χ
		if (startAddr>=endAddr){return -2;}//��Χ����
		DWORD Start=((DWORD)startAddr/4096)*4096;		
		DWORD Start_Offset=startAddr-Start;
		//�����ڴ�
		DWORD len;
		byte *bytes=_StringtoBytes(str,&len);
		byte *byteStep=_GetStep(bytes,len);//��ȡ����

		for (DWORD i = Start; i < endAddr; i += 4096)
		{
			DWORD offset = _ReadPage(hProcess, i,bytes,byteStep,len,Start_Offset);
			if (offset != -1)
			{
				if(bytes!=NULL) 
					delete bytes;//ɾ��ָ��
				if(hProcess!=NULL)
					::CloseHandle(hProcess);//�ص����̾��

				return i + offset;	//����ҵ��򷵻�λ��
			}
			Start_Offset=0;//��ԭ��ʼƫ��λ��
		}
		if(bytes!=NULL) 
			delete bytes;
		if(hProcess!=NULL)
			::CloseHandle(hProcess);
	}
	else
	{
		return -3;//�򿪽���ʧ��
	}
	return -4;//δ�ҵ�������
}
