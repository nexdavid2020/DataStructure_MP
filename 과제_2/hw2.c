#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 202001100 정태현 
// 자료구조 2차 과제 

typedef struct linkedlist{
    int data;
    struct linkedlist* prev;
    struct linkedlist* next;
} Linkedlist;

typedef Linkedlist* listPointer;

// 더블 링키드의 구조가 그냥 list가 제일 작은 수를 가르키고 있어야!


// 함수들
void print_forward(listPointer list);
void print_reverse(listPointer list);
void insert(listPointer* list, int n);  // 오름차순으로 Insert되어야!
void delete(listPointer* list, int n);



int main(int argc, char *argv[]){

    // 넘겨준 인자 개수가 2가 아닌 경우 예외 처리
    if(argc != 2){
        fprintf(stderr, "usage: ./hw2 input_filename\n");
        exit(1);
    }

    //파일 r모드로 열기
    FILE* fp = fopen(argv[1], "r");
    if(fp == NULL){
        fprintf(stderr, "The input file does not exist.\n");
        exit(1);
    }

    // 변수들
    char line[300];    // 한 줄 다 저장할 배열
    char command[20];  // 명령어만 저장할 배열
    int value; // 숫자 담기는 변수
    listPointer list = NULL;

    // 한 줄씩 읽어오기!
    while(fgets(line, sizeof(line), fp) != NULL){
        int n = sscanf(line, "%s %d", command, &value);

        // printf("%s %d\n", command, value);
        // printf("%d\n", n);
        
        if(n == 2){

            // Insert인 경우
            if(strcmp(command, "INSERT") == 0){
                insert(&list, value);
            }

            // Delete인 경우
            if(strcmp(command, "DELETE") == 0){
                delete(&list, value);
            }

        }else if(n == 1){ 
            // 명령어만 있는 경우!

            if(strcmp(command, "ASCEND") == 0){
                // ASCEND인 경우
                print_forward(list);
            }

            if(strcmp(command, "DESCEND") == 0){
                // DESCEND인 경우
                print_reverse(list);
            }
        }else{
            // 사실 실제로 실행될 일 없음 -> input 파일에는 오류 없다고 하셨음
            // fprintf(stderr, "잘못된 입력\n");
        }

    }

    return 0;
}

// print_forward()
void print_forward(listPointer list) {
    
    listPointer curr;
    FILE *outfile;
    outfile = fopen("hw2_result.txt", "a"); 

    if(list) {
        curr = list;
        
        while(1) {
            fprintf(outfile, "%d ", curr->data);
            printf("%d ", curr->data);
            curr = curr->next;
            if(curr == list) break;
        }      
    }
    fprintf(outfile, "\n");
    printf("\n");
    fclose(outfile);
}

// print_reverse()
void print_reverse(listPointer list) {

    listPointer curr;
    FILE *outfile;
    outfile = fopen("hw2_result.txt", "a"); 
    
    if(list) { // list가 널이 아닐 때 실행

        curr = list->prev;
        
        while(curr != list) {
            fprintf(outfile, "%d ", curr->data);
            printf("%d ", curr->data);
            curr = curr->prev;
        }
        fprintf(outfile, "%d ", curr->data);
        printf("%d ", curr->data);
    }

    fprintf(outfile, "\n");
    printf("\n");
    fclose(outfile);
}

void insert(listPointer* list, int n){ // 리스트의 시작은 *list
    listPointer curr;  
    listPointer first;
    listPointer last;
    listPointer temp;

    temp = (listPointer)malloc(sizeof(Linkedlist));
    if(temp == NULL){
        fprintf(stderr, "동적할당 실패\n");
        exit(1);
    }
    temp->data = n;
        

    if(*list == NULL){ // 첫 노드 들어간다~
        temp->next = temp;
        temp->prev = temp;
        *list = temp;
    }else{ // 첫 노드 아니야

        first = *list; // 첫 노드 주소 담기
        curr = first; // 현재 노드 curr
        last = first->prev;

        if(first == last){ // 리스트에 노드가 한 개밖에 없을 때
            if(curr->data == n){
                return; // 중복 허용 x
            }
            temp->next = curr;
            curr->prev = temp;
            curr->next = temp;
            temp->prev = curr;

            if(curr->data > n){
                *list = temp;
            }else{
                *list = curr;
            }
        
        }else{ // 노드가 한 개가 아닐 때
        
            while(curr->next != first){

                if(curr->data == n){
                    return; // 중복 허용 x
                }
                
                if(curr->data > n){
                    temp->prev = curr->prev;
                    temp->next = curr;
                    curr->prev->next = temp;
                    curr->prev = temp;
                    
                    if(curr==first){ // curr가 첫 노드 였을 때 *list를 temp로 바꿔줘야 함
                        *list = temp;
                    }
                    return;
                }else{ // curr->data < n
                    curr = curr->next;
                }                
            }

            if(last->data > n){
                temp->prev = last->prev;
                temp->next = last;
                last->prev->next = temp;
                last->prev = temp;
            }else if(last->data == n){
                return; // 중복 허용 x
            }else{
                // last->data < n
                // 새로 들어온 노드가 리스트의 마지막으로 들어가야 할 때
                last->next = temp;
                temp->prev = last;
                temp->next = first;
                first->prev = temp;
            }

        }
    }
}

void delete(listPointer* list, int n){

    listPointer curr;  
    listPointer first;
    listPointer last;
    listPointer nodeToDelete;

    if(*list == NULL){  // 링키드 리스트가 비어있을 때
        fprintf(stderr, "링키드 리스트 비어있어!\n");
        exit(1);
    
    }else{
        first = *list; // 첫 노드 주소 담기
        curr = first; // 현재 노드 curr
        last = (*list)->prev; // 마지막 노드 담기

        if(first == last){ // 링키드 리스트에 노드가 하나일 때
            
            if(first->data == n){ // 삭제해야
                (*list) = NULL;  // NULL로 초기화

                free(first);// 삭제
            }else{  // 삭제할 데이터 없음
                return; // 함수 종료
            }

        }else{ // 링키드 리스트 내에 노드가 여렀일 때

            while(curr->next != first){

                if(curr->data == n){
                    nodeToDelete = curr;
                    curr->prev->next = curr->next;
                    curr->next->prev = curr->prev;

                    if (curr == first) { // 만약 삭제하려는 노드가 링키드 리스트의 첫 노드이면 
                        *list = first->next; // 리스트의 시작점을 변경 해줘야 함
                    }
                    free(nodeToDelete); // 삭제
                    return;
                }
                curr = curr->next;
            }

            if(last->data == n){ // 삭제할 노드가 마지막 노드일 때 체크해줘야 함
                nodeToDelete = last;
                last->prev->next = first;
                first->prev = last->prev;
                free(nodeToDelete); // 삭제
            }
        }
    }
}