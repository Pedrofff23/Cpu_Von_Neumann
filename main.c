#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "instrucao.h"
/**
 * @brief
Esse simulador deve mostrar o conteúdo
dos registradores no fim de cada ciclo de máquina, quando haverá uma pausa até apertar uma tecla para iniciar o próximo
ciclo
 */
unsigned int mbr,
        mar,
        imm,
        pc = 0,
        reg[8];
unsigned char ir,
        ro0,//reg[ro0]
        ro1,
        e,
        l,
        g;
unsigned char memoria[154];

void busca(){
    mar = pc;
    mbr = memoria[mar++];
    for (int i=1; i < 4; i++) {
        mbr = (mbr << 8 ) | memoria[mar++];
    }
}
void decodifica(){
    ir = mbr >> 24;
    // decodificaçao de add ate xor
    if(ir >= add && ir<= xor){
        ro0 = (mbr & lgcA) >> 21;
        ro1 = (mbr & lgcB) >> 18;
    }
    // decodificaçao de not
    if(ir == not){
        ro0 = (mbr & lgcA) >> 21;
    }
    // decodificaçao de je ate jmp
    if(ir>=je && ir<=jmp){
        mar= ( mbr & maskmar);
    }
    //decodificaçao de ld e st
    if(ir >= ld && ir <= st ){
        ro0 = (mbr & lgcA) >> 21;
        mar= ( mbr & maskmar);
    }
    //decodificaçao de movi ate rsh
    if(ir>= movi && ir<=rsh){
        ro0 = (mbr & lgcA) >> 21;
        imm = ( mbr & maskmar);
    }
}
void executa() {
    if (ir == hlt){
    }
    if (ir == nop) {
        pc = pc + 4;
    }
    if (ir == add) {
        reg[ro0] = reg[ro0] + reg[ro1];
        pc += 4;
    }
    if (ir == sub) {
        reg[ro0] = reg[ro0] - reg[ro1];
        pc += 4;
    }
    if (ir == mul) {
        reg[ro0] = reg[ro0] * reg[ro1];
        pc += 4;
    }
    if (ir == div) {
        reg[ro0] = reg[ro0] / reg[ro1];
        pc += 4;
    }
    if (ir == cmp) {
        if (reg[ro0] == reg[ro1]) {
            e = 1;
        } else {
            e = 0;
        }
        if (reg[ro0] < reg[ro1]) {
            l = 1;
        } else {
            l = 0;
        }
        if (reg[ro0] > reg[ro1]) {
            g = 1;
        } else {
            g = 0;
        }
        pc += 4;
    }
    if (ir == movr) {
        reg[ro0] = reg[ro1];
        pc += 4;
    }
    if (ir == and) {
        reg[ro0] = reg[ro0] & reg[ro1];
        pc += 4;
    }
    if (ir == or) {
        reg[ro0] = reg[ro0] | reg[ro1];
        pc += 4;
    }
    if (ir == xor) {
        reg[ro0] = reg[ro0] ^ reg[ro1];
        pc += 4;
    }
    if (ir == not) {
        reg[ro0] = !reg[ro0];
        pc += 4;
    }
    if (ir == je) {
        if (e == 1) {
            pc = mar;
        }else{
            pc += 4;
        }
    }
    if (ir == jne) {
        if (e == 0) {
            pc = mar;
        }else{
            pc += 4;
        }
    }
    if (ir == jl) {
        if (l == 1) {
            pc = mar;
        }else{
            pc += 4;
        }
    }
    if (ir == jle) {
        if (e == 1 | l == 1) {
            pc = mar;
        }else{
            pc += 4;
        }
    }
    if (ir == jg) {
        if (g == 1) {
            pc = mar;
        }else{
            pc += 4;
        }
    }
    if (ir == jge) {
        if(e == 1 | g == 1){
            pc = mar;
        }else {
            pc += 4;
        }
    }
    if (ir == jmp) {
        pc = mar;
    }
    if (ir == ld) {
        mbr = memoria[mar++];
        for (int i = 1; i < 4 ; i++) {
            mbr = (mbr << 8 ) | memoria[mar++];
        }
        reg[ro0] = mbr;
        pc += 4;
    }
    if (ir == st) {
        mbr =  reg[ro0];
        memoria[mar++] = mbr >> 24;
        memoria[mar++] = (mbr & 0x00ff0000) >> 16;
        memoria[mar++] = (mbr & 0x0000ff00) >> 16;
        memoria[mar] = mbr & 0x000000ff;
        pc += 4;
    }
    if (ir == movi){
        reg[ro0] = imm;
        pc += 4;
    }
    if (ir == addi){
        reg[ro0] = reg[ro0] + imm;
        pc += 4;
    }
    if (ir == subi){
        reg[ro0] = reg[ro0] - imm;
        pc += 4;
    }
    if (ir == muli){
        reg[ro0] = reg[ro0] * imm;
        pc += 4;
    }
    if (ir == divi){
        reg[ro0] = reg[ro0] / imm;
        pc += 4;
    }
    if (ir == lsh){
        reg[ro0]= (reg[ro0] << imm);
        pc += 4;
    }
    if (ir == rsh){
        reg[ro0]= (reg[ro0] >> imm);
        pc += 4;
    }
}

int criar_palavra(char instrucao[], unsigned int reg1, unsigned int reg2, unsigned int menOuImm, int inicio)
{
    int palavra;
    if(strcmp(instrucao,"ld")== 0){
        palavra = ld;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 21) | menOuImm;
    }else if(strcmp(instrucao,"st")== 0){
        palavra = st;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 21) | menOuImm;
    }else if(strcmp(instrucao,"add")== 0){
        palavra = add;
        palavra = (palavra << 3) |reg1;
        palavra = (palavra << 3) | reg2;
        palavra =  palavra << 18;
    }else if(strcmp(instrucao,"sub")== 0) {
        palavra = sub;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 3) | reg2;
        palavra = palavra << 18;
    }else if(strcmp(instrucao,"mul")== 0) {
        palavra = mul;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 3) | reg2;
        palavra = palavra << 18;
    }else if(strcmp(instrucao,"div")== 0) {
        palavra = div;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 3) | reg2;
        palavra = palavra << 18;
    }else if(strcmp(instrucao,"cmp")== 0) {
        palavra = cmp;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 3) | reg2;
        palavra = palavra << 18;
    }else if(strcmp(instrucao,"movr")== 0) {
        palavra = movr;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 3) | reg2;
        palavra = palavra << 18;
    }
    else if(strcmp(instrucao,"and")== 0) {
        palavra = and;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 3) | reg2;
        palavra = palavra << 18;
    }else if(strcmp(instrucao,"or")== 0) {
        palavra = or;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 3) | reg2;
        palavra = palavra << 18;
    }else if(strcmp(instrucao,"xor")== 0) {
        palavra = xor;
        palavra = (palavra << 3) | ro0;
        palavra = (palavra << 3) | palavra;
        palavra = palavra << 18;
    }else if(strcmp(instrucao,"not")== 0) {
        palavra = not;
        palavra = (palavra << 3)|ro0;
        palavra = palavra << 21;
    }else if(strcmp(instrucao,"je")== 0) {
        palavra = je;
        palavra = (palavra << 24) | menOuImm;
    }else if(strcmp(instrucao,"jne")== 0) {
        palavra = jne;
        palavra = (palavra << 24)| menOuImm;
    }else if(strcmp(instrucao,"jl")== 0) {
        palavra = jl;
        palavra = (palavra << 24)| menOuImm;
    }else if(strcmp(instrucao,"jg")== 0) {
        palavra = jg;
        palavra = (palavra << 24)| menOuImm;
    }else if(strcmp(instrucao,"jle")== 0) {
        palavra = jle;
        palavra = (palavra << 24)| menOuImm;
    }else if(strcmp(instrucao,"jge")== 0) {
        palavra = jge;
        palavra = (palavra << 24)| menOuImm;
    }else if(strcmp(instrucao,"jmp")== 0) {
        palavra = jmp;
        palavra = (palavra << 24)| menOuImm;
    }else if(strcmp(instrucao,"movi")== 0) {
        palavra = movi;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 21) | menOuImm;
    }else if(strcmp(instrucao,"addi")== 0) {
        palavra = addi;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 21) | menOuImm;
    }else if(strcmp(instrucao,"subi")== 0) {
        palavra = subi;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 21) | menOuImm;
    }else if(strcmp(instrucao,"muli")== 0) {
        palavra = muli;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 21) | menOuImm;
    }else if(strcmp(instrucao,"divi")== 0) {
        palavra = divi;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 21) | menOuImm;
    }else if(strcmp(instrucao,"lsh")== 0) {
        palavra = lsh;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 21) | menOuImm;
    }else if(strcmp(instrucao,"rsh")== 0) {
        palavra = rsh;
        palavra = (palavra << 3) | reg1;
        palavra = (palavra << 21) | menOuImm;
    }else if(strcmp(instrucao,"nop")== 0) {
        palavra = nop;
        palavra = palavra << 24;
    }else{
        palavra  = 0;
    }
    memoria[inicio++] = palavra >> 24;
    memoria[inicio++] = (palavra & 0x00ff0000) >> 16;
    memoria[inicio++] = (palavra & 0x0000ff00) >> 16;
    memoria[inicio] = palavra & 0x000000ff;

    return palavra;
}

void lerTexto()
{
    FILE *arq;
    char *pl;
    char str[50];
    char instrucao[10];
    int count = 0;
    int inicio;
    char tipo;
    unsigned int palavra;
    unsigned int reg1;
    unsigned int reg2;
    unsigned int menOuImm;
    arq = fopen("instrucoes.txt", "r");
    if (NULL == arq) {
        printf("Arquivo instrucoes.txt nao encontrado \n");
    } else{
        while (fgets(str, 50, arq) != NULL) {
            pl = strtok(str, ";");
            while(count < 3)
            {
                if(count == 0)
                {
                    inicio = (int) strtol(pl,NULL,16); //comeco da leitura
                } else if(count == 1)
                {
                    tipo = *pl;
                } else{
                    if(tipo == 'i')
                    {
                        pl = strtok(pl,", ");
                        if(strcmp(pl, "add") == 0 || strcmp(pl, "sub") == 0 ||
                           strcmp(pl, "mul") == 0 || strcmp(pl, "div") == 0 ||
                           strcmp(pl, "cmp") == 0 || strcmp(pl, "movr") == 0 ||
                           strcmp(pl, "and") == 0 || strcmp(pl, "or") == 0 ||
                           strcmp(pl, "xor") == 0)
                        {
                            strcpy(instrucao,pl);  //Pega a instrucao do texto
                            pl = strtok(NULL,"r, ");
                            reg1 = (int) strtol(pl, NULL, 16); //armazena no ro1
                            pl = strtok(NULL,"r, ");
                            reg2 = (int) strtol(pl, NULL, 16); //armazena no ro2

                            criar_palavra(instrucao,reg1,reg2,0,inicio);
                        } else if(strcmp(pl, "not") == 0)
                        {
                            strcpy(instrucao,pl);
                            pl = strtok(NULL,"r, ");
                            reg1 = (int) strtol(pl, NULL, 16);
                            criar_palavra(instrucao,reg1,0x0,0x0,inicio);
                        } else if(strcmp(pl, "je") == 0 || strcmp(pl, "jne") == 0 ||
                                  strcmp(pl, "jl") == 0 || strcmp(pl, "jle") == 0 ||
                                  strcmp(pl, "jg") == 0 || strcmp(pl, "jge") == 0 ||
                                  strcmp(pl, "jmp") == 0)
                        {
                            strcpy(instrucao,pl);
                            pl = strtok(NULL,", ");
                            menOuImm = (int) strtol(pl, NULL, 16);
                            palavra = criar_palavra(instrucao,0x00,0x00,menOuImm,inicio);
                        } else if(strcmp(pl, "ld") == 0 || strcmp(pl,"st") == 0)
                        {
                            strcpy(instrucao,pl);
                            pl = strtok(NULL, "r, ");
                            reg1 = (int) strtol(pl,NULL,16);
                            pl = strtok(NULL, ", ");
                            menOuImm = (int) strtol(pl, NULL, 16);
                            palavra = criar_palavra(instrucao,reg1,0x00,menOuImm,inicio);
                        }else if(strcmp(pl, "addi") == 0 || strcmp(pl, "subi") == 0 ||
                                 strcmp(pl, "muli") == 0 || strcmp(pl, "divi") == 0 ||
                                 strcmp(pl, "movi") == 0 ||strcmp(pl, "lsh") == 0 ||
                                 strcmp(pl, "rsh") == 0)
                        {
                            strcpy(instrucao, pl);
                            pl = strtok(NULL,"r, ");
                            reg1 = (int) strtol(pl, NULL, 16);
                            pl = strtok(NULL,"r, ");
                            menOuImm = (int) strtol(pl,NULL,16);
                            palavra = criar_palavra(instrucao,reg1,0x00,menOuImm,inicio);
                        }else if(strcmp(pl, "nop") == 0 || strcmp(pl, "hlt") == 0)
                        {
                            strcpy(instrucao,pl);
                            palavra = criar_palavra(instrucao,0x00,0x00,0x00,inicio);
                        }
                    } else{
                        palavra = (int) strtol(pl,NULL,16);
                        memoria[inicio++] = palavra >> 24;
                        memoria[inicio++] = (palavra & 0x00ff0000) >> 16;
                        memoria[inicio++] = (palavra & 0x0000ff00) >> 16;
                        memoria[inicio] = palavra & 0x000000ff;
                    }
                }
                pl = strtok(NULL,";");
                count++;
            }
            count = 0;
        }
    }
    fclose(arq);
}

int main() {
    lerTexto();
    pc = 0;
    ir=1;
    while (ir!=hlt){
        busca();
        decodifica();
        executa();
        for (int i=0; i < 8; i++) {
            printf("Registrador %d tem o valor %x\n", i, reg[i]);
        }
        printf("---------------Memorias-----------------\n");
        for (int i = 0; i < 154 ; i++) {
            printf("%d=[%x]  ",i,memoria[i]);
            if(i%10 == 0 && i!=0){
                printf("\n");
            }
        }
        printf("\nPressione entera para executar o proximo ciclo de instrucao\n");
        getchar();
    }
}