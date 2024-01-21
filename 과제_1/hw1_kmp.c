#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 자료구조 첫번째 과제
// 20200110 정태현
// hw1_kmp.c 

#define MAX_STRING_SIZE    10000000
#define MAX_PATTERN_SIZE   3000       

char string[MAX_STRING_SIZE];
char pattern[MAX_PATTERN_SIZE];  

int matchCount = 0;
int matIdx[MAX_STRING_SIZE] = {0,};
int failure[MAX_PATTERN_SIZE];  // failure 배열

// 함수 선언
void fail(char* pat);
void pmatch(char* string, char* pat);

int main(void){

    // string 읽기
    FILE* fp = fopen("string.txt", "r");
    if(fp == NULL){
        printf("The string file does not exist.\n");
        exit(1);
    }
        
    fgets(string, sizeof(string), fp);    // string.txt에서 string 문자열 받아오기

    // pattern 읽기
    fp = fopen("pattern.txt", "r");   
    if(fp == NULL){
        printf("The string file does not exist.\n");
        exit(1);
    }

    fgets(pattern, sizeof(pattern), fp);  // pattern.txt에서 pattern 문자열 받아오기

    fclose(fp);  // 파일 close
   

   
    // pattern의 failure 배열 T[] 구하기!
    fail(pattern);
    
    // 핵심 연산 
    pmatch(string, pattern); 


    /////////// 이제 결과를 담을 파일 만들고 그것에 결과 넣기 //////////////
    FILE* outfp = fopen("result_kmp.txt", "w"); // 쓰기 모드로 열기
    if(outfp == NULL){ 
        printf("파일오픈 실패!\n");
        exit(1);
    }


    fprintf(outfp, "%d\n", matchCount); // 몇 번 매치 되었늦지 첫 줄에 쓰고
    for(int i=0; i<matchCount; i++){    // 두 번 째 줄 부터 하나하나 씩 idx찍어주기
        fprintf(outfp, "%d ", matIdx[i]);
    }

    return 0;
}

void fail(char* pat){ 

    int i, j;
    int n = strlen(pat);

    failure[0] = -1;   // T[0] = -1; 로 세팅
 
    for(j=1; j<n; j++) {

        i = failure[j-1];
        while((pat[j] != pat[i+1]) && (i >= 0)){
            i = failure[i];
        }

        if(pat[j] == pat[i+1]) 
            failure[j] = i+1;
        else 
            failure[j] = -1;
    }
}

void pmatch(char* string, char* pat){

// kmp 알고리즘을 바탕으로 함수 구성

    int i = 0, j = 0;
    int lens = strlen(string);
    int lenp = strlen(pat);

    while(i < lens && j < lenp) {   

        if(string[i] == pat[j]){
            i++;
            j++;
            if(j == lenp){
                matIdx[matchCount] = (i - lenp);  // 시작인덱스를 넣고
                matchCount++;            // 매치카운트 하나 증가시키고
                j = failure[j-1] + 1;    // j값 수정
            }
        }else if(j == 0){
            i++;
        }else{
            j = failure[j-1] + 1;
        }  
    }
}



