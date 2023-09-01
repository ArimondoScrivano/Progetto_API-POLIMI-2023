#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Station: double linked list
struct Station {
    //km from the start
    int distance;
    //VEHICLES
    int vehicles[512];
    int veicoli_presenti;
    struct Station* next;
    struct Station* prev;
};

struct list{
    int stazione;
    struct list* next;
    struct list* prev;
};

struct inverted_List{
    struct Station* stazione;
    struct inverted_List* next;
    struct inverted_List* prev;
};


int stazioni_presenti=0;




int Station_Finder(struct Station* array[], int station_to_find){
    int left = 0;
    //IL NUMERO DEGLI ELEMENTI MASSIMI SET=200
    int right = stazioni_presenti-1;
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (array[mid]->distance == station_to_find) {
            return mid;
        } else if (array[mid]->distance < station_to_find) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;
}

int insertAndFindIndex(struct Station* array[], struct Station* element, int distance) {
    //IL NUMERO DEGLI ELEMENTI MASSIMI SET =200
    int i = stazioni_presenti-1;
    while (i >= 0 && array[i]->distance > distance) {
        array[i + 1] = array[i];
        i--;
    }
    array[i + 1] = element;
    return i + 1;
}

int Binary_Search(int array[], int articoli_inseriti, int k) {
    int left = 0;
    int right = articoli_inseriti-1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (array[mid] == k) {
            return mid;
        } else if (array[mid] < k) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return -1;
}



int Partition(int A[], int p, int r){
    int x= A[r];
    int i= p-1;
    int temp;
    for(int j=p; j<r; j++) {
        if (A[j] >= x) {
            i = i + 1;
            temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }
    }
    temp= A[i+1];
    A[i+1]= A[r];
    A[r]= temp;
    return i+1;
}



void  Quicksort(int A[], int p, int r){
    if(p<r){
        int q=  Partition(A, p, r);
        Quicksort(A,p, q-1);
        Quicksort(A,q+1,r);
    }
}



char* aggiungi_stazione(struct Station** stations, int distanza_stazione, int A[], int elementi_inseriti, struct Station* array_di_stazioni[] ) {
    if (stazioni_presenti==0) {
        (*stations)->distance = distanza_stazione;
        (*stations)->veicoli_presenti= elementi_inseriti;
        Quicksort(A,0,elementi_inseriti-1);

        for(int i=0; i<elementi_inseriti; i++){
            (*stations)->vehicles[i]= A[i];
        }
        stazioni_presenti=1;

        return "aggiunta";
    }

    int indice= Station_Finder(array_di_stazioni, distanza_stazione);
    if(indice!= -1){
        return "non aggiunta";
    }else{
        struct Station* nuovo_nodo= (struct Station*)malloc(sizeof(struct Station));
        nuovo_nodo->distance= distanza_stazione;
        nuovo_nodo->veicoli_presenti= elementi_inseriti;
        memset(nuovo_nodo->vehicles, 0, sizeof nuovo_nodo->vehicles);
        indice= insertAndFindIndex(array_di_stazioni,nuovo_nodo, distanza_stazione);

        //PRIMO ELEMENTO AGGIORNATO
        if(indice==0){
            nuovo_nodo->prev=NULL;
            nuovo_nodo->next= array_di_stazioni[indice+1];
            if(stazioni_presenti> 0){
                array_di_stazioni[indice+1]->prev=nuovo_nodo;
            }
            (*stations)= nuovo_nodo;
            Quicksort(A,0,elementi_inseriti-1);
            for(int i=0; i<elementi_inseriti; i++){
                nuovo_nodo->vehicles[i]= A[i];
            }
            //AGGIUNGIAMO UNA STAZIONE
            stazioni_presenti++;

            return "aggiunta";
        }
        //ULTIMO ELEMENTO AGGIORNATO
        if(indice==stazioni_presenti){
            nuovo_nodo->next= NULL;
            nuovo_nodo->prev=array_di_stazioni[indice-1];
            array_di_stazioni[indice -1]->next= nuovo_nodo;
            Quicksort(A,0,elementi_inseriti-1);
            for(int i=0; i<elementi_inseriti; i++){
                nuovo_nodo->vehicles[i]= A[i];
            }
            //AGGIUNGIAMO UNA STAZIONE
            stazioni_presenti++;
            return "aggiunta";
        }


        //ELEMENTO INTERMEDIO
        nuovo_nodo->next= array_di_stazioni[indice+1];
        nuovo_nodo->prev= array_di_stazioni[indice -1];

        if(indice!= stazioni_presenti) {
            array_di_stazioni[indice + 1]->prev = nuovo_nodo;
        }
        array_di_stazioni[indice -1]->next= nuovo_nodo;
        Quicksort(A,0,elementi_inseriti-1);
        for(int i=0; i<elementi_inseriti; i++){
            nuovo_nodo->vehicles[i]= A[i];
        }
        //AGGIUNGIAMO UNA STAZIONE
        stazioni_presenti++;

        return "aggiunta";


    }

}

char* demolisci_stazione(struct Station** head, int target_distance, struct Station* array_di_stazioni[]) {

    int indice= Station_Finder(array_di_stazioni, target_distance);
    if(indice==-1){
        return "non demolita";
    }else{

        if(indice !=0) {
            array_di_stazioni[indice - 1]->next = array_di_stazioni[indice + 1];
            if(array_di_stazioni[indice + 1]!=NULL){
                array_di_stazioni[indice + 1]->prev = array_di_stazioni[indice - 1];
            }

        }else{

            if(array_di_stazioni[indice + 1]!=NULL){
                array_di_stazioni[indice + 1]->prev = NULL;
                (*head)= (*head)->next;
            }

        }
        if(stazioni_presenti>1){
            while(indice!= stazioni_presenti-1 ){
                array_di_stazioni[indice]=array_di_stazioni[indice +1];
                indice++;
            }
            array_di_stazioni[indice]=NULL;
        }

        //SOTTRAIAMO UNA STAZIONE
        stazioni_presenti--;

        return "demolita";
    }
}

char* rottama_auto(struct Station** stations,int distanza_stazione, int autonomia_auto_da_rottamare, struct Station* array_di_stazioni[]) {
    int indice= Station_Finder(array_di_stazioni, distanza_stazione);

    if(indice==-1){
        return "non rottamata";
    }else{
        int i= Binary_Search(array_di_stazioni[indice]->vehicles,array_di_stazioni[indice]->veicoli_presenti, autonomia_auto_da_rottamare);
        if(i== -1){
            return "non rottamata";
        }else{
            while (i< 511 && array_di_stazioni[indice]->vehicles[i] != 0) {
                array_di_stazioni[indice]->vehicles[i] = array_di_stazioni[indice]->vehicles[i + 1];
                i++;
            }
            array_di_stazioni[indice]->vehicles[511] = 0;
            array_di_stazioni[indice]->veicoli_presenti--;
            return "rottamata";
        }
    }

}

char* aggiungi_auto(struct Station* stations, int distanza_stazione, int autonomia_auto, struct Station* array_di_stazioni[]) {
    int indice= Station_Finder(array_di_stazioni, distanza_stazione);

    if(indice==-1){
        return "non aggiunta";
    }else{
        //ECCEDUTO LIMITE MASSIMO
        if(array_di_stazioni[indice]->vehicles[511]!=0){
            return "non aggiunta";
        }
        //CASO AUMENTO DI AUTONOMIA
        if(array_di_stazioni[indice]->vehicles[0]< autonomia_auto){
            for(int k=array_di_stazioni[indice]->veicoli_presenti; k>0; k-- ){
                array_di_stazioni[indice]->vehicles[k]=  array_di_stazioni[indice]->vehicles[k -1];
            }
            array_di_stazioni[indice]->vehicles[0]= autonomia_auto;

            array_di_stazioni[indice]->veicoli_presenti++;
            return "aggiunta";
        }
        int i= array_di_stazioni[indice]->veicoli_presenti ;
        while( array_di_stazioni[indice]->vehicles[i-1] < autonomia_auto){
            array_di_stazioni[indice]->vehicles[i]=  array_di_stazioni[indice]->vehicles[i -1];
            i--;
        }

        array_di_stazioni[indice]->vehicles[i]= autonomia_auto;

        array_di_stazioni[indice]->veicoli_presenti++;
        return "aggiunta";
    }

}


void recursive_path_finder( struct Station* current_pointer,int stazione_partenza, struct list** lista_percorso){

    //ANCORA NON SIAMO ARRIVATI ALLA FINE
    if((*lista_percorso)->stazione - current_pointer->distance <= current_pointer->vehicles[0]){
        //ARRIVIAMO ALLA STAZIONE
        while((*lista_percorso)->prev!=NULL){

            if((*lista_percorso)->prev->stazione - current_pointer->distance <= current_pointer->vehicles[0]){
                struct list *puntatore = (*lista_percorso);
                (*lista_percorso) = (*lista_percorso)->prev;
                (*lista_percorso)->next = NULL;
                free(puntatore);
            }else{
                break;
            }
        }
        //AGGIUNGIAMO IL NODO ALLA LISTA
        struct list *nuovo_nodo = (struct list *) malloc(sizeof(struct list));
        (*lista_percorso)->next = nuovo_nodo;
        nuovo_nodo->stazione = current_pointer->distance;
        nuovo_nodo->next = NULL;
        nuovo_nodo->prev = (*lista_percorso);
        (*lista_percorso) = (*lista_percorso)->next;
    }
    if(current_pointer->distance!=stazione_partenza) {
        //ANCORA NON SIAMO ARRIVATI ALLA FINE
        return recursive_path_finder(current_pointer->prev, stazione_partenza, lista_percorso);
    }else{
        return;
    }
}




void recursive_path_finder_INV( struct Station* current_pointer, struct Station* stazione_finale, int stazione_arrivo, struct inverted_List** lista_percorso){
//CONTROLLO SE SIAMO ARRIVATI A TERMINAZIONE

    if(current_pointer->distance - stazione_arrivo <= current_pointer->vehicles[0]){
        //AGGIUNGIAMO IL NODO ALLA LISTA
        struct inverted_List* nuovo_nodo = (struct inverted_List *) malloc(sizeof(struct inverted_List));
        (*lista_percorso)->next = nuovo_nodo;
        nuovo_nodo->stazione = stazione_finale;
        nuovo_nodo->next = NULL;
        nuovo_nodo->prev = (*lista_percorso);
        (*lista_percorso) = (*lista_percorso)->next;
        //AGGIUNGIAMO IL NODO FINALE ALLA LISTA

        return;
    }

    struct inverted_List* nuovo_nodo = (struct inverted_List *) malloc(sizeof(struct inverted_List));
    (*lista_percorso)->next = nuovo_nodo;
    nuovo_nodo->stazione = current_pointer;
    nuovo_nodo->next = NULL;
    nuovo_nodo->prev = (*lista_percorso);
    struct Station* stationMaxAutonomy= current_pointer;

    //NODI A CUI POSSO ARRIVARE
    while(current_pointer->distance - stationMaxAutonomy->distance <= current_pointer->vehicles[0]){
        //STIAMO PARTENDO DA UN NUMERO MAGGIORE ALLA PARTENZA E INFERIORE ALL'ARRIVO
        stationMaxAutonomy= stationMaxAutonomy->prev;
    }
    stationMaxAutonomy= stationMaxAutonomy->next;

    //NON RIESCO AD ARRIVARE DA NESSUNA PARTE
    if(stationMaxAutonomy->distance== current_pointer->distance){
        return;
    }
    int counter=0;
    while(stationMaxAutonomy->distance!= current_pointer->distance){

        if(stationMaxAutonomy->distance - stazione_arrivo <= stationMaxAutonomy->vehicles[0]){
            nuovo_nodo->stazione = stationMaxAutonomy;
            break;
        }else{
            if((current_pointer->distance - stationMaxAutonomy->distance + stationMaxAutonomy->vehicles[0] ) > counter){
                counter=current_pointer->distance - stationMaxAutonomy->distance + stationMaxAutonomy->vehicles[0];
                nuovo_nodo->stazione= stationMaxAutonomy;
            }

        }
        stationMaxAutonomy= stationMaxAutonomy->next;
    }

    if(nuovo_nodo==NULL ){
        return;
    }else{
        (*lista_percorso)=(*lista_percorso)->next;
        recursive_path_finder_INV((*lista_percorso)->stazione, stazione_finale, stazione_arrivo, lista_percorso);
    }


}





void pianifica_percorso(struct Station* mainStation, int stazione_uno, int stazione_due, struct list** lista_percorso, struct inverted_List** inverted_list, struct Station* array_di_stazioni[]){

    int stazione_arrivo1= Station_Finder(array_di_stazioni,stazione_due);
    int stazione_partenza2 = Station_Finder(array_di_stazioni, stazione_uno);

    if(stazione_arrivo1== -1 || stazione_partenza2== -1){

        return;
    }else{
        struct Station* stazione_arrivo= array_di_stazioni[stazione_arrivo1];
        struct Station* stazione_partenza = array_di_stazioni[stazione_partenza2];

        if(stazione_uno< stazione_due){
            (*lista_percorso)->stazione= stazione_due;
            recursive_path_finder( stazione_arrivo->prev,stazione_uno, lista_percorso);
        }else{
            (*inverted_list)->stazione= stazione_partenza;

            recursive_path_finder_INV( stazione_partenza,stazione_arrivo,stazione_due, inverted_list);

        }
    }


}



void mainFunction( struct Station* mainStation, struct Station* array_di_stazioni[]){
    char stringa[30];
    FILE *file;

    file = stdin;
    if (file == NULL) {
        return ;
    }

    while (fscanf(file, "%s", stringa) ==1) {

        //---------------AGGIUNGI-STAZIONE-------------------
        if (stringa[0] == 'a' && stringa[9] == 's') {
            int distanza;
            int numeri_in_ingresso;
            int numeri[512];
            memset(numeri, 0, sizeof numeri);
            int n = 0;
            if(fscanf(file,"%d", &distanza)==1){}
            if(fscanf(file,"%d", &numeri_in_ingresso)==1){}
            int elementi_inseriti= numeri_in_ingresso;
            while (numeri_in_ingresso > 0) {
                if(fscanf(file,"%d", &numeri[n])==1){}
                n++;
                numeri_in_ingresso--;
            }

            printf("%s\n", aggiungi_stazione(&mainStation,distanza,numeri,elementi_inseriti,array_di_stazioni));

        }
        //----------------AGGIUNGI-AUTO---------
        if (stringa[0] == 'a' && stringa[9] == 'a') {
            int distanza_stazione;
            if(fscanf(file,"%d", &distanza_stazione)==1){}
            int autonomia_auto;
            if(fscanf(file,"%d", &autonomia_auto)==1){}
            printf("%s\n", aggiungi_auto(mainStation,distanza_stazione,autonomia_auto,array_di_stazioni));
        }

        //-------------ROTTAMA-AUTO------
        if (stringa[0] == 'r') {
            int distanza_stazione;
            int autonomia_auto;
            if(fscanf(file,"%d", &distanza_stazione)==1){}
            if(fscanf(file,"%d", &autonomia_auto)==1){}
            printf("%s\n", rottama_auto(&mainStation, distanza_stazione, autonomia_auto,array_di_stazioni));

        }
        //-------DEMOLISCI STAZIONE------
        if (stringa[0] == 'd') {
            int distanza_stazione;
            if(fscanf(file,"%d", &distanza_stazione)==1){}
            printf("%s\n", demolisci_stazione(&mainStation, distanza_stazione,array_di_stazioni));
        }

        //---------PIANIFICA PERCORSO--------
        if (stringa[0] == 'p') {
            int stazione_uno;
            int stazione_due;
            if(fscanf(file,"%d", &stazione_uno)==1){}
            if(fscanf(file,"%d", &stazione_due)==1){}
            if (stazione_uno == stazione_due) {
                printf("%d\n", stazione_due);
            } else {
                if(stazione_uno < stazione_due) {
                    struct list *lista_percorso = (struct list *) malloc(sizeof(struct list));

                    lista_percorso->stazione = -1;
                    lista_percorso->next = NULL;
                    lista_percorso->prev = NULL;
                    pianifica_percorso(mainStation, stazione_uno, stazione_due, &lista_percorso, NULL,array_di_stazioni);

                    if (lista_percorso->stazione != stazione_uno || lista_percorso->stazione == -1) {

                        printf("nessun percorso\n");
                    } else {

                        while (lista_percorso != NULL) {
                            printf("%d ", lista_percorso->stazione);
                            lista_percorso = lista_percorso->prev;
                            if(lista_percorso->prev==NULL){
                                printf("%d", lista_percorso->stazione);
                                break;
                            }
                        }
                        printf("\n");
                    }
                }else{
                    struct inverted_List *lista_percorso = (struct inverted_List*) malloc(sizeof(struct  inverted_List));
                    lista_percorso->stazione = NULL;
                    lista_percorso->next = NULL;
                    lista_percorso->prev = NULL;
                    pianifica_percorso(mainStation, stazione_uno, stazione_due, NULL,&lista_percorso, array_di_stazioni);

                    if(lista_percorso->stazione->distance!=stazione_due){
                        printf("nessun percorso\n");
                    }else{

                        struct Station* Possibile_nodo_Migliore= NULL;
                        lista_percorso=lista_percorso->prev;
                        while(lista_percorso->stazione->distance!= stazione_uno){
                            lista_percorso= lista_percorso->prev;
                            Possibile_nodo_Migliore= lista_percorso->next->stazione;
                            while(lista_percorso->stazione->distance - Possibile_nodo_Migliore->prev->distance <= lista_percorso->stazione->vehicles[0]){
                                Possibile_nodo_Migliore= Possibile_nodo_Migliore->prev;
                            }

                            while(Possibile_nodo_Migliore != lista_percorso->next->stazione ){
                                if(Possibile_nodo_Migliore->distance - lista_percorso->next->next->stazione->distance <= Possibile_nodo_Migliore->vehicles[0]){
                                    lista_percorso->next->stazione= Possibile_nodo_Migliore;
                                    break;
                                }
                                Possibile_nodo_Migliore=Possibile_nodo_Migliore->next;
                            }

                        }
                        while(lista_percorso!=NULL){
                            printf("%d ", lista_percorso->stazione->distance);
                            lista_percorso= lista_percorso->next;
                            if(lista_percorso->next== NULL){
                                printf("%d", lista_percorso->stazione->distance);
                                break;
                            }
                        }
                        printf("\n");
                    }

                }
            }
        }
    }
    fclose(file);
}



int main() {
    struct Station* mainStation= (struct Station*)malloc(sizeof(struct Station));
    mainStation->distance=-1;
    mainStation->next=NULL;
    mainStation->prev= NULL;
    //ARRAY DI STAZIONI
    struct Station* array_di_stazioni[20000];
    array_di_stazioni[0]=mainStation;
    mainFunction(mainStation, array_di_stazioni);
    return 0;
}