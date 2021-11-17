#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>


//GESTIONE FRASI
typedef struct Righe{
    char riga[1025];
    struct Righe* up;
    struct Righe*down;
}Righe;
typedef Righe * righepointer;
righepointer RIGHENILL;
righepointer Inseriscicache (righepointer cache){
    //la cache non è mai vuota c'è sempre il nodo con ".\n"
    bool inserisci = true;
    righepointer temp = cache;
    char frase [1025];
    fgets(frase, 1025, stdin);
    while(temp->down!=RIGHENILL){
        if(strcmp(frase, temp->riga)==0){
            inserisci=false;
            return temp;
        }
        temp=temp->down;
    }
    if(strcmp(frase, temp->riga)==0){
        inserisci=false;
        return temp;
    }

    if(inserisci){
        temp->down=(righepointer)malloc(sizeof(Righe));
        strcpy(temp->down->riga, frase);
        temp->down->down=RIGHENILL;
        temp->down->up=temp;
        temp=temp->down;
    }
    return temp;
}



righepointer RicercaCache(righepointer head, char * frase){
    while(head!=RIGHENILL){
        if(strcmp(head->riga, frase)==0){
            return head;
        }
        head=head->down;
    }
}

//GESTIONE DECK
typedef struct EL{
	int key;
    char * contenuto;
	struct EL* sx;
    struct EL* dx;
}node;
typedef node* pointer;
pointer NILL;

pointer Ricerca (pointer testina,pointer fine, int valore){
    if(valore>fine->key){
        return NILL;
    }
    while(valore>1){
        if(testina!=NILL){
            testina=testina->dx;
        }
        valore--;
    }
    return testina;
}


pointer RicercaConMax(pointer testina, pointer fine, int valore){
    if(valore>=fine->key){
        return fine;
    }
    while(valore>1){
        if(testina->dx!=NILL){
            testina=testina->dx;
        }
        valore--;
    }
    return testina;
}
//INSERIMENTO DECK
void Scalatura (pointer head){
    int num=1;
    while(head!=NILL){
        head->key=num;
        num++;
        head=head->dx;
    }
}

pointer InserisciDeck (pointer testina, int chiave, righepointer frase){
    pointer newnode;
    newnode=(pointer)malloc(sizeof(node));
    newnode->dx=NILL;
    newnode->sx=NILL;
    newnode->key=chiave;
    (newnode->contenuto)=&(frase->riga[0]);

    if(testina==NILL){
        return newnode;
    }
    else{
        testina->dx=newnode;
        newnode->sx=testina;
        return newnode;
    }
}


//GESTIONE UNDO E REDO
typedef struct nodostack{
    int key;
    int numeronodi;
    struct nodostack* up;
    struct nodostack* down;
    struct nodostack* state;
    struct nodostack* endstate;
    char * oldsentence;
}nodostack;
typedef nodostack * URpointer;
URpointer URNILL; 
URpointer SalvaStato(URpointer operazione, pointer testina){
    if(testina!=NILL){
        operazione->state=(URpointer)malloc(sizeof(nodostack));
        operazione->state->key=testina->key;
        operazione->state->oldsentence=testina->contenuto;
        operazione->state->state=SalvaStato(operazione->state, testina->dx);
    }
    else{
        operazione->state=URNILL;
    }
    return operazione->state;
}
URpointer CreaStato(pointer head, pointer tail, URpointer stack, int comando, int check){
    URpointer temp;
    int i;
    stack->down=(URpointer)malloc(sizeof(nodostack));
    stack->down->up=stack;
    stack=stack->down;
    stack->key=comando;
    if(tail!=NILL){
        stack->numeronodi=tail->key;
    }
    else{
        stack->numeronodi=0;
    }
    stack->down=URNILL;
    if(head!=NILL){
        if(check==0){
            //delte, change, oppure aggiunta per la prima volta
            stack->state=SalvaStato(stack, head);
            temp=stack->state;
            i=stack->numeronodi;
            while(i>1){
                temp=stack->state;
                i--;
            }
            stack->endstate=temp;
        }
        else{
            stack->state=stack->up->state;
            stack->up->endstate->state=SalvaStato(stack->up->endstate, head);
            i=stack->numeronodi;
            while(i>1){
                temp=stack->state;
                i--;
            }
            stack->endstate=temp;
        }

    }
    else{
        stack->state=URNILL;
    }
    return stack;
    //ritorna l'ultima posizione, passa l'ultima posizione che punta a nill
}

pointer DoUndo(pointer head, pointer tail, int iterazioni, URpointer codastack, righepointer rhead){
    righepointer frase;
    pointer testina;
    int i =codastack->numeronodi;
    codastack=codastack->state;
    if(codastack!=URNILL){
        while(i>0){
            if(head==NILL){
                frase=RicercaCache(rhead, codastack->oldsentence);
                tail=InserisciDeck(tail, codastack->key, frase);
            }
            else{
                head->contenuto=codastack->oldsentence;
                tail=head;
                head=head->dx;
            }
            codastack=codastack->state;
            i--;     
        }
        if(head==NILL){
            head=tail;
        }
        else{
            tail->dx=NILL;   
            head=tail;         
        }
    }
    else{
        head=NILL;
    }
    return head;
}

pointer DoRedo (pointer head, pointer tail,  int iterazioni, URpointer codastack, righepointer rhead){
    righepointer frase;
    pointer testina;
    int i=codastack->numeronodi;
    testina=head; 
    codastack=codastack->state;
    if(codastack!=URNILL){
        while(i>0){
            if(head==NILL){
                frase=RicercaCache(rhead, codastack->oldsentence);
                tail=InserisciDeck(tail, codastack->key, frase);
            }
            else{
                head->contenuto=codastack->oldsentence;
                tail=head;
                head=head->dx;
            }
            codastack=codastack->state;  
            i--;   
        }
        if(head==NILL){
            head=tail;
        }
        else{
            tail->dx=NILL;
            head=tail;            
        }
    }
    else{
        head=NILL;
    }
    return head;
}
pointer RestoreHead(pointer tail){
    if(tail==NILL){
        return tail;
    }
    while(tail->sx!=NILL){
        tail=tail->sx;
    }
    return tail;
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
    int modo;
    
    
    //FILE DECK
    NILL =(pointer)malloc(sizeof(node));
    pointer head=NILL;
    pointer tail=head;
    pointer newnode;
    pointer testina;
    pointer salva;

    //FILE FRASI
    RIGHENILL=(righepointer)malloc(sizeof(Righe));
    righepointer headcache=(righepointer)malloc(sizeof(Righe));
    headcache->up=RIGHENILL;
    headcache->down=RIGHENILL;
    strcpy(headcache->riga, zero);
    righepointer tailcache = headcache;

    //FILE STACK
    URNILL=(URpointer)malloc(sizeof(nodostack));
    URpointer headstack =(URpointer)malloc(sizeof(nodostack));
    headstack->up=URNILL;
    headstack->down=URNILL;
    headstack->state=URNILL;
    headstack->key=0;
    URpointer tailstack = headstack;





    while(program){
        
        fgets(command, 30, stdin);
        if(command[0]=='q'){
            program = false;
        }
        while(command[i]>='0' && command[i]<='9'){
            chartoint[i]=command[i];
            i++;
        }
        chartoint[i]='\0';
        num_1=atoi(chartoint);
        //NESTING DI UNDO E REDO



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
                
                fgets(command, 30, stdin);
                i=0;
                while(command[i]>='0' && command[i]<='9'){
                    chartoint[i]=command[i];
                    i++;
                }
                chartoint[i]='\0';
                num_1=atoi(chartoint);

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
                        while(contatore>=1){
                            if(tailstack->up!=URNILL){
                                tailstack=tailstack->up;
                            }
                            contatore--;
                        }
                        URpointer tempstack=tailstack->state;
                        while(tempstack!=URNILL){
                            tempstack=tempstack->state;
                        }
                        tail=DoUndo(head, tail, contatore, tailstack, headcache);
                        head=RestoreHead(tail);
                        
                    }
                    //FACCIO REDO
                    else if(contatore<0){
                        while(contatore<=-1){
                            if(tailstack->down!=URNILL){
                                tailstack=tailstack->down;
                            }
                            contatore++;
                        }
                        URpointer tempstack=tailstack->state;
                        while(tempstack!=URNILL){
                            tempstack=tempstack->state;
                        }
                        tail=DoRedo(head, tail, contatore, tailstack, headcache);
                        head=RestoreHead(tail);
                        
                    }

                    if(command[0]=='q'){
                        program = false;
                        c='q';
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
        modo=1;
        
        
        //CASI CHANGE DELETE E PRINT                
        //CASO CHANGE
        if(c == 'c'){
            tailstack->down=URNILL;
            if(comando != tailstack->key){
                comando=tailstack->key;
            }
            bool salvare=true;
            redo_counter=0;
            undo_counter++;                   
            while(num_1<=num_2){
                if(head == NILL){
                    modo=0;
                    tailcache=Inseriscicache(headcache);
                    head=InserisciDeck(head, num_1, tailcache);
                    tail=head;
                    salvare=false;

                }
                else{
                    testina=Ricerca(head, tail, num_1);
                    if(testina==NILL){
                        tailcache=Inseriscicache(headcache);
                        tail=InserisciDeck(tail, num_1, tailcache);
                        if(salvare==true){
                            salva=tail;
                            salvare=false;
                        }
                    }
                    else{
                        modo=0;
                        tailcache=Inseriscicache(headcache);
                        testina->contenuto=tailcache->riga;
                        salvare=false;
                    }        
                }
                num_1++;
            }
            comando++;
            if(modo==0){
                tailstack=CreaStato(head,tail, tailstack, comando, modo);
            }
            else{
                tailstack=CreaStato(salva,tail, tailstack, comando, modo);
            }
        }

        //CASO DELETE
        if(c=='d'){
            modo=0;
            redo_counter=0;
            undo_counter++;
            tailstack->down=URNILL;
            if(comando != tailstack->key){
                comando=tailstack->key;
            }
            pointer fine;
            if(head != NILL){
                if(num_1==0){
                    if(num_2!=0){
                        num_1++;
                        testina=head;
                        fine=RicercaConMax(head, tail, num_2);
                        if(fine==tail){
                            head=NILL;
                            tail=NILL;
                        }
                        else{
                            head=fine->dx;
                            fine->dx->sx=NILL;
                        }
                    }
                }
                else{
                    testina=RicercaConMax(head, tail, num_1);
                    fine=RicercaConMax(head, tail, num_2);
                    if(testina==head){
                        if(fine==tail){
                            head=NILL;
                            tail=NILL;
                        }
                        else{
                            head=fine->dx;
                            head->sx=NILL;
                        }
                    }
                    else{
                        if(fine==tail){
                            tail=testina->sx;
                            tail->dx=NILL;
                        }
                        else{
                            testina->sx->dx=fine->dx;
                            fine->dx->sx=testina->sx;
                            testina->sx=NILL;
                            fine->dx=NILL;
                        }
                    }
                }
            }
            Scalatura(head);
            comando++;
            if(head!=NILL){
                tailstack=CreaStato(head,tail, tailstack, comando, modo);
            }
            else{
                tailstack=CreaStato(NILL, NILL, tailstack, comando, modo);
            }
        }

        //CASO PRINT
        if(c == 'p'){
            if(head!=NILL){
                if(num_1==0){
                    if(num_2==0){
                        fputs(zero, stdout);
                    }
                    else{
                        fputs(zero, stdout);
                        num_1++;
                        testina=Ricerca(head,tail, num_1);
                        while(num_1<=num_2){
                            if(testina != NILL){
                                fputs((testina->contenuto), stdout);
                                testina=testina->dx;
                            }
                            else{
                                fputs(zero, stdout);
                            }
                            num_1++;
                        }
                    }
                }
                else{
                    testina=Ricerca(head, tail, num_1);
                    while(num_1<=num_2){
                        if(testina!=NILL){
                            fputs((testina->contenuto), stdout);
                            testina=testina->dx;
                        }
                        else{
                        fputs(zero, stdout);
                        }
                        num_1++;
                    }
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
    return 0;
}