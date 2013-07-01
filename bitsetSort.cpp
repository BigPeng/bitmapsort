#include <bitset>
#define MAX_NUM 1000000//������������Ҫ��λ��
using namespace std;

int main(){
	FILE *readFp,*writeFp;
	readFp = fopen("phoneNumer.txt","r");		
	writeFp = fopen("bitsetSorted.txt","w");	
	bitset<MAX_NUM> bitmap;
	for(long i=0;i<MAX_NUM;i++){//�Ƚ�λͼ���Ի�Ϊ0
		bitmap.set(i,0);
	}
	printf("Begin set bitmap...\n");
	long number = 0;
	long count=0;
	while(fscanf(readFp,"%ld\n",&number) != EOF){
		//printf("%ld\t",number);
		bitmap.set(number,1);//��number����λ����Ϊ1	
		if(count++ % 10000 == 0)
			printf("count:%ld\tnumber:%ld\n",count,number);
	}
	printf("Begin search bitmap...\n");
	for(long i=0;i<MAX_NUM;i++){
		if(bitmap[i] == 1)//��λ1��λ������������ļ�
			fprintf(writeFp,"%ld\n",i);
	}
	fclose(writeFp);
	fclose(readFp);
}