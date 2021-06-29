#include <NTL/ZZ.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;
using namespace NTL;

ZZ modulo(ZZ a, ZZ b)
{
    ZZ r = a - ((a/b)*b);
    if (r < 0)
    {
        return r += b;
    }
    return r;
}

ZZ pow2(ZZ a,ZZ b)
{
    ZZ res;
    ZZ n;
    n=0;
    res =1;
    while (n != b){
        res *= a;
        n++;
    }
    return res;
}

ZZ k_ary(ZZ base, ZZ exp, ZZ N)
{
    ZZ t = ZZ(1);
    while (exp > ZZ(0))
    {
        if (modulo(exp, ZZ(2)) != ZZ(0))
            t = modulo((t * base) ,N);

        base = modulo((base * base), N);
        exp /= ZZ(2);
    }
    return modulo(t, N);
}

ZZ euclides(ZZ num1, ZZ num2)
{
    ZZ q, r = ZZ(1);
    if (num1 < num2)
    {
        ZZ temp = num1;
        num1 = num2;
        num2 = temp;
    }
    while (r != 0)
    {
        //num1 = (q*num2) + r
        q = num1/num2;
        r = num1 - (q*num2);

        if (r != 0) // para que en la ultima operacion no haga el cambio y obtenga el num2
        {
            num1 = num2;
            num2 = r;
        }
    }
    ZZ mcd = num2;
    return mcd;
}

ZZ euclides_extendido(ZZ num_1, ZZ num_2)
{
    ZZ q, r = ZZ(1);
    ZZ r1, r2, s1, s2, t1, t2, s, t;

    r1 = num_1; r2 = num_2;
    s1 = 1; s2 = 0;
    t1 = 0; t2 = 1;
    // ---------------------------------------
    while (r != 0) // r != 0
    {
        //num1 = (q*num2) + r
        q = r1/r2;
        r = r1 - (q*r2);

        if (r != 0) // para que en la ultima operacion no haga el cambio y obtenga el num2
        {
            r1 = r2;
            r2 = r;
        }

        s = s1 - q * s2;
        s1 = s2;
        s2 = s;

        t = t1 - q * t2;
        t1 = t2;
        t2 = t;
        //inversa es s1 y mcd es r2;
    }

    /*cout << " -------- Algoritmo de Euclides ----------" << endl;
    cout << "El MCD es: " << r2 << endl;

    cout << " -------- Algoritmo de Euclides Extendido ----------"  << endl;
    cout << "x = " << s1 << endl;
    cout << "y = " << t1 << endl;
    cout << num_1 << "(" << s1 << ") + " << num_2 << "(" << t1 << ") = " << r2 << endl;
    cout << "La inversa es: " << s1 << endl;*/

    return s1;
}

ZZ inversa (ZZ clave_a, ZZ longitud)
{
    clave_a = euclides_extendido(clave_a,longitud);
    //cout << "clave a sin modulo: " << clave_a << endl;
    if (clave_a < 0)
    {
        clave_a = modulo(clave_a,longitud);
    }
    //cout << "clave a con modulo: " << clave_a << endl;
    return clave_a;
}

bool PrimeNumber(ZZ n) //verificador de primalidad (se puede cambiar)
{
    ZZ m = n/2;
    for (int i = 2; i <= m ; i++)
    {
        if (n % i == 0)
        {
            return true; //no es primo
        }
    }
    return false; // es primo
}

bool Miller(ZZ p,int iteration)
{
    if (p < 2)
    {
        return false;
    }

    if (p != 2 && modulo(p, ZZ(2)) == 0)
    {
        return false;
    }

    ZZ s = p - 1;
    while ( modulo(s, ZZ(2)) == 0)
    {
        s = s / 2;
    }

    for (int i = 0; i < iteration; i++)
    {
        ZZ a = modulo ( ZZ(rand()), (p - 1) + 1), temp = s;
        ZZ mod = k_ary(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = k_ary(mod, mod, p);
            temp = temp * 2;
        }
        if (mod != p - 1 && modulo(temp, ZZ(2)) == 0)
        {
            return true; // no es primo
        }
    }
    return false; //es primo
}

bool isPrime(ZZ p) {

    // generate the number
    ZZ checkNumber = pow2(ZZ(2), p) - ZZ(1);

    // First number of the series
    ZZ nextval = ZZ(4) % checkNumber;

    // Generate the rest (p-2) terms
    // of the series.
    for (ZZ i = ZZ(1); i < p - 1; i++)
        nextval = (nextval * nextval - 2) % checkNumber;

    // now if the (p-1)th term is 0 return true
    // else false.
    if (nextval == ZZ(0))
    {
        //cout << "Es primo"<<endl;
        return false;
    }
    else
    {
        //cout << "No es primo" << endl;
        return true;
    }
}

void GK_searchPQ(ZZ &p, ZZ &q)
{
    do
    {
        do
        {
            p = rand()%100000;
            //p = rand();
        //} while ( isPrime(p) ); //true = no sea primo     false = es primo
        //} while ( PrimeNumber(p) ); //true = no sea primo     false = es primo
        } while ( Miller(p,5) ); //true = no sea primo     false = es primo

        do
        {
            q = rand()%100000;
            //q = rand();
        //} while ( isPrime(q) ); //true = no sea primo     false = es primo
        //} while ( PrimeNumber(q) ); //true = no sea primo     false = es primo
        } while ( Miller(q,5) ); //true = no sea primo     false = es primo

    } while(euclides(p,q) != 1);
}

ZZ GK_searchN(ZZ p, ZZ q)
{
    return  p*q;
}

ZZ GK_searchfhi(ZZ p, ZZ q)
{
    ZZ fhi = (p-1)*(q-1);
    return fhi;
}

ZZ GK_searchE(ZZ fhi)
{
    ZZ e;
    do
    {
        e = rand()%1000;
        //e = rand();
    }while(euclides(e,fhi) != 1);
    return e;
}

void RE_searchP(ZZ n, ZZ &p, ZZ &q)
{
    do
    {
        for (ZZ i = ZZ(2); i < n; ++i)
        {
            for (ZZ j = ZZ(2); j < n; ++j)
            {
                if ( i*j == n && (!PrimeNumber(i)) && (!PrimeNumber(j))) //true = compuesto     false = primo
                //if (i*j == n && (!isPrime(i)) && (!isPrime(j)) ) //true = compuesto     false = primo
                {
                    p = i;
                    q = j;
                    break;
                }
            }
        }
        //cout << "no se encontro p y q" << endl;
    }while(euclides(p,q) != 1);
}

ZZ ModularArithmetic(ZZ mensaje, ZZ n, ZZ total) //se puede remplazar
{
    ZZ x = ZZ(1), nmod2, elevado2, resmodn, n_2 = ZZ(1);

    while (n_2 != 0)
    {
        cout << "------------------------------------------" << endl;
        cout << "x = " << x << endl;
        cout << "n = " << n << endl;
        nmod2 = modulo(n, ZZ(2));

        if(x == 1 && nmod2 == 1)
        {
            elevado2 = mensaje;
            resmodn = mensaje;

            cout << "n mod 2 -> " << n << " mod 2 = "  << nmod2 << endl;
            cout << "^2 = " << elevado2 << endl;
            cout << "res mod n = " << resmodn << endl;
            cout << "------------------------------------------" << endl;

            n_2 = n/2;
            n = n_2;
            x = x + 1;
        }

        else if(nmod2 == 0)
        {
            ZZ temp = elevado2;
            ZZ aux = pow2(elevado2, ZZ(2)); //ZZ aux = pow(elevado2, ZZ(2));
            elevado2 = modulo( aux, total);

            cout << "n mod 2  -> " << n << " mod 2 = " << nmod2 << endl;
            cout << "^2 -> " << temp << "^2 mod " << total << " = " << elevado2 << endl;
            cout << "------------------------------------------" << endl;

            n_2 = n/2;
            n = n_2;
            x = x + 1;
        }
        else if (nmod2 == 1)
        {
            ZZ temp = elevado2;
            ZZ aux = pow2(elevado2, ZZ(2));
            elevado2 = modulo( aux, total);

            ZZ temp2 = resmodn;
            ZZ aux2 = resmodn*elevado2;
            resmodn = modulo(aux2,total);

            cout << "n mod 2  -> " << n << " mod 2 = " << nmod2 << endl;
            cout << "^2 -> " << temp << "^2 mod " << total << " = " << elevado2 << endl;
            cout << "res mod total -> " << temp2 << "*" << elevado2 << " mod " << total << " : " << resmodn << endl;
            cout << "------------------------------------------" << endl;

            n_2 = n/2;
            n = n_2;
            x = x + 1;
        }
    }
    mensaje = resmodn;
    return mensaje;
}

int NumberOfDigits(int num)
{
    int contador = 1;
    while(num/10>0)
    {
        num=num/10;
        contador++;
    }
    return contador;
}

ZZ NumberOfDigitsZZ(ZZ num)
{
    ZZ contador = ZZ(1);
    while(num/10>0)
    {
        num=num/10;
        contador++;
    }
    return contador;
}

int NumberOfDigitsRint(ZZ num){
    stringstream buffer;
    buffer << num;
    string number = buffer.str();
    return number.length();
}

int ZZ_to_int(ZZ num)
{
    stringstream buffer;
    buffer << num;
    string number = buffer.str();
    return atoi(number.c_str());
}

ZZ String_To_ZZ(string str)
{
    ZZ result(NTL::INIT_VAL, str.c_str());
    return result;
}

string IncreaseZeros(int pos, int total_number_digits, string mensaje_num)
{
    int contador = NumberOfDigits(pos); //2
    string num_alf = to_string(pos); // 3(stirng)

    while ( contador < total_number_digits) //2 < 2
    {
        mensaje_num.append("0");
        contador++;
    }
    mensaje_num.append(num_alf);//14
    cout << "funcion increaseZero: " << mensaje_num << endl ;
    return mensaje_num;
}

string ObtainMessageInNumbers(string mensaje, string alfabeto)
{
    string mensaje_num;
    int total_number_digits = NumberOfDigits( alfabeto.length() ); //2

    for (int i = 0; i < mensaje.length(); i++)
    {
        for (int pos = 0; pos < alfabeto.length(); pos++)
        {
            if ( mensaje[i] == alfabeto[pos]) //c = 14
            {
                mensaje_num = IncreaseZeros(pos,total_number_digits,mensaje_num); //0214
                break;
            }
        }
    }
    return mensaje_num;
}

string SeparateIntoBlocks( int indice, int k, string mensaje_num) //0 - 3 - mensaje
{
    string matriz[1000];
    int inicio = 0;
    for (int i = 0; i < (mensaje_num.length() / k); i++) // 0 - 6
    {
        matriz[i] = mensaje_num.substr(inicio,k);
        inicio = inicio + k; //0+3 = 3
    }

    return matriz[indice];
}

string ObtainMessageInLetters( int pos, string alfabeto)
{
    string mensaje_num;

    for (int i = 0; i < alfabeto.length(); i++) // 0 - 25
    {
        if ( pos == i) // 2 == 2
        {
            mensaje_num = alfabeto[i];
            //cout << "pos: " << pos << endl;
            //cout << "i: " << i << endl;
            break;
        }
    }
    return mensaje_num;
}
