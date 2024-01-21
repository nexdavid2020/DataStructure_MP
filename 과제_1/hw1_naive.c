#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 자료구조 첫번째 과제
// 20200110 정태현
// hw1_naive.c 

#define MAX_STRING_SIZE   10000000
#define MAX_PATTERN_SIZE  3000     

char string[MAX_STRING_SIZE];
char pattern[MAX_PATTERN_SIZE];     
int matchCount = 0;
int matIdx[MAX_STRING_SIZE] = {0,};

void nfind(char *string, char *pat);

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
   

    // 핵심 계산
    nfind(string, pattern);  // 이거 실행하면 몇개 일치하는지, 그리고 어디가 시작점인지 다 담김


    /////////// 이제 결과를 담을 파일 만들고 그것에 결과 넣기 //////////////
    FILE* outfp = fopen("result_naive.txt", "w"); // 쓰기 모드로 열기
    if(outfp == NULL){ 
        printf("파일오픈 실패!\n");
        exit(1);
    }

    fprintf(outfp, "%d\n", matchCount);   // 몇 번 매치 되었늦지 첫 줄에 쓰고
    for(int i=0; i < matchCount; i++){    // 두 번 째 줄 부터 하나하나 씩 idx찍어주기
        fprintf(outfp, "%d ", matIdx[i]);
    }

    return 0;
}


void nfind(char *string, char *pat) {
    /* match the last character of the pattern first, then match from the beginning */
    int i, j;
    int start = 0;
  
    int lasts = strlen(string) - 1;
    int lastp = strlen(pat) - 1;

    int endmatch = lastp;  // 앤드 매치 부분을 패턴의 마지막 인덱스로 박아 놓기

    for(i = 0; endmatch <= lasts; endmatch++, start++) {
        if(string[endmatch] == pat[lastp]){
            for(j=0, i=start; j<lastp && string[i] == pat[j]; i++, j++){
            }
        }else{ // 만약에 끝이 다르면 다음 칸 비교하러 바로 ㄱㄱ
            continue;
        }   

        if(j == lastp){ // 매치가 일어난 경우!
            matIdx[matchCount] = start; // 매치 시작 인덱스 넣기
            matchCount++; // 매치 총 개수 더해주기
        }

    }

    return;  // 함수 종료
}