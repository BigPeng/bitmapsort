#include "random.h"



/********将bit所在位设置为1**********/
void setOne(long * p, short bit){
	*p |= 1<<bit;
}
/********将bit所在位设置0**********/
void setZero(long *p, short bit){
	*p &=~(1<<bit);
}
/*******生成0-2^bitNum-1之间的随机数**************/
long randLong(short bitNum){
	long a = 0;
	long *p = &a;	
	for(short i= 0;i < bitNum; i++){
		short bit = rand() % 2;
		if(bit == 0)
			setZero(p,i);
		else
			setOne(p,i);
	}
	return *p;
}
void main1(){
	const int max = 16;
	int count[max];
	for(short i=0;i<max;i++)
		count[i] = 0;
	long num=1000000;	
	for(long i=0;i<num;i++){
		long index = randLong(4);		
		count[index]++;		
	}
	for(short i=0;i<max;i++)
		printf("%d:\t%lf\n",i,1.0*count[i]/num);	
}