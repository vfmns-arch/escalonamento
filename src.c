#include <stdio.h>
#include <stdlib.h>

typedef struct{
  char name[4];
  unsigned int periodo;
  unsigned int deadline;
  unsigned int burst;
  unsigned int lifetime;
  unsigned int progress;
  unsigned int extime;
  unsigned int l;
  unsigned int f;
  unsigned int k;
  struct task *next;
} task;
int main(int argc, char **argv) {
  if(argc < 3) {
    fprintf(stderr, "Erro: %s <rate|edf> <file>\n", argv[0]);
    return 1;
  }

  FILE *fptr = fopen(argv[2], "r");
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
    new_task->extime = 0;
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
  //rate
  if(argv[1] == "rate"){
    fptr = fopen("rate_vfmns.txt", "w");
    if(fptr == NULL){
      perror("fopen rate");
      return 1;
    }
    fprinf(fptr, "EXECUTION BY RATE\n\n");
    int count = 0;
    task *top = NULL;
    task *curr;
    task *aux;
    while(count < 101){
      unsigned int idle = 0;
      curr = head;
      //organiza a fila e adiciona elementos
      while(curr != NULL){
        if(count % curr->periodo == 0){
          aux = malloc(sizeof(task));
          *aux = *curr;
          task *re = aux;
          re->next = top;
          while(re->next != NULL && re->next->periodo >= re->periodo){
            aux = re->next;
            re->next = aux->next;
            aux->next = re;
          }
          if(re->next == top){
            fprintf(fptr, "[%s] for %d units - H\n", top->name, top->extime);
            top = re;
          }
        }
        curr = curr->next;
      }
      //atualiza o topo
      if(top == NULL){
        idle++;
      }else{
        top->extime++;
        if(++top->progress == top->burst){
          aux = top;
          top = top->next;
          fprintf(fptr, "[%s] for %d units - F\n", aux->name, aux->extime);
          free(aux);
        }else if(++top->lifetime == top->deadline){
          aux = top;
          top = top->next;
          fprintf(fptr, "[%s] for %d units - L\n", aux->name, aux->extime);
          free(aux);
        }
        aux = top->next;
        task *last = top;
        task *auxb;
        //atualiza o resto da fila
        while(aux != NULL){
          aux->lifetime++;
          if(aux->lifetime == aux->deadline){
            last->next = aux->next;
            auxb = aux;
            aux = aux->next;
            free(auxb);
          }else{
            last = aux;
            aux = aux->next;
          }
        }
        if(idle > 0){
          fprintf("idle for %d units\n", idle);
          idle = 0;
        }
      }
    }
    //free o que sobrou da fila
    while(top != NULL){
      aux = top;
      top = top->next;
      free(aux);
    }

    
    fclose(fptr);
  }else if(argv[1] == "edf"){
    
  }

  //free os inicializadores
  task *current = head;
  while(current != NULL){
      task *next = current->next;
      free(current);
      current = next;
  }
  return 0;
}
