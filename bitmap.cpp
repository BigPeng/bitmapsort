#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include "random.h"

#define MAX_NUM 16777216//最大的数,也就是需要的位
#define BYTE_NUM (1+MAX_NUM/8)//字节数
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
	unsigned char *bitmap;	//位图指针
	bitmap = (unsigned char *)malloc(BYTE_NUM*sizeof(unsigned char));
	if(bitmap == NULL){
		printf("Malloc failed\n");
		return -1;
	}	
	setAllZero(bitmap,BYTE_NUM);//将位图所有位设置为0
	setBitmap(bitmap,"phoneNumber.txt");//扫描待排文件，将位图对应位设置为1
	getSorted(bitmap,"bitmapSort.txt");	//扫描位图，将位图为1的位号输出到文件
	free(bitmap);//释放位图
	return 0;
}
/*****顺序遍历位图输出记录，从而实现排序****************/
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
/***********设置待排序数据的位图**************/
bool setBitmap(unsigned char *bitmap,char *fileName){
	FILE *readFp;
	printf("Setting bitmap...\n");
	readFp = fopen(fileName,"r");
	if(readFp == NULL)
		return false;	
	long phoneNum=0;
	while(fscanf(readFp,"%ld\n",&phoneNum) != EOF){
		setOne(bitmap,phoneNum);//将	phoneNum位设置为1	
	}
	fclose(readFp);
	return true;
}
/******先将位图清零********/
void setAllZero(unsigned char *bitmap,long size){
	for(long i=0;i<size;i++)
		*(bitmap+i) &= 0;
}
/*************************************************
将指定的位置为1
(loc>>3)相当于整除2^3=8，即定位到字节数，MASK=0x07,loc&MASK相当于loc%8
***************************************************/
void setOne(unsigned char *bitmap,long loc){
	*(bitmap+(loc>>3)) |= (1<<(loc&MASK));//
}

/******查找指定的位是否为1********/
int find(unsigned char *bitmap,long loc){
	return ((*(bitmap+(loc>>3))) & (1<<(loc&MASK))) == (1<<(loc&MASK));	
}

/********读待排序的数据，存在数组中**********/
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
/*******写排序后的文件***************/
void writeSorted(char *fileName,long * data,long length){
	FILE * writeFp;
	writeFp = fopen(fileName,"w");
	for(long i=0;i<length;i++)
		fprintf(writeFp,"%ld\n",*(data+i));
	fclose(writeFp);

}
/********一趟快速排序*************/
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
/**************快速排序************/
void quickSort(long *data,long low,long high){
	if (low < high){
		long pivotLoc = partion(data,low,high);
		//printf("%ld\t",pivotLoc);
		quickSort(data,low,pivotLoc-1);
		quickSort(data,pivotLoc+1,high);
	}
}

/*********生成一个左右摆动的序列:1，-1,2，-2...**************/
long getNextIndex(long size,long index){
	static short tag = -1;
	static long left = 0;
	static long right = 0;
	if (index == -1){//对不同的index,需要将static变量重置
		tag = -1;
		left = 0;
		right = 0;
	}
//	printf("left: %ld right: %d\n",left,right);
	if(index + (left - 1) < 0 && index +(right + 1) >= size)  
		return 0;//已经遍历完，不需要再找了
	if (index + (left - 1) < 0)
		return ++right;//左边已经越出界限了，试探右边
	if (index+(right + 1)>=size) 
		return --left;//右边已经越出界限了，试探左边
	if (tag == -1){//左右都没有出界，左右依次试探	
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
		while(find(bitmap,index+offset) == 1){//这个数已经用过或者不存在
			offset = getNextIndex(maxNum,index);
			//printf("offset:%ld\n",offset);
			if(offset == 0){//查找的偏移量为0说明数都用过了
				fclose(phoneNumFile);
				return;
			}
		}
		getNextIndex(maxNum,-1);//将static变量重置
		long loc = index+offset;
		setOne(bitmap,loc);
		//printf("index:%ld offset:%ld\n",index,offset);
		fprintf(phoneNumFile,"%ld\n",loc);
		if(++count > percent)//保存了80%终止
			break;
		if(count%1000000==0)
			printf("count:\t%ld\n",count);
	}
	fclose(phoneNumFile);
}
