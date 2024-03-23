#include "stdafx.h"
//////////////////////////////////////////////////////////////////////////
/*
作者：xiexiaolesky
QQ群：678200
*/
// Sunday算法  从右向左匹配如果有匹配则返回从右至左的偏移量，如果没有匹配则返回 0 ！
#define MAX_SIZE_BYTE 256
byte *_GetStep(byte *bytes,DWORD len)
{
	static byte Step[MAX_SIZE_BYTE];
	for (UINT i=0;i<MAX_SIZE_BYTE;i++)
	{
		Step[i]=(byte)len+1;//如果不存在则设置值为 “小串长度+1”
	}
	for (UINT i = 0; i<len; i++)
	{
		Step[bytes[i]]=(byte)(len-i);//如果存在则设置值为 “从右边数的位置(从1开始)”
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

//判断是否可读 如果可读就返回 该页的偏移
DWORD _ReadPage(HANDLE m_hProcess, DWORD BaseAddr, byte* bytes,byte *byteStep,DWORD len,DWORD Start_Offset)
{ 
     BYTE arrayBytes[4096];
	 //每次读取1页内存
     if (!::ReadProcessMemory(m_hProcess, (LPVOID)BaseAddr,arrayBytes, 4096, NULL))
     {
         return  -1;//此页不可读
	 }
	return _ReadOffset(arrayBytes,bytes,byteStep,len,Start_Offset);
} 
//返回字符串的字节数组指针 和 对数组的长度赋值 (个人写法 可能存在问题！)
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
	if (processId == NULL) return - 1;//进程ID为空
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_OPERATION |
		PROCESS_VM_WRITE |
		PROCESS_VM_READ, FALSE, processId);
	if (hProcess != NULL)
	{	
		//设定搜索范围
		if (startAddr>=endAddr){return -2;}//范围错误
		DWORD Start=((DWORD)startAddr/4096)*4096;		
		DWORD Start_Offset=startAddr-Start;
		//遍历内存
		DWORD len;
		byte *bytes=_StringtoBytes(str,&len);
		byte *byteStep=_GetStep(bytes,len);//获取步长

		for (DWORD i = Start; i < endAddr; i += 4096)
		{
			DWORD offset = _ReadPage(hProcess, i,bytes,byteStep,len,Start_Offset);
			if (offset != -1)
			{
				if(bytes!=NULL) 
					delete bytes;//删掉指针
				if(hProcess!=NULL)
					::CloseHandle(hProcess);//关掉进程句柄

				return i + offset;	//如果找到则返回位置
			}
			Start_Offset=0;//还原开始偏移位置
		}
		if(bytes!=NULL) 
			delete bytes;
		if(hProcess!=NULL)
			::CloseHandle(hProcess);
	}
	else
	{
		return -3;//打开进程失败
	}
	return -4;//未找到特征码
}
