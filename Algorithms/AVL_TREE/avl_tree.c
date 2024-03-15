#include <stdio.h>
#include <stdlib.h>

// AVL 트리 노드 구조체
typedef struct AVLNode {
    int key; // 노드의 키 값
    struct AVLNode* left; // 노드의 왼쪽 자식을 가리키는 포인터
    struct AVLNode* right; // 노드의 오른쪽 자식을 가리키는 포인터
    int height; // 노드의 높이
} AVLNode;

// 노드의 높이(깊이) 정보를 얻기 위한 함수
int getHeight(AVLNode* node) {
    if (node == NULL) // 노드가 존재하지 않는 경우
        return 0;
    return node->height; 
}

// 두 정수 중 큰 값을 얻기 위한 함수
int max(int a, int b) {
    return (a > b) ? a : b;
}

// 새로운 AVL 노드를 생성하기 위한 함수
AVLNode* createNode(int key) {
    AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode)); // 새로운 노드에게 메모리 할당
    newNode->key = key; // 노드의 키 값 설정
    newNode->left = NULL; // 왼쪽 자식 없음
    newNode->right = NULL; // 오른쪽 자식 없음
    newNode->height = 1; // 높이 1로 설정
    return newNode;
}

// 오른쪽으로 회전하도록 (right rotation) 해주는 함수
AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left; // y의 왼쪽 자식으로 x 설정
    AVLNode* T2 = x->right; // x의 오른쪽 자식으로 T2 설정

    x->right = y; // x가 가리키는 오른쪽은 y
    y->left = T2; // y가 가리키는 왼쪽은 T2

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1; // y의 높이는 y의 왼쪽 높이와 오른쪽 높이 중 큰 값 + 1
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1; // x의 높이는 x의 왼쪽 높이와 오른쪽 높이 중 큰 값 + 1

    return x; // 서브트리의 새로운 root 노드인 x 반환
}

// 왼쪽으로 회전하도록 (left rotation) 해주는 함수
AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right; // x의 오른쪽 자식으로 y 설정
    AVLNode* T2 = y->left; // y의 오른쪽 자식으로 T2 설정

    y->left = x; // y가 가리키는 왼쪽은 x
    x->right = T2; // x가 가리키는 오른쪽은 T2

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1; // x의 높이는 x의 왼쪽 높이와 오른쪽 높이 중 큰 값 + 1
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1; // y의 높이는 y의 왼쪽 높이와 오른쪽 높이 중 큰 값 + 1

    return y; // 서브트리의 새로운 root 노드인 y 반환
}

// 노드의 균형 요소를 얻기 위한 함수
int getBalanceFactor(AVLNode* node) {
    if (node == NULL) // 노드가 존재하지 않는 경우
        return 0;
    return getHeight(node->left) - getHeight(node->right); // 노드의 왼쪽 높이 - 오른쪽 높이
}

// AVL 트리에 노드를 삽입하기 위한 함수
AVLNode* insertNode(AVLNode* root, int key) {
    // Perform the normal BST insertion
    if (root == NULL) // root가 비어있는 경우
        return createNode(key); 

    if (key < root->key) // root가 가리키는 키 값이 삽입하는 노드의 키 값보다 큰 경우
        root->left = insertNode(root->left, key); // root의 왼쪽에 노드 삽입
    else if (key > root->key) // root가 가리키는 키 갑이 삽입하는 노드의 키 값보다 작은 경우
        root->right = insertNode(root->right, key); // root의 오른쪽에 노드 삽입
    else // 키 값이 동일할 경우
        return root;

    // 부모 이상의 노드의 높이를 갱신
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1; // root의 높이는 root의 왼쪽 높이와 오른쪽 높이 중 큰 값 + 1

    // 균형 요소를 확인하고 필요 시 다시 균형을 맞추기
    int balanceFactor = getBalanceFactor(root); // 균형 요소 확인

    // Left의 Left인 경우
    if (balanceFactor > 1 && key < root->left->key) // 왼쪽의 서브트리가 불균형하고 삽입된 키 값이 root의 왼쪽 서브트리 키 값보다 작을 경우
        return rotateRight(root); // 오른쪽으로 회전

    // Right의 Right인 경우
    if (balanceFactor < -1 && key > root->right->key) // 오른쪽의 서브트리가 불균형하고 삽입된 키 값이 root의 오른쪽 서브트리 키 값보다 큰 경우
        return rotateLeft(root); // 왼쪽으로 회전

    // Left의 Right인 경우
    if (balanceFactor > 1 && key > root->left->key) { // 왼쪽의 서브트리가 불균형하고 삽입된 키 값이 root의 왼쪽 서브트리 키 값보다 큰 경우
        root->left = rotateLeft(root->left); // root의 왼쪽 서브트리를 왼쪽으로 회전시켜 균형을 맞춤
        return rotateRight(root); // 전체 트리를 오른쪽으로 회전시켜 균형을 맞춤
    }

    // Right의 Left인 경우
    if (balanceFactor < -1 && key < root->right->key) { // 오른쪽의 서브트리가 불균형하고 삽입된 키 값이 root의 오른쪽 서브트리 키 값보다 작은 경우
        root->right = rotateRight(root->right); // root의 오른쪽 서브트리를 오른쪽으로 회전시켜 균형을 맞춤
        return rotateLeft(root); // 전체 트리를 왼쪽으로 회전시켜 균형을 맞춤
    }

    return root;
}

// AVL 트리에서 제일 작은 값을 가진 노드를 찾기 위한 함수
AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL) // 현재 노드의 왼쪽 자식이 없을 때까지 반복
        current = current->left; // 왼쪽으로 한 칸씩 이동하면서 가리킴
    return current; // 제일 작은 현재 노드의 값 반환
}

// AVL 트리에서 노드를 삭제하기 위한 함수
AVLNode* deleteNode(AVLNode* root, int key) {
    // 일반적인 이진 탐색 트리 삭제 수행
    if (root == NULL) // root가 비어있는 경우
        return root;

    if (key < root->key) // 입력받은 키의 값이 root가 가리키는 키 값보다 작은 경우 
        root->left = deleteNode(root->left, key); // root의 왼쪽 서브트리에서 노드 삭제
    else if (key > root->key) // 입력받은 키의 값이 root가 가리키는 키 값보다 큰 경우
        root->right = deleteNode(root->right, key); // root의 오른쪽 서브트리에서 노드 삭제
    else {
        // 노드를 찾았으니 삭제 수행하기
        if (root->left == NULL || root->right == NULL) { // 자식이 한 개이거나 자식이 없는 노드
            AVLNode* temp = root->left ? root->left : root->right;

            // 자식이 없는 경우
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else // 자식이 한 개인 경우
                *root = *temp; // 비어있지 않은 자식의 내용들을 복사

            free(temp); // temp의 메모리 해제
        } else {
            // 자식이 두 개인 노드라면 중위(inorder) 후임(successor)을 얻음 
            AVLNode* temp = minValueNode(root->right); // 현재 노드의 오른쪽 서브트리에서 가장 작은 키 값을 가진 노드

            // 중위 후임의 데이터를 현재 노드에 복사
            root->key = temp->key;

            // 중위 후임 삭제
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL) // 트리가 노드 하나만 가지고 있는 경우
        return root;

    // 현재 노드의 높이 갱신하기
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1; // 왼쪽의 높이와 오른쪽의 높이 중 큰 값 + 1

    // 균형 요소를 확인하고 필요 시 다시 균형을 맞추기
    int balanceFactor = getBalanceFactor(root); // 균형 요소 확인

    // Left의 Left인 경우
    if (balanceFactor > 1 && getBalanceFactor(root->left) >= 0) // 왼쪽의 서브트리가 불균형이고 왼쪽 서브트리의 왼쪽 서브트리가 불균형인 경우
        return rotateRight(root); // 오른쪽으로 회전

    // Left의 Right인 경우
    if (balanceFactor > 1 && getBalanceFactor(root->left) < 0) { // 왼쪽의 서브트리가 불균형이고 왼쪽 서브트리의 오른쪽 서브트리가 불균형인 경우
        root->left = rotateLeft(root->left); // 왼쪽 서브트리를 왼쪽으로 회전
        return rotateRight(root); // 전체 트리를 오른쪽으로 회전시켜 균형을 맞춤
    }

    // Right의 Right인 경우
    if (balanceFactor < -1 && getBalanceFactor(root->right) <= 0) // 오른쪽의 서브트리가 불균형이고 오른쪽 서브트리의 오른쪽 서브트리가 불균형인 경우
        return rotateLeft(root); // 왼쪽으로 회전

    // Right의 Left인 경우
    if (balanceFactor < -1 && getBalanceFactor(root->right) > 0) { // 오른쪽의 서브트리가 불균형이고 오른쪽 서브트리의 왼쪽 서브트리가 불균형인 경우
        root->right = rotateRight(root->right); // 오른쪽 서브트리를 오른쪽으로 회전
        return rotateLeft(root); // 전체트리를 왼쪽으로 회전시켜 균형을 맞춤
    }

    return root;
}

// AVL 트리에서 키를 찾기 위한 함수
AVLNode* search(AVLNode* root, int key) {
    if (root == NULL || root->key == key) // root가 비어있고 root가 가리키는 key의 값이 찾는 값인 경우
        return root; 

    if (key < root->key) // root가 가리키는 key의 값이 찾는 값보다 큰 경우
        return search(root->left, key);
    else // root가 가리키는 key의 값이 찾는 값보다 작은 경우
        return search(root->right, key);
}

// AVL 트리의 중위 순회를 수행하기 위한 함수
void inorderTraversal(AVLNode* root) {
    if (root != NULL) { // root가 존재할 때(=서브트리가 있을 때)
        inorderTraversal(root->left); // 현재 노드의 왼쪽 서브트리를 중위 순회 
        printf("%d ", root->key);
        inorderTraversal(root->right); // 현재 노드의 오른쪽 서브트리를 중위 순회
    }
}

int main() {
    AVLNode* root = NULL;
    int node[6]; // 6개의 노드를 입력받아 저장하기 위한 배열

    printf("6개의 노드를 추가해 주세요 : ");
    for (int i=0; i<6; i++) {
        scanf("%d", &node[i]); // 노드를 입력받아 배열에 차례대로 저장
    }
    // 노드 삽입하기
    root = insertNode(root, node[0]);
    root = insertNode(root, node[1]);
    root = insertNode(root, node[2]);
    root = insertNode(root, node[3]);
    root = insertNode(root, node[4]);
    root = insertNode(root, node[5]);

    printf("AVL Tree의 중위 순회 : ");
    inorderTraversal(root);
    printf("\n");

    // 노드 검색하기
    int key;
    printf("검색할 노드의 키 값을 입력해 주세요 : "); // 검색할 노드의 키 값 입력받기
    scanf("%d", &key);

    AVLNode* searchResult = search(root, key); // 입력받은 키 값을 가진 노드 검색
    if (searchResult != NULL) // 입력받은 키 값을 가진 노드가 존재할 경우
        printf("키 값이 %d인 노드를 AVL 트리에서 찾았습니다.\n", key);
    else // 입력받은 키 값을 가진 노드가 존재하지 않는 경우
        printf("키 값이 %d인 노드를 AVL 트리에서 찾지 못했습니다.\n", key);

    // 노드 삭제하기
    int deleteKey;
    printf("삭제할 노드의 키 값을 입력해 주세요 : "); // 삭제할 노드의 키 값 입력받기
    scanf("%d", &deleteKey);

    searchResult = search(root, deleteKey); // 삭제할 노드의 키 값 검색하기
    if (searchResult != NULL) { // 입력받은 키 값을 가진 노드가 존재할 경우
        root = deleteNode(root, deleteKey);
        printf("키 값이 %d인 노드를 AVL 트리에서 삭제한 후의 중위 순회입니다 : ", deleteKey);
    }else { // 입력받은 키 값을 가진 노드가 존재하지 않는 경우
        printf("키 값이 %d인 노드를 AVL 트리에서 찾지 못했습니다.\n", deleteKey);
        printf("현재 AVL 트리의 중위 순회 입니다 : ");
    }
    inorderTraversal(root);
    printf("\n");

    return 0;
}
