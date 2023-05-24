#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define N_TERMINAIS 4
#define N_PROCESS 6
#define N_OPERACOES 6
#define READ 0
#define WRITE 1

void processa(int *saldos, char *opreracao);
void consultaSaldos(int *saldos);

int main(void) {
  int saldos[N_PROCESS];
  int saldo[N_TERMINAIS][N_OPERACOES][2];

  for (int i = 0; i < N_PROCESS; i++) {
    saldos[i] = 0;
  }

  // criação dos pipes
  for (int i = 0; i < N_TERMINAIS; i++)
    for (int j = 0; j < N_OPERACOES; j++) {
      if (pipe(saldo[i][j]) < 0) {
        perror("Erro no pipe");
        exit(EXIT_FAILURE);
      }
    }

  pid_t pids[4];
  for (int p = 0; p < N_TERMINAIS; p++) {
    pids[p] = fork(); // criação dos processos filho
    if (pids[p] == 0) {
      for (int i = 0; i < N_OPERACOES; i++) {
        close(saldo[p][i][READ]);
      }

      printf("Servidor le ATM %d\n", p + 1);

      FILE *arquivo;
      char linha[100]; // Defina o tamanho adequado para armazenar cada linha

      // Determina o ATM que vai ser lido
      char filename[10];
      sprintf(filename, "ATM%d.txt", p + 1);

      // Abrir o arquivo para leitura
      arquivo = fopen(filename, "r");
      if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1; // Terminar o programa com código de erro
      }

      int i = 0;
      // Ler cada linha do arquivo e exibi-la na tela
      while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        //  Escreve no pipe a linha
        if (write(saldo[p][i++][WRITE], linha, strlen(linha) + 1) == -1) {
          perror("Erro ao escrever no pipe");
          exit(EXIT_FAILURE);
        }
        sleep(2);
      }

      exit(EXIT_SUCCESS);
    }
  }

  for (int i = 0; i < N_TERMINAIS; i++)
    if (waitpid(pids[i], NULL, 0) == -1) {
      perror("Erro no waitpid do processo 1");
      exit(EXIT_FAILURE);
    }

  // Processo pai
  for (int i = 0; i < N_TERMINAIS; i++)
    for (int j = 0; j < N_OPERACOES; j++)
      close(saldo[i][j][WRITE]);

  char output[256];
  // le pipe
  for (int i = 0; i < N_TERMINAIS; i++) {
    for (int j = 0; j < N_OPERACOES; j++) {
      if (read(saldo[i][j][READ], output, sizeof(output)) == -1) {
        perror("Erro ao ler do pipe");
        exit(EXIT_FAILURE);
      }
      processa(saldos, output);
    }
  }
  consultaSaldos(saldos);
}

void consultaSaldos(int *saldos) {
  for (int i = 0; i < N_PROCESS; i++)
    printf("Saldo[%d] = %d\n", i + 1, saldos[i]);
}

void processa(int *saldos, char *linha) {
  char *nun;
  nun = strtok(linha, ",");

  int operacao[3];

  int i = 0;
  while (nun != NULL) {
    operacao[i++] = atoi(nun);
    nun = strtok(NULL, ",");
  }

  if (operacao[0] == 0) {
    saldos[operacao[1]] += operacao[2];
  } else {
    if (saldos[operacao[1]] < operacao[2])
      printf("Não foi possivel sacar a quantia: %d. Conta %d - Saldo = %d\n",
             operacao[2], operacao[1], saldos[operacao[1]]);
    else
      saldos[operacao[1]] -= operacao[2];
  }
}
