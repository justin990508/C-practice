#include <stdio.h>
#include <stdlib.h>

    int num; // 물건들의 수를 나타내는 변수
    int value[25]; // 물건들의 가격을 나타내는 배열
    int weight[25]; // 물건들의 무게를 나타내는 배열
    int Max_weight; // 배낭이 담을 수 있는 최대 무게를 나타내는 변수
    int Best_value; // 현재 얻을 수 있는 최고 가격
    int Curr_value; // 배낭에 들어있는 현재 물건들의 가격
    int Curr_weight; // 배낭에 들어있는 현재 무게

// 최적의 방법을 위해 재귀적으로 검색
void knapsack(int i) {
    if (i == num) {
        if (Curr_value > Best_value) { // 현재 가격이 최고 가격보다 높을 경우
            Best_value = Curr_value; // 최고 가격을 현재 가격으로 변경
        }
        return;
    }
    // 물건 i를 제외하고 시도해보기
    knapsack(i + 1);

    // 배낭이 담을 수 있는 최대 무게를 넘지 않을 경우 물건 i를 포함하기
    if (Curr_weight + weight[i] <= Max_weight) { // 물건 i의 무게를 더했을 때 최대 무게 제한을 넘지 않을 경우 
        Curr_weight += weight[i]; // 현재 무게에 물건 i의 무게를 더함
        Curr_value += value[i]; // 현재 가격에 물건 i의 가격을 더함
        knapsack(i + 1); // 물건 i를 제외할 때
        Curr_weight -= weight[i]; // 현재 무게에서 물건 i의 무게를 뺌
        Curr_value -= value[i]; // 현재 가격에서 물건 i의 가격을 뺌
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

    // 현재의 상태와 최고 가격을 초기화
    Curr_weight = 0;
    Curr_value = 0;
    Best_value = 0;

    // Backtraing을 이용하여 최적의 방법 탐색
    knapsack(0);

    // Knapsack 함수를 이용하여 배낭에 담을 수 있는 최고 가격 출력
    printf("배낭의 최대 무게를 넘지 않고 담을 수 있는 최고 가격은 %d 입니다.\n", Best_value);

    return 0;
}
