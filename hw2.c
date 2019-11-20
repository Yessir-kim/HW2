#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct __list {
    struct __node *head;
    struct __node *tail;
} list;

typedef struct __node {
    int type; // or -> 0 , and -> 1 , not -> 2, int -> 3
    int integer; // 만약 type이 3이라면 integet 값, 아니면 0
    int block; // block boundary 표현 (차례대로 1 2 3 4 5..)
    struct __node *next;
    struct __node *prev;
} node;

int empty(node *p);
node* newNode(list *L, int type, int integer, int block);
void getWord(char string[], char word[]);
int size(node *p);
void show_node(list* L, node *p);
node* Linked_Node(list *L, char string[], node *p, int* index, int block);
void deletenode(list* L, node* p, node* prev);
int blockcheck(node* p, int block);
int maxblock(list* L, node* p);
int Notnumber(node*p);
node* NNFchange_more(list* L, node* p, int block);
node* NNFchange_one(list* L, node* p, int block);
void NNFconverter(list* L, node* p, int size);
void NNFcheck(list* L, node* p);
void Rearrange(list *L, node* p);
int dis_change(list* L, node* p, node* first, int node[]);
void distribute(list* L, node* p, int node[], int* curr); // p는 첫번째 노드
int clause_check(int size, int node[]);
void print_DNF(int node[], int size);
int pro_value(int node[], int size, int index);
int node_index(int node[], int size, int num);
void satisfiable(int node[], int size);
void DNFform_check(int node[], int size, int org[], int org_size);

int main () {
  char text[10000];
  int index = 0;
  int block = 0;
  fgets(text, sizeof(text)/sizeof(char), stdin);
  //printf("%s",string);
  node *root = NULL;
  list *plist = (list *)malloc(sizeof(list)); // Linked list 생성
  node *head = (node *)malloc(sizeof(node));
  node *tail = (node *)malloc(sizeof(node));
	plist->head = head;
	plist->tail = tail;

  tail->prev = head;
  tail->next = NULL;
  head->next = tail;
  head->prev = NULL;   // Doubly linkendlist

  Linked_Node(plist, text, root, &index, block);
  //show_node(plist, plist->head->next); // Linkendlist succ

  int Notnum = Notnumber(plist->head->next);
  //printf("%d",Notnum);
  if(Notnum >= 1) NNFcheck(plist, plist->head);

  Rearrange(plist,plist->head->next); // NULL node delete
  //printf("\n");
//  show_node(plist, plist->head->next); // NNF form succ

  //printf("\n");

  node* check = plist->head->next;
  int flist[size(plist->head->next)*3];
  int list_size = 0;
  for(int i = 0; check != plist->tail; i++, check = check->next)
  {
    if(check->type == 1)
      flist[i] = 'a';
    else if(check->type == 0)
      flist[i] = 'o';
    else
      flist[i] = check->integer;
    if(check->block > check->next->block || check->next == plist->tail)
    {
      //printf("%d %d\n",check->block,check->next->block);
      if(check->next->type == 3)
      {
       //printf("%d %d\n",check->block,check->next->block);
        flist[i+1] = 0;
        i++;
        list_size += 2;
      }
      else if(check->next->type == 1 || check->next->type == 0)
      {
        //printf("%d %d\n",check->block,check->next->block);
        flist[i+1] = 0;
        i++;
        list_size += 2;
      }
      else if(check->next == plist->tail)
      {
        //printf("%d %d\n",check->block,check->next->block);
        flist[i+1] = 0;
        i++;
        list_size += 2;
      }
      else
        list_size++;
    }
    else if(check->block == check->next->block && check->type == 3)
    {

      if(check->next->type == 1 || check->next->type == 0)
      {
        //printf("%d %d\n",check->block,check->next->block);
        flist[i+1] = 0;
        i++;
        list_size += 2;
      }
      else
        list_size++;
    }
    else
      list_size++;
  }
  int curr = 0;
  int* org = (int*)malloc(sizeof(int)*list_size);
  for(int i = 0; i < list_size; i++)
    org[i] = flist[i];
  int org_size = list_size;
  distribute(plist, plist->head->next, flist, &curr);

  if (curr == 0) curr = list_size;
  
  printf("\n");
  for(int j = 0; j < curr; j++)
    printf("%d ", flist[j]);
  
  // DNF형식이 맞으면 출력해야 함
  // 아니라면 그냥 종료하는걸로 하자..
  // DNF는 conjunction으로 묶인 절이 한개 이거나 전체가 disjunction으로 묶인 절이 여러개인 경우임
  //printf("\n");
   DNFform_check(flist, curr, org, org_size);
  //int temp = clause_check(curr, flist);
  //printf("%d", temp);
  return 0;
}

void distribute(list* L, node* p, int node[], int* curr) // p는 첫번째 노드
{
  if(L->tail == p) return; // base case
  if(maxblock(L, L->head->next) == p->block) return; //printf("max return");
   // 가장 위에 있는 clause
  if(p->type == 1) // and
  {
    if(blockcheck(p,p->block) == 0)
    {
      distribute(L,p->next,node, curr); // pass
    }
    else
    {
      distribute(L,p->next,node, curr);
      *curr = dis_change(L, p, L->head->next, node);
      return;
    }
  }
  else if(p->type == 0)
  {
    if(blockcheck(p,p->block) == 0)
    {
      distribute(L,p->next,node, curr); // pass
    }
    else
    {
      if(L->head->next == p) // pass
      {
        distribute(L,p->next,node, curr); // pass
      }
      else
      {
        distribute(L,p->next,node, curr);
        *curr = dis_change(L, p, L->head->next, node);
        return;
      }
    }
  }
  else
  {
    distribute(L, p->next,node, curr); // pass
  }
}

int dis_change(list* L, node* p, node* first, int node[])
{
  int index = 0;
  int t_list[size(L->head->next)*3];
  //printf("in1");
  while(first != p)
  {
    t_list[index] = node[index];
    //printf("%d\n",t_list[index]);
    index++; // 현재위치의 인덱스를 얻음 (연산자 위치)
    first = first->next;
  }
  //printf("in2");
  int fix_index = index;
  int arch_clause[10]; // 3의 부분이 연산자의 개수에 따라 달라짐
  int single_clause[10];
  int j = 0, k = 0;
  //printf("in3");
  while(abs(p->block - first->block) <= 1 && first->next != L->tail) // 나누는 부분 (0을 기준으로 나눌 것임)
  {
    int* clause = (int*)malloc(sizeof(size(L->head->next)));
    if(node[index] == 97 || node[index] == 111 || node[index] == 0)
    {
      int i = 0;
      index += 1;
      printf("\n");
      while(node[index] != 0)
      {
        clause[i] = node[index++]; // 절의 개수를 기반으로 만든 배열에 저장하는 부분
        //printf("%d ", clause[i]);
        printf("\n");
        first = first->next;
        i++;
      }
      if(i > 1)
      {
        j = i; // 배열 사이즈
        for(int q = 0; q < i; q++)
        {
          arch_clause[q] = clause[q];
          //printf("%d", clause[q]);
        }

      }
      else
      {
        single_clause[k] = clause[i-1];
        k++; // 배열 사이즈
      }
      free(clause);
    }
    else index++;
  }
  /*
  for(int h = 0; h < k; h++)
    printf("%d ",single_clause[h]);
  for(int h = 0; h < j; h++)
    printf("%d ",arch_clause[h]);
    */
  int change_type;
  if(p->type == 0) change_type = 111;
  else change_type = 97;

  t_list[fix_index] = change_type;
  if(k != 0)
  {
    for(int q = 1; q < j; q++)
    {
      t_list[++fix_index] = arch_clause[0];
      t_list[++fix_index] = arch_clause[q];
      for(int w = 0; w < k; w++)
        t_list[++fix_index] = single_clause[w];
      t_list[++fix_index] = 0;
    }
    for(int e = 0; e < fix_index+1; e++)
      node[e] = t_list[e];
    return fix_index+1;
  }
  return 0;
}

int empty(node *p) {
	return p == NULL;
}

node* newNode(list *L, int type, int integer, int block) {
	node *newNode = (node *)malloc(sizeof(node));
	newNode->type = type;
  newNode->integer = integer;
  newNode->block = block;
  //하나도 없을 때
  if(L->head->next == L->tail)
  {
      L->head->next = L->tail->prev = newNode;
      newNode->next = L->tail;
      newNode->prev = L->head;
  }
  else
  {
    L->tail->prev = L->tail->prev->next = newNode;
    newNode->next = L->tail;
    newNode->prev = L->tail->prev;
  } // 있다면은

	return newNode;
}

void getWord(char string[], char word[])
{
  for (int i = 0; i < strlen(string); i++){
    if (string[i] == ' ' && i != 0)
      return;
    word[i] = string[i];
  }
}

int size(node *p) {
	if (empty(p)) return 0;
	int count = 0;
	for (node *c = p; c != NULL; c = c->next, count++);
	return count;
}

void show_node(list* L, node *p)
{
  node *temp;
  //int n = size(p);
  for(temp = p; temp != L->tail; temp = temp->next)
    printf("%d %d %d\n",temp->type, temp->integer, temp->block);
}

// recursion으로 구현할 것
// string을 받아와서 노드에 계속 연결할 것
// 1. '('를 만날경우 block을 +1해줌

node* Linked_Node(list *L, char string[], node *p, int* index, int block)
{
  char word[100];
  int check;
  if (*index >= strlen(string))
  {
    if(block != 0) // pair가 맞지 ㅇ낳는 경우
    {
      printf("Invalid form!");
      exit(-1);
    }
    else return NULL;
  }

  getWord(string+*index, word);
  if(string[*index + strlen(word)] == ' ')
    *index += strlen(word) + 1;
  else
    *index += strlen(word);

  if(word[0] == '(')
  {
    block += 1;
    if(word[1] == 'o' && word[2] == 'r') // or case
    {
       p = newNode(L,0,0,block);
       Linked_Node(L, string, p, index, block);
    }
    else if(word[1] == 'a' && word[2] == 'n' && word[3] == 'd') // and case
    {
       p = newNode(L,1,0,block);
       Linked_Node(L, string, p, index, block);
    }
    else if(word[1] == 'n' && word[2] == 'o' && word[3] == 't') // not case
    {
       p = newNode(L,2,0,block);
       Linked_Node(L, string, p, index, block);
    }
    else
    {
        printf("Invalid form!"); // ( 뒤에 and, or not이 들어오지 않는 경우
        exit(-1);
    }
  }
  else if(word[0] >= 'a' && word[0] <= 'z') // a와 z 사이라면 integer가 들어오는 부분
  {
    int number = 0;
    int size = strlen(word);
    int i;
    int block_temp = 0;
    int in_check = 0;
    for(i = 1; i < size; i++)
    {
      if (word[i] >= '0' && word[i] <= '9')
      {
        number = (number * 10) + word[i] - '0';
        in_check++;
      }
      if(word[i] == ')') block_temp += 1;
    }
    if(in_check == 0)
    {
      // proposition variable 뒤에 숫자가 오지 않는 경우
      printf("Invalid form!");
      exit(-1);
    }
    p = newNode(L,3,number,block);
    Linked_Node(L, string, p, index, block - block_temp);
  }
  return p;
}

void deletenode(list* L, node* p, node* prev)
{
  node *temp = (node *)malloc(sizeof(node));
  temp = p;

  if (L->head == prev)
  {
    L->head = temp->next;
    temp->next->prev = NULL;
  }
  else
  {
    prev->next = temp->next;
    temp->next->prev = prev;
  }
  free(temp);
}

int blockcheck(node* p, int block)
{
  int count = 0;
  while(p->block == block)
  {
    count++;
    p = p->next;
  }
  return count;
}

int maxblock(list* L, node* p)
{
  int max = p->block;
  while(p != L->tail)
  {
    if(max < p->block) max = p->block;
    p = p->next;
  }
  //printf("%d\n",max);
  return max;
}

int Notnumber(node*p)
{
  if (empty(p)) return 0;
	int count = 0;
	for (node *c = p; c != NULL; c = c->next)
  {
    if(c->type == 2)
      count++;
  }
	return count;
}

node* NNFchange_more(list* L, node* p, int block)
{
  node* copy = p->next;
  int num;
  //if (L->head == p->prev)
  //  deletenode(L,p,NULL);
  //else
  deletenode(L, p, p->prev);
  node* org_p = copy;
  while(copy->block > block)
  {
    if(copy == L->tail) break;
    num = blockcheck(copy, copy->block);
    if(num > 2)
    {
      while(num != 0 && copy != L->tail)
      {
        if(copy->type == 3)
          copy->integer = copy->integer * -1;
        else if(copy->type == 1)
          copy->type = 0;
        else if(copy->type == 0)
          copy->type = 1;
        copy->block -= 1;
        copy = copy->next;
        num--;
      }
    }
    if(copy->block == block)
      copy->block -= 1;
    copy = copy->next;
  }

  return org_p;
}

node* NNFchange_one(list* L, node* p, int block)
{
  node* copy = p->next;
  int num;
  //if (L->head == p->prev)
  //  deletenode(L,p,NULL);
  //else
  deletenode(L, p, p->prev);
  node* org_p = copy;
  while(copy->block > block)
  {
    if(copy == L->tail) break;
    num = blockcheck(copy, copy->block);
    if(num > 2)
    {
      while(num != 0 && copy != L->tail)
      {
        if(copy->type == 3)
          copy->integer = copy->integer * -1;
        else if(copy->type == 1)
          copy->type = 0;
        else if(copy->type == 0)
          copy->type = 1;
        copy = copy->next;
        num--;
      }
    }
    copy = copy->next;
  }

  return org_p;
}

void NNFconverter(list* L, node* p, int size)
{
  // 처음에 함수가 호출된 시점에 node는 not이 시작되는 시점임
  if(L->tail == p) // base case
    return;
  //printf("%d\n",p->type);
  if(p->type == 2)
  {
    if(Notnumber(L->head->next) > 1)
      p = NNFchange_more(L, p, p->block);
    else
      p = NNFchange_one(L, p, p->block);
    NNFconverter(L, p, p->block);
  }
  else if(p->type == 0) // or case
  {
    p->type = 1;
    p->block -= 1;
    NNFconverter(L, p->next, size);
  }
  else if(p->type == 1) // and case
  {
    p->type = 0;
    p->block -= 1;
    NNFconverter(L, p->next, size);
  }
  else
  {
    p->integer = p->integer * -1;
    p->block -= 1;
    NNFconverter(L, p->next, size);
  }
}

void NNFcheck(list* L, node* p)
{
  node* temp = p;

  if(temp == NULL) return;

  if(temp->type == 2)
  {
    NNFconverter(L, temp, temp->block);
    return;
  }
  else
  {
    NNFcheck(L, temp->next);
  }
}

void Rearrange(list *L, node* p) // p == L->head->next
{
  node *new = (node *)malloc(sizeof(node));
  new = L->head;
  L->head = new;
  while(p != L->tail)
  {
    if(p->type == 1 || p->type == 2 || p->type == 3 || p->type == 0)
    {
      new->next = p;
      p->prev = new;
      new = p;
    }
    p = p->next;
  }
  new->next = p;
  p->prev = new;
  L->tail = p;
}

int clause_check(int size, int node[])
{
  int clause_num = 0;
  for(int i = 0; i <= size; i++)
  {
    //printf("%d ",i+1);
    if(node[i] == 111 || node[i] == 97) // and 나 or 이면
      if(node[i+1] != 97 && node[i+1] != 111 && node[i+1] != 0) // achiv chaek
      {
          clause_num++;
          //printf("Type 1");
      }
    if(i != 0)
    {
      if(node[i-1] == 0 && node[i+1] == 0) // single check
      {
        clause_num++;
        //printf("Type 2");
      }
    }
    //printf("\n");
  }
  return clause_num;
}

void print_DNF(int node[], int size)
{
  for(int i = 0; i < size; i++)
  {
    if(node[i] != 97 && node[i] != 111 && node[i] != 0)
    {
      printf("%d ",node[i]);
      if(node[i+1] == 97 || node[i+1] == 111)
        printf("\n");
      else if(node[i+1] == 0)
        printf("\n");
    }
  }
}

int pro_value(int node[], int size, int index)
{
  //printf("%d",size);
  for(int i = index; i < size; i++)
  {
      for(int j = i + 1; j < size; j++)
      {
        //printf("in");
        if(node[j] != 97 && node[j] != 111 && node[j] != 0)
          if(node[i] == -1 * node[j]) return 0;
        break;
      }
  }
  return 1;
}

int node_index(int node[], int size, int num)
{
  int clause_num = 0;
  for(int i = 0; i <= size; i++)
  {
    //printf("%d ",i+1);
    if(node[i] == 111 || node[i] == 97) // and 나 or 이면
      if(node[i+1] != 97 && node[i+1] != 111 && node[i+1] != 0) // achiv chaek
      {
          clause_num++;
          //printf("Type 1");
      }
    if(i != 0)
    {
      if(node[i-1] == 0 && node[i+1] == 0) // single check
      {
        clause_num++;
        //printf("Type 2");
      }
    }
    //printf("\n");
    if(clause_num == num) return i;
  }
}

void satisfiable(int node[], int size)
{
  printf("%d\n",0);
  for(int i = 0; i < size; i++)
  {
    if(node[i] != 111 && node[i] != 97 && node[i] != 0)
      printf("%d ",node[i]);
  }
}

void DNFform_check(int node[], int size, int org[], int org_size)
{
  int max_clause = clause_check(size, node);
  //printf("%d", max_clause);
  if(max_clause == 1)
  {
    if(node[0] == 111)
    {
      print_DNF(node, size); // set
      satisfiable(org, org_size);
    }
    else if(node[0] == 97)
    {
      if(pro_value(node, size, 0))
      {
        print_DNF(node, size); // set
        satisfiable(org, org_size);
      }
      else //unset
      {
        print_DNF(node, size);
        printf("%d\n",0);
        printf("UNSAT\n");
      }
    }
  }
  else if(max_clause > 1)
  {
    if(node[0] == 97)
    {
      printf("Not DNF form!");
      exit(-1);
    }
    else if(node[0] == 111)
    {
      int next_index = node_index(node, size, 2);
      if(node[next_index] == 97)
      {
        while(max_clause != 2)
        {
          if(!pro_value(node, size, max_clause))
          {
            print_DNF(node, size);
            printf("%d\n",0);
            printf("UNSAT\n");
            break;
          }
          else
          {
            print_DNF(node, size);
            satisfiable(org, org_size); //set
          }
          max_clause--;
        }
      }
      else
      {
        print_DNF(node, size);
        satisfiable(org, org_size); //set
      }
    }
  }
}
