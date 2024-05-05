/*
Projeto 3 - Banco

Descrição:

Deve - se desenvolver uma aplicação onde ao ser inicializada, solicite ao usuário
escolher o que deseja fazer no banco, como: criar uma conta, efetuar saque, efetuar depósito,
efetuar transferência, listar contas ou sair do sistema
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/*
aqui será criado 2 structs, uma para armazenar os dados do cliente,
e a outra para armazenar os dados da conta bancária.
*/

typedef struct {
    int codigo;
    char nome[60];
    char email[50];
    char CPF[20];
    char dataNascimento[20];
    char dataCadastrada[20];
} Cliente;

typedef struct {
    int numero;
    Cliente cliente;
    float saldo;
    float limite;
    float saldoTotal; //saldo + limite
} Conta;

void menu();
void infoCliente(Cliente cliente);
void infoConta(Conta conta);
void criarConta();
void efetuarSaque();
void efetuarDeposito();
void efetuarTransferencia();
void listarContas();
float atualizaSaldoTotal(Conta conta);
Conta buscarContaPorNumero(int numero);
void sacar(Conta conta, float valor);
void depositar(Conta conta, float valor);
void transferir(Conta conta_origem, Conta conta_destino, float valor);

static Conta contas[60];
static int contador_contas = 0;
static int contador_cliente = 0;

int main () {
    menu();
    return 0;
}

void menu() {
    printf("\n\t ============================================================================================================\n");;
    printf("\n\t =========== Bem vindo cliente, é nosso prazer e compromisso tornar sua vida financeira mais fácil ! ========\n");

    printf("\n\t Por gentileza, selecione uma opção abaixo.\n");

    printf("\n\t 1 - Criar uma conta \n");
    printf("\n\t 2 - Efetuar saque \n");
    printf("\n\t 3 - Efetuar depósito \n");
    printf("\n\t 4 - Efetuar tranferência \n");
    printf("\n\t 5 - Listar contas existentes \n");
    printf("\n\t 6 - Sair da aplicação \n");

    int opcao;
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {
        case 1:
            criarConta();
            break;
        case 2:
            efetuarSaque();
            break;
        case 3:
            efetuarDeposito();
            break;
        case 4: 
            efetuarTransferencia();
            break;
        case 5:
            listarContas();
            break;
        case 6: 
            printf("\n\t Até a próxima !");
            sleep(2);
            exit(0);
        default:
            printf("\n\t Opçção inválida...");
            sleep(2);
            menu();
            break;
    }
}

void infoCliente(Cliente cliente) {
    printf("\n\t Cógigo: %d \nNome do cliente: %s \nData de nascimento: %s \nCadastro: %s",
            cliente.codigo, strtok(cliente.nome, "\n"), strtok(cliente.dataNascimento, "\n"),
            strtok(cliente.dataCadastrada, "\n"));
}

//OBS: para pegar um dado de uma struct, é o nome do dado da struct.a variável
// ex: cliente.codigo

void infoConta(Conta conta) {
    printf("\n\t N° da conta: %d\n\t Cliente: %s\n\t Data de nascimento: %s\n\t Data de cadastro: %s\n\t Saldo total: R$ %.2f\n",
            conta.numero, strtok(conta.cliente.nome, "\n"), strtok(conta.cliente.dataNascimento, "\n"), 
            strtok(conta.cliente.dataCadastrada, "\n"), conta.saldoTotal);
}

//OBS 2: acima vemos um exemplo de struct da struct

void criarConta() {
    Cliente cliente; // é necessário pegar o dado da struct cliente para se criar uma conta.
    
    //fazendo uma estrutura lógica para que o programa entenda a data de cadastro.
    char dia[3]; // 3 é devido o fato de toda string em C terminar com o \0 no final.
    char mes[3]; //08\0
    char ano[5]; //2024\0
    char data_cadastro[20];
    time_t t = time(NULL); //uso da biblioteca <time.h> incluída no cabeçalho do programa.
    struct tm tm = *localtime(&t); //fazendo uma nova struct pra pegar os dados posteriormente.
    
    
    //criando uma lógica para definir o dia:
    
    if (tm.tm_mday < 10) {
        sprintf(dia, "0%d", tm.tm_mday); //se o dia for menor que 10, vai colocar um 0 na frente do número ex: 03/05/2024.
    } else {
        sprintf(dia, "%d", tm.tm_mday);
    }
    
    //aplicando a mesma lógica para o mês:
    
    if (tm.tm_mon < 10) {
        sprintf(mes, "0%d", tm.tm_mon + 1); //se o mês for menor que 10, vai colocar um 0 na frente do número ex: 03/05/2024.
    } else {
        sprintf(mes, "%d", tm.tm_mon + 1); // + 1 é porque em programação, 0 é equivalente a janeiro, 11 é equivalente a dezembro.
    }
    
    //criando agora uma lógica para o ano:
    
    sprintf(ano, "%d", tm.tm_year + 1900); //1900 porque por algum motivo, o C não retorna o ano atual, então deve-se somar 1900.
    
    
    //criando a lógica de apresentação da data de cadastro para o programa.
    
    strcpy(data_cadastro, ""); //porque a data de cadastro começa vazia
    strcat(data_cadastro, dia); // até este ponto, tem-se apenas o dia concatenado. ex: 03
    strcat(data_cadastro, "/"); // até aqui temos o dia e uma barra concatenado ex: 03/
    strcat(data_cadastro, mes); //aqui, continua o de cima com o mês, ex: 03/05
    strcat(data_cadastro, "/"); // mais uma vez continuando o de cima, logo, temos por exemplo: 03/05/
    strcat(data_cadastro, ano); // finalizou com o ano, então teremos por exemplo: 03/05/2024
    strcat(data_cadastro, "\0"); //esse \0 é para informar para o programa que a string encerrou.
    strcpy(cliente.dataCadastrada, data_cadastro);
    
    //tudo isso que aqui foi feito, foi de informar ao programa a a data de cadastro do cliente usando a data do sistema operacional.
    
    //fazendo a lógica para se criar um cliente:
    
    printf("\n\t Informe os dados cadastrais abaixo.\n");
    cliente.codigo = contador_cliente + 1;
    
    printf("\n\t Nome: ");
    fgets(cliente.nome, 60, stdin);
    
    printf("\n\t E-mail: ");
    fgets(cliente.email, 50, stdin);
    
    printf("\n\t CPF: ");
    fgets(cliente.CPF, 20, stdin);
    
    printf("\n\t Data de nascimento: ");
    fgets(cliente.dataNascimento, 20, stdin);
    
    contador_cliente++;
    //o contador_cliente ++ é para attribuir 1 depois que o cliente for cadastrado, isso porque a princípio, não há cliente
    //porque o contador_cliente é inicializado em 0 lá em cima do código.
    
    //criando a lógica para criar a conta, uma vez que agora sim há cliente(s) para se criar uma conta:
    
    contas[contador_contas].numero = contador_contas + 1;
    contas[contador_contas].cliente = cliente;
    contas[contador_contas].saldo = 0.0;
    contas[contador_contas].limite = 0.0;
    contas[contador_contas].saldoTotal = atualizaSaldoTotal(contas[contador_contas]);
    
    printf("\n\t Conta criada com sucesso!\n");
    printf("\n");
    printf("\n\t Dados da conta criados. \n");
    printf("\n");
    infoConta(contas[contador_contas]);
    contador_contas++;
    
    sleep(4);
    menu();
    
}

float atualizaSaldoTotal(Conta conta) {
    return conta.saldo + conta.limite;
}

Conta buscarContaPorNumero(int numero) {
    Conta c;
    if (contador_contas > 0) {
        for (int i = 0; i < contador_contas; i++) {
            if (contas[i].numero == numero) {
                c = contas[i];
            }
        }
    }
    return c;
}

void sacar(Conta conta, float valor) {
    if (valor > 0 && conta.saldoTotal >= valor) { 
        for (int i = 0; i < contador_contas; i++) {
            if (contas[i].numero == conta.numero) {
                if (contas[i].saldo >= valor) {
                    contas[i].saldo = contas[i].saldo - valor;
                    contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
                    printf("\n\t Saque efetuado com sucesso.\n");
                } else {
                    float restante = contas[i].saldo - valor;
                    contas[i].limite = contas[i].limite - restante;
                    contas[i].saldo = 0.0;
                    contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
                    printf("\n\t Saque efetuado com sucesso.\n");
                }
            }
        }
    } else {
        printf("\n\t Saque não realizado. Tente novamente...\n");
    }
}

/*a função acima é para comparar se o número da conta que o usuário
digitou é igual ao vetor de contas posíveis, se sim, o programa
deve selecionar a conta que o usuário digitou.*/

/*
a função acima começa fazendo uma verificação, se o valor é diferente de 0,
se não for, é impossível fazer um saque, pois ningúem saca nada no banco, tem que
haver dinheiro na conta, E ao mesmo tempo que tem que ter dinheiro na conta,
esse dinheiro deve ser maior ou iguak ao saldo total, caso contrário, o usuário
cairá na condião else, ou seja, não vai realizar o saque.

supondo que a primeira verificação for verdadeira, o programa vai varrer o vetor
e fará outra verificação:

1) se o número da conta que o usuário digitou é a mesma que a conta foi cadastrada;
a partir desta verificação, o programa vai tirar um valor do saldo X do usuário, depois
vai atualizar o salto total, pois o saldo total por definição é a diferença entre o
saldo e o valor que o usuário quer sacar.

Depois o programa fará ainda outro teste, para zerar depois o saldo e atualizr o
saldo total 
*/

void depositar(Conta conta, float valor){
	if(valor > 0){
		for(int i = 0; i < contador_contas; i++){
			if(contas[i].numero == conta.numero){
				contas[i].saldo = contas[i].saldo + valor;
				contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
				printf("\n\t Depósito efetuado com sucesso!\n");
			}
		}
	}else{
		printf("\n\t Erro ao efetuar depósito. Tente novamente.\n");
	}
}


void transferir(Conta conta_origem, Conta conta_destino, float valor) {
    if (valor > 0 && conta_origem.saldoTotal >= valor)  {
        for (int contaOrigem = 0; contaOrigem < contador_contas; contaOrigem++) {
            if (contas[contaOrigem].numero == conta_origem.numero) {
                for (int contaDestino = 0; contaDestino < contador_contas; contaDestino++) {
                    if (contas[contaDestino].numero == conta_destino.numero) {
                        if (contas[contaOrigem].saldo >= valor) {
                            contas[contaOrigem].saldo = contas[contaOrigem].saldo - valor;
                            contas[contaDestino].saldo = contas[contaDestino].saldo + valor;
                            contas[contaOrigem].saldoTotal = atualizaSaldoTotal(contas[contaOrigem]);
                            contas[contaDestino].saldoTotal = atualizaSaldoTotal(contas[contaDestino]);
                            printf("\n\t Tranferência feita com êxito!\n");
                        } else {
                            float restante = contas[contaOrigem].saldo - valor;
                            contas[contaOrigem].limite = contas[contaOrigem].limite + restante;
                            contas[contaOrigem].saldo = 0.0;
                            contas[contaDestino].saldo = contas[contaDestino].saldo + valor;
                            contas[contaOrigem].saldoTotal = atualizaSaldoTotal(contas[contaOrigem]);
                            contas[contaDestino].saldoTotal = atualizaSaldoTotal(contas[contaDestino]);
                            printf("\n\t Tranferência feita com êxito");
                        }
                    }
                }
            }
        }
    } else {
        printf("\n\t Tranferência não realizada. Tente novamente.\n");
    }
}
/*
a função acima começça da mesma forma que a função sacar, isto é, fazendo
uma  verificação se o valor a ser transferido é maior que 0, uma vez que 
nunca será possível que o valor transferido seja negativo, ao mesmo tempo
este valor precisa ser maior ou igual a conta origem, pois se não for, não
será possível realizar a transferência, portanro, se o valor recebido pelo
programa não obedecer estes 2 critérios, irá cair na condição else.

se os critérios forem atendidos, o programa varrerá todo o vetor da conta
origem e se a conta origem obtiver o mesmo número da conta que está sendo 
passada, fará a mesma coisa para a conta destino, isto é, se o vetor da
conta destino for varrido e o número da conta destino for igual a que o
usuário irá digitar, acontecerá isto:

1) O saldo da conta origem vai perder o valor que está sendo transferido (debitar)
2) O saldo da conta destino vai receber o valor que foi transferido (creditar)
3) tanto a conta origem como a conta destino vão ter seus respectivos saldos 
atualizados.
*/

void efetuarSaque() {
    if (contador_contas > 0) {
        int numero;
        printf("\n\t Informe o número da conta: ");
        scanf("%d", &numero);
        
        Conta conta = buscarContaPorNumero(numero);
        
        if (conta.numero == numero) {
            float valor;
            printf("\n\t Informe o valor do saque: ");
            scanf("%f", &valor);
            
            sacar(conta, valor);
        } else {
            printf("\n\t não foi encontrada uma conta com o número %d\n", numero);
        }
        
    } else {
        printf("\n\t Ainda não existem contas para saque.\n");
    }
    sleep(2);
    menu();
}

void efetuarDeposito() {
    if (contador_contas > 0) {
        int numero;
        printf("\n\t Informe o número da conta: ");
        scanf("%d", &numero);
        
        Conta conta = buscarContaPorNumero(numero);
        
        if (conta.numero == numero) {
            float valor;
            printf("\n\t Informe o valor do depósito: ");
            scanf("%f", &valor);
            
            depositar(conta, valor);
        } else {
            printf("\n\t não foi encontrada uma conta com o número %d\n", numero);
        }
        
    } else {
        printf("\n\t Ainda não existem contas para depósito.\n");
    }
    sleep(2);
    menu();
}

void efetuarTransferencia() {
    if (contador_contas > 0) {
        int numero_origem, numero_destino;
        printf("\n\t Digite o número da sua conta: ");
        scanf("%d", &numero_origem);
        
        Conta conta_o = buscarContaPorNumero(numero_origem);
        
        if (conta_o.numero == numero_origem) {
            printf("\n\t Digite o número da conta destino: ");
            scanf("%d", &numero_destino);
            
            Conta conta_d = buscarContaPorNumero(numero_destino);
            
            if (conta_d.numero == numero_destino) {
                float valor;
                printf("\n\t Informe o valor para transferência: ");
                scanf("%f", &valor);
                
                transferir(conta_o, conta_d, valor);
            } else {
                printf("\n\t Não conseguimos localizar a conta destino com N° %d. Tente novamente", numero_destino);
            }
        } else {
            printf("\n\t Não conseguimos localizar a conta origem com N° %d. Tente novamente", numero_origem);
        }
    } else {
        printf("\n\t Ainda não existem contas para transferência.\n");
    }
    sleep(2);
    menu();
}

void listarContas() {
    if (contador_contas > 0) {
        printf("\n\t Listagem de contas.\n");
        printf("\n\t -----------------------------------------------------------------------------------------------\n");
        for (int i = 0; i < contador_contas; i++) {
            infoConta(contas[i]);
            printf("\n\t *******************************************************************************************\n");
            sleep(1);
        }
        sleep(2);
        menu();
    } else {
        printf("\n\t Não temos ainda contas cadastradas.\n");
        sleep(2);
        menu();
    }
}

