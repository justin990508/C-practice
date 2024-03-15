#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int num; // 물건들의 수를 나타내는 변수
int value[25]; // 물건들의 가격을 나타내는 배열
int weight[25]; // 물건들의 무게를 나타내는 배열
int Max_weight; // 배낭이 담을 수 있는 최대 무게를 나타내는 변수
int Best_value; // 지금까지 얻을 수 있는 최고 가격

// Branch-And-Bound 탐색트리를 위한 노드를 정의
typedef struct node {
    int level; // 탐색트리에서 노드의 수준
    int value; // 노드에 있는 물건들을 선택하여 얻을 수 있는 가격
    int weight; // 노드에 있는 물건들의 무게
    bool selected[25]; // 각 물건들이 선택되었는지 가리키는 배열
} node;

// 노드를 저장하기 위한 우선순위 큐를 정의
typedef struct priority_queue {
    int size;
    node* data[25];
} priority_queue;

// 새로운 노드 생성
node* Create_node(int level, int value, int weight, bool* selected) {
    node* New_node = (node*) malloc(sizeof(node)); // malloc() 함수로 메모리 동적 할당
    New_node->level = level; // New_node가 가리키는 노드 구조체의 level 필드를 Create_node() 함수에 전달된 level 매개변수의 값으로 설정
    New_node->value = value; // New_node가 가리키는 노드 구조체의 value 필드를 Create_node() 함수에 전달된 value 매개변수의 값으로 설정
    New_node->weight = weight; // New_node가 가리키는 노드 구조체의 weight 필드를 Create_node() 함수에 전달된 weight 매개변수의 값으로 설정
    for (int i = 0; i < num; i++) {
        New_node->selected[i] = selected[i]; // 새로운 노드의 selected 배열을 인수로 전달된 노드의 selected 배열과 동일하게 초기화
    }
    return New_node;
}

// 두 노드의 Bound 값을 비교
int Compare_nodes(node* a, node* b) {
    // 현재 사용 가능한 물건을 선택하고, 남은 배낭의 용량을 맞출 수 있는 가장 가치있는 물건으로 채우는 것으로 추정되는 최고 가격을 계산하여 a, b의 Upper Bound를 계산    
    double bound_a = ((double) a->value) + ((Max_weight - a->weight) * ((double) value[a->level])) / ((double) weight[a->level]);
    double bound_b = ((double) b->value) + ((Max_weight - b->weight) * ((double) value[b->level])) / ((double) weight[b->level]);
    if (bound_a > bound_b) { // 노드 a의 bound 값이 더 클 경우
        return -1;
    } else if (bound_a < bound_b) { // 노드 b의 bound 값이 더 클 경우
        return 1;
    } else { // 노드 a와 노드 b의 bound 값이 같을 경우
        return 0;
    }
}

// 우선순위 큐를 빈 대기열로 초기화
void Initialize_queue(priority_queue* q) {
    q->size = 0;
}

// 우선순위 큐에 새로운 노드 추가
void Insert_node(priority_queue* q, node* New_node) {
    int i = q->size;
    // 새로운 노드의 bound 값에 기초하여 큐에서 올바른 위치를 찾는 while문 
    while (i > 0 && Compare_nodes(q->data[i-1], New_node) > 0) { // 새로운 노드보다 높은 바운드 값을 가진 요소가 큐에 있는 한 루프가 실행되도록 함
        q->data[i] = q->data[i-1]; // 큐의 노드를 하나씩 아래로 이동하여 새로운 노드에 대한 공간을 생성
        i--; // 루프는 변수 i를 감소시켜 새로운 노드의 올바른 위치를 찾을 때까지 실행
    }
    q->data[i] = New_node; // 큐의 올바른 위치에 새로운 노드를 추가
    q->size++; // 우선순위 큐에 새로운 노드가 추가되었을 때, 큐의 크기 1씩 증가
}

// 우선순위 큐에서 가장 높은 bound 값을 가진 노드를 제거하고 반환
node* Remove_node(priority_queue* q) {
    node* result = q->data[0]; // 가장 높은 우선순위를 가지는 큐의 첫 번째 요소를 가리키는 result
    q->size--; // 큐의 크기 1씩 감소시킴 
    // 큐의 한 위치씩 이동하며 큐에서 첫 번째 요소를 제거하고 다른 요소의 위치를 업데이트
    for (int i = 0; i < q->size; i++) {
        q->data[i] = q->data[i+1]; // 다음으로 우선순위가 높은 요소를 가리킴
    }
    return result;
}

// Branch-And-Bound를 사용하여 최적의 방법 탐색
void knapsack() {
    // 우선순위 큐와 Root 노드 초기화
    priority_queue q; // 우선순위 큐 q를 생성
    Initialize_queue(&q); // q 초기화
    bool selected[25] = {0}; // selected 배열을 모두 false로 생성
    Insert_node(&q, Create_node(-1, 0, 0, selected)); // level은 -1, value는 0, weight는 0, selected는 모두 false로 새로운 노드 생성하고 q 큐에 노드 추가

    while (q.size > 0) {
        // 우선순위 큐에서 가장 높은 bound 값을 가진 노드를 선택
        node* Curr_node = Remove_node(&q);
      
        if (Curr_node->level == num-1) { // 노드를 확장할 수 있는지 검사
       
        if (Curr_node->value > Best_value) { // 최고의 방법이 필요할 경우 최고의 방법을 업데이트 하는 Leaf 노드
            Best_value = Curr_node->value;
        }
        } else { // 노드를 확장하고 우선순위 큐에 child들을 추가하는 Non-leaf 노드
        int next_level = Curr_node->level + 1;     
        bool not_selected[25]; // 다음 물건을 선택하지 않는 경우
        for (int i = 0; i <= Curr_node->level; i++) { 
            not_selected[i] = Curr_node->selected[i]; // 물건이 선택되지 않았을 경우 다음 단계의 새로운 노드를 생성하는 데 사용하기 위해
        }
        Insert_node(&q, Create_node(next_level, Curr_node->value, Curr_node->weight, not_selected)); // 다음 level의 새로운 노드를 만들어 q 큐에 추가

        bool selected[25]; // 다음 물건을 선택하는 경우
        for (int i = 0; i <= Curr_node->level; i++) {
            selected[i] = Curr_node->selected[i];
        }
        selected[next_level] = true; // 다음 level의 물건 선택
        int New_weight = Curr_node->weight + weight[next_level]; // 다음 level의 물건의 무게를 현재 노드의 무게에 더함
        int New_value = Curr_node->value + value[next_level]; // 다음 level의 물건의 가격을 현재 노드의 가격에 더함
        // 새로 계산된 무게가 배낭의 최대 무게 제한을 넘지 않고, 새로 계산된 가격이 현재 얻어진 최고 가격보다 클 경우
        if (New_weight <= Max_weight && ((double) New_value) > ((double) Best_value)) { 
            Insert_node(&q, Create_node(next_level, New_value, New_weight, selected)); // 다음 level의 새로운 노드를 만들어 q 큐에 추가 
        }
    }

    free(Curr_node); // 현재 노드의 메모리 해제
}
}

int main() {
   printf("물건의 수를 입력해주세요 :");
    scanf("%d", &num); // 물건의 수를 입력 받음

    printf("배낭이 담을 수 있는 최대 무게를 입력해주세요 :");
    scanf("%d", &Max_weight); // 배낭이 수용할 수 있는 최대 무게를 입력 받음

    printf("물건들의 가격을 입력해주세요 :");
    for (int i = 0; i < num; i++) {
        scanf("%d", &value[i]); // 물건들의 가격을 하나씩 입력 받아 배열에 저장
    }

    printf("물건들의 무게를 가격을 입력한 순서와 동일한 순서로 입력해주세요 :");
    for (int i = 0; i < num; i++) {
        scanf("%d", &weight[i]); // 가격을 입력 받은 순서와 동일한 순서로 물건들의 무게를 하나씩 입력 받아 배열에 저장
    }
// 얻을 수 있는 최고 가격 초기화
Best_value = 0;

// Branch-And-Bound를 이용하여 최적의 방법 탐색
knapsack();

// Knapsack 함수를 이용하여 배낭에 담을 수 있는 최고 가격 출력
printf("배낭의 최대 무게를 넘지 않고 담을 수 있는 최고 가격은 %d 입니다.\n", Best_value);

return 0;
}