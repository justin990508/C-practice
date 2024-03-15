#include <stdio.h>
#include <stdlib.h>

// 최대값을 찾기 위한 max 함수 구현
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Knapsack 함수 구현
int Knapsack(int Max_weight, int weight[], int value[], int num) {
    int K[num+1][Max_weight+1]; // 입력 받은 배낭의 최대 무게와 물건들로 얻을 수 있는 최고 가격을 저장
    
    for (int i = 0; i <= num; i++) {
        for (int j = 0; j <= Max_weight; j++) {
            if (i == 0 || j == 0) // 물건이 0개이거나 배낭의 최대 무게가 0일 경우
                K[i][j] = 0; // 0으로 초기화
            else if (weight[i-1] <= j) // 물건의 무게가 배낭의 최대 무게보다 작을 경우
                K[i][j] = max(value[i-1] + K[i-1][j-weight[i-1]], K[i-1][j]); // 현재 물건을 배낭에 포함하는 것이 더 높은 가격을 얻을 수 있는지 계산
            else // 물건의 무게가 배낭의 최대 무게를 넘는 경우
                K[i][j] = K[i-1][j]; // 최고 가격은 이전 최대 값 사용
        }
    }
    return K[num][Max_weight]; // 결과 반환
}

int main() {
    int num; // 물건들의 수를 나타내는 변수
    int value[25]; // 물건들의 가격을 나타내는 배열
    int weight[25]; // 물건들의 무게를 나타내는 배열
    int Max_weight; // 배낭이 담을 수 있는 최대 무게를 나타내는 변수
 

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

    // Knapsack 함수를 이용하여 배낭에 담을 수 있는 최고 가격 출력
    printf("배낭의 최대 무게를 넘지 않고 담을 수 있는 최고 가격은 %d 입니다.\n", Knapsack(Max_weight, weight, value, num));
    
    return 0;
}
