#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Todo {
    int id;  
    char name[50];            
    char description[100];    
    char time[20];           
    int priority;
    int status; 
    struct Todo* next;      
} Todo;

typedef struct LinkedList {
    Todo* head;              
} LinkedList;

void updateStatus(LinkedList *list);
void initList(LinkedList* list);
void addTodo(LinkedList* list);
void displayTodos(LinkedList* list);
void deleteTodo(LinkedList* list, char* todoName);
void saveToFile(LinkedList* list);
void loadFromFile(LinkedList* list);
void searchTodoById(LinkedList* list);
void searchTodoByTimeOrDescription(LinkedList* list);
void displayMenu(); 
int generateRandomId(LinkedList* list);
int isValidTime(const char* time);
int isDuplicateName(LinkedList* list, const char* name);
void editTodo(LinkedList* list);
void sortTodosByPrior(LinkedList* list);

int main() {
    LinkedList list;
    initList(&list);
    loadFromFile(&list);

    int choice;
    do {
        displayMenu();
        scanf("%d", &choice);
        getchar();  
        switch (choice) {
            case 1:
                addTodo(&list);
                break;
            case 2:
                displayTodos(&list);
                break;
            case 3: {
                char todoName[50];
                printf("Nhap ten cong viec can xoa: ");
                fgets(todoName, sizeof(todoName), stdin);
                strtok(todoName, "\n");
                deleteTodo(&list, todoName);
                break;
            }
            case 4: 
                searchTodoById(&list);
                break;
            case 5:
                searchTodoByTimeOrDescription(&list);
                break;
            case 6:
                editTodo(&list);  
                break;
            case 8:
            	sortTodosByPrior(&list);
            	break;
            case 7:
            	updateStatus(&list);
            	break;
            case 0:
                saveToFile(&list);
                exit(0);  
                break;
            default:
                printf("Lua chon khong hop le.\n");
        }
    } while (1);

    return 0;
}

void initList(LinkedList* list) {
    list->head = NULL;
}

int generateRandomId(LinkedList* list) {
    int id;
    int isUnique;

    do {
        isUnique = 1;
        id = rand() % 100000000000; 

       
        Todo* current = list->head;
        while (current != NULL) {
            if (current->id == id) {
                isUnique = 0;
                break;
            }
            current = current->next;
        }
    } while (!isUnique);
    return id;
}

int isValidTime(const char* time) {
    int hours, minutes;
    return (sscanf(time, "%2d:%2d", &hours, &minutes) == 2 && hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59);
}

int isDuplicateName(LinkedList* list, const char* name) {
    Todo* current = list->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 1;  
        }
        current = current->next;
    }
    return 0;  
}

int addToPriority() {
    int priority;
    do {
        printf("Ban co muon them thu tu uu tien?:\n"); 
        printf("1. Co\n"); 
        printf("2. Khong\n"); 
        printf("Nhap lua chon: ");
        scanf("%d", &priority);
        getchar(); 
    } while (priority != 1 && priority != 2);


}  
void addTodo(LinkedList* list) {
    Todo newTodo;
    newTodo.id = generateRandomId(list);

    printf("Nhap ten cong viec: ");
    fgets(newTodo.name, sizeof(newTodo.name), stdin);
    strtok(newTodo.name, "\n");

    if (strlen(newTodo.name) == 0) {
        printf("Loi: Ten cong viec khong duoc de trong.\n");
        return;
    }

    if (isDuplicateName(list, newTodo.name)) {
        printf("Loi: Cong viec co ten '%s' da ton tai.\n", newTodo.name);
        return;
    }

    printf("Nhap mo ta cong viec: ");
    fgets(newTodo.description, sizeof(newTodo.description), stdin);
    strtok(newTodo.description, "\n");

    printf("Nhap thoi gian (HH:MM): ");
    fgets(newTodo.time, sizeof(newTodo.time), stdin);
    strtok(newTodo.time, "\n");

    if (!isValidTime(newTodo.time)) {
        printf("Loi: Thoi gian khong hop le. Dinh dang thoi gian phai la HH:MM (00:00 - 23:59).\n");
        return;
    }
	newTodo.status = 0;
    int addPriority;
    printf("Ban co muon them thu tu uu tien (1 : co / 0 : khong)? ");
    scanf("%d", &addPriority);
    getchar();  
	
	while(addPriority != 0 && addPriority != 1) {
        printf("Hay nhap dung lua chon\n");
        scanf("%d", &addPriority);
        getchar();
    }
	
    if (addPriority == 1) {
        printf("Nhap thu tu uu tien (1 la quan trong nhat, 2 la quan trong nhi, ...): ");
        scanf("%d", &newTodo.priority);
        getchar();  
    } else {
        newTodo.priority = 0; 
    }

 Todo* newNode = (Todo*)malloc(sizeof(Todo));
    *newNode = newTodo;
    newNode->next = list->head;
    list->head = newNode;
}

void displayMenu() {
    printf("\n *********************************************\n");
    printf(" *              MENU CHINH                   *\n");
    printf(" *********************************************\n");
    printf(" * 1. Them cong viec                         *\n");
    printf(" * 2. Hien thi lich lam viec                 *\n");
    printf(" * 3. Xoa cong viec                          *\n");
    printf(" * 4. Tim kiem cong viec theo ID             *\n");
    printf(" * 5. Tim kiem cong viec                     *\n");
    printf(" *     theo thoi gian hoac mo ta             *\n");
    printf(" * 6. Sua cong viec                          *\n");
    printf(" * 7. Sua trang thai                         *\n");
    printf(" * 8. Sap xep theo thu tu uu tien            *\n");
    printf(" * 0. Luu va thoat                           *\n");
    printf(" *********************************************\n");
    printf(" Chon mot tuy chon: ");
}
 
void displayTodos(LinkedList* list) {
    if (list->head == NULL) {   
        printf("Lich trinh ngay mai cua ban trong.\n");
        return;
    }

    printf("+------------+--------------------------------+----------------------------------------+------------+-------------------+-----------------+\n");
    printf("| ID         | Ten cong viec                  | Mo ta cong viec                        | Thoi gian  | Thu tu uu tien    | Trang thai      |\n");
    printf("+------------+--------------------------------+----------------------------------------+------------+-------------------+-----------------+\n");
    
    Todo* node = list->head;
    while (node != NULL) {
        char status[20];
        if (node->status == 1) {
            strcpy(status, "Da hoan thanh");
        }
        else {
            strcpy(status, "Chua hoan thanh");   
        }
        
        if(node->priority == 0){
            printf("| %-10d | %-30s | %-38s | %-10s |                   | %-15s |\n", 
                   node->id, node->name, node->description, node->time, status);
        }
        else{
            printf("| %-10d | %-30s | %-38s | %-10s | %-17d | %-15s |\n", 
                   node->id, node->name, node->description, node->time, node->priority, status);
        } 
        node = node->next;
    }

    printf("+------------+--------------------------------+----------------------------------------+------------+-------------------+-----------------+\n");
}

void updateStatus(LinkedList* list) {
    char keyword[200];
    printf("Nhap ten cong viec: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    Todo* current = list->head;
    int found = 0;
    int choice;

    printf("Do dai keyword: %lu\n", strlen(keyword));
    printf("Keyword: '");
    for(int i = 0; i < strlen(keyword); i++) {
        printf("%c", keyword[i]);
    }
    printf("'\n");

    while (current != NULL) {
       
        printf("Do dai current->name: %lu\n", strlen(current->name));
        printf("Current name: '");
        for(int i = 0; i < strlen(current->name); i++) {
            printf("%c", current->name[i]);
        }
        printf("'\n");

        if (strcmp(current->name, keyword) == 0) {
            printf("Tim thay cong viec: %s\n", current->name);

            char status[20];
            if (current->status == 1) {
                strcpy(status, "Da hoan thanh");
            } else {
                strcpy(status, "Chua hoan thanh");
            }
            printf("Trang thai hien tai: %s\n", status);

            printf("Ban co muon doi trang thai cong viec nay khong (1: Yes, 0: No)?\n");
            scanf("%d", &choice);
            getchar();  

            while (choice != 1 && choice != 0) {
                printf("Nhap gia tri hop le (1: Yes, 0: No).\n");
                scanf("%d", &choice);
                getchar();
            }

            if (choice == 1) {
                current->status = (current->status == 1) ? 0 : 1;
                printf("Trang thai da duoc cap nhat.\n");
            } else {
                printf("Khong thay doi trang thai.\n");
            }

            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("Khong tim thay cong viec nao phu hop.\n");
    }
}

void deleteTodo(LinkedList* list, char* todoName) {
    Todo* current = list->head;
    Todo* previous = NULL;

    while (current != NULL && strcmp(current->name, todoName) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) return; 

    if (previous == NULL) {
        list->head = current->next; 
    } else {
        previous->next = current->next; 
    }

    free(current);
}

void saveToFile(LinkedList* list) {
    FILE* file = fopen("data.txt", "w");
    if (file == NULL) {
        printf("Khong the mo file.\n");
        return;
    }
    
    Todo* node = list->head;
    while (node != NULL) {
        fprintf(file, "%d,%s,%s,%s,%d,%d\n", node->id, node->name, node->description, node->time, node->priority,node->status);
        node = node->next;
    }
    fclose(file);   
    printf("Luu file thanh cong!\n");
}

void loadFromFile(LinkedList* list) {
    FILE* file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("Khong the mo file.\n");
        return;
    }

    while (1) {
        Todo newTodo;
        if (fscanf(file, "%d,%49[^,],%99[^,],%19[^,],%d,%d\n", &newTodo.id, newTodo.name, newTodo.description, newTodo.time, &newTodo.priority,&newTodo.status) != 6) {
            break;
        }
        Todo* newNode = (Todo*)malloc(sizeof(Todo));
        *newNode = newTodo;
        newNode->next = list->head;
        list->head = newNode;
    }
    fclose(file);
}
 
void searchTodoById(LinkedList* list) {
    int id;
    printf("Nhap ID cong viec can tim: ");
    scanf("%d", &id);
    getchar(); 

    Todo* current = list->head;
    int found = 0;
    while (current != NULL) {
        if (current->id == id) {
            printf("Tim thay cong viec:\n");
            printf("ID: %d\n", current->id);
            printf("Ten cong viec: %s\n", current->name);
            printf("Mo ta: %s\n", current->description);
            printf("Thoi gian: %s\n", current->time);
            printf("Thu tu uu tien : %d\n",current->priority);
            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("Khong tim thay cong viec co ID '%d'.\n", id);
    }
}

void searchTodoByTimeOrDescription(LinkedList* list) {
    char keyword[200];
    printf("Nhap thoi gian hoac mo ta can tim: ");
    fgets(keyword, sizeof(keyword), stdin);
    strtok(keyword, "\n"); 
    Todo* current = list->head;
    int found = 0;
    while (current != NULL) {
        if (strstr(current->time, keyword) != NULL || strstr(current->description, keyword) != NULL) {
            printf("Tim thay cong viec:\n");
            printf("ID: %d\n", current->id);
            printf("Ten cong viec: %s\n", current->name);
            printf("Mo ta: %s\n", current->description);
            printf("Thoi gian: %s\n", current->time);
            printf("Thu tu uu tien : %d\n",current->priority);
            found = 1;
        }
        current = current->next;
    }
    if (!found) {
        printf("Khong tim thay cong viec nao phu hop.\n");
    }
}
void editTodo(LinkedList* list) {
    char name[50]; 
    printf("Nhap ten cong viec can sua: ");
    fgets(name, sizeof(name), stdin);   
    strtok(name, "\n");  

    Todo* current = list->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {   
            printf("Thong tin cong viec hien tai:\n");
            printf("ID: %d\n", current->id);
            printf("Ten cong viec: %s\n", current->name);
            printf("Mo ta: %s\n", current->description);
            printf("Thoi gian: %s\n", current->time);
            printf("Uu tien: %d\n", current->priority);

            int newPriority;
            do {
                printf("Nhap muc do uu tien moi (1-3, 1 la uu tien cao nhat): ");
                scanf("%d", &newPriority);
                getchar();   

                if (newPriority < 1) {
                    printf("Muc do uu tien khong hop le. Vui long nhap gia tri lon hon 1.\n");
                }
            } while (newPriority < 1 );

            current->priority = newPriority;
            printf("Da cap nhat muc do uu tien thanh cong!\n");
            return;
        }
        current = current->next;
    }

    printf("Khong tim thay cong viec co ten '%s'.\n", name);   
}
void sortTodosByPrior(LinkedList* list) {
    if (list->head == NULL || list->head->next == NULL) {
        return; 
    }

    Todo* current;
    Todo* nextNode;
    int swapped;

    do {
        swapped = 0;
        current = list->head;

        while (current != NULL && current->next != NULL) {
            nextNode = current->next;

            if ((current->priority == 0 && nextNode->priority != 0) || 
                (current->priority != 0 && nextNode->priority != 0 && current->priority > nextNode->priority)) {

                int tempId = current->id;
                char tempName[50], tempDescription[100], tempTime[20];
                int tempPriority;

                tempPriority = current->priority;
                tempId = current->id;
                strcpy(tempName, current->name);
                strcpy(tempDescription, current->description);
                strcpy(tempTime, current->time);

                current->priority = nextNode->priority;
                current->id = nextNode->id;
                strcpy(current->name, nextNode->name);
                strcpy(current->description, nextNode->description);
                strcpy(current->time, nextNode->time);

                nextNode->priority = tempPriority;
                nextNode->id = tempId;
                strcpy(nextNode->name, tempName);
                strcpy(nextNode->description, tempDescription);
                strcpy(nextNode->time, tempTime);

                swapped = 1; 
            }
            current = current->next;
        }
    } while (swapped);
}


