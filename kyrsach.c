#include <stdio.h>
#include <locale.h>
#include <malloc.h>
#include <windows.h>
#define _HUGE_ENUF  1e+300
#define INFINITY ((float)(_HUGE_ENUF * _HUGE_ENUF))
#define MAXLINE 1024
#define name_sort_type 1
#define category_sort_type 2
#define forever for(;;)

struct Product
{
 char name[MAXLINE];
 char category[MAXLINE];
 float price;
};

struct Store
{
 struct Product* products;
 int was_changed;
};


void store_initialize(const char* name_of_file, struct Store* S, int* size);
void menu(struct Store* S, int* size);
void add_product_to_store(struct Store* S, int* size);
void quickSort(struct Product* data, int const len, char sort_type);
int* find_product(char* name, struct Store* S, int size);
int choose_product(char* name, struct Store* S, int size);
void edit_product_in_store(struct Store* S, int* size);
void delete_product_from_store(struct Store* S, int* size);
void database_content_output(struct Store* S, int* size);
void Selection_of_the_minimum_basket(struct Store* S, int* size);
void save(const char* name_of_file, struct Store* S, int* size);

int main()
{
 setlocale(LC_ALL, "RUS");
 SetConsoleCP(1251);
 SetConsoleOutputCP(1251);
 struct Store S;
 int size;
 store_initialize("base.txt", &S, &size);
 menu(&S, &size);
 save("base.txt", &S, &size);
 if (S.products) free(S.products);
 return 0;
}

void store_initialize(const char* name_of_file, struct Store* S, int* size)
{
 S->products = NULL;
 S->was_changed = 0;
 FILE* fpin;
 fopen_s(&fpin, name_of_file, "rt");// открыть файл для чтения
 if (fpin == NULL)
  return; // ошибка при открытии файла

 fscanf_s(fpin, "%d", size);
 S->products = (struct Product*)malloc((*size) * sizeof(struct Product));
 if (!S->products)
  *size = 0;
 for (int i = 0; i < *size; i++)
 {
  fgets(S->products[i].name, MAXLINE, fpin);
  fgets(S->products[i].name, MAXLINE, fpin);
  S->products[i].name[strlen(S->products[i].name) - 1] = '\0';
  fgets(S->products[i].category, MAXLINE, fpin);
  S->products[i].category[strlen(S->products[i].category) - 1] = '\0';
  fscanf_s(fpin, "%f", &S->products[i].price);
 }
 fclose(fpin); // закрыть входной файл
}

void menu(struct Store* S, int* size)
{
 int m = -1;
 forever
 {
  printf("Меню\n 1. Пополнение базы\n 2. Редактирование базы\n 3. Удаление записей\n");
  printf(" 4. Вывод содержимого базы\n 5. Подбор минимальной корзины\n 0. Выход\n-> ");
  scanf_s("%d", &m);
  switch (m)
  {
  case 1: add_product_to_store(S, size);    break;
  case 2: edit_product_in_store(S, size);    break;
  case 3: delete_product_from_store(S, size);   break;
  case 4: database_content_output(S, size);   break;
  case 5: Selection_of_the_minimum_basket(S, size); break;
  case 0: return;
  default: printf("выбран не существующий пункт меню");
  }
 }
}

void add_product_to_store(struct Store* S, int* size)
{
 S->products = (struct Product*)realloc(S->products, ((*size) + 1) * sizeof(struct Product));
 printf("\tВведите название товара: ");
 gets(S->products[*size].name, MAXLINE);
 gets(S->products[*size].name, MAXLINE);
 printf("\tВведите категорию товара: ");
 gets(S->products[*size].category, MAXLINE);
 printf("\tВведите цену товара: ");
 scanf_s("%f", &S->products[*size].price);
 (*size)++;
 S->was_changed = 1;
}

void quickSort(struct Product* data, int const len, char sort_type)
{
 int const lenD = len;
 struct Product pivot;
 int ind = lenD / 2;
 int i, j = 0, k = 0;
 if (lenD > 1)
 {
  struct Product* L = (struct Product*)malloc(lenD * sizeof(struct Product));
  struct Product* R = (struct Product*)malloc(lenD * sizeof(struct Product));
  pivot = data[ind];
  for (i = 0; i < lenD; i++)
   if (i != ind)
   {
    if (sort_type == 1 ? strcmp(data[i].name, pivot.name) < 0 : strcmp(data[i].category, pivot.category) < 0)
    {
     L[j] = data[i];
     j++;
    }
    else
    {
     R[k] = data[i];
     k++;
    }
   }
  quickSort(L, j, sort_type);
  quickSort(R, k, sort_type);
  for (int cnt = 0; cnt < lenD; cnt++)
  {
   if (cnt < j)
    data[cnt] = L[cnt];
   else if (cnt == j)
    data[cnt] = pivot;
   else
    data[cnt] = R[cnt - (j + 1)];
  }
  free(L);
  free(R);
 }
}

int* find_product(char* name, struct Store* S, int size)
{
 int i_size = 5, i_i = 0;
 int* indexes = (int*)malloc(i_size * sizeof(int));
 for (int k = 0; k < i_size; k++)
  indexes[k] = -1;
 quickSort(S->products, size, name_sort_type);
 for (int i = 0; i < size; i++)
 {
  if (!strcmp(S->products[i].name, name))
  {
   if (i_size == i_i)
   {
    indexes = (int*)realloc(indexes, (i_size += 5) * sizeof(int));
    for (int k = 0; k < 5; k++)
     indexes[i_size + k] = -1;
   }
   indexes[i_i] = i;
   i_i++;
  }
 }
 return indexes;
}

int choose_product(char* name, struct Store* S, int size)
{
 int* indexes = find_product(name, S, size);
 int index;
 if (indexes[0] == -1)
 {
  printf("\t\tНебыло найдено товаров по вашему запросу.\n");
  free(indexes);
  return -1;
 }
 printf("\t\tБыли найдены следующие товары:\n");
 for (int i = 0; indexes[i] != -1; i++)
 {
  printf("\t\t %d. %s - %s : $%f\n",
   i + 1,
   S->products[indexes[i]].name,
   S->products[indexes[i]].category,
   S->products[indexes[i]].price);
 }

 printf("\n\t\tВыберите номер товара:\n\t\t-> ");
 scanf_s("%d", &index);
 index = indexes[index - 1];
 free(indexes);
 return index;
}

void edit_product_in_store(struct Store* S, int* size)
{
 char name[MAXLINE];
 printf("\tВведите название товара: ");
 gets(name, MAXLINE);
 gets(name, MAXLINE);

 int index = choose_product(name, S, *size);
 if (index == -1) return;
 printf("\tВведите название товара: ");
 gets(S->products[index].name, MAXLINE);
 gets(S->products[index].name, MAXLINE);
 printf("\tВведите категорию товара: ");
 gets(S->products[index].category, MAXLINE);
 printf("\tВведите цену товара: ");
 scanf_s("%f", &S->products[index].price);
 S->was_changed = 1;
}

void delete_product_from_store(struct Store* S, int* size)
{
 char name[MAXLINE];
 printf("\tВведите название товара: ");
 gets_s(name, MAXLINE);
 gets_s(name, MAXLINE);
 int index = choose_product(name, S, *size);
 int i_i = 0;
 struct Product* P = (struct Product*)malloc(((*size) - 1) * sizeof(struct Product));

 for (int i = 0; i < *size; i++)
 {
  if (i == index) continue;
  P[i_i] = S->products[i];
  i_i++;
 }
 (*size)--;
 free(S->products);
 S->products = P;
}

void database_content_output(struct Store* S, int* size)
{
 int m = -1;
 while (m < 1 || m>2)
 {
  printf("\tВыберите вывод содержимого базы\n\t 1. по названию товара\n\t 2. по категориям товара\n\t-> ");
  scanf_s("%d", &m);
  switch (m)
  {
  case 1: quickSort(S->products, *size, name_sort_type);  break;
  case 2: quickSort(S->products, *size, category_sort_type); break;
  default: printf("\tвыбран не существующий пункт меню");  break;
  }
 }
 for (int i = 0; i < *size; i++)
 {
  printf("\t\t %d. %s - %s : $%f\n",
   i + 1,
   S->products[i].name,
   S->products[i].category,
   S->products[i].price);
 }
}

void Selection_of_the_minimum_basket(struct Store* S, int* size)
{
 char name[MAXLINE];
 gets(name, MAXLINE);
 float min_price;
 int min_i;
 int* indexes = NULL;
 int basket_size = 1, basket_i = 0;
 struct Product* basket = (struct Product*)malloc(basket_size * sizeof(struct Product));

 printf("\tДля выхода введите '0'\n");
 forever
 {
  printf("\tВведите название товара: ");
  gets(name, MAXLINE);
  if (!strcmp(name, "0")) break;
  indexes = find_product(name, S, *size);
  if (indexes[0] == -1)
   printf("\t\tНебыло найдено товаров по вашему запросу.\n");
  else
  {
   min_price = INFINITY;
   min_i = 0;
   for (int i = 0; indexes[i] != -1; i++)
   {
    if (min_price > S->products[indexes[i]].price)
    {
     min_price = S->products[indexes[i]].price;
     min_i = indexes[i];
    }
   }
   if (basket_size == basket_i)
   {
    basket = (struct Product*)realloc(basket, ++basket_size * sizeof(struct Product));
   }
   strcpy_s(basket[basket_i].name, MAXLINE, S->products[min_i].name);
   strcpy_s(basket[basket_i].category, MAXLINE, S->products[min_i].category);
   basket[basket_i].price = S->products[min_i].price;
   basket_i++;
  }
 }
 quickSort(basket, basket_size, name_sort_type);
 printf("\n\t\tКорзина:\n");
 for (size_t i = 0; i < basket_size; i++)
 {
  printf("\t\t %d. %s - %s : $%f\n",
   i + 1,
   basket[i].name,
   basket[i].category,
   basket[i].price);
 }
}

void save(const char* name_of_file, struct Store* S, int* size)
{
 if (S->was_changed)
 {
  FILE* fpin;
  fopen_s(&fpin, name_of_file, "wt");// открыть файл для чтения
  if (fpin == NULL)
   return; // ошибка при открытии файла
  fprintf(fpin, "%d\n", *size);
  for (int i = 0; i < *size; i++)
   fprintf(fpin, "%s\n%s\n%f\n", S->products[i].name, S->products[i].category, S->products[i].price);
  fclose(fpin);
 }
 S->was_changed = 0;
}
