#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Property{
    char category[51];
    char city[51];
    char street[101];
    int area;
    int price;
    char description[201];
    struct Property *next;
} PROPERTY;

int headCheck(PROPERTY **first){
  if (*first == NULL){
      return 1;
  }
    return 0;
}

int createListFromFile(PROPERTY **first, int val){
    *first = malloc(sizeof(PROPERTY));
    int count = 0;
    char input;
    FILE *file;

    val = 0;

    if ((file = fopen("reality.txt", "r")) == NULL) {
        printf("Zaznamy neboli nacitane");
    }
    else{
        while ((input = fgetc(file))!= EOF) {
            if (input == '&' || input == '$') {
                val++;
            }
        }
        rewind(file);

        PROPERTY *current = NULL, *last = NULL;
        while ((fscanf(file, "%c ", &input)) != EOF) {
            count++;
            current = malloc(sizeof(PROPERTY));

            fgets(current->category, 51, file);
            fgets(current->city, 51, file);
            fgets(current->street, 101, file);
            fscanf(file,"%d %d ", &current->area, &current->price);
            fgets(current->description, 201, file);

            if (count != 1) {
                last->next = current;
                current->next = NULL;
                last = current;
            }
            else{
                *first = last = current;
            }
        }
        printf("Nacitalo sa %d zaznamov\n", val);
    }
    return val;
}

int printList(PROPERTY *first, int val) {
    headCheck(&first);

    PROPERTY *current = first;
    int i;
    for (i = 1; i <= val; i++) {
        printf("%d. \n", i);

        printf("kategoria ponuky: %s", current->category);
        printf("miesto ponuky: %s", current->city);
        printf("ulica: %s", current->street);
        printf("rozloha v m2: %d\n", current->area);
        printf("cena: %d\n", current->price);
        printf("popis: %s", current->description);

        if(current->next == NULL){
            break;
        }
        current = current->next;
    }
    return 0;
}

void addProperty(PROPERTY **first, int *val){
    int position, i = 1;

    scanf(" %d ", &position);

    if(*first == NULL) {
        *first = malloc(sizeof(PROPERTY));
        position = 1;
    }
    PROPERTY *current = *first;
    struct Property *temp = malloc(sizeof(PROPERTY));

    fgets(temp->category, 51, stdin);
    fgets(temp->city, 51, stdin);
    fgets(temp->street, 101, stdin);
    scanf("%d %d ", &temp->area, &temp->price);
    fgets(temp->description, 201, stdin);

    if(position == 1) {
        PROPERTY *newNode = malloc(sizeof(PROPERTY));
        newNode = temp;
        newNode->next = *first;
        *first = newNode;
    }
    else if(position > 1) {
        position = 0;
        for(i = 1; i < position; i++) {
            if(current->next == NULL){
                break;
            }
            current = current->next;
        }
        PROPERTY *newNode = malloc(sizeof(PROPERTY));
        newNode = temp;
        if(current->next){
            newNode->next = current->next;
        }
        else{
            newNode->next = NULL;
        }
        current->next = newNode;
    }
    (*val)++;
}

char *toLower(char *str) {
    int i;
    char *retStr = strdup(str);
    for (i = 0 ; str[i]; ++i){
       retStr[i] = tolower(str[i]);
    }
    return retStr;
}

int deleteProperty(PROPERTY **first, int *val){
    PROPERTY *current = *first;
    PROPERTY *prev = malloc(sizeof(PROPERTY));
    prev = NULL;
    int delCounter = 0, changePrev =0;
    char *delSubstr = calloc(53, sizeof(char));

    scanf(" %[^\n]", delSubstr);
    delSubstr = toLower(delSubstr);

    while (current) {
        changePrev = 1;
        if (strstr(toLower(current->city), delSubstr)) {
            if (prev && current->next) {
                  prev->next = current->next;
                          changePrev = 0;
                (*val)--;
                delCounter++;
            }
            else if (!prev && current->next) {
                while (strstr(toLower(current->city), delSubstr)) {
                    if (current->next) {
                        *first = (*first)->next;
                        current = *first;
                    }
                    else {
                        free(current);
                        *first = NULL;
                        (*val) = 0;
                        break;
                    }
                }
                (*val)--;
                delCounter++;
            }
            else if (!prev && !current->next) {
                free(current);
                *first = NULL;
                (*val) = 0;
                delCounter++;
            }
            else if (prev && !current->next) {
                free(current);
                current = prev->next = NULL;
                (*val)--;
                delCounter++;
                break;
            }
        }
        if (changePrev) {
            prev = current;
            current = current->next;
             }
            else {
                 current = current->next;
            }
    }
    printf("Vymazalo sa %d zaznamov\n", delCounter);
    return *val;
}

int addPropertyByCity(PROPERTY **first){
    headCheck(&first);

    char searchCity[51];
    int editCount = 0;

    fscanf(stdin, "%s ", searchCity);

    PROPERTY *new = NULL;
    new = malloc(sizeof(PROPERTY));

    fgets(new->category, 51, stdin);
    fgets(new->city, 51, stdin);
    fgets(new->street, 101, stdin);
    fscanf(stdin, "%d %d", &new->area, &new->price);
    getc(stdin);
    fgets(new->description, 201, stdin);

    PROPERTY *current = *first;
    char *searchCityPointer = toLower(searchCity);

    while(1){
    char *currentLocation = toLower(current->city);

      if (strstr(currentLocation, searchCityPointer)){

          strcpy(current->category, new->category);
          strcpy(current->city, new->city);
          strcpy(current->street, new->street);
          current->area = new->area;
          current->price = new->price;
          strcpy(current->description, new->description);

          editCount++;
      }
      if (current->next == NULL){
          break;
      }
    current = current->next;
    }
    printf("Aktualizovalo sa %d zaznamov\n", editCount);
    return 0;
}


int searchPropertyByPrice(PROPERTY **first){
    headCheck(&first);

    int searchPrice = 0;
    int counter = 0;

    scanf("%d", &searchPrice);

    PROPERTY *current = NULL;
    current = *first;

    while (current->next != NULL) {
        if (current->price <= searchPrice) {
            counter++;

            printf("%d.\n
            kategoria ponuky: %s
            miesto ponuky: %s
            ulica: %s
            rozloha v m2: %d\n
            cena: %d\n
            popis: %s",
            counter,
            current->category,
            current->city,
            current->street,
            current->area,
            current->price,
            current->description);

        }
        current = current->next;
    }
        if (current->price <= searchPrice) {
            counter++;

            printf("%d.\n
            kategoria ponuky: %
            smiesto ponuky: %s
            ulica: %s
            rozloha v m2: %d\n
            cena: %d\n
            popis: %s",
            counter,
            current->category,
            current->city,
            current->street,
            current->area,
            current->price,
            current->description);
            
        }
        if (counter == 0) {
            printf("V ponuke su len reality s vyssou cenou\n");
        }
    return 0;
}

int main()
{
    char input = 0;
    int val = 0;
    PROPERTY *first = NULL;

    while (input != 'k') {
        scanf("%c", &input);

        if (input == 'n') {
            val = createListFromFile(&first, val);
        }
        if (input == 'v') {
            printList(first, val);
        }
        if (input == 'p') {
            addProperty(&first, &val);
        }
        if (input == 'z') {
            deleteProperty(&first, &val);
        }
        if (input == 'h') {
            searchPropertyByPrice(&first);
        }
        if (input == 'a') {
            getc(stdin);
            addPropertyByCity(&first);
        }
    }
    return 0;
}
