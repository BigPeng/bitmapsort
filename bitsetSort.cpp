#include <bitset>
#define MAX_NUM 1000000//最多的数，即需要的位数
using namespace std;

int main(){
	FILE *readFp,*writeFp;
	readFp = fopen("phoneNumer.txt","r");		
	writeFp = fopen("bitsetSorted.txt","w");	
	bitset<MAX_NUM> bitmap;
	for(long i=0;i<MAX_NUM;i++){//先将位图初试化为0
		bitmap.set(i,0);
	}
	printf("Begin set bitmap...\n");
	long number = 0;
	long count=0;
	while(fscanf(readFp,"%ld\n",&number) != EOF){
		//printf("%ld\t",number);
		bitmap.set(number,1);//将number所在位设置为1	
		if(count++ % 10000 == 0)
			printf("count:%ld\tnumber:%ld\n",count,number);
	}
	printf("Begin search bitmap...\n");
	for(long i=0;i<MAX_NUM;i++){
		if(bitmap[i] == 1)//将位1的位输出到已排序文件
			fprintf(writeFp,"%ld\n",i);
	}
	fclose(writeFp);
	fclose(readFp);
}