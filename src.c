#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct result{
  char name[4];
  unsigned int l;
  unsigned int f;
  unsigned int k;
};
struct task{
  unsigned int periodo;
  unsigned int deadline;
  unsigned int burst;
  unsigned int lifetime;
  unsigned int progress;
  struct task *next;
  struct result *id;
};
typedef struct task task;
typedef struct result result;
FILE *fptr;
unsigned int extime = 0;
int flag = 0;
unsigned int t;
task *org(task *head, task *top, unsigned int count){
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
        if(top != NULL){
          fprintf(fptr, "[%s] for %u units - H\n", top->id->name, extime);
        }
        top = re;
        extime = 0;
      }
    }
    curr = curr->next;
  }
  return top;
}
//free todos os elementos atrasados a partir de current
task *flush(task *current){
  while(current != NULL && ++current->lifetime == current->deadline){
    task *aux = current;
    current = current->next;
    aux->id->l++;
    free(aux);
  }
  return current;
}
void execute(task *head){
  unsigned int count = 0;
  task *top = NULL;
  unsigned int idle = 0;
  task *aux;
  while(count <= t){
    top = org(head, top, count);
    //atualiza o topo
    if(top == NULL){
      idle++;
    }else{
      extime++;
      if(idle > 0){
        fprintf(fptr, "idle for %u units\n", idle);
        idle = 0;
      }
      if(++top->progress == top->burst){
        top->id->f++;
        aux = top;
        top = top->next;
        fprintf(fptr, "[%s] for %u units - F\n", aux->id->name, extime);
        free(aux);
        extime = 0;
      }
      if(top != NULL && ++top->lifetime == top->deadline){
        aux = top;
        top = top->next;
        top = flush(top);
        aux->id->l++;
        fprintf(fptr, "[%s] for %u units - L\n", aux->id->name, extime);
        free(aux);
        extime = 0;
      }
      //atualiza o resto da fila
      aux = top;
      while(aux != NULL){
        task *last = aux;
        aux = aux->next;
        aux = flush(aux);
        last->next = aux;
      }
    }
    count++;
  }
  //free o resto da fila
  while(top != NULL){
    top->id->k++;
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
  //read o tempo
  if(fgets(string, sizeof(string), fptr) != NULL){
    if(sscanf(string, "%u", &t) != 1){
      printf("tempo invalido");
      return 1;
    }
  }
  while(fgets(string, sizeof(string), fptr) != NULL){ 
    result *new_id = malloc(sizeof(result));
    task *new_task = malloc(sizeof(task));
    if(new_task == NULL || new_id == NULL){
      perror("malloc");
      fclose(fptr);
      return 1;
    }
    if(sscanf(string, "%3s %u %u %u", new_id->name, &new_task->periodo, &new_task->deadline, &new_task->burst) != 4){
      free(new_task);
      continue;
    }
    new_task->id = new_id;
    new_task->lifetime = 0;
    new_task->progress = 0;
    new_id->f = 0;
    new_id->l = 0;
    new_id->k = 0;
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
  //rate
  if(strcmp(argv[1], "rate") == 0){
    fptr = fopen("rate_vfmns.txt", "w");
    if(fptr == NULL){
      perror("fopen rate");
      return 1;
    }
    fprintf(fptr, "EXECUTION BY RATE\n\n\n");
    execute(head);
  }else if(strcmp(argv[1], "edf") == 0){
    flag = 1;
    fptr = fopen("edf_vfmns.txt", "w");
    if(fptr == NULL){
      perror("fopen edf");
      return 1;
    }
    fprintf(fptr, "EXECUTION BY EDF\n\n\n");
    execute(head);
  }
  //escreve os resultados e free os inicializadores
  fprintf(fptr, "\n\nLOST DEADLINES\n");
  task *a = head;
  while(a != NULL){
    fprintf(fptr, "[%s] %u\n", a->id->name, a->id->l);
    a = a->next;
  }
  a = head;
  fprintf(fptr, "\n\nCOMPLETE EXECUTION\n");
  while(a != NULL){
    fprintf(fptr, "[%s] %u\n", a->id->name, a->id->f);
    a = a->next;
  }
  fprintf(fptr, "\n\nKILLED\n");
  while(head != NULL){
    fprintf(fptr, "[%s] %u\n", head->id->name, head->id->k);
    a = head->next;
    free(head->id);
    free(head);
    head = a;
  }
  fclose(fptr);
  return 0;
}
