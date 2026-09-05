#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  char name[4];
  unsigned int periodo;
  unsigned int deadline;
  unsigned int burst;
  unsigned int lifetime;
  unsigned int progress;
  unsigned int l;
  unsigned int f;
  unsigned int k;
  struct task *next;
} task;
FILE *fptr;
unsigned int extime = 0;
void org(task *head, task *top, unsigned int count){
  task *curr = head;
  while(curr != NULL){
    if(count % curr->periodo == 0){
      task *aux = malloc(sizeof(task));
      *aux = *curr;
      task *re = aux;
      re->next = top;
      unsigned int a;
      unsigned int b;
      while(re->next != NULL){
        if(flag){
          a = re->next->deadline;
          b = re->deadline;
        }else{
          a = re->next->periodo;
          b = re->periodo;
        }
        if(a >= b){
          break;
        }
        aux = re->next;
        re->next = aux->next;
        aux->next = re;
      }
      if(re->next == top){
        fprintf(fptr "[%s] for %u units - H\n", top->name, extime);
        top = re;
        extime = 0;
      }
    }
    curr = curr->next;
  }
  return;
}
//free todos os elementos atrasados a partir de current
task *flush(task *current){
  while(current != NULL && ++current->lifetime == curren->deadline){
    task *aux = current;
    current = current->next;
    free(aux);
  }
  return current;
}
void execute(task *head){
  unsigned int count = 0;
  task *top = NULL;
  unsigned int idle = 0;
  task *aux;
  while(count < 101){
    org(head, top, count);
    //atualiza o topo
    if(top == NULL){
      idle++;
    }else{
      extime++;
      if(idle > 0){
        fprintf(fptr, "idle for %u units\n", idle);
      }
      if(++top->progress == top->burst){
        aux = top;
        top = top->next;
        fprintf(fptr, "[%s] for %u units - F\n", aux->name, extime);
        free(aux);
        extime = 0;
      }
      if(++top->lifetime == top->deadline){
        aux = top;
        top = top->next;
        top = flush(top);
        fprintf(fptr, "[%s] for %u units - L\n", aux->name, extime);
        free(aux);
        extime = 0;
      }
      //atualiza o resto da fila
      aux = top;
      while(aux != NULL){
        aux = aux->next;
        aux = flush(aux);
      }
    }
    count++;
  }
  while(top != NULL){
    aux = top;
    top = top->next;
    free(aux);
  }
  return;
}
int main(int argc, char **argv) {
  if(argc < 3) {
    fprintf(stderr, "Erro: %s <rate|edf> <file>\n", argv[0]);
    return 1;
  }

  fptr = fopen(argv[2], "r");
  if(fptr == NULL) {
    perror("fopen");
    return 1;
  }
  if(strcmp(argv[1], "rate") != 0 && strcmp(argv[1], "edf") != 0){
    fprintf(stderr, "Erro: arg invalido '%s'", argv[1]);
    return 1;
  }
  task *head = NULL;
  task *tail = NULL;
  char string[200];
  while(fgets(string, sizeof(string), fptr) != NULL){ 
    task *new_task = malloc(sizeof(task));
    if(new_task == NULL){
      perror("malloc");
      fclose(fptr);
      return 1;
    }
    if(sscanf(string, "%3s %u %u %u", new_task->name, &new_task->periodo, &new_task->deadline, &new_task->burst) != 3){
      free(new_task);
      continue;
    }
    new_task->lifetime = 0;
    new_task->progress = 0;
    new_task->f = 0;
    new_task->l = 0;
    new_task->h = k;
    new_task->next = NULL;
    if(head == NULL){
      head = new_task;
      tail = new_task;
    }else{
      tail->next = new_task;
      tail = new_task;
    }
  }
  fclose(fptr);
  unsigned int count = 0;
  task *top = NULL;
  unsigned int idle = 0;
  task *aux;
  //rate
  if(argv[1] == "rate"){
    fptr = fopen("rate_vfmns.txt", "w");
    if(fptr == NULL){
      perror("fopen rate");
      return 1;
    }
    fprinf(fptr, "EXECUTION BY RATE\n\n");
    execute(head);
  }else if(argv[1] == "edf"){
    flag = 1;
    fptr = fopen("edf_vfmns.txt", "w");
    if(fptr == NULL){
      perror("fopen edf");
      return 1;
    }
    execute(head);
  }
  fclose(fptr);

  //free os inicializadores
  while(head != NULL){
      task *a = head->next;
      free(head);
      head = a;
  }
  return 0;
}
