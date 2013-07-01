#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include "random.h"

#define MAX_NUM 16777216//������,Ҳ������Ҫ��λ
#define BYTE_NUM (1+MAX_NUM/8)//�ֽ���
#define MASK 0x07



void setAllZero(unsigned char *p,long size);
void setOne(unsigned char *p,long loc);
int find(unsigned char *p,long loc);
bool getSorted(unsigned char *bitmap,char *fileName);
bool setBitmap(unsigned char *bitmap,char *fileName);
void readAsArray(char *fileName,long *phoneNums,long *count);
void writeSorted(char *fileName,long * data,long length);
void quickSort(long *data,long low,long high);
void testQuickSort();
int bitmapSort();
long getNextIndex(long size,long index);
void makePhoneNum(unsigned char *bitmap,long maxNum,short bitSize);



void makeTestData(){
	unsigned char *bitmap;	
	bitmap = (unsigned char *)malloc(BYTE_NUM*sizeof(unsigned char));
	setAllZero(bitmap,BYTE_NUM);
	makePhoneNum(bitmap,MAX_NUM,24);
	free(bitmap);
}
int main1(){ 
	//makeTestData();
	testQuickSort();
	//bitmapSort();
	return 0;	
}
void testQuickSort(){
	long length;	
	long *phoneNums=new long[11702526];
	readAsArray("phoneNumber.txt",phoneNums,&length);
	printf("Begin quick sort\n");
	quickSort(phoneNums,0,length-1);
	printf("Write sorted\n");
	writeSorted("quicksort.txt",phoneNums,length);
}
int bitmapSort(){
	unsigned char *bitmap;	//λͼָ��
	bitmap = (unsigned char *)malloc(BYTE_NUM*sizeof(unsigned char));
	if(bitmap == NULL){
		printf("Malloc failed\n");
		return -1;
	}	
	setAllZero(bitmap,BYTE_NUM);//��λͼ����λ����Ϊ0
	setBitmap(bitmap,"phoneNumber.txt");//ɨ������ļ�����λͼ��Ӧλ����Ϊ1
	getSorted(bitmap,"bitmapSort.txt");	//ɨ��λͼ����λͼΪ1��λ��������ļ�
	free(bitmap);//�ͷ�λͼ
	return 0;
}
/*****˳�����λͼ�����¼���Ӷ�ʵ������****************/
bool getSorted(unsigned char *bitmap,char *fileName){
	printf("Search bitmap...\n");
	FILE *writeFp;
	writeFp = fopen(fileName,"w");
	if(writeFp == NULL)
		return false;
	long phoneNum=0;
	for(phoneNum = 0; phoneNum < MAX_NUM; phoneNum += 1){
		if(find(bitmap,phoneNum)){
			fprintf(writeFp,"%ld\n",phoneNum);
		}
	}
	fclose(writeFp);
	return true;
}
/***********���ô��������ݵ�λͼ**************/
bool setBitmap(unsigned char *bitmap,char *fileName){
	FILE *readFp;
	printf("Setting bitmap...\n");
	readFp = fopen(fileName,"r");
	if(readFp == NULL)
		return false;	
	long phoneNum=0;
	while(fscanf(readFp,"%ld\n",&phoneNum) != EOF){
		setOne(bitmap,phoneNum);//��	phoneNumλ����Ϊ1	
	}
	fclose(readFp);
	return true;
}
/******�Ƚ�λͼ����********/
void setAllZero(unsigned char *bitmap,long size){
	for(long i=0;i<size;i++)
		*(bitmap+i) &= 0;
}
/*************************************************
��ָ����λ��Ϊ1
(loc>>3)�൱������2^3=8������λ���ֽ�����MASK=0x07,loc&MASK�൱��loc%8
***************************************************/
void setOne(unsigned char *bitmap,long loc){
	*(bitmap+(loc>>3)) |= (1<<(loc&MASK));//
}

/******����ָ����λ�Ƿ�Ϊ1********/
int find(unsigned char *bitmap,long loc){
	return ((*(bitmap+(loc>>3))) & (1<<(loc&MASK))) == (1<<(loc&MASK));	
}

/********������������ݣ�����������**********/
void readAsArray(char *fileName,long *phoneNums,long *count){
	FILE *readFp;
	readFp = fopen(fileName,"r");
	*count=0;
	long phoneNum=0;
	while(fscanf(readFp,"%ld\n",&phoneNum) != EOF){
		phoneNums[(*count)++] = phoneNum;
	}
	fclose(readFp);
}
/*******д�������ļ�***************/
void writeSorted(char *fileName,long * data,long length){
	FILE * writeFp;
	writeFp = fopen(fileName,"w");
	for(long i=0;i<length;i++)
		fprintf(writeFp,"%ld\n",*(data+i));
	fclose(writeFp);

}
/********һ�˿�������*************/
long partion(long *data,long low,long high){
    long pivotkey = *(data+low);
    while(low < high){
        while (low < high && *(data+high) >= pivotkey)
            high -= 1;
         *(data+low) = *(data+high);
        while(low < high && *(data+low) <= pivotkey)
            low += 1;
        *(data+high) = *(data+low);
	}
   *(data+low) = pivotkey;
    return low;
}
/**************��������************/
void quickSort(long *data,long low,long high){
	if (low < high){
		long pivotLoc = partion(data,low,high);
		//printf("%ld\t",pivotLoc);
		quickSort(data,low,pivotLoc-1);
		quickSort(data,pivotLoc+1,high);
	}
}

/*********����һ�����Ұڶ�������:1��-1,2��-2...**************/
long getNextIndex(long size,long index){
	static short tag = -1;
	static long left = 0;
	static long right = 0;
	if (index == -1){//�Բ�ͬ��index,��Ҫ��static��������
		tag = -1;
		left = 0;
		right = 0;
	}
//	printf("left: %ld right: %d\n",left,right);
	if(index + (left - 1) < 0 && index +(right + 1) >= size)  
		return 0;//�Ѿ������꣬����Ҫ������
	if (index + (left - 1) < 0)
		return ++right;//����Ѿ�Խ�������ˣ���̽�ұ�
	if (index+(right + 1)>=size) 
		return --left;//�ұ��Ѿ�Խ�������ˣ���̽���
	if (tag == -1){//���Ҷ�û�г��磬����������̽	
		tag *= -1;
		return ++right;
	}else{
		tag *= -1;
		return --left;
	}
}

void makePhoneNum(unsigned char *bitmap,long maxNum,short bitSize){
	FILE * phoneNumFile = fopen("phoneNumber.txt","w");
	long count = 0;
	long percent = 0.8*maxNum;
	while(true){
		long index = randLong(bitSize);
		long offset = 0;
		while(find(bitmap,index+offset) == 1){//������Ѿ��ù����߲�����
			offset = getNextIndex(maxNum,index);
			//printf("offset:%ld\n",offset);
			if(offset == 0){//���ҵ�ƫ����Ϊ0˵�������ù���
				fclose(phoneNumFile);
				return;
			}
		}
		getNextIndex(maxNum,-1);//��static��������
		long loc = index+offset;
		setOne(bitmap,loc);
		//printf("index:%ld offset:%ld\n",index,offset);
		fprintf(phoneNumFile,"%ld\n",loc);
		if(++count > percent)//������80%��ֹ
			break;
		if(count%1000000==0)
			printf("count:\t%ld\n",count);
	}
	fclose(phoneNumFile);
}
