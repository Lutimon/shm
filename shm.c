/****************************************
  > File Name: shm.c
  > Author: lucky
  > Mail: min80231993@163.com 
  > Created Time: 2017年08月02日 星期三 08时50分46秒
 *************************************/

#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define		SHM_KEY	57483
#define		SIZE	4096	

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("usage: %s <operate>[create,write,read] [addr] [len] [data]\n", argv[0]);
		return -1;
	}
	if(strcmp(argv[1], "create") == 0){
		int shmid = shmget(SHM_KEY, SIZE, 0666 | IPC_CREAT);
		if(shmid == -1){
			perror("shmget");
			return -1;
		}
	} else{
		if(argc < 4){
			printf("no set <addr> <len>!\n");
			printf("usage: %s <operate>[create,write,read] [addr] [len] [data]\n", argv[0]);
			return -1;
		}
		int shmid = shmget(SHM_KEY, 0, 0);
		if(shmid == -1){
			perror("shmget");
			return -1;
		}
		void *mem = shmat(shmid, (void*)0, 0);
		if((void*)-1 == mem){
			perror("shmat");
			return -1;
		}

		if(strcmp(argv[1], "write") == 0){
			int len = atoi(argv[3]);
			if(len + 4 != argc){
				printf("write data and len are not match!\n");
				printf("usage: %s <operate>[create,write,read] [addr] [len] [data]\n", argv[0]);
				shmdt(mem);
				return -1;
			}

			unsigned char buf[len];
			printf("write data: ");
			int i;
			for(i = 0; i < len; i++){
				buf[i] = atoi(argv[4 + i]);
				printf("%d ", buf[i]);
			}
			printf("\n");
			memcpy((unsigned char*)mem + atoi(argv[2]), buf, len);

		} else if(strcmp(argv[1], "read") == 0){
			int len = atoi(argv[3]);
			unsigned char buf[len];
			memcpy(buf, (unsigned char*)mem + atoi(argv[2]), len);
			printf("read data: ");
			int i;
			for(i = 0; i < len; i++){
				printf("%d ", buf[i]);
			}
			printf("\n");
		}
		shmdt(mem);
	}
	return 0;
}
