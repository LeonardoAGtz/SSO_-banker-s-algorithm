#include <iostream>
#include "windows.h"

using namespace std;

int mA_necesidad[10][10]; //Matriz de necesidad
int mB_asignacion[10][10]; //Matriz de asignacion
int mA_mB[10][10]; //Resta de las matrices
int v_recurso[10]; //Vector de recursos
int v_disponibilidad[10]; //Vector de disponibiliad
int v_finalizado[10]; //1 = SI, 0 =NO
int v_activo[10];
int nproc; //numero de procesos
int nrec; //numero de recursos.
int proceso_activo; //numero del proceso que se encuentra en actual ejecución.

bool data_ok = true; //me dirá si los datos automaticos son cnsistentes a la regla
bool seguro = true; //me dirá si se puede seguir o si hay una que no, y por ende esta en estado no seguro.

void generar_data_auto(); //Genera los datos de forma automatica
void generar_data_manual(); //Se ingresan los datos de forma manual.
void print_all(); //Imprime en formato las matrices y los vectores.
void banquero(); //Algoritmo principal
bool restantes(); //Retorna true si al menos un proceso no ha finalizado

int main(){

    ///INFORMACIÓN DEL ALUMNO Y PROGRAMA
    cout << "LEONARDO ARANDA GUTIERREZ\n217761528" << endl;
    cout << "CENTRO UNIVERSITARIO DE CIENCIAS EXACTAS E INGENIERIAS" << endl;
    cout << "SEMINARIO DE SISTEMAS OPERATIVOS" << endl;
    cout << "ALGORITMO DEL BANQUERO" << endl;
    cout << "------------------------------------------------------" << endl;

    cout << "Ingrese el numero de procesos (MAX 10): ";
    cin >> nproc;
    cout << "Ingrese el numero de recursos (MAX 10): ";
    cin >> nrec;
    getchar();


    while (true) {
        string op;
        system("cls");
        cout << "Generar datos manual o automatico?" << endl;
        cout << "1) Manual" << endl;
        cout << "2) Automatico" << endl;
        cout << "Opcion: ";
        getline(cin, op);

        if (op == "1") {
            generar_data_manual();
            if (data_ok == false) {
                cout << "Los datos ingresados no son consistentes" << endl;
            }else if(data_ok == true){
                break;
            }
        }else if(op == "2"){
            generar_data_auto();
            break;
        }else{
            cout << "Ingrese una opcion valida" << endl;
        }
    }

    system("cls");

    print_all();
    Sleep(1500);

    banquero();
    system("pause");

    return 0;
}

void banquero(){
    bool aplica = true;

    while (restantes()) { //SI QUEDAN RESTANTES
        for (int i=0;i<nproc;++i) { //Me muevo en los procesos
            aplica = true;

            if (v_finalizado[i] == 0) {//SI EL PROCESO NO HA FINALIZADO
                 SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),6);
                cout << endl << "Validando disponibilidad..." << endl << endl;
                 SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
                for (int j=0;j<nrec;++j) { //Me muevo en los recursos
                    v_activo[i] = 1;

                    if (v_disponibilidad[j] < mA_mB[i][j]) { // si el vector de disposibilidad es menor que el vector de la resta de matrices, se desacarta aplicacion en la columna.
                        aplica = false;
                    }
                }
                if (aplica == false) {
                    //no se puede despachar ahora
                     SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),4);
                    cout << endl << "El proceso no puede ser despachado por el momento..." << endl << endl;
                     SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
                      v_activo[i] = 0;
                }else if (aplica == true) {
                    //algoritmo de cambio
                     SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),3);
                    cout << endl << "Ejecutando proceso..." << endl << endl;
                     SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
                    for (int x=0;x<nrec;++x) {
                        //PASO 1
                        if (mA_mB[i][x] > 0) { //si la posicion del vector es mayor que 0, es decir, necesita
////                            int prestamo = mB_asignacion[i][x] - mA_mB[i][x];
////                            mA_mB[i][x] += prestamo; //se le suma la cantidad que tiene el vector de disponibilidad en la posicion
////                            v_disponibilidad[x] -= prestamo; //se resta lo otorgado

                            mB_asignacion[i][x] += mA_mB[i][x];
                            v_disponibilidad[x] -= mA_mB[i][x];
                            mA_mB[i][x] = 0;
                        }
                    }
                    Sleep(1500);
                    print_all();
//                    //Paso 2
//                    for (int j = 0; j < nrec;++j) {
//                        mB_asignacion[i][j] += mA_mB[i][j]; //Agregao los valres vector de la resta de matrices a la matris de asigancion
//                        mA_mB[i][j] = 0; ///Eliminao los valores vector de la matriz resta
//                    }
                    //Paso 3
                     SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),13);
                    cout << endl << "Liberando recursos..." << endl << endl;
                     SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
                    for (int j=0;j <nrec;++j) {
                        mA_necesidad[i][j] -= mB_asignacion[i][j]; //Resto los valores vector de la matriz a y b
                        v_disponibilidad[j] += mB_asignacion[i][j]; //recupero el valor de disposibilidad
                        mB_asignacion[i][j] = 0; //elimino los valores de la tabla de asigancon.
                    }
                    v_finalizado[i] = 1;
                    v_activo[i] = 0;
                    Sleep(1500);
                    print_all();
                }
            }
        }
        bool flag = false;

        for (int i=0;i<nproc;++i) {
            if (v_finalizado[i] == 1) {
                flag = true;
            }
        }

        if (flag == false) {
            seguro = false;

        }

        if (seguro == false){
             SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),4);
            cout<< "La condicion es no segura, no se puede ejecutar el caso" << endl;
             SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
            break;
        }
    }
    if (seguro == true) {
        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),2);
       cout<< "El proceso termino con exito, estado seguro" << endl;
        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
    }

}

bool restantes(){
    bool flag = false;

    for (int i = 0; i < nproc;++i) {
        if (v_finalizado[i] == 0) {
            flag = true;
        }
    }

    return flag;
}

void generar_data_auto(){
    //GENERACIÓN DE VALORES RANDOM
    //GENERANDO VALORES A MATRIZ DE NECESIDAD (A)
    for (int i=0;i<nproc;++i) {
        for (int j=0;j<nrec;++j) {
            int dat = rand()%9;
            mA_necesidad[i][j] = dat;
        }
    }
    //GENERANDO VALORES A MATRIZ DE ASIGNACIÓN (B)
    for (int i=0;i<nproc;++i) {
        for (int j=0;j<nrec;++j) {
            while (true) {
                int dat = rand()%4;
                if (dat <= mA_necesidad[i][j]) {
                    mB_asignacion[i][j] = dat;
                    break;
                }
            }
        }
    }
    //GENERANDO VALORES A VECTOR DE DISPONIBILIDAD (D)
    for (int i=0;i<nrec;++i) {
        int dat = rand()%3;
        v_disponibilidad[i] = dat;
    }


    for (int j=0;j<nrec;++j) {
        int value=0;
        for (int i=0;i<nproc;++i) {
            value += mB_asignacion[i][j];
        }
        value += v_disponibilidad[j];
        v_recurso[j] = value;
    }
    //GENERAR RESTA DE MATRIZ (A-B)
    for (int i=0;i<nproc;++i) {
        for (int j=0;j<nrec;++j) {
            int dat = mA_necesidad[i][j] - mB_asignacion[i][j];
            mA_mB[i][j] = dat;
        }
    }

    for (int i=0;i<nproc;++i) {
        v_finalizado[i] = 0;
    }

    for (int i=0;i<nproc;++i) {
        v_activo[i] = 0;
    }
}

void generar_data_manual(){

    //VALORES A MATRIZ DE NECESIDAD (A)
    cout << endl << endl << "Ingrese valores a la matriz de necesidad..." << endl;
    int dat = 0;
    for (int i=0;i<nproc;++i) {
        for (int j=0;j<nrec;++j) {
            cout << "Ingrese valor para el proceso [" << i+1 << "], recurso [" << j+1 << "]: ";
            cin >> dat;
            cout << endl;
            mA_necesidad[i][j] = dat;
        }
    }

    //VALORES VECTOR RECURSOS
    cout << endl << endl << "Ingrese valores al vector de recursos" << endl;
    for (int j=0;j<nrec;++j) {
        cout << "Ingrese valor para el recurso [" << j+1 << "]: ";
        cin >> dat;
        cout << endl;
        v_recurso[j] = dat;
    }

    //VALORES A MATRIZ DE ASIGNACIÓN (B)
    cout << endl << endl << "Ingrese valores a la matriz de asignacion" << endl;
    for (int i=0;i<nproc;++i) {
        for (int j=0;j<nrec;++j) {
            cout << "Ingrese valor para el proceso [" << i+1 << "], recurso [" << j+1 << "]: ";
            cin >> dat;
            cout << endl;
            mB_asignacion[i][j] = dat;
        }
    }

    //GENERANDO VALORES A VECTOR DE DISPONIBILIDAD (D)
    for (int i=0;i<nrec;++i) {
        dat = v_recurso[i];
        for (int j=0;j<nproc;++j) {
            dat = dat - mB_asignacion[j][i];
        }
        if (dat < 0) {
            data_ok = false;
        }
        v_disponibilidad[i] = dat;
    }

    //GENERAR RESTA DE MATRIZ (A-B)
    for (int i=0;i<nproc;++i) {
        for (int j=0;j<nrec;++j) {
            int dat = mA_necesidad[i][j] - mB_asignacion[i][j];
            mA_mB[i][j] = dat;
        }
    }

    for (int i=0;i<nproc;++i) {
        v_finalizado[i] = 0;
    }

    for (int i=0;i<nproc;++i) {
        v_activo[i] = 0;
    }
}

void print_all(){
    //impresiones de pantalla
    cout << endl << endl << endl;


    SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),5);
    cout << "N   ";
    for (int i=0;i<nrec;++i) {
        cout << "R" << i+1 << "  ";
    }
    cout << "FIN?";
    cout << "\t";

    cout << "A   ";
    for (int i=0;i<nrec;++i) {
        cout << "R" << i+1 << "  ";
    }
    cout << "\t";

    cout << "R   ";
    for (int i=0;i<nrec;++i) {
        cout << "R" << i+1 << "  ";
    }
    cout << endl;
    SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);

    for (int i=0;i<nproc;++i) {
        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),5);
        cout << "P" << i+1 << "  ";
        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
        for (int j=0;j<nrec;++j) {
            SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),2);
            cout << mA_necesidad[i][j] << "   ";
            SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
        }
        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),5);
        if (v_finalizado[i] == 1) {
            cout << "S";
        }else{
            cout << "N";
        }
        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
        cout << "   \t";

        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),5);
        cout << "P" << i+1 << "  ";
        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
        for (int j=0;j<nrec;++j) {
            SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),2);
            cout << mB_asignacion[i][j] << "   ";
            SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
        }
        cout << "\t";

        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),5);
        cout << "P" << i+1 << "  ";
        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
        for (int j=0;j<nrec;++j) {
            SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),2);
            cout << mA_mB[i][j] << "   ";
            SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
        }
        if (v_activo[i] == 1) {
            SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),1);
            cout << "*";
            SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
        }else{
            cout << " ";
        }
        cout << endl << endl;
    }
    
    /*    cout << "A  R1  R2  R3  R4  FIN    MATRIZ A NECESIDAD" << endl;
    for (int i=0;i<nproc;++i) {
        cout << "P" << i+1 << " ";
        for (int j=0;j<nrec;++j) {
            cout << mA_necesidad[i][j];
            cout << "   ";
        }
        cout << "N";
        cout << endl;
    }

    cout << endl;

    cout << "B  R1  R2  R3  R4  FIN   MATRIZ B ASIGNACION" << endl;
    for (int i=0;i<nproc;++i) {
        cout << "P" << i+1 << " ";
        for (int j=0;j<nrec;++j) {
            cout << mB_asignacion[i][j];
            cout << "   ";
        }
        cout << "N";
        cout << endl;
    }

    cout << endl;

    cout << "C  R1  R2  R3  R4  MATRIZ A-B" << endl;
    for (int i=0;i<nproc;++i) {
        cout << "P" << i+1 << " ";
        for (int j=0;j<nrec;++j) {
            cout << mA_mB[i][j];
            cout << "   ";
        }
        cout << "N";
        cout << endl;
    }

    cout << "R1  R2  R3    VECTOR RECURSO" << endl;
    for (int i=0;i<nrec;++i) {
        cout << v_recurso[i];
    }
    cout << endl;

    cout << "R1  R2  R3    VECTOR DISPONIBILIDAD" << endl;
    for (int i=0;i<nrec;++i) {
        cout << v_disponibilidad[i];
    }
    cout << endl;*/
    cout << "    ";
    SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),5);
    for (int i=0;i<nrec;++i) {
        cout << "R" << i+1 <<"  ";
    }
    cout << "\t\t    ";
    for (int i=0;i<nrec;++i) {
        cout << "R" << i+1 <<"  ";
    }SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
    cout << endl << "    ";
    for (int i=0;i<nrec;++i) {
        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),2);
        cout << v_recurso[i] << "   ";
        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
    }
    cout << "\t\t    ";
    for (int i=0;i<nrec;++i) {
        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),2);
        cout << v_disponibilidad[i] << "   ";
        SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),7);
    }

    cout << endl << endl << endl;

}
