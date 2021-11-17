#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>


typedef struct EL{
    int key;
    bool colore;
    char contenuto [1025];
    struct EL* parent;
    struct EL* sx;
    struct EL* dx;
}node;

typedef node* pointer;
pointer NILL;





//ROTAZIONI ALBERO
pointer LeftRotate (pointer albero, pointer x){
    pointer y;
    y = x->dx;
    x->dx = y->sx;

    if(y->sx != NILL){
        y->sx->parent = x;
    }

    y->parent = x->parent;

    if(x->parent == NILL){
        albero=y;
    }

    else if(x->parent->sx->key == x->key){
        x->parent->sx = y;
    }

    else{
        x->parent->dx = y;
    }

    y->sx = x;
    x->parent = y;

    return albero;
}

pointer RightRotate (pointer albero, pointer y){
    pointer x;
    x = y->sx;
    y->sx = x->dx;

    if(x->dx != NILL){
        x->dx->parent=y;
    }

    x->parent=y->parent;

    if(y->parent == NILL){
        albero = x;
    }

    else if (y->parent->sx->key == y->key){
        y->parent->sx = x;
    }

    else{
        y->parent->dx =x;
    }
    x->dx = y;
    y->parent = x;
    return albero;
}

//INSERIMENTO ALBERO
pointer CreaNuovoNodo(int chiave){
    pointer nuovonodo;
    nuovonodo=(pointer)malloc(sizeof(node));
    nuovonodo->parent=NILL;
    nuovonodo->dx=NILL;
    nuovonodo->sx=NILL;
    nuovonodo->key=chiave;
    nuovonodo->colore = true;//ogni nodo inserito deve essere rosso, poi si ripara l'albero
    return nuovonodo;
}
pointer CreaNuovoNodoSuccessivo(int chiave, pointer riferimento){
    riferimento=NILL;
    riferimento=(pointer)malloc(sizeof(node));
    riferimento->parent=NILL;
    riferimento->dx=NILL;
    riferimento->sx=NILL;
    riferimento->key=chiave;
    riferimento->colore = true;//ogni nodo inserito deve essere rosso, poi si ripara l'albero
    return riferimento;
}

pointer RiparaRBInserisci (pointer albero, pointer nodo){
    pointer zio;
    while(nodo->parent->colore == true){
        if(nodo->parent->key == nodo->parent->parent->sx->key){
            zio = nodo->parent->parent->dx;

            if(zio->colore == true){
                nodo->parent->colore = false;
                zio->colore = false;
                nodo->parent->parent->colore = true;
                nodo = nodo->parent->parent;
            }

            else{
                if(nodo->key == nodo->parent->dx->key){
                    nodo = nodo->parent;
                    albero = LeftRotate(albero, nodo);
                }
                nodo->parent->colore = false;
                nodo->parent->parent->colore = true;
                albero = RightRotate(albero, nodo->parent->parent);
            }
        }

        else{
            zio = nodo->parent->parent->sx;

            if(zio->colore == true){
                nodo->parent->colore = false;
                zio->colore = false;
                nodo->parent->parent->colore = true;
                nodo = nodo->parent->parent;
            }

            else{
                if(nodo->key == nodo->parent->sx->key){
                    nodo = nodo->parent;
                    albero = RightRotate(albero, nodo);
                }
                nodo->parent->colore = false;
                nodo->parent->parent->colore = true;
                albero = LeftRotate(albero, nodo->parent->parent);
            }
        }
    }

    albero->colore = false;
    return albero;
}

pointer InserisciAlbero (pointer albero, pointer  newnode){
    pointer curr = albero;
    pointer pre = NILL;

    while(curr!=NILL){
        pre=curr;
        if(newnode->key<curr->key){
            curr=curr->sx;
        }
        else{
            curr=curr->dx;
        }

    }
    newnode->parent=pre;

    if(pre==NILL){
        albero=newnode;
    }

    else if(newnode->key < pre->key){
        pre->sx = newnode;
    }

    else{
        pre->dx = newnode;
    }
    albero = RiparaRBInserisci (albero, newnode);
    return albero;
}

//FUNZIONI STANDARD ALBERO
pointer min (pointer albero){
    pointer curr = albero;
    while(curr->sx != NILL){
        curr=curr->sx;
    }
    return curr;
}

pointer max (pointer albero){
    pointer curr = albero;
    while(curr->dx != NILL){
        curr = curr->dx;
    }
    return curr;
}

pointer successore (pointer albero){
    if(albero->dx != NILL){
        return min(albero->dx);
    }
    pointer temp;
    temp=albero->parent;
    while(temp != NILL && temp->dx->key == albero->key){
        albero = temp;
        temp = temp->parent;
    }
    return temp;
}

void Stampa (pointer inizio, int fine){
    if(inizio==NILL){
        return;
    }

    if(inizio->key==fine){
        fputs(inizio->contenuto, stdout);
        return;
    }
    Stampa(inizio->sx, fine);
    fputs(inizio->contenuto, stdout);
    Stampa(inizio->dx, fine);
    return;
}

pointer Ricerca (pointer testina, int valore){
    if(testina == NILL || testina->key==valore){
        return testina;
    }
    if(testina->key<valore){
        return Ricerca(testina->dx, valore);
    }
    else{
        return Ricerca(testina->sx, valore);
    }
}

pointer Scalatura (pointer albero, int minimo){
    pointer testina = Ricerca(albero, minimo);
    int corretto = 1;
    while(testina!=NILL){
        testina->key=corretto;
        corretto++;
        testina=successore(testina);
    }
    return albero;
}

//CANCELLAZIONE ALBERO
pointer RiparaRBCancella (pointer albero, pointer nodo){
    pointer fratello;
    while(nodo != albero && nodo->colore==false){
        if(nodo->parent->sx->key == nodo->key){
            fratello = nodo->parent->dx;

            //CASO 1 sx
            if(fratello->colore == true){
                fratello->colore=false;
                nodo->parent->colore=true;
                albero = LeftRotate(albero, nodo->parent);
                fratello= nodo->parent->dx;
            }

            //CASO 2 sx
            if(fratello->sx->colore==false && fratello->dx->colore==false){
                fratello->colore=true;
                nodo = nodo->parent;
            }

                //CASO 3 E 4 sx
            else{
                if(fratello->dx->colore==false){
                    fratello->sx->colore=false;
                    fratello->colore=true;
                    albero = RightRotate(albero, fratello);
                    fratello = nodo->parent->dx;
                }
                fratello->colore=nodo->parent->colore;
                nodo->parent->colore=false;
                fratello->dx->colore=false;
                albero = LeftRotate(albero, nodo->parent);
                nodo = albero;
            }

        }

        else{
            fratello = nodo->parent->sx;

            //CASO 1 dx
            if(fratello->colore == true){
                fratello->colore=false;
                nodo->parent->colore=true;
                albero = RightRotate(albero, nodo->parent);
                fratello = nodo->parent->sx;
            }

            //CASO 2 dx
            if(fratello->sx->colore==false && fratello->dx->colore==false ){
                fratello->colore=true;
                nodo = nodo->parent;
            }

                //CASO 3 E 4 sx
            else{
                if(fratello->sx->colore==false){
                    fratello->dx->colore=false;
                    fratello->colore=true;
                    albero = LeftRotate(albero, fratello);
                    fratello = nodo->parent->sx;
                }

                fratello->colore=nodo->parent->colore;
                nodo->parent->colore=false;
                fratello->sx->colore=false;
                albero = RightRotate(albero, nodo->parent);
                nodo = albero;
            }
        }
    }
    nodo->colore = false;
    return albero;
}

pointer Cancella (pointer albero, pointer valore){
    pointer cancellare = valore;
    pointer pre;
    if(cancellare->parent!=NILL){
        pre=cancellare->parent;
    }
    else{
        pre=NILL;
    }

    if(pre == NILL && cancellare->sx==NILL && cancellare->dx==NILL){
        albero = NILL;
        free(cancellare);
        return albero;
    }
    //CASO 1: IL NODO E' FOGLIA VIENE SEMPLICEMENTE ELIMINATO sostituito da foglia nulla
    if(cancellare->sx == NILL && cancellare->dx == NILL && pre!=NILL){
        pointer NULLLEAF;
        NULLLEAF=(pointer)malloc(sizeof(node));
        NULLLEAF->colore=false;
        if(pre->sx != NILL && pre->dx != NILL){
            if(pre->sx->key == cancellare->key){
                pre->sx = NULLLEAF;
                NULLLEAF->parent=pre;
                NULLLEAF->sx=NILL;
                NULLLEAF->dx=NILL;
                cancellare->parent=NILL;
                if(cancellare->colore==false){
                    albero = RiparaRBCancella(albero, NULLLEAF);
                }
                pre->sx=NILL;
                NULLLEAF->parent=NILL;

            }
            if(pre->dx->key == cancellare->key){
                pre->dx = NULLLEAF;
                NULLLEAF->parent=pre;
                NULLLEAF->sx=NILL;
                NULLLEAF->dx=NILL;
                cancellare->parent = NILL;
                if(cancellare->colore==false){
                    albero = RiparaRBCancella(albero, NULLLEAF);
                }
                pre->dx=NILL;
                NULLLEAF->parent=NILL;
            }
        }

        else{
            if(pre->sx == NILL){
                pre->dx = NULLLEAF;
                NULLLEAF->parent=pre;
                NULLLEAF->sx=NILL;
                NULLLEAF->dx=NILL;
                cancellare->parent = NILL;
                if(cancellare->colore==false){
                    albero = RiparaRBCancella(albero, NULLLEAF);
                }
                pre->dx=NILL;
                NULLLEAF->parent=NILL;
            }

            if(pre->dx == NILL){
                pre->sx = NULLLEAF;
                NULLLEAF->parent=pre;
                NULLLEAF->sx=NILL;
                NULLLEAF->dx=NILL;
                cancellare->parent = NILL;
                if(cancellare->colore==false){
                    albero = RiparaRBCancella(albero, NILL);
                }
                pre->sx=NILL;
                NULLLEAF->parent=NILL;
            }
        }
        free(NULLLEAF);
    }




    if(pre!=NILL){
        //CASO 2: L'ELEMENTO HA UN FIGLIO, VIENE SOSTITUITO DA ESSO
        //CASO 2.1 HA FIGLIO SINISTRO
        if(cancellare->sx != NILL && cancellare->dx == NILL){
            if (pre->sx->key == cancellare->key){
                pre->sx=cancellare->sx;
                cancellare->sx->parent=pre;
                cancellare->parent=NILL;
                cancellare->sx=NILL;
                albero = RiparaRBCancella(albero, pre->sx);
            }
            else if (pre->dx->key == cancellare->key){
                pre->dx=cancellare->sx;
                cancellare->sx->parent=pre;
                cancellare->parent=NILL;
                cancellare->sx=NILL;
                albero = RiparaRBCancella(albero, pre->dx);
            }
        }
        //CASO 2.2 HA FIGLIO DESTRO
        if(cancellare->sx == NILL && cancellare->dx != NILL){
            if(pre!=NILL){
                if (pre->sx->key == cancellare->key){
                    pre->sx=cancellare->dx;
                    cancellare->dx->parent=pre;
                    cancellare->parent=NILL;
                    cancellare->dx=NILL;
                    albero = RiparaRBCancella(albero, pre->sx);
                }

                else if (pre->dx->key == cancellare->key){
                    pre->dx=cancellare->dx;
                    cancellare->dx->parent=pre;
                    cancellare->parent=NILL;
                    cancellare->dx=NILL;
                    albero = RiparaRBCancella(albero, pre->dx);
                }
            }
        }

        //CASO 3: L'ELEMENTO HA DUE FIGLI COPIO IL VALORE DEL SUCCESORE SU DI ESSO ED ELIMINO IL NODO
        if(cancellare->sx != NILL && cancellare->dx != NILL){
            pointer next = successore (cancellare);
            //il successore non ha mai figlio sinistro;
            if (pre->sx->key == cancellare->key){
                pre->sx=next;
                next->parent=pre;
                next->sx=cancellare->sx;
                next->sx->parent=next;
                cancellare->dx=NILL;
                cancellare->sx=NILL;
                cancellare->parent=NILL;
                albero = RiparaRBCancella(albero, next);
            }

            else if (pre->dx->key == cancellare->key){
                pre->dx=next;
                next->parent=pre;
                next->sx=cancellare->sx;
                next->sx->parent=next;
                cancellare->dx=NILL;
                cancellare->sx=NILL;
                cancellare->parent=NILL;
                albero = RiparaRBCancella(albero, next);
            }
        }
    }


    else{
        //CASO 2
        //CASO 2.1 HA FIGLIO SINISTRO
        if(cancellare->sx != NILL && cancellare->dx == NILL){
            albero = cancellare->sx;
            cancellare->sx->parent=NILL;
            //cancellare->sx->sx=NILL;
            //cancellare->sx->dx=NILL;
            cancellare->sx=NILL;
            albero=RiparaRBCancella(albero, albero);
        }
        //CASO 2.2 HA FIGLIO DESTRO
        if(cancellare->sx == NILL && cancellare->dx != NILL){
            albero = cancellare->dx;
            cancellare->dx->parent=NILL;
            //cancellare->dx->dx=NILL;
            //cancellare->dx->sx=NILL;
            cancellare->dx=NILL;
            albero=RiparaRBCancella(albero, albero);

        }

        //CASO 3: L'ELEMENTO HA DUE FIGLI COPIO IL VALORE DEL SUCCESORE SU DI ESSO ED ELIMINO IL NODO
        if(cancellare->sx != NILL && cancellare->dx != NILL){
            pointer next = successore (cancellare);
            //il successore non ha mai figlio sinistro;
            next->sx=cancellare->sx;
            next->dx=cancellare->dx;
            cancellare->sx = NILL;
            cancellare->dx = NILL;
            next->parent->sx = NILL;
            next->parent = NILL;
            next->sx->parent=next;
            next->dx->parent=next;
            albero = next;
            albero = RiparaRBCancella(albero, next);
        }
    }

    free(cancellare);
    return albero;
}

//MEMORIA AUSILIARIA
//STACK
typedef struct stacknode{
    char contenuto [1025];
    int comando;
    struct stacknode* parent;
    struct stacknode* sx;
    struct stacknode* dx;
}stacknode;

typedef stacknode* stackpointer;
stackpointer STACKNILL;

stackpointer InserisciStack (stackpointer lista, char * contenuto, int chiave){
    stackpointer newnodostack, temp=lista;
    newnodostack = (stackpointer)malloc(sizeof(stacknode));
    newnodostack->sx=STACKNILL;
    newnodostack->dx=STACKNILL;
    strcpy(newnodostack->contenuto, contenuto);
    if(lista==STACKNILL){
        return newnodostack;

    }
    else{
        while(temp->dx != STACKNILL){
            temp=temp->dx;
        }
        temp->dx=newnodostack;
        newnodostack->sx=temp;
    }
    return newnodostack;
}


//PER UNDO
//reverse comando = 0 viene da una change ma il nodo non c'era va fatta una delete
//reverse comando = 1 viene da una change ma il nodo c'era, va cambiato il contenuto
//reverse comando = 2 viene da una delete va fatta un'aggiunta

//PER REDO
//reverse comando = 0 viene da una change ma il nodo non c'era va fatta un'aggiunta
//reverse comando = 1 viene da una change ma il nodo c'era, va cambiato il contenuto
//reverse comando = 2 viene da una delete va cancellata


void deallocate (pointer node){
    //do nothing if passed a non-existent node
    if (node == NILL){
        return;
    }
    //now onto the recursion
    deallocate(node->sx);
    deallocate(node->dx);
    free(node);
}
void stackdeallocate(stackpointer stack){
    if(stack == STACKNILL){
        return;
    }
    stackpointer temp = stack;
    stack = stack->sx;
    free(temp);
    stackdeallocate(stack);
}



int main(){
    //PROGRAMMA
    bool program = true;
    char sentence [1025];
    char command [30];
    char chartoint [30];
    char zero [] = ".\n";
    char c;
    int comando=0;
    int num_1,num_2;
    int undo_counter=0;
    int redo_counter=0;
    int i=0;
    int j=0;


    //FILE ALBERO
    NILL =(pointer)malloc(sizeof(node));
    NILL->colore = false;
    pointer albero=NILL;
    pointer newnode;
    pointer testina;
    pointer segnaposto=(pointer)malloc(sizeof(node));
    segnaposto->colore=false;
    strcpy(segnaposto->contenuto, "*");
    //STACK
    STACKNILL=(stackpointer)malloc(sizeof(stacknode));
    stackpointer stack = STACKNILL;
    int contatoredeallocate = 0;

    FILE * fp;
    //fp = fopen("Write_Only_1_input.txt","r");
    //fp = fopen("Write_Only_2_input.txt","r");
    //fp = fopen("Bulk_Reads_1_input.txt","r");
    //fp = fopen("Bulk_Reads_2_input.txt","r");
    //fp = fopen("Time_for_a_change_1_input.txt","r");
    //fp = fopen("Time_for_a_change_2_input.txt","r");
    //fp = fopen("Rolling_Back_1_input.txt","r");
    //fp = fopen("Rolling_Back_2_input.txt","r");
    fp = fopen("test10.txt","r");
    //fp = fopen("test20.txt","r");
    //fp = fopen("test50.txt","r");
    //fp = fopen("test100.txt","r");
    //fp = fopen("test150.txt","r");
    //fp = fopen("test200.txt","r");
    //fp = fopen("test500.txt","r");
    //fp = fopen("test1000.txt","r");

    while(program){
        fgets(command, 30, fp);

        if(command[0]=='q'){
            program = false;
        }

        while(command[i]>='0' && command[i]<='9'){
            chartoint[i]=command[i];
            i++;
        }
        chartoint[i]='\0';
        num_1=atoi(chartoint);

        if(command[i]=='u' || command[i]=='r'){
            int contatore = 0;
            //contatore mi dirà se positivo di fare undo altrimenti se negativo di fare redo
            if(command[i]=='u'){

                if(num_1>undo_counter){

                    num_1=undo_counter;
                }

                undo_counter=undo_counter-num_1;
                redo_counter=redo_counter+num_1;
                contatore=contatore+num_1;

            }
            else{

                if(num_1>redo_counter){

                    num_1=redo_counter;
                }

                undo_counter=undo_counter+num_1;
                redo_counter=redo_counter-num_1;
                contatore=contatore-num_1;
            }


            while(command[i]=='u' || command[i]=='r'){

                fgets(command, 30, fp);
                //DOPO U/R LEGGO Q
                //FINISCO SENZA FARE NIENTE

                //DOPO U/R LEGGO U/R
                if(command[i]=='u' || command[i]=='r'){

                    if(command[i]=='u'){
                        if(num_1>undo_counter){
                            num_1=undo_counter;
                        }
                        undo_counter=undo_counter-num_1;
                        redo_counter=redo_counter+num_1;
                        contatore=contatore+num_1;
                        c=command[i];
                    }
                    else if(command[i]=='r'){
                        if(num_1>redo_counter){
                            num_1=redo_counter;
                        }
                        undo_counter=undo_counter+num_1;
                        redo_counter=redo_counter-num_1;
                        contatore=contatore-num_1;
                        c=command[i];
                    }
                }
                else{

                    //FACCIO UNDO
                    if(contatore>0){
                        //se la stack viene eliminata mettere comando=0
                    }
                        //FACCIO REDO
                    else if(contatore<0){

                    }

                    if(command[0]=='q'){
                        program = false;
                    }

                    else{

                        i=0;
                        while(command[i]>='0' && command[i]<='9'){
                            chartoint[i]=command[i];
                            i++;
                        }
                        c=command[i];
                        chartoint[i]='\0';
                        num_1=atoi(chartoint);
                        i++;
                        while(command[i]>='0' && command[i]<='9'){
                            chartoint[j]=command[i];
                            i++;
                            j++;
                        }
                        chartoint[j]='\0';
                        num_2=atoi(chartoint);
                        c=command[i];
                    }
                }

            }
        }
        else{
            i++;
            while(command[i]>='0' && command[i]<='9'){
                chartoint[j]=command[i];
                i++;
                j++;
            }
            chartoint[j]='\0';
            num_2=atoi(chartoint);
            c=command[i];
        }
        //FINE LETTURA DI ISTRUZIONI
        //CASI CHANGE DELETE E PRINT
        //CASO CHANGE
        if(c == 'c'){

            comando++;
            redo_counter=0;
            undo_counter++;
            while(num_1<=num_2){
                if(albero == NILL){

                    newnode=CreaNuovoNodo(num_1);
                    fgets(sentence, 1025, fp);
                    strcpy(newnode->contenuto, sentence);
                    //stack = InserisciStack(stack, newnode->contenuto);
                    albero = InserisciAlbero(albero, newnode);
                }
                else{
                    testina=Ricerca(albero, num_1);
                    if(testina==NILL){
                        //newnode=CreaNuovoNodo(num_1);
                        newnode=CreaNuovoNodoSuccessivo(num_1, newnode);
                        //stack = InserisciStack(stack, newnode->contenuto, num_1);
                        fgets(sentence, 1025, fp);
                        strcpy(newnode->contenuto, sentence);
                        albero = InserisciAlbero(albero, newnode);
                    }
                    else{
                        //stack = InserisciStack(stack, testina->contenuto, num_1);
                        fgets(sentence, 1025, fp);
                        strcpy(testina->contenuto, sentence);
                    }
                }
                num_1++;
            }
            fgets(sentence, 30, fp);
            contatoredeallocate=1;
        }

        if(c=='d'){
            comando++;
            redo_counter=0;
            undo_counter++;
            pointer cancellare;
            if(albero!=NILL){
                if(num_1==0){
                    if(num_2!=0){
                        num_1++;
                    }
                }
                if(num_1==1){
                    //printf("caso 1,%d\n", num_2);
                    testina=min(albero);
                    //printf("%s\n", testina->contenuto);
                    while(num_1<=num_2){
                        if(testina!=NILL){
                            albero=Cancella(albero, testina);
                        }
                        if(albero!=NILL){
                            testina=min(albero);
                        }
                        else{
                            testina=NILL;
                        }
                        num_1++;
                    }
                }
                else{
                    //printf("caso y,x\n");
                    testina=Ricerca(albero, num_1);
                    while(num_1<=num_2){
                        if(testina!=NILL){
                            albero=Cancella(albero, testina);
                        }
                        num_1++;
                        if(albero!=NILL){
                            testina=Ricerca(albero, num_1);
                        }
                        else{
                            testina=NILL;
                        }
                    }
                }

            }
        }

        //CASO PRINT
        if(c == 'p'){
            if(albero!=NILL){
                if(num_1==0){
                    fputs(zero, stdout);
                    num_1++;
                }
                if(num_1==1){
                    if(albero!=NILL){
                        testina=min(albero);
                    }
                    else{
                        testina=NILL;
                    }
                }
                while(num_1<=num_2){
                    if(testina!=NILL){
                        fputs(testina->contenuto, stdout);
                        testina=successore(testina);
                    }
                    else{
                        fputs(zero, stdout);
                    }
                    num_1++;
                }
            }
            else{
                while(num_1<=num_2){
                    fputs(zero, stdout);
                    num_1++;
                }
            }
        }
        i=0;
        j=0;
    }

    //free di tutto l'albero
    if(contatoredeallocate==1){
        //stackdeallocate(stack);
    }
    deallocate(albero);
    free(NILL);
    free(segnaposto);


    return 0;
}

//false = nero
//true = rosso


//struttura dati di memoria