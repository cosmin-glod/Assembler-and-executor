
# ASAMBLOR ȘI EXECUTOR ARHITECTURA RISC
  Proiect pentru Arhitectura Sistemelor de Calcul (Arhitectura Calculatoarelor), Facultatea de Matematică și Informatică, Secția Informatică, Universiatea din București, anul 2023-2024.
  Proiect realizat de:



## Echipa Ultima săptămână, seria 15, grupa 151

- [@Andu9](https://github.com/Andu9) Niță Alexandru
- [@cosmin371](https://github.com/cosmin371) Glod Cosmin-Stefan
- [@danielbirsan](https://github.com/danielbirsan) Birsan Gheorghe Daniel

## Despre
-Asamblorul codifică instrucțiunile și le scrie într-un fisier binar, urmand sa fie executate de catre executor. In acest repo se gasesc codificarile facute pentru instructiuni, registri si functiile din C++.


##  Instalare si requirements

- Compilator C++: (g++ 11.4).
- Editor de text sau IDE: (Code::Blocks, Visual Studio Code).
- Sistem de operare: linux/windows

+Binary viewer (ex: https://binary-viewer.en.softonic.com/download)


## Testare

Pentru testare s-au folosit următoarele fișiere sursă:

https://marz.utk.edu/my-courses/cosc230/book/example-risc-v-assembly-programs/


## Rularea codului

<h3>Linux</h3>

<h4>Asamblor:</h4> 

- se descarcă codul sursă
- se rulează următoarea comandă în terminal în folderul cu sursa
 ```bash
 g++ asamblor.cpp -o asamblor
 
 ```
- în fișierul <b>"input.in"</b> se pune programul in assembly risc-v ce urmează a fi asamblat
- se rulează comanda
 ```bash
./asamblor
 
 ```
- in fisierul <b> "executabil.bin"</b> se află binarul din urma executării


<h4>Executor: </h4>

- se descarcă codul sursă
- se rulează comanda în terminal în folderul cu sursa

 ```bash
g++ executor.cpp -o executor
 
 ```

- în fișierul <b>"nume_fisier.in"</b> trebuie să se afle starea inițială a regiștrilor sub forma
  <p><b> nume_registru tipul_de_date valoare </b>  separate prin câte un spațiu </p>
- <b>string</b> pentru <b>const char *</b>sau <b>char *</b>
- <b>int</b> pentru <b>unsigned long, int </b>
- <b>arrayInt </b> pentru <b> int arr[], const int arr[] </b>
- <b>arrayLong </b> pentru <b>vectori de long</b>
- <b> Coordinate& </b> pentru <b>struct Coordinate </b>
- <b>float </b> pentru <b> float </b>
- <b>float& </b> pentru <b> float& </b>
- <b> LL* </b> pentru <b> LL* </b>
- <b>arrayFloat </b> pentru <b> float a[], float matrice[][] </b>

<h3>Exemplu</h3>

 ```bash
a0 string abcd
a1 int 78
fa0 float -5.6
a1 float& 2.3 1.2
a0 arrayInt 5 3 -4 1
a1 arrayLong 10 4 -5 -3 0
a2 arrayFloat 1.2 5.2 -80.4
a2 arrayFloat& 3.4
a1 arrayDouble 6.4
a0 Coordinate& 3.5 -4.7
a2 LL* 1 2 3 [lista unde pozișia de start este prima valoare]
a1 LL* 5 [elementul ce este adăugat in lista]
 
 ```


- se rulează comanda următoare în terminalul cu sursa
 ```bash
./executor
 
 ```
- dupa executare în fișierul <b>"nume_fisier.out"</b> se va află starea finală a regiștrilor

-se deschide fișierul folosit comanda:
 ```bash
cat nume_fisier.out
 
 ```
-se afișează pe ecran starea registrilor folosiți în program, cu exceptia regiștrilor sb, ra si zero (pentru registrii în care o adresa, vom afișa conținutul din memorie)


## Cerința Proiectului
 Crearea unui standard de cod mașină și apoi un executor pentru fișierele binare generate
- https://cs.unibuc.ro/~crusu/asc/Arhitectura%20Sistemelor%20de%20Calcul%20(ASC)%20-%20Proiect%200x00%202023.pdf

## Bibliografie - Linkuri utile
- [Detalii despre proiect](https://cs.unibuc.ro/~crusu/asc/Arhitectura%20Sistemelor%20de%20Calcul%20(ASC)%20-%20Proiect%200x00%202023.pdf)
- [Cele 12 programe model](https://marz.utk.edu/my-courses/cosc230/book/example-risc-v-assembly-programs/)
- [Documentatia de assembly risc-v](https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf)
- [Detalii despre registrii risc-v](https://en.wikichip.org/wiki/risc-v/registers)
- [Cum scriem/citim un fisier binar?](https://medium.com/@zekumoru/how-to-work-with-binary-files-in-c-520a852ee04a)


## Cum scriem/citim un fisier binar ?
https://medium.com/@zekumoru/how-to-work-with-binary-files-in-c-520a852ee04a#:~:text=Binary%20Operations,binary%20%7C%20ios%3A%3A%20trunc)%3B

## License

[MIT](https://choosealicense.com/licenses/mit/)

