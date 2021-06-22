#include <NTL/ZZ.h>
#include <iostream>
#include "librerias.h"

using namespace std;
using namespace NTL;

class RSA
{
    private:
        ZZ d;
	    string alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ,.#";

    public:
        ZZ n;
        ZZ e;
        string mensaje;
        string mensaje_cifrado;
        string mensaje_decifrado;

        RSA()
        { }

        RSA(int _n, int _e, string _mensaje)
        {
            n = _n;
            e = _e;
            mensaje = _mensaje;
        }

        void GenerateKeys()
        {
            ZZ p_0 = ZZ(0), q_0 = ZZ(0);
            GK_searchPQ(p_0,q_0);
            //p_0 = 17;
            //q_0 = 43;

            //int n;
            n = GK_searchN(p_0,q_0);

            ZZ fhi = GK_searchfhi(p_0,q_0);

            //int e;
            //e = 101;
            e = GK_searchE(fhi);

            //int d;
            d = inversa(e,fhi);

            cout << "p: " << p_0 << endl;
            cout << "q: " << q_0 << endl;
            cout << "fhi: " << fhi << endl;
            cout << "---------------------------------------------------------------" << endl;
            cout << "clave publica e: " << e << endl;
            cout << "clave privada d:" << d << endl;
            cout << "n: " << n << endl;
            cout << "---------------------------------------------------------------"  << endl;
        }

        string EmisorEncryption()
        {
            cout << "--------------------- DATOS -----------------------------------" << endl;
            cout << "Tu mensaje es: " << mensaje << endl;
            cout << "clave publica e: " << e << endl;
            cout << "n: " << n << endl;
            cout << "---------------------------------------------------------------" << endl;

            string s_fin_temp;
            bool flag;
            do //para completar mas caracteres
            {
                ZZ verificadorAddCaracteres = mensaje.length() * NumberOfDigitsZZ( ZZ(alfabeto.length()));
                cout << "Numero de caracteres que hay en el mensaje: " << verificadorAddCaracteres << endl;

                if ( modulo(verificadorAddCaracteres, NumberOfDigitsZZ(n)-1) == 0 )
                {
                    cout << "Tu mensaje esta correcto" << endl;
                    flag = false;
                }
                else
                {
                    cout << "Aumentamos caracter #" << endl;
                    mensaje.append( "#" );
                    flag = true;
                }
            } while (flag);

            cout << "--------------------- DATOS -----------------------------------" << endl;
            cout << "Tu nuevo mensaje es: " << mensaje << endl;
            cout << " ----------------- Transformando a numeros --------------------" << endl;
            s_fin_temp = ObtainMessageInNumbers(mensaje,alfabeto); //mensaje en numeros completo
            cout << "Mensaje transformado en numeros "  << s_fin_temp << endl;
            cout << " --------------------------------------------------------------" << endl;

            ZZ i_fin_temp = NumberOfDigitsZZ(n)-1; //3
            ZZ i_fin_temp2 = ZZ(s_fin_temp.length()) / i_fin_temp; //18/3 = 6

            for (int i = 0; i < i_fin_temp2; ++i) //es 6 = mensaje_num.length() / k
            {
                //string aux = ObtainMessageInNumbers(mensaje,alfabeto); //cambiar
                //cout << "Mensaje en numeros: " << aux << endl;

                int k = NumberOfDigitsRint(n);
                //string s_num_of_matriz = SeparateIntoBlocks(i, k-1,aux); //021
                string s_num_of_matriz = SeparateIntoBlocks(i, k-1,s_fin_temp); //021
                cout << "Mensaje separado en bloques: " << s_num_of_matriz << endl;
                ZZ i_num_of_matriz = ZZ(atoi(s_num_of_matriz.c_str())); //convierto el numero a entero -> 21


                cout << "formula: (" << i_num_of_matriz << ")^" << e << " mod " << n << endl;
                ZZ i_aux;
                //i_aux = ModularArithmetic(i_num_of_matriz,e,n); //Potenciacion modular con el numero -> 234
                //i_aux = binpow(i_num_of_matriz,e,n);
                //i_aux = powerZZ(i_num_of_matriz,e,n);
                i_aux = k_ary(i_num_of_matriz,e,n);
                cout << "Resultado de la exponeciacion: " << i_aux << endl;

                // ------------ hacer que sea de tamaño N ------------------
                ZZ total_number_digits = NumberOfDigitsZZ(n); //4
                int aux_int = ZZ_to_int(i_aux);
                cout << "pos en entero: " << aux_int << endl;
                cout << "numerod de digitos de n: " << total_number_digits << endl;
                mensaje_cifrado = IncreaseZeros(aux_int, ZZ_to_int(total_number_digits),mensaje_cifrado);
                cout << "Mensaje cifrado es: " << mensaje_cifrado << endl;
                cout << endl;
            }

            cout << "El mensaje cifrado final es: " << mensaje_cifrado << endl;

            return mensaje;
        }

        string ReceptorDecoded() //n= p * q
        {
            ZZ p,q;
            //RE_searchP(n,p,q);
            //p = 383;
            //q = 421;

            cout << "Ingrese la clave p: " << endl; cin >> p;
            cout << "Ingrese la clave q: " << endl; cin >> q;

            ZZ fhi;
            fhi  = GK_searchfhi(p,q);

            //private_key;
            d = inversa(e,fhi);

            cout << "--------------------- DATOS -----------------------------------" << endl;
            cout << "Tu mensaje es: " << mensaje << endl;
            cout << "p: " << p << endl;
            cout << "q: " << q << endl;
            cout << "fhi: " << fhi << endl;
            cout << "private_key: " << d << endl;
            cout << "n: " << n << endl;
            cout << "---------------------------------------------------------------" << endl;

            ZZ i_fin_temp = NumberOfDigitsZZ(n); //4
            ZZ i_fin_temp2 = ZZ(mensaje.length()) / i_fin_temp; //6 bloques

            string mensaje_decifrado_num;
            for (int i = 0; i < i_fin_temp2; ++i) //es 6 = mensaje_num.length() / k
            {
                //string s_num_matriz = SeparateIntoBlocks(i, ZZ_to_int(NumberOfDigitsZZ(n)) ,mensaje); //0234
                string s_num_matriz = SeparateIntoBlocks(i, ZZ_to_int(i_fin_temp) ,mensaje); //0234

                ZZ i_num_matriz = String_To_ZZ(s_num_matriz); //234

                cout << "formula: (" << i_num_matriz << ")^" << d << " mod " << n << endl;

                ZZ i_aux;
                //i_aux = ModularArithmetic(i_num_matriz,d,n); //21
                //i_aux = binpow(i_num_matriz,d,n); //21
                //i_aux = powerZZ(i_num_matriz,d,n); //21
                i_aux = k_ary(i_num_matriz,d,n); //21
                int i_aux2 = ZZ_to_int(i_aux);

                int total_number_digits = NumberOfDigitsRint(n)-1; //3
                mensaje_decifrado_num = IncreaseZeros(i_aux2,total_number_digits,mensaje_decifrado_num); //021412...
                cout << "Mensaje desifrado incompleto: " << mensaje_decifrado_num << endl;
            }

            cout << "El mensaje descifrado en numeros es: " << mensaje_decifrado_num << endl;

            ZZ i_fin_temp3 = ZZ(NumberOfDigits(alfabeto.length())); //2
            ZZ i_fin_temp4 = ZZ(mensaje_decifrado_num.length()) / i_fin_temp3; // 18/2 = 9 bloques
            for (int i = 0; i < i_fin_temp4; ++i) //es 9 = mensaje_num.length() / 2
            {
                string s_num_matriz = SeparateIntoBlocks(i, NumberOfDigits(alfabeto.length()),mensaje_decifrado_num); //02
                cout << s_num_matriz << endl;

                int i_num_matriz = atoi(s_num_matriz.c_str());
                string aux = ObtainMessageInLetters(i_num_matriz,alfabeto);
                mensaje_decifrado.append(aux);
            }

            cout << "El mensaje descifrado es: " << mensaje_decifrado << endl;

            return mensaje;
        }
};

int main()
{
    string mensaje;
    int respuesta;
    int e, n;

    cout << "----------- Progrmama de criptografia - RSA -------------------" << endl;
    cout << "Que deseas hacer: " << endl;
    cout << "Cifrar un mensaje (Emisor) .................................. 1" << endl;
    cout << "Descifrar un mensaje (Receptor) ............................. 2" << endl;
    cout << "Crear claves publicas/privadas (Receptor).................... 3" << endl;
    cout << "Respuesta:  ";
    cin >> respuesta;

    if(respuesta == 1) //emisor
    {
        cout << "Ingrese el mensaje que desea cifrar: ";
        cin.ignore();
        //cin >> mensaje;
        getline(cin,mensaje);
        cout << "-------------------------------------------------------------" << endl;
        cout << "Ingrese la clave publica e: " << endl;
        cin >> e;
        cout << "Ingrese la clave publica n: " << endl;
        cin >> n;

        RSA emisor(n,e,mensaje);
        emisor.EmisorEncryption();
        cout << "-------------------------------------------------------------" << endl;
    }
    else if (respuesta == 2) //receptor
    {
        cout << "Ingrese el mensaje que desea descifrar: ";
        cin.ignore();
        //cin >> mensaje;
        getline(cin,mensaje);
        cout << "-------------------------------------------------------------" << endl;
        cout << "Ingrese la clave publica e: " << endl;
        cin >> e;
        cout << "Ingrese la clave publica n: " << endl;
        cin >> n;

        RSA receptor(n,e,mensaje);
        receptor.ReceptorDecoded();
        cout << "-------------------------------------------------------------" << endl;
    }

    else if (respuesta == 3) //generar claves
    {
        cout << "-------------- Generando claves publcas ---------------- " << endl;
        RSA public_key;
        public_key.GenerateKeys();
    }
}
